/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

    This file is part of PygmyOS.

    PygmyOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PygmyOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PygmyOS.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "pygmy_profile.h"

#define SHIELD_HOME         PA9 // COM1_TX
#define SHIELD_UP           PA0 // A0
#define SHIELD_DOWN         PA1 // A1
#define SHIELD_CENTER       PC8 // D3
#define SHIELD_LEFT         PA7 // TA1
#define SHIELD_RIGHT        PA6 // TA0
#define SHIELD_SHUTDOWN     PA5 // DAC2
#define SHIELD_MICPWR       PA3 // COM2_RX
#define SHIELD_MIC          PA2 // COM2_TX

#define LCD_SCK             PC6 // D1
#define LCD_MOSI            PC7 // D2
#define LCD_A0              PB9 // T3
#define LCD_RESET           PB6 // T0
#define LCD_CS              PB5 // D0
#define LCD_BL              PA8 // MCO