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


#include <stdarg.h>
#include <stdlib.h>
//#include <malloc.h>
//#include "alloc.h"
#include "pygmy_profile.h"
//#include "pygmy_gui.h"
//const u8 PYGSYS_INVBITS[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
const u8 PYGMY_DAYSINMONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
//const u32 PYGMY_BITMASKS[] = { BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11,BIT12,BIT13,BIT14,BIT15,
//                              BIT16,BIT17,BIT18,BIT19,BIT20,BIT21,BIT22,BIT23,BIT24,BIT25,BIT26,BIT27,BIT28,BIT29,BIT30,BIT31};
//const u32 PYGMY_INVBITMASKS[] = {BIT31,BIT30,BIT29,BIT28,BIT27,BIT26,BIT25,BIT24,BIT23,BIT22,BIT21,BIT20,BIT19,BIT18,BIT17,BIT16,
//                               BIT15,BIT14,BIT13,BIT12,BIT11,BIT10,BIT9,BIT8,BIT7,BIT6,BIT5,BIT4,BIT3,BIT2,BIT1,BIT0};

// Global System Variables                          
                           
PYGMYTASK           pygmyGlobalTasks[ PYGMY_MAXTASKS ];
PYGMYMESSAGE        pygmyGlobalMessages[ PYGMY_MAXMESSAGES ];
PYGMYSYSTEM         pygmyGlobalData;

PYGMYCOMMANDQUEUE   *pygmyGlobalQueues[ PYGMY_MAXQUEUES ];
/*#ifndef __PYGMY_SYS_HEAPLEN 
    #ifdef __PYGMYNEBULA_HP
        #define __PYGMY_SYS_HEAPLEN 92160
    #else
        #define __PYGMY_SYS_HEAPLEN 2048
    #endif
#endif
*/
//u8 globalHeap[ __PYGMY_SYS_HEAPLEN ];
                           
//--------------------------------------------------------------------------------------------
//-------------------------------Pygmy OS Memory Allocation Wrappers--------------------------     
void HardFault_Handler( void )
{ 
  print( COM3, "\rHard Fault" );
 // PYGMY_RESET;
}

/*void *sysAllocate( u32 ulLen )
{
    // u32 *ptr;

    //__heap_lock();
    //ptr = malloc( ulLen );
    //__heap_unlock();
    //return( NULL );
    return( malloc( ulLen ) );
}

void *sysReallocate( void *vPtr, u32 ulLen )
{
    return( realloc( vPtr, ulLen ) ); 
}

void sysFree( void *vPtr )
{
    free( vPtr );
}

void sysHeapInit( void )
{
    u32 *vPtr;

    for( vPtr = (u32*)0x20002000; (u32)vPtr < 0x20016000; vPtr++ ){
        *vPtr = 0x00000000;
    } // for
}*/

//-------------------------------Pygmy OS Memory Allocation Wrappers--------------------------
//--------------------------------------------------------------------------------------------
  
