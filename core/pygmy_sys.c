/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

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


#include <stdarg.h>
#include "pygmy_sys.h"

//#ifdef __PYGMY_BOOT
const u8 PYGSYS_INVBITS[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
const u8 PYGMY_DAYSINMONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const u32 PYGMY_BITMASKS[] = { BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11,BIT12,BIT13,BIT14,BIT15,
                              BIT16,BIT17,BIT18,BIT19,BIT20,BIT21,BIT22,BIT23,BIT24,BIT25,BIT26,BIT27,BIT28,BIT29,BIT30,BIT31};
const u32 PYGMY_INVBITMASKS[] = {BIT31,BIT30,BIT29,BIT28,BIT27,BIT26,BIT25,BIT24,BIT23,BIT22,BIT21,BIT20,BIT19,BIT18,BIT17,BIT16,
                               BIT15,BIT14,BIT13,BIT12,BIT11,BIT10,BIT9,BIT8,BIT7,BIT6,BIT5,BIT4,BIT3,BIT2,BIT1,BIT0};
//#endif
// Global System Variables

PYGMYTASK           pygmyGlobalTasks[ PYGMY_MAXTASKS ];
PYGMYMESSAGE        pygmyGlobalMessages[ PYGMY_MAXMESSAGES ];
PYGMYSYSTEM         pygmyGlobalData;

u8 sysInit( void )
{
    u32 ulID, ulFreq, ulXTAL, ulPLL;
  
    pygmyGlobalData.MCUID = getIDCode( );
    
    if( pygmyGlobalData.MCUID == 0x0416 ){
        // L152
        ulXTAL = 8000000;
        ulFreq = 32000000;
        //ulPLL = RCC_PLL_X4|BIT16|BIT1;;
        //FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY1;
    } else if( pygmyGlobalData.MCUID == 0x0420 || pygmyGlobalData.MCUID == 0x0428 ){
        // F100 
        ulXTAL = 12000000;
        ulFreq = 24000000;
        //ulPLL = BIT16|BIT1;;
    } else{
        // F103
        ulXTAL = 8000000;
        ulFreq = 72000000;
        //ulPLL = RCC_PLL_X9|BIT16|BIT15|BIT14|BIT10|BIT1;
        //FPEC->ACR = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2;
    } // else
    pygmyGlobalData.MainClock = ulFreq;

    
    //initTasks();
    //initMessages();

	return( 0 );
}

//--------------------------------------------------------------------------------------------
//-------------------------------------Pygmy OS IO Stream-------------------------------------
void streamInit( void )
{
    u16 i;
    
    for( i = 0; i < MAXCOMPORTS; i++ ){
        streamReset( i );
    } // for
    
}

u8 streamReset( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].RXBufferLen  = 0;
        pygmyGlobalData.Stream[ ucStream ].RXIndex      = 0;
        //pygmyGlobalData.Stream[ ucStream ].RXInIndex    = 0;
        pygmyGlobalData.Stream[ ucStream ].RXLen        = 0; 
        pygmyGlobalData.Stream[ ucStream ].TXBufferLen  = 0;
        pygmyGlobalData.Stream[ ucStream ].TXIndex      = 0;
        //pygmyGlobalData.Stream[ ucStream ].TXInIndex    = 0;
        pygmyGlobalData.Stream[ ucStream ].TXLen        = 0;
        pygmyGlobalData.Stream[ ucStream ].Put          = (void *)TaskException_Handler;
        pygmyGlobalData.Stream[ ucStream ].Get          = (void *)TaskException_Handler;
        pygmyGlobalData.Stream[ ucStream ].RXBuffer     = NULL;
        pygmyGlobalData.Stream[ ucStream ].TXBuffer     = NULL;
        
        return( 1 );
    } // if

    return( 0 );
}
/*
u8 setStream( u8 ucStream, PYGMYFIFO *pygmyFIFO )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].RXIndex    = pygmyFIFO->RXIndex;
        pygmyGlobalData.Stream[ ucStream ].RXInIndex  = pygmyFIFO->RXInIndex;
        pygmyGlobalData.Stream[ ucStream ].RXLen      = pygmyFIFO->RXLen; 
        pygmyGlobalData.Stream[ ucStream ].TXIndex    = pygmyFIFO->TXIndex;
        pygmyGlobalData.Stream[ ucStream ].TXInIndex  = pygmyFIFO->TXInIndex;
        pygmyGlobalData.Stream[ ucStream ].TXLen      = pygmyFIFO->TXLen;
        pygmyGlobalData.Stream[ ucStream ].Put        = pygmyFIFO->Put;
        pygmyGlobalData.Stream[ ucStream ].Get        = pygmyFIFO->Get;
        pygmyGlobalData.Stream[ ucStream ].RXBuffer   = pygmyFIFO->RXBuffer;
        pygmyGlobalData.Stream[ ucStream ].TXBuffer   = pygmyFIFO->TXBuffer;
        
        return( 1 );
    } // if

    return( 0 );
}*/

void streamResetRX( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].RXIndex = 0;
        pygmyGlobalData.Stream[ ucStream ].RXLen = 0;
    } // if
}

void streamResetTX( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].TXIndex = 0;
        pygmyGlobalData.Stream[ ucStream ].TXLen = 0;
    } // if
}

void streamTXChar( u8 ucStream, void *pygmyUSART )
{
    USART_TYPEDEF *ptrUSART = (USART_TYPEDEF *)pygmyUSART;
    
    if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].TXLen ){
		--pygmyGlobalData.Stream[ ucStream ].TXLen;  
		ptrUSART->DR = pygmyGlobalData.Stream[ ucStream ].TXBuffer[ pygmyGlobalData.Stream[ ucStream ].TXIndex ]; 
        pygmyGlobalData.Stream[ ucStream ].TXIndex = ( pygmyGlobalData.Stream[ ucStream ].TXIndex + 1 ) % 
            pygmyGlobalData.Stream[ ucStream ].TXBufferLen;
    } else{
        ptrUSART->CR1 &= ~USART_TXEIE;
    } // else
}

u8 streamGetChar( u8 ucStream )
{
    u16 i;

    if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].RXLen ){
        i = pygmyGlobalData.Stream[ ucStream ].RXIndex;
        pygmyGlobalData.Stream[ ucStream ].RXIndex = ( pygmyGlobalData.Stream[ ucStream ].RXIndex+1 ) % pygmyGlobalData.Stream[ ucStream ].RXBufferLen; // modulo prevents overrun
        --pygmyGlobalData.Stream[ ucStream ].RXLen;
        return( pygmyGlobalData.Stream[ ucStream ].RXBuffer[ i ] );
    } // if

    return(  0 );
}

