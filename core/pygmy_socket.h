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

//#pragma once
#ifndef __PYGMY_HEADER_SOCKET
	#define __PYGMY_HEADER_SOCKET
#include "pygmy_profile.h"
//#include "pygmy_file.h"

#define SOCKET_CMD_MASK         0xE0
#define SOCKET_SCAN             0
#define SOCKET_OPEN             0x20    
#define SOCKET_CLOSE            0x40
#define SOCKET_DATA             0x60
#define SOCKET_ACK              0x80
#define SOCKET_NACK             0xA0
 
#define SOCKET_TYPE_MASK        0x7F    
#define SOCKET_TX               0x80
#define SOCKET_RX               0x00
  
#define SOCKET_BLANK            0 // 
#define SOCKET_COMLINK          1 // Com Link
#define SOCKET_FILE             2 // File TX Request
#define SOCKET_AVSTREAM         3 // AV Streaming Data         
#define SOCKET_CONTROL          4 // Control/Movement
#define SOCKET_CMDLINE          5 

typedef struct {
                #ifdef __PYGMY_PFATLITE
                PYGMYFILE File;
                #else
                PYGMYFILE *File;
                #endif // __PYGMY_PFATLITE
                //PYGMYFUNC Action;
                u32 StartTime;
                u32 LastActive;
                u32 ExpireTime;
                u32 DestID;
                u32 SrcID;
                u8 Command;
                u8 Type;
                u8 Chunk;
                u8 Len;
                u8 Retry;
                u8 MaxRetries;
                u8 PayloadLen;
                u8 *Buffer;     // Socket Code assumes Buffer is dynamically allocated
                u8 BufferLen;
                u8 BufferIndex;
                u8 *Name;       // Socket Code assumes Name is dynamically allocated
                u8 Payload[ 32 ];
                u8 CR;
                void (*Run)(u8*, u16);
                } PYGMYSOCKET;

typedef struct {
                u32 DestID; // socket is LSB nibble
                u32 SrcID; // socket is LSB nibble
                u16 CRC;
                u8 Command;
                u8 Len;
                u8 Chunk;
                u8 Type;
                u8 Payload[ 25 ];
                } PYGMYPACKET;
                

void threadSocketMonitor( void );
u32 socketGetID( void );
u8 socketHandler( u8 *ucBuffer );//PYGMYPACKET *pygmyPacket );
void socketInit( void );
void socketList( void );
void socketBuildPayload( PYGMYSOCKET *pygmySocket, u8 ucCommand, u8 *ucBuffer, u8 ucLen );
PYGMYSOCKET *socketGet( u32 ulDestID, u32 ulSrcID );
void socketCopy( PYGMYSOCKET *fromSocket, PYGMYSOCKET *toSocket );
void socketClose( PYGMYSOCKET *pygmySocket );
PYGMYSOCKET *socketOpenFromPacket( PYGMYPACKET *pygmyPacket );
PYGMYSOCKET *socketOpen( u32 ulDestID, u8 ucType );
void socketFile( PYGMYSOCKET *pygmySocket, u8 *ucFileName, u8 ucTX );
u8 socketSendFile( u32 ulDestID, u8 *ucFileName );
u8 socketRequestFile( u32 ulDestID, u8 *ucFileName );
u8 socketSaveData( PYGMYSOCKET *pygmySocket, PYGMYPACKET *pygmyPacket );
void socketResend( PYGMYSOCKET *pygmySocket );
void socketSendClose( PYGMYSOCKET *pygmySocket );
void socketSendData( PYGMYSOCKET *pygmySocket );
void socketSendAck( PYGMYSOCKET *pygmySocket );
void socketSendNack( PYGMYSOCKET *pygmySocket );
void socketSendDataFromString( PYGMYSOCKET *pygmySocket, u8 *ucString );
u8 socketSendControl( u32 ulDestID, u8 *ucTaskName );
u8 socketRequestControl( u32 ulDestID, u8 *ucTaskName );
void socketControl( PYGMYSOCKET *pygmySocket, u8 *ucTaskName, u8 ucTX );
u8 socketRequestCommandLine( u32 ulDestID );
u8 socketOpenCommandLine( u32 ulDestID );
void socketCommandLine( PYGMYSOCKET *pygmySocket, u8 ucTX );
void socketPayload( PYGMYSOCKET *pygmySocket, u8 ucCommand, u8 *ucBuffer, u8 ucLen );

#endif // __PYGMY_HEADER_SOCKET

