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

PYGMYSOCKET *globalRFSockets;
u32 globalRFConfig;
PYGMYSPIPORT pygmyRFSPI;
u8 globalRFSocketCount, *globalRFSubNetMask;
const u8 ucAddrP0[] = {0x78,0x78,0x78,0x78,0x78};
u32 globalRFID;

//--------------------------------------------------------------------------------------------
//---------------------------------------Socket Funcs-----------------------------------------

u32 socketGetID( void )
{
    return( globalRFID );
}

u8 putsSOCKET( u8 *ucBuffer )
{
    PYGMYSOCKET *pygmySocket;
    
    //pygmySocket = 
}

void socketInit( void )
{
    globalRFSockets = sysAllocate( sizeof( PYGMYSOCKET ) );
    globalRFSocketCount = 0; 
    taskNewSimple( "sockets", 5000, (void*)threadSocketMonitor );
}

PYGMYSOCKET *socketOpen( u32 ulDestID, u8 ucType )
{
    PYGMYPACKET pygmyPacket;

    pygmyPacket.DestID  = ulDestID;
    pygmyPacket.SrcID = 0;
    pygmyPacket.Type    = ucType;
    print( COM3, "\rSocket Opening 0x%08X", ulDestID );

    return( socketOpenFromPacket( &pygmyPacket ) );
}

PYGMYSOCKET *socketOpenFromPacket( PYGMYPACKET *pygmyPacket )
{
    PYGMYSOCKET *tmpSocket;
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    /*for( i = 0; i < globalRFSocketCount; i++ ){
        if( globalRFSockets[ i ].DestID == pygmyPacket->DestID ){
            break;
        } // if
    } // for
    */
    
    
    //if( i != globalRFSocketCount ){
    //    tmpSocket = &globalRFSockets[ i ];
    //} else{
    if( socketGet( pygmyPacket->DestID, pygmyPacket->SrcID ) ){
        return( NULL );
    } // if
    tmpSocket = sysReallocate( globalRFSockets, sizeof( PYGMYSOCKET ) * ( globalRFSocketCount + 1 ) );
    if( !tmpSocket ){
        return( NULL );
        
    } // if
    //} // else
    //tmpSocket = &globalRFSockets[ globalRFSocketCount ];
    globalRFSockets = tmpSocket;
    tmpSocket->StartTime    = timeGet();
    tmpSocket->LastActive   = tmpSocket->StartTime;
    tmpSocket->ExpireTime   = tmpSocket->StartTime + 10;
    tmpSocket->DestID       = pygmyPacket->DestID;
    tmpSocket->SrcID        = ( globalRFID & 0xFFFFFF00 ) | ( tmpSocket->DestID & 0x000000FF );
    tmpSocket->Command      = pygmyPacket->Command;
    tmpSocket->Type         = pygmyPacket->Type;
    tmpSocket->Chunk        = 0;
    tmpSocket->Retry        = 0;
    tmpSocket->MaxRetries   = 4;
    tmpSocket->Buffer       = NULL;
    tmpSocket->BufferLen    = 0;
    tmpSocket->BufferIndex  = 0;
    tmpSocket->CR           = 0;
    if( pygmyPacket->Type == RF_CONTROL ){
        tmpSocket->Name = sysAllocate( len( pygmyPacket->Payload ) + 1 );
        if( tmpSocket->Name ){
            copyString( pygmyPacket->Payload, tmpSocket->Name );
        } // if
    } else{
        tmpSocket->Name = NULL;
    } // else
    ++globalRFSocketCount;

    return( tmpSocket );
}

void socketResend( PYGMYSOCKET *pygmySocket )
{
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    if( pygmySocket->PayloadLen < 33 ){
        rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
    } // if
}

