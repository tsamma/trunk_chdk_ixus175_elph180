/*===================================================================================================
    usb_remote.c

        -   called every 10 mSec from kbd.c
        -   when enabled via CHDK menu, monitors the state of USB power and takes action on a change
        -   consists of :
                -   a user selectable driver module that watches the USB power state
                    and sets the state of a "virtual" remote switch
                -   a user selectable control module that translates the state of the
                    "virtual" remote switch to KEY_PRESS actions
                -   [FUTURE] extends the script command get_usb_power() to return the current state of
                    the "virtual" remote switch, allowing scripts to easily work with all switch types
                -   [FUTURE]  supplies a framework for adding support to read PWM devices (e.g. gentles) and
                    to perform control actions based on those signals (at some point somebody might even use
                    this to build a Arduino-type remote control keypad
  ===================================================================================================*/

#include "camera_info.h"
#include "stdlib.h"
#include "clock.h"
#include "modes.h"
#include "shooting.h"
#include "conf.h"
#include "action_stack.h"
#include "gui_draw.h"
#include "usb_remote.h"


/*===================================================================================================
    Variables
  ===================================================================================================*/
int usb_sync_wait_flag=0 ;
int usb_remote_active=0 ;
int bracketing_timeout = 0 ;
int usb_power=0;
static int usb_count=0;
static int logic_module_usb_count = 0 ;
int remote_mark_count, remote_space_count;

#define USB_BUFFER_SIZE 16
static int usb_buffer[USB_BUFFER_SIZE] ;
static int * usb_buffer_in = usb_buffer ;
static int * usb_buffer_out = usb_buffer ;


enum SWITCH_TYPE            switch_type = SW_NONE ;
enum CONTROL_MODULE         control_module = RMT_NONE ;
enum VIRTUAL_REMOTE_STATE   virtual_remote_state = REMOTE_RESET ;
enum DRIVER_STATE           driver_state = SW_RESET;
enum LOGIC_MODULE_STATE     logic_module_state = LM_RESET;
enum USB_POWER_STATE        usb_state = USB_POWER_OFF ;
enum CAMERA_MODE            camera_mode = CAMERA_MODE_PLAYBACK ;

/*===================================================================================================
    Debug and error handling
  ===================================================================================================*/


 #ifdef USB_REMOTE_DEBUGGING

#define NUMBER_OF_ERRORS 21

int debug_errors[NUMBER_OF_ERRORS] ;

void debug_error(int err_num)
{
    if ( (err_num > 0) && (err_num <= NUMBER_OF_ERRORS ))
    {
        --err_num ;
        if ( ++debug_errors[err_num] > 999 ) debug_errors[err_num]=0 ;
    }
}

#else

void debug_error(int err_num)
{
}

#endif

#ifdef USB_REMOTE_RECORD

struct debug_rec_remote_state {
    long tick;
    char usb;
    char driver_state;
    char virtual_remote_state;
    char usb_count;
    char logic_module_state;
    char usb_sync_wait;
    char usb_remote_active;
};

#define DEBUG_MAX_REC 8192


struct debug_rec_remote_state *debug_rec_buf = NULL;

static int debug_num_rec = 0;
static int debug_num_rec_file = 0;

static void debug_dump_rec()
{
    char fn[12];
    int fd;
    sprintf(fn, "A/RC%05d.DAT", debug_num_rec_file);
    debug_num_rec_file++;

    fd = open(fn, O_WRONLY|O_CREAT, 0777);
    if (fd>=0)
    {
        write(fd, debug_rec_buf, sizeof(struct debug_rec_remote_state) * debug_num_rec);
        close(fd);
    }
    debug_num_rec = 0;
}

static void debug_add_rec()
{
    static int last_active = 0;
    if (debug_num_rec >= DEBUG_MAX_REC) return;
    if (!debug_rec_buf) debug_rec_buf = umalloc(sizeof(struct debug_rec_remote_state) * DEBUG_MAX_REC);
    if (!debug_rec_buf) return;

    debug_rec_buf[debug_num_rec].tick = get_tick_count();
    debug_rec_buf[debug_num_rec].usb = usb_state;

    debug_rec_buf[debug_num_rec].driver_state = driver_state;
    debug_rec_buf[debug_num_rec].virtual_remote_state = virtual_remote_state;
    debug_rec_buf[debug_num_rec].usb_count = usb_count;
    debug_rec_buf[debug_num_rec].logic_module_state = logic_module_state;
    debug_rec_buf[debug_num_rec].usb_sync_wait = usb_sync_wait_flag;
    debug_rec_buf[debug_num_rec].usb_remote_active = usb_remote_active;

    debug_num_rec++;
    if ((debug_num_rec >= DEBUG_MAX_REC) ||
        (last_active && !usb_remote_active)) debug_dump_rec();

    last_active = usb_remote_active;
}

