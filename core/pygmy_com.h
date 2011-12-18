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


#pragma once

#include "pygmy_profile.h"

//--------------------------------------------------------------------------------------------
//------------------------------------------I2C-----------------------------------------------
typedef struct
{
  vu16 CR1;
  u16 RESERVED0;
  vu16 CR2;
  u16 RESERVED1;
  vu16 OAR1;
  u16 RESERVED2;
  vu16 OAR2;
  u16 RESERVED3;
  vu16 DR;
  u16 RESERVED4;
  vu16 SR1;
  u16 RESERVED5;
  vu16 SR2;
  u16 RESERVED6;
  vu16 CCR;
  u16 RESERVED7;
  vu16 TRISE;
  u16 RESERVED8;
} I2C_TYPEDEF;

//------------------------------------------END I2C-----------------------------------------------
//--------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------
//------------------------------------------SPI-----------------------------------------------
typedef struct
{
  vu16 CR1;
  u16 RESERVED0;
  vu16 CR2;
  u16 RESERVED1;
  vu16 SR;
  u16  RESERVED2;
  vu16 DR;
  u16  RESERVED3;
  vu16 CRCPR;
  u16 RESERVED4;
  vu16 RXCRCR;
  u16  RESERVED5;
  vu16 TXCRCR;
  u16  RESERVED6;
} SPI_TYPEDEF;

// Deprecated
#define PYGMY_SPI2_RESET                 PYGMY_RCC_SPI2_DISABLE; PYGMY_RCC_SPI2_ENABLE
#define PYGMY_SPI1_RESET                 PYGMY_RCC_SPI1_DISABLE; PYGMY_RCC_SPI1_ENABLE
#define PYGMY_SPI1_SETMASTER             SPI1->CR1 |= CR1_SPE_Set
#define PYGMY_SPI1_CLEARMASTER           SPI1->CR1 &= ~CR1_SPE_Set
#define PYGMY_SPI2_SETMASTER             SPI2->CR1 |= CR1_SPE_Set
#define PYGMY_SPI2_CLEARMASTER           SPI2->CR1 &= ~CR1_SPE_Set
#define PYGMY_SPI1_SEND( u16Data )       SPI1->DR = u16Data
#define PYGMY_SPI2_SEND( u16Data )       SPI2->DR = u16Data
#define PYGMY_SPI1_RECEIVE               SPI1->DR
#define PYGMY_SPI2_RECEIVE               SPI2->DR
#define PYGMY_SPI1_WAIT                  while( SPI1->SR & BSY  )
#define PYGMY_SPI2_WAIT                  while( SPI2->SR & BSY  )
#define PYGMY_SPI1_ENABLE                SPI1->CR1 |= SPE
#define PYGMY_SPI1_DISABLE               SPI1->CR1 &= ~SPE
#define PYGMY_SPI2_ENABLE                SPI2->CR1 |= SPE
#define PYGMY_SPI2_DISABLE               SPI2->CR1 &= ~SPE



#define SPI_DIRECTION_2Lines_FullDuplex     0x0000
#define SPI_DIRECTION_2Lines_RxOnly         0x0400
#define SPI_DIRECTION_1Line_Rx              0x8000
#define SPI_DIRECTION_1Line_Tx              0xC000
#define SPI_MODE_MASTER                     0x0104
#define SPI_MODE_SLAVE                      0x0000
#define SPI_DATASIZE_16b                    0x0800
#define SPI_DATASIZE_8b                     0x0000
#define SPI_CPOL_LOW                        0x0000
#define SPI_CPOL_HIGH                       0x0002
#define SPI_CPHA_1EDGE                      0x0000
#define SPI_CPHA_2EDGE                      0x0001
#define SPI_NSS_SOFT                        0x0200
#define SPI_NSS_HARD                        0x0000
#define SPI_BAUDRATEPRESCALER_2             0x0000
#define SPI_BAUDRATEPRESCALER_4             0x0008
#define SPI_BAUDRATEPRESCALER_8             0x0010
#define SPI_BAUDRATEPRESCALER_16            0x0018
#define SPI_BAUDRATEPRESCALER_32            0x0020
#define SPI_BAUDRATEPRESCALER_64            0x0028
#define SPI_BAUDRATEPRESCALER_128           0x0030
#define SPI_BAUDRATEPRESCALER_256           0x0038
#define SPI_FIRSTBIT_MSB                    0x0000
#define SPI_FIRSTBIt_LSB                    0x0080
#define SPI_DMAREQ_Tx                       0x0002
#define SPI_DMAREQ_Rx                       0x0001
#define SPI_NSSINTERNALSOFT_SET             0x0100
#define SPI_NSSINTERNALSOFT_RESET           0xFEFF
#define SPI_CRC_TX                          0x00
#define SPI_CRC_RX                          0x01
#define SPI_DIRECTION_RX                    0xBFFF
#define SPI_DIRECTION_TX                    0x4000
#define SPI_IT_TXE                          0x71
#define SPI_IT_RXNE                         0x60
#define SPI_IT_ERR                          0x50
#define SPI_IT_OVR                          0x56
#define SPI_IT_MODF                         0x55
#define SPI_IT_CRCERR                       0x54
#define SPI_FLAG_RXNE                       0x0001
#define SPI_FLAG_TXE                        0x0002
#define SPI_FLAG_CRCERR                     0x0010
#define SPI_FLAG_MODF                       0x0020
#define SPI_FLAG_OVR                        0x0040
#define SPI_FLAG_BSY                        0x0080