u8 streamPutChar( u8 ucStream, u8 ucChar )
{
    u16 uiIndex;
    
    //for( i = 0; i < 1000; i++ ){
        if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].TXLen < ( pygmyGlobalData.Stream[ ucStream ].TXBufferLen - 1 ) ){
            uiIndex = ( pygmyGlobalData.Stream[ ucStream ].TXIndex + pygmyGlobalData.Stream[ ucStream ].TXLen ) % pygmyGlobalData.Stream[ ucStream ].TXBufferLen;
            pygmyGlobalData.Stream[ ucStream ].TXBuffer[ uiIndex ] = ucChar;
            ++pygmyGlobalData.Stream[ ucStream ].TXLen;
			return( 1 );
        } // if
        //delay( 50 );
    //}
    
    return( 0 );
}

u8 streamPopChar( u8 ucStream )
{
    u16 i;

    if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].RXLen ){
        --pygmyGlobalData.Stream[ ucStream ].RXLen;
        i = ( pygmyGlobalData.Stream[ ucStream ].RXIndex + ( pygmyGlobalData.Stream[ ucStream ].RXLen ) ) % pygmyGlobalData.Stream[ ucStream ].RXBufferLen;
        return( pygmyGlobalData.Stream[ ucStream ].RXBuffer[ i ] );
    } // if
    
    return( 0 );
}

u8 streamPeekChar( u8 ucStream )
{
    u16 i;

    if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].RXLen ){
        i = ( pygmyGlobalData.Stream[ ucStream ].RXIndex + ( pygmyGlobalData.Stream[ ucStream ].RXLen - 1 ) ) % pygmyGlobalData.Stream[ ucStream ].RXBufferLen;
        return( pygmyGlobalData.Stream[ ucStream ].RXBuffer[ i ] );
    } // if
    
    return( 0 );
}

void streamPushChar( u8 ucStream, u8 ucChar )
{
    u16 i;

    if( ucStream < MAXCOMPORTS && pygmyGlobalData.Stream[ ucStream ].RXLen < pygmyGlobalData.Stream[ ucStream ].RXBufferLen){
        i = ( pygmyGlobalData.Stream[ ucStream ].RXIndex + pygmyGlobalData.Stream[ ucStream ].RXLen ) % pygmyGlobalData.Stream[ ucStream ].RXBufferLen;
         //print( COM1, "\nIndex: %d Len: %d\n", pygmyGlobalData.Stream[ COM1 ].RXIndex, pygmyGlobalData.Stream[ COM1 ].RXLen );
        ++pygmyGlobalData.Stream[ ucStream ].RXLen;
         //print( COM1, "\nIndex: %d Len: %d\n", pygmyGlobalData.Stream[ COM1 ].RXIndex, pygmyGlobalData.Stream[ COM1 ].RXLen );
    
        pygmyGlobalData.Stream[ ucStream ].RXBuffer[ i ] = ucChar;
    } // if
}

u8 streamSetPut( u8 ucStream, void *ptrFunc )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].Put = ptrFunc;
    
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetGet( u8 ucStream, void *ptrFunc )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].Get = ptrFunc;
    
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetRXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].RXBuffer = ucBuffer;
        pygmyGlobalData.Stream[ ucStream ].RXBufferLen = uiLen;
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetTXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen )
{
    if( ucStream < MAXCOMPORTS ){
        pygmyGlobalData.Stream[ ucStream ].TXBuffer = ucBuffer;
        pygmyGlobalData.Stream[ ucStream ].TXBufferLen = uiLen;
        return( 1 );
    } // if
    
    return( 0 );
}

//--------------------------------------------------------------------------------------------
//----------------------------------Pygmy OS Command/Response---------------------------------

void initCommandQueue( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    u16 i;

    pygmyCmdQueue->Index = 0;
    pygmyCmdQueue->Count = 0;
    for( i = 0; i < PYGMY_MAXCOMMANDS; i++ ){
        deleteCommand( pygmyCmdQueue, i );
    } // for
}

void printCommand( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler();
}

void deleteCommand( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u16 uiIndex )
{
    pygmyCmdQueue->Commands[ uiIndex ].Status = 0;
    pygmyCmdQueue->Commands[ uiIndex ].Retry = 0;
    pygmyCmdQueue->Commands[ uiIndex ].Name = NULL;
    pygmyCmdQueue->Commands[ uiIndex ].EventHandler = (void *) TaskException_Handler;
    pygmyCmdQueue->Commands[ uiIndex ].PrintHandler = (void *) TaskException_Handler;
    //pygmyGlobalData.ATCommands[ uiIndex ].ErrorHandler = (void *) Pygmy_SYS_TaskExceptionHandler;
    pygmyCmdQueue->Commands[ uiIndex ].TimeStamp = 0;
    pygmyCmdQueue->Commands[ uiIndex ].Expire = 0;
    if( pygmyCmdQueue->Count ){
        --pygmyCmdQueue->Count;
    } // if

    processCommands( pygmyCmdQueue ); // Call to move on to next queued command, if any
}

void processCommands( PYGMYCOMMANDQUEUE *pygmyCmdQueue ) 
{
    u16 i;

    if( pygmyCmdQueue->Count ){
        for( i = 0; i < PYGMY_AT_MAXCOMMANDS; i++ ){
            if( !pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status ){
                pygmyCmdQueue->Index = ( pygmyCmdQueue->Index + 1 ) % PYGMY_AT_MAXCOMMANDS;
            } else{
                break;
            } // else
        } // for
        
        if( !(pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status & PYGMY_AT_TX ) ){ 
            // Command Expires "Expire" seconds after it is actually executed for the first time
            if( pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire > PYGMY_AT_MAXEXPIRE ){
                pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire = PYGMY_AT_MAXEXPIRE;
            } // if
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire += time();
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler( );//(u8*)PYGMY_CMD_NULL );
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status |= PYGMY_AT_TX;
        } // if
        if( pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire <= time() ){
            if( !pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry ){
                deleteCommand( pygmyCmdQueue, pygmyCmdQueue->Index );
                if( pygmyCmdQueue->Count ){
                    processCommands( pygmyCmdQueue );
                } // if
            } else{
                --pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry;
                pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status &= ~PYGMY_AT_TX;
                pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status |= PYGMY_AT_RETRY; // Prevents possible skipping to the next command prematurely
            } // else
        } // if
    } 
}

