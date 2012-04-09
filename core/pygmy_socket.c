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
#include "pygmy_socket.h"

PYGMYSOCKET *globalSockets;
u8 globalSocketCount;
u32 globalSocketID;

//--------------------------------------------------------------------------------------------
//---------------------------------------Socket Funcs-----------------------------------------

u32 socketGetID( void )
{
    return( globalSocketID );
}

u8 putsSOCKET( u8 *ucBuffer )
{
    //PYGMYSOCKET *pygmySocket;
    
}

void socketInit( void )
{
    u8 i, *strID;

    strID = sysGetID( );
    pdiaEncode( 0, PDIA_NEW, &globalSocketID );
    for( i = 0; i < 12; i++ ){
        pdiaEncode( strID[ i ], PDIA_ADD, &globalSocketID );
    }
    pdiaEncode( 0, PDIA_END, &globalSocketID );
    globalSockets = sysAllocate( sizeof( PYGMYSOCKET ) );
    globalSocketCount = 0; 
    taskNewSimple( "sockets", 5000, (void*)threadSocketMonitor );
}

PYGMYSOCKET *socketOpen( u32 ulDestID, u8 ucType )
{
    PYGMYPACKET pygmyPacket;

    pygmyPacket.DestID  = ulDestID;
    pygmyPacket.SrcID = 0;
    pygmyPacket.Type    = ucType;
    //print( COM3, "\rSocket Opening 0x%08X", ulDestID );

    return( socketOpenFromPacket( &pygmyPacket ) );
}

PYGMYSOCKET *socketOpenFromPacket( PYGMYPACKET *pygmyPacket )
{
    PYGMYSOCKET *tmpSocket;

    PYGMY_WATCHDOG_REFRESH;
    
    if( socketGet( pygmyPacket->DestID, pygmyPacket->SrcID ) ){
        return( NULL );
    } // if
    tmpSocket = sysReallocate( globalSockets, sizeof( PYGMYSOCKET ) * ( globalSocketCount + 1 ) );
    if( !tmpSocket ){
        return( NULL );
        
    } // if
    
    globalSockets = tmpSocket;
    tmpSocket->StartTime    = timeGet();
    tmpSocket->LastActive   = tmpSocket->StartTime;
    tmpSocket->ExpireTime   = tmpSocket->StartTime + 10;
    tmpSocket->DestID       = pygmyPacket->DestID;
    tmpSocket->SrcID        = ( globalSocketID & 0xFFFFFF00 ) | ( tmpSocket->DestID & 0x000000FF );
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
    ++globalSocketCount;

    return( tmpSocket );
}

void socketResend( PYGMYSOCKET *pygmySocket )
{
    PYGMY_WATCHDOG_REFRESH;
    if( pygmySocket->PayloadLen < 33 ){
        rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
    } // if
}

void threadSocketMonitor( void )
{
    u8 i;

    for( i = 0; i < globalSocketCount; i++ ){
        if( ( globalSockets[ i ].CR & BIT0 ) || 
            ( globalSockets[ i ].Type == RF_CMDLINE && globalSockets[ i ].Command == RF_ACK ) ){
            continue;
        } // if
        if( globalSockets[ i ].Retry == 0xFF || globalSockets[ i ].Retry >= globalSockets[ i ].MaxRetries ||
            globalSockets[ i ].LastActive >= globalSockets[ i ].ExpireTime ){
            socketClose( &globalSockets[ i ] );
            //print( COM3, "\rClosed Socket" );
        } else if( timeGet() > globalSockets[ i ].LastActive + 3 ){
            ++globalSockets[ i ].Retry;
            socketResend( &globalSockets[ i ] );
        } // else
    } // for
}