//--------------------------------------------------------------------------------------------
//-------------------------------------------SPI Bits-----------------------------------------
// SPI_CR1
#define SPI_BIDIMODE            BIT15               
#define SPI_BIDIOE              BIT14                 
#define SPI_CRCEN               BIT13              
#define SPI_CRCNEXT             BIT12              
#define SPI_DFF                 BIT11                
#define SPI_RXONLY              BIT10              
#define SPI_SSM                 BIT9                   
#define SPI_SSI                 BIT8                  
#define SPI_LSBFIRST            BIT7                   
#define SPI_SPE                 BIT6                  
#define SPI_BR2                 0
#define SPI_BR4                 BIT3                  
#define SPI_BR8                 BIT4                   
#define SPI_BR16                BIT4|BIT3             
#define SPI_BR32                BIT5                
#define SPI_BR64                BIT5|BIT3          
#define SPI_BR128               BIT5|BIT4              
#define SPI_BR256               BIT5|BIT4|BIT3        
#define SPI_MSTR                BIT2                
#define SPI_CPOL                BIT1                 
#define SPI_CPHA                BIT0                  
//SPI_CR2
#define SPI_TXEIE               BIT7                
#define SPI_RXNEIE              BIT6                
#define SPI_ERRIE               BIT5               
#define SPI_SSOE                BIT2                 
#define SPI_TXDMAEN             BIT1             
#define SPI_RXDMAEN
//SPI_SR Status Register
#define SPI_BSY                 BIT7              
#define SPI_OVR                 BIT6           
#define SPI_MODF                BIT5             
#define SPI_CRCERR              BIT4              
#define SPI_UDR                 BIT3             
#define SPI_CHSIDE              BIT2              
#define SPI_TXE                 BIT1               
#define SPI_RXNE                BIT0                

//----------------------------------------End SPI---------------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//------------------------------------Hardware USARTS-----------------------------------------
typedef struct
{
  vu16 SR;
  u16 RESERVED0;
  vu16 DR;
  u16 RESERVED1;
  vu16 BRR;
  u16 RESERVED2;
  vu16 CR1;
  u16 RESERVED3;
  vu16 CR2;
  u16 RESERVED4;
  vu16 CR3;
  u16 RESERVED5;
  vu16 GTPR;
  u16 RESERVED6;
} USART_TYPEDEF;

