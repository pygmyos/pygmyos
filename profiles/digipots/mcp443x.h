/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

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

// Warning!!! All addresses defined below are left shifted i nibble ( 4 bits )
#define DIGIPOT_WIPER0          0x00
#define DIGIPOT_WIPER1          0x10 
#define DIGIPOT_TCON0           0x40
#define DIGIPOT_STATUS          0x50
#define DIGIPOT_WIPER2          0x60
#define DIGIPOT_WIPER3          0x70
#define DIGIPOT_TCON1           0xA0

// Warning!!! All commands defined below are left shifted 2 bits
#define DIGIPOT_CMD_WRITE       0x00
#define DIGIPOT_CMD_INC         0x04
#define DIGIPOT_CMD_DEC         0x08
#define DIGIPOT_CMD_READ        0x0C

#define DIGIPOT_BASEADDRESS     0x2C 

u8 digipotIncWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper );
u8 digipotDecWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper );
u8 digipotSetWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper, u16 uiPos );
u16 digipotGetWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper );
