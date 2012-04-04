/**************************************************************************
    Pygmy Gems ( Pygmy Operating System )
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

#pragma once
#include "pygmy_profile.h"

#define AT24HC02B_ADDR0         0x50
#define AT24HC02B_ADDR1         0x51
#define AT24HC02B_ADDR2         0x52
#define AT24HC02B_ADDR3         0x53
#define AT24HC02B_ADDR4         0x54
#define AT24HC02B_ADDR5         0x55
#define AT24HC02B_ADDR6         0x56
#define AT24HC02B_ADDR7         0x57

void eepromOpen( u8 ucAddress, u8 ucSCL, u8 ucSDA, u8 ucWP );
void eepromEnableWP( void );
void eepromDisableWP( void );
void eepromErase( void );
u8 *eepromQueryBus( void );
u8 eepromGetAddress( void );
void eepromPollAck( void );
u8 eepromPutChar( u8 ucAddress, u8 ucChar );
u8 eepromPutString( u8 ucAddress, u8 *ucBuffer );
u8 eepromPutBuffer( u8 ucAddress, u8 *ucBuffer, u8 ucLen );
u8 eepromGetChar( u8 ucAddress );
void eepromGetBuffer( u8 ucAddress, u8 *ucBuffer, u8 ucLen );