//--------------------------------------------------------------------------------------------
//-----------------------------------Pygmy OS Basic Functions--------------------------------- 
u8 sysInit( void )
{
    u16 i;

    //RCC->APB2ENR |= (RCC_IOPBEN|RCC_IOPCEN|RCC_IOPAEN);
    //pygmyGlobalData.Status = 0;
    //pygmyGlobalData.MCUID = fpecMCUID( );
    //pygmyGlobalData.XTAL = PYGMY_XTAL;
    //pygmyGlobalData.PLL = BIT16|BIT1;

    if( pygmyGlobalData.MCUID == DESC_STM32L152 ){
        pygmyGlobalData.FlashControl  = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY1;
        pygmyGlobalData.MainClock     = 32000000;
        pygmyGlobalData.DelayTimer    = PYGMY_TIMER11;
        pygmyGlobalData.PWMTimer      = PYGMY_TIMER9;
    } else if( pygmyGlobalData.MCUID == DESC_STM32F100MD || pygmyGlobalData.MCUID == DESC_STM32F100HD ){
        pygmyGlobalData.MainClock     = 24000000;
        pygmyGlobalData.DelayTimer    = PYGMY_TIMER15;
        pygmyGlobalData.PWMTimer      = PYGMY_TIMER16;
    } else if( pygmyGlobalData.MCUID == DESC_STM32F103XLD ){
        pygmyGlobalData.FlashControl  = (FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2);
        pygmyGlobalData.PLL           = RCC_PLL_X9|BIT16|BIT15|BIT14|BIT1;
        pygmyGlobalData.MainClock     = 72000000;
        pygmyGlobalData.DelayTimer    = PYGMY_TIMER9;
        pygmyGlobalData.PWMTimer      = PYGMY_TIMER10;
        PYGMY_RCC_TIMER9_ENABLE;
        PYGMY_RCC_TIMER10_ENABLE;
    } else if ( pygmyGlobalData.MCUID == DESC_STM32F103HD ){
        pygmyGlobalData.FlashControl  = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2;
        pygmyGlobalData.MainClock     = 72000000;
        pygmyGlobalData.DelayTimer    = PYGMY_TIMER1;
        pygmyGlobalData.PWMTimer      = PYGMY_TIMER8;
    } else{ // STM32F103LD and MD
        pygmyGlobalData.FlashControl  = FPEC_ACR_PRFTBE | FPEC_ACR_LATENCY2; 
        pygmyGlobalData.MainClock     = 72000000;
        pygmyGlobalData.DelayTimer    = PYGMY_TIMER1;
        pygmyGlobalData.PWMTimer      = PYGMY_TIMER0;
    } // else
    FPEC->ACR = pygmyGlobalData.FlashControl;
    PYGMY_RCC_HSI_ENABLE;
    PYGMY_RCC_HSE_ENABLE;
    while( !PYGMY_RCC_HSE_READY );
    RCC->CFGR2 = 0;
    RCC->CFGR = pygmyGlobalData.PLL;
    PYGMY_RCC_PLL_ENABLE;
    while( !PYGMY_RCC_PLL_READY );
    sysEnableTimerClock( pygmyGlobalData.DelayTimer );
    // The following implements default stream config from the profile
    // If no defaults are defined, configuration must be done manually
    /*GPIOB->CRH &= ~( PIN10_CLEAR | PIN11_CLEAR );
    GPIOB->CRH |= ( PIN10_OUT50_ALTPUSHPULL | PIN11_IN_FLOAT );

    USART3->BRR = ( ( (72000000 >> 3 ) / 2304000 ) << 4 ) + ( ( ( 72000000 / 230400 ) ) & 0x0007 );
    //USART3->BRR = ( ( (ulClock >> 3 ) / 9600 ) << 4 ) + ( ( ( ulClock / 9600 ) ) & 0x0007 );
    USART3->CR3 = USART_ONEBITE;
    USART3->CR1 = ( USART_OVER8 | USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
    return;*/
    #ifdef __PYGMYSTREAMS
        streamInit();
        
        #ifdef __PYGMYSTREAMSOCKET
            #ifdef __PYGMYCOMMANDS
                streamSetGet( SOCKET, cmdGetsSOCKET );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( SOCKET, globalSocketBuffer, globalSocketBufferLen );
            streamSetPut( SOCKET, putsSOCKET );
        #endif
        #ifdef __PYGMYSTREAMUSB
        
        #endif
        #ifdef __PYGMYSTREAMCOM1
            #ifdef __PYGMYCOMMANDS
                streamSetGet( COM1, cmdGetsCOM1 );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( COM1, globalCOM1RXBuffer, __PYGMYCOM1BUFFERLEN );
            streamSetPut( COM1, putsUSART1 );
        #endif
        #ifdef __PYGMYSTREAMCOM2
            #ifdef __PYGMYCOMMANDS
                streamSetGet( COM2, cmdGetsCOM2 );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( COM2, globalCOM2RXBuffer, __PYGMYCOM2BUFFERLEN );
            streamSetPut( COM2, putsUSART2 );
        #endif
        #ifdef __PYGMYSTREAMCOM3 // USB and STDIO on Nebulas
            comOpen( COM3, 230400, 0 );
            #ifdef __PYGMYCOMMANDS
                streamSetGet( COM3, cmdGetsCOM3 );
                streamEnableEcho( COM3 );
                streamEnableBackspace( COM3 );
                streamEnableActionChars( COM3 );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( COM3, globalCOM3RXBuffer, __PYGMYCOM3BUFFERLEN );
            streamSetPut( COM3, putsUSART3 );
        #endif
        #ifdef __PYGMYSTREAMCOM4 
            #ifdef __PYGMYCOMMANDS
                streamSetGet( COM4, cmdGetsCOM4 );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( COM4, globalCOM4RXBuffer, __PYGMYCOM4BUFFERLEN );
            streamSetPut( COM4, putsUSART4 );
        #endif
        #ifdef __PYGMYSTREAMCOM5
            #ifdef __PYGMYCOMMANDS
                streamSetGet( COM5, cmdGetsCOM5 );
            #endif // __PYGMYCOMMANDS
            streamSetRXBuffer( COM5, globalCOM5RXBuffer, __PYGMYCOM5BUFFERLEN );
            streamSetPut( COM5, putsUSART5 );
        #endif
        #ifdef __PYGMYFILES
            #ifdef __PYGMYSTREAMFILE
                streamSetPut( FILE, putsFILE );
            #endif
        #endif
        #ifdef __PYGMYSTREAMLCD
            
            streamSetPut( LCD, putsLCD );
        #endif
        #ifdef __PYGMYNEBULA
            streamSetSTDIO( COM3 );
        #endif
        #ifdef __PYGMYCOMMANDS
            cmdInit();
        #endif // __PYGMYCOMMANDS
    #endif
    timeInit();
    #ifdef __PYGMYFILES
        #ifdef __PYGMY_PFATLITE
            fileMountRoot();
        #endif
    #endif
    
    #ifdef __PYGMYTASKS
        taskInit();
    #endif
    #ifdef __PYGMYMESSAGES
        msgInit();
    #endif
    #ifdef __PYGMYLCDSHIELD
        lcdInit();
    #endif
    #ifdef __PYGMYMODEMSHIELD
        modemInit();
    #endif
    for( i = 0; i < PYGMY_MAXQUEUES; i++ ){
        pygmyGlobalQueues[ i ] = NULL;
    } // for
    
	return( 0 );
}

float sysGetADCVoltage( void )
{
    return( 3.3 );
}

/*
void sysSetPipe( u8 ucPortFrom, u8 ucPortTo )
{
    // ToDo: Finish stream piping
    if( ucPortFrom >= MAXCOMPORTS || ucPortTo >= MAXCOMPORTS ){
        return;
    } // if
    #ifdef __PYGMYCOMMANDS
    streamSetGet( COM3, cmdGetsCOM3 );
    #endif // __PYGMYCOMMANDS
    
    streamSetPut( COM3, putsUSART3 );
}*/

