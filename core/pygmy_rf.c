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
// [ DESTID 4B ][ PORT 1B ][ PCOUNT 1B ][ DATA 25B ][ CRC 1B ]
// 

PYGMYRFSOCKET *globalRFSockets;//[ RF_MAXSOCKETS ];
u32 globalRFConfig;
PYGMYSPIPORT pygmyRFSPI;
u8 globalRFSocketCount, *globalRFSubNetMask;
const u8 ucAddrP0[] = {0x78,0x78,0x78,0x78,0x78};
u32 globalRFID;
//u8 globalRFTXPayload[ 32 ];

u32 rfGetID( void )
{
    return( globalRFID );
}

//--------------------------------------------------------------------------------------------
//-----------------------------------------RF Receive-----------------------------------------
void rfRX( void )
{
    //static u16 uiRecurse = 0;
    PYGMYRFPACKET pygmyPacket;
    u16 uiCRC;
    u8 i, ucLen, ucStatus, ucPipe, *ucBuffer, ucPayload[ 32 ];
    
    PYGMY_WATCHDOG_REFRESH;
    //++uiRecurse;
    ucBuffer = ucPayload;
    ucStatus = rfGetStatus();
    ucPipe = ( ucStatus & 0x0E ) >> 1;
    if( ucStatus & BIT6 ){
        // Payload Received
        if( ucPipe == 7 ){
            return;
        } // if
        ucLen = rfGetRXPayloadLen( ); // Payloads are a maximum of 32 bytes
        //print( COM3, "\rLoading RF Buffer of %d bytes\r", ucLen );
        // Now load payload into buffer
        RF_CS_LOW;
        spiWriteByte( &pygmyRFSPI, RF_RX_PAYLOAD );
        for( i = 0; i < ucLen; i++ ){
            ucBuffer[ i ] = spiReadByte( &pygmyRFSPI );
            delay( 100 );
            //print( COM3, " %X", ucBuffer[ i ] );
        } // if
        RF_CS_HIGH;
        // Payload loaded, now fill out packet
        //print( COM3, "\rPacket Received:" );
        pygmyPacket.DestID     = convertBufferToU32( ucBuffer );
        //print( COM3, "\r\tDestID: %X", pygmyPacket.DestID );
        pygmyPacket.Command    = ( ( *( ucBuffer + 4 ) ) & 0xE0 ); // >> 5
        //print( COM3, "\r\tCommand: %d", pygmyPacket.Command );
        pygmyPacket.Len        = ( *(ucBuffer + 4 ) & 0x1F );
        //print( COM3, "\r\tPayload Len: %d", pygmyPacket.Len );
        pygmyPacket.Chunk      = *( ucBuffer + 5 );
        //print( COM3, "\r\tChunk: %d", pygmyPacket.Chunk );
        pygmyPacket.CRC        = convertBufferToU16( ( ucBuffer + 6 + pygmyPacket.Len ) );
    
        uiCRC = sysCRC16( ucBuffer, pygmyPacket.Len + 6 );
        //print( COM3, "\r\tCalculated CRC: %X for %d bytes", uiCRC, pygmyPacket.Len );
        if( uiCRC != pygmyPacket.CRC ){
            //print( COM3, "\r\tPacket CRC %X does not match %X", pygmyPacket.CRC, uiCRC ); 
            return;
        } // else
        ucBuffer = ucBuffer + 6;
        if( pygmyPacket.Command == RF_OPEN ){
            pygmyPacket.Type = *(ucBuffer++);
            //print( COM3, "\r\tType: %d", pygmyPacket.Type );
            pygmyPacket.SrcID = convertBufferToU32( ucBuffer );
            //print( COM3, "\r\tSRCID: %X", pygmyPacket.SrcID );
            ucBuffer += 4;
        } // if
        
        for( i = 0; i < pygmyPacket.Len; i++ ){
            pygmyPacket.Payload[ i ] = ucBuffer[ i ]; 
        } // for
        // Now decode packet
        
        rfSocketHandler( &pygmyPacket );

        rfClearStatus();
    } //if
    if( ucStatus & BIT5 ){
        rfSetRX();
    } // if 
    //print( COM3, "\rRL %d", uiRecurse );
    // uiRecurse = 0;
}

