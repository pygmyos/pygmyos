#include "pygmy_web.h"
#include "pygmy_sys.h"
#include "pygmy_file.h"
#include "pygmy_string.h"
#include "pygmy_nvic.h"
#include "rf24.h"

const u8 PYGMY_CMD_NEWLINE[] = "\r";
const u8 PYGMY_CMD_CRLF[] = {13,10,'\0'};
const u8 PYGMY_CMD_CMDPROMPT[] = "\r> ";
const u8 PYGMY_CMD_CTRLZ[] = {0x1A,'\0'};
const u8 PYGMY_CMD_ESC[] = {0x1B,'\0'};
const u8 PYGMY_CMD_NULL[] = "";

PYGMYCOMMANDQUEUE pygmyGlobalWEBQueue;
PYGMYWEBSTACK pygmyGlobalStack;
PYGMYWEBSOCKET pygmyGlobalSockets[ 16 ];

const PYGMYCMD PYGMYCOMMANDS[] = {
                                    "send", Pygmy_CMD_send,
                                    "receive", Pygmy_CMD_receive,
                                    "mark", Pygmy_WEB_MARK,
                                    "welcome", Pygmy_WEB_WELCOME,
                                    "hello", Pygmy_WEB_HELLO,
                                    //(u8*)PYGMY_CMD_config, Pygmy_CMD_config,
                                    "reset", Pygmy_CMD_reset,
                                    //(u8*)PYGMY_CMD_run, Pygmy_CMD_run,
                                    //(u8*)PYGMY_CMD_ps, Pygmy_CMD_ps,
                                    //(u8*)PYGMY_CMD_pipe, Pygmy_CMD_pipe,
                                    //(u8*)PYGMY_CMD_login, Pygmy_CMD_login,
                                    //(u8*)PYGMY_CMD_logout, Pygmy_CMD_logout,
                                    //(u8*)PYGMY_CMD_file, Pygmy_CMD_file,
                                    //(u8*)PYGMY_CMD_alarm, Pygmy_CMD_alarm,
                                    //(u8*)PYGMY_CMD_bootlock, Pygmy_CMD_bootlock,
                                    "", Pygmy_CMD_null, // No Commands after NULL
                                    };
         
u8 Pygmy_WEB_TestBuffer( u8 *ucBuffer1, u8 *ucBuffer2, u16 uiLen )
{
    u8 i;
    
    for( i = 0; i < uiLen; i++ ){
        if( ucBuffer1[ i ] != ucBuffer2[ i ] ){
            break;
        } // if
    } // for

    if( i == uiLen ){
        return( 1 );
    } // if

    return( 0 );
}

void Pygmy_WEB_InitSockets( void )
{
    u8 i, ii;
    
    for( i = 0; i < 16; i++ ){
        pygmyGlobalSockets[ i ].ID = 0;
        pygmyGlobalSockets[ i ].Len = 0;
        pygmyGlobalSockets[ i ].Process = ( void * )NULL;
        for( ii = 0; ii < 12; ii++ ){
            pygmyGlobalSockets[ i ].MAC[ ii ] = 0xFF;
        } // for 
    } // for
    
}

u8 Pygmy_WEB_GetSocket( u32 uiID )
{
    u8 i;

    for( i = 0; i < 16; i++ ){
        if( pygmyGlobalSockets[ i ].ID == uiID ){
            return( i );
        } // if
    } // for
    
    return( 0xFF );
}

void Pygmy_WEB_CloseSocket( PYGMYWEBSOCKET *pygmySocket )
{
    u8 i, uiSocket;

    uiSocket = Pygmy_WEB_GetSocket( pygmySocket->ID );
    if( uiSocket != 0xFF ){
        pygmyGlobalSockets[ uiSocket ].ID = 0;
        pygmyGlobalSockets[ uiSocket ].Len = 0;
        pygmyGlobalSockets[ uiSocket ].Process = ( void * )NULL;
        for( i = 0; i < 12; i++ ){
            pygmyGlobalSockets[ uiSocket ].MAC[ i ] = 0xFF;
        } // for 
    } // if
}

