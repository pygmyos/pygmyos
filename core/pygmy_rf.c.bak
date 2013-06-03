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

#include "pygmy_profile.h"

// WARNING! This file is Preliminary and still under initial construction!
// RF communicates through sockets, each socket is negotiated for one
// transaction between two devices. Once the transaction is completed,
// the socket should be closed.
// [ DESTID 4B ][ PORT 1B ][ PCOUNT 1B ][ DATA 25B ][ CRC 2B ]
// Command Socket
    // string formatted numerical value up to twenty digits counting sign and decimal point
    // Response in TX mode is CLOSE, Response in RX mode is DATA followed by CLOSE 
    // [ DESTID 4B ][ COMMAND | LEN 1B ][ TYPE 1B ][ SRCID 4B ][ PAYLOAD 1-20B ][ CRC 2B ]

u8 *globalRFSubNetMask;
const u8 ucAddrP0[] = {0x78,0x78,0x78,0x78,0x78};
//const u8 ucAddrP0[] = {0xE7,0xE7,0xE7,0xE7,0xE7};
u32 globalRFConfig;
PYGMYSPIPORT pygmyRFSPI;


//--------------------------------------------------------------------------------------------
//-----------------------------------------RF Receive-----------------------------------------
void rfRX( void )
{
    u8 i, ucLen, ucStatus, ucPipe, *ucBuffer, ucPayload[ 32 ];
    
    PYGMY_WATCHDOG_REFRESH;
    ucBuffer = ucPayload;
    ucStatus = rfGetStatus();
    ucPipe = ( ucStatus & 0x0E ) >> 1;
    #ifdef __PYGMY_DEBUG_SOCKETS
        print( COM3, "\rRX IRQ" );
    #endif // __PYGMY_DEBUG_SOCKETS
    if( ucStatus & BIT6 ){
        // Payload Received
        #ifdef __PYGMY_DEBUG_SOCKETS
            print( COM3, "\rReceived Payload" );
        #endif // __PYGMY_DEBUG_SOCKETS
        if( ucPipe == 7 ){
            #ifdef __PYGMY_DEBUG_SOCKETS
                print( COM3, "\rPipe Error" );
            #endif // __PYGMY_DEBUG_SOCKETS
            return;
        } // if
        ucLen = rfGetRXPayloadLen( ); // Payloads are a maximum of 32 bytes
        #ifdef __PYGMY_DEBUG_SOCKETS
            print( COM3, "\r%d Bytes Received\r", ucLen );
        #endif // __PYGMY_DEBUG_SOCKETS
        // Now load payload into buffer
        RF_CS_LOW;
        spiWriteByte( &pygmyRFSPI, RF_RX_PAYLOAD );
        for( i = 0; i < ucLen; i++ ){
            ucBuffer[ i ] = spiReadByte( &pygmyRFSPI );
            //delay( 1 );
            #ifdef __PYGMY_DEBUG_SOCKETS
                print( COM3, " %X", ucBuffer[ i ] );
                //print( COM3, "\rSending Payload" );
            #endif // __PYGMY_DEBUG_SOCKETS
            
        } // if
        RF_CS_HIGH;
        //spiGetBuffer( &pygmyRFSPI, RF_RX_PAYLOAD, ucBuffer, ucLen );
        #ifdef __PYGMY_DEBUG_SOCKETS
            print( COM3, "\rPassing to Handler" );
        #endif // __PYGMY_DEBUG_SOCKETS
        #ifdef __PYGMYSOCKETS
        socketHandler( ucBuffer );//&pygmyPacket );
        #endif
        rfClearStatus();
    } //if
    if( ucStatus & BIT5 ){
        rfSetRX();
    } // if 
}


void rfInit( void )
{
    spiConfig( &pygmyRFSPI, RF_CS, RF_SCK, RF_MISO, RF_MOSI, 0 );
    pinConfig( RF_EN, OUT );
    RF_EN_LOW;
    pinConfig( RF_IRQ, PULLUP );
    pinInterrupt( rfRX, RF_IRQ, TRIGGER_FALLING, 3 );
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RETR, 0x0F );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RETR );
    spiWriteByte( &pygmyRFSPI, 0x0F ); // 15 retries, 250 microsec delay between tries
    RF_CS_HIGH;
    //rfSetTXPower( RF_2MBPS, 0 );
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR, BIT0 );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD, BIT0 );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE, RF_REG_FEATURE_EN_DPL );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL );
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_ENAA, BIT0 );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENAA );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    rfFlushTX();
    rfFlushRX();
    
    rfWriteAddress( RF_REG_TXADDR, (u8*)ucAddrP0 );
    rfWriteAddress( RF_REG_RXADDR_P0, (u8*)ucAddrP0 );
    rfSetRX();
}

void rfSetTXPower( u8 ucDataRate, u8 ucPower )
{
    if( ucDataRate == RF_250KBPS ){
        ucDataRate = RF_REG_SETUP_RF_DRLOW;
    } else if( ucDataRate == RF_1MBPS ){
        ucDataRate = 0;
    } else {
        ucDataRate = RF_REG_SETUP_RF_DRHIGH;
    } // else
    if( ucPower > 3 ){
        ucPower = 3;
    } // if
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RF, ( ucDataRate | ucPower ) );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RF );
    spiWriteByte( &pygmyRFSPI, ( ucDataRate | ucPower ) );
    RF_CS_HIGH;
}