void sysEnableComClock( u8 ucPort )
{
    #ifdef __PYGMYSTREAMCOM1
        if( ucPort == COM1 ){
            PYGMY_RCC_USART1_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM1
    #ifdef __PYGMYSTREAMCOM2
        if( ucPort == COM2 ){
            PYGMY_RCC_USART2_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM2
    #ifdef __PYGMYSTREAMCOM3
        if( ucPort == COM3 ){
            PYGMY_RCC_USART3_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM3
    #ifdef __PYGMYSTREAMCOM4
        if( ucPort == COM4 ){
            PYGMY_RCC_USART4_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM4
    #ifdef __PYGMYSTREAMCOM5
        if( ucPort == COM5 ){
            PYGMY_RCC_USART5_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM5
    #ifdef __PYGMYSTREAMCOM6
        if( ucPort == COM6 ){
            PYGMY_RCC_USART6_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM6
    #ifdef __PYGMYSTREAMSP1
        if( ucPort == SP1 ){
            PYGMY_RCC_SPI1_ENABLE;
        } // if
    #endif // __PYGMYSTREAMSP1
    #ifdef __PYGMYSTREAMSP2
        if( ucPort == SP2 ){
            PYGMY_RCC_SPI2_ENABLE;
        } // if 
    #endif // __PYGMYSTREAMSP2
    #ifdef __PYGMYSTREAMSP3
        if( ucPort == SP3 ){
            PYGMY_RCC_SPI3_ENABLE;
        } // if
    #endif // __PYGMYSTREAMSP3
    #ifdef __PYGMYSTREAMBUS1
        if( ucPort == BUS1 ){
            PYGMY_RCC_I2C1_ENABLE;
        } // if
    #endif // __PYGMYSTREAMBUS1
    #ifdef __PYGMYSTREAMBUS2
        if( ucPort == BUS2 ){
            PYGMY_RCC_I2C2_ENABLE;
        } // if
    #endif // __PYGMYSTREAMBUS2
    #ifdef __PYGMYSTREAMBUS3
        if( ucPort == BUS3 ){
            PYGMY_RCC_I2C3_ENABLE;
        } // if
    #endif // __PYGMYSTREAMBUS3
}
 
u32 sysRandom( u32 ulFrom, u32 ulTo )
{
    u32 ulRand, i;

    //pygmyGlobalData.Random <<= 1;
    //pygmyGlobalData.Random += timeGet();
    pygmyGlobalData.Random += ( pygmyGlobalData.Random << 1 ) ^ ( timeGet() + ADC1->DR + GPIOA->IDR + GPIOB->IDR + GPIOD->IDR + GPIOC->IDR );
    ulRand = pygmyGlobalData.Random;
    for( i = 0; i < 32; i++ ){
        ulRand >>= 1;
        if( ulRand >= ulFrom && ulRand <= ulTo ){
            return( ulRand );
        } // if
    } // for
    ulRand = pygmyGlobalData.Random % ( ulTo - ulFrom );
    if( ulRand > ulTo ) {
        ulRand = ulTo;
    } else if( ulRand < ulFrom ){
        ulRand = ulFrom;
    } // else if

    return( ulRand );
}

void sysDisableComClock( u8 ucPort )
{
    #ifdef __PYGMYSTREAMCOM1
        if( ucPort == COM1 ){
            PYGMY_RCC_USART1_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM1
    #ifdef __PYGMYSTREAMCOM2
        if( ucPort == COM2 ){
            PYGMY_RCC_USART2_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM2
    #ifdef __PYGMYSTREAMCOM3
        if( ucPort == COM3 ){
            PYGMY_RCC_USART3_ENABLE;
        } // if
    #endif // __PYGMYSTREAMCOM3
    #ifdef __PYGMYSTREAMCOM4
        if( ucPort == COM4 ){
            PYGMY_RCC_USART4_DISABLE;
        } // if
    #endif // __PYGMYSTREAMCOM4
    #ifdef __PYGMYSTREAMCOM5
        if( ucPort == COM5 ){
            PYGMY_RCC_USART5_DISABLE;
        } // if
    #endif // __PYGMYSTREAMCOM5
    #ifdef __PYGMYSTREAMCOM6
        if( ucPort == COM6 ){
            PYGMY_RCC_USART6_DISABLE;
        } // if
    #endif // __PYGMYSTREAMCOM6
    #ifdef __PYGMYSTREAMSP1
        if( ucPort == SP1 ){
            PYGMY_RCC_SPI1_DISABLE;
        } // if
    #endif // __PYGMYSTREAMSP1
    #ifdef __PYGMYSTREAMSP2
        if( ucPort == SP2 ){
            PYGMY_RCC_SPI2_DISABLE;
        } // if
    #endif // __PYGMYSTREAMSP2
    #ifdef __PYGMYSTREAMSP3
        if( ucPort == SP3 ){
            PYGMY_RCC_SPI3_DISABLE;
        } // if 
    #endif // __PYGMYSTREAMSP3
    #ifdef __PYGMYSTREAMBUS1
        if( ucPort == BUS1 ){
            PYGMY_RCC_I2C1_DISABLE;
        } // if
    #endif // __PYGMYSTREAMBUS1
    #ifdef __PYGMYSTREAMBUS2
        if( ucPort == BUS2 ){
            PYGMY_RCC_I2C2_DISABLE;
        } // if
    #endif // __PYGMYSTREAMBUS2
    #ifdef __PYGMYSTREAMBUS3
        if( ucPort == BUS3 ){
            PYGMY_RCC_I2C3_DISABLE;
        } // if
    #endif // __PYGMYSTREAMBUS3
}

void sysEnableTimerClock( u8 ucTimer )
{
    if( ucTimer == PYGMY_TIMER1 ){
        PYGMY_RCC_TIMER1_ENABLE;
    } else if( ucTimer == PYGMY_TIMER2 ){
        PYGMY_RCC_TIMER2_ENABLE;
    } else if( ucTimer == PYGMY_TIMER3 ){
        PYGMY_RCC_TIMER3_ENABLE;
    } else if( ucTimer == PYGMY_TIMER4 ){
        PYGMY_RCC_TIMER4_ENABLE;
    } else if( ucTimer == PYGMY_TIMER5 ){
        PYGMY_RCC_TIMER5_ENABLE;
    } else if( ucTimer == PYGMY_TIMER6 ){
        PYGMY_RCC_TIMER6_ENABLE;
    } else if( ucTimer == PYGMY_TIMER7 ){
        PYGMY_RCC_TIMER7_ENABLE;
    } else if( ucTimer == PYGMY_TIMER8 ){
        PYGMY_RCC_TIMER8_ENABLE;
    } else if( ucTimer == PYGMY_TIMER9 ){
        PYGMY_RCC_TIMER9_ENABLE;
    } else if( ucTimer == PYGMY_TIMER10 ){
        PYGMY_RCC_TIMER10_ENABLE;
    } else if( ucTimer == PYGMY_TIMER11 ){
        PYGMY_RCC_TIMER11_ENABLE;
    } else if( ucTimer == PYGMY_TIMER12 ){
        PYGMY_RCC_TIMER12_ENABLE;
    } else if( ucTimer == PYGMY_TIMER13 ){
        PYGMY_RCC_TIMER13_ENABLE;
    } else if( ucTimer == PYGMY_TIMER14 ){
        PYGMY_RCC_TIMER14_ENABLE;
    } else if( ucTimer == PYGMY_TIMER15 ){
        PYGMY_RCC_TIMER15_ENABLE;
    } else if( ucTimer == PYGMY_TIMER16 ){
        PYGMY_RCC_TIMER16_ENABLE;
    } else if( ucTimer == PYGMY_TIMER17 ){
        PYGMY_RCC_TIMER17_ENABLE;
    } // else if
}

void sysDisableTimerClock( u8 ucTimer )
{
    if( ucTimer == PYGMY_TIMER1 ){
        PYGMY_RCC_TIMER1_DISABLE;
    } else if( ucTimer == PYGMY_TIMER2 ){
        PYGMY_RCC_TIMER2_DISABLE;
    } else if( ucTimer == PYGMY_TIMER3 ){
        PYGMY_RCC_TIMER3_DISABLE;
    } else if( ucTimer == PYGMY_TIMER4 ){
        PYGMY_RCC_TIMER4_DISABLE;
    } else if( ucTimer == PYGMY_TIMER5 ){
        PYGMY_RCC_TIMER5_DISABLE;
    } else if( ucTimer == PYGMY_TIMER6 ){
        PYGMY_RCC_TIMER6_DISABLE;
    } else if( ucTimer == PYGMY_TIMER7 ){
        PYGMY_RCC_TIMER7_DISABLE;
    } else if( ucTimer == PYGMY_TIMER8 ){
        PYGMY_RCC_TIMER8_DISABLE;
    } else if( ucTimer == PYGMY_TIMER9 ){
        PYGMY_RCC_TIMER9_DISABLE;
    } else if( ucTimer == PYGMY_TIMER10 ){
        PYGMY_RCC_TIMER10_DISABLE;
    } else if( ucTimer == PYGMY_TIMER11 ){
        PYGMY_RCC_TIMER11_DISABLE;
    } else if( ucTimer == PYGMY_TIMER12 ){
        PYGMY_RCC_TIMER12_DISABLE;
    } else if( ucTimer == PYGMY_TIMER13 ){
        PYGMY_RCC_TIMER13_DISABLE;
    } else if( ucTimer == PYGMY_TIMER14 ){
        PYGMY_RCC_TIMER14_DISABLE;
    } else if( ucTimer == PYGMY_TIMER15 ){
        PYGMY_RCC_TIMER15_DISABLE;
    } else if( ucTimer == PYGMY_TIMER16 ){
        PYGMY_RCC_TIMER16_DISABLE;
    } else if( ucTimer == PYGMY_TIMER17 ){
        PYGMY_RCC_TIMER17_DISABLE;
    } // else if
}

u8 sysEnableTimerInterrupt( u8 ucTimer )
{
    u8 ucIRQ;
    
    ucIRQ = interruptGetTimerVector( ucTimer );
    if( ucIRQ ){
        interruptEnable( ucIRQ );
        return( 1 );
    } // if
    
    return( 0 ); // Invalid timer selected
}

u8 sysGetDelayTimer( void )
{
    return( pygmyGlobalData.DelayTimer );
}

u8 sysGetPWMTimer( void )
{
    return( pygmyGlobalData.PWMTimer );
}

u32 sysGetMainClock( void )
{
    return( pygmyGlobalData.MainClock );
}

void sysSetMainClock( u32 ulFreq )
{
    // ToDo: Add preverification code for clock
    // ToDo: Add code to apply clock settings to RCC
    pygmyGlobalData.MainClock = ulFreq;
}

u32 sysGetXTAL( void )
{
    return( pygmyGlobalData.XTAL );
}

void sysSetXTAL( u32 ulFreq )
{
    pygmyGlobalData.XTAL = ulFreq;
}
//-----------------------------------Pygmy OS Basic Functions---------------------------------
//--------------------------------------------------------------------------------------------
 
//--------------------------------------------------------------------------------------------
//----------------------------------Pygmy OS Command/Response---------------------------------
#ifdef __PYGMYCOMMANDS
void cmdInitQueue( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXQUEUES; i++ ){
        if( !pygmyGlobalQueues[ i ] ){
            pygmyGlobalQueues[ i ] = pygmyCmdQueue;
            break;
        } // if
    } // for
    pygmyCmdQueue->Index = 0;
    pygmyCmdQueue->Count = 0;
    for( i = 0; i < PYGMY_MAXCOMMANDS; i++ ){
        cmdDelete( pygmyCmdQueue, i );
    } // for
}

void cmdPrint( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler();
}

void cmdDelete( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u16 uiIndex )
{
    print( STDIO, "\rCmdDelete %s", pygmyCmdQueue->Commands[ uiIndex ].Name );
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

    cmdProcess( pygmyCmdQueue ); // Call to move on to next queued command, if any
}

void cmdProcess( PYGMYCOMMANDQUEUE *pygmyCmdQueue ) 
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
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire += timeGet();
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler( );//(u8*)PYGMY_CMD_NULL );
            pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status |= PYGMY_AT_TX;
        } // if
        if( pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire <= timeGet() ){
            if( !pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry ){
                cmdDelete( pygmyCmdQueue, pygmyCmdQueue->Index );
                if( pygmyCmdQueue->Count ){
                    cmdProcess( pygmyCmdQueue );
                } // if
            } else{
                --pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry;
                pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status &= ~PYGMY_AT_TX;
                pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status |= PYGMY_AT_RETRY; // Prevents possible skipping to the next command prematurely
            } // else
        } // if
    } 
}

void cmdReplace( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd )
{
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Status        = PYGMY_AT_QUEUED;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry         = pygmyCmd->Retry;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Name          = pygmyCmd->Name;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].EventHandler  = pygmyCmd->EventHandler;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].PrintHandler  = pygmyCmd->PrintHandler;
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].TimeStamp     = timeGet();
    pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Expire        = pygmyCmd->Expire;

    ++pygmyCmdQueue->Count;
    cmdProcess( pygmyCmdQueue );
}

u8 cmdGetRetriesRemaining( PYGMYCOMMANDQUEUE *pygmyCmdQueue )
{
    // returns retries for command currently executing
    
    return( pygmyCmdQueue->Commands[ pygmyCmdQueue->Index ].Retry );
}

u8 cmdIsQueued( PYGMYCOMMANDQUEUE *pygmyCmdQueue, u8 *ucName  )
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
    } // for

    return( ucCount );
}

