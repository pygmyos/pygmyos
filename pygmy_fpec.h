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

#pragma once

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

u8 fpecLock( void );
u8 fpecUnlock( void );
u8 fpecProcessIHEX( u8 *ucBuffer );
u8 fpecWriteLong( u16 *uiAddress, u32 ulData );
u8 fpecWriteWord( u16 *uiAddress, u16 uiData );
u8 fpecEraseProgramMemory( u16 uiStart, u16 uiEnd );
u8 fpecErasePage( u32 ulAddress );