void replaceCommand( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd )
{
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status        = PYGMY_AT_QUEUED;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry         = pygmyCmd->Retry;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Name          = pygmyCmd->Name;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].EventHandler  = pygmyCmd->EventHandler;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler  = pygmyCmd->PrintHandler;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].TimeStamp     = time();
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire        = pygmyCmd->Expire;

    ++pygmyCmdQueue->Count;
    processCommands( pygmyCmdQueue );
}

u8 getCommandRetriesRemaining( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    // returns retries for command currently executing
    
    return( pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry );
}

u8 isCommandQueued( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u8 *ucName  )
{
    u16 i;
    u8 ucCount;

    for( i = 0, ucCount = 0; i < PYGMY_AT_MAXCOMMANDS; i++ ){
        if( !pygmyCmdQueue->Commands[ i ].Status ){
            continue;
        } // if
        if( isStringSame( pygmyCmdQueue->Commands[ i ].Name, ucName ) ){
            ++ucCount;
        } // if
    } // ofr

    return( ucCount );
}

u8 queueCommand( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd )
{
    u16 uiIndex;

    if( ( pygmyCmdQueue->Count + 1 ) < PYGMY_AT_MAXCOMMANDS ){
        // Calculate Index with modulus so that if Index + Count are larger than array, Index will wrap
        // Remainder of ( ( ATCount + 1 ) + ATIndex % MAX = location for new command element
    
        uiIndex = ( pygmyCmdQueue->Index + pygmyCmdQueue->Count ) % PYGMY_AT_MAXCOMMANDS;

        pygmyCmdQueue->Commands[ uiIndex ].Status        = PYGMY_AT_QUEUED;
        pygmyCmdQueue->Commands[ uiIndex ].Retry         = pygmyCmd->Retry;
        pygmyCmdQueue->Commands[ uiIndex ].Name          = pygmyCmd->Name;
        pygmyCmdQueue->Commands[ uiIndex ].EventHandler  = pygmyCmd->EventHandler;
        pygmyCmdQueue->Commands[ uiIndex ].PrintHandler  = pygmyCmd->PrintHandler;
        //pygmyGlobalData.ATCommands[ uiIndex ].ErrorHandler  = pygmyATCommand->ErrorHandler;
        pygmyCmdQueue->Commands[ uiIndex ].TimeStamp     = time();
        pygmyCmdQueue->Commands[ uiIndex ].Expire        = pygmyCmd->Expire;
        
        ++pygmyCmdQueue->Count;
        processCommands( pygmyCmdQueue );
        return( 1 );
    } 
        
    return( 0 );
}

//--------------------------------End Pygmy OS Command/Response-------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//--------------------------------Pygmy OS Data Integrity Algorithm---------------------------


u8 sysCRC8( u8 *ucBuffer, u16 uiLen )
{
    u16 i;
    u8 ucCRC;

    for( i = 0, ucCRC = 0; i < uiLen - 1; i++ ){
        ucCRC += ucBuffer[ i ];
    } // for
    ucCRC = 1 + ( 0xFF ^ (u8)ucCRC );

    return( ucCRC );
}

u16 sysCRC16( u8 *ucBuffer, u16 uiLen )
{
    u16 i, uiCRC;

    for( i = 0, uiCRC = 0; i < uiLen - 1; i++ ){
        uiCRC += ucBuffer[ i ];
    } // for
    uiCRC = 1 + ( 0xFFFF ^ (u16)uiCRC ); 

    return( uiCRC );
}

u8 pdiaEncode( u8 ucByte, u8 ucMode, u32 *ulSum )
{
    static u32 ulAccumulator, ulMask, ulIndex;
    u8 ucFrameComplete;
    
    ucFrameComplete = 0;
    
    switch( ucMode ){
        case PDIA_NEW:
            ulAccumulator = 1;
            ulMask = 0;
            ulIndex = 0;
        break;
        case PDIA_ADD:
            if( ulIndex++ == 16384 ){
                *ulSum = (u32)( ( ulMask << 16 ) | ulAccumulator );
                ulIndex = 0;
                ucFrameComplete = 1;
            }
            ulAccumulator = ( ulAccumulator + ucByte ) % 65521;
            ulMask = ( ulMask + ulAccumulator ) % 65521;
            
        break;
        case PDIA_END:
            *ulSum = (u32)( ( ulMask << 16 ) | ulAccumulator );
            ucFrameComplete = 1;
        break;
    }
    
    return( ucFrameComplete );
}

void pdiaPrintInteger( u8 ucMode, u32 *ulSum, u8 ucStream, u8 *ucFormat, u32 ulData )
{
    u8 ucBuffer[ 20 ];
    
    convertIntToString( ulData, ucFormat, ucBuffer );
    pdiaPrintString( ucMode, ulSum, ucStream, ucBuffer );
}

void pdiaPrintString( u8 ucMode, u32 *ulSum, u8 ucStream, u8 *ucBuffer )
{
    u16 i;

    // Note: !!! This function only complies with PDIA spec to 16384 chars !!!
    //if(ucStream < PYGMY_SYS_MAXPIPES && pygmyGlobalData.Stream[ ucStream ] ){
    if( ucStream < MAXCOMPORTS ){//&& pygmyGlobalData.Stream[ ucStream ] ){
        
        if( ucMode == PDIA_NEW ){
            pdiaEncode( 0, PDIA_NEW, ulSum );
        }
        for( i = 0; ucBuffer[ i ]; i++ ){
            pdiaEncode( ucBuffer[ i ], PDIA_ADD, ulSum );
        }
        if( ucMode == PDIA_END ){
            pdiaEncode( 0, PDIA_END, ulSum );
        }
        pygmyGlobalData.Stream[ ucStream ].Put( ucBuffer );
    }
}

//-----------------------------End Pygmy OS Data Integrity Algorithm--------------------------
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//--------------------------------------Pygmy OS Tasking--------------------------------------

u16 processTasks( void )
{
    u32 ulTime;
    u16 i;

    ulTime = time();
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( pygmyGlobalTasks[ i ].ID != PYGMY_BLANKID ){
            if( pygmyGlobalTasks[ i ].Expire ){
                if( pygmyGlobalTasks[ i ].Expire == ulTime ){
                    //deleteTask( &pygmyGlobalTasks[ i ] );
                    deleteTask( "", pygmyGlobalTasks[ i ].ID );
                    continue; // Do Not finish processing task after it has ended
                } // if
            } // if
            if( pygmyGlobalTasks[ i ].Timer != 0  ){            // Zero means no timer function
                if( pygmyGlobalTasks[ i ].Timer == 1 ){         // One is the reload point
                    pygmyGlobalTasks[ i ].Timer = pygmyGlobalTasks[ i ].Reload;
                    if( pygmyGlobalTasks[i].Busy == 0 ){
                        pygmyGlobalTasks[ i ].Busy = 1;         // Lock thread until execution is complete
                        pygmyGlobalTasks[ i ].EventHandler();   // EventHandler is called on every reload point
                        pygmyGlobalTasks[ i ].Busy = 0;         // Unlock Thread
                    } // if
                } else {
                    --pygmyGlobalTasks[ i ].Timer;              // Else countdown the timer
                } // else  
            } // else
        } // if
    } // for i
    
    return( 1 );
}

