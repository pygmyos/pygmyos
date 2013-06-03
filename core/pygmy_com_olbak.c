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
#include <stdlib.h>
#include "pygmy_profile.h"

#ifdef __PYGMYSTREAMS
    PYGMYFIFO globalStreams[ MAXCOMPORTS ];
    u8 globalStreamEcho[ MAXCOMPORTS ];
    u8 globalSTDIO = STDIO;
#endif
#ifdef __PYGMYSTREAMCOM1
    u8 globalCOM1RXBuffer[ __PYGMYCOM1BUFFERLEN ];
#endif
#ifdef __PYGMYSTREAMCOM2
    u8 globalCOM2RXBuffer[ __PYGMYCOM2BUFFERLEN ];
#endif
#ifdef __PYGMYSTREAMCOM3
    u8 globalCOM3RXBuffer[ __PYGMYCOM3BUFFERLEN ];
#endif
#ifdef __PYGMYSTREAMCOM4
    u8 globalCOM4RXBuffer[ __PYGMYCOM4BUFFERLEN ];
#endif
#ifdef __PYGMYSTREAMCOM5
    u8 globalCOM5RXBuffer[ __PYGMYCOM5BUFFERLEN ];
#endif
#ifdef __PYGMYSTREAMUSER
    
#endif
void comConfig( u8 ucPort, u8 ucProtocol, u8 ucOptions, u32 uiRate )
{
    USART_TYPEDEF *ptrUSART;
    // ToDo: Add multi-protocol support
   
    sysEnableComClock( ucPort );
    #ifdef __PYGMYSTREAMCOM1
        if( ucPort == COM1 ){
            PYGMY_RCC_USART1_ENABLE;
            PYGMY_RCC_GPIOA_ENABLE;
            interruptEnable( USART1_IRQ );
            interruptSetPriority( USART1_IRQ, 1 );
            ptrUSART = USART1;
            if( ucOptions & RTS ){
                PA12_ALT;
            } // if
            if( ucOptions & CTS ){
                PA11_FLOAT;
            } // if
            PA9_ALT;
            PA10_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM1
    #ifdef __PYGMYSTREAMCOM2
        if( ucPort == COM2 ){
            PYGMY_RCC_USART2_ENABLE;
            PYGMY_RCC_GPIOA_ENABLE;
            interruptSetPriority( USART2_IRQ, 0 );
            interruptEnable( USART2_IRQ );
            ptrUSART = USART2;
            if( ucOptions & RTS ){
                PA1_ALT;
            } // if
            if( ucOptions & CTS ){
                PA0_FLOAT;
            } // if
            PA2_ALT;
            PA3_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM2
    #ifdef __PYGMYSTREAMCOM3
        if( ucPort == COM3 ){
            PYGMY_RCC_USART3_ENABLE;
            PYGMY_RCC_GPIOB_ENABLE;
            interruptSetPriority( USART3_IRQ, 1 );
            interruptEnable( USART3_IRQ );
            ptrUSART = USART3;
            if( ucOptions & RTS ){
                PB14_ALT;
            } // if
            if( ucOptions & CTS ){
                PB13_FLOAT;
            } // if
            PB10_ALT;
            PB11_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM3
    #ifdef __PYGMYSTREAMCOM4
        if( ucPort == COM4 ){
            PYGMY_RCC_USART4_ENABLE;
            PYGMY_RCC_GPIOC_ENABLE;
            interruptEnable( USART4_IRQ );
            interruptSetPriority( USART4_IRQ, 1 );
            ptrUSART = USART4;
            PC10_ALT;
            PC11_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM4
    #ifdef __PYGMYSTREAMCOM5
        if( ucPort == COM5 ){
            PYGMY_RCC_USART5_ENABLE;
            PYGMY_RCC_GPIOC_ENABLE;
            PYGMY_RCC_GPIOD_ENABLE;
            interruptEnable( USART5_IRQ );
            interruptSetPriority( USART5_IRQ, 1 );
            ptrUSART = USART5;
            PC12_ALT;
            PD2_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM5
    #ifdef __PYGMYSTREAMCOM6
        if( ucPort == COM6 ){
            PYGMY_RCC_USART6_ENABLE;
            PYGMY_RCC_GPIOG_ENABLE;
            interruptEnable( USART6_IRQ );
            interruptSetPriority( USART6_IRQ, 1 );
            ptrUSART = USART6;
            PG10_ALT;
            PG11_PULLUP;
        } // if
    #endif // __PYGMYSTREAMCOM4  
            
    ptrUSART->CR3 = 0;
    // Warning! CTS and RTS are not supported on UART4-UART5
    if( ucOptions & RTS ){
        ptrUSART->CR3 |= USART_RTSE;
    } // if
    if( ucOptions & CTS ){
        ptrUSART->CR3 |= USART_CTSE;
    } // if
    ptrUSART->BRR = comGenerateBaud( sysGetMainClock(), uiRate );
    ptrUSART->CR3 = 0;//USART_ONEBITE;
    if( ucOptions & TXIE ){
        ptrUSART->CR1 = ( USART_OVER8 | USART_UE | USART_TXEIE | USART_RXNEIE | USART_TE | USART_RE  );
    } else{
        ptrUSART->CR1 = ( USART_OVER8 | USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
    } // else
    //ptrUSART->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
    ptrUSART->SR = 0;
}

void comDisable( u8 ucPort )
{
    if( ucPort == 1 ){
        USART1->CR1 &= ~USART_UE;
        PYGMY_RCC_USART1_DISABLE;
    } else if( ucPort == 2 ){
        USART2->CR1 &= ~USART_UE;
        PYGMY_RCC_USART1_DISABLE;
    } else if( ucPort == 3 ){
        USART3->CR1 &= ~USART_UE;
        PYGMY_RCC_USART1_DISABLE;
    } // else if
}

void comEnable( u8 ucPort )
{
    if( ucPort == 1 ){
        PYGMY_RCC_USART1_ENABLE;
        USART1->CR1 |= USART_UE;
    } else if( ucPort == 2 ){
        PYGMY_RCC_USART1_ENABLE;
        USART2->CR1 |= USART_UE;
    } else if( ucPort == 3 ){
        PYGMY_RCC_USART1_ENABLE;
        USART3->CR1 |= USART_UE;
    } // else if
}

u16 comGenerateBaud( u32 ulClock, u32 ulRate )
{
    u16 uiBRR;

    // APB bus is limited to 36MHz, if clock is above that then div by 4
    ulClock = sysGetMainClock();
    if( ulClock == 72000000 || ulClock == 48000000  ){
        ulClock /= 4;
    } // if
    uiBRR = ( ( (ulClock >> 3 ) / ulRate ) << 4 ) + ( ( ( ulClock / ulRate ) ) & 0x0007 );
    
    return( (u16)uiBRR ); 
}

void parallelConfig( PYGMYPARALLELPORT *pygmyPort, u8 ucWidth, u8 ucCS, u8 ucA0, u8 ucWR, u8 ucRD, u8 ucD0 )
{
    u8 i;

    pinConfig( ucCS, OUT );
    pinConfig( ucA0, OUT );
    pinConfig( ucWR, OUT );
    pinConfig( ucRD, OUT );

    // Width sets the number of bits in the data bus, 8 and 16 are the expected values
    if( ucWidth < 17 ){
        pygmyPort->Width = ucWidth;
    } else{
        pygmyPort->Width = 8;
    } // else
    for( i = 0, pygmyPort->Mask = 0; i < pygmyPort->Width; i++, ucD0++ ){
        pygmyPort->Mask |= PYGMY_BITMASKS[ ucD0 % 16 ]; // pre-build bitmask for later port operations
        pinConfig( ucD0, OUT );
    } // for
    
    pygmyPort->PortCS   = pinGetPort( ucCS );
    pygmyPort->PortA0   = pinGetPort( ucA0 );
    pygmyPort->PortWR   = pinGetPort( ucWR );
    pygmyPort->PortRD   = pinGetPort( ucRD );
    pygmyPort->PortDATA = pinGetPort( ucD0 );
    
    pygmyPort->PinCS = PYGMY_BITMASKS[ ucCS % 16 ];
    pygmyPort->PinA0 = PYGMY_BITMASKS[ ucA0 % 16 ];
    pygmyPort->PinWR = PYGMY_BITMASKS[ ucWR % 16 ];
    pygmyPort->PinRD = PYGMY_BITMASKS[ ucRD % 16 ];
    pygmyPort->PinD0 = PYGMY_BITMASKS[ ucD0 % 16 ];
    
    pinSet( ucCS, HIGH );
    pinSet( ucA0, HIGH );
    pinSet( ucWR, HIGH );
    pinSet( ucRD, HIGH );
}

void parallelWrite( PYGMYPARALLELPORT *pygmyPort, u8 ucA0, u16 uiWord )
{
    uiWord = ( uiWord << pygmyPort->PinD0 ) & pygmyPort->Mask;
    if( ucA0 ){
        pygmyPort->PortA0->BSRR = pygmyPort->PinA0;
    } else{
        pygmyPort->PortA0->BRR = pygmyPort->PinA0;
    } // else
    
    pygmyPort->PortCS->BRR      = pygmyPort->PinCS;
    pygmyPort->PortDATA->ODR    &= ~pygmyPort->Mask;
    pygmyPort->PortDATA->ODR    |= uiWord;
    pygmyPort->PortWR->BRR      = pygmyPort->PinWR; // Go high to set to inactive
    pygmyPort->PortWR->BSRR     = pygmyPort->PinWR; // Go low to clock out data/cmd
    pygmyPort->PortCS->BSRR     = pygmyPort->PinCS;
}

/*
void parallelWriteWord( PYGMYPARALLELPOR *pygmyPort, u16 uiWord )

void parallelWriteCommand( PYGMYPARALLELPOR *pygmyPort, u8 ucByte )


void parallelWriteBuffer( PYGMYPARALLELPORT *pygmyPort, u8 *ucBuffer, u16 uiLen )
{

}*/

u16 parallelRead( PYGMYPARALLELPORT *pygmyPort, u8 ucA0 )
{
    u16 uiWord;

    uiWord = pygmyPort->PortDATA->IDR & pygmyPort->Mask;
    uiWord >>= pygmyPort->PinD0;
    
    return( uiWord );
}

void spiPutCommand( PYGMYSPIPORT *pygmySPI, u8 ucByte )
{
    // write one byte with CS cycle to SPI device, generally used for command transfer
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteByte( pygmySPI, ucByte );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiWriteAddress( PYGMYSPIPORT *pygmySPI, u64 Address )
{
    u16 i, AddressBytes;

    AddressBytes = pygmySPI->CR >> 4;

    for( i = 0; i < AddressBytes; i++ ){
        spiWriteByte( pygmySPI, (u8)((u64) Address >> ( 64 - ( i * 8 ) ) ) );
    } // for
    
    /*if( pygmySPI->CR & SPILONGADDRESS ){
        spiWriteByte( pygmySPI, (u8)((u32) ulAddress >> 24 ) );
        spiWriteByte( pygmySPI, (u8)((u32) ulAddress >> 16 ) );
        spiWriteByte( pygmySPI, (u8)((u32) ulAddress >> 8 ) );
    } else if( pygmySPI->CR & SPIWORDADDRESS ){
        spiWriteByte( pygmySPI, (u8)((u32) ulAddress >> 8 ) );
    } // else if
    spiWriteByte( pygmySPI, (u8) ulAddress );
    */
}

void spiPutChar( PYGMYSPIPORT *pygmySPI, u32 ulAddress, u8 ucByte )
{
    // Writes one char with prepended address and CS cycle
    //if( ( pygmySPI->CR & 0x03) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;
    //} else{
    //    pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
    //} // else
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONWRITE ){
        spiReadByte( pygmySPI );
    } // if
    spiWriteByte( pygmySPI, ucByte );
    //if( ( pygmySPI->CR & 0x03) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;
    //} else{
    //    pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
    //} // else
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiPutWord( PYGMYSPIPORT *pygmySPI, u32 ulAddress, u16 uiData )
{
    // writes a bigendian 16bit word with prepended address and CS cycle
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONWRITE ){
        spiReadByte( pygmySPI );
    } // if
    spiWriteByte( pygmySPI, (u8)((u16) uiData >> 8 ) );
    spiWriteByte( pygmySPI, (u8) uiData );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiPutLong( PYGMYSPIPORT *pygmySPI, u32 ulAddress, u32 ulData )
{
    // writes a bigendian 32bit  long with prepended address and CS cycle
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONWRITE ){
        spiReadByte( pygmySPI );
    } // if
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 24 ) );
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 16 ) );
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 8 ) );
    spiWriteByte( pygmySPI, (u8) ulData );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiPutBuffer( PYGMYSPIPORT *pygmySPI, u32 ulAddress, u8 *ucBuffer, u32 ulLen )
{
    u32 i;
    
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONWRITE ){
        spiReadByte( pygmySPI );
    } // if
    for( i = 0; i < ulLen; i++ ){
        spiWriteByte( pygmySPI, ucBuffer[ i ] );
    } // for
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

u8 spiGetChar( PYGMYSPIPORT *pygmySPI, u32 ulAddress )
{
    u8 DataByte;
    
    //if( ( pygmySPI->CR & 0x03) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
    //    pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;
   // } else{
    //    pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
    //} // else
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONREAD ){
        spiReadByte( pygmySPI );
    } // if
    DataByte = spiReadByte( pygmySPI );
    //pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    //if( ( pygmySPI->CR & 0x03) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
    //    pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;
    //} else{
    //    pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
    //} // else
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS; 

    return( DataByte );
}

