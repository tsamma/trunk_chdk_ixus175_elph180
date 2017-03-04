--[[
GENERATED PROPCASE TABLE
use propcase.lua instead
--]]
return {
  AE_LOCK=3,          -- 0 = AE not locked, 1 = AE locked
  AF_ASSIST_BEAM=5,          -- 0=disabled,  1=enabled
  REAL_FOCUS_MODE=6,          -- 0 = AF, 1 = Macro, 4 = MF (g7x)
  AF_FRAME=8,          -- 1 = FlexiZone, 2 = Face AiAF / Tracking AF
  AF_LOCK=11,         -- 0 = AF not locked, 1 = AF locked (not verified, g7x AF lock just enables MF at current dist)
  CONTINUOUS_AF=12,         -- 0 = Continuous AF off, 1 = Continuous AF on (g7x)
  FOCUS_STATE=18,         -- 1 AF done
  AV2=22,         -- (philmoz, May 2011) - this value causes overrides to be saved in JPEG and shown on Canon OSD
  AV=23,         -- This values causes the actual aperture value to be overriden
  MIN_AV=27,         -- or 28?
  USER_AV=30,         -- or 29, values differ slightly. 29 appears to have round APEX96 vals, 30 matches PROPCASE_AV
  BRACKET_MODE=33,         -- 0 = 0ff, 1 = exposure, 2 = focus (MF only) (g7x)
  BV=38,
  SHOOTING_MODE=53,         -- 54 shows C as distinct mode
  CUSTOM_SATURATION=60,         -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  QUALITY=62,
  CUSTOM_CONTRAST=64,         -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  FLASH_SYNC_CURTAIN=69,         -- 0 first, 1 second
  SUBJECT_DIST2=70,
  DELTA_SV=84,         -- TODO not certain
  DIGITAL_ZOOM_MODE=97,         -- Digital Zoom Mode/State 0 = off, 1=standard, 2 = 1.5x, 3 = 2.0x
  DIGITAL_ZOOM_POSITION=101,        -- also 269?
  DRIVE_MODE=108,        -- 0 = single, 1 = cont, 2 = cont AF
  OVEREXPOSURE=109,        -- TODO guessed
  DISPLAY_MODE=111,
  EV_CORRECTION_1=113,
  FLASH_ADJUST_MODE=127,    -- TODO ??? guessed ps6 +6
  FLASH_FIRE=128,    -- TODO guessed ps6 +6
  FLASH_EXP_COMP=133,    -- APEX96 units
  FOCUS_MODE=139,    -- 0 = auto, 1 = MF
  FLASH_MANUAL_OUTPUT=147,        -- TODO guessed (ps6 had unsure comments too)
  FLASH_MODE=149,        -- 0 = Auto, 1 = ON, 2 = OFF
  IS_MODE=151,        -- 0 = Continuous, 1 = only Shoot, 2 = OFF (399 "dynamic IS" setting?)
  ISO_MODE=155,
  METERING_MODE=163,        -- 0 = Evaluative, 1 = Spot, 2 = Center weighted avg
  VIDEO_FRAMERATE=173,        -- 0=30, 7=60 (g7x)
  VIDEO_RESOLUTION=176,        -- 5=1920x1280, 4=1280x720 2=640x480 (g7x)
  CUSTOM_BLUE=182,        -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  CUSTOM_GREEN=182,        -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  CUSTOM_RED=184,        -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  CUSTOM_SKIN_TONE=185,        -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  MY_COLORS=193,        -- 0 = Off, 1 = Vivid, 2 = Neutral, 3 = B/W, 4 = Sepia, 5 = Positive Film, 6 = Lighter Skin Tone, 7 = Darker Skin Tone, 8 = Vivid Red, 9 = Vivid Green, 10 = Vivid Blue, 11 = Custom Color
  ND_FILTER_STATE=201,        -- 0 = out, 1 = in
  OPTICAL_ZOOM_POSITION=204, 
  EXPOSURE_LOCK=215,        -- Old PROPCASE_SHOOTING value - gets set when set_aelock called or AEL button pressed
  EV_CORRECTION_2=216,        -- g7x ok, ps6 +6
  IS_FLASH_READY=217,        -- not certain
  IMAGE_FORMAT=219,        -- 0 = RAW, 1 = JPEG, 2 = RAW+JPEG (g7x)
  RESOLUTION=227,        -- 0 = L, 2 = M1, 3 = M2, 5 = S
  ORIENTATION_SENSOR=228,
  TIMER_MODE=232,        -- 0 = OFF, 1 = 2 sec, 2 = 10 sec, 3 = Custom
  TIMER_DELAY=233,        -- timer delay in msec
  CUSTOM_SHARPNESS=234,        -- Canon Menu slide bar values: 255, 254, 0, 1, 2
  SUBJECT_DIST1=254,        -- 262 MF value?
  SV_MARKET=255,
  TV2=271,        -- (philmoz, May 2011) - this value causes overrides to be saved in JPEG and shown on Canon OSD
  TV=272,        -- Need to set this value for overrides to work correctly
  USER_TV=274,
  WB_MODE=279,        -- 0 = Auto, 1 = Daylight, 2 = Shade, 3 = Cloudy, 4 = Tungsten, 5 = Fluorescent, 7 = flash, 11 = under water, 6 = Fluorescent H, 9 = Custom 1, 10 = custom 2
  WB_ADJ=280,
  SERVO_AF=306,        -- 0 = Servo AF off, 1 = Servo AF on
  ASPECT_RATIO=207,        -- 0 = 4:3, 1 = 16:9, 2 = 3:2, 3 = 1:1, 4 = 4:5
  SV=354,        -- (philmoz, May 2011) - this value causes overrides to be saved in JPEG and shown on Canon OSD
  TIMER_SHOTS=384,        -- Number of shots for TIMER_MODE=Custom
  SHOOTING_STATE=359,        -- Goes to 1 soon after half press, 2 around when override hook called, 3 after shot start, back to 2 when shoot_full released, back to 0 when half released
  SHOOTING=1001,        -- fake, emulated by wrapper using SHOOTING_STATE
}