u16 initTasks( void )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        pygmyGlobalTasks[ i ].Name          = NULL;
        pygmyGlobalTasks[ i ].ID            = PYGMY_BLANKID;
        pygmyGlobalTasks[ i ].Timer         = 0;
        pygmyGlobalTasks[ i ].Reload        = 0;
        pygmyGlobalTasks[ i ].EventHandler  = ( void * ) TaskException_Handler;
        pygmyGlobalTasks[ i ].TimeStamp     = 0;
        pygmyGlobalTasks[ i ].Expire        = 0;
    } // for   

    return( 1 );
}

void TaskException_Handler( void )
{
    // ToDo: Add any relevant debug or cleanup code here
}

u16 newSimpleTask( u8 *ucName, u32 ulTimer, PYGMYFUNCPTR EventHandler )
{
    // ID = 0 calls for auto generated ID
    // Timer = 2, 1 is reload, this provides approx 1 millisec delay
    // Reload = ulTimer, This value will be loaded into timer on each reload at 1
    // Expire = 0, doesn't expire

    return( newTask( ucName, 0, 2, ulTimer, 0, EventHandler ) ); // return ID or 0 for fail
}

u16 newTask( u8 *ucName, u16 uiID, u32 ulTimer, u32 ulReload, u32 ulExpire, PYGMYFUNCPTR EventHandler )
{ 
    u16 i;

    if( uiID >= PYGMY_SERVICEDID ){
        return( 0 );
    } // if
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( uiID && ( pygmyGlobalTasks[ i ].ID == uiID || 
            isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ) ){
            return( 0 );
            //return( PYGMY_TASK_EXISTS );
        } // if
    } // for
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( pygmyGlobalTasks[ i ].ID == PYGMY_BLANKID ){
            if( uiID ){
                pygmyGlobalTasks[ i ].ID = uiID; // ID was provided
            } else{
                pygmyGlobalTasks[ i ].ID = PYGMY_TEMPID | i; // Generate unique ID
            } // else
            pygmyGlobalTasks[ i ].Busy          = 0;
            pygmyGlobalTasks[ i ].Name          = ucName;
            pygmyGlobalTasks[ i ].Timer         = ulTimer;
            if( ulTimer ){               // Do not allow Reload if Timer == 0
                pygmyGlobalTasks[ i ].Reload    = ulReload;
            } else{
                pygmyGlobalTasks[ i ].Reload    = 0;
            } // else
            pygmyGlobalTasks[ i ].EventHandler  = EventHandler;
            pygmyGlobalTasks[ i ].TimeStamp     = time( );
            pygmyGlobalTasks[ i ].Expire        = ulExpire;

            //return( PYGMY_TASK_SUCCESS );
            return( pygmyGlobalTasks[ i ].ID );
        } // if
    } // for

    return( 0 );
    //return( PYGMY_TASK_FAIL );
}

u8 isTaskRunning( u8 *ucName, u16 uiID ) 
{
    // Return 1 for task found and 0 for not found
    u16 i;
    
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( ( uiID && pygmyGlobalTasks[ i ].ID == uiID ) || 
            isStringSame( pygmyGlobalTasks[ i ].Name, ucName ) ){
            
            return( 1 );
        } // if
    }//for

    return( 0 );
}

u8 deleteTask( u8 *ucName, u16 uiID )
{
    u16 i; 
	u8 ucDeleted;
    
    // This loop iterates to MAX to delete all instances of a given name
    for( i = 0, ucDeleted = 0; i < PYGMY_MAXTASKS; i++ ){
        if( ( uiID && pygmyGlobalTasks[ i ].ID == uiID ) || isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ){
            pygmyGlobalTasks[ i ].Name          = NULL;
            pygmyGlobalTasks[ i ].ID            = PYGMY_BLANKID;
            pygmyGlobalTasks[ i ].Timer         = 0;
            pygmyGlobalTasks[ i ].Reload        = 0;
            pygmyGlobalTasks[ i ].EventHandler  = ( void * ) TaskException_Handler;
            pygmyGlobalTasks[ i ].TimeStamp     = 0;
            ucDeleted = 1;
        } // if
    } // for
   
    return( ucDeleted );
}

u8 getTask( u8 *ucName, u16 uiID, PYGMYTASK *pygmyTask )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( ( uiID && pygmyGlobalTasks[ i ].ID == uiID ) || 
            ( isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ) ){
            pygmyTask->Name = pygmyGlobalTasks[ i ].Name;
            pygmyTask->ID = pygmyGlobalTasks[ i ].ID;
            pygmyTask->Timer = pygmyGlobalTasks[ i ].Timer;
            pygmyTask->Reload = pygmyGlobalTasks[ i ].Reload;
            pygmyTask->EventHandler = pygmyGlobalTasks[ i ].EventHandler;
            pygmyTask->TimeStamp = pygmyGlobalTasks[ i ].TimeStamp;
            pygmyTask->Expire = pygmyGlobalTasks[ i ].Expire;
            
            return( 1 );
        } // if
    } // for
    
    return( 0 );
}

void listTask( PYGMYTASK *pygmyTask, u16 uiTask )
{
    pygmyTask->ID               = pygmyGlobalTasks[ uiTask ].ID;
    pygmyTask->Name             = pygmyGlobalTasks[ uiTask ].Name;
    pygmyTask->Timer            = pygmyGlobalTasks[ uiTask ].Timer;
    pygmyTask->Reload           = pygmyGlobalTasks[ uiTask ].Reload;
    pygmyTask->EventHandler     = pygmyGlobalTasks[ uiTask ].EventHandler;
    pygmyTask->TimeStamp        = pygmyGlobalTasks[ uiTask ].TimeStamp;
    pygmyTask->Expire           = pygmyGlobalTasks[ uiTask ].Expire;
}

//------------------------------------End Pygmy OS Tasking------------------------------------
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//-------------------------------------Pygmy OS Messaging-------------------------------------