u8 cmdQueue( PYGMYCOMMANDQUEUE *pygmyCmdQueue, PYGMYCOMMAND *pygmyCmd )
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
        pygmyCmdQueue->Commands[ uiIndex ].TimeStamp     = timeGet();
        pygmyCmdQueue->Commands[ uiIndex ].Expire        = pygmyCmd->Expire;
        print( STDIO, "\rCmdQueue %s", pygmyCmdQueue->Commands[ uiIndex ].Name );
        ++pygmyCmdQueue->Count;
        cmdProcess( pygmyCmdQueue );
        return( 1 );
    } 
        
    return( 0 );
}

void *cmdListQueue( u16 uiIndex )
{
    return( pygmyGlobalQueues[ uiIndex ] );
}

#endif // __PYGMYCOMMANDS
//--------------------------------End Pygmy OS Command/Response-------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//--------------------------------Pygmy OS Data Integrity Algorithm---------------------------
u32 sysGenerateBit( u8 ucBit )
{
    u32 ulData = BIT0;
    u8 i;

    if( ucBit > 32 ){
        return( 0 );
    } // if
    
    for( i = 0; i < ucBit; i++ ){
        ulData <<= 1;
    } // for
    
    return( ulData );
}

u8 sysFlipU8( u8 ucData )
{
    return( sysFlipBits( ucData, 8 ) );
}

