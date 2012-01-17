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

// WARNING! This file is Preliminary and still under initial construction!
// RF communicates through sockets, each socket is negotiated for one
// transaction between two devices. Once the transaction is completed,
// the socket should be closed.
// [ DESTID 4B ][ PORT 1B ][ PCOUNT 1B ][ DATA 25B ][ CRC 1B ]
// 

PYGMYRFSOCKET pygmyGlobalRFSockets[ RF_MAXSOCKETS ];
u32 globalRFConfig;
PYGMYSPIPORT pygmyRFSPI;
const u8 ucAddrP0[] = {0x78,0x78,0x78,0x78,0x78};
u32 pygmyGlobalRFID;
u8 globalRFTXPayload[ 32 ];

u32 rfGetID( void )
{
    return( pygmyGlobalRFID );
}

void rfGetRXPayload( u8 *ucBuffer )
{
    u8 i, ii, ucPipe, ucLen;
    
    ucPipe = rfGetStatus() & (BIT3|BIT2|BIT1) >> 1;
    if( ucPipe == 7 ){
        return;
    } // if
    
    while( !rfIsRXFIFOEmpty() ){
        ucLen = rfGetRXPayloadLen( );
        RF_CS_LOW;
        spiWriteByte( &pygmyRFSPI, RF_RX_PAYLOAD );
        for( i = 0; i < ucLen; i++ ){
            ucBuffer[ i ] = spiReadByte( &pygmyRFSPI );
        } // if
        ucBuffer[ i ] = '\0';
        RF_CS_HIGH;
        //print( COM3, ucBuffer );
        rfProcessPacket( ucBuffer, ucLen );
    } // while
    rfClearStatus();
}

void rfSendPacket( PYGMYRFPACKET *pygmyPacket )
{
    u8 i, ucIndex, ucBuffer[ 32 ];

    ucIndex = 0;
    ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->DestID >> 24 );
    ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->DestID >> 16 );
    ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->DestID >> 8 );
    ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->DestID ); 
    ucBuffer[ ucIndex++ ] = (u8) ( ( pygmyPacket->Command << 5 ) | pygmyPacket->Len );
    ucBuffer[ ucIndex++ ] = (u8) pygmyPacket->Chunk;
    if( pygmyPacket->Type == RF_OPEN ){
        ucBuffer[ ucIndex++ ] = (u8) pygmyPacket->Type;
        ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->SrcID >> 24 );
        ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->SrcID >> 16 );
        ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->SrcID >> 8 );
        ucBuffer[ ucIndex++ ] = (u8)( pygmyPacket->SrcID );
        
    } else{
        for( i = 0; i < pygmyPacket->Len; i++ ){
            ucBuffer[ ucIndex++ ] = (u8)pygmyPacket->Payload[ i ];
        } // for
    } // else
    
    print( COM3, "\rucIndex: %d", ucIndex );
    pygmyPacket->CRC = sysCRC16( ucBuffer, ucIndex );
    ucBuffer[ ucIndex++ ] = (u8)(pygmyPacket->CRC >> 8);
    ucBuffer[ ucIndex++ ] = (u8)pygmyPacket->CRC;
    print( COM3, "\rSending:" );
    for( i = 0; i < ucIndex; i++ ){
        print( COM3, " ( %X )", ucBuffer[ i ] );
    } // for
    rfPutTXBuffer( ucIndex, ucBuffer );
}