// Status Register SR
#define USART_CTS                   BIT9
#define USART_LBD                   BIT8
#define USART_TXE                   BIT7
#define USART_TC                    BIT6
#define USART_RXNE                  BIT5
#define USART_IDLE                  BIT4
#define USART_ORE                   BIT3
#define USART_NF                    BIT2
#define USART_FE                    BIT1
#define USART_PE                    BIT0
// Baud Rate Register BRR
#define USART_MANTISSA_MASK         0xFFF0
#define USART_FRACTION_MASK         0x000F
// Control Register CR1
#define USART_OVER8                 BIT15 // default is 0 ( OVER16 )
#define USART_UE                    BIT13 // USART Enable
#define USART_M                     BIT12 // Data bits
#define USART_WAKE                  BIT11 
#define USART_PCE                   BIT10 // Parity Control Enable
#define USART_PS                    BIT9  // 0 = Even Parity, 1 = Odd Parity
#define USART_PEIE                  BIT8  // 1 = Parity Interrupt Enabled
#define USART_TXEIE                 BIT7  // 1 = Transmit Interrupt Enabled
#define USART_TCIE                  BIT6  // 1 = Transmit Complete Interrupt Enabled
#define USART_RXNEIE                BIT5  // 1 = RXNE Interrupt Enabled
#define USART_IDLEIE                BIT4  // 1 = IDLE Interrupt Enabled
#define USART_TE                    BIT3  // Transmitter Enable
#define USART_RE                    BIT2  // Receiver Enabled
#define USART_RWU                   BIT1  // Wake-up Mute
#define USART_SBK                   BIT0  // Send Break
// Control Register CR2
#define USART_LINEN                 BIT14 // 1 = Enable Line-End Synch Breaks
#define USART_STOP1                 BIT13 | BIT12 // ( Both bits must be cleared )
#define USART_STOPP5                BIT12
#define USART_STOP2                 BIT13
#define USART_STOP2P5               BIT13 | BIT12 // ( Both Bits must be set )
#define USART_CLKEN                 BIT11 // 1 = Enable SCLK for Synchronous mode
#define USART_CPOL                  BIT10 // 1 = Steady High Clock Polarity
#define USART_CPHA                  BIT9  
#define USART_LBCL                  BIT8
#define USART_LBDIE                 BIT6 // LINE Break Interrupt Enable
#define USART_LBDL                  BIT5
// CR BIT3-BIT0 are Multiprocessor Node Address
// Control Register CR3
#define USART_ONEBITE               BIT11 // 1 = One bit sample, 0 = three bit
#define USART_CTSIE                 BIT10 // 1 = CTS Interrupt Enabled
#define USART_CTSE                  BIT9  // 1 = CTS Enabled
#define USART_RTSE                  BIT8  // 1 = RTS Enabled
#define USART_DMAT                  BIT7  // 1 = DMA is Enabled for Transmission
#define USART_DMAR                  BIT6  // 1 = DMA is Enabled for Receive
#define USART_SCEN                  BIT5  // 1 = Smartcard Mode Enabled
#define USART_NACK                  BIT4  // 1 = SmartCard NACK Enabled
#define USART_HDSEL                 BIT3  // 1 = Half Duplex Mode
#define USART_IRLP                  BIT2  // 1 = IrDA Low Power Mode Enabled
#define USART_IREN                  BIT1  // 1 = IrDA Enabled
#define USART_EIE                   BIT0  // 1 = Error Interrupt Enabled


#define SPI2_BASE                   (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE                   (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE                 (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE                 (APB1PERIPH_BASE + 0x4800)
#define USART4_BASE                 (APB1PERIPH_BASE + 0x4C00)
#define USART5_BASE                 (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE                   (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE                   (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE                   (APB1PERIPH_BASE + 0x5C00)
#define CAN_BASE                    (APB1PERIPH_BASE + 0x6400)

#define SPI1_BASE                   (APB2PERIPH_BASE + 0x3000)
#define USART1_BASE                 (APB2PERIPH_BASE + 0x3800)

#define SDIO_BASE                   (AHBPERIPH_BASE + 0x8000)

#define SDIO                        ((SDIO_TYPEDEF *) SDIO_BASE)
#define SPI1                        ((SPI_TYPEDEF *) SPI1_BASE)
#define SPI2                        ((SPI_TYPEDEF *) SPI2_BASE)
#define SPI3                        ((SPI_TYPEDEF *) SPI3_BASE)
#define USART1                      ((USART_TYPEDEF *) USART1_BASE)
#define USART2                      ((USART_TYPEDEF *) USART2_BASE)
#define USART3                      ((USART_TYPEDEF *) USART3_BASE)
#define USART4                      ((USART_TYPEDEF *) USART4_BASE)
#define USART5                      ((USART_TYPEDEF *) USART5_BASE)
#define I2C1                        ((I2C_TYPEDEF *) I2C1_BASE)
#define I2C2                        ((I2C_TYPEDEF *) I2C2_BASE)
#define I2C3                        ((I2C_TYPEDEF *) I2C3_BASE)
#define CAN                         ((CAN_TYPEDEF *) CAN_BASE)