u8 Pygmy_WEB_OpenSocket( PYGMYWEBSOCKET *pygmySocket )
{
    u8 i, uiSocket;
    
    uiSocket = Pygmy_WEB_GetSocket( 0 ); // Get first empty socket
    if( uiSocket == 0xFF ){
        return( 0 ); // No room left for new socket
    } // if
    pygmyGlobalSockets[ uiSocket ].ID = pygmySocket->ID;
    pygmyGlobalSockets[ uiSocket ].Len = pygmySocket->Len;
    pygmyGlobalSockets[ uiSocket ].Process = pygmySocket->Process;
    for( i = 0; i < 12; i++ ){
        pygmyGlobalSockets[ uiSocket ].MAC[ i ] = pygmySocket->MAC[ i ];
    } // for 
    
    return( 1 );
}

u8 Pygmy_WEB_RX( u8 *ucParams )                                
{
    PYGMYWEBNODE pygmyNodeTo, pygmyNodeFrom;
    u8 ucBuffer[ 32 ], ucMAC[ 12 ], ucSocket[] = {0,0,0,1};
    u16 i;
    
    
    Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rRX Packet:\r\tFrom: " );
    for( i = 0; i < 12; i++ ){
        ucMAC[ i ] = 0xFF ;
        pygmyNodeFrom.MAC[ i ] = pygmyGlobalStack.Buffer[ i ];
        Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "2X", pygmyGlobalStack.Buffer[ i ] );
    } // for
    Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\r\tTo: " );
    for( ; i < 24; i++ ){
        pygmyNodeTo.MAC[ i - 12 ] = pygmyGlobalStack.Buffer[ i ];
        Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "2X", pygmyGlobalStack.Buffer[ i ] );
    } // for 
    //Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tSignal Quality: %d", Pygmy_RF24_GetSignalQuality( ) );
    if( Pygmy_WEB_TestBuffer( pygmyGlobalStack.Self.MAC, pygmyNodeTo.MAC, 12 ) || 
        Pygmy_WEB_TestBuffer( ucMAC, pygmyNodeTo.MAC, 12 ) ){
        if( pygmyGlobalStack.Buffer[ 29 ] == PYGMY_WEB_HELLO ){
            Pygmy_SYS_ReleaseCurrentCommand( &pygmyGlobalWEBQueue );
            Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tWELCOME" );
            Pygmy_WEB_WELCOME( "" );
        } else if( pygmyGlobalStack.Buffer[ 29 ] == PYGMY_WEB_MARK ){
            Pygmy_SYS_ReleaseCurrentCommand( &pygmyGlobalWEBQueue );
            Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tHELLO" );
            Pygmy_WEB_HELLO( "" );
        } else if( pygmyGlobalStack.Buffer[ 29 ] == PYGMY_WEB_WELCOME ){
            Pygmy_SYS_ReleaseCurrentCommand( &pygmyGlobalWEBQueue );
            Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tCONNECTED" );
            pygmyNodeFrom.Signal = 0xFF;
            pygmyNodeFrom.Class = 0;
            if( Pygmy_WEB_AddNode( &pygmyGlobalStack, &pygmyNodeFrom ) ){
                Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tNode is in stack" );
            } // if
            Pygmy_WEB_ACK( "" );
        } else if( pygmyGlobalStack.Buffer[ 29 ] ==  PYGMY_WEB_DATA ){
            
        } else if( pygmyGlobalStack.Buffer[ 29 ] == PYGMY_WEB_ACK ){
            Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r\tTransaction Complete" );
            Pygmy_SYS_ReleaseCurrentCommand( &pygmyGlobalWEBQueue );
        } // else if
    } else if( Pygmy_WEB_TestBuffer( pygmyGlobalStack.Buffer, ucSocket, 4 ) ){
        for( i = 0; i < pygmyGlobalStack.Buffer[ 5 ] && i < 32; i++ ){
            pygmyGlobalSockets[ 0 ].Buffer[ i ] = pygmyGlobalStack.Buffer[ i + 7 ];
        } // for
        pygmyGlobalSockets[ 0 ].Len = pygmyGlobalStack.Buffer[ 5 ];
        //ucBuffer[ i ] = '\0';
        //Pygmy_SYS_Print( PYGMY_SYS_COM3, "\r%s", ucBuffer );
        Pygmy_SYS_ReleaseCurrentCommand( &pygmyGlobalWEBQueue );
        pygmyGlobalSockets[ 0 ].Process( "" );
        //Pygmy_WEB_ACK( "" );
    } // else if
    return( 0 );
}
    
