#include "pygmy_sys.h"

#pragma once

#define PYGMY_WEB_MARK              0
#define PYGMY_WEB_HELLO             1
#define PYGMY_WEB_WELCOME           2
#define PYGMY_WEB_DECLINE           3
#define PYGMY_WEB_ACK               4
#define PYGMY_WEB_NACK              5
#define PYGMY_WEB_BYE               6
#define PYGMY_WEB_QUERY             7
#define PYGMY_WEB_DATA              8
#define PYGMY_WEB_CLASS0            0
#define PYGMY_WEB_CLASS1            BIT6
#define PYGMY_WEB_CLASS2            BIT7
#define PYGMY_WEB_CLASS3            (BIT7|BIT6)

#define PYGMY_WEB_MAXNODES              32

typedef struct{
                u32 ID;
                u8 Len;
                u8 Count;
                u8 Action;
                u8 MAC[ 12 ];
                u8 Buffer[ 26 ]; // 32 minus 4 for id, 1 for count, 1 for len
                PYGMYCMDPTR Process;
                } PYGMYWEBSOCKET;

typedef struct{
                u8 MAC[ 12 ];
                u8 Class;
                u8 Time;
                u8 Signal;
                } PYGMYWEBNODE;

typedef struct{
                PYGMYWEBNODE Nodes[ PYGMY_WEB_MAXNODES ];
                PYGMYWEBNODE Self;
                PYGMYCOMMANDQUEUE Commands;
                u32 Status;
                u8 Buffer[ 32 ];
                } PYGMYWEBSTACK;

u8 Pygmy_WEB_RX( u8 *ucParams ); 

u8 Pygmy_WEB_OpenSocket( PYGMYWEBSOCKET *pygmySocket );
void Pygmy_WEB_CloseSocket( PYGMYWEBSOCKET *pygmySocket );
u8 Pygmy_WEB_GetSocket( u32 uiID );
void Pygmy_WEB_InitSockets( void );
            
u8 Pygmy_WEB_MARK( u8 *ucParams );
u8 Pygmy_WEB_WELCOME( u8 *ucParams );
u8 Pygmy_WEB_HELLO( u8 *ucParams );
u8 Pygmy_WEB_SocketData( u8 *ucParams );
u8 Pygmy_WEB_DATA( u8 *ucParams );
u8 Pygmy_WEB_ACK( u8 *ucParams );
            
u8 Pygmy_WEB_TestBuffer( u8 *ucBuffer1, u8 *ucBuffer2, u16 uiLen );
void Pygmy_WEB_InitStack( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode );
u8 Pygmy_WEB_AddNode( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode );
u8 Pygmy_WEB_GetIsNodeInStack( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode );
void Pygmy_WEB_Send_MARK( void );
void Pygmy_WEB_Send_HELLO( void );
void Pygmy_WEB_Send_WELCOME( void );
void Pygmy_WEB_Send_DECLINE( void );
void Pygmy_WEB_Send_ACK( void );
void Pygmy_WEB_Send_NACK( void );
void Pygmy_WEB_Send_BYE( void );
void Pygmy_WEB_Send_QUERY( void );
void Pygmy_WEB_Send_DATA( void );
void Pygmy_WEB_SendSocketData( void );
u8 Pygmy_WEB_ReceiveFile( u8 *ucParams );
u8 Pygmy_WEB_SendFile( u8 *ucParams );

extern PYGMYCOMMANDQUEUE pygmyGlobalWEBQueue;
extern PYGMYWEBSTACK pygmyGlobalStack;
extern PYGMYWEBSOCKET pygmyGlobalSockets[];
extern const PYGMYCMD PYGMYCOMMANDS[];