u16 spiGetWord( PYGMYSPIPORT *pygmySPI, u32 ulAddress )
{
    u16 uiData;

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONREAD ){
        spiReadByte( pygmySPI );
    } // if
    uiData = (u16)spiReadByte( pygmySPI ) << 8;
    uiData |= (u16)spiReadByte( pygmySPI );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
    
    return( uiData );
}

u32 spiGetLong( PYGMYSPIPORT *pygmySPI, u32 ulAddress )
{
    u32 ulData;

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONREAD ){
        spiReadByte( pygmySPI );
    } // if
    ulData = (u32)spiReadByte( pygmySPI ) << 24;
    ulData |= (u32)spiReadByte( pygmySPI ) << 16;
    ulData |= (u32)spiReadByte( pygmySPI ) << 8;
    ulData |= (u32)spiReadByte( pygmySPI );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;

    return( ulData );
}

void spiGetBuffer( PYGMYSPIPORT *pygmySPI, u32 ulAddress, u8 *ucBuffer, u32 ulLen )
{
    u32 i;

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulAddress );
    if( pygmySPI->CR & SPIDUMMYONREAD ){
        spiReadByte( pygmySPI );
    } // if
    for( i = 0; i < ulLen; i++ ){
        ucBuffer[ i ] = spiReadByte( pygmySPI );
    } // for
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}