u8 socketHandler( u8 *ucBuffer )
{
    PYGMYPACKET pygmyPacket;
    PYGMYSOCKET *pygmySocket;
    u32 ulID;
    u16 i, uiCRC;

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
        #ifdef __PYGMYMESSAGES
        } else if( ( pygmyPacket.Type & RF_TYPE_MASK ) == RF_CONTROL ){
            //print( COM3, "\rReceived Control " );
            if( pygmyPacket.Type & RF_TX){
                //print( COM3, "Request" );
                // File requested, opened for WRITE by originating device, open local for READ
                if( taskIsRunning( pygmyPacket.Payload, 0 ) ){
                    //print( COM3, "\rTask Found" );
                    // Send message requesting data from task
                    // Task must call socketSendDataFromString to complete transaction
                    msgSend( pygmyPacket.Payload, PYGMY_UNMARKEDID, "TX", pygmySocket->DestID );
                    //print( COM3, "\rMessage sent to task" );
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
        #endif // __PYGMYMESSAGES
        #ifdef __PYGMYCOMMANDS
        } else if( ( pygmyPacket.Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            pygmySocket->Buffer = sysAllocate( 64 ); // This is the base starting point, buffer will expand as needed
            pygmySocket->BufferLen = 64;
            if( pygmySocket->Buffer ){
                //print( COM3, "\rAllocated Command Line Buffer" );
                socketSendAck( pygmySocket );
            } else{
                //print( COM3, "\rMem Fail Allocating Command Line Buffer" );
                socketSendClose( pygmySocket );
                return( 0 );
            } // else
        #endif // __PYGMYCOMMANDS
        } // else if      
    } // if
    
    pygmySocket = socketGet( pygmyPacket.DestID, pygmySocket->SrcID );
    if( !pygmySocket ){
        return( 0 );
    } // if

    if( ( pygmyPacket.Command & RF_CMD_MASK ) == RF_CLOSE ){
        //print( COM3, "\rData Transfer Complete" );
        socketSendAck( pygmySocket );
        socketClose( pygmySocket );
    } else if( ( pygmyPacket.Command & RF_CMD_MASK ) == RF_DATA ){
        //print( COM3, "\rRF_DATA" );
        if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            if( !socketSaveData( pygmySocket, &pygmyPacket ) ){
                //print( COM3, "\rMem full" );
                socketSendClose( pygmySocket );
            } else{
                socketSendAck( pygmySocket );
            } // else
        #ifdef __PYGMYMESSAGES
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CONTROL ){
            //print( COM3, "\rData Received: %s", ucBuffer );
            msgSend( pygmySocket->Name, PYGMY_UNMARKEDID, "RX", convertStringToInt( pygmyPacket.Payload ) );
            socketSendClose( pygmySocket );
        #endif // __PYGMYMESSAGES
        #ifdef __PYGMYCOMMANDS
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            //print( COM3, "\rData Received: " );
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
        #endif // __PYGMYCOMMANDS
        } // else if  
    } else if( ( pygmyPacket.Command & RF_CMD_MASK )== RF_ACK ){  
        pygmySocket->CR |= BIT0;
        //print( COM3, "\rAck Received" );
        //print( COM3, "\rCommand: 0x%02X", pygmySocket->Command & RF_CMD_MASK );
        if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_CLOSE ){
            // Local socket is closed only after ACK is received in response to CLOSE
            //print( COM3, "\rRemote Socket Closed" );
            socketClose( pygmySocket );
            return( 0 ); 
        } // if
        if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_FILE ){
            //print( COM3, "\rReceived ACK %d", pygmySocket->Chunk );
            socketSendData( pygmySocket );
        #ifdef __PYGMYMESSAGES
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CONTROL ){
            msgSend( pygmyPacket.Payload, PYGMY_UNMARKEDID, "TX", pygmySocket->DestID );
        #endif // __PYGMYMESSAGES
        #ifdef __PYGMYCOMMANDS
        } else if( ( pygmySocket->Type & RF_TYPE_MASK ) == RF_CMDLINE ){
            //print( COM3, "\rSocket Opened" );
        #endif // __PYGMYCOMMANDS
        } // else if
    } else if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_NACK ){
        socketResend( pygmySocket );
    } else if( ( pygmySocket->Command & RF_CMD_MASK ) == RF_SCAN ){
        //print( COM3, "\rRF_SCAN" );
    } // else if
    
    return( 1 );
}