u8 rfLoadPacket( u8 *ucBuffer, PYGMYRFPACKET *pygmyPacket )
{
    // Unified Socket
    // CMDS[ CCC LLLLL ] MSB bits are command, LSB are Payload Len
    // RF_OPEN
    // RF_CLOSE
    // RF_NEXT
    // RF_LAST
    // RF_SCAN
    //
    // RF_FILE
    // RF_AVSTREAM
    // RF_COMLINK
    // RF_CONTROL
    // IDs = ( PDIASUM & 0xFFFFFF00 ) 
    // [ DESTID 3B PORT 1B ][ CMD 1B ][ CHUNK 1B ][ PAYLOAD 1-24B ][ CRC 2B ]

    // New Socket
        // [ DESTID 0xFF 4B ][ RF_OPEN 1B ][ 0x00 1B ][ RF_FILE 1B ][ SRCID 0xFF 4B ][ FILELEN 2B ][ FILENAME 0x00 1-12B ][ CRC 2B ]
        // [ DESTID 0xFF 4B ][ RF_OPEN 1B ][ 0x00 1B ][ RF_AVSTREAM 1B ][ SRCID 0xFF 4B ][ CRC 2B ]
        // [ DESTID 0xFF 4B ][ RF_OPEN 1B ][ 0x00 1B ][ RF_COMLINK 1B ][ SRCID 0xFF 4B ][ CRC 2B ]
        // [ DESTID 0xFF 4B ][ RF_OPEN 1B ][ 0x00 1B ][ RF_CONTROL 1B ][ SRCID 0xFF 4B ][ CRC 2B ]
    // Close Socket
        // [ DESTID PORT 4B ][ RF_CLOSE 1B ][ 0x00 1B ][ CRC 2B ]
    // New Socket Response
        // [ DESTID PORT 4B ][ RF_OPEN 1B ][ 0x00 1B ][ CRC 2B ]
    // New Socket Response or Ack/Nack Packet
        // [ DESTID PORT 4B ][ RF_NEXT/RF_LAST 1B ][ 0x00 1B ][ CRC 2B ]
    u8 i;
    
    // Load common sections
    pygmyPacket->DestID     = convertBufferToU32( ucBuffer );
    print( COM3, "\rDestID: %X", pygmyPacket->DestID );
    pygmyPacket->Command    = ( ( *( ucBuffer + 4 ) ) & 0xE0 ) >> 5;
    print( COM3, "\rCommand: %d", pygmyPacket->Command );
    pygmyPacket->Len        = ( *(ucBuffer + 4 ) & 0x1F );
    print( COM3, "\rPayload Len: %d", pygmyPacket->Len );
    pygmyPacket->Chunk      = *( ucBuffer + 5 );
    print( COM3, "\rChunk: %d", pygmyPacket->Chunk );
    pygmyPacket->CRC        = convertBufferToU16( ( ucBuffer + 6 + pygmyPacket->Len ) );
   
    if( sysCRC16( ucBuffer, pygmyPacket->Len + 6 ) != pygmyPacket->CRC ){
        print( COM3, "\rCRC %X does not match %X", pygmyPacket->CRC, sysCRC16( ucBuffer, pygmyPacket->Len + 6 ) ); 
        return( 0 );
    } // else
    for( i = 0; i < pygmyPacket->Len; i++ ){
        pygmyPacket->Payload[ i ] = ucBuffer[ 6 + i ]; 
    } // for
    if( pygmyPacket->Command == RF_OPEN ){
        pygmyPacket->Type       = pygmyPacket->Payload[ 0 ];
        print( COM3, "\rType: %d", pygmyPacket->Type );
        pygmyPacket->SrcID = convertBufferToU32( ( pygmyPacket->Payload + 1 ) );
        print( COM3, "\rSRCID: %X", pygmyPacket->SrcID );
        /*if( pygmyPacket->Type == RF_FILE ){
            for( i = 0; i < 13; i++ ){
                pygmyPacket->Payload[ i ] = pygmyPacket->Payload[ 5 + i ];
                if( pygmyPacket->Payload[ i ] == '\0' ){
                    break; // Check for NULL String Terminator
                } // if 
            } // for
            print( COM3, "\rFilename: %s", pygmyPacket->Payload );
        } // if
        */
    } // if

}

void rfProcessPacket( u8 *ucBuffer, u8 ucLen )
{    
    PYGMYRFPACKET pygmyPacket;

    rfLoadPacket( ucBuffer, &pygmyPacket );
    if( pygmyPacket.Command == RF_OPEN ){
        pygmyPacket.Command = RF_NEXT;
        pygmyPacket.DestID = pygmyPacket.SrcID;
        pygmyPacket.SrcID = pygmyGlobalRFID;
        if( rfOpenSocket( pygmyPacket.DestID, pygmyPacket.Type ) == 0xFF ) {
            print( COM3, "\rSockets Full" );
        } else{
            print( COM3, "\rSocket Open" );
            rfSendPacket( &pygmyPacket );
        } // else
    } else if( pygmyPacket.Command == RF_CLOSE ){
        
    } else if( pygmyPacket.Command == RF_NEXT ){
        
    } else if( pygmyPacket.Command == RF_LAST ){
        
    } else if( pygmyPacket.Command == RF_SCAN ){
        
    } // else if
    
}

