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

#include "pygmy_profile.h"
#include "profiles/eeprom/at24hc02b.h"

PYGMYI2CPORT globalI2CEEPROM;
u8 globalEEPROMWP;

void eepromOpen( u8 ucAddress, u8 ucSCL, u8 ucSDA, u8 ucWP )
{
    globalEEPROMWP = ucWP;
    if( globalEEPROMWP != NONE ){
        pinConfig( globalEEPROMWP, OUT );
        pinSet( globalEEPROMWP, HIGH );
    } // if
    i2cConfig( &globalI2CEEPROM, ucSCL, ucSDA, ucAddress, I2CSPEEDFAST );
    //delay( 5000 );
    //print( COM3, "\rEEPROM Ready" );
    //i2cResetBus( &globalI2CEEPROM );
}

void eepromErase( void )
{
    u16 i;
    u8 ucChar = 0xFF;

    for( i = 0; i < 256; i++ ){
        i2cWriteBuffer( &globalI2CEEPROM, (u8)i, &ucChar, 1 );
        delay( 5000 );
    } // for
}

void eepromPutChar( u16 uiAddress, u8 ucChar )
{
    u16 i;
    
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
    i2cWriteByte( &globalI2CEEPROM, (u8)uiAddress );
    i2cWriteByte( &globalI2CEEPROM, ucChar );
    i2cStop( &globalI2CEEPROM );
    //u8 *ucPtr;
    
    //ucPtr = ucChar;
    //print( COM3, "\rWriting %c", ucChar );
    //i2cWriteBuffer( &globalI2CEEPROM, (u8)uiAddress, &ucChar, 1 );
    //print( COM3, " Waiting..." );
    delay( 5000 );
    //print( COM3, "Done." );
}

void eepromPutString( u16 uiAddress, u8 *ucBuffer )
{   
    print( COM3, "\rWriting to EEPROM:" );
    for( ; *ucBuffer; uiAddress++, ucBuffer++ ){
        eepromPutChar( uiAddress, *(ucBuffer) );
        print( COM3, " (%c %c)", *ucBuffer, eepromGetChar( uiAddress ) );
        
        //i2cWriteBuffer( &globalI2CEEPROM, (u8)uiAddress, ucBuffer, 1 );
        //delay( 5000 );
    } // for
}

void eepromPutBuffer( u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;

    for( i = 0; i < uiLen; i++ ){
        eepromPutChar( (u8)(uiAddress+i), *(ucBuffer++) );
        //i2cWriteBuffer( &globalI2CEEPROM, (u8)(uiAddress+i), ucBuffer+i, i );
        //delay( 5000 );
    } // for
}

u8 eepromGetChar( u16 uiAddress )
{
    u16 i;
    u8 ucChar;

    //i2cReadBuffer( &globalI2CEEPROM, (u8)uiAddress, &ucChar, 1 );

    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
    
    i2cWriteByte( &globalI2CEEPROM, (u8)uiAddress );
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address | 1 ); // Read
    ucChar = i2cReadByte( &globalI2CEEPROM ); // Clock Out Byte
    
    i2cWriteBit( &globalI2CEEPROM, 1 ); // High NACK to end sequence
    i2cStop( &globalI2CEEPROM );

    return( ucChar );
}

void eepromGetBuffer( u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    i2cReadBuffer( &globalI2CEEPROM, (u8)uiAddress, ucBuffer, uiLen );
}
