Index:
	01 - Introduction & Camera Specific Notes

	02 - Installation

	03 - FAQ

	04 - Links, Urls

	05 - GPL, License

	06 - uBASIC copyright notice


************************************


01 - Introduction

Hi, 

this is the small readme to CHDK. It provides just enough info to get you rolling, for more information use the links in the bottom.


CHDK is a firmware enhancement that operates on a number of Canon Cameras. CHDK gets loaded into your camera's memory upon bootup (either manually or automatically). It provides additional functionality beyond that currently provided by the native camera firmware.

CHDK is not a permanent firmware upgrade: you decide how it is loaded (manually or automatically) and you can always easily remove it. 

Main features:

    * Save images in RAW format
    * Ability to run "Scripts" to automate the camera
    * Live histogram (RGB, blended, luminance and for each RGB channel)
    * Zebra mode (blinking highlights and shadows to show over/under exposed areas)
    * An "always on" full range Battery indicator
    * Ability to turn off automatic dark-frame subtraction
    * a higher compression movie mode, and double the maximum video file size
    * exposure times as long as 65 seconds
    * exposure times as little as 1/10,000 of a second
    * ability to use the USB port for a remote trigger input 

Additional features:

    * a depth-of-field (DOF)-calculator
    * File browser
    * Text reader
    * Calendar
    * Some fun tools and games 

