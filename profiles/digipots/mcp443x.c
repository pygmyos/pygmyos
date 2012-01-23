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

// This file supports the Microchip MCP443X 4 Channel Digipots
// Functions in the file assume that the 8 bit versions are being used
// High voltage functions aren't directly supported

//  I2C Inc Frame:
//  Byte 1: [START]([0][1][0][1][1][A1][A0][RW])                    // RW = 0
//  Byte 2: [ACK]([ADDR3][ADDR2][ADDR1][ADDR0][C1][C0][X][X])[STOP] // C1,C0 = 0,1

//  I2C Dec Frame:
//  Byte 1: [START]([0][1][0][1][1][A1][A0][RW])                    // RW = 0
//  Byte 2: [ACK]([ADDR3][ADDR2][ADDR1][ADDR0][C1][C0][X][X])[STOP] // C1,C0 = 1,0

//  I2C Write Frame: 
//  Byte 1: [START]([0][1][0][1][1][A1][A0][RW])                    // RW = 0
//  Byte 2: [ACK]([ADDR3][ADDR2][ADDR1][ADDR0][C1][C0][X][D8])      // C1,C0 = 0,0
//  Byte 3: [ACK]([D7][D6][D5][D4][D3][D2][D1][D0])[STOP]

// I2C Read Frame:
//  Byte 1: [START]([0][1][0][1][1][A1][A0][RW])                    // RW = 1
//  Byte 2: [ACK]([ADDR3][ADDR2][ADDR1][ADDR0][C1][C0][X][X])       // C1, C0 = 1,1
//  Byte 3: [START]([0][1][0][1][1][A1][A0][RW])
//  Byte 4: [ACK]([X][X][X][X][X][X][X][A8])
//  Byte 5: [ACK]([A7][A6][A5][A4][A3][A2][A1][A0])[STOP]

#include "pygmy_profile.h"

u8 const PYGMYDIGIPOTWIPERS[] = { DIGIPOT_WIPER0, DIGIPOT_WIPER1, DIGIPOT_WIPER2, DIGIPOT_WIPER3 };

u8 digipotIncWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper )
{
    u8 ucCmd;

    if( ucWiper > 3 ){
        return( FALSE );
    } // if
    ucCmd = PYGMYDIGIPOTWIPERS[ ucWiper ] | DIGIPOT_CMD_INC;
    
    i2cWriteBuffer( pygmyI2C, ucCmd, NULL, 0 );
    
    return( TRUE );
}

u8 digipotDecWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper )
{
    u8 ucCmd;

    if( ucWiper > 3 ){
        return( FALSE );
    } // if
    ucCmd = PYGMYDIGIPOTWIPERS[ ucWiper ] | DIGIPOT_CMD_DEC;
    
    i2cWriteBuffer( pygmyI2C, ucCmd, NULL, 0 );
    
    return( TRUE );
}

u8 digipotSetWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper, u16 uiPos )
{
    u8 ucCmd, ucData[ 1 ];

    if( ucWiper > 3 ){
        return( FALSE );
    } // if
    ucCmd = PYGMYDIGIPOTWIPERS[ ucWiper ] | DIGIPOT_CMD_WRITE | ( uiPos & BIT8 );
    ucData[ 0 ] = (u8)uiPos;
    i2cWriteBuffer( pygmyI2C, ucCmd, ucData, 1 );
    
    return( TRUE );

}

u16 digipotGetWiper( PYGMYI2CPORT *pygmyI2C, u8 ucWiper )
{
    u16 uiWiperPos;
    u8 ucCmd, ucData[ 2 ];
    
    if( ucWiper > 3 ){
        return( 0xFFFF );
    } // if
    ucCmd = PYGMYDIGIPOTWIPERS[ ucWiper ] | DIGIPOT_CMD_READ;
    i2cReadBuffer( pygmyI2C, ucCmd, ucData,  2 );
    uiWiperPos = ((u16)ucData[ 0 ] << 9 ) | ucData[ 1 ];

    return( uiWiperPos );
}