u16 initMessages( void )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){
        pygmyGlobalMessages[ i ].DestID         = PYGMY_BLANKID;
        pygmyGlobalMessages[ i ].DestName       = NULL;
        pygmyGlobalMessages[ i ].Message        = NULL;
        pygmyGlobalMessages[ i ].Value          = 0;
        pygmyGlobalMessages[ i ].TimeStamp      = 0;
    } // for
    
    return( 1 );
}

u8 deleteMessage( u8 *ucName, u16 uiID )
{
    u16 i;
    
    i = getMessageIndex( ucName, uiID );
    if( i != PYGMY_NOMESSAGE ){
        pygmyGlobalMessages[ i ].DestID         = PYGMY_BLANKID; 
        pygmyGlobalMessages[ i ].DestName       = NULL;
        pygmyGlobalMessages[ i ].Message        = NULL;
        pygmyGlobalMessages[ i ].Value          = 0;
        pygmyGlobalMessages[ i ].TimeStamp      = 0;
    
        return( 1 );
    } // if

    return( 0 );
}

u8 isMessage( u8 *ucName, u16 uiID )
{
    if( getMessageIndex( ucName, uiID ) == PYGMY_NOMESSAGE ){
        return( 0 );
    } // if
    
    return( 1 );
}

u16 getMessageIndex( u8 *ucName, u16 uiID )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){
        if( ( uiID && pygmyGlobalMessages[ i ].DestID == uiID ) || 
            ( ucName[ 0 ] && isStringSame( ucName, pygmyGlobalMessages[ i ].DestName ) ) ) {
            
            return( i );
        } // if
    } // for

    return( PYGMY_NOMESSAGE );
}

u8 getMessage( u8 *ucName, u16 uiID, PYGMYMESSAGE *pygmyMsg )
{
    u16 i;

    i = getMessageIndex( ucName, uiID );
    if( i != PYGMY_NOMESSAGE ){
        pygmyMsg->DestName          = pygmyGlobalMessages[ i ].DestName;
        pygmyMsg->Message           = pygmyGlobalMessages[ i ].Message;
        pygmyMsg->Value             = pygmyGlobalMessages[ i ].Value;
        pygmyMsg->TimeStamp         = pygmyGlobalMessages[ i ].TimeStamp;
            
        deleteMessage( ucName, uiID );
        return( 1 );
    } // if
    
    return( 0 );
}

u8 sendMessage( u8 *ucName, u16 uiID, u8 *ucMessage, u16 uiValue )
{
    u16 i;
    
    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){
        if( pygmyGlobalMessages[ i ].DestID == PYGMY_BLANKID ) {
            if( uiID == PYGMY_BLANKID ){
                pygmyGlobalMessages[ i ].DestID = uiID;
            } else{
                pygmyGlobalMessages[ i ].DestID = PYGMY_UNMARKEDID;
            } // else
            pygmyGlobalMessages[ i ].DestName   = ucName;
            pygmyGlobalMessages[ i ].Message    = ucMessage;
            pygmyGlobalMessages[ i ].Value      = uiValue;
            pygmyGlobalMessages[ i ].TimeStamp  = time();
            return( 1 );
        } // if
    } // for
    
    return( 0 );
}

u16 processMessages( void )
{
    u32 ulTime;
    u16 i, ii;

    ulTime = time();
    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){                       // Cycle through messages to check for any unserviced
        if (  pygmyGlobalMessages[ i ].DestID && !( pygmyGlobalMessages[ i ].DestID & PYGMY_SERVICEDID ) ){// Serviced ID set after task notified
            // Check to make sure message hasn't gone stale, delete if it has
            if( pygmyGlobalMessages[ i ].TimeStamp + 5 == ulTime ){
                deleteMessage( pygmyGlobalMessages[ i ].DestName, pygmyGlobalMessages[ i ].DestID );
                //deleteMessage( &pygmyGlobalMessages[ i ] );
                continue; // Do Not finish processing task after it has ended
            } // if 
            for( ii = 0; ii < PYGMY_MAXTASKS; ii++ ){               // Message deleted after task retrieves
                if( pygmyGlobalMessages[ i ].DestID == pygmyGlobalTasks[ ii ].ID ) {
                    if( pygmyGlobalTasks[ ii ].Busy == 0 ){
                        pygmyGlobalTasks[ ii ].Busy = 1;            // Lock thread until execution is complete
                        pygmyGlobalTasks[ ii ].EventHandler();      // Notify task of message
                        pygmyGlobalTasks[ ii ].Busy = 0;
                    } // if
                    break;
                } // if
            } // for
            if( ii == PYGMY_MAXTASKS )   // If task no longer running, delete message
                pygmyGlobalMessages[ i ].DestID = PYGMY_BLANKID;
        } // if
    } // for

    return( 1 );
}

void listMessage( PYGMYMESSAGE *pygmyMsg, u16 uiMsg )
{
     pygmyMsg->DestID           = pygmyGlobalMessages[ uiMsg ].DestID;     
     pygmyMsg->DestName         = pygmyGlobalMessages[ uiMsg ].DestName;         
     pygmyMsg->Message          = pygmyGlobalMessages[ uiMsg ].Message;
     pygmyMsg->Value            = pygmyGlobalMessages[ uiMsg ].Value;
     pygmyMsg->TimeStamp        = pygmyGlobalMessages[ uiMsg ].TimeStamp;
}

//-----------------------------------End Pygmy OS Messaging-----------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//----------------------------------------Pygmy OS ID-----------------------------------------

u8 *getIDString( void ) // ( u8 *ucBuffer )
{
    static u8 ucIDBuffer[ 25 ];
    u8 i, *ucID;

    ucIDBuffer[ 0 ] = '\0';
    ucID = (u8*)0x1FFFF7E8; // Address of 96 bit Unique ID
    for( i = 0; i < 12; i++ ){
        //convertIntToString( ucID[ 11 - i ], "2x", (u8*)(ucBuffer + len( ucBuffer ) ) );
        convertIntToString( ucID[ 11 - i ], "2x", (u8*)( ucIDBuffer + ( i * 2 ) ) );
    } // for
    
    return( (u8*)ucIDBuffer );
}

u8 *getID( void )
{
    // Returns unique device ID
    u8 *ucID;
    
    ucID = (u8*)0x1FFFF7E8; // Address of 96 bit Unique ID
    
    return( ucID );
}

u32 descriptorGetMainClock( void )
{
    // Returns the HSE programmed into the descriptor page by bootloader
    u32 *ulMainClock;
    
    ulMainClock = (u32*)(0x08000000 + ( ( SIZEREG->Pages - 2 ) * 1024 ) + 4); 
    
    return( *ulMainClock );
}