u8 Pygmy_WEB_AddNode( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode )
{
    u8 i, ii;
    
    if( Pygmy_WEB_GetIsNodeInStack( pygmyStack, pygmyNode ) ){
        return( 2 );
    } // return

    for( i = 0; i < PYGMY_WEB_MAXNODES; i++ ){
        if( !pygmyStack->Nodes[ i ].MAC[ 0 ] ){
            for( ii = 0; ii < 12; ii++ ){
                pygmyStack->Nodes[ i ].MAC[ ii ] = pygmyNode->MAC[ ii ];
            } // for
            pygmyStack->Nodes[ i ].Class = pygmyNode->Class;
            pygmyStack->Nodes[ i ].Signal = pygmyNode->Signal;
            //pygmyStack->Nodes[ i ].Status = 0;
            // ToDo: Add current Time // pygmyStack->Nodes[ i ].Time;
            return( 1 );
        } // if
    } // for

    return( 0 );
}

u8 Pygmy_WEB_GetIsNodeInStack( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode )
{
    u8 i, ii;
    
    for( i = 0; i < PYGMY_WEB_MAXNODES; i++ ){
        for( ii = 0; ii < 12; ii++ ){
            if( pygmyStack->Nodes[ i ].MAC[ ii ] != pygmyNode->MAC[ ii ] ){
                break;
            } // if
        }// for
        if( ii == 12 ){
            return( 1 );
        } // if
    } // for

    return( 0 );
}

u8 Pygmy_CMD_null( u8 *ucParams )
{
    return( 0 );
}                                

u8 Pygmy_CMD_reset( u8 *ucParams )
{
    // The Following should only be called after firmware update is saved in
    // file named boot.hex
    PYGMY_SYS_RESET;
    
    return( 1 );
}

u8 Pygmy_CMD_send( u8 *ucParams )
{
    u8 i, *ucSubString;
    
    ucSubString = Pygmy_STRING_GetNextSubString( ucParams, PYGMY_STRING_NEWLINE );
    if( ucSubString ){
        Pygmy_WEB_SocketData( ucSubString );
    } // if
    /*ucSubString = Pygmy_STRING_GetNextSubString( ucParams, PYGMY_STRING_SEPARATORS );
    if( ucSubString ){
        //Pygmy_RF24_SetTX();
        pygmyGlobalSockets[ i ].ID = 0x00000001;
        pygmyGlobalSockets[ i ].Len = Pygmy_STRING_StringLen( ucSubString );
        for( i = 0; i < pygmyGlobalSockets[ 0 ].Len; i++ ){
            pygmyGlobalSockets[ i ].Buffer[ i ] = ucSubString[ i ];
        } // for
        Pygmy_WEB_SocketData( "" );
        //Pygmy_RF24_PutTXBuffer( , pygmyGlobalSocket.Buffer);
    
    } // if
    */

    return( 1 );
}

u8 Pygmy_CMD_receive( u8 *ucParams )
{
    Pygmy_RF24_SetRX( );
    return( 1 );
}

u8 Pygmy_WEB_MARK( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;

    pygmyCommand.Name = "WEB_MARK";
    pygmyCommand.PrintHandler = Pygmy_WEB_Send_MARK;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 10;

    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );

    return( 1 );
}

u8 Pygmy_WEB_WELCOME( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;

    pygmyCommand.Name = "WEB_WELCOME";
    pygmyCommand.PrintHandler = Pygmy_WEB_Send_WELCOME;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 10;

    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );

    return( 1 );
}

u8 Pygmy_WEB_HELLO( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;

    pygmyCommand.Name = "WEB_HELLO";
    pygmyCommand.PrintHandler = Pygmy_WEB_Send_HELLO;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 10;

    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );

    return( 1 );
}

u8 Pygmy_WEB_DATA( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;

    pygmyCommand.Name = "WEB_DATA";
    pygmyCommand.PrintHandler = Pygmy_WEB_Send_DATA;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 10;

    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );

    return( 1 );
}

