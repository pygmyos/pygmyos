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
u8 globalEEPROMWP = 0;

void eepromOpen( u8 ucAddress, u8 ucSCL, u8 ucSDA, u8 ucWP )
{
    globalEEPROMWP = ucWP;
    if( globalEEPROMWP != NONE ){
        pinConfig( globalEEPROMWP, OUT );
        pinSet( globalEEPROMWP, HIGH );
    } // if
    i2cConfig( &globalI2CEEPROM, ucSCL, ucSDA, ucAddress, I2CSPEEDFAST );
    // Clear bus
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
    i2cWriteByte( &globalI2CEEPROM, 0xFF );
    i2cWriteByte( &globalI2CEEPROM, 0 );
    i2cStop( &globalI2CEEPROM );
}

void eepromEnableWP( void )
{
    if( globalEEPROMWP != NONE ){
        pinSet( globalEEPROMWP, HIGH );
    } // if
}

void eepromDisableWP( void )
{
    if( globalEEPROMWP != NONE ){
        pinSet( globalEEPROMWP, LOW );
    } // if
}

void eepromErase( void )
{
    u16 i;
    u8 ucChar = 0xFF;

    for( i = 0; i < 0xFF; i++ ){
        eepromPutChar( i, 0x00 );
        if( !( i % 4 ) ){ 
            print( STDIO, "." );
        } // if
    } // for
}

u8 *eepromQueryBus( void )
{
    static u8 ucEEPROM[ 8 ];
    u8 i;

    for( i = 0; i < 8; i++ ){
        i2cStart( &globalI2CEEPROM );
        
        if( i2cWriteByte( &globalI2CEEPROM, ( ( 0x50 + i ) << 1 ) | 1 ) ){
            //print( COM3, "\rWB NACK" );
            ucEEPROM[ i ] = 0; // NACK is high, if no ACK ( low ) read, then device not present
        } else{
            //print( COM3, "\rWB ACK" );
            ucEEPROM[ i ] = 0x50 | i;
        } // else
        i2cStop( &globalI2CEEPROM );
    } // for

    return( ucEEPROM );
}

u8 eepromGetAddress( void )
{
    u8 ucChar;

    i2cStart( &globalI2CEEPROM );
    //i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address | 1 ); // Read
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address | 1 ); // Read
    ucChar = i2cReadByte( &globalI2CEEPROM ); // Clock Out Byte
    i2cWriteBit( &globalI2CEEPROM, 1 ); // High NACK to end sequence
    i2cStop( &globalI2CEEPROM );

    return( ucChar );
}

void eepromPollAck( void )
{
    u16 i;
    u8 ucAck;

    for( i = 0; i < 0xFFFF; i++  ){
        i2cDelay( &globalI2CEEPROM );
        i2cStart( &globalI2CEEPROM );
        ucAck = i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
        i2cStop( &globalI2CEEPROM );
        if( !ucAck ){
            //print( COM3, "\rFound Ack" );
            break;
        } // if
    } // for
}

u8 eepromPutChar( u8 ucAddress, u8 ucChar )
{
    u8 i, ucRetry, ucAck;

    PYGMY_WATCHDOG_REFRESH;
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
    i2cWriteByte( &globalI2CEEPROM, ucAddress );
    i2cWriteByte( &globalI2CEEPROM, ucChar );
    i2cStop( &globalI2CEEPROM );
    eepromPollAck();
    //delayms( 5 );
    
    return( TRUE );
}

u8 eepromPutString( u8 ucAddress, u8 *ucBuffer )
{   
    for( ; *ucBuffer; ucAddress++, ucBuffer++  ){
        if( !eepromPutChar( ucAddress, *ucBuffer ) ){
            return( FALSE );
        } // if
    } // for
    
    return( TRUE );
}

u8 eepromPutBuffer( u8 ucAddress, u8 *ucBuffer, u8 ucLen )
{
    u8 i;

    for( i = 0; i < ucLen; i++ ){
        if( !eepromPutChar( ucAddress + i, *ucBuffer ) ){
            return( FALSE );   
        } // else
    } // for
    
    return( TRUE );
}

u8 eepromGetChar( u8 ucAddress )
{
    u8 ucChar;

    PYGMY_WATCHDOG_REFRESH;
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address );
    i2cWriteByte( &globalI2CEEPROM, ucAddress );
    //i2cStop( &globalI2CEEPROM );
    i2cStart( &globalI2CEEPROM );
    i2cWriteByte( &globalI2CEEPROM, globalI2CEEPROM.Address | 1 ); // Read
    ucChar = i2cReadByte( &globalI2CEEPROM ); // Clock in Byte
    
    i2cWriteBit( &globalI2CEEPROM, 1 ); // High NACK to end sequence
    i2cStop( &globalI2CEEPROM );

    return( ucChar );
}

void eepromGetBuffer( u8 ucAddress, u8 *ucBuffer, u8 ucLen )
{
    u8 i;

    if( ucLen > 255 ){
        return;
    } // if
    for( i = 0; i < ucLen; i++ ){
        ucBuffer[ i ] = eepromGetChar( ucAddress + i );
    } // for
    //i2cReadBuffer( &globalI2CEEPROM, ucAddress, ucBuffer, ucLen );
}
