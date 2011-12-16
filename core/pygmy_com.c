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


void comConfig( u8 ucPort, u8 ucProtocol, u8 ucOptions, u32 uiRate )
{
    // ToDo: Add multi-protocol support
    // ToDo: Rewrite to use pointer to USART registers to consolidate shared code
    if( ucPort == COM1 ){
        PYGMY_RCC_USART1_ENABLE;
        enableInterrupt( USART1_IRQ );
        setInterruptPriority( USART1_IRQ, 1 );
        USART1->CR3 = 0;
        if( ucOptions & RTS ){
            pinConfig( PA12, ALT );
            USART1->CR3 |= USART_RTSE;
        } // if
        if( ucOptions & CTS ){
            pinConfig( PA11, IN );
            USART1->CR3 |= USART_CTSE;
        } // if
        pinConfig( PA9, ALT );
        pinConfig( PA10, IN );
        USART1->BRR = generateBaud( pygmyGlobalData.MainClock, uiRate );
        if( ucOptions & TXIE ){
            USART1->CR1 = ( USART_UE | USART_TXEIE | USART_RXNEIE | USART_TE | USART_RE  );
        } else{
            USART1->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        } // else
    } else if( ucPort == COM2 ){
        PYGMY_RCC_USART2_ENABLE;
        setInterruptPriority( USART2_IRQ, 2 );
        enableInterrupt( USART2_IRQ );
        if( ucOptions & RTS ){
            pinConfig( PA1, ALT );
            USART2->CR3 |= USART_RTSE;
        } // if
        if( ucOptions & CTS ){
            pinConfig( PA0, IN );
            USART2->CR3 |= USART_CTSE;
        } // if
        pinConfig( PA2, ALT );
        pinConfig( PA3, IN );
        USART2->BRR = generateBaud( pygmyGlobalData.MainClock, uiRate );
        USART2->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        if( ucOptions & TXIE ){
            USART2->CR1 = ( USART_UE | USART_TXEIE | USART_RXNEIE | USART_TE | USART_RE  );
        } else{
            USART2->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        } // else
    } else if( ucPort == COM3 ){
        PYGMY_RCC_USART3_ENABLE;
        setInterruptPriority( USART3_IRQ, 3 );
        enableInterrupt( USART3_IRQ );
        if( ucOptions & RTS ){
            pinConfig( PB14, ALT );
            USART3->CR3 |= USART_RTSE;
        } // if
        if( ucOptions & CTS ){
            pinConfig( PB13, IN );
            USART3->CR3 |= USART_CTSE;
        } // if
        pinConfig( PB10, ALT );
        pinConfig( PB11, IN );
        USART3->BRR = generateBaud( pygmyGlobalData.MainClock, uiRate );
        USART3->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        //if( ucOptions & TXIE ){
        //    USART3->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        //} else{
        //    USART3->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );
        //} // else
    } // else if
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
    // USARTDIV = DIV_Mantissa + (DIV_Fraction / 8 × (2 – OVER8))
    u16 uiBRR;
    
    // APB bus is limited to 36MHz, if clock is above that then div by 4
    if( uiClock == 72000000 || uiClock == 48000000  ){
        uiClock /= 4;
    } // if
    uiBRR = ( ( (uiClock >> 3 ) / uiRate ) << 4 ) + ( ( ( uiClock / uiRate ) ) & 0x0007 );

    return( (u16)uiBRR ); 
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

u8 putsUSART2( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART2->SR & USART_TXE ){
            USART2->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
}

u8 putsUSART3( u8 *ucBuffer )
{ 
    for( ; *ucBuffer ; ){
        if( USART3->SR & USART_TXE ){
            USART3->DR = *(ucBuffer++);
        } // if   
    } // for
    
    return( 1 );
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
