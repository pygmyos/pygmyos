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

#define AT24HC02B_ADDR0         0x50
#define AT24HC02B_ADDR1         0x51
#define AT24HC02B_ADDR2         0x52
#define AT24HC02B_ADDR3         0x53
#define AT24HC02B_ADDR4         0x54
#define AT24HC02B_ADDR5         0x55
#define AT24HC02B_ADDR6         0x56
#define AT24HC02B_ADDR7         0x57

void eepromOpen( u8 ucAddress, u8 ucSCL, u8 ucSDA, u8 ucWP );
void eepromErase( void );
void eepromPutChar( u16 uiAddress, u8 ucChar );
void eepromPutString( u16 uiAddress, u8 *ucBuffer );
void eepromPutString( u16 uiAddress, u8 *ucBuffer );
void eepromPutBuffer( u16 uiAddress, u8 *ucBuffer, u16 uiLen );
u8 eepromGetChar( u16 uiAddress );
void eepromGetBuffer( u16 uiAddress, u8 *ucBuffer, u16 uiLen );