void spiWriteLong( PYGMYSPIPORT *pygmySPI, u32 ulData )
{
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 24 ) );
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 16 ) );
    spiWriteByte( pygmySPI, (u8)((u32) ulData >> 8 ) );
    spiWriteByte( pygmySPI, (u8) ulData );
}


void spiWriteByte( PYGMYSPIPORT *pygmySPI, u8 ucByte )
{
    // Clocks out 8 bits
    u16 i;
    
    /*if( ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        // Mode 1
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	
        for( i = 0; i < 8; i++ ){ 
            if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
                pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
            } else{                                             //
                pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
            } // else                                           //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;         // clock must start low, transition high 
        } // for
    } else if( ( pygmySPI->CR & 0x03 ) == SPIMODE2 ){
        // Mode 2
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;
        for( i = 0; i < 8; i++ ){ 
            if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
                pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
            } else{                                             //
                pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
            } // else                                           //
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;         // clock must start low, transition high 
        } // for
    } else if( ( pygmySPI->CR & 0x03 ) == SPIMODE3 ){
        // Mode 3
        pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
        for( i = 0; i < 8; i++ ){ 
            if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
                pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
            } else{                                             //
                pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
            } // else                                           //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;         // clock must start low, transition high 
        } // for
    } else{
        // Mode 0
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	
        for( i = 0; i < 8; i++ ){ 
            if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
                pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
            } else{                                             //
                pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
            } // else                                           //
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;         // clock must start low, transition high 
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
        } // for
    } // else
*/
    //if( ( pygmySPI->CR & 0x03 ) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        // MODE0 and MODE1 use Clock Polarity Low
        // Clock starts low, low-high-low clocks data in or out
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        
    //} else{
        // MODE2 and MODE3 use Clock Polarity High
        // Clock starts high, high-low-high clocks data in or out
    //    pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;
    //} // else 
    //if( ( pymgySPI->CR & 0x03 ) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        for( i = 0; i < 8; i++ ){ 
            if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
                pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
            } else{                                             //
                pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
            } // else                                           //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;         // clock must start low, transition high 
        } // for
    ///} else{
    //    for( i = 0; i < 8; i++ ){ 
    //        if( ucByte & ( BIT7 >> i ) ){ //if( ucByte & PYGMY_INVBITMASKS[ i ] ){
    //            pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
    //        } else{                                             //
    //            pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;    // MasterOutSlaveIn low if bit clear
    //        } // else                                           //
    //        pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;         // clock must start low, transition high  
    //        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
    //    } // for
    //} // else
    //if( ( pygmySPI->CR & 0x03 ) == SPIMODE0 || ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        // MODE0 and MODE1 use Clock Polarity Low
        // Clock starts low, low-high-low clocks data in or out
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        
    //} else{
        // MODE2 and MODE3 use Clock Polarity High
        // Clock starts high, high-low-high clocks data in or out
    //    pygmySPI->PortSCK->SBRR = pygmySPI->PinSCK;
    //} // else
    
} 
/*
void spiWriteWord( PYGMYSPIPORT *pygmySPI, u16 uiWord )
{
    // Clocks out 16 bits
	u16 i;
	
	pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;		    // Clock starts low, low-high-low clocks data in or out
	for( i = 16; i < 32; i++ ){ 			                    // 
		//if( uiWord & ( 0x8000 >> i ) ){	                    // 
        if( uiWord & PYGMY_INVBITMASKS[ i ] ){
            pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
		} else{ 		                                    //
            pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;  // MasterOutSlaveIn low if bit clear
        } // else                                           // 
		pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;		    // clock must start low, transition high
		pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;		// Low transition finishes clock sequence
    } // for
} 

void spiWriteLong( PYGMYSPIPORT *pygmySPI, u32 ulLong )
{
    // Clocks out 32 bits
	u16 i;
	
	pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;			// Clock starts low, low-high-low clocks data in or out
	for( i = 0; i < 32; i++ ){ 			                    // 
		//if( ulLong & ( 0x80000000 >> i ) ){	                // 
        if( ulLong & PYGMY_INVBITMASKS[ i ] ) {
            pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
		} else{ 		                                    //        
            pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;  // MasterOutSlaveIn low if bit clear
        } // else                                           //     
		pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;	        // clock must start low, transition high
		pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;        // Low transition finishes clock sequence
    } // for
} */

u8 spiReadByte( PYGMYSPIPORT *pygmySPI )
{
    // Clocks in 8 bits
    u16 i;
    u8 DataByte;

    pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;             // Clock starts low, low-high-low clocks data in or out
	//for( i = 24, ucByte = 0; i < 32; i++ ){                   // 
	//	pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
        //if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
        //    ucByte |= PYGMY_INVBITMASKS[ i ];
	//		//ucByte |= ( BIT7 >> i );                        //
        //} // if                                             //
	//	pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
        // } // for
	for( i = 0, ucByte = 0; i < 8; i++ ){                   // 
		pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
        if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
            //ucByte |= PYGMY_INVBITMASKS[ i ];
			ucByte |= ( BIT7 >> i );                        //
        } // if                                             //
		pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
    } // for
    
    
    /*if( ( pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        // Mode 1
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	
        for( i = 0, DataByte = 0; i < 8; i++ ){                   // 
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
            if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
                DataByte |= ( BIT7 >> i );                        //
            } // if                                             //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
        } // for
    } else if( ( pygmySPI->CR & 0x03 ) == SPIMODE2 ){
        // Mode 2
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	
        for( i = 0, DataByte = 0; i < 8; i++ ){                   // 
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
            if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
                DataByte |= ( BIT7 >> i );                        //
            } // if                                             //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
        } // for
    } else if( ( pygmySPI->CR & 0x03 ) == SPIMODE3 ){
        // Mode 3
        pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	
        for( i = 0, DataByte = 0; i < 8; i++ ){                   // 
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
            if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
                DataByte |= ( BIT7 >> i );                        //
            } // if                                             //
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
        } // for
    } else{
        // Mode 0
        pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;	
        for( i = 0, DataByte = 0; i < 8; i++ ){                   // 
            pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
            if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
                DataByte |= ( BIT7 >> i );                        //
            } // if                                             //
            pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
        } // for
    } // else

    return( DataByte );*/
}