u32 descriptorGetXTAL( void )
{
    // Returns the HSE programmed into the descriptor page by bootloader
    u32 *ulXTAL;
    
    ulXTAL = (u32*)(0x08000000 + ( ( SIZEREG->Pages - 2 ) * 1024 ) + 2); 
    
    return( *ulXTAL );
}

u32 descriptorGetID( void )
{
    // Returns the device ID containing type/family
    // Low Power devices = 0x0416
    // Low density devices = 0x412
    // Mediumdensity devices = 0x410
    // High density devices = 0x414
    // XL density devices = 0x430
    // Connectivity devices = 0x418
    // Low and medium density value line devices = 0x420
    // High density value line devices = 0x428
    u32 *ulID, ulDescriptorID;
    
    ulID = (u32*)(0x08000000 + ( ( SIZEREG->Pages - 2 ) * 1024 )); 
    print( COM3, "\rID Address: %8X", ulID );
    return( *ulID );
}

u32 getIDRevision( void )
{
    // Returns the device revision from Debug cell registers
    // ToDo: Add at bootloader level
    u32 *ulRev;
    
    ulRev = (u32*)0xE0042000;
    
    return( ( *ulRev & 0xFFFFF000 ) >> 12 );
}


//--------------------------------------End Pygmy OS ID---------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//---------------------------------------Pygmy OS Print---------------------------------------

void println( u8 ucStream, u8 *ucBuffer, ... )
{
    print( ucStream, ucBuffer );
    print( ucStream, "\r" );
}

void print( u8 ucStream, u8 *ucBuffer, ... )
{
    PYGMYTIME pygmyTime;
    u8 i, ii, ucValue[ 3 ], *sValue, sFormat[ 13 ], ucIntBuffer[ 20 ];
    u16 uiLen;
    va_list ap;
    
    
    ucValue[ 2 ] = 0;
    va_start( ap, ucBuffer );
    for( ; *ucBuffer; ucBuffer++ ){
        
        if( *ucBuffer == '%' ){ // Found format specifier
            // first collect precision, if any
            for( i = 0; i < 12 && ( isNumeric( *(++ucBuffer) ) || *ucBuffer == '-' 
                || *ucBuffer == '+' ); i++ ){
                sFormat[ i ] = *ucBuffer;
            } // for
            sFormat[ i ] = 0; // Terminate format string
            if( *ucBuffer == '%' ){ // Format specifer was only escaping '%' char, print
                ucValue[ 0 ] = '%';
                 pygmyGlobalData.Stream[ ucStream ].Put( ucValue );
            } else if( *ucBuffer == 'c' ){ 
                ucValue[ 0 ] = (u8)va_arg( ap, int );
                 pygmyGlobalData.Stream[ ucStream ].Put( ucValue );
            } else if( *ucBuffer == 's' ){
                sValue = va_arg( ap, u8 * );
                uiLen = len( sValue );
                
                ucValue[ 0 ] = ' ';
                if( sFormat[ 0 ] == '-' ){
                    pygmyGlobalData.Stream[ ucStream ].Put( sValue );
                    ii = convertStringToInt( (u8*)(sFormat+1) );
                    if( ii > uiLen ){
                        ii -= uiLen;
                    } else{
                        ii = 0;
                    } // else
                    for( i = 0; i < ii; i++ ){
                         pygmyGlobalData.Stream[ ucStream ].Put( ucValue );
                    } // for
                } else{
                    ii = convertStringToInt( sFormat );
                    if( ii > uiLen ){
                        ii -= uiLen;
                    } else{
                        ii = 0;
                    }
                    for( i = 0; i < ii; i++ ){
                         pygmyGlobalData.Stream[ ucStream ].Put( ucValue );
                    } // for
                     pygmyGlobalData.Stream[ ucStream ].Put( sValue );
                } // else
            } else if( *ucBuffer == 'i' || *ucBuffer == 'd' ||
                *ucBuffer == 'x' || *ucBuffer == 'X' || *ucBuffer == 'o' ){
                sFormat[ i++ ] = *ucBuffer; // PrintInteger requires format char
                sFormat[ i ] = 0; // terminate at new index
                convertIntToString( va_arg( ap, u32 ), sFormat, ucIntBuffer );
                 pygmyGlobalData.Stream[ ucStream ].Put( ucIntBuffer );
            } else if( *ucBuffer == 't' ){
                convertSecondsToSystemTime( va_arg( ap, s32 ), &pygmyTime );
                print( ucStream, "%4d-%2d-%2d %2d:%2d:%2d", pygmyTime.Year,pygmyTime.Month,pygmyTime.Day,
                    pygmyTime.Hour,pygmyTime.Minute,pygmyTime.Second );
            }
        } else{
            ucValue[ 0 ] = *ucBuffer;
            ucValue[ 1 ] = 0;
             pygmyGlobalData.Stream[ ucStream ].Put( ucValue );
        }
    } // for
    va_end( ap );
}

u8 putsUSART1FIFO( u8 *ucBuffer )
{
    
	return( 1 );
}

u8 putsUSART2FIFO( u8 *ucBuffer )
{
	return( 1 );
}