u8 rfSocketHandler( PYGMYRFPACKET *pygmyPacket )
{
    PYGMYRFSOCKET *pygmySocket;
    u32 ulID;

    PYGMY_WATCHDOG_REFRESH;
    if( ( pygmyPacket->Command & RF_CMD_MASK ) == RF_OPEN ){
        ulID = pygmyPacket->DestID;
        pygmyPacket->DestID = pygmyPacket->SrcID;
        pygmySocket = rfOpenSocketFromPacket( pygmyPacket );
        if( !pygmySocket ) {
            return( 0 );
        } //
        pygmySocket->SrcID = ulID;
        pygmySocket->LastActive = timeGet();
        if( ( pygmyPacket->Type & RF_TYPE_MASK ) == RF_FILE ){
            if( pygmyPacket->Type & RF_TX){
                // File requested, opened for WRITE by originating device, open local for READ
                if( fileOpen( &pygmySocket->File, pygmyPacket->Payload, READ ) ){
                    //print( COM3, "\rFile READ transfer for %s started...", pygmyPacket->Payload );
                    rfSendData( pygmySocket );
                } else{
                    //print( COM3, "\rFile READ transfer for %s failed", pygmyPacket->Payload );
                    rfSendClose( pygmySocket );
                    return( 0 );
                } // else
            } else{
                // File incoming, opened for READ by originating device, open local for WRITE
                if( fileOpen( &pygmySocket->File, pygmyPacket->Payload, WRITE ) ){
                    //rfListSockets();
                    //print( COM3, "\rFile WRITE transfer for %s started...", pygmyPacket->Payload );
                    rfSendAck( pygmySocket );
                } else{
                    //print( COM3, "\rFile WRITE transfer for %s failed", pygmyPacket->Payload );
                    rfSendClose( pygmySocket );
                    return( 0 );
                } // else
            } // else
        } // else      
    } // if
    
    pygmySocket = rfGetSocket( pygmyPacket->DestID, pygmySocket->SrcID );
    if( !pygmySocket ){
        return( 0 );
    } // if

    if( ( pygmyPacket->Command & RF_CMD_MASK ) == RF_CLOSE ){
        print( COM3, "\rData Transfer Complete" );
        rfSendAck( pygmySocket );
        rfCloseSocket( pygmySocket );
    } else if( ( pygmyPacket->Command & RF_CMD_MASK ) == RF_DATA ){
        //print( COM3, "\rRF_DATA" );
        if( !rfSaveData( pygmySocket, pygmyPacket ) ){
            print( COM3, "\rMem full" );
            rfSendClose( pygmySocket );
        } else{
            rfSendAck( pygmySocket );
        } // else
    } else if( ( pygmyPacket->Command & RF_CMD_MASK )== RF_ACK ){   
        if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            //print( COM3, "\rReceived ACK %d", pygmySocket->Chunk );
            if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
                // Local socket is closed only after ACK is received in response to CLOSE
                print( COM3, "\rRemote Socket Closed" );
                //rfListSockets();
                rfCloseSocket( pygmySocket );
            } else{ 
                rfSendData( pygmySocket );
            } // else
        } 
    } else if( ( pygmyPacket->Command & RF_CMD_MASK ) == RF_NACK ){
        //print( COM3, "\rRF_NACK" );
        rfResend( pygmySocket );
    } else if( ( pygmyPacket->Command & RF_CMD_MASK ) == RF_SCAN ){
        //print( COM3, "\rRF_SCAN" );
    } // else if
    
    return( 1 );
}

//---------------------------------------End RF Receive---------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//---------------------------------------Socket Funcs-----------------------------------------

void rfInitSockets( void )
{
    globalRFSockets = sysAllocate( sizeof( PYGMYRFSOCKET ) );
    globalRFSocketCount = 0; // was 1
}