void spiReadBuffer( PYGMYSPIPORT *pygmySPI, u8 *ucBuffer, u16 uiLen )
{
    u16 i; 

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = spiReadByte( pygmySPI );
    } // for
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiWriteBuffer( PYGMYSPIPORT *pygmySPI, u8 *ucBuffer, u16 uiLen )
{
    u16 i; 

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    for( i = 0; i < uiLen; i++ ){
        spiWriteByte( pygmySPI, ucBuffer[ i ] );
    } // for
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

void spiConfig( PYGMYSPIPORT *pygmySPI, u8 ucCS, u8 ucSCK, u8 ucMISO, u8 ucMOSI, u8 ucCR )
{
    if( ( pygmySPI->CR & 0x03 ) == SPIMODE0 || (pygmySPI->CR & 0x03 ) == SPIMODE1 ){
        pinSet( ucSCK, LOW );
    } else{
        pinSet( ucSCK, HIGH );
    } // else
    pinSet( ucCS, HIGH );
    pinConfig( ucCS, OUT );
    pinConfig( ucSCK, OUT );
    if( ucMOSI != NONE ){
        pinConfig( ucMOSI, OUT );
        pygmySPI->PortMOSI  = pinGetPort( ucMOSI );
        pygmySPI->PinMOSI   = sysGenerateBit( ucMOSI % 16 );//PYGMY_BITMASKS[ ucMOSI % 16 ];
    } // if
    if( ucMISO != NONE ){
        pinConfig( ucMISO, IN );
        pygmySPI->PortMISO  = pinGetPort( ucMISO );
        pygmySPI->PinMISO   = sysGenerateBit( ucMISO % 16 );//PYGMY_BITMASKS[ ucMISO % 16 ];
    } // if

    pygmySPI->PortCS    = pinGetPort( ucCS );
    pygmySPI->PortSCK   = pinGetPort( ucSCK );   
    
    pygmySPI->PinCS     = sysGenerateBit( ucCS % 16 );//PYGMY_BITMASKS[ ucCS % 16 ];
    pygmySPI->PinSCK    = sysGenerateBit( ucSCK % 16 );//PYGMY_BITMASKS[ ucSCK % 16 ];   
    
    pygmySPI->CR        = ucCR;//0xFF;
    //pinSet( ucCS, HIGH );
}

//--------------------------------------I2C Hardware Interface-------------------------------------
//-------------------------------------------------------------------------------------------------
void i2cInit( I2C_TYPEDEF *i2c )
{
  /*
    #ifndef __PYGMYI2COWNADDRESS
        #define __PYGMYI2COWNADDRESS 0x10
    #endif // __PYGMYI2COWNADDRESS
    i2cSetAddress( I2C1, 0, __PYGMYI2COWNADDRESS );
    i2cSetAddress( I2C1, 1, __PYGMYI2COWNADDRESS + 3 );
    i2cSetAddress( I2C2, 0, __PYGMYI2COWNADDRESS + 1 );
    i2cSetAddress( I2C2, 1, __PYGMYI2COWNADDRESS + 4 );
    i2cSetAddress( I2C3, 0, __PYGMYI2COWNADDRESS + 2 );
    i2cSetAddress( I2C3, 1, __PYGMYI2COWNADDRESS + 5 );
    i2c->CR2 = ( I2C_ITEVTEN|I2C_ITERREN|36 ); // 36 Periph Clock
    i2c->CR1 = ( I2C_ACK|I2C_STOP|I2C_PE );
    */
}

void i2cSetAddress( I2C_TYPEDEF *i2c, u8 ucDual, u16 uiAddress )
{   
    /*u8 i;

    if( i2c == I2C1 ){
        i = 0;
    } else if( i2c == I2C2 ){
        i = 1;
    } else{
        i = 2;
    } // else
    uiAddress <<= 1 ;
    if( ucDual && !( uiAddress & ( BIT9|BIT8 ) ) ){
        globalI2COwnAddress[ i ][ ucDual ] = 0x03FE & uiAddress;
    } else {
        globalI2C
    }
    if(  ){
        i2c->OAR1 = BIT15 | ( globalI2COwnAddress[ i ][ 0 ]  );
        globalI2COwnAddress[ i ][ 1 ] = 0;
    } else if( ucDual ){
        globalI2COwnAddress[ i ][ 1 ] = 0x00FE & uiAddress;
    } // else
    i2c->OAR2 = globalI2COwnAddress[ i ][ 1 ] | ( ucDual & BIT0 );
    */
}

void i2cReset( I2C_TYPEDEF *i2c )
{
    i2c->CR1 |= I2C_SWRST;
}

#ifdef __PYGMYSTREAMSBUS1
void I2C1_EV_IRQHandler( void ) 
{ 
    if( I2C1->SR & I2C_RXNE){
        streamPushChar( BUS1, I2C1->DR ); 
        if( globalStreams[ BUS1 ].Get ){
            globalStreams[ BUS1 ].Get();
        } // if
    } // if
    if( I2C1->SR & USART_TXE ){
        streamTXChar( BUS1, I2C1 );
		
    } // if
    //I2C1->SR = 0;
}
#endif // __PYGMYSTREAMSBUS1

void I2C1_ER_IRQHandler( void ) 
{

}

void I2C2_EV_IRQHandler( void ) 
{ 

}

void I2C2_ER_IRQHandler( void ) 
{

}

void I2C3_EV_IRQHandler( void )
{

}

void I2C3_ER_IRQHandler( void )
{

}

//------------------------------------End I2C Hardware Interface-----------------------------------
//-------------------------------------------------------------------------------------------------

//--------------------------------------I2C Software Interface-------------------------------------
//-------------------------------------------------------------------------------------------------

void i2cConfig( PYGMYI2CPORT *pygmyI2C, u8 ucSCL, u8 ucSDA, u8 ucAddress, u16 uiCR )
{
    u32 ulSpeed;

    pinConfig( ucSCL, PULLUP );
    pinConfig( ucSDA, PULLUP );
    
    pygmyI2C->SCL       = ucSCL;
    pygmyI2C->SDA       = ucSDA;
    pygmyI2C->PortSCL   = pinGetPort( ucSCL );
    pygmyI2C->PortSDA   = pinGetPort( ucSDA );
                        
    pygmyI2C->PinSCL    = BIT0 << ( ucSCL % 16 ); //PYGMY_BITMASKS[ ucSCL % 16 ];
    pygmyI2C->PinSDA    = BIT0 << ( ucSDA % 16 ); //PYGMY_BITMASKS[ ucSDA % 16 ];
    
    pygmyI2C->Address   = ucAddress << 1;
    if( uiCR ){
        pygmyI2C->CR    = uiCR;
    } else{
        pygmyI2C->CR    = I2CSPEEDFAST;
    } // else
    
    // This function assumes a 50 MHz maximum GPIO speed!
    ulSpeed = sysGetMainClock();
    if( ulSpeed > 50000000 ){
        ulSpeed  = 50000000;
    } // if
    if( pygmyI2C->CR & I2CSPEEDSTANDARD ){
        ulSpeed /= ( 100000 * 4 );
    } else if( pygmyI2C->CR & I2CSPEEDFAST ){
        ulSpeed /= ( 400000 * 4 );
    } else if( pygmyI2C->CR & I2CSPEEDFASTPLUS ){
        ulSpeed /= ( 1000000 * 4 );
    } else if( pygmyI2C->CR & I2CSPEEDHIGH ){
        ulSpeed /= ( 3400000 * 4 );
    } else{ 
        // Low Speed Default
        ulSpeed /= ( 10000 * 4 );
    } // else
    pygmyI2C->Speed     = ulSpeed;
}

void i2cDelay( PYGMYI2CPORT *pygmyI2C ) 
{ 
    u16 i;

    for( i = 0; i < pygmyI2C->Speed; i++ ){
        //asm( "nop" );
    } // if
}
 
void i2cStretch( PYGMYI2CPORT *pygmyI2C )
{
    u16 i;

    for( i = 0; i < 60000; i++ ){
        if( pygmyI2C->PortSCL->IDR & pygmyI2C->PinSCL ){
            break;
        } // if
    } // for
}
 
void i2cStart( PYGMYI2CPORT *pygmyI2C )
{
    // Allow SDA to float
    pinConfig( pygmyI2C->SDA, PULLUP );
    i2cDelay( pygmyI2C );
    // allow clock stretching
    pinConfig( pygmyI2C->SCL, PULLUP );
    i2cStretch( pygmyI2C );   
   
    if ( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
        // ToDo: Handle loss of arbitration
        // print( COM3, "\rBus error on START" ); // Debug Output
        i2cResetBus( pygmyI2C );
    } //
    pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output 
    pinConfig( pygmyI2C->SDA, OUT ); // set to output to drive low
    
    i2cDelay( pygmyI2C );
    
    pygmyI2C->PortSCL->ODR &= ~pygmyI2C->PinSCL; // set state before setting to output
    pinConfig( pygmyI2C->SCL, OUT );
}
 
void i2cStop( PYGMYI2CPORT *pygmyI2C )
{
    pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output 
    pinConfig( pygmyI2C->SDA, OUT ); // set to output to drive low
    i2cDelay( pygmyI2C );
    // Allow for SCL stretching   
    pinConfig( pygmyI2C->SCL, PULLUP );
    i2cStretch( pygmyI2C );
    // Allow SDA to float and check state ( arbitration test )
    pinConfig( pygmyI2C->SDA, PULLUP );
    i2cDelay( pygmyI2C );
    //pinSet( MCO, LOW ); // used to test timing
    if ( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
        // print( COM3, "\rBus error on STOP" ); // Debug Output
        i2cResetBus( pygmyI2C );
        // ToDo: Handle loss of arbitration
    } // 
    //pinSet( MCO, HIGH ); // used to test timing
}
 
void i2cWriteBit( PYGMYI2CPORT *pygmyI2C, u8 ucBit )
{
    // write one bit
    if ( ucBit ){
        pinConfig( pygmyI2C->SDA, PULLUP );
    } else{ 
        pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output
        pinConfig( pygmyI2C->SDA, OUT );
    } // else
    i2cDelay( pygmyI2C );
    // allow clock stretching 
    pinConfig( pygmyI2C->SCL, PULLUP );
    
    i2cStretch( pygmyI2C );
    // If SDA should be 1, check that it isn't being push/pulled by another device
    
    if ( ucBit ){  
        pinConfig( pygmyI2C->SDA, PULLUP );  
        if( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
            // print( COM3, "\rBus error on WRITE" ); // Debug Output
            //i2cResetBus( pygmyI2C );
            // ToDo: Handle loss of arbitration
        } // if
    } // if   
    i2cDelay( pygmyI2C );
    pygmyI2C->PortSCL->ODR &= ~pygmyI2C->PinSCL; // set state before setting to output
    pinConfig( pygmyI2C->SCL, OUT );
}
 

u8 i2cReadBit( PYGMYI2CPORT *pygmyI2C )
{
    // read one bit
    u8 ucBit;

    pinConfig( pygmyI2C->SDA, PULLUP );
    // allow clock stretching 
    i2cDelay( pygmyI2C );
    pinConfig( pygmyI2C->SCL, PULLUP );
    
    i2cStretch( pygmyI2C );
    
    // Clock in whenSCL high
    
    if( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ){
        ucBit = 1;
    } else{
        ucBit = 0;
    } // else
    
    i2cDelay( pygmyI2C );
    pygmyI2C->PortSCL->ODR &= ~pygmyI2C->PinSCL; // set state before setting to output
    pinConfig( pygmyI2C->SCL, OUT );
        
    return( ucBit );
}
/*
u8 i2cWriteBuffer( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;
    
    i2cStart( pygmyI2C );
    i2cWriteByte( pygmyI2C, pygmyI2C->Address );
    if( pygmyI2C->CR & I2CWORDADDRESS ){
        i2cWriteByte( pygmyI2C, (u8)( (u16)uiAddress >> 8 )); // Write MSB if 16bit addressing
    } // if
    i2cWriteByte( pygmyI2C, (u8)uiAddress );
    for( i = 0; i < uiLen; i++ ){
        i2cWriteByte( pygmyI2C, ucBuffer[ i ] );
    } // for
    i2cStop( pygmyI2C );
}

u8 i2cReadBuffer( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;
    
    i2cStart( pygmyI2C );
    i2cWriteByte( pygmyI2C, pygmyI2C->Address );
    if( pygmyI2C->CR & I2CWORDADDRESS ){
        i2cWriteByte( pygmyI2C, (u8)( (u16)uiAddress >> 8 )); // Write MSB if 16bit addressing
    } // if
    i2cWriteByte( pygmyI2C, (u8)uiAddress );
    i2cStart( pygmyI2C );
    i2cWriteByte( pygmyI2C, pygmyI2C->Address | 1 );
    for( i = 0; i < uiLen; i++ ){
        *(ucBuffer++) = i2cReadByte( pygmyI2C );
        if( i + 1 < uiLen ){
            i2cWriteBit( pygmyI2C, 0 ); // Ack for next byte
        } // if
    } // for
    i2cWriteBit( pygmyI2C, 1 ); // Nack when all bytes read
    i2cStop( pygmyI2C );
}*/

u8 i2cWriteByte( PYGMYI2CPORT *pygmyI2C, u8 ucByte )
{
    u8 i;
    u8 ucAck;

    for ( i = 0; i < 8; i++ ){
        if( ucByte & BIT7 ){
            i2cWriteBit( pygmyI2C, 1 );
        } else{
            i2cWriteBit( pygmyI2C, 0 );
        } // else
        ucByte <<= 1;
    } // for
    ucAck = i2cReadBit( pygmyI2C );
    if( ucAck ){
        // ToDo: Add code to handle Ack error
    } // if

    return( ucAck );
}
 
u8 i2cReadByte( PYGMYI2CPORT *pygmyI2C )
{
    u8 ucByte;
    u8 i;
        
    for( i = 0, ucByte = 0; i < 8; i++ ){
        ucByte = ( ucByte << 1 ) | i2cReadBit( pygmyI2C );
    } // for
    //i2cWriteBit( pygmyI2C, 0 ); // Low ACK
    //pinConfig( pygmyI2C->SDA, PULLUP );

    return( ucByte );
}

void i2cResetBus( PYGMYI2CPORT *pygmyI2C )
{
    u8 i;

    //1) Master tries to assert a Logic 1 on the SDA line
    //2) Master still sees a Logic 0 and then generates a clock pulse on SCL (1-0-1 transition)
    //3) Master examines SDA. If SDA = 0, go to Step 2; if SDA = 1, go to Step 4
    //4) Generate a STOP condition
    pinConfig( pygmyI2C->SCL, OUT );
    for ( i = 0; i < 9; i++ ){
        pygmyI2C->PortSDA->ODR |= pygmyI2C->PinSDA; // set state before setting to output 
        pinConfig( pygmyI2C->SDA, OUT ); // set to output to drive low
        if( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
            i2cStop( pygmyI2C );
            return;
        } // if
        i2cDelay( pygmyI2C );
        pygmyI2C->PortSCL->ODR |= pygmyI2C->PinSCL; // set state before setting to output
        i2cDelay( pygmyI2C );
        pygmyI2C->PortSCL->ODR &= ~pygmyI2C->PinSCL; // set state before setting to output
        i2cDelay( pygmyI2C );
        pygmyI2C->PortSCL->ODR |= pygmyI2C->PinSCL; // set state before setting to output
    } // for
}

u8 i2cPutString( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 *ucBuffer )
{
    for( ; *ucBuffer; uiAddress++, ucBuffer++  ){
        if( !i2cPutChar( pygmyI2C, uiAddress, *ucBuffer ) ){
            return( FALSE );
        } // if
    } // for

    return( TRUE );
}

u8 i2cPutBuffer( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;

    for( i = 0; i < uiLen; i++ ){
        if( !i2cPutChar( pygmyI2C, uiAddress + i, *ucBuffer ) ){
            return( FALSE );   
        } // else
    } // for
    
    return( TRUE );
}

u8 i2cPutChar( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 ucChar )
{
    u8 ucAck;

    ucAck = 0;
    PYGMY_WATCHDOG_REFRESH;
    i2cStart( pygmyI2C );
    ucAck |= i2cWriteByte( pygmyI2C, pygmyI2C->Address );
    if( pygmyI2C->CR & I2CWORDADDRESS ){
        ucAck |= i2cWriteByte( pygmyI2C, (u8)( (u16)uiAddress >> 8 )); // Write MSB if 16bit addressing
    } // if
    ucAck |= i2cWriteByte( pygmyI2C, (u8)uiAddress );
    ucAck |= i2cWriteByte( pygmyI2C, ucChar );
    i2cStop( pygmyI2C );
    
    if( ucAck ){
        return( FALSE );
    } // if
    if( pygmyI2C->CR & I2CPOLLFORACK ){
        i2cPollAck( pygmyI2C );
    } // if
    
    return( TRUE );
}

u8 i2cGetChar( PYGMYI2CPORT *pygmyI2C, u16 uiAddress )
{
    u8 ucChar;

    PYGMY_WATCHDOG_REFRESH;
    i2cStart( pygmyI2C);
    i2cWriteByte( pygmyI2C, pygmyI2C->Address );
    if( pygmyI2C->CR & I2CWORDADDRESS ){
        i2cWriteByte( pygmyI2C, (u8)((u16)uiAddress >> 8) );
    } // if
    i2cWriteByte( pygmyI2C, (u8)uiAddress );
    i2cStart( pygmyI2C );
    i2cWriteByte( pygmyI2C, pygmyI2C->Address | 1 ); // Read
    ucChar = i2cReadByte( pygmyI2C ); // Clock in Byte
    
    i2cWriteBit( pygmyI2C, 1 ); // High NACK to end sequence
    i2cStop( pygmyI2C );

    return( ucChar );
}

void i2cGetBuffer( PYGMYI2CPORT *pygmyI2C, u16 uiAddress, u8 *ucBuffer, u16 uiLen )
{
    u8 i;

    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = i2cGetChar( pygmyI2C, uiAddress + i );
    } // for
}