u16 sysFlipU16( u16 uiData )
{
    return( sysFlipBits( uiData, 16 ) );
}

u32 sysFlipU32( u32 ulData )
{
    return( sysFlipBits( ulData, 32 ) );
}

u32 sysFlipBits( u32 ulData, u8 ucBits )
{
    // ucBits is intended to be in 8 bit increments, not required though
    u32 ulFlip;
    u8 i;
    
    for( i = 0, ulFlip = 0; i < ucBits; i++ ){
        ulFlip |= ( ulData & BIT0 );
        ulFlip <<= 1;
        ulData >>= 1;
    } // for

    return( ulFlip );
}

u8 sysCRC8( u8 *ucBuffer, u16 uiLen )
{
    u16 i;
    u8 ucCRC;

    for( i = 0, ucCRC = 0; i < uiLen; i++ ){
        ucCRC += ucBuffer[ i ];
    } // for
    ucCRC = 1 + ( 0xFF ^ (u8)ucCRC );

    return( ucCRC );
}

u16 sysCRC16( u8 *ucBuffer, u16 uiLen )
{
    u16 i, uiCRC;

    for( i = 0, uiCRC = 0; i < uiLen; i++ ){
        uiCRC += ucBuffer[ i ];
    } // for
    uiCRC = 1 + ( 0xFFFF ^ (u16)uiCRC ); 
    
    return( uiCRC );
}