u8 Pygmy_WEB_SocketData( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;
    //PYGMYWEBSOCKET pygmySocket;

    pygmyCommand.Name = "WEB_SOCKET";
    pygmyCommand.PrintHandler = Pygmy_WEB_SendSocketData;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 10;
    
    pygmyGlobalSockets[ 0 ].ID = 0x00000001;
    pygmyGlobalSockets[ 0 ].Len = 0;
    pygmyGlobalSockets[ 0 ].Count = 0;
    pygmyGlobalSockets[ 0 ].Action = 2; // NEWFILE
    pygmyGlobalSockets[ 0 ].Process = Pygmy_WEB_SendFile;
    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );
    Pygmy_WEB_SendFile( ucParams );

    return( 1 );
}

u8 Pygmy_WEB_ACK( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCommand;

    pygmyCommand.Name = "WEB_ACK";
    pygmyCommand.PrintHandler = Pygmy_WEB_Send_ACK;
    pygmyCommand.EventHandler = Pygmy_WEB_RX;
    pygmyCommand.Expire = 1;
    pygmyCommand.Retry = 5;

    Pygmy_SYS_QueueCommand( &pygmyGlobalWEBQueue, &pygmyCommand );

    return( 1 );
}

void Pygmy_WEB_InitStack( PYGMYWEBSTACK *pygmyStack, PYGMYWEBNODE *pygmyNode )
{
    u16 i;

    for( i = 0; i < PYGMY_WEB_MAXNODES; i++ ){
        pygmyStack->Nodes[ i ].MAC[ 0 ] = '\0';
        pygmyStack->Nodes[ i ].Class = 0;
        pygmyStack->Nodes[ i ].Signal = 0;
        pygmyStack->Nodes[ i ].Time = 0;
    } // for
    for( i = 0; i < 12; i++ ){
        pygmyStack->Self.MAC[ i ] = pygmyNode->MAC[ i ];
    } // for
    pygmyStack->Self.Class = pygmyNode->Class;
    pygmyStack->Status = 0;
}