void i2cPollAck( PYGMYI2CPORT *pygmyI2C )
{
    // Waits for device to ack address
    // Used by EEPROM and other devices to show busy status
    u16 i;
    u8 ucAck;

    for( i = 0; i < 0xFFFF; i++  ){
        i2cDelay( pygmyI2C );
        i2cStart( pygmyI2C );
        ucAck = i2cWriteByte( pygmyI2C, pygmyI2C->Address );
        i2cStop( pygmyI2C );
        if( !ucAck ){
            break;
        } // if
    } // for
}

//--------------------------------------I2C Software Interface-------------------------------------
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//-------------------------------------Pygmy OS IO Stream-------------------------------------
#ifdef __PYGMYSTREAMS
void streamInit( void )
{
    u8 i;
    
    for( i = 0; i < MAXCOMPORTS; i++ ){
        streamReset( i );
    } // for
    
}

u8 streamReset( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].RXBufferLen   = 0;
        globalStreams[ ucStream ].RXIndex       = 0;
        globalStreams[ ucStream ].RXLen         = 0; 
        globalStreams[ ucStream ].TXBufferLen   = 0;
        globalStreams[ ucStream ].TXIndex       = 0;
        globalStreams[ ucStream ].TXLen         = 0;
        globalStreams[ ucStream ].Put           = (void *)TaskException_Handler;
        globalStreams[ ucStream ].Get           = (void *)TaskException_Handler;
        globalStreams[ ucStream ].RXBuffer      = NULL;
        globalStreams[ ucStream ].TXBuffer      = NULL;
        globalStreams[ ucStream ].CR            = 0;
        globalStreams[ ucStream ].ActionChars   = "\r";
        return( 1 );
    } // if

    return( 0 );
}