u32 pdiaEncodeString( u8 *ucString )
{
    u32 ulSum;
    // Warning!!! Only use with strings under 16384 bytes long!
    //print( COM3, "\rPDIA (%s): ", ucString );
    pdiaEncode( 0, PDIA_NEW, &ulSum );
    for( ; *ucString; ){
        pdiaEncode( *(ucString++), PDIA_ADD, &ulSum );
    } // for
    pdiaEncode( 0, PDIA_END, &ulSum );
    //print( COM3, "0x%08X", ulSum);
    return( ulSum );
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

#ifdef __PYGMYSTREAMS
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
        globalStreams[ ucStream ].Put( ucBuffer );
    }
}
#endif // __PYGMYSTREAMS
//-----------------------------End Pygmy OS Data Integrity Algorithm--------------------------
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//--------------------------------------Pygmy OS Tasking--------------------------------------
#ifdef __PYGMYTASKS
u16 taskProcess( void )
{
    u32 ulTime;
    u16 i;

    ulTime = timeGet();
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( pygmyGlobalTasks[ i ].ID != PYGMY_BLANKID ){
            if( pygmyGlobalTasks[ i ].Expire ){
                if( pygmyGlobalTasks[ i ].Expire == ulTime ){
                    //taskDelete( "", pygmyGlobalTasks[ i ].ID );
                    taskDelete( pygmyGlobalTasks[ i ].Name );
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

void taskInit( void )
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
    pygmyGlobalData.Status |= PYGMY_TASK_INIT;
}

u8 taskNewSimple( u8 *ucName, u32 ulTimer, PYGMYFUNCPTR EventHandler )
{
    // ID = 0 calls for auto generated ID
    // Timer = 2, 1 is reload, this provides approx 1 millisec delay
    // Reload = ulTimer, This value will be loaded into timer on each reload at 1
    // Expire = 0, doesn't expire

    //return( taskNew( ucName, 0, 2, ulTimer, 0, EventHandler ) ); // return ID or 0 for fail
    return( taskNew( ucName, ulTimer, ulTimer, 0, EventHandler ) );
}

//u16 taskNew( u8 *ucName, u16 uiID, u32 ulTimer, u32 ulReload, u32 ulExpire, PYGMYFUNCPTR EventHandler )
u8 taskNew( u8 *ucName, u32 ulTimer, u32 ulReload, u32 ulExpire, PYGMYFUNCPTR EventHandler )
{ 
    u16 i;

    //if( uiID >= PYGMY_SERVICEDID ){
    //    return( 0 );
    //} // if
    /*for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( uiID && ( pygmyGlobalTasks[ i ].ID == uiID || 
            isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ) ){
            return( 0 );
            //return( PYGMY_TASK_EXISTS );
        } // if
    } // for
    */
    if( taskIsRunning( ucName ) ){
        return( FALSE );
    } // if
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        if( pygmyGlobalTasks[ i ].ID == PYGMY_BLANKID ){
            //if( uiID ){
            //    pygmyGlobalTasks[ i ].ID = uiID; // ID was provided
            //} else{
                pygmyGlobalTasks[ i ].ID = PYGMY_TEMPID | i; // Generate unique ID
            //} // else
            pygmyGlobalTasks[ i ].Busy          = 0;
            pygmyGlobalTasks[ i ].Name          = ucName;
            pygmyGlobalTasks[ i ].Timer         = ulTimer;
            if( ulTimer ){               // Do not allow Reload if Timer == 0
                pygmyGlobalTasks[ i ].Reload    = ulReload;
            } else{
                pygmyGlobalTasks[ i ].Reload    = 0;
            } // else
            pygmyGlobalTasks[ i ].EventHandler  = EventHandler;
            pygmyGlobalTasks[ i ].TimeStamp     = timeGet( );
            pygmyGlobalTasks[ i ].Expire        = ulExpire;

            //return( PYGMY_TASK_SUCCESS );
            return( TRUE );//pygmyGlobalTasks[ i ].ID );
        } // if
    } // for

    return( FALSE );
}

//u8 taskIsRunning( u8 *ucName, u16 uiID )
u8 taskIsRunning( u8 *ucName )
{
    // Return 1 for task found and 0 for not found
    u16 i, uiLen;
    
    uiLen = taskGetCount();
    for( i = 0; i < uiLen; i++ ){
        if( pygmyGlobalTasks[ i ].Name && isStringSame( pygmyGlobalTasks[ i ].Name, ucName ) ){
            
            return( TRUE );
        } // if
    }//for

    return( FALSE );
}

// u8 taskDelete( u8 *ucName, u16 uiID )
u8 taskDelete( u8 *ucName )
{
    u16 i, uiLen; 
	//u8 ucDeleted;
    
    // This loop iterates to MAX to delete all instances of a given name
    uiLen = taskGetCount();
    for( i = 0; i < uiLen; i++ ){
        //if( ( uiID && pygmyGlobalTasks[ i ].ID == uiID ) || isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ){
        if( pygmyGlobalTasks[ i ].Name && isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ){
            pygmyGlobalTasks[ i ].Name          = NULL;
            pygmyGlobalTasks[ i ].ID            = PYGMY_BLANKID;
            pygmyGlobalTasks[ i ].Timer         = 0;
            pygmyGlobalTasks[ i ].Reload        = 0;
            pygmyGlobalTasks[ i ].EventHandler  = ( void * ) TaskException_Handler;
            pygmyGlobalTasks[ i ].TimeStamp     = 0;
            return( TRUE );
        } // if
    } // for
   
    return( FALSE );
}

//u8 taskGet( u8 *ucName, u16 uiID, PYGMYTASK *pygmyTask )
PYGMYTASK  *taskGet( u8 *ucName )
{
    u16 i, uiLen;

    uiLen = taskGetCount();
    for( i = 0; i < uiLen; i++ ){
        if( pygmyGlobalTasks[ i ].Name && isStringSame( ucName, pygmyGlobalTasks[ i ].Name ) ){
            //pygmyTask->Name = pygmyGlobalTasks[ i ].Name;
            //pygmyTask->ID = pygmyGlobalTasks[ i ].ID;
            //pygmyTask->Timer = pygmyGlobalTasks[ i ].Timer;
            //pygmyTask->Reload = pygmyGlobalTasks[ i ].Reload;
            //pygmyTask->EventHandler = pygmyGlobalTasks[ i ].EventHandler;
            //pygmyTask->TimeStamp = pygmyGlobalTasks[ i ].TimeStamp;
            //pygmyTask->Expire = pygmyGlobalTasks[ i ].Expire;
            
            return( &pygmyGlobalTasks[ i ] );
        } // if
    } // for
    
    return( NULL );
    /*u16 i;

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
    
    return( 0 );*/
}

u16 taskGetCount( void )
{
    // update with count value once tasks are dynamically allocated
    return( PYGMY_MAXTASKS );
}

PYGMYTASK *taskGetByIndex( u16 uiIndex )
{
    if( uiIndex >= taskGetCount() ){
        return( NULL );
    } // if
    if( pygmyGlobalTasks[ uiIndex ].ID == PYGMY_BLANKID ){
        return( NULL );
    } // if

    return( &pygmyGlobalTasks[ uiIndex ] );
}
/*
void taskList( PYGMYTASK *pygmyTask, u16 uiTask )
{
    pygmyTask->ID               = pygmyGlobalTasks[ uiTask ].ID;
    pygmyTask->Name             = pygmyGlobalTasks[ uiTask ].Name;
    pygmyTask->Timer            = pygmyGlobalTasks[ uiTask ].Timer;
    pygmyTask->Reload           = pygmyGlobalTasks[ uiTask ].Reload;
    pygmyTask->EventHandler     = pygmyGlobalTasks[ uiTask ].EventHandler;
    pygmyTask->TimeStamp        = pygmyGlobalTasks[ uiTask ].TimeStamp;
    pygmyTask->Expire           = pygmyGlobalTasks[ uiTask ].Expire;
}*/
#endif // __PYGMYTASKS

void TaskException_Handler( void )
{
    // ToDo: Add any relevant debug or cleanup code here
}

//------------------------------------End Pygmy OS Tasking------------------------------------
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//-------------------------------------Pygmy OS Messaging-------------------------------------
#ifdef __PYGMYMESSAGES
u16 msgInit( void )
{
    u16 i;

    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){
        pygmyGlobalMessages[ i ].DestID         = PYGMY_BLANKID;
        pygmyGlobalMessages[ i ].DestName       = NULL;
        pygmyGlobalMessages[ i ].Message        = NULL;
        pygmyGlobalMessages[ i ].Value          = 0;
        pygmyGlobalMessages[ i ].TimeStamp      = 0;
    } // for
    pygmyGlobalData.Status |= PYGMY_MSG_INIT;

    return( 1 );
}

u8 msgDelete( u8 *ucName, u16 uiID )
{
    u16 i;
    
    i = msgGetIndex( ucName, uiID );
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

u8 msgIs( u8 *ucName, u16 uiID )
{
    if( msgGetIndex( ucName, uiID ) == PYGMY_NOMESSAGE ){
        return( 0 );
    } // if
    
    return( 1 );
}

u16 msgGetIndex( u8 *ucName, u16 uiID )
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

u8 msgGet( u8 *ucName, u16 uiID, PYGMYMESSAGE *pygmyMsg )
{
    u16 i;

    i = msgGetIndex( ucName, uiID );
    if( i != PYGMY_NOMESSAGE ){
        
        pygmyMsg->DestName          = pygmyGlobalMessages[ i ].DestName;
        pygmyMsg->Message           = pygmyGlobalMessages[ i ].Message;
        pygmyMsg->Value             = pygmyGlobalMessages[ i ].Value;
        pygmyMsg->TimeStamp         = pygmyGlobalMessages[ i ].TimeStamp;
            
        msgDelete( ucName, uiID );
        return( 1 );
    } // if
    
    return( 0 );
}

u8 msgSend( u8 *ucName, u16 uiID, u8 *ucMessage, u32 ulValue )
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
            pygmyGlobalMessages[ i ].Value      = ulValue;
            pygmyGlobalMessages[ i ].TimeStamp  = timeGet();
            
            return( 1 );
        } // if
    } // for
    
    return( 0 );
}