#endif


/*---------------------------------------------------------------------------------------------------------

    kbd_synch_delay :
        support the sync delay option of USB remote

    Adds a fixed hard coded loop delay before releasing the shooting process in sync mode

  ---------------------------------------------------------------------------------------------------------*/

static int synch_delay_tick = 2800;                                     // default calibration value - not tuned for any camera


void kbd_synch_delay(int delay_value)
{
    int count1;

    for (count1=0;count1<delay_value;count1++)                          //wait delay_value * 0.1ms
    {
        asm volatile (
        "mov r1, #0\n"
        ".loop: \n\t"
        "add r1, r1, #1\n\t"
        "cmp r1, %0\n\t"
        "blt .loop"
        :
        : "r" (synch_delay_tick)
        : "r1" );

    }
}


#define CALIBRATE_SYNCH_DELAY 1
#undef CALIBRATE_SYNCH_DELAY                                            // Note : comment this out during testing to calibrate a camera's sync delay

#ifdef CALIBRATE_SYNCH_DELAY

#define KBD_CALIBRATE_TIME 100

static int synch_delay_calib = 0;

void kbd_calibrate_synch_delay()
{
    if (synch_delay_calib) return;
    int t0 = get_tick_count();
    kbd_synch_delay(10 * KBD_CALIBRATE_TIME);
    int t = get_tick_count() - t0;

    synch_delay_tick = synch_delay_tick * KBD_CALIBRATE_TIME / t;

    if (synch_delay_tick < 100) synch_delay_tick = 2800;                /*something went wrong */
    if (synch_delay_tick > 100000) synch_delay_tick = 2800;

    int fd;
    char buf[64];
    sprintf(buf, "%d %d ", synch_delay_tick, t);
    fd = open("A/CALIB.TXT", O_WRONLY|O_CREAT, 0777);
    if (fd>=0) {
      write(fd, buf, strlen(buf));
      close(fd);
      }
    synch_delay_calib = 1;
}
#endif

/*---------------------------------------------------------------------------------------------------------

    usb_remote_status_led :
        Turn on a camera LED while the camera is waiting for a USB 1->0 transistion to terminate sync wait mode

  ---------------------------------------------------------------------------------------------------------*/

void usb_remote_status_led(int state)
{
    if (camera_info.cam_remote_sync_status_led)
        *(int*)camera_info.cam_remote_sync_status_led=state ? 0x46 : 0x44;
}

/*---------------------------------------------------------------------------------------------------------

    usb_remote_key()

    - called from the kbd_process() keyboard handler or usb_HPtimer_good()
    - monitors USB power state and stores in global variable remote_state
    - captures the time of 0->1 and 1->0 transitions and buffers them
    - stores most recent power on pulse width in global variable usb_power
    - stores most recent pulse count in global variable usb_count
  ---------------------------------------------------------------------------------------------------------*/

void usb_buffer_insert(int value)
{
    if ( ++usb_buffer_in > &usb_buffer[USB_BUFFER_SIZE-1] ) usb_buffer_in = usb_buffer ;    
    if ( usb_buffer_in == usb_buffer_out )
    {
        if ( ++usb_buffer_out > &usb_buffer[USB_BUFFER_SIZE-1] ) usb_buffer_out = usb_buffer ;
    }
    *usb_buffer_in = value ;
}
 