u8 rfGetSignalQuality( void )
{
    u8 ucSignalQuality;

    //spiPutChar( &pygmyRFSPI, RF_READ|RF_REG_OBSERVETX,  
    //RF_CS_LOW;
    pygmyRFSPI.PortCS->BRR = pygmyRFSPI.PinCS;
    spiWriteByte( &pygmyRFSPI, RF_READ|RF_REG_OBSERVETX );
    ucSignalQuality = 15 - ( RF_REG_OBSERVETX_ARCCNT & spiReadByte( &pygmyRFSPI ) ); 
    pygmyRFSPI.PortCS->BSRR = pygmyRFSPI.PinCS;
    //RF_CS_HIGH;
    
    rfClearStatus();
    
    return( ucSignalQuality );
}

u8 rfGetTX( void )
{
    return( globalRFConfig & BIT0 );
}

u8 rfSetTX( void )
{
    globalRFConfig |= BIT0;
    RF_EN_LOW;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_EN_CRC ); 
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_EN_CRC );
    rfClearStatus( );
    RF_EN_HIGH;
}

u8 rfSetRX( void )
{
    globalRFConfig &= ~BIT0;
    RF_EN_LOW;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_PRIM_RX|RF_REG_CONFIG_EN_CRC ); 
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_PRIM_RX|RF_REG_CONFIG_EN_CRC );
    rfClearStatus( );
    RF_EN_HIGH;
}

void rfFlushTX( void )
{
    //spiPutCommand( &pygmyRFSPI, RF_TX_FLUSH );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_FLUSH );
    RF_CS_HIGH;
}

void rfFlushRX( void )
{
    //spiPutCommand( &pygmyRFSPI, RF_RX_FLUSH );
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_RX_FLUSH );
    RF_CS_HIGH;
}

void rfPutTXBuffer( u8 *ucBuffer, u16 uiLen )
{
    u8 i;

    #ifdef __PYGMY_DEBUG_SOCKETS
        print( COM3, "\rTX Packet:" );
    #endif // __PYGMY_DEBUG_SOCKETS
    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    for( i = 0; i < uiLen; i++ ){
        #ifdef __PYGMY_DEBUG_SOCKETS
            print( COM3, " %02X", *ucBuffer );
        #endif // __PYGMY_DEBUG_SOCKETS
        spiWriteByte( &pygmyRFSPI, *(ucBuffer++) ); 
        
    } // for
    RF_CS_HIGH;
    
    //spiPutBuffer( &pygmyRFSPI, RF_TX_PAYLOAD, ucBuffer, uiLen );
    //#ifdef __PYGMY_DEBUG_SOCKETS
    //    print( COM3, "\rWait for TX Clear" );
    //#endif // __PYGMY_DEBUG_SOCKETS
    while( !(rfGetStatus() & BIT5 ) );
    //for( i = 0; i < 20 && !( rfGetStatus() & BIT5 ); i++ ){
        // Default set by rfInit is 15 tries at 250 microseconds delay between
        //delay( 50 );
    //} // for
    #ifdef __PYGMY_DEBUG_SOCKETS
        print( COM3, "Done." );
    #endif // __PYGMY_DEBUG_SOCKETS
    if( i == 20 ){
        // If link is lost then Flush TX to prevent TX lock
        // High level socket protocol may retry later as appropriate
        rfFlushTX( );
    } // if
    //while( !( rfGetStatus() & BIT5 ) ){;}
    rfClearStatus();
    rfSetRX();
}

u8 rfGetStatus( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    //pygmyRFSPI.PortCS->BRR = pygmyRFSPI.PinCS;
    ucStatus = spiReadByte( &pygmyRFSPI );
    //pygmyRFSPI.PortCS->BSRR = pygmyRFSPI.PinCS;
    RF_CS_HIGH;

    return( ucStatus );
}

u8 rfGetRXPayloadLen( void )
{
    u8 ucByte;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_RX_LEN );
    ucByte = spiReadByte( &pygmyRFSPI  );
    RF_CS_HIGH;
    
    return( ucByte ); 
    //return( spiGetChar( &pygmyRFSPI, RF_RX_LEN ) );
}

u8 rfIsRXFIFOEmpty( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_REG_FIFOSTATUS );
    ucStatus = spiReadByte( &pygmyRFSPI ) & BIT0;
    RF_CS_HIGH;

    return( ucStatus );
    //return( spiGetChar( &pygmyRFSPI, RF_REG_FIFOSTATUS ) & BIT0 );
}

void rfClearStatus( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_STATUS );
    spiWriteByte( &pygmyRFSPI, BIT6|BIT5|BIT4 ); 
    RF_CS_HIGH;
    //spiPutChar( &pygmyRFSPI, RF_WRITE|RF_REG_STATUS, BIT6|BIT5|BIT4 );
}

void rfWriteAddress( u8 ucReg, u8 *ucAddress )
{
    u8 i;

    // ucAddress is presumed to be default 5 byte mode
    //spiPutBuffer( &pygmyRFSPI, RF_WRITE|ucReg, ucAddress, 5 );
    RF_CS_LOW;
    
    spiWriteByte( &pygmyRFSPI, RF_WRITE|ucReg );
    for( i = 0; i < 5; i++ ){
        spiWriteByte( &pygmyRFSPI, ucAddress[ 4 - i ] ); // byte order must be inverted
    } // for
    RF_CS_HIGH;
}

//---------------------------------------End RF Receive---------------------------------------
//--------------------------------------------------------------------------------------------