#ifdef __PYGMYCOMMANDS
void socketList( void )
{
    PYGMYSOCKET *tmpSocket;
    u8 i;
    
    print( COM3, "\rOpen Sockets:" );
    for( i = 0; i < globalSocketCount; i++ ){
        tmpSocket =  &globalSockets[ i ];
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
#endif // __PYGMYCOMMANDS

PYGMYSOCKET *socketGet( u32 ulDestID, u32 ulSrcID )
{
    // Match may be made on either Src or Dest, they are a pair
    // unique to eachother. If only one is used, the other value
    // should be 0
    u8 i;

    PYGMY_WATCHDOG_REFRESH;
    for( i = 0; i < globalSocketCount; i++ ){
        if( globalSockets[ i ].DestID == ulDestID || globalSockets[ i ].DestID == ulSrcID||
            globalSockets[ i ].SrcID == ulDestID || globalSockets[ i ].SrcID == ulSrcID ){
            return( &globalSockets[ i ] );
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
    if( globalSocketCount ){
        tmpSockets = sysAllocate( sizeof( PYGMYSOCKET ) * ( globalSocketCount - 1 ) );
        if( tmpSockets ){
            --globalSocketCount;
            for( i = 0; i < globalSocketCount; i++ ){
                socketCopy( &globalSockets[ i ], &tmpSockets[ i ] );
            } // for
            sysFree( globalSockets );
            globalSockets = tmpSockets;
        } // if
    }
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
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_FILE);   
    ulID = ( globalSocketID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucFileName++);
    } // for
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    pygmySocket->PayloadLen = 13+ucLen;
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
}

u8 socketSaveData( PYGMYSOCKET *pygmySocket, PYGMYPACKET *pygmyPacket )
{
    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->Chunk = pygmyPacket->Chunk + 1;
    
    return( filePutBuffer( &pygmySocket->File, pygmyPacket->Len, pygmyPacket->Payload ) );
}

u8 socketSendFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    if( fileOpen( &pygmyFile, ucFileName, READ ) ){
        pygmySocket = socketOpen( ulDestID, RF_FILE );
        if( pygmySocket ){ 
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            socketFile( pygmySocket, ucFileName, 0 );
            
            return( TRUE );
        } // if
    } // if
    
    return( FALSE );
}

u8 socketRequestFile( u32 ulDestID, u8 *ucFileName )
{
    PYGMYSOCKET *pygmySocket;
    PYGMYFILE pygmyFile;

    PYGMY_WATCHDOG_REFRESH;
    
    if( fileOpen( &pygmyFile, ucFileName, WRITE ) ){
        pygmySocket = socketOpen( ulDestID, RF_TX|RF_FILE );
        if( pygmySocket ){ 
            fileCopyHandle( &pygmyFile, &pygmySocket->File );
            socketFile( pygmySocket, ucFileName, RF_TX ); // TX on recipient side

            return( 1 );
        } // if
    } // if
    
    return( 0 );
}

u8 socketSendControl( u32 ulDestID, u8 *ucTaskName )
{
    PYGMYSOCKET *pygmySocket;

    PYGMY_WATCHDOG_REFRESH;
    if( !taskIsRunning( ucTaskName, 0 ) ){
        return( FALSE );
    } // if
    pygmySocket = socketOpen( ulDestID, RF_RX|RF_CONTROL );
    if( pygmySocket ){ 
        socketControl( pygmySocket, ucTaskName, RF_RX ); // TX on recipient side
        
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
    pygmySocket = socketOpen( ulDestID, RF_RX|RF_CMDLINE );
    if( pygmySocket ){ 
        pygmySocket->Buffer = sysAllocate( 64 ); // This is base starting point, buffer will expand as needed
        pygmySocket->BufferLen = 64;
        if( pygmySocket->Buffer ){
            socketCommandLine( pygmySocket, RF_RX ); // TX on recipient side
        } else{
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
    u8 ucLen;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->Command = RF_OPEN;
    //ucLen = len( ucTaskName )+1;
    pygmySocket->LastActive = timeGet();
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_CMDLINE);   
    ulID = ( globalSocketID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
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

    PYGMY_WATCHDOG_REFRESH;
    if( !taskIsRunning( ucTaskName, 0 ) ){
        return( FALSE );
    } // if
    pygmySocket = socketOpen( ulDestID, RF_TX|RF_CONTROL );
    if( pygmySocket ){ 
        socketControl( pygmySocket, ucTaskName, RF_TX ); // TX on recipient side
        
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
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );

    *(pygmySocket->Payload+4) = (u8) ( RF_OPEN|(ucLen+5) ); // len of filename plus null and 32 bit ID
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    *(pygmySocket->Payload+6) = (ucTX|RF_CONTROL);   
    ulID = ( globalSocketID & 0xFFFFFF00 ) | ( pygmySocket->DestID & 0x000000FF );
    convertU32ToBuffer( ulID, pygmySocket->Payload+7, BIGENDIAN );
    
    for( i = 0; i < ucLen; i++ ){
        *(pygmySocket->Payload+11+i) = *(ucTaskName++);
    } // for
    uiCRC = sysCRC16( pygmySocket->Payload, 11+ucLen );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+11+ucLen, BIGENDIAN );
    pygmySocket->PayloadLen = 13+ucLen;
    rfPutTXBuffer( 13+ucLen, pygmySocket->Payload );
}

void socketBuildPayload( PYGMYSOCKET *pygmySocket, u8 ucCommand, u8 *ucBuffer, u8 ucLen )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->Len = ucLen;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->LastActive = timeGet();
    pygmySocket->Command = ucCommand;
    pygmySocket->PayloadLen = pygmySocket->Len + 8;
    convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );
    *(pygmySocket->Payload+4) = (u8) ( RF_CLOSE );
    *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    for( i = 0; i < pygmySocket->Len; i++ ){
        *(pygmySocket->Payload+6+i) = *(ucBuffer++);
    } // for
    uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
}

void socketSendClose( PYGMYSOCKET *pygmySocket )
{
    u16 uiCRC;

    //PYGMY_WATCHDOG_REFRESH;
    //pygmySocket->CR &= ~BIT0;
    //pygmySocket->LastActive = timeGet();
    // pygmySocket->Command = RF_CLOSE;
    //convertU32ToBuffer( pygmySocket->DestID, pygmySocket->Payload , BIGENDIAN );

    // *(pygmySocket->Payload+5) = (u8) pygmySocket->Chunk;
    // *(pygmySocket->Payload+4) = (u8) ( RF_CLOSE );
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
    //print( COM3, "\rSending Data From String: " );
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
    
    //uiCRC = sysCRC16( pygmySocket->Payload, pygmySocket->Len + 6 );
    //convertU16ToBuffer( uiCRC, pygmySocket->Payload+pygmySocket->Len + 6, BIGENDIAN );
    pygmySocket->PayloadLen = pygmySocket->Len + 8;
    
    rfPutTXBuffer( pygmySocket->PayloadLen, pygmySocket->Payload );
}

void socketSendData( PYGMYSOCKET *pygmySocket )
{
    u16 uiCRC;

    PYGMY_WATCHDOG_REFRESH;
    pygmySocket->CR &= ~BIT0;
    pygmySocket->LastActive = timeGet();
    pygmySocket->Len = fileGetBuffer( &pygmySocket->File, 24, pygmySocket->Payload+6 );
    pygmySocket->Command = RF_DATA; 
    if( pygmySocket->Len == 0 ){
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