void rfListSockets( void )
{
    u8 i, ucCmd;

    for( i = 0; i < RF_MAXSOCKETS; i++ ){
        if( !pygmyGlobalRFSockets[ i ].DestID ){
            continue;
        } // if
        print( COM3, "\rDestID\t\tType\tCount\tCR\tStartTime" );
        print( COM3, "\r%X\t", pygmyGlobalRFSockets[ i ].DestID );
        ucCmd = pygmyGlobalRFSockets[ i ].Type >> 5;
        if( ucCmd == RF_CONTROL ){
            print( COM3, "control" );
        } else if( ucCmd == RF_COMLINK ){
            print( COM3, "comlink" );
        } else if( ucCmd == RF_FILE ){
            print( COM3, "file" );
        } else if( ucCmd == RF_AVSTREAM ){
            print( COM3, "avstream" );
        } else{
            print( COM3, "blank" );
        } // else
        print( COM3, "\t%d", pygmyGlobalRFSockets[ i ].Chunk );
        print( COM3, "\t%2X", pygmyGlobalRFSockets[ i ].CR );
        print( COM3, "\t%t", pygmyGlobalRFSockets[ i ].StartTime);
    } // for
}

void rfInitSockets( void )
{
    u8 i;
    
    for( i = 0; i < RF_MAXSOCKETS; i++ ){
        rfCloseSocket( i );
    } // for
}

void rfCloseSocket( u8 ucSocket )
{
    pygmyGlobalRFSockets[ ucSocket ].DestID         = 0;
    pygmyGlobalRFSockets[ ucSocket ].Type           = 0;
    pygmyGlobalRFSockets[ ucSocket ].Chunk          = 0;
    pygmyGlobalRFSockets[ ucSocket ].CR             = 0;
    pygmyGlobalRFSockets[ ucSocket ].StartTime      = 0;
}

u8 rfFindSocket( u32 ulDest )
{
    u8 i;
    
    for( i = 0; i < RF_MAXSOCKETS; i++ ){
        if( ( pygmyGlobalRFSockets[ i ].DestID & 0xFFFFFF00 ) == ( ulDest & 0xFFFFFF00 ) ){
            return( i );
        } // if
    } // for
    
    return( 0xFF );
}

u8 rfOpenSocket( u32 ulDest, u8 ucType )
{
    u8 i;

    for( i = 0; i < RF_MAXSOCKETS; i++ ){
        if( !pygmyGlobalRFSockets[ i ].DestID ){
            pygmyGlobalRFSockets[ i ].DestID        = ulDest & 0xFFFFFF00;
            //pygmyGlobalRFSockets[ i ].SrcID         = pygmyGlobalRFID | i; // i is socket, LSB nibble
            pygmyGlobalRFSockets[ i ].Type          = ucType;
            pygmyGlobalRFSockets[ i ].Chunk         = 0;
            pygmyGlobalRFSockets[ i ].CR            = RF_CR_OPEN|RF_CR_PENDING;
            pygmyGlobalRFSockets[ i ].StartTime     = timeGet();
            //rfSendOpenSocket( ulDest, ucType );
            return( i );
        } // if
    } // for
    return( 0xFF );
}

void rfSendOpenCommand( u8 ucSocket )
{
    PYGMYRFPACKET pygmyPacket;
    
    pygmyPacket.DestID      = pygmyGlobalRFSockets[ ucSocket ].DestID;
    pygmyPacket.Type        = pygmyGlobalRFSockets[ ucSocket ].Type;
    pygmyPacket.SrcID       = pygmyGlobalRFID;
    pygmyPacket.Len         = 5;
    pygmyPacket.Command     = RF_OPEN;
    
    rfSendPacket( &pygmyPacket );
}

void rfSendCloseCommand( u8 ucSocket )
{
    PYGMYRFPACKET pygmyPacket;
    
    pygmyPacket.DestID      = pygmyGlobalRFSockets[ ucSocket ].DestID;
    pygmyPacket.Type        = pygmyGlobalRFSockets[ ucSocket ].Type;
    pygmyPacket.SrcID       = pygmyGlobalRFID;
    pygmyPacket.Len         = 5;
    pygmyPacket.Command     = RF_CLOSE;
    
    rfSendPacket( &pygmyPacket );
}

void rfSendNextCommand( u8 ucSocket )
{
    PYGMYRFPACKET pygmyPacket;
    
    pygmyPacket.DestID      = pygmyGlobalRFSockets[ ucSocket ].DestID;
    pygmyPacket.Type        = pygmyGlobalRFSockets[ ucSocket ].Type;
    pygmyPacket.SrcID       = pygmyGlobalRFID;
    pygmyPacket.Len         = 5;
    pygmyPacket.Command     = RF_NEXT;
    
    rfSendPacket( &pygmyPacket );
}