u8 streamGetSTDIO( void )
{
    return( globalSTDIO );
}

void streamSetSTDIO( u8 ucStream )
{
    globalSTDIO = ucStream;
    
    globalStreams[ STDIO ].RXBufferLen  = globalStreams[ ucStream ].RXBufferLen;
    globalStreams[ STDIO ].RXIndex      = globalStreams[ ucStream ].RXIndex;
    globalStreams[ STDIO ].RXLen        = globalStreams[ ucStream ].RXLen; 
    globalStreams[ STDIO ].TXBufferLen  = globalStreams[ ucStream ].TXBufferLen;
    globalStreams[ STDIO ].TXIndex      = globalStreams[ ucStream ].TXIndex;
    globalStreams[ STDIO ].TXLen        = globalStreams[ ucStream ].TXLen;
    globalStreams[ STDIO ].Put          = globalStreams[ ucStream ].Put;
    globalStreams[ STDIO ].Get          = globalStreams[ ucStream ].Get;
    globalStreams[ STDIO ].RXBuffer     = globalStreams[ ucStream ].RXBuffer;
    globalStreams[ STDIO ].TXBuffer     = globalStreams[ ucStream ].TXBuffer;
    globalStreams[ STDIO ].CR           = globalStreams[ ucStream ].CR;
    globalStreams[ STDIO ].ActionChars  = globalStreams[ ucStream ].ActionChars;
}   

void streamResetRX( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].RXIndex = 0;
        globalStreams[ ucStream ].RXLen = 0;
    } // if
}

void streamResetTX( u8 ucStream )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].TXIndex = 0;
        globalStreams[ ucStream ].TXLen = 0;
    } // if
}

void streamTXChar( u8 ucStream, void *pygmyPeriph )
{
    //USART_TYPEDEF *ptrPeriph = (USART_TYPEDEF *)pygmyUSART;
    
    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].TXLen ){
		--globalStreams[ ucStream ].TXLen;  
		((USART_TYPEDEF *)pygmyPeriph)->DR = globalStreams[ ucStream ].TXBuffer[ globalStreams[ ucStream ].TXIndex ]; 
        globalStreams[ ucStream ].TXIndex = ( globalStreams[ ucStream ].TXIndex + 1 ) % 
            globalStreams[ ucStream ].TXBufferLen;
    } else{
        ((USART_TYPEDEF *)pygmyPeriph)->CR1 &= ~USART_TXEIE;
    } // else
}