u8 putsUSART3FIFO( u8 *ucBuffer )
{   
    u16 i;
	
    for( ; *ucBuffer; ){
		USART3->CR1 &= ~USART_TXEIE;
        for( i = 0; i < 1000; i++ ){
			if( streamPutChar( COM3, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART3->CR1 |= USART_TXEIE;
		}
		
    } // for
    
    return( 1 );
}

#ifdef USART4
u8 putsUSART4FIFO( u8 *ucBuffer )
{
	return( 1 );
}
#endif
#ifdef USART5
u8 putsUSART5FIFO( u8 *ucBuffer )
{
	return( 1 );
}
#endif

void streamSetPrintFile( void *pygmyFile )
{
    pygmyGlobalData.File = pygmyFile;
}

u8 putsFILE( u8 *ucBuffer )
{
    return( filePutString( pygmyGlobalData.File, ucBuffer ) );
}




//-------------------------------------End Pygmy OS Print-------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//------------------------------------Pygmy CMD Interface-------------------------------------
u8 executeCmd( u8 *ucBuffer, PYGMYCMD *pygmyCmds )
{
    u16 ii;
    u8 *ucCommand;
    
    ucCommand = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( !ucCommand ){
        return( 0 );
    } // if
        
    for( ii = 0; 1; ii++ ){
        if( isStringSame( NULL, pygmyCmds[ ii ].Name ) ){
            return( 0 );
        } // if
        if( isStringSame( ucCommand, pygmyCmds[ ii ].Name ) ){
            // Parameters should not be passed here, they are passed using getNextSubString
            if( pygmyCmds[ ii ].Call( (u8*)"" ) ){
                return( 1 );
            }else{
                return( 0 );
            } // else
        } // if
    } // for
}

//----------------------------------End Pygmy CMD Interface-----------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//---------------------------------------Pygmy OS XTAL----------------------------------------

void setMainClock( u32 ulClock )
{
    u16 i;
    // ToDo: Upgrade this function with selectable prescaler and multiplier  
    PYGMY_RCC_HSE_ENABLE;
    while( !(PYGMY_RCC_HSE_READY) );
    // The following PLL Multiplier * XTAL must not exceed 24MHz for 100 Series MCU 
    RCC->CFGR2 = 0;
    RCC->CFGR = BIT15|BIT14|BIT16|BIT1;//BIT15|BIT14 set ADC prediv to 8
    PYGMY_RCC_PLL_ENABLE;
    while( !(PYGMY_RCC_PLL_READY) );
    // HSI Must be ON for Flash Program/Erase Operations
    PYGMY_RCC_HSI_ENABLE;
    PYGMY_RCC_TIMER1_ENABLE;
    SYSTICK->LOAD = ( pygmyGlobalData.MainClock * 2 ) / 1000; // 24000; // Based on  ( 2X the System Clock ) / 1000
    SYSTICK->VAL = ( pygmyGlobalData.MainClock * 2 ) / 1000; // 24000;
    SYSTICK->CTRL = 0x07;   // Enable system timer  
}

void delay( u32 ulDelay )
{
    // This function uses Timer1 to provide an accurate microsecond delay
    // MainClock must be set to 1MHz min, 8MHz or higher recommended
    if( ){ // TIM10 // F103XL
        TIM10->CR1 = 0;                          // Disable before configuring timer
        if( ulDelay > 0x0000FFFF ){
            TIM10->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
            ulDelay &= 0x0000FFFF;
        } // 
        ulDelay *= ( pygmyGlobalData.MainClock / 1000000 );
        if( ulDelay < 60 ){ 
            // Minimum number of cycles supported
            ulDelay = 60;
        } // 
        TIM10->CR2 = 0;                          //
        TIM10->SMCR = 0;                         //
        TIM10->DIER = 0;                         // DMA and interrupt enable register
        TIM10->CNT = 0;                          // Count Register
        TIM10->ARR =  ulDelay - 60; // Auto Reload Register
        TIM10->SR = 0;
        TIM10->CR1 = ( TIM_ARPE | TIM_OPM | TIM_CEN );      // Enable single shot count
        while( (TIM10->CR1 & TIM_CEN) );         // Wait for count to complete 
    } else if( ){ // TIM15 // F100
        TIM15->CR1 = 0;                          // Disable before configuring timer
        if( ulDelay > 0x0000FFFF ){
            TIM15->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
            ulDelay &= 0x0000FFFF;
        } // 
        ulDelay = (( pygmyGlobalData.MainClock / 1000000 ) * ulDelay);
        if( ulDelay < 60 ){
            ulDelay = 60;
        } // 
        TIM15->CR2 = 0;                          //
        TIM15->SMCR = 0;                         //
        TIM15->DIER = 0;                         // DMA and interrupt enable register
        TIM15->CNT = 0;                          // Count Register
        TIM15->ARR =  ulDelay - 60; // Auto Reload Register
        TIM15->SR = 0;
        TIM15->CR1 = ( TIM_ARPE | TIM_OPM | TIM_CEN );      // Enable single shot count
        while( (TIM15->CR1 & TIM_CEN) );         // Wait for count to complete 
    } else{ // TIM1 // F103
        // Warning! F103 devies with less than 768KB Flash do not have extra
        // multipurpose timers and must share Timer1. In this case, Timer1
        // should not be used for PWM output.
        //PYGMY_RCC_TIMER1_ENABLE;
        TIM1->CR1 = 0;                          // Disable before configuring timer
        if( ulDelay > 0x0000FFFF ){
            TIM1->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
            ulDelay &= 0x0000FFFF;
        } // 
        ulDelay = (( pygmyGlobalData.MainClock / 1000000 ) * ulDelay);
        if( ulDelay < 60 ){
            ulDelay = 60;
        } // 
        TIM1->CR2 = 0;                          //
        TIM1->SMCR = 0;                         //
        TIM1->DIER = 0;                         // DMA and interrupt enable register
        TIM1->CNT = 0;                          // Count Register
        TIM1->ARR =  ulDelay - 60; // Auto Reload Register
        TIM1->SR = 0;
        TIM1->CR1 = ( TIM_ARPE | TIM_OPM | TIM_CEN );      // Enable single shot count
        while( (TIM1->CR1 & TIM_CEN) );         // Wait for count to complete 
    } // else
}

/*void delay( u32 ulDelay )
{
    // This function uses Timer1 to provide an accurate microsecond delay
    // MainClock must be set to 1MHz min, 8MHz or higher recommended
    TIM1->CR1 = 0;                          // Disable before configuring timer
    if( ulDelay > 0x0000FFFF ){
        TIM1->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
        ulDelay &= 0x0000FFFF;
    } // 
    ulDelay = (( pygmyGlobalData.MainClock / 1000000 ) * ulDelay);
    if( ulDelay < 60 ){
        ulDelay = 60;
    } // 
    TIM1->CR2 = 0;                          //
    TIM1->SMCR = 0;                         //
    TIM1->DIER = 0;                         // DMA and interrupt enable register
    TIM1->CNT = 0;                          // Count Register
    TIM1->ARR =  ulDelay - 60; // Auto Reload Register
    TIM1->SR = 0;
    TIM1->CR1 = ( TIM_ARPE | TIM_OPM | TIM_CEN );      // Enable single shot count
    while( (TIM1->CR1 & TIM_CEN) );         // Wait for count to complete
}*/

void setStopWatch( void )
{
    pygmyGlobalData.StopWatch = ( pygmyGlobalData.MainClock * 2 ) - SYSTICK->VAL;
}

u32 getStopWatch( void )
{
    pygmyGlobalData.StopWatch += ( pygmyGlobalData.MainClock * 2 ) - SYSTICK->VAL;
    return( pygmyGlobalData.StopWatch );
}

void mcoEnable( u8 ucSource )
{ 
    const u32 PYGMY_MCO[] = { RCC_MCO_SYSCLOCK, RCC_MCO_INTERNALRC, RCC_MCO_EXTCLOCK, RCC_MCO_PLLDIV2 };

    if( ucSource > 3 ){
        return;
    } // if
    PYGMY_RCC_GPIOA_ENABLE;
    GPIOA->CRH &= ~PIN8_CLEAR;
    GPIOA->CRH |= PIN8_OUT50_ALTPUSHPULL;
    RCC->CFGR &= ~RCC_MCO_CLEAR;
    RCC->CFGR |= PYGMY_MCO[ ucSource ];                 
}

void mcoDisable( void )
{
    RCC->CFGR &= ~RCC_MCO_CLEAR;
}

//-------------------------------------End Pygmy OS XTAL--------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//-----------------------------------Pygmy OS IRQ Handlers------------------------------------
void SysTick_Handler( void )
{
    PYGMY_WATCHDOG_REFRESH; 
    pygmyGlobalData.StopWatch += 100;

    #ifdef __PYGMY_TASKS 
        processTasks();
    #endif
    #ifdef __PYGMY_MESSAGES
        processMessages();
    #endif

}

#ifndef __PYGMY_BOOT
void USART1_IRQHandler( void )
{
    if( USART1->SR & USART_RXNE){
        streamPushChar( COM1, USART1->DR ); 
        if( pygmyGlobalData.Stream[ COM1 ].Get ){
            pygmyGlobalData.Stream[ COM1 ].Get();
        } // if
    } // if
    if( USART1->SR & USART_TXE ){
        streamTXChar( COM1, USART1 );
		
    } // if
    //USART1->SR &= ~USART_RXNE;
    USART1->SR = 0;
}

void USART2_IRQHandler( void )
{
    if( USART2->SR & USART_RXNE){
        streamPushChar( COM2, USART2->DR ); 
        if( pygmyGlobalData.Stream[ COM2 ].Get ){
            pygmyGlobalData.Stream[ COM2 ].Get();
        } // if
    } // if
    if( USART2->SR & USART_TXE ){
        streamTXChar( COM2, USART2 );
    } // if
    USART2->SR = 0;
}


void USART3_IRQHandler( void )
{
    if( USART3->SR & USART_RXNE){
        streamPushChar( COM3, USART3->DR ); 
        if( pygmyGlobalData.Stream[ COM3 ].Get ){
            pygmyGlobalData.Stream[ COM3 ].Get();
        } // if
    } // if
    if( USART3->SR & USART_TXE ){
       //streamTXChar( COM3, USART3 );
	   if( pygmyGlobalData.Stream[ COM3 ].TXLen ){
			--pygmyGlobalData.Stream[ COM3 ].TXLen;  
			USART3->DR = pygmyGlobalData.Stream[ COM3 ].TXBuffer[ pygmyGlobalData.Stream[ COM3 ].TXIndex ]; 
			pygmyGlobalData.Stream[ COM3 ].TXIndex = ( pygmyGlobalData.Stream[ COM3 ].TXIndex + 1 ) % 
				pygmyGlobalData.Stream[ COM3 ].TXBufferLen;
		} else {
			USART3->CR1 &= ~USART_TXEIE;
		} // else
    } // if
    USART3->SR = 0;
}
#endif

#ifdef USART4
void USART4_IRQHandler( void )
{
    if( USART4->SR & USART_RXNE){
        streamPushChar( COM4, USART4->DR ); 
        if( pygmyGlobalData.Stream[ COM4 ].Get ){
            pygmyGlobalData.Stream[ COM4 ].Get();
        } // if
    } // if
    if( USART4->SR & USART_TXE ){
       streamTXChar( COM4, USART4 );
    } // if
    USART4->SR = 0;
}
#endif

#ifdef USART5
void USART5_IRQHandler( void )
{
    if( USART5->SR & USART_RXNE){
        streamPushChar( COM5, USART5->DR ); 
        if( pygmyGlobalData.Stream[ COM5 ].Get ){
            pygmyGlobalData.Stream[ COM5 ].Get();
        } // if
    } // if
    if( USART5->SR & USART_TXE ){
       streamTXChar( COM5, USART5 );
    } // if
    USART5->SR = 0;
}
#endif

//-----------------------------------Pygmy OS IRQ Handlers------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//-----------------------------------------Deprecated-----------------------------------------

/*void Pygmy_SYS_PrintInteger( u8 ucStream, u8 *ucFormat, u32 ulData )
{
    u8 ucBuffer[ 20 ];

    convertIntToString( ulData, ucFormat, ucBuffer );
    Pygmy_SYS_PrintString( ucStream, ucBuffer );
}*/

/*
void Pygmy_SYS_PrintString( u8 ucStream, u8 *ucBuffer )
{
    if(ucStream < PYGMY_SYS_MAXPIPES && pygmyGlobalData.Stream[ ucStream ] ){
        pygmyGlobalData.Stream[ ucStream ]->Put( ucBuffer );
    } // if
}*/
/*
void Pygmy_SYS_PrintTime( u8 ucStream, PYGMYTIME *pygmyTime )
{
    print( ucStream, "%4d-%2d-%2d %2d:%2d:%2d", pygmyTime->Year,pygmyTime->Month,pygmyTime->Day,
        pygmyTime->Hour,pygmyTime->Minute,pygmyTime->Second );
}*/
/*
void resetRXBuffer( u8 ucStream )
{
    pygmyGlobalData.Stream[ ucStream ]->RXIndex = 0;
    //pygmyGlobalData.Stream[ ucStream ]->RXInIndex = 0;
    pygmyGlobalData.Stream[ ucStream ]->RXBuffer[ 0 ] = '\0';
}*/
//const u8 PYGSYS_BITS[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
//const u8 LCD_HEXCHARS[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
//u8 pygmyGlobalCOM1Buffer[ PYGMY_USART1_BUFFERLEN ];
/*PYGMYTASK           pygmyGlobalTasks[ PYGMY_MAXTASKS ];
PYGMYMESSAGE        pygmyGlobalMessages[ PYGMY_MAXMESSAGES ];
PYGMYATCOMMANDQUEUE pygmyGlobalCmdQueue[ PYGMY_MAXATCOMMANDS ];
PYGMYSYSTEM     pygmyGlobalData;
PYGMYFIFO       pygmyUSART1;
//PYGMYFIFO       pygmyDTMF;
PYGMYFIFO       pygmyUSART3;
PYGMYFIFO       pygmyFileFIFO;
u8 globalDTMFBuffer[64];
u8 globalUSART3Buffer[ 128 ];
u8 globalUSART1Buffer[ 640 ];
*/

//---------------------------------------End Deprecated---------------------------------------
//--------------------------------------------------------------------------------------------