void rfSendLastCommand( u8 ucSocket )
{
    PYGMYRFPACKET pygmyPacket;
    
    pygmyPacket.DestID      = pygmyGlobalRFSockets[ ucSocket ].DestID;
    pygmyPacket.Type        = pygmyGlobalRFSockets[ ucSocket ].Type;
    pygmyPacket.SrcID       = pygmyGlobalRFID;
    pygmyPacket.Len         = 5;
    pygmyPacket.Command     = RF_LAST;
    
    rfSendPacket( &pygmyPacket );
}

void rfSendOpenSocket( u32 ulDest, u8 ucType  )
{
    PYGMYRFPACKET pygmyPacket;
    
    pygmyPacket.DestID      = ulDest;
    pygmyPacket.Type        = ucType;
    pygmyPacket.SrcID       = pygmyGlobalRFID;
    pygmyPacket.Len         = 5;
    pygmyPacket.Command     = RF_OPEN;
    
    rfSendPacket( &pygmyPacket );
    
    /*u16 uiCRC;
    u8 ucBuffer[ 32 ];

    //ucBuffer = globalRFTXPayload;
    ucBuffer[ 0 ] = ulDest >> 24;
    ucBuffer[ 1 ] = ulDest >> 16;
    ucBuffer[ 2 ] = ulDest >> 8;
    ucBuffer[ 3 ] = 0xFF; // No port defined yet
    if( ucType == RF_FILE ){
    
    } else if( ucType == RF_AVSTREAM ){
    
    } else if( ucType == RF_COMLINK ){
        ucBuffer[ 4 ] = RF_OPEN | RF_COMLINK;
        ucBuffer[ 5 ] = 0;
        uiCRC = sysCRC16( ucBuffer, 6 );
        ucBuffer[ 6 ] = (u8)uiCRC >> 8;
        ucBuffer[ 7 ] = (u8)uiCRC;
    } // else if
    rfPutTXBuffer( 8, ucBuffer );*/
}
/*
void rfCloseSocket( u8 ucSocket )
{
    pygmyGlobalRFSockets[ ucSocket ].DestID = 0;
}*/

void rfRX( void )
{
    u8 ucStatus, ucBuffer[ 40 ];
    
    ucStatus = rfGetStatus();
    
    if( ucStatus & BIT6 ){
        rfGetRXPayload( ucBuffer );
    } //if
    if( ucStatus & BIT5 ){
        rfSetRX();
    } // if 
}

void rfInit( void )
{
    u8 i, *strID;

    strID = sysGetID( );
    pdiaEncode( 0, PDIA_NEW, &pygmyGlobalRFID );
    for( i = 0; i < 12; i++ ){
        pdiaEncode( strID[ i ], PDIA_ADD, &pygmyGlobalRFID );
    }
    pdiaEncode( 0, PDIA_END, &pygmyGlobalRFID );
    spiConfig( &pygmyRFSPI, RF_CS, RF_SCK, RF_MISO, RF_MOSI );
    pinConfig( RF_EN, OUT );
    RF_EN_LOW;
    pinConfig( RF_IRQ, PULLUP );
    pinInterrupt( rfRX, RF_IRQ, TRIGGER_FALLING );

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL );
    RF_CS_HIGH;
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

u8 rfGetSignalQuality( void )
{
    u8 ucSignalQuality;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_READ|RF_REG_OBSERVETX );
    ucSignalQuality = 15 - ( RF_REG_OBSERVETX_ARCCNT & spiReadByte( &pygmyRFSPI ) ); 
    RF_CS_HIGH;
    
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
    rfClearStatus( );
    RF_EN_HIGH;
}

void rfFlushTX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_FLUSH );
    RF_CS_HIGH;
}

void rfFlushRX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_RX_FLUSH );
    RF_CS_HIGH;
}

void rfPutString( u8 *ucBuffer )
{
    u8 i;

    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    for( i = 0; *ucBuffer; i++ ){
        if( i == 32 ){
            RF_CS_HIGH;
            while( !( rfGetStatus() & BIT5 ) ){;}
            rfClearStatus();
            i = 0;
            RF_CS_LOW;
            spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
        } // if
        spiWriteByte( &pygmyRFSPI, *(ucBuffer++) ); 
    } // for
    RF_CS_HIGH;
    while( !( rfGetStatus() & BIT5 ) ){;}
    //rfFlushTX();
    rfClearStatus();
    rfSetRX();
}

void rfSocketPutString( u8 ucSocket, u8 *ucString )
{

}

