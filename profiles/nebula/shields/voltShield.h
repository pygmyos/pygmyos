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

#ifndef SHIELD_SCL
    #define SHIELD_SCL              TX1
#endif
#ifndef SHIELD_SDA
    #define SHIELD_SDA              RX1
#endif
#define SHIELD_INSW1            T0
#define SHIELD_RES2             T1
#define SHIELD_RES1             T2
#define SHIELD_RESET            T3
#define SHIELD_VOLT2            A0
#define SHIELD_VOLT1            A1
#define SHIELD_VOLT3            A2
#define SHIELD_VOLT4            A3
#define SHIELD_INSW2            D2
#define SHIELD_INSW3            D1
#define SHIELD_RES3             TA0
#define SHIELD_INSW4            TA1
#define SHIELD_RES4             A6
#define SHIELD_VENABLE          A7



// General Call Commands, may cause device conflicts
//#define DIGIPOT_WRITEWIPER0     0x40
//#define DIGIPOT_WRITEWIPER1     0x48
//#define DIGIPOT_WRITETCON       0x60
//#define DIGIPOT_INCWIPER0       0x42
//#define DIGIPOT_DECWIPER0       0x44
//#define DIGIPOT_INCWIPER1       0x4A
//#define DIGIPOT_DECWIPER1       0x4C
// End General Call Commands, may cause device conflicts


#define VOLTAGE                 0
#define RESISTANCE              1
#define AC                      0
#define DC                      1

void voltShieldInit( void );
u16 voltShieldGetGain( u8 ucChannel );
void voltShieldSetGain( u8 ucChannel, u16 uiGain );
void voltShieldSetCoupling( u8 ucChannel, u8 ucCoupling );
void voltShieldSetInputMode( u8 ucChannel, u8 ucMode );
u16 voltShieldGetVoltage( u8 ucChannel );
u16 voltShieldGetResistance( u8 ucChannel );