u16 msgProcess( void )
{
    u32 ulTime;
    u16 i, ii;

    ulTime = timeGet();
    //print( COM3, "\rProcessing Messages" );
    for( i = 0; i < PYGMY_MAXMESSAGES; i++ ){                       // Cycle through messages to check for any unserviced
        //print( COM3, "." );
        if (  pygmyGlobalMessages[ i ].DestID && !( pygmyGlobalMessages[ i ].DestID & PYGMY_SERVICEDID ) ){// Serviced ID set after task notified
            // Check to make sure message hasn't gone stale, delete if it has
            
            if( pygmyGlobalMessages[ i ].TimeStamp + 5 == ulTime ){
                msgDelete( pygmyGlobalMessages[ i ].DestName, pygmyGlobalMessages[ i ].DestID );
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

void msgList( PYGMYMESSAGE *pygmyMsg, u16 uiMsg )
{
    pygmyMsg->DestID           = pygmyGlobalMessages[ uiMsg ].DestID;     
    pygmyMsg->DestName         = pygmyGlobalMessages[ uiMsg ].DestName;         
    pygmyMsg->Message          = pygmyGlobalMessages[ uiMsg ].Message;
    pygmyMsg->Value            = pygmyGlobalMessages[ uiMsg ].Value;
    pygmyMsg->TimeStamp        = pygmyGlobalMessages[ uiMsg ].TimeStamp;
}
#endif
//-----------------------------------End Pygmy OS Messaging-----------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//----------------------------------------Pygmy OS ID-----------------------------------------

u8 *sysGetIDString( void ) // ( u8 *ucBuffer )
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

u8 *sysGetID( void )
{
    // Returns unique device ID
    u8 *ucID;
    
    ucID = (u8*)0x1FFFF7E8; // Address of 96 bit Unique ID
    
    return( ucID );
}

void *sysGetFile( void )
{
    return( &pygmyGlobalData.File );
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
    
    u32 *ulID;
    
    ulID = (u32*)(0x08000000 + ( ( SIZEREG->Pages - 2 ) * 1024 )); 
    //print( COM3, "\rID Address: %8X", ulID );
    return( *ulID );
}

u32 sysGetIDRevision( void )
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
#ifdef __PYGMYSTREAMS
void println( u8 ucStream, u8 *ucBuffer, ... )
{
    print( ucStream, ucBuffer );
    print( ucStream, "\r" );
}

void print( u8 ucStream, u8 *ucBuffer, ... )
{
    PYGMYTIME pygmyTime;
    u8 i, ii, ucValue[ 3 ], *sValue, sFormat[ 16 ], ucIntBuffer[ 40 ], iType;
    u16 uiLen;
    va_list ap;
     
    if( ucBuffer == NULL ){
        return;
    } // if
    ucValue[ 2 ] = 0;
    va_start( ap, ucBuffer );
    for( ; *ucBuffer; ucBuffer++ ){
        
        if( *ucBuffer == '%' ){ // Found format specifier
            // first collect precision, if any
            for( i = 0; i < 12 && ( isNumeric( *(++ucBuffer) ) || *ucBuffer == '-' 
                || *ucBuffer == '+' || *ucBuffer == '.' ); i++ ){
                sFormat[ i ] = *ucBuffer;
            } // for
            sFormat[ i ] = 0; // Terminate format string
            if( *ucBuffer == '%' ){ // Format specifier was only escaping '%' char, print
                ucValue[ 0 ] = '%';
                globalStreams[ ucStream ].Put( ucValue );
            } else if( *ucBuffer == 'c' ){ 
                ucValue[ 0 ] = (u8)va_arg( ap, int );
                 globalStreams[ ucStream ].Put( ucValue );
            } else if( *ucBuffer == 's' ){
                sValue = va_arg( ap, u8 * );
                uiLen = len( sValue );
                
                ucValue[ 0 ] = ' ';
                if( sFormat[ 0 ] == '-' ){
                    globalStreams[ ucStream ].Put( sValue );
                    ii = convertStringToInt( (u8*)(sFormat+1) );
                    if( ii > uiLen ){
                        ii -= uiLen;
                    } else{
                        ii = 0;
                    } // else
                    for( i = 0; i < ii; i++ ){
                         globalStreams[ ucStream ].Put( ucValue );
                    } // for
                } else{
                    ii = convertStringToInt( sFormat );
                    if( ii > uiLen ){
                        ii -= uiLen;
                    } else{
                        ii = 0;
                    } // if
                    for( i = 0; i < ii; i++ ){
                         globalStreams[ ucStream ].Put( ucValue );
                    } // for
                    globalStreams[ ucStream ].Put( sValue );
                } // else
            } else if( *ucBuffer == 'i' || *ucBuffer == 'd' ||
                *ucBuffer == 'x' || *ucBuffer == 'X' || *ucBuffer == 'o' || *ucBuffer == 'f' || *ucBuffer == 'l' ){
                iType = 16;
                if( *ucBuffer == 'l' ){
                    ++ucBuffer;
                    if( *ucBuffer == 'l' ){
                        ++ucBuffer;
                        iType = 64;
                    } // if
                } // if
                sFormat[ i++ ] = *ucBuffer; // PrintInteger requires format char
                sFormat[ i ] = 0; // terminate at new index
                
                if( iType == 64 ){
                    convertIntToString( va_arg( ap, u64 ), sFormat, ucIntBuffer ); 
                } else if( *ucBuffer == 'f' ){
                    convertFloatToString( va_arg( ap, double ), sFormat, ucIntBuffer );
                } else{
                    convertIntToString( va_arg( ap, u32 ), sFormat, ucIntBuffer ); 
                } // else
                globalStreams[ ucStream ].Put( ucIntBuffer );
            //} else if( *ucBuffer == 'f' ){
                
            } else if( *ucBuffer == 't' ){
                convertSecondsToSystemTime( va_arg( ap, s32 ), &pygmyTime );
                print( ucStream, "%04d-%02d-%02d %02d:%02d:%02d", pygmyTime.Year,pygmyTime.Month,pygmyTime.Day,
                    pygmyTime.Hour,pygmyTime.Minute,pygmyTime.Second );
            }
        } else{
            ucValue[ 0 ] = *ucBuffer;
            ucValue[ 1 ] = 0;
            globalStreams[ ucStream ].Put( ucValue );
        }
    } // for
    va_end( ap );
}


void streamSetPrintFile( void *pygmyFile )
{
    pygmyGlobalData.File = pygmyFile;
}

#ifdef __PYGMYSTREAMFILE
u8 putsFILE( u8 *ucBuffer )
{
    return( filePutString( pygmyGlobalData.File, ucBuffer ) );
}
#endif // __PYGMYSTREAMFILE
#endif // __PYGMYSTREAMS
//-------------------------------------End Pygmy OS Print-------------------------------------
//--------------------------------------------------------------------------------------------





//--------------------------------------------------------------------------------------------
//---------------------------------------Pygmy OS XTAL----------------------------------------

void setMainClock( u32 ulClock )
{
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

#pragma push_options
#pragma optimize ("O0")
void delayms( u32 ulDelay )
{
    delay( ulDelay * 1000 );
}

void delay( u32 ulDelay )
{
    // This function uses a general purpose timer to provide an accurate microsecond delay
    // MainClock must be set to 1MHz min, 8MHz or higher recommended
    
    TIMER *pygmyTimer;
    u32 i;
    
    PYGMY_WATCHDOG_REFRESH;
    if( pygmyGlobalData.DelayTimer == PYGMY_TIMER1 ){
        // F103LD, F103MD, F103HD
        // Warning! F103 devies with less than 768KB Flash do not have extra
        // multipurpose timers and must share Timer1. In this case, Timer1
        // should not be used for PWM output. 
        
        PYGMY_RCC_TIMER1_ENABLE;
        TIM1->CR1 = 0;                          // Disable before configuring timer
        ulDelay = (( pygmyGlobalData.MainClock / 1000000 ) * ulDelay);
        if( ulDelay > 0x0000FFFF ){
            TIM1->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
            ulDelay &= 0x0000FFFF;
        } // 
        print( COM3, "\rDelay: %d", ulDelay );
        //ulDelay = (( pygmyGlobalData.MainClock / 1000000 ) * ulDelay);
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
    } else {
        //
        pygmyTimer = sysGetTimer( pygmyGlobalData.DelayTimer );
        PYGMY_RCC_TIMER9_ENABLE;
        pygmyTimer->CR1 = 0;                          // Disable before configuring timer
        ulDelay *= ( pygmyGlobalData.MainClock / 1000000 );
        if( ulDelay > 0x0000FFFF ){
            pygmyTimer->PSC = ( pygmyGlobalData.MainClock / 1000000 ) * ( ulDelay >> 16 );
            ulDelay &= 0x0000FFFF;
        } // 
        //ulDelay *= ( pygmyGlobalData.MainClock / 1000000 );
        if( ulDelay < 60 ){ 
            // Minimum number of cycles supported
            ulDelay = 60;
        } // 
        pygmyTimer->CR2 = 0;                          //
        pygmyTimer->SMCR = 0;                         //
        pygmyTimer->DIER = 0;                         // DMA and interrupt enable register
        pygmyTimer->CNT = 0;                          // Count Register
        pygmyTimer->ARR =  ulDelay - 60; // Auto Reload Register
        pygmyTimer->SR = 0;
        pygmyTimer->CR1 = ( TIM_ARPE | TIM_OPM | TIM_CEN );      // Enable single shot count
        while(( pygmyTimer->CR1 & TIM_CEN ) );         // Wait for count to complete
           
    } // else
    PYGMY_WATCHDOG_REFRESH;
}
#pragma pop_options

void *sysGetTimer( u8 ucTimer )
{
    // This function converts a numeric Timer representation to a
    // pointer to Timer structure
    switch( ucTimer ){
        case PYGMY_TIMER1:
            return( TIM1 );
        case PYGMY_TIMER2:
            return( TIM2 );
        case PYGMY_TIMER3:
            return( TIM3 );
        case PYGMY_TIMER4:
            return( TIM4 );
        case PYGMY_TIMER5:
            return( TIM5 );
        case PYGMY_TIMER6:
            return( TIM6 );
        case PYGMY_TIMER7:
            return( TIM7 );
        case PYGMY_TIMER8:
            return( TIM8 );
        case PYGMY_TIMER9:
            return( TIM9 );
        case PYGMY_TIMER10:
            return( TIM10 );
        case PYGMY_TIMER11:
            return( TIM11 );
        case PYGMY_TIMER12:
            return( TIM12 );
        case PYGMY_TIMER13:
            return( TIM13 );
        case PYGMY_TIMER14:
            return( TIM14 );
        case PYGMY_TIMER15:
            return( TIM15 );
        case PYGMY_TIMER16:
            return( TIM16 );
        case PYGMY_TIMER17:
            return( TIM17 );
        default:
            return( NULL );
    } // switch
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

void stopwatchStart( void )
{
    //pygmyGlobalData.StopWatch = ( pygmyGlobalData.MainClock * 2 ) - SYSTICK->VAL;
    pygmyGlobalData.StopWatch = 0;
}

u32 stopwatchGet( void )
{
    //pygmyGlobalData.StopWatch += ( pygmyGlobalData.MainClock * 2 ) - SYSTICK->VAL;
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
 //void f () __attribute__ ((weak, 
__attribute__ ((weak)) void SysTick_Handler( void ) 
{
    PYGMY_WATCHDOG_REFRESH; 
    
    pygmyGlobalData.StopWatch += 10;

    #ifdef __PYGMYTASKS 
        if( pygmyGlobalData.Status & PYGMY_TASK_INIT ){
            //print( COM3, "\rProc Tasks" );
            taskProcess();
        } // if
    #endif
    #ifdef __PYGMYMESSAGES
        if( pygmyGlobalData.Status & PYGMY_MSG_INIT ){
            msgProcess();
        } // if
    #endif
    #ifdef __PYGMYSPRITES
        guiSpriteProcess( );
    #endif
    //if( pygmyGlobalData.XModem ){
    //    xmodemProcessTimer( pygmyGlobalData.XModem );
    //} // if
}

//-----------------------------------Pygmy OS IRQ Handlers------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//-----------------------------------------Deprecated-----------------------------------------

//---------------------------------------End Deprecated---------------------------------------
//--------------------------------------------------------------------------------------------