void rfPutTXBuffer( u16 uiLen, u8 *ucBuffer )
{
    u8 i;
    
    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    for( i = 0; i < uiLen; i++ ){
        if( i == 32 ){
            RF_CS_HIGH;
            while( !( rfGetStatus() & BIT5 ) ){;}
            rfClearStatus();
            uiLen -= 32;
            RF_CS_LOW;
            spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
        } // if
        spiWriteByte( &pygmyRFSPI, ucBuffer[ i ] ); 
    } // for
    RF_CS_HIGH;
    while( !( rfGetStatus() & BIT5 ) ){;}
    rfClearStatus();
    rfSetRX();
}

u8 rfGetStatus( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    ucStatus = spiReadByte( &pygmyRFSPI );
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
}

u8 rfIsRXFIFOEmpty( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_REG_FIFOSTATUS );
    ucStatus = spiReadByte( &pygmyRFSPI ) & BIT0;
    RF_CS_HIGH;

    return( ucStatus );
}
/*
void rfConfigAsTX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_EN_CRC ); // Unique part of address
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    //spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    //spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL|RF_REG_FEATURE_EN_ACK_PAY );
    RF_CS_HIGH;
}*/

/*
void rfConfigAsRX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_PRIM_RX|RF_REG_CONFIG_EN_CRC ); // Unique part of address
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL|RF_REG_FEATURE_EN_ACK_PAY );
    RF_CS_HIGH;
}*/

void rfClearStatus( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_STATUS );
    spiWriteByte( &pygmyRFSPI, BIT6|BIT5|BIT4 ); 
    RF_CS_HIGH;
}

void rfWriteAddress( u8 ucReg, u8 *ucAddress )
{
    //const u8 ucAddrP0[] = {0xB3,0xB4,0xB5,0xB6,0x05};
    u8 i;

    // ucAddress is presumed to be default 5 byte mode
    RF_CS_LOW;
    
    spiWriteByte( &pygmyRFSPI, RF_WRITE|ucReg );
    for( i = 0; i < 5; i++ ){
        spiWriteByte( &pygmyRFSPI, ucAddress[ 4 - i ] ); // byte order must be inverted
    }
    RF_CS_HIGH;
}

/*
void rfSPIWriteByte( u8 ucByte )
{
	u16 i;
	
    RF_SCK_LOW;		            // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 8; i++ ){ 		// Clock out Write CMD
        if( ucByte & ( BIT7 >> i ) ){		
            RF_MOSI_HIGH;          // SlaveInMasterOut high if bit set
        } else{
            RF_MOSI_LOW;           // SlaveInMasterOut low if bit clear
        }
        RF_SCK_HIGH;				// clock must start low, transition high
        RF_SCK_LOW;			    // Low transition finishes clock sequence
    }
    
} 

void rfSPIWriteWord( u16 uiWord )
{
	u16 i;
	
    RF24_SCK_LOW;					    // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 16; i++ ){ 			// 
        if( uiWord & ( 0x8000 >> i ) ){
            RF24_MOSI_HIGH;              // SlaveInMasterOut high if bit set
        } else{ 		
            RF24_MOSI_LOW;               // SlaveInMasterOut low if bit clear
        }
        RF24_SCK_HIGH;				    // clock must start low, transition high
        RF24_SCK_LOW;				    // Low transition finishes clock sequence
    } 
} 

void rfSPIWriteLong( u32 ulLong )
{
	u16 i;

    RF24_SCK_LOW;					    // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 32; i++ ){ 			// Clock out Write CMD
        if( ulLong & ( 0x80000000 >> i ) ){
            RF24_MOSI_HIGH;              // SlaveInMasterOut high if bit set
        } else{ 		
            RF24_MOSI_LOW;               // SlaveInMasterOut low if bit clear
        }
        RF24_SCK_HIGH;				    // clock must start low, transition high
        RF24_SCK_LOW;				    // Low transition finishes clock sequence
    }
} 

u8 rfSPIReadByte( void )
{
    u16 i;
	u8 ucByte;
	
    RF24_SCK_LOW;
    for( i = 0, ucByte = 0; i < 8; i++ ){    
        RF24_SCK_HIGH;				        // clock must start low, transition high 			
        if( RF24_MISO_STATE ){                // Test port input for high and set bit in ucByte
            ucByte |= ( BIT7 >> i );
        }
        RF24_SCK_LOW;				        // Low transition finishes clock sequence
    }

	return( ucByte );
}

void rfTXPacket( void )
{

}

void rfRXPacket( void )
{
    
}*/
