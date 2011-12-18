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

#ifdef __PYGMYSTREAMS
    PYGMYFIFO globalStreams[ MAXCOMPORTS ];
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

void comConfig( u8 ucPort, u8 ucProtocol, u8 ucOptions, u32 uiRate )
{
    USART_TYPEDEF *ptrUSART;
    // ToDo: Add multi-protocol support
   
    //sysEnableComClock( ucPort );
    #ifdef __PYGMYSTREAMCOM1
        if( ucPort == COM1 ){
            PYGMY_RCC_USART1_ENABLE;
            enableInterrupt( USART1_IRQ );
            setInterruptPriority( USART1_IRQ, 1 );
            ptrUSART = USART1;
            if( ucOptions & RTS ){
                PA12_ALT;
            } // if
            if( ucOptions & CTS ){
                PA11_FLOAT;
            } // if
            PA9_ALT;
            PA10_FLOAT;
        } // if
    #endif // __PYGMYSTREAMCOM1
    #ifdef __PYGMYSTREAMCOM2
        if( ucPort == COM2 ){
            PYGMY_RCC_USART2_ENABLE;
            setInterruptPriority( USART2_IRQ, 2 );
            enableInterrupt( USART2_IRQ );
            if( ucOptions & RTS ){
                PA1_ALT;
            } // if
            if( ucOptions & CTS ){
                PA0_FLOAT;
            } // if
            PA2_ALT;
            PA3_FLOAT;
        } // if
    #endif // __PYGMYSTREAMCOM2
    #ifdef __PYGMYSTREAMCOM3
        if( ucPort == COM3 ){
            PYGMY_RCC_USART3_ENABLE;
            setInterruptPriority( USART3_IRQ, 3 );
            enableInterrupt( USART3_IRQ );
            if( ucOptions & RTS ){
                PB14_ALT;
            } // if
            if( ucOptions & CTS ){
                PB13_FLOAT;
            } // if
            PB10_ALT;
            PB11_FLOAT;
        } // if
    #endif // __PYGMYSTREAMCOM3
    #ifdef __PYGMYSTREAMCOM4
        if( ucPort == COM4 ){
            PYGMY_RCC_USART4_ENABLE;
            enableInterrupt( USART4_IRQ );
            setInterruptPriority( USART4_IRQ, 1 );
            ptrUSART = USART4;
            PC10_ALT;
            PC11_FLOAT;
        } // if
    #endif // __PYGMYSTREAMCOM4
    #ifdef __PYGMYSTREAMCOM5
        if( ucPort == COM5 ){
            PYGMY_RCC_USART5_ENABLE;
            enableInterrupt( USART5_IRQ );
            setInterruptPriority( USART5_IRQ, 1 );
            ptrUSART = USART5;
            PC12_ALT;
            PD2_FLOAT;
        } // if
    #endif // __PYGMYSTREAMCOM5
    #ifdef __PYGMYSTREAMCOM6
        if( ucPort == COM6 ){
            PYGMY_RCC_USART6_ENABLE;
            enableInterrupt( USART6_IRQ );
            setInterruptPriority( USART6_IRQ, 1 );
            ptrUSART = USART6;
            PG10_ALT;
            PG11_FLOAT;
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
    ptrUSART->BRR = generateBaud( sysGetMainClock(), uiRate );
    if( ucOptions & TXIE ){
        ptrUSART->CR1 = ( USART_UE | USART_TXEIE | USART_RXNEIE | USART_TE | USART_RE  );
    } else{
        ptrUSART->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
    } // else
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

u16 generateBaud( u32 uiClock, u32 uiRate )
{
    // USARTDIV = DIV_Mantissa + (DIV_Fraction / 8 � (2 � OVER8))
    u16 uiBRR;
    
    // APB bus is limited to 36MHz, if clock is above that then div by 4
    if( uiClock == 72000000 || uiClock == 48000000  ){
        uiClock /= 4;
    } // if
    uiBRR = ( ( (uiClock >> 3 ) / uiRate ) << 4 ) + ( ( ( uiClock / uiRate ) ) & 0x0007 );

    return( (u16)uiBRR ); 
}

#ifdef __PYGMYSTREAMCOM1
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
u8 putsUSART2( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART2->SR & USART_TXE ){
            USART2->DR = *(ucBuffer++);
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
u8 putsUSART3( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART3->SR & USART_TXE ){
            USART3->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
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
    
    pygmyPort->PortCS   = getPortFromPin( ucCS );
    pygmyPort->PortA0   = getPortFromPin( ucA0 );
    pygmyPort->PortWR   = getPortFromPin( ucWR );
    pygmyPort->PortRD   = getPortFromPin( ucRD );
    pygmyPort->PortDATA = getPortFromPin( ucD0 );
    
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
    
    pygmyPort->PortCS->BRR = pygmyPort->PinCS;
    pygmyPort->PortDATA->ODR &= ~pygmyPort->Mask;
    pygmyPort->PortDATA->ODR |= uiWord;
    pygmyPort->PortWR->BRR = pygmyPort->PinWR; // Go high to set to inactive
    pygmyPort->PortWR->BSRR = pygmyPort->PinWR; // Go low to clock out data/cmd
    pygmyPort->PortCS->BSRR = pygmyPort->PinCS;
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

void spiWriteByte( PYGMYSPIPORT *pygmySPI, u8 ucByte )
{
    // Clocks out 8 bits
	u16 i;
	
	pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Clock starts low, low-high-low clocks data in or out
	for( i = 24; i < 32; i++ ){ 		                        // 
		//if( ucByte & ( BIT7 >> i ) ){                       // 	
        if( ucByte & PYGMY_INVBITMASKS[ i ] ){
            pygmySPI->PortMOSI->BSRR = pygmySPI->PinMOSI;   // MasterOutSlaveIn high if bit set
		} else{                                             //
            pygmySPI->PortMOSI->BRR = pygmySPI->PinMOSI;  // MasterOutSlaveIn low if bit clear
        } // else                                           //
		pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;			// clock must start low, transition high
		pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	    // Low transition finishes clock sequence
    } // for
} 

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
} 

u8 spiReadByte( PYGMYSPIPORT *pygmySPI )
{
    // Clocks in 8 bits
    u16 i;
	u8 ucByte;
	
    pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;             // Clock starts low, low-high-low clocks data in or out
	for( i = 24, ucByte = 0; i < 32; i++ ){                   // 
		pygmySPI->PortSCK->BSRR = pygmySPI->PinSCK;          // clock must start low, transition high 			
        if( pygmySPI->PortMISO->IDR & pygmySPI->PinMISO ){    // Test port input for high and set bit in ucByte
            ucByte |= PYGMY_INVBITMASKS[ i ];
			//ucByte |= ( BIT7 >> i );                        //
        } // if                                             //
		pygmySPI->PortSCK->BRR = pygmySPI->PinSCK;	        // Low transition finishes clock sequence
    } // for
	
	return( ucByte );
}

void spiConfig( PYGMYSPIPORT *pygmySPI, u8 ucCS, u8 ucSCK, u8 ucMISO, u8 ucMOSI )
{
    pinConfig( ucCS, OUT );
    pinConfig( ucSCK, OUT );
    if( ucMOSI != NONE ){
        pinConfig( ucMOSI, OUT );
        pygmySPI->PortMOSI  = getPortFromPin( ucMOSI );
        pygmySPI->PinMOSI   = PYGMY_BITMASKS[ ucMOSI % 16 ];
    } // if
    if( ucMISO != NONE ){
        pinConfig( ucMISO, IN );
        pygmySPI->PortMISO  = getPortFromPin( ucMISO );
        pygmySPI->PinMISO   = PYGMY_BITMASKS[ ucMISO % 16 ];
    } // if

    pygmySPI->PortCS    = getPortFromPin( ucCS );
    pygmySPI->PortSCK   = getPortFromPin( ucSCK );   
    
    pygmySPI->PinCS     = PYGMY_BITMASKS[ ucCS % 16 ];
    pygmySPI->PinSCK    = PYGMY_BITMASKS[ ucSCK % 16 ];   
    
    pinSet( ucSCK, LOW );
    pinSet( ucCS, HIGH );
}

void i2cConfig( PYGMYI2CPORT *pygmyI2C, u8 ucSCL, u8 ucSDA, u16 uiSpeed )
{
    pinConfig( ucSCL, PULLUP );
    pinConfig( ucSDA, PULLUP );
    
    pygmyI2C->SCL = ucSCL;
    pygmyI2C->SDA = ucSDA;
    pygmyI2C->PortSCL   = getPortFromPin( ucSCL );
    pygmyI2C->PortSDA   = getPortFromPin( ucSDA );
                        
    pygmyI2C->PinSCL    = PYGMY_BITMASKS[ ucSCL % 16 ];
    pygmyI2C->PinSDA    = PYGMY_BITMASKS[ ucSDA % 16 ];
    pygmyI2C->Speed = uiSpeed;
    //pygmyI2C->Status = 0;
}

void i2cDelay( PYGMYI2CPORT *pygmyI2C ) 
{ 
    volatile u16 i; 
    
    for( i = 0; i < pygmyI2C->Speed; ) {
        ++i;
    } // for
}
 
 
void i2cStart( PYGMYI2CPORT *pygmyI2C )
{
    u16 i;
    
    // if already started then restart
    //if ( pygmyI2C->Status ) {
        // float SDA  
        pinConfig( pygmyI2C->SDA, PULLUP );
        i2cDelay( pygmyI2C );
        // allow clock stretching
        pinConfig( pygmyI2C->SCL, PULLUP );
        for( i = 0; i < 0xFFFF; i++ ){
            if( pygmyI2C->PortSCL->IDR & pygmyI2C->PinSCL ){
                break;
            } // if
        } // for
    //}
    if ( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
        // ToDo: Handle loss of arbitration
    } //
    pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output 
    pinConfig( pygmyI2C->SDA, OUT ); // set to output to drive low
    
    i2cDelay( pygmyI2C );
    
    pygmyI2C->PortSCL->ODR &= ~pygmyI2C->PinSCL; // set state before setting to output
    pinConfig( pygmyI2C->SCL, OUT );
    
    //pygmyI2C->Status = 1; // mark transaction as started
}
 
void i2cStop( PYGMYI2CPORT *pygmyI2C )
{
    u16 i;

    pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output 
    pinConfig( pygmyI2C->SDA, OUT ); // set to output to drive low
    i2cDelay( pygmyI2C );
    // allow clock stretching   
    pinConfig( pygmyI2C->SCL, PULLUP );
    for( i = 0; i < 0xFFFF; i++ ){
        if( !( pygmyI2C->PortSCL->IDR & pygmyI2C->PinSCL ) ){
            break;
        } // if
    } // for 
    // while SCL is high, allow SDA to float high and check state
    pinConfig( pygmyI2C->SDA, PULLUP );
    if ( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
        // ToDo: Handle loss of arbitration
    } // 
    i2cDelay( pygmyI2C );
    //pygmyI2C->Status = 0; // Transaction terminated
}
 
void i2cWriteBit( PYGMYI2CPORT *pygmyI2C, u8 ucBit )
{
    // write one bit
    u16 i;

    if ( ucBit ){
        pinConfig( pygmyI2C->SDA, PULLUP );
    } else{ 
        pygmyI2C->PortSDA->ODR &= ~pygmyI2C->PinSDA; // set state before setting to output
        pinConfig( pygmyI2C->SDA, OUT );
    } // else
    i2cDelay( pygmyI2C );
    // allow clock stretching 
    pinConfig( pygmyI2C->SCL, PULLUP );
    for( i = 0; i < 0xFFFF; i++ ){
        if( !( pygmyI2C->PortSCL->IDR & pygmyI2C->PinSCL ) ){
            break;
        } // if
    } // for
    // once SCL is high data is valid ( clocked in ) 
    // If SDA should be 1, check that SDA isn't being driven by another node
    
    if ( ucBit ){  
        pinConfig( pygmyI2C->SDA, PULLUP );  
        if( !( pygmyI2C->PortSDA->IDR & pygmyI2C->PinSDA ) ){
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
    u16 i;
    u8 ucBit;

    pinConfig( pygmyI2C->SDA, PULLUP );
    i2cDelay( pygmyI2C );
    // allow clock stretching 
    pinConfig( pygmyI2C->SCL, PULLUP );
    for( i = 0; i < 0xFFFF; i++ ){
        if( !( pygmyI2C->PortSCL->IDR & pygmyI2C->PinSCL ) ){
            break;
        } // if
    } // for
    
    // data clock in with SCL high
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

u8 i2cWriteBuffer( PYGMYI2CPORT *pygmyI2C, u8 ucAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;
    
    i2cStart( pygmyI2C );
    i2cWriteByte( pygmyI2C, ucAddress );
    for( i = 0; i < uiLen; i++ ){
        i2cWriteByte( pygmyI2C, ucBuffer[ i ] );
    } // for
    i2cStop( pygmyI2C );
}

u8 i2cWriteByte( PYGMYI2CPORT *pygmyI2C, u8 ucByte )
{
    // write a byte and return ack/nack from slave
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
    
    return( ucAck );
}
 
u8 i2cReadByte( PYGMYI2CPORT *pygmyI2C )
{
    // read and return one byte  
    u8 ucByte;
    u8 i;
        
    for( i = 0, ucByte = 0; i < 8; i++ ){
        ucByte = ( ucByte << 1 ) | i2cReadBit( pygmyI2C );
    } // for
    i2cWriteBit( pygmyI2C, 0 ); // Low ACK
        
    return( ucByte );
}

//--------------------------------------------------------------------------------------------
//-------------------------------------Pygmy OS IO Stream-------------------------------------
#ifdef __PYGMYSTREAMS
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
        globalStreams[ ucStream ].RXBufferLen  = 0;
        globalStreams[ ucStream ].RXIndex      = 0;
        globalStreams[ ucStream ].RXLen        = 0; 
        globalStreams[ ucStream ].TXBufferLen  = 0;
        globalStreams[ ucStream ].TXIndex      = 0;
        globalStreams[ ucStream ].TXLen        = 0;
        globalStreams[ ucStream ].Put          = (void *)TaskException_Handler;
        globalStreams[ ucStream ].Get          = (void *)TaskException_Handler;
        globalStreams[ ucStream ].RXBuffer     = NULL;
        globalStreams[ ucStream ].TXBuffer     = NULL;
        
        return( 1 );
    } // if

    return( 0 );
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

void streamTXChar( u8 ucStream, void *pygmyUSART )
{
    USART_TYPEDEF *ptrUSART = (USART_TYPEDEF *)pygmyUSART;
    
    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].TXLen ){
		--globalStreams[ ucStream ].TXLen;  
		ptrUSART->DR = globalStreams[ ucStream ].TXBuffer[ globalStreams[ ucStream ].TXIndex ]; 
        globalStreams[ ucStream ].TXIndex = ( globalStreams[ ucStream ].TXIndex + 1 ) % 
            globalStreams[ ucStream ].TXBufferLen;
    } else{
        ptrUSART->CR1 &= ~USART_TXEIE;
    } // else
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

u8 streamPopChar( u8 ucStream )
{
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
    u16 i;

    if( ucStream < MAXCOMPORTS && globalStreams[ ucStream ].RXLen ){
        i = ( globalStreams[ ucStream ].RXIndex + ( globalStreams[ ucStream ].RXLen - 1 ) ) % globalStreams[ ucStream ].RXBufferLen;
        return( globalStreams[ ucStream ].RXBuffer[ i ] );
    } // if
    
    return( 0 );
}

void streamPushChar( u8 ucStream, u8 ucChar )
{
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
#endif

#ifdef __PYGMYSTREAMCOM1
void USART1_IRQHandler( void )
{
    if( USART1->SR & USART_RXNE){
        streamPushChar( COM1, USART1->DR ); 
        if( globalStreams[ COM1 ].Get ){
            globalStreams[ COM1 ].Get();
        } // if
    } // if
    if( USART1->SR & USART_TXE ){
        streamTXChar( COM1, USART1 );
		
    } // if
    USART1->SR = 0;
}
#endif

#ifdef __PYGMYSTREAMCOM2
void USART2_IRQHandler( void )
{
    if( USART2->SR & USART_RXNE){
        streamPushChar( COM2, USART2->DR ); 
        if( globalStreams[ COM2 ].Get ){
            globalStreams[ COM2 ].Get();
        } // if
    } // if
    if( USART2->SR & USART_TXE ){
        streamTXChar( COM2, USART2 );
    } // if
    USART2->SR = 0;
}
#endif

#ifdef __PYGMYSTREAMCOM3
void USART3_IRQHandler( void )
{
    if( USART3->SR & USART_RXNE){
        streamPushChar( COM3, USART3->DR ); 
        if( globalStreams[ COM3 ].Get ){
            globalStreams[ COM3 ].Get();
        } // if
    } // if
    if( USART3->SR & USART_TXE ){
       streamTXChar( COM3, USART3 );
	   /*if( pygmyGlobalData.Stream[ COM3 ].TXLen ){
			--pygmyGlobalData.Stream[ COM3 ].TXLen;  
			USART3->DR = pygmyGlobalData.Stream[ COM3 ].TXBuffer[ pygmyGlobalData.Stream[ COM3 ].TXIndex ]; 
			pygmyGlobalData.Stream[ COM3 ].TXIndex = ( pygmyGlobalData.Stream[ COM3 ].TXIndex + 1 ) % 
				pygmyGlobalData.Stream[ COM3 ].TXBufferLen;
		} else {
			USART3->CR1 &= ~USART_TXEIE;
		} // else
        */
    } // if
    USART3->SR = 0;
}
#endif

#ifdef __PYGMYSTREAMCOM4
void USART4_IRQHandler( void )
{
    if( USART4->SR & USART_RXNE){
        streamPushChar( COM4, USART4->DR ); 
        if( globalStreams[ COM4 ].Get ){
            globalStreams[ COM4 ].Get();
        } // if
    } // if
    if( USART4->SR & USART_TXE ){
       streamTXChar( COM4, USART4 );
    } // if
    USART4->SR = 0;
}
#endif

#ifdef __PYGMYSTREAMCOM5
void USART5_IRQHandler( void )
{
    if( USART5->SR & USART_RXNE){
        streamPushChar( COM5, USART5->DR ); 
        if( globalStreams[ COM5 ].Get ){
            globalStreams[ COM5 ].Get();
        } // if
    } // if
    if( USART5->SR & USART_TXE ){
       streamTXChar( COM5, USART5 );
    } // if
    USART5->SR = 0;
}
#endif