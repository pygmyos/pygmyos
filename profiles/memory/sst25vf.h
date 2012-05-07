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

#pragma once

#include "pygmy_profile.h"
#include "pygmy_type.h"
#include <stdarg.h>

// ------------------ Defines for SST25VF080B SPI Flash and compatible ---------------------

#define SST25VF_READ                    0x03
#define SST25VF_FASTREAD                0x0B
#define SST25VF_ERASE                   0x20
#define SST25VF_ERASE_4KB               0x20
#define SST25VF_ERASE_32KB              0x52
#define SST25VF_ERASE_64KB              0xD8
#define SST25VF_ERASE_ALL               0x60
#define SST25VF_WRITE_BYTE              0x02
//#define SST25VF_WRITE_WORD            0xAD
#define SST25VF_RDSR                    0x05
#define SST25VF_EWSR                    0x50
#define SST25VF_WRSR                    0x01
#define SST25VF_WREN                    0x06
#define SST25VF_WRDI                    0x04
#define SST25VF_RDID                    0x90
#define SST25VF_JEDECID                 0x9F
#define SST25VF_EBSY                    0x70
#define SST25VF_DBSY                    0x80
#define SST25VF_STATUS_BUSY             0x01 // BIT0
#define SST25VF_STATUS_WEL              0x02 // BIT1
#define SST25VF_STATUS_BP0              0x04 // BIT2
#define SST25VF_STATUS_BP1              0x08 // BIT3
#define SST25VF_STATUS_BP2              0x10 // BIT4
#define SST25VF_STATUS_BP3              0x20 // BIT5
#define SST25VF_STATUS_AAI              0x40 // BIT6
#define SST25VF_STATUS_BPL              0x80 // BIT7
#define SST25VF_MAXSECTORS              256  // Sectors in 1MB Flash IC
#define SST25VF_SECTORSIZE              4096 // Bytes per sector

#define SST25VF_ID                      0xBF
#define SST25VF_ID_MEMTYPE              0x25 // FLASH
#define SST25VF_ID_32M                  0x4A // SST25VF032B
#define SST25VF_ID_16M                  0x41 // SST25VF016B
#define SST25VF_ID_8M                   0x8E // SST25VF080B
#define SST25VF_ID_4M                   0x8D // SST25VF040B

extern const PYGMYMEMIO SST25VF;

u8 sst25vfGetChar( PYGMYSPIPORT *pygmySPI, u64 ulAddress );
u8 sst25vfPutChar( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 ucByte );
u16 sst25vfGetWord( PYGMYSPIPORT *pygmySPI, u64 ulAddress );
u8 sst25vfPutWord( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u16 uiWord );
u32 sst25vfGetLong( PYGMYSPIPORT *pygmySPI, u64 ulAddress );
u8 sst25vfPutLong( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u32 ulLong );
u8 sst25vfGetBuffer( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u8 sst25vfPutBuffer( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 *ucBuffer, u16 uiLen );
u8 sst25vfEraseAll( PYGMYSPIPORT *pygmySPI );
u8 sst25vfEraseSector( PYGMYSPIPORT *pygmySPI, u64 ulSector );
u8 sst25vfOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, ... );
u8 sst25vfVOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, va_list vaList );
u8 sst25vfClose( PYGMYSPIPORT **pygmySPI );
u8 sst25vfDesc( PYGMYSPIPORT *pygmySPI, PYGMYMEMDESC *pygmyDesc );
u32 sst25vfID( PYGMYSPIPORT *pygmySPI );
u32 sst25vfStatus( PYGMYSPIPORT *pygmySPI );
void sst25vfWriteEnable( PYGMYSPIPORT *pygmySPI );
void sst25vfWaitForBusy( PYGMYSPIPORT *pygmySPI );