void streamEchoChar( u8 ucStream, u8 ucChar )
{
    u8 ucString[ 2 ] = { ucChar, '\0' };

    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].Put( ucString );
    } // if
}

u8 streamGetChar( u8 ucStream )
{
    u16 i;

    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].RXLen ){
        i = globalStreams[ ucStream ].RXIndex;
        globalStreams[ ucStream ].RXIndex = ( globalStreams[ ucStream ].RXIndex+1 ) % globalStreams[ ucStream ].RXBufferLen; // modulo prevents overrun
        --globalStreams[ ucStream ].RXLen;
        return( globalStreams[ ucStream ].RXBuffer[ i ] );
    } // if

    return(  0 );
}

u8 streamPutChar( u8 ucStream, u8 ucChar )
{
    u16 uiIndex;
    
    //for( i = 0; i < 1000; i++ ){
        if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].TXLen < ( globalStreams[ ucStream ].TXBufferLen - 1 ) ){
            uiIndex = ( globalStreams[ ucStream ].TXIndex + globalStreams[ ucStream ].TXLen ) % globalStreams[ ucStream ].TXBufferLen;
            globalStreams[ ucStream ].TXBuffer[ uiIndex ] = ucChar;
            ++globalStreams[ ucStream ].TXLen;
			return( 1 );
        } // if
        //delay( 50 );
    //}
    
    return( 0 );
}

void streamFIFOToString( u8 ucStream, u8 *ucBuffer )
{
    for( ; globalStreams[ ucStream ].RXLen; ){
        *(ucBuffer++) = streamGetChar( ucStream );
    } // for
    *ucBuffer = '\0'; // NULL Terminate
}

u8 streamIsEmpty( u8 ucStream )
{
    if( globalStreams[ ucStream ].RXLen ){
        return( 0 ); // Still data in buffer
    } // if
    
    return( 1 ); // Buffer empty
}

u8 streamPopChar( u8 ucStream )
{
    // Function returns last char received and decrements length
    u16 i;

    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].RXLen ){
        --globalStreams[ ucStream ].RXLen;
        i = ( globalStreams[ ucStream ].RXIndex + ( globalStreams[ ucStream ].RXLen ) ) % globalStreams[ ucStream ].RXBufferLen;
        return( globalStreams[ ucStream ].RXBuffer[ i ] );
    } // if
    
    return( 0 );
}

u8 streamPeekChar( u8 ucStream )
{
    // Function returns value of last char without affecting FIFO
    u16 i;

    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].RXLen ){
        i = ( globalStreams[ ucStream ].RXIndex + ( globalStreams[ ucStream ].RXLen - 1 ) ) % globalStreams[ ucStream ].RXBufferLen;
        return( globalStreams[ ucStream ].RXBuffer[ i ] );
    } // if
    
    return( 0 );
}

void streamPushChar( u8 ucStream, u8 ucChar )
{
    // Function appends char to FIFO and increments length
    u16 i;

    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].RXLen < globalStreams[ ucStream ].RXBufferLen){
        i = ( globalStreams[ ucStream ].RXIndex + globalStreams[ ucStream ].RXLen ) % globalStreams[ ucStream ].RXBufferLen;
        ++globalStreams[ ucStream ].RXLen;
        globalStreams[ ucStream ].RXBuffer[ i ] = ucChar;
    } // if
}

u8 streamSetPut( u8 ucStream, void *ptrFunc )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].Put = ptrFunc;
    
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetGet( u8 ucStream, void *ptrFunc )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].Get = ptrFunc;
    
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetRXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].RXBuffer = ucBuffer;
        globalStreams[ ucStream ].RXBufferLen = uiLen;
        return( 1 );
    } // if

    return( 0 );
}

u8 streamSetTXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen )
{
    if( ucStream < MAXCOMPORTS ){
        globalStreams[ ucStream ].TXBuffer = ucBuffer;
        globalStreams[ ucStream ].TXBufferLen = uiLen;
        return( 1 );
    } // if
    
    return( 0 );
}



void streamSetActionChars( u8 ucStream, u8 *ucString )
{
    // Warning! ucString must be NULL terminated
    globalStreams[ ucStream ].ActionChars = ucString;
}

void streamDisableDefaultGet( u8 ucStream )
{
    globalStreams[ ucStream ].CR |= PYGMY_STREAMS_USERHANDLER;
}

void streamEnableDefaultGet( u8 ucStream )
{
    globalStreams[ ucStream ].CR &= ~PYGMY_STREAMS_USERHANDLER;
}

u8 streamGetEcho( u8 ucStream )
{
    return( globalStreams[ ucStream ].CR & PYGMY_STREAMS_ECHO );
}

void streamEnableEcho( u8 ucStream )
{
    globalStreams[ ucStream ].CR |= PYGMY_STREAMS_ECHO;
    
}

void streamDisableEcho( u8 ucStream )
{
    globalStreams[ ucStream ].CR &= ~PYGMY_STREAMS_ECHO;
}

void streamEnableActionChars( u8 ucStream )
{
    globalStreams[ ucStream ].CR |= PYGMY_STREAMS_ACTIONCHARS;
}

void streamDisableActionChars( u8 ucStream )
{
    globalStreams[ ucStream ].CR &= ~PYGMY_STREAMS_ACTIONCHARS;
}

void streamEnableBackspace( u8 ucStream )
{
    globalStreams[ ucStream ].CR |= PYGMY_STREAMS_BACKSPACE;
}

void streamDisableBackspace( u8 ucStream )
{
    globalStreams[ ucStream ].CR &= ~PYGMY_STREAMS_BACKSPACE;
}

void streamHandler( u8 ucPort, u8 ucChar )
{
    u8 ucAction;

    ucAction = isCharInString( ucChar, globalStreams[ ucPort ].ActionChars );
    if( ( globalStreams[ ucPort ].CR & PYGMY_STREAMS_ECHO ) && !ucAction ){
        streamEchoChar( ucPort, ucChar );
    } //
    if( ( globalStreams[ ucPort ].CR & PYGMY_STREAMS_BACKSPACE ) && ucChar == '\b' ){
        streamPopChar( ucPort );
    } else{
        streamPushChar( ucPort, ucChar );
    } // else
    if( globalStreams[ ucPort ].Get ){
        if( !( globalStreams[ ucPort ].CR & PYGMY_STREAMS_ACTIONCHARS ) || 
            (( globalStreams[ ucPort ].CR & PYGMY_STREAMS_ACTIONCHARS ) && ucAction )){
            globalStreams[ ucPort ].Get();
        } // if
    } // if
}

#ifdef __PYGMYSTREAMCOM1
void USART1_IRQHandler( void )
{
    u8 ucChar;

    if( USART1->SR & USART_RXNE){
        ucChar = USART1->DR;
        streamHandler( COM1, ucChar );
    } // if
    if( USART1->SR & USART_TXE ){
       streamTXChar( COM1, USART1 );
    } // if
    USART1->SR = 0;
}

u8 putcUSART1( u8 Byte )
{
    u16 i;

    for( i = 0; i < 1000; i++ ){
        if( USART1->SR & USART_TXE ){
            USART1->DR = Byte;
            return( TRUE );
        } // if
    } // for

    return( FALSE );
}