void usb_remote_key( void )
{
    static int pulse_count=0 ;

    usb_state = get_remote_state() ;

    if(conf.remote_enable)
    {
        if (usb_state)
        {                                                                   // USB power is ON
            if (remote_mark_count<30000) remote_mark_count++ ;              // track how long the USB power is ON
            if (remote_space_count != 0)                                    // is this the 0 -> 1 transistion?
            {                                                               //
                usb_buffer_insert(remote_space_count);                      // insert space length into buffer
                remote_space_count = 0 ;                                    // reset the counter
            }
        }
        else
        {                                                                   // USB power if OFF
            if(remote_space_count>-30000) remote_space_count-- ;            // track how long the USB power is OFF (note space counts are negative)
            if (remote_mark_count != 0)                                     // is this the 1 -> 0 transistion?
            {                                                               //
                pulse_count++ ;                                             // count pulses transistions
                usb_power = remote_mark_count;                              // transfer most recent pulse length to variable read by scripts
                usb_buffer_insert(remote_mark_count);                       // insert pulse length into buffer
                remote_mark_count = 0;                                      // reset the counter
                camera_info.state.kbd_last_clicked = 0xFF;                  // flag the remote key as the last one pressed (for scripts)
                camera_info.state.kbd_last_clicked_time = get_tick_count(); // store key release time too
            }                                                               //
            if ((remote_space_count < -50) && (pulse_count > 0))            // pulse counting done if no activity for 50 timer periods
            {
                usb_count = pulse_count ;
                logic_module_usb_count = pulse_count ;
                pulse_count = 0 ;
            }
        }
    }
}


/*---------------------------------------------------------------------------------------------------------

     High Precision USB Remote Timer Callback routines.

  ---------------------------------------------------------------------------------------------------------*/
int usb_HPtimer_error_count;

int usb_HPtimer_good(int time, int interval) 
{
    usb_HPtimer_handle=0;
    start_usb_HPtimer(interval) ;
    usb_remote_key() ;
    return 0;
}
 
int usb_HPtimer_bad(int time, int interval) 
{
    usb_HPtimer_error_count++;
    return (usb_HPtimer_good(time, interval));
}

/*---------------------------------------------------------------------------------------------------------

     clear_usb_power()

  ---------------------------------------------------------------------------------------------------------*/

void clear_usb_power()
{
    usb_power = 0 ;
    usb_count = 0 ;
    logic_module_usb_count = 0;
    usb_buffer_out = usb_buffer_in = usb_buffer ;
}

/*---------------------------------------------------------------------------------------------------------

     get_usb_power()

  ---------------------------------------------------------------------------------------------------------*/


int get_usb_power(int mode)
{
    int x = 0;

    switch( mode)
    {
        case SINGLE_PULSE :
            x = usb_power;
            usb_power = 0;
            break ;
        case USB_STATE :
            x=usb_state;
            break ;
        case BUFFERED_PULSE :
            if ( usb_buffer_out != usb_buffer_in )
            {
                if ( ++usb_buffer_out > &usb_buffer[USB_BUFFER_SIZE-1] ) usb_buffer_out = usb_buffer ;
                x = *usb_buffer_out ;
            }
            break ;
        case PULSE_COUNT :
            x = usb_count;
            usb_count = 0;
            break ;
        case LM_PULSE_COUNT :
            x = logic_module_usb_count;
            logic_module_usb_count = 0;
            break ;
        case HPTIMER_ERROR_COUNT :
            x = usb_HPtimer_error_count;
            usb_HPtimer_error_count = 0;
            break ;
    }
    return x;
}


/*===================================================================================================

   main USB remote processing routine - called every 10 mSec fronm kbd.c

  ===================================================================================================*/

extern void (*usb_driver[])( ) ;
extern void (*usb_module_play[])( ) ;
extern void (*usb_module_shoot[])( ) ;
extern void (*usb_module_video[])( ) ;

static int rmt_state = RMT_DISABLED ;

void set_usb_remote_state()
{
    if (conf.remote_enable)
    {
        rmt_state = RMT_ENABLED ;
        switch_type = conf.remote_switch_type ;
        control_module = conf.remote_control_mode ;
    }
    else
    {
        rmt_state = RMT_DISABLED ;
        usb_remote_active = 0 ;
    }
    virtual_remote_state = driver_state = logic_module_state = REMOTE_RESET ;
}