/*
u32 rfGetNextSocketID( void )
{
    u8 i;
    
    for( i = 0; i < globalRFSocketCount; i++ ){
        globalRFSockets[ i ].tmp
    } // for
}*/

void rfListSockets( void )
{
    PYGMYRFSOCKET *tmpSocket;
    u8 i;
    
    print( COM3, "\rOpen Sockets:" );
    for( i = 0; i < globalRFSocketCount; i++ ){
        tmpSocket =  &globalRFSockets[ i ];
        print( COM3, "\r\rDest,Src: 0x%08X,0x%08X", tmpSocket->DestID, tmpSocket->SrcID );
        print( COM3, "\r\tStart:\t%t", tmpSocket->StartTime );
        print( COM3, "\r\tActive:\t%t", tmpSocket->LastActive );
        print( COM3, "\r\tType:\t0x%02X", tmpSocket->Type );
        print( COM3, "\r\tCommand:0x%02X", tmpSocket->Command );
        print( COM3, "\r\tChunk:\t0x%02X", tmpSocket->Chunk );
        print( COM3, "\r\tLen:\t0x%02X", tmpSocket->Len );
        
        if( ( tmpSocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            print( COM3, " %s", tmpSocket->File.Name );
            if( tmpSocket->Type & RF_TX ){
                print( COM3, " TX" );
            } else{
                print( COM3, " RX" );
            } // else
        } // if
    } //
}

PYGMYRFSOCKET *rfGetSocket( u32 ulDestID, u32 ulSrcID )
{
    // Match may be made on either Src or Dest, they are a pair
    // unique to eachother. If only one is used, the other value
    // should be 0
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    for( i = 0; i < globalRFSocketCount; i++ ){
        if( globalRFSockets[ i ].DestID == ulDestID || globalRFSockets[ i ].DestID == ulSrcID||
            globalRFSockets[ i ].SrcID == ulDestID || globalRFSockets[ i ].SrcID == ulSrcID ){
            //print( COM3, "\rFound Socket DestID: 0x%8X SrcID: 0x%8X", ulDestID, ulSrcID );
            return( &globalRFSockets[ i ] );
        } // if
    } // for

    return( NULL );
}

void rfCopySocket( PYGMYRFSOCKET *fromSocket, PYGMYRFSOCKET *toSocket )
{
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    fileCopyHandle( &fromSocket->File, &toSocket->File );
    //toSocket->Action        = fromSocket->Action;
    toSocket->StartTime     = fromSocket->StartTime;
    toSocket->LastActive    = fromSocket->LastActive;
    toSocket->DestID        = fromSocket->DestID;
    toSocket->SrcID         = fromSocket->SrcID;
    toSocket->Command       = fromSocket->Command;
    toSocket->Type          = fromSocket->Type;
    toSocket->Chunk         = fromSocket->Chunk;
    for( i = 0; i < 32; i++ ){
        toSocket->Payload[ i ] = fromSocket->Payload[ i ];
    } // for
}

void rfCloseSocket( PYGMYRFSOCKET *pygmySocket )
{
    PYGMYRFSOCKET *tmpSockets;
    u8 i;
    
    //print( COM3, "\rClosing Local Socket" );
    PYGMY_WATCHDOG_REFRESH;
    if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
        //print( COM3, "\r\tClosing File" );
        fileClose( &pygmySocket->File );
    } // if
    pygmySocket->DestID = 0;
    pygmySocket->SrcID = 0;
    if( globalRFSocketCount ){
        //print( COM3, "\r\tAllocating..." );
        tmpSockets = sysAllocate( sizeof( PYGMYRFSOCKET ) * ( globalRFSocketCount - 1 ) );
        if( tmpSockets ){
            //print( COM3, "\r\tCopying sockets" );
            --globalRFSocketCount;
            for( i = 0; i < globalRFSocketCount; i++ ){
                rfCopySocket( &globalRFSockets[ i ], &tmpSockets[ i ] );
            } // for
            sysFree( globalRFSockets );
            globalRFSockets = tmpSockets;
        } // if
    }
    //print( COM3, "\r\tConnection Closed" );
}