void threadSocketMonitor( void )
{
    u8 i;

    for( i = 0; i < globalRFSocketCount; i++ ){
        if( ( globalRFSockets[ i ].CR & BIT0 ) || 
            ( globalRFSockets[ i ].Type == RF_CMDLINE && globalRFSockets[ i ].Command == RF_ACK ) ){
            continue;
        } // if
        print( COM3, "\rMonitor" );
        if( globalRFSockets[ i ].Retry == 0xFF || globalRFSockets[ i ].Retry >= globalRFSockets[ i ].MaxRetries ||
            globalRFSockets[ i ].LastActive >= globalRFSockets[ i ].ExpireTime ){
            socketClose( &globalRFSockets[ i ] );
            print( COM3, "\rClosed Socket" );
        } else if( timeGet() > globalRFSockets[ i ].LastActive + 3 ){
            ++globalRFSockets[ i ].Retry;
            socketResend( &globalRFSockets[ i ] );
            print( COM3, "\rPacket Resent %d of %d", globalRFSockets[ i ].Retry, globalRFSockets[ i ].MaxRetries );
        } // else
    } // for
}


u8 socketHandler( u8 *ucBuffer )//PYGMYPACKET *pygmyPacket )
{
    PYGMYPACKET pygmyPacket;
    PYGMYSOCKET *pygmySocket;
    u32 ulID;
    u16 i, uiCRC;
    u8 *ucPtr;

    PYGMY_WATCHDOG_REFRESH;
    pygmyPacket.DestID     = convertBufferToU32( ucBuffer );
    pygmyPacket.Command    = ( ( *( ucBuffer + 4 ) ) & 0xE0 );
    pygmyPacket.Len        = ( *(ucBuffer + 4 ) & 0x1F );
    pygmyPacket.Chunk      = *( ucBuffer + 5 );
    pygmyPacket.CRC        = convertBufferToU16( ( ucBuffer + 6 + pygmyPacket.Len ) );
    
    uiCRC = sysCRC16( ucBuffer, pygmyPacket.Len + 6 );
    if( uiCRC != pygmyPacket.CRC ){ 
        return;
    } // else
    ucBuffer = ucBuffer + 6;
    if( pygmyPacket.Command == RF_OPEN ){
        pygmyPacket.Type = *(ucBuffer++);
        pygmyPacket.SrcID = convertBufferToU32( ucBuffer );
        ucBuffer += 4;
    } // if
        
    for( i = 0; i < pygmyPacket.Len; i++ ){
        pygmyPacket.Payload[ i ] = ucBuffer[ i ]; 
    } // for
    // Now decode packet
    
    if( ( pygmyPacket.Command & RF_CMD_MASK ) == RF_OPEN ){
        ulID = pygmyPacket.DestID;
        pygmyPacket.DestID = pygmyPacket.SrcID;
        pygmySocket = socketOpenFromPacket( &pygmyPacket );
        if( !pygmySocket ) {
            return( 0 );
        } //
        pygmySocket->SrcID = ulID;
        pygmySocket->LastActive = timeGet();
        pygmySocket->Type = pygmyPacket.Type;
        if( ( pygmyPacket.Type & RF_TYPE_MASK ) == RF_FILE ){
            if( pygmyPacket.Type & RF_TX){
                // File requested, opened for WRITE by originating device, open local for READ
                if( fileOpen( &pygmySocket->File, pygmyPacket.Payload, READ ) ){
                    socketSendData( pygmySocket );
                } else{
                    socketSendClose( pygmySocket );
                    return( 0 );
                } // else
            } else{
                // File incoming, opened for READ by originating device, open local for WRITE
                if( fileOpen( &pygmySocket->File, pygmyPacket.Payload, WRITE ) ){
                    socketSendAck( pygmySocket );
                } else{
                    socketSendClose( pygmySocket );
                    return( 0 );
                } // else
            } // else
        } else if( ( pygmyPacket.Type & RF_TYPE_MASK ) == RF_CONTROL ){
            print( COM3, "\rReceived Control " );
            if( pygmyPacket.Type & RF_TX){
                print( COM3, "Request" );
                // File requested, opened for WRITE by originating device, open local for READ
                if( taskIsRunning( pygmyPacket.Payload, 0 ) ){
                    print( COM3, "\rTask Found" );
                    // Send message requesting data from task
                    // Task must call socketSendDataFromString to complete transaction
                    msgSend( pygmyPacket.Payload, PYGMY_UNMARKEDID, "TX", pygmySocket->DestID );
                    print( COM3, "\rMessage sent to task" );
                } else{
                    socketSendClose( pygmySocket );
                    return( 0 );
                } // else
            } else{
                
                // Command Incoming, check for requested task
                if( taskIsRunning( pygmyPacket.Payload, 0 ) ){
                    socketSendAck( pygmySocket );
                } else{
                    // Task was not found
                    socketSendClose( pygmySocket );
                    return( 0 );
                } // else
            } // else
        } else if( ( pygmyPacket.Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            pygmySocket->Buffer = sysAllocate( 64 ); // This is the base starting point, buffer will expand as needed
            pygmySocket->BufferLen = 64;
            if( pygmySocket->Buffer ){
                print( COM3, "\rAllocated Command Line Buffer" );
                socketSendAck( pygmySocket );
            } else{
                print( COM3, "\rMem Fail Allocating Command Line Buffer" );
                socketSendClose( pygmySocket );
                return( 0 );
            } // else
        } // else if      
    } // if
    
    pygmySocket = socketGet( pygmyPacket.DestID, pygmySocket->SrcID );
    if( !pygmySocket ){
        return( 0 );
    } // if

    if( ( pygmyPacket.Command & RF_CMD_MASK ) == RF_CLOSE ){
        print( COM3, "\rData Transfer Complete" );
        socketSendAck( pygmySocket );
        socketClose( pygmySocket );
    } else if( ( pygmyPacket.Command & RF_CMD_MASK ) == RF_DATA ){
        //print( COM3, "\rRF_DATA" );
        if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            if( !socketSaveData( pygmySocket, &pygmyPacket ) ){
                print( COM3, "\rMem full" );
                socketSendClose( pygmySocket );
            } else{
                socketSendAck( pygmySocket );
            } // else
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CONTROL ){
            print( COM3, "\rData Received: %s", ucBuffer );
            msgSend( pygmySocket->Name, PYGMY_UNMARKEDID, "RX", convertStringToInt( pygmyPacket.Payload ) );
            socketSendClose( pygmySocket );
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            print( COM3, "\rData Received: " );
            if( ( pygmyPacket.Len + pygmySocket->BufferIndex ) > pygmySocket->BufferLen ){
                ucPtr = sysReallocate( pygmySocket->Buffer, pygmyPacket.Len + pygmySocket->BufferIndex );
                if( !ucPtr ){
                    // Mem fail!
                    socketSendClose( pygmySocket );
                    sysFree( pygmySocket->Buffer );
                    return( 0 );
                } // if
                // If memory was reallocated with success, update Buffer pointer
                pygmySocket->Buffer = ucPtr;
            } // if
            for( i = 0; i < pygmyPacket.Len; i++ ){
                if( pygmyPacket.Payload[ i ] == '\r' ){
                    // If carriage return reached, terminate buffer and pass to cmdExecute
                    pygmySocket->Buffer[ pygmySocket->BufferIndex ] = '\0';
                    pygmySocket->BufferIndex = 0; // Reset for next transfer
                    cmdExecute( pygmySocket->Buffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
                    break;
                } else{
                    // Copy Payload to Buffer
                    pygmySocket->Buffer[ pygmySocket->BufferIndex++ ] = pygmyPacket.Payload[ i ];
                } // else
            } // for
            socketSendAck( pygmySocket );
        } // else if
    } else if( ( pygmyPacket.Command & RF_CMD_MASK )== RF_ACK ){  
        pygmySocket->CR |= BIT0;
        print( COM3, "\rAck Received" );
        print( COM3, "\rCommand: 0x%02X", pygmySocket->Command & RF_CMD_MASK );
        if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
            // Local socket is closed only after ACK is received in response to CLOSE
            print( COM3, "\rRemote Socket Closed" );
            socketClose( pygmySocket );
            return( 0 ); 
        } // if
        if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            //print( COM3, "\rReceived ACK %d", pygmySocket->Chunk );
            /*if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
                // Local socket is closed only after ACK is received in response to CLOSE
                print( COM3, "\rRemote Socket Closed" );
                //rfListSockets();
                socketClose( pygmySocket );
            } else{ */
                socketSendData( pygmySocket );
            //} // else
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CONTROL ){
            /*if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
                // Local socket is closed only after ACK is received in response to CLOSE
                print( COM3, "\rRemote Socket Closed" );
                socketClose( pygmySocket );
            } else{*/
                print( COM3, "\rSending message for some reason" );
                msgSend( pygmyPacket.Payload, PYGMY_UNMARKEDID, "TX", pygmySocket->DestID );
            //} // else
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            print( COM3, "\rSocket Opened" );
            /*if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
                // Local socket is closed only after ACK is received in response to CLOSE
                print( COM3, "\rRemote Socket Closed" );
                socketClose( pygmySocket );
            } // if*/
        } // else if
    } else if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_NACK ){
        socketResend( pygmySocket );
    } else if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_SCAN ){
        //print( COM3, "\rRF_SCAN" );
    } // else if
    
    return( 1 );
}

void socketList( void )
{
    PYGMYSOCKET *tmpSocket;
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
        print( COM3, "\r\tRetry:\t%d", tmpSocket->Retry );
        
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

PYGMYSOCKET *socketGet( u32 ulDestID, u32 ulSrcID )
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

void socketCopy( PYGMYSOCKET *fromSocket, PYGMYSOCKET *toSocket )
{
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    fileCopyHandle( &fromSocket->File, &toSocket->File );
    //toSocket->Action        = fromSocket->Action;
    toSocket->StartTime     = fromSocket->StartTime;
    toSocket->LastActive    = fromSocket->LastActive;
    toSocket->ExpireTime    = fromSocket->ExpireTime;
    toSocket->DestID        = fromSocket->DestID;
    toSocket->SrcID         = fromSocket->SrcID;
    toSocket->Command       = fromSocket->Command;
    toSocket->Type          = fromSocket->Type;
    toSocket->Chunk         = fromSocket->Chunk;
    toSocket->Len           = fromSocket->Len;
    toSocket->Retry         = fromSocket->Retry;
    toSocket->MaxRetries    = fromSocket->MaxRetries;
    toSocket->PayloadLen    = fromSocket->PayloadLen;
    toSocket->Buffer        = fromSocket->Buffer;
    toSocket->BufferLen     = fromSocket->BufferLen;
    toSocket->BufferIndex   = fromSocket->BufferIndex;
    toSocket->CR            = fromSocket->CR;
    for( i = 0; i < 32; i++ ){
        toSocket->Payload[ i ] = fromSocket->Payload[ i ];
    } // for
}

void socketClose( PYGMYSOCKET *pygmySocket )
{
    PYGMYSOCKET *tmpSockets;
    u8 i;
    
    //print( COM3, "\rClosing Local Socket" );
    PYGMY_WATCHDOG_REFRESH;
    if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
        //print( COM3, "\r\tClosing File" );
        fileClose( &pygmySocket->File );
    } // if
    pygmySocket->DestID = 0;
    pygmySocket->SrcID = 0;
    if( pygmySocket->Name ){
        // Test and free name if used
        sysFree( pygmySocket->Name );
    } // if
    if( pygmySocket->Buffer ){
        // Test and free name if used
        sysFree( pygmySocket->Buffer );
    } // if
    if( globalRFSocketCount ){
        //print( COM3, "\r\tAllocating..." );
        tmpSockets = sysAllocate( sizeof( PYGMYSOCKET ) * ( globalRFSocketCount - 1 ) );
        if( tmpSockets ){
            //print( COM3, "\r\tCopying sockets" );
            --globalRFSocketCount;
            for( i = 0; i < globalRFSocketCount; i++ ){
                socketCopy( &globalRFSockets[ i ], &tmpSockets[ i ] );
            } // for
            sysFree( globalRFSockets );
            globalRFSockets = tmpSockets;
        } // if
    }
    //print( COM3, "\r\tConnection Closed" );
}


void socketFile( PYGMYSOCKET *pygmySocket, u8 *ucFileName, u8 ucTX )
{
    u32 ulID;
    u16 uiCRC;
    u8 i, ucLen;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->Command = RF_OPEN;
    ucLen = len( ucFileName )+1;
    pygmySocket->LastActive = timeGet();
    print( COM3, "\rTimeStamp: %t", pygmySocket->LastActive );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    print( COM3, "\rBuffered ID" );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_FILE);   
    ulID = ( globalRFID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucFileName++);
    } // for
    print( COM3, "\rBuilt Payload" );
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    print( COM3, "\rAppended CRC" );
    pygmySocket->PayloadLen = 13+ucLen;
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
    print( COM3, "\rTX Payload Complete" );
}

u8 socketSaveData( PYGMYSOCKET *pygmySocket, PYGMYPACKET *pygmyPacket )
{
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->Chunk = pygmyPacket->Chunk + 1;
    
    return( filePutBuffer( &pygmySocket->File, pygmyPacket->Len, pygmyPacket->Payload ) );
}

u8 socketSendFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    //print( COM3, "\rPreparing to send %s", ucFileName );
    PYGMY_WATCHDOG_REFRESH;
    if( fileOpen( &pygmyFile, ucFileName, READ ) ){
        //print( COM3, "\rFile Opened" );
        pygmySocket = socketOpen( ulDestID, RF_FILE );
        if( pygmySocket ){ 
            //print( COM3, "\rSocket Opened 0x%X", pygmySocket->DestID );
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            socketFile( pygmySocket, ucFileName, 0 );
            
            return( 1 );
        } // if
    } // if
    
    return( 0 );
}

u8 socketRequestFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    
    if( fileOpen( &pygmyFile, ucFileName, WRITE ) ){
        print( COM3, "\rOpening Socket..." );
        pygmySocket = socketOpen( ulDestID, RF_TX|RF_FILE );
        if( pygmySocket ){ 
            print( COM3, "Socket Opened" );
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            print( COM3, "\rFile Handle Copied for %s", ucFileName );
            socketFile( pygmySocket, ucFileName, RF_TX ); // TX on recipient side
            print( COM3, "\rsocketFile Sent" );
            return( 1 );
        } // if
    } // if
    
    return( 0 );
}

u8 socketSendControl( u32 ulDestID, u8 *ucTaskName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    if( !taskIsRunning( ucTaskName, 0 ) ){
        print( COM3, "\rTask %s not running", ucTaskName );
        return( FALSE );
    } // if
    print( COM3, "\rOpening Socket..." );
    pygmySocket = socketOpen( ulDestID, RF_RX|RF_CONTROL );
    if( pygmySocket ){ 
        print( COM3, "Socket Opened" );
        socketControl( pygmySocket, ucTaskName, RF_RX ); // TX on recipient side
        print( COM3, "\rControl Request Sent" );
        
        return( TRUE );
    } // if

    return( FALSE );
}

u8 socketRequestCommandLine( u32 ulDestID )
{
    
}

u8 socketOpenCommandLine( u32 ulDestID )
{
    PYGMYSOCKET *pygmySocket;

    PYGMY_WATCHDOG_REFRESH;
    print( COM3, "\rOpening Socket..." );
    pygmySocket = socketOpen( ulDestID, RF_RX|RF_CMDLINE );
    if( pygmySocket ){ 
        print( COM3, "Socket Opened" );
        pygmySocket->Buffer = sysAllocate( 64 ); // This is base starting point, buffer will expand as needed
        pygmySocket->BufferLen = 64;
        if( pygmySocket->Buffer ){
            
            socketCommandLine( pygmySocket, RF_RX ); // TX on recipient side
            print( COM3, "\rCommand Line Open Sent" );
        } else{
            print( COM3, "\rMemory Full\rClosing Socket" );
            socketClose( pygmySocket );
        } // else
    
        return( TRUE );
    } // if

    return( FALSE );
}

void socketCommandLine( PYGMYSOCKET *pygmySocket, u8 ucTX )
{
    u32 ulID;
    u16 uiCRC;
    u8 i, ucLen;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->Command = RF_OPEN;
    //ucLen = len( ucTaskName )+1;
    pygmySocket->LastActive = timeGet();
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_CMDLINE);   
    ulID = ( globalRFID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    /*for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucTaskName++);
    } // for
    */
    ucLen = 0;
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    pygmySocket->PayloadLen = 13+ucLen;
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
    //print( COM3, "\rTX Payload Complete" );
}

u8 socketRequestControl( u32 ulDestID, u8 *ucTaskName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    if( !taskIsRunning( ucTaskName, 0 ) ){
        //print( COM3, "\rTask %s not running", ucTaskName );
        return( FALSE );
    } // if
    //print( COM3, "\rOpening Socket..." );
    pygmySocket = socketOpen( ulDestID, RF_TX|RF_CONTROL );
    if( pygmySocket ){ 
        //print( COM3, "Socket Opened" );
        socketControl( pygmySocket, ucTaskName, RF_TX ); // TX on recipient side
        //print( COM3, "\rControl Request Sent" );
        
        return( TRUE );
    } // if

    return( FALSE );
}

void socketControl( PYGMYSOCKET *pygmySocket, u8 *ucTaskName, u8 ucTX )
{
    u32 ulID;
    u16 uiCRC;
    u8 i, ucLen;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->Command = RF_OPEN;
    ucLen = len( ucTaskName )+1;
    pygmySocket->LastActive = timeGet();
    //print( COM3, "\rTimeStamp: %t", pygmySocket->LastActive );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    //print( COM3, "\rBuffered ID" );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_CONTROL);   
    ulID = ( globalRFID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucTaskName++);
    } // for
    //print( COM3, "\rBuilt Payload" );
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    //print( COM3, "\rAppended CRC" );
    pygmySocket->PayloadLen = 13+ucLen;
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
    //print( COM3, "\rTX Payload Complete" );
}

void socketSendClose( PYGMYSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    //print( COM3, "\rSending Close Request" );
    pygmySocket->LastActive = timeGet();
    //print( COM3, "\r\tTimestamp" );
    pygmySocket->Command = RF_CLOSE;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+4) = (u8) ( RF_CLOSE );
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload + 6, BIGENDIAN );
    pygmySocket->PayloadLen = 8;
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
    
} 
/*
void socketSendControlResponse( PYGMYSOCKET *pygmySocket, u8 *ucTaskName, u8 *ucString )
{
    u16 i, uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    print( COM3, "\rSending Data From String: " );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Len = len( ucString ) + 1;
    if( pygmySocket->Len > 22 ){
        return;
    } // if
    pygmySocket->Command = RF_DATA; 
    if( pygmySocket->Len == 0 ){
        socketSendClose( pygmySocket );
        return;
    } // if
    for( i = 0; i < pygmySocket->Len; i++ ){
        *(pygmySocket->Payload+6+i) = *(ucString++);
    } // for
    *(pygmySocket->Payload+4) = (u8)( RF_DATA | pygmySocket->Len );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk++;
    
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
    pygmySocket->PayloadLen = pygmySocket->Len + 8;
    
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}*/

void socketSendDataFromString( PYGMYSOCKET *pygmySocket, u8 *ucString )
{
    u16 i, uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    print( COM3, "\rSending Data From String: " );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Len = len( ucString )+1;
    if( pygmySocket->Len > 22 ){
        return;
    } // if
    pygmySocket->Command = RF_DATA; 
    if( pygmySocket->Len == 0 ){
        socketSendClose( pygmySocket );
        return;
    } // if
    for( i = 0; i < pygmySocket->Len; i++ ){
        *(pygmySocket->Payload+6+i) = *(ucString++);
    } // for
    *(pygmySocket->Payload+4) = (u8)( RF_DATA | pygmySocket->Len );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk++;
    
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
    pygmySocket->PayloadLen = pygmySocket->Len + 8;
    
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}

void socketSendData( PYGMYSOCKET *pygmySocket )
{
    u16 i, uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    //print( COM3, "\rSending Data: " );
    pygmySocket->LastActive = timeGet();
    pygmySocket->Len = fileGetBuffer( &pygmySocket->File, 24, pygmySocket->Payload+6 );
    pygmySocket->Command = RF_DATA; 
    if( pygmySocket->Len == 0 ){
        // *(pygmySocket->Payload+4) = (u8) RF_CLOSE;
        socketSendClose( pygmySocket );
        return;
    } // if
    *(pygmySocket->Payload+4) = (u8)( RF_DATA | pygmySocket->Len );
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk++;
    
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
    pygmySocket->PayloadLen = pygmySocket->Len + 8;
    
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}

void socketSendAck( PYGMYSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->LastActive = timeGet();
    pygmySocket->Command = RF_ACK;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_ACK );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload + 6, BIGENDIAN );
    pygmySocket->PayloadLen = 8;
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}

void socketSendNack( PYGMYSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->LastActive = timeGet();
    pygmySocket->Command = RF_NACK;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+4) = (u8) ( RF_NACK );
    uiCRC = sysCRC16( pygmySocket->Payload, 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+ 6, BIGENDIAN );
    pygmySocket->PayloadLen = 8;
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}

//--------------------------------------------------------------------------------------------
//-----------------------------------------RF Receive-----------------------------------------
void rfRX( void )
{
    //static u16 uiRecurse = 0;
    PYGMYPACKET pygmyPacket;
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
        
        socketHandler( ucBuffer );//&pygmyPacket );

        rfClearStatus();
    } //if
    if( ucStatus & BIT5 ){
        rfSetRX();
    } // if 
    //print( COM3, "\rRL %d", uiRecurse );
    // uiRecurse = 0;
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
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RETR );
    spiWriteByte( &pygmyRFSPI, 0x0F ); // 15 retries, 250 microsec delay between tries
    RF_CS_HIGH;
    //rfSetTXPower( RF_2MBPS, 0 );
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
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_SETUP_RF );
    spiWriteByte( &pygmyRFSPI, ( ucDataRate | ucPower ) );
    RF_CS_HIGH;
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

    print( COM3, "\rClocking out %d bytes\r", uiLen);
    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    print( COM3, "\r" );
    for( i = 0; i < uiLen; i++ ){
        print( COM3, " 0x%2X", *ucBuffer );
        /*
        // The following is only used for raw transfers over 32 bytes
        if( i == 32 ){
            RF_CS_HIGH;
            while( !( rfGetStatus() & BIT5 ) ){;}
            rfClearStatus();
            i = 0;
            RF_CS_LOW;
            spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
        } // if
        */
        spiWriteByte( &pygmyRFSPI, *(ucBuffer++) ); 
    } // for
    RF_CS_HIGH;
    for( i = 0; i < 40 && !( rfGetStatus() & BIT5 ); i++ ){
        // Default set by rfInit is 15 tries at 250 microseconds delay between
        delay( 100 );
    } // for
    //if( i == 40 ){
        // If link is lost then Flush TX to prevent TX lock
        // High level socket protocol may retry later as appropriate
        rfFlushTX( );
    //} // if
    //while( !( rfGetStatus() & BIT5 ) ){;}
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

//---------------------------------------End RF Receive---------------------------------------
//--------------------------------------------------------------------------------------------