//----------------------------------End Hardware USARTS---------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//-----------------------------------Pygmy OS Comports----------------------------------------
#define DATA        1
#define COMMAND     0

typedef struct {
                //u8 Status;
                u16 RXBufferLen;
                u16 RXIndex;
                u16 RXLen;
                u16 TXBufferLen;
                u16 TXIndex;
                u16 TXLen;
                PYGMYCMDPTR Put;
                PYGMYVOIDPTR Get;
                u8 *RXBuffer;
                u8 *TXBuffer;
                } PYGMYFIFO;    

enum {
        STDIO,
        #ifdef __PYGMYSTREAMCOM1
            COM1,
        #endif
        #ifdef __PYGMYSTREAMCOM2
            COM2,
        #endif
        #ifdef __PYGMYSTREAMCOM3
            COM3,
        #endif
        #ifdef __PYGMYSTREAMCOM4
            COM4,
        #endif
        #ifdef __PYGMYSTREAMCOM5
            COM5,
        #endif
        #ifdef __PYGMYSTREAMCOM6
            COM6,
        #endif
        #ifdef __PYGMYSTREAMSP1
            SP1,
        #endif
        #ifdef __PYGMYSTREAMSP2
            SP2,
        #endif
        #ifdef __PYGMYSTREAMSP3
            SP3,
        #endif
        #ifdef __PYGMYSTREAMBUS1
            BUS1,
        #endif
        #ifdef __PYGMYSTREAMBUS2
            BUS2,
        #endif
        #ifdef __PYGMYSTREAMBUS3
            BUS3,
        #endif
        #ifdef __PYGMYSTREAMSDIO
            SDIO,
        #endif
        #ifdef __PYGMYSTREAMFILE
            FILE,
        #endif
        #ifdef __PYGMYSTREAMLCD
            LCD,
        #endif
        #ifdef __PYGMYSTREAMUSER1
            USER1,
        #endif
        #ifdef __PYGMYSTREAMUSER2
            USER2,
        #endif
        #ifdef __PYGMYSTREAMUSER3
            USER3
        #endif
        MAXCOMPORTS,
        };

enum {
        RS232,
        SPI,
        I2C,
        BITBANG,
        };
 

#define RTS     BIT0
#define CTS     BIT1
#define STOP1   BIT2
#define STOP15  BIT3
#define TXIE    BIT4
#define TXFIFO  TXIE
            
typedef struct{
                u8 SCL;
                u8 SDA;
                GPIO *PortSCL;
                GPIO *PortSDA;
                u16 PinSCL;
                u16 PinSDA;
                u16 Speed;
                //u8 Status;
                } PYGMYI2CPORT;
            
typedef struct{
                GPIO *PortCS;
                GPIO *PortSCK;
                GPIO *PortMOSI;
                GPIO *PortMISO;
                u16 PinCS;
                u16 PinSCK;
                u16 PinMOSI;
                u16 PinMISO;
                } PYGMYSPIPORT;
            
typedef struct{
                GPIO *PortCS;
                GPIO *PortA0;
                GPIO *PortWR;
                GPIO *PortRD;
                GPIO *PortDATA;
                u16 PinCS;
                u16 PinA0;
                u16 PinWR;
                u16 PinRD;
                u16 PinD0;
                u16 Mask;
                u8 Width;
                } PYGMYPARALLELPORT;

#ifdef __PYGMYSTREAMS
    extern PYGMYFIFO globalStreams[];
#endif
#ifndef __PYGMYCOMBUFFERLEN
    #define __PYGMYCOMBUFFERLEN 128 // Create default if none defined in profile
#endif
#ifndef __PYGMYCOM1BUFFERLEN
    #define __PYGMYCOM1BUFFERLEN __PYGMYCOMBUFFERLEN
#endif
#ifndef __PYGMYCOM2BUFFERLEN
    #define __PYGMYCOM2BUFFERLEN __PYGMYCOMBUFFERLEN
#endif
#ifndef __PYGMYCOM3BUFFERLEN
    #define __PYGMYCOM3BUFFERLEN __PYGMYCOMBUFFERLEN
#endif   
#ifndef __PYGMYCOM4BUFFERLEN
    #define __PYGMYCOM4BUFFERLEN __PYGMYCOMBUFFERLEN
