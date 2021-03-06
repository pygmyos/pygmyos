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
#ifndef __PYGMY_HEADER_FPEC
	#define __PYGMY_HEADER_FPEC

#include "pygmy_sys.h"

typedef struct{
    volatile u32 ACR;
    volatile u32 KEYR;
    volatile u32 OPTKEYR;
    volatile u32 SR;
    volatile u32 CR;
    volatile u32 AR;
    volatile u32 RESERVED;
    volatile u32 OBR;
    volatile u32 WRPR;
    volatile u32 RESERVED2[ 8 ]; // XL
    volatile u32 KEYR2; // XL
    volatile u32 RESERVED3; // XL
    volatile u32 SR2; // XL
    volatile u32 CR2; // XL
    volatile u32 AR2; // XL
} FPEC_TYPEDEF;    

typedef struct{
    volatile u16 RDP;
    volatile u16 USER;
    volatile u16 Data0;
    volatile u16 Data1;   
    volatile u16 WRP0;
    volatile u16 WRP1;
    volatile u16 WRP2;
    volatile u16 WRP3;
} OB_TYPEDEF; // Option Regs

typedef struct{
    volatile u16 Pages;
} SIZEREG_TYPEDEF;

#define SIZEREG_BASE                ((u32)0x1FFFF7E0) // Number of 1KB Flash Pages in device
#define FPEC_BASE                   ((u32)0x40022000) // Flash Registers Starting Address
#define OB_BASE                     ((u32)0x1FFFF800) // Flash Option Registers Starting Address
#define FPEC                        ((FPEC_TYPEDEF *) FPEC_BASE)
//#define OB                          ((OB_TypeDef *) OB_BASE) 
#define SIZEREG                     ((SIZEREG_TYPEDEF *) SIZEREG_BASE )

#define FPEC_BASEADDRESS            0x08000000

#define FPEC_RDPRT                  0x00A5
#define FPEC_KEY1                   0x45670123
#define FPEC_KEY2                   0xCDEF89AB
#define FPEC_ACR_HLFCYA             BIT3

#define FPEC_SR_EOP                 BIT5
#define FPEC_SR_WRPRTERR            BIT4
#define FPEC_SR_PGERR               BIT2
#define FPEC_SR_BSY                 BIT0

#define FPEC_CR_EOPIE               BIT12
#define FPEC_CR_ERRIE               BIT10
#define FPEC_CR_OPTWRE              BIT9
#define FPEC_CR_LOCK                BIT7
#define FPEC_CR_STRT                BIT6
#define FPEC_CR_OPTER               BIT5
#define FPEC_CR_OPTPG               BIT4
#define FPEC_CR_MER                 BIT2
#define FPEC_CR_PER                 BIT1
#define FPEC_CR_PG                  BIT0

#define FPEC_OBR_RST_STDBY          BIT4
#define FPEC_OBR_RST_STOP           BIT3
#define FPEC_OBR_WDG_SW             BIT2
#define FPEC_OBR_RDPRT              BIT1
#define FPEC_OBR_OPTERR             BIT0

#define FPEC_ACR_PRFTBS             BIT5
#define FPEC_ACR_PRFTBE             BIT4
#define FPEC_ACR_HLFCYA             BIT3
#define FPEC_ACR_LATENCY0           0
#define FPEC_ACR_LATENCY1           BIT0
#define FPEC_ACR_LATENCY2           BIT1

#define FPEC_MAXBANK1               0x0807FFFF
#define FPEC_DENSITY_XL             BIT0
#define FPEC_DENSITY_ODDPAGE        BIT1

#define IHEX_DATA                   0x00
#define IHEX_EOF                    0x01
#define IHEX_EXTENDEDSEGMENTADDR    0x02
#define IHEX_STARTSEGMENTADDR       0x03
#define IHEX_EXTENDEDLINEARADDR     0x04
#define IHEX_STARTLINEARADDR        0x05
#define IHEX_BYTECOUNT_OFFSET       1 // 2 char field
#define IHEX_ADDR_OFFSET            3 // 4 char field
#define IHEX_TYPE_OFFSET            8 // 2 char field from 7-8, 7 always 0
#define IHEX_DATA_OFFSET            9 // ( BYTECOUNT * 2 ) long field 
//#define IHEX_CHECKSUM_OFFSET     

//---------------------------------------------------------------------------------------------
//-------------------------------Pygmy Descriptor Defines--------------------------------------
#define DESC_STM32F100LD                  0x0420
#define DESC_STM32F100MD                  0x0420
#define DESC_STM32F100HD                  0x0428
#define DESC_STM32F103LD                  0x0412
#define DESC_STM32F103MD                  0x0410
#define DESC_STM32F103HD                  0x0414
#define DESC_STM32F103XLD                 0x0430
#define DESC_STM32F105                    0x0418
#define DESC_STM32F107                    0x0418
#define DESC_STM32L151                    0x0416
#define DESC_STM32L152                    0x0416

#define PYGMY_ID_STM32F100LD              0x0420
#define PYGMY_ID_STM32F100MD              0x0420
#define PYGMY_ID_STM32F100HD              0x0428
#define PYGMY_ID_STM32F103LD              0x0412
#define PYGMY_ID_STM32F103MD              0x0410
#define PYGMY_ID_STM32F103HD              0x0414
#define PYGMY_ID_STM32F103XLD             0x0430
#define PYGMY_ID_STM32F105                0x0418
#define PYGMY_ID_STM32F107                0x0418
#define PYGMY_ID_STM32L151                0x0416
#define PYGMY_ID_STM32L152                0x0416

u8 fpecLock( void );
u8 fpecUnlock( u8 Bank );
u8 fpecProcessIHEX( u8 *Buffer );
u8 fpecWriteLong( u16 *Address, u32 Data );
u8 fpecWriteWord( u16 *Address, u16 Data );
u8 fpecWriteByte( u32 Address, u8 Data );
u8 fpecEraseProgramMemory( void );
u8 fpecErasePage( u32 Address );
u8 fpecEraseRange( u32 AddressStart, u32 AddressEnd );
u8 fpecWriteDescriptor( u16 Descriptor, u32 Value );
u32 fpecGetIHEXLength( PYGMYFILE *File );
u32 fpecGetID( void );
u32 fpecFlashSectorSize( void );
u32 fpecFlashSize( void );
u32 fpecRAMSize( void );
u32 fpecMCUID( void );

#endif // __PYGMY_HEADER_FPEC