void Pygmy_WEB_Send_MARK( void )
{
    u32 ulTime;
    u8 ucBuffer[ 32 ], i, *ucID;
    
    ulTime = Pygmy_SYS_GetTime( );
    for( i = 0; i < 12; i++ ){
        ucBuffer[ i ] = pygmyGlobalStack.Self.MAC[ i ];
    } // for
    for( ; i < 24; i++ ){ // FFFFFFFFFFFFFFFFFFFFFFF denotes that message is for all
        ucBuffer[ i ] = 0xFF;
    } // for
    ucBuffer[ i++ ] = (u8)ulTime >> 24;
    ucBuffer[ i++ ] = (u8)ulTime >> 16;
    ucBuffer[ i++ ] = (u8)ulTime >> 8;
    ucBuffer[ i++ ] = (u8)ulTime;
    ucBuffer[ i++ ] = 0x00; // Slot for MARK device always starts at 0
    ucBuffer[ i++ ] = PYGMY_WEB_MARK; // Device class of capabilities
    ucBuffer[ i++ ] = pygmyGlobalStack.Self.Class; // Reserved
    ucBuffer[ i++ ] = 0x00; // No Payload
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

void Pygmy_WEB_Send_HELLO( void )
{
    //u32 ulTime;
    u8 i, ucBuffer[ 32 ];
    
    //ulTime = Pygmy_SYS_GetTime( );
    for( i = 0; i < 12; i++ ){
        ucBuffer[ i ] = pygmyGlobalStack.Self.MAC[ i ];
    } // for
    for( ; i < 24; i++ ){ // FFFFFFFFFFFFFFFFFFFFFFF denotes that message is for all
        ucBuffer[ i ] = 0xFF;//pygmyGlobalStack.Nodes[ 0 ].MAC[ i ]; // 0 is MARK device, 
    } // for
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 24;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 16;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 8;
    ucBuffer[ i++ ] = 0;//(u8)ulTime;
    ucBuffer[ i++ ] = 0x00; // Slot for MARK device always starts at 0
    ucBuffer[ i++ ] = PYGMY_WEB_HELLO; // Device class of capabilities
    ucBuffer[ i++ ] = pygmyGlobalStack.Self.Class; // Reserved
    ucBuffer[ i++ ] = 0x00; // No Payload
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

void Pygmy_WEB_Send_WELCOME( void )
{
    //u32 ulTime;
    u8 i, ucBuffer[ 32 ];
    
    //ulTime = Pygmy_SYS_GetTime( );
    for( i = 0; i < 12; i++ ){
        ucBuffer[ i ] = pygmyGlobalStack.Self.MAC[ i ];
    } // for
    for( ; i < 24; i++ ){ // FFFFFFFFFFFFFFFFFFFFFFF denotes that message is for all
        ucBuffer[ i ] = 0xFF;
    } // for
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 24;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 16;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 8;
    ucBuffer[ i++ ] = 0;//(u8)ulTime;
    ucBuffer[ i++ ] = 0x00; // Slot for MARK device always starts at 0
    ucBuffer[ i++ ] =  PYGMY_WEB_WELCOME; // Device class of capabilities
    ucBuffer[ i++ ] = pygmyGlobalStack.Self.Class; // Reserved
    ucBuffer[ i++ ] = 0x00; // No Payload
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

void Pygmy_WEB_Send_DECLINE( void )
{

}

void Pygmy_WEB_Send_ACK( void )
{
    //u32 ulTime;
    u8 i, ucBuffer[ 32 ];
    
    //ulTime = Pygmy_SYS_GetTime( );
    for( i = 0; i < 12; i++ ){
        ucBuffer[ i ] = pygmyGlobalStack.Self.MAC[ i ];
    } // for
    for( ; i < 24; i++ ){ // FFFFFFFFFFFFFFFFFFFFFFF denotes that message is for all
        ucBuffer[ i ] = 0xFF;
    } // for
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 24;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 16;
    ucBuffer[ i++ ] = 0;//(u8)ulTime >> 8;
    ucBuffer[ i++ ] = 0;//(u8)ulTime;
    ucBuffer[ i++ ] = 0x00; // Slot for MARK device always starts at 0
    ucBuffer[ i++ ] =  PYGMY_WEB_ACK; // Device class of capabilities
    ucBuffer[ i++ ] = pygmyGlobalStack.Self.Class; // Reserved
    ucBuffer[ i++ ] = 0x00; // No Payload
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

void Pygmy_WEB_Send_NACK( void )
{

}

void Pygmy_WEB_Send_BYE( void )
{

}

void Pygmy_WEB_Send_QUERY( void )
{

}

void Pygmy_WEB_Send_DATA( void )
{
    u8 i, ucBuffer[ 32 ];
    
    for( i = 0; i < 12; i++ ){
        ucBuffer[ i ] = pygmyGlobalStack.Self.MAC[ i ];
    } // for
    for( ; i < 24; i++ ){ // FFFFFFFFFFFFFFFFFFFFFFF denotes that message is for all
        ucBuffer[ i ] = 0xFF;
    } // for
    ucBuffer[ i++ ] = 0;// PDIA
    ucBuffer[ i++ ] = 0;// PDIA
    ucBuffer[ i++ ] = 0;// PDIA
    ucBuffer[ i++ ] = 1;// PDIA
    ucBuffer[ i++ ] = 0x00; // Slot for MARK device always starts at 0
    ucBuffer[ i++ ] =  PYGMY_WEB_ACK; // Device class of capabilities
    ucBuffer[ i++ ] = pygmyGlobalStack.Self.Class; // Reserved
    ucBuffer[ i++ ] = 0x00; // Len of Payload in bytes
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

void Pygmy_WEB_SendSocketData( void )
{
    u8 i, ucBuffer[ 32 ];

    ucBuffer[ 0 ] = (u8)( pygmyGlobalSockets[ 0 ].ID >> 24 ); // Socket ID
    ucBuffer[ 1 ] = (u8)( pygmyGlobalSockets[ 0 ].ID >> 16 ); // Socket ID
    ucBuffer[ 2 ] = (u8)( pygmyGlobalSockets[ 0 ].ID >> 8 ); // Socket ID
    ucBuffer[ 3 ] = (u8)( pygmyGlobalSockets[ 0 ].ID ); // Socket ID
    ucBuffer[ 4 ] = pygmyGlobalSockets[ 0 ].Count; // Count
    ucBuffer[ 5 ] = pygmyGlobalSockets[ 0 ].Len;
    ucBuffer[ 6 ] = pygmyGlobalSockets[ 0 ].Action;
    for( i = 0; i < pygmyGlobalSockets[ 0 ].Len; i++ ){
        ucBuffer[ i + 7 ] = pygmyGlobalSockets[ 0 ].Buffer[ i ];
    } // for
    Pygmy_RF24_PutTXBuffer( 32, ucBuffer );
}

u8 Pygmy_WEB_ReceiveFile( u8 *ucParams )
{
    static PYGMYFILE pygmyFile;
    u8 i;

    if( pygmyGlobalSockets[ 0 ].Action == 0 ){ // NACK
        Pygmy_FILE_Seek( &pygmyFile, PYGMY_FILE_SEEK_CURRENT, -(pygmyGlobalSockets[ 0 ].Len ) );
        pygmyGlobalSockets[ 0 ].Action = 3;
    } // if
    if( pygmyGlobalSockets[ 0 ].Action == 2 ){ // NEWFILE
        ucParams[ 7 + pygmyGlobalSockets[ 0 ].Len ] = '\0';
        Pygmy_FILE_Open( &pygmyFile, pygmyGlobalSockets[ 0 ].Buffer, PYGMY_FILE_ATTRIB_WRITE );
        pygmyGlobalSockets[ 0 ].Action = 3;
        pygmyGlobalSockets[ 0 ].Len = 0;
        Pygmy_WEB_SendSocketData( );
        return( 1 );
    }  // if
    if( pygmyGlobalSockets[ 0 ].Action == 4 ){ // CLOSEFILE
        Pygmy_FILE_Close( &pygmyFile );
        return( 1 );
    } // if
    if( pygmyGlobalSockets[ 0 ].Action == 1 || pygmyGlobalSockets[ 0 ].Action == 3 ){ // ACK or ADDFILE
        for( i = 0; i < pygmyGlobalSockets[ 0 ].Len; i++ ){
            Pygmy_FILE_PutChar( &pygmyFile, pygmyGlobalSockets[ 0 ].Buffer );
        } // for
        pygmyGlobalSockets[ 0 ].Len = 0;
        pygmyGlobalSockets[ 0 ].Action = 1; // ADDFILE
        Pygmy_WEB_SendSocketData( );
        return( 1 );
    } // if

    return( 0 );
}

u8 Pygmy_WEB_SendFile( u8 *ucParams )
{
    static PYGMYFILE pygmyFile;
    u8 i;

    if( pygmyGlobalSockets[ 0 ].Action == 0 ){ // NACK
        Pygmy_FILE_Seek( &pygmyFile, PYGMY_FILE_SEEK_CURRENT, -(pygmyGlobalSockets[ 0 ].Len ) );
        pygmyGlobalSockets[ 0 ].Action = 3;
    } // if
    if( pygmyGlobalSockets[ 0 ].Action == 2 ){ // NEWFILE
        //ucParams[ pygmyGlobalSockets[ 0 ].Len ] = '\0'
        Pygmy_STRING_StringCopyToString( ucParams, pygmyGlobalSockets[ 0 ].Buffer + 7 );
        if( !Pygmy_FILE_Open( &pygmyFile, ucParams, PYGMY_FILE_ATTRIB_READ ) ){
            Pygmy_SYS_Print( PYGMY_SYS_COM3, "\rFile Error" );
        } // if
        pygmyGlobalSockets[ 0 ].Action = 3;
        pygmyGlobalSockets[ 0 ].Len = 0;
        Pygmy_WEB_SendSocketData( );
        return( 1 );
    }  // if
    if( pygmyGlobalSockets[ 0 ].Action == 4 ){ // CLOSEFILE
        Pygmy_FILE_Close( &pygmyFile );
        return( 1 );
    } // if
    if( pygmyGlobalSockets[ 0 ].Action == 1 || pygmyGlobalSockets[ 0 ].Action == 3 ){ // ACK or ADDFILE
        for( i = 0; i < 25; i++ ){
            if( pygmyFile.Attributes & PYGMY_FILE_ATTRIB_EOF ){
                break;
            } // if
            pygmyGlobalSockets[ 0 ].Buffer[ i ] = Pygmy_FILE_GetChar( &pygmyFile );
        } // for
        pygmyGlobalSockets[ 0 ].Len = i;
        ++pygmyGlobalSockets[ 0 ].Count;
        pygmyGlobalSockets[ 0 ].Action = 3; // ADDFILE
        Pygmy_WEB_SendSocketData( );
        return( 1 );
    } // if

    return( 0 );
    
}