PYGMYRFSOCKET *rfOpenSocketFromPacket( PYGMYRFPACKET *pygmyPacket )
{
    PYGMYRFSOCKET *tmpSocket;
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    for( i = 0; i < globalRFSocketCount; i++ ){
        if( globalRFSockets[ i ].DestID == pygmyPacket->DestID ){
            break;
        } // if
    } // for
    
    if( i != globalRFSocketCount ){
        tmpSocket = &globalRFSockets[ i ];
    } else{
        tmpSocket = sysReallocate( globalRFSockets, sizeof( PYGMYRFSOCKET ) * ( globalRFSocketCount + 1 ) );
        if( tmpSocket ){
            globalRFSockets = tmpSocket;
        } // if
    } // else
    tmpSocket = &globalRFSockets[ globalRFSocketCount ];
    tmpSocket->StartTime    = timeGet();
    tmpSocket->LastActive   = tmpSocket->StartTime;
    tmpSocket->DestID       = pygmyPacket->DestID;
    tmpSocket->SrcID        = ( globalRFID & 0xFFFFFF00 ) | ( tmpSocket->DestID & 0x000000FF );
    tmpSocket->Command      = pygmyPacket->Command;
    tmpSocket->Type         = pygmyPacket->Type;
    tmpSocket->Chunk        = 0;
    ++globalRFSocketCount;
    //rfListSockets();

    return( tmpSocket );
}

PYGMYRFSOCKET *rfOpenSocket( u32 ulDestID, u8 ucType )
{
    PYGMYRFPACKET pygmyPacket;

    pygmyPacket.DestID  = ulDestID;
    pygmyPacket.Type    = ucType;
    
    return( rfOpenSocketFromPacket( &pygmyPacket ) );
}

void rfFile( PYGMYRFSOCKET *pygmySocket, u8 *ucFileName, u8 ucTX )
{
    u32 ulID;
    u16 uiCRC;
    u8 i, ucLen;

    PYGMY_WATCHDOG_REFRESH;
    ucLen = len( ucFileName )+1;
    pygmySocket->LastActive = timeGet();
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_FILE);   
    ulID = ( globalRFID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    
    for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucFileName++);
    } // for
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
}

u8 rfSaveData( PYGMYRFSOCKET *pygmySocket, PYGMYRFPACKET *pygmyPacket )
{
    u8 i;

    /*print( COM3, "\rSaving Data: " );
    for( i = 0; i < pygmyPacket->Len; i++ ){
        print( COM3, " 0x%2X", pygmyPacket->Payload[ i ] );
    } // for
    */
    /*if( pygmyPacket->Len == 0){
        return( 0 );
    } else{*/
    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->Chunk = pygmyPacket->Chunk + 1;
    if( pygmyPacket->Len < 24 ){
        print( COM3, "\r" );
        for( i = 0; i < pygmyPacket->Len; i++ ){
            print( COM3, " 0x%02X", pygmyPacket->Payload[i] );
        } // for
    } // if
    
    return( filePutBuffer( &pygmySocket->File, pygmyPacket->Len, pygmyPacket->Payload ) );
    //} // else
    
}

void rfResend( PYGMYRFSOCKET *pygmySocket )
{
    
}

u8 rfSendFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYRFSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    //print( COM3, "\rPreparing to send %s", ucFileName );
    PYGMY_WATCHDOG_REFRESH;
    if( fileOpen( &pygmyFile, ucFileName, READ ) ){
        //print( COM3, "\rFile Opened" );
        pygmySocket = rfOpenSocket( ulDestID, RF_FILE );
        if( pygmySocket ){ 
            //print( COM3, "\rSocket Opened 0x%X", pygmySocket->DestID );
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            rfFile( pygmySocket, ucFileName, 0 );
            
            return( 1 );
        } // if
    } // if
    
    return( 0 );
}

