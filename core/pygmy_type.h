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
#include <stdarg.h>

typedef struct {
                u8  (*GetChar)      ( void *, u64 );
                u8  (*PutChar)      ( void *, u64, u8 );
                u16 (*GetWord)      ( void *, u64 );
                u8  (*PutWord)      ( void *, u64, u16 );
                u32 (*GetLong)      ( void *, u64 );
                u8  (*PutLong)      ( void *, u64, u32 );
                u8  (*GetBuffer)    ( void *, u64, u8*, u16 );
                u8  (*PutBuffer)    ( void *, u64, u8*, u16 );
                u8  (*Open)         ( void **, u64, ... );
                u8  (*VOpen)        ( void **, u64, va_list );
                u8  (*Close)        ( void ** );
                u8  (*Desc)         ( void *, void * );
                u8  (*Status)       ( void * );
                u8  (*EraseAll)     ( void * );
                u8  (*EraseSector)  ( void *, u64 );
                } PYGMYMEMIO;

typedef struct {
                u32 Capacity;       // Sectors x Sector Size
                u16 Sectors;        // Total number of sectors as defined below
                u16 SectorSize;     // Smallest erasable size ( generally 256B-4KB )
                u8 Attributes;      // Pygmy File Attributes
                u8 Manufacturer;    // 1 byte JDEC ID
                u8 Type;            // JDEC Memory Type
                } PYGMYMEMDESC;     // Generic Memory Descriptor

#define START                       1 // BIT0
#define CURRENT                     2 // BIT1
#define END                         4 // BIT2
            
#define READ                        BIT0
#define WRITE                       BIT1
#define APPEND                      BIT2
#define MEDIARAM                    BIT2 // Volume Level, covers all memory that allows random read/write access
#define MEDIAFLASH                  BIT3 // Volume Level, covers flash memory ( write cycle limitations )
#define HIDDEN                      BIT3 
#define FOLDER                      BIT4
#define SYSTEM                      BIT5
#define BURIED                      BIT6 // Any folder or file that is non-root
#define EOF                         BIT7
#define VOLUMEFULL                  BIT7
#define VOLUMEERROR                 BIT6
            
/*typedef struct {
                PYGMYFILE File;
                u32 Status;
                u32 Timeout;
                u32 Transaction;
                u32 Count;
                u8 Buffer[ 132 ];
                u8 Index;
                u8 Port;
                } PYGMYXMODEM;*/
            