Why would I want to use CHDK?

    * To get Raw file capability on cameras that don't have that ability
    * To get the ability to use scripts
    * to be able to know the battery status at all times (not just when it's about to run out of power)
    * you want or need any of the other enhancement features that CHDK provides 

What are scripts? Scripts are Lua or uBASIC language programs that give you the ability to control the operation of the camera under program control. They have been used to add or extend the native capability of the camera: more flexible intervalometers, extended-range exposure compensation, extended bracketing ability, lightning photography, etc. See the script pages for more details.

************************************
IXUS160 / ELPH160 Notes 
************************************
Porting thread: http://chdk.setepontos.com/index.php?topic=12321

To enter CHDK <ALT> mode use the [>] Play button.

This is a beta port, use at your own risk!

WARNING
During early development of this port, one camera suffered an unrecoverable
hardware failure apparently connected to a CHDK crash. Another camera suffered
a similar failure but was eventually recovered. See discussion starting at
http://chdk.setepontos.com/index.php?topic=12321.msg123736#msg123736
for details.
Several possible causes were identified and fixed, but the actual cause has
not been conclusively identified.

Use of CHDK is always at your own risk.

TESTED
******
Filewrite task
RAW/DNG shooting
PTP support
Overrides

NOT IMPLEMENTED
***************
video stuff
exmem support

KNOWN BUGS
**********
Dark frame control: changes to this setting only take effect _after_ the next shot

 
THANKS
******

This port could not have been done without the help of the CHDK folks.
Without any specific order, thanks to reyalp, waterwingz, srsa_4c;
Thanks to amavroidis for donating the camera to help testing;
Thanks to markvandenborre for testing my billions of failures;
********************************

02 - Installation

To install CHDK on your Canon P&S camera,  you need to know the model number and firmware version of your cameras in order to get the right version of CHDK for your camera  Details on how to determine this can be found here :

http://chdk.wikia.com/wiki/Downloads


Once you have the correct CHDK installation file for your camera,  you need to prepare and load your camera's SD card with CHDK.  The best method for doing this depends on how you intend to use your camera and the date your particular carmera was released.  Details can be found here :

http://chdk.wikia.com/wiki/Prepare_your_SD_card

Once you have correctly loaded CHDK,  you can refer to the following section for instructions on how to use CHDK.

****************************************

03 - FAQ

1. What is CHDK?

    CHDK is not just one thing! The term CHDK refers to free software currently available for many (but not all) Canon PowerShot compact digital cameras that you can load onto your camera's memory card to give your camera greatly enhanced capabilities. 

2. Am I likely to be interested in CHDK?

    The enhanced capabilities that CHDK provides are most likely to be of interest to experienced photographers - if you believe that your Canon PowerShot camera already has more features than you will ever need, you probably won't be interested in CHDK. 

3. Is CHDK safe to use?

    Yes CHDK is experimental and has risks, but with tens of thousands of happy users it can generally be considered safe. CHDK doesn't make any actual changes to your camera. If you delete the CHDK software from your memory card, or if you choose not to activate the CHDK software on the card (or remove and replace the batteries), then the camera will behave absolutely normally - nothing has been (or ever is) changed, so the warranty is not affected. 

4. How does CHDK work?

    CHDK makes use of the microprocessor that controls the camera (every digital camera contains a microprocessor) to act as a programmable computer that provides the extra capabilities. 

5. What extra capabilities does CHDK provide?

    The current set of extra capabilities fall into six categories:

        a. Enhanced ways of recording images - you can capture still pictures in RAW format (as well as JPEG), and for video images you can have increased recording time and length (1 hour or 2 GB), and a greatly increased range of compression options. 
        b. Additional data displays on the LCD screen - histogram, battery life indicator, depth of field, and many more. 
        c. Additional photographic settings that are not available on the camera by itself - longer exposure times (up to 65 seconds), faster shutter speeds (1/25,000 sec, and faster in some cases), automatic bracketing of exposure, etc. 
        d. The ability for the camera to run programs ('scripts', written in a micro-version of the BASIC language) stored on the memory card - these programs allow you to set the camera to perform a sequence of operations under the control of the program. For example, a camera can be programmed to take multiple pictures for focus bracketing, or take a picture when it detects that something in the field of view moves or changes brightness. 
        e. The ability to take a picture, or start a program on the memory card, by sending a signal into the USB port - you can use the USB cable to take a picture remotely. 
        f. The ability to do a number of other more useful (and fun) things, such as act as a mini file browser for the memory card, let you play games on the LCD screen, etc. 

6. What else should I know?

    Developers around the world are continuing to add new features to CHDK. Because the idea of using the camera's microprocessor is so flexible, various developers have made different versions of CHDK, and new features continue to be developed - for example, one version of CHDK has features assist in taking stereo photographs, and even allows two cameras to be synchronized to take pictures at the same time (with an accuracy of better than 0.1 milliseconds, providing they are the same camera model). 

The best place to find out more about CHDK is to follow the CHDK wiki  ( http://chdk.wikia.com/wiki/CHDK ) and CHDK user forum ( http://chdk.setepontos.com/ ).




********************************

04 - URLs, Links


For more information visit the following links:

CHDK Forum :                    http://chdk.setepontos.com/index.php

CHDK Wiki:                      http://chdk.wikia.com/wiki/CHDK
CHDK Wiki FAQ:                  http://chdk.wikia.com/wiki/FAQ

CHDK Features :                 http://chdk.wikia.com/wiki/CHDK_User_Manual#Introduction
CHDK in Brief :                 http://chdk.wikia.com/wiki/CHDK_in_Brief
CHDK User Quick Start Guide :   http://chdk.wikia.com/wiki/CHDK_User_Quick_Start_Guide.pdf
CHDK User Manual :              http://chdk.wikia.com/wiki/CHDK_User_Manual
CHDK for Dummies :              http://chdk.wikia.com/wiki/CHDK_for_Dummies
CHDK Installation Guide :       http://chdk.wikia.com/wiki/File:CHDK_Installation_Guide.pdf

One Page Users Guide :          http://chdk.wikia.com/wiki/One_Page_Ultra-Quick_Users_Guide

Prepare Your SD Card :          http://chdk.wikia.com/wiki/Prepare_your_SD_card




*********************************

05 - GPL


/*
 * This file is part of CHDK.
 * Copyright (C) 2008 The CHDK Team
 * CHDK - CHDK Wiki
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA,
 * 02110-1301  USA
 */

*********************************

06 - uBASIC copyright notice

CHDK contains a modified version of Adam Dunkels uBASIC. 

The following notice applies to the original uBASIC code:

Copyright (c) 2006, Adam Dunkels
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the author nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