u8 rfRequestFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYRFSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    if( fileOpen( &pygmyFile, ucFileName, WRITE ) ){
        pygmySocket = rfOpenSocket( ulDestID, RF_TX|RF_FILE );
        if( pygmySocket ){ 
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            rfFile( pygmySocket, ucFileName, RF_TX ); // TX on recipient side
        
            return( 1 );
        } // if
    } // if
    
    return( 0 );
    
}

/*
void rfSendOpen( PYGMYRFSOCKET *pygmySocket )
{
    u16 uiCRC;

    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN );
    convertU32ToBuffer( globalRFID, pygmySocket->Payload+6, BIGENDIAN );
    
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 10 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload->Len + 6, BIGENDIAN );
    rfPutTXBuffer( 8, pygmySocket->Payload );
}*/

void rfSendClose( PYGMYRFSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    //print( COM3, "\rSending Close Request" );
    pygmySocket->LastActive = timeGet();
    //print( COM3, "\r\tTimestamp" );
    pygmySocket->Command = RF_CLOSE;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+4) = (u8) ( RF_CLOSE );
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload + 6, BIGENDIAN );
    //print( COM3, "\r\tSending Close Packet" );
    rfPutTXBuffer( 8, pygmySocket->Payload );
    
}

void rfSendData( PYGMYRFSOCKET *pygmySocket )
{
    u16 i, uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    //print( COM3, "\rSending Data: " );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Len = fileGetBuffer( &pygmySocket->File, 24, pygmySocket->Payload+6 );
    pygmySocket->Command = RF_DATA; 
    if( pygmySocket->Len == 0 ){
        // *(pygmySocket->Payload+4) = (u8) RF_CLOSE;
        rfSendClose( pygmySocket );
        return;
    } // if
    *(pygmySocket->Payload+4) = (u8)( RF_DATA | pygmySocket->Len );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk++;
    
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
    rfPutTXBuffer( pygmySocket->Len + 8, pygmySocket->Payload );
}

void rfSendAck( PYGMYRFSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    //print( COM3, "\rACK %d", pygmySocket->Chunk );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Command = RF_ACK;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_ACK );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload + 6, BIGENDIAN );
    rfPutTXBuffer( 8, pygmySocket->Payload );
}

void rfSendNack( PYGMYRFSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    //print( COM3, "\rNACK" );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Command = RF_NACK;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+4) = (u8) ( RF_NACK );
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+ 6, BIGENDIAN );
    rfPutTXBuffer( 8, pygmySocket->Payload );
}

void rfInit( void )
{
    u8 i, *strID;

    strID = sysGetID( );
    pdiaEncode( 0, PDIA_NEW, &globalRFID );
    for( i = 0; i < 12; i++ ){
        pdiaEncode( strID[ i ], PDIA_ADD, &globalRFID );
    }
    pdiaEncode( 0, PDIA_END, &globalRFID );
    spiConfig( &pygmyRFSPI, RF_CS, RF_SCK, RF_MISO, RF_MOSI );
    pinConfig( RF_EN, OUT );
    RF_EN_LOW;
    pinConfig( RF_IRQ, PULLUP );
    pinInterrupt( rfRX, RF_IRQ, TRIGGER_FALLING, 3 );

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

void rfPutTXBuffer( u16 uiLen, u8 *ucBuffer )
{
    u8 i;

    //print( COM3, "\rClocking out %d bytes\r", uiLen);
    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    //print( COM3, "\r" );
    for( i = 0; i < uiLen; i++ ){
        //print( COM3, " 0x%2X", *ucBuffer );
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

void rfClearStatus( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_STATUS );
    spiWriteByte( &pygmyRFSPI, BIT6|BIT5|BIT4 ); 
    RF_CS_HIGH;
}

void rfWriteAddress( u8 ucReg, u8 *ucAddress )
{
    u8 i;

    // ucAddress is presumed to be default 5 byte mode
    RF_CS_LOW;
    
    spiWriteByte( &pygmyRFSPI, RF_WRITE|ucReg );
    for( i = 0; i < 5; i++ ){
        spiWriteByte( &pygmyRFSPI, ucAddress[ 4 - i ] ); // byte order must be inverted
    }
    RF_CS_HIGH;
}