u8 putsUSART1( u8 *ucBuffer )
{
    for( ; *ucBuffer ; ){
        if( USART1->SR & USART_TXE ){
            USART1->DR = *(ucBuffer++);
        } // if   
    } // for

    return( 1 );
}

u8 putsUSART1FIFO( u8 *ucBuffer )
{
    u16 i;
	
    for( ; *ucBuffer; ){
		USART1->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM1BUFFERLEN; i++ ){
			if( streamPutChar( COM1, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART1->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
	return( 1 );
}
#endif // __PYGMYSTREAMCOM1

#ifdef __PYGMYSTREAMCOM2
void USART2_IRQHandler( void )
{
    u16 i;
    u8 ucChar, *ucChars;

    if( USART2->SR & USART_RXNE){
        ucChar = USART2->DR;
        //USART2->SR = 0;
        USART3->DR = ucChar;
        //return;
        if( ucChar > 31 ){
            if( globalStreams[ COM2 ].RXLen < globalStreams[ COM2 ].RXBufferLen){
                i = ( globalStreams[ COM2 ].RXIndex + globalStreams[ COM2 ].RXLen ) % globalStreams[ COM2 ].RXBufferLen;
                ++globalStreams[ COM2 ].RXLen;
                globalStreams[ COM2 ].RXBuffer[ i ] = ucChar;
            } // if
            //streamPushChar( COM2, ucChar );
        } // if
        //if( globalStreams[ ucPort ].CR & PYGMY_STREAMS_ACTIONCHARS ) ){
        //if( globalStreams[ COM2 ].RXLen ){
        //for( ucChars = globalStreams[ COM2 ].ActionChars; *ucChars; ) {
        //    if( ucChar == *(ucChars++) ){
            if( ucChar == '>' || ( globalStreams[ COM2 ].RXLen > 1 && ucChar == 10 ) ){
                //if( globalStreams[ COM2 ].Get ){
                    globalStreams[ COM2 ].Get();
                //break;
            } // if
        //}// // for
        //}
        //streamHandler( COM2, ucChar );
        //} // if
    } // if
    //if( USART2->SR & USART_TXE ){
    //    print( COM3, "TX" );
    //   streamTXChar( COM2, USART2 );
    //} // if
    USART2->SR = 0;
}
//#else

//#endif

u8 putsUSART2( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART2->SR & USART_TXE ){
            USART2->DR = *(ucBuffer++);
            //while( !(USART2->SR & USART_TXE ) );
        } // if   
    } // for
    
    return( 1 );
}

u8 putsUSART2FIFO( u8 *ucBuffer )
{   
    u16 i;
	
    for( ; *ucBuffer; ){
		USART2->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM2BUFFERLEN; i++ ){
			if( streamPutChar( COM2, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART2->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
	return( 1 );
}
#endif // __PYGMYSTREAMCOM2

#ifdef __PYGMYSTREAMCOM3
void USART3_IRQHandler( void )
{
    u8 ucChar;//, ucAction;

    if( USART3->SR & USART_RXNE){
        if( globalStreams[ COM3 ].CR & PYGMY_STREAMS_USERHANDLER ){
            globalStreams[ COM3 ].Get();
        } else{
            ucChar = USART3->DR;
            streamHandler( COM3, ucChar );
        } // else
    } // if
    if( USART3->SR & USART_TXE ){
       streamTXChar( COM3, USART3 );
    } // if
    USART3->SR = 0;
}

u8 putcUSART3( u8 ucData )
{
    u16 i;

    for( i = 0; i < 1000; i++ ){
        if( USART3->SR & USART_TXE ){
            USART3->DR = ucData;
            return( TRUE );
        } // if
    } // for

    return( FALSE );
}

u8 putBufferUSART3( u8 *ucBuffer, u16 uiLen )
{
     u16 i;

    for( i = 0; i < uiLen ; i++ ){
        putcUSART3( ucBuffer[ i ] ) ;
    } // for
}

u8 putsUSART3( u8 *ucBuffer )
{ 
    //u16 i;

    for( ; *ucBuffer ; ){
        //if( USART3->SR & USART_TXE ){
        //    USART3->DR = *(ucBuffer++);
        //    return( TRUE );
        //} // if  
        if( !putcUSART3( *(ucBuffer++) ) ){
            return( FALSE );
        } // if
    } // for
    
    return( TRUE );
}

u8 putsUSART3FIFO( u8 *ucBuffer )
{   
    u16 i;
	
    for( ; *ucBuffer; ){
		USART3->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM3BUFFERLEN; i++ ){
			if( streamPutChar( COM3, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART3->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
    return( 1 );
}
#endif // __PYGMYSTREAMCOM3

#ifdef __PYGMYSTREAMCOM4
void USART4_IRQHandler( void )
{
    u8 ucChar;//, ucAction;

    if( USART4->SR & USART_RXNE){
        ucChar = USART4->DR;
        streamHandler( COM4, ucChar );
    } // if
    if( USART4->SR & USART_TXE ){
       streamTXChar( COM4, USART4 );
    } // if
    USART4->SR = 0;
}

u8 putsUSART4( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART4->SR & USART_TXE ){
            USART4->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
}

u8 putsUSART4FIFO( u8 *ucBuffer )
{
    u16 i;
	
    for( ; *ucBuffer; ){
		USART4->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM4BUFFERLEN; i++ ){
			if( streamPutChar( COM4, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART4->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
	return( 1 );
}
#endif // __PYGMYSTREAMCOM4

#ifdef __PYGMYSTREAMCOM5
void USART5_IRQHandler( void )
{
    u8 ucChar;//, ucAction;

    if( USART5->SR & USART_RXNE){
        ucChar = USART5->DR;
        streamHandler( COM5, ucChar );
    } // if
    if( USART5->SR & USART_TXE ){
       streamTXChar( COM5, USART5 );
    } // if
    USART5->SR = 0;
}

u8 putsUSART5( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART5->SR & USART_TXE ){
            USART5->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
}

u8 putsUSART5FIFO( u8 *ucBuffer )
{
    u16 i;
	
    for( ; *ucBuffer; ){
		USART5->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM5BUFFERLEN; i++ ){
			if( streamPutChar( COM5, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART5->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
	return( 1 );
}
#endif // __PYGMYSTREAMCOM5

#ifdef __PYGMYSTREAMCOM6
void USART6_IRQHandler( void )
{
    u8 ucChar;

    if( USART6->SR & USART_RXNE){
        ucChar = USART6->DR;
        streamHandler( COM6, ucChar );
    } // if
    if( USART6->SR & USART_TXE ){
       streamTXChar( COM5, USART6 );
    } // if
    USART6->SR = 0;
}

u8 putsUSART6( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART6->SR & USART_TXE ){
            USART6->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
}

u8 putsUSART6FIFO( u8 *ucBuffer )
{
    u16 i;
	
    for( ; *ucBuffer; ){
		USART6->CR1 &= ~USART_TXEIE;
        for( i = 0; i < __PYGMYCOM6BUFFERLEN; i++ ){
			if( streamPutChar( COM6, *ucBuffer ) ){
				++ucBuffer;
				break;
			} // if
			USART6->CR1 |= USART_TXEIE;
		} // for
		
    } // for
    
	return( 1 );
}
#endif // __PYGMYSTREAMCOM6
#endif // __PYGMYSTREAMS