#endif   
#ifndef __PYGMYCOM5BUFFERLEN
    #define __PYGMYCOM5BUFFERLEN __PYGMYCOMBUFFERLEN
#endif
#ifdef __PYGMYSTREAMCOM1
    extern u8 globalCOM1RXBuffer[];
#endif
#ifdef __PYGMYSTREAMCOM2
    extern u8 globalCOM2RXBuffer[];
#endif
#ifdef __PYGMYSTREAMCOM3
    extern u8 globalCOM3RXBuffer[];
#endif
#ifdef __PYGMYSTREAMCOM4
    extern u8 globalCOM4RXBuffer[];
#endif
#ifdef __PYGMYSTREAMCOM5
    extern u8 globalCOM5RXBuffer[];
#endif


//--------------------------------End Pygmy OS Comports----------------------------------------  
//---------------------------------------------------------------------------------------------

void comConfig( u8 ucPort, u8 ucProtocol, u8 ucOptions, u32 uiRate );
void comDisable( u8 ucPort );
void comEnable( u8 ucPort );
u16 generateBaud( u32 uiClock, u32 uiRate );
u8 putsUSART1( u8 *ucBuffer );
u8 putsUSART2( u8 *ucBuffer );
u8 putsUSART3( u8 *ucBuffer );
u8 putsUSART4( u8 *ucBuffer );
u8 putsUSART5( u8 *ucBuffer );
u8 putsUSART1FIFO( u8 *ucBuffer );
u8 putsUSART2FIFO( u8 *ucBuffer );
u8 putsUSART3FIFO( u8 *ucBuffer );
u8 putsUSART4FIFO( u8 *ucBuffer );
u8 putsUSART5FIFO( u8 *ucBuffer );
void resetRXBuffer( u8 ucStream );
         
void parallelConfig( PYGMYPARALLELPORT *pygmyPort, u8 ucWidth, u8 ucCS, u8 ucA0, u8 ucWR, u8 ucRD, u8 ucD0 );
void parallelWrite( PYGMYPARALLELPORT *pygmyPort, u8 ucA0, u16 uiWord );
u16 parallelRead( PYGMYPARALLELPORT *pygmyPort, u8 ucA0 );            
            
void spiConfig( PYGMYSPIPORT *pygmySPI, u8 ucCS, u8 ucSCK, u8 ucMISO, u8 ucMOSI );           
void spiWriteByte( PYGMYSPIPORT *pygmySPI, u8 ucByte );
void spiWriteWord( PYGMYSPIPORT *pygmySPI, u16 uiWord );
void spiWriteLong( PYGMYSPIPORT *pygmySPI, u32 ulLong );
u8 spiReadByte( PYGMYSPIPORT *pygmySPI );
            
void i2cConfig( PYGMYI2CPORT *pygmyI2C, u8 ucSCL, u8 ucSDA, u16 uiSpeed );
void i2cDelay( PYGMYI2CPORT *pygmyI2C );
void i2cStart( PYGMYI2CPORT *pygmyI2C );
void i2cStop( PYGMYI2CPORT *pygmyI2C );
void i2cWriteBit( PYGMYI2CPORT *pygmyI2C, u8 ucBit );
u8 i2cReadBit( PYGMYI2CPORT *pygmyI2C );
u8 i2cWriteBuffer( PYGMYI2CPORT *pygmyI2C, u8 ucAddress, u8 *ucBuffer, u16 uiLen );
u8 i2cWriteByte( PYGMYI2CPORT *pygmyI2C, u8 ucByte );
u8 i2cReadByte( PYGMYI2CPORT *pygmyI2C );
    
void streamInit( void );
u8 streamReset( u8 ucStream );
void streamResetRX( u8 ucStream );
void streamResetTX( u8 ucStream );
void streamTXChar( u8 ucStream, void *pygmyUSART );
u8 streamGetChar( u8 ucStream );
u8 streamPutChar( u8 ucStream, u8 ucChar );
u8 streamPopChar( u8 ucStream );
u8 streamPeekChar( u8 ucStream );
void streamPushChar( u8 ucStream, u8 ucChar );
u8 streamSetPut( u8 ucStream, void *ptrFunc );
u8 streamSetGet( u8 ucStream, void *ptrFunc );
u8 streamSetRXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen );
u8 streamSetTXBuffer( u8 ucStream, u8 *ucBuffer, u16 uiLen );