int handle_usb_remote()
{
    if (conf.remote_enable)
    {
        if (camera_info.state.mode_play)
            camera_mode = CAMERA_MODE_PLAYBACK ;
        else
            camera_mode = camera_info.state.mode_video ? CAMERA_MODE_VIDEO : CAMERA_MODE_SHOOTING ;

        (*usb_driver[switch_type])(get_usb_power(USB_STATE));           // jump to driver state machine

        switch( camera_mode )
        {
            case CAMERA_MODE_PLAYBACK :
                (*usb_module_play[switch_type])();                      // jump to control module state machine
                break ;
            case CAMERA_MODE_SHOOTING :
                (*usb_module_shoot[control_module])();                  // jump to control module state machine
                break ;
            case CAMERA_MODE_VIDEO :
                (*usb_module_video[control_module])();                  // jump to control module state machine
                break ;
            default :
                conf.remote_enable = 0 ;
                break ;
        }

        usb_remote_active = ((logic_module_state  > 1) || (driver_state  > 1) || (virtual_remote_state  > 1) ) ? 1 : 0 ;

        #ifdef USB_REMOTE_RECORD
            debug_add_rec();
        #endif

        #ifdef USB_REMOTE_DEBUGGING
            extern int sync_counter ;
            extern long physw_status[3] ;
            extern int usb_buffer[] ;
            extern int * usb_buffer_in ;
            extern int * usb_buffer_out ;
            extern const char* gui_USB_switch_types[] ;
            extern const char* gui_USB_control_modes[];
            extern EXPO_BRACKETING_VALUES bracketing;
            char buf[64] ;
            static int debug_print = 0 ;
            static int startup_delay = 0 ;
            int i, buff_pos,  *buff_ptr ;

            if ( startup_delay < 100 ) startup_delay++ ;                // need to give things time to settle before using draw_string()
            else
            {
                if ( debug_print++ > 10000 ) debug_print = 0 ;

                if ((debug_print%2) == 0)
                {
                    switch( virtual_remote_state )
                    {
                        case  REMOTE_RESET :
                            sprintf(buf,"RESET      ") ;
                            break;
                        case  REMOTE_RELEASE :
                            sprintf(buf,"RELEASED  ") ;
                            break;
                        case  REMOTE_HALF_PRESS :
                            sprintf(buf,"HALF PRESS    ") ;
                            break;
                        case  REMOTE_FULL_PRESS :
                            sprintf(buf,"FULL PRESS    ") ;
                            break;
                        default :
                            sprintf(buf,"ERROR    ") ;
                            break;
                    }
                    draw_string(2,16,buf,MAKE_COLOR(COLOR_YELLOW,COLOR_RED));
                }
                else
                {
                    sprintf(buf,"RMT=%d drv=%d lgc=%d  sync=%d  tmo=%d  ", usb_remote_active, driver_state, logic_module_state, usb_sync_wait_flag, (bracketing_timeout?bracketing_timeout-get_tick_count():0));
                    draw_string(2,48,buf,MAKE_COLOR(COLOR_BLACK,COLOR_YELLOW));
                }

                if (((debug_print+25)%100) ==0 )
                {
                    sprintf(buf,"switch=%d logic=%d sync=%s mode=%d  ", switch_type, control_module, conf.synch_enable?"yes":"no", camera_mode) ;
                    draw_string(2,32,buf,MAKE_COLOR(COLOR_YELLOW,COLOR_BLACK));
                    sprintf(buf,"sync count=%d, pulse count=%d width=%d  b=%d  ", sync_counter, usb_count, usb_power,   bracketing.shoot_counter);
                    draw_string(2,64,buf,MAKE_COLOR(COLOR_BLACK,COLOR_YELLOW));
                    sprintf(buf,"physw=%d err=%d %d %d  ", physw_status[0]&0x03, debug_errors[0],  debug_errors[1],  debug_errors[2] );
                    draw_string(2,80,buf,MAKE_COLOR(COLOR_BLACK,COLOR_YELLOW));
                }

                if (((debug_print+75)%100) == 0 )
                {
                    buff_ptr = usb_buffer_in ;
                    buff_pos = 0 ;

                    for ( i=0 ; i<16 ; i++ )
                    {
                        sprintf(&buf[buff_pos],"%d ", *buff_ptr) ;
                        buff_pos = strlen(buf) ;
                        if ( buff_pos > 45 )
                        {
                            buf[45] = 0 ;
                            i=17 ;
                        }
                        if ( buff_ptr-- == usb_buffer )  buff_ptr = &usb_buffer[15] ;
                    }
                    draw_string(2,96,buf,MAKE_COLOR(COLOR_BLACK,COLOR_YELLOW));
                }
            }
        #endif
    }

    return usb_remote_active ;
}
