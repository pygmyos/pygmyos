/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

// This file is intended to be used as a template
// To use, copy into your project folder and:
// Add the board specific header ( if needed )
// Add any defines for your specific project 
// Include at the top of main.c
// Comment out lines to disable functionallity

#pragma once

#define __PYGMYNEBULA

#define __PYGMYCOMMANDS
#define __PYGMYFILES
#define __PYGMYTASKS
#define __PYGMYSPRITES
//#define __PYGMYMESSAGES

//#define __PYGMYMODEMSHIELD
//#define __PYGMYVOLTSHIELD
//#define __PYGMYWEATHERSHIELD
//#define __PYGMYMOTIONSHIELD
//#define __PYGMYIMUSHIELD
#define __PYGMYLCDSHIELD

//---------------------------------- Stream Setting Defines-----------------------------------
//--------------------------------------------------------------------------------------------
#define __PYGMYSTREAMS
    //#define __PYGMYSTREAMCOM1
    //#define __PYGMYSTREAMCOM2
    #define __PYGMYSTREAMCOM3
    //#define __PYGMYSTREAMCOM4
    //#define __PYGMYSTREAMCOM5
    #define __PYGMYSTREAMLCD
    #define __PYGMYSTREAMFILE
    //#define __PYGMYSTREAMUSER
    #define __PYGMY
//---------------------------------- Stream Setting Defines-----------------------------------
//--------------------------------------------------------------------------------------------

#include "pygmy_sys.h"
#include "pygmy_cmd.h"
#include "pygmy_debug.h"
#include "pygmy_fpec.h"
#include "pygmy_port.h"
#include "pygmy_com.h"
#include "pygmy_audio.h"
#include "pygmy_adc.h"
#include "pygmy_file.h"
#include "pygmy_lcd.h"
#include "pygmy_gui.h"
#include "pygmy_nvic.h"
#include "pygmy_rtc.h"
#include "pygmy_string.h"
#include "pygmy_rf.h"

//-------------------------------------------------------------------------------------
// Place Hardware Defines Below -------------------------------------------------------
//-------------------------------------------------------------------------------------

#include "profiles/nebula/pygmy_nebula.h"
//#include "profiles/nebula/shields/lcdVaritronixParallel.h"
//#include "profiles/nebula/shields/voltShield.h"
//#include "profiles/digipots/mcp443x.h"
//#include "profiles/nebula/shields/gsmModem.h"

//-------------------------------------------------------------------------------------
// Place User Defines Below -----------------------------------------------------------
//-------------------------------------------------------------------------------------


