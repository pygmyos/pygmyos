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
#ifndef __PYGMY_HEADER_FILE
	#define __PYGMY_HEADER_FILE

#include "pygmy_profile.h"

// ------------------ Defines for SST25VF080B SPI Flash and compatible ---------------------

#define PYGMY_FLASH_READ                          0x03
#define PYGMY_FLASH_FASTREAD                      0x0B
#define PYGMY_FLASH_ERASE                         0x20
#define PYGMY_FLASH_ERASE_4KB                     0x20
#define PYGMY_FLASH_ERASE_32KB                    0x52
#define PYGMY_FLASH_ERASE_64KB                    0xD8
#define PYGMY_FLASH_ERASE_ALL                     0x60
#define PYGMY_FLASH_WRITE_BYTE                    0x02
//#define PYGMY_FLASH_WRITE_WORD                    0xAD
#define PYGMY_FLASH_RDSR                          0x05
#define PYGMY_FLASH_EWSR                          0x50
#define PYGMY_FLASH_WRSR                          0x01
#define PYGMY_FLASH_WREN                          0x06
#define PYGMY_FLASH_WRDI                          0x04
#define PYGMY_FLASH_RDID                          0x90
#define PYGMY_FLASH_JEDECID                       0x9F
#define PYGMY_FLASH_EBSY                          0x70
#define PYGMY_FLASH_DBSY                          0x80
#define PYGMY_FLASH_STATUS_BUSY                   BIT0
#define PYGMY_FLASH_STATUS_WEL                    BIT1
#define PYGMY_FLASH_STATUS_BP0                    BIT2
#define PYGMY_FLASH_STATUS_BP1                    BIT3
#define PYGMY_FLASH_STATUS_BP2                    BIT4
#define PYGMY_FLASH_STATUS_BP3                    BIT5
#define PYGMY_FLASH_STATUS_AAI                    BIT6
#define PYGMY_FLASH_STATUS_BPL                    BIT7
#define PYGMY_FLASH_MAXSECTORS                    256  // Sectors in 1MB Flash IC
#define PYGMY_FLASH_SECTORSIZE                    4096 // Bytes per sector

#define PYGMY_FILE_MAXFILENAMELEN                12 // 25 - NULL sring terminator
#define PYGMY_FILE_MAXVOLUMENAMELEN              12 // 19 - NULL string terminator      

#define PYGMY_FILE_VOLUME_FIELD_NAME              0   // Name 13 Byte field including NULL terminator
#define PYGMY_FILE_VOLUME_FIELD_ATTRIB            15  // Attrib 1 Byte field
#define PYGMY_FILE_VOLUME_FIELD_SECTORS           16  // Sectors 2 Byte field
#define PYGMY_FILE_VOLUME_FIELD_SECTORSIZE        18  // SectorSize 2 Byte field
#define PYGMY_FILE_VOLUME_FIELD_MAXFILES          20  // MaxFiles 4 byte field
#define PYGMY_FILE_VOLUME_FIELD_MEDIASIZE         24  // MediaLen 4 byte field ( total file memory on chip )  
#define PYGMY_FILE_VOLUME_FIELD_ENTRIESPERFAT     28  // Number of Alloc / ID pairs per FAT sector, 2 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FILESSECTORS      30  // Number of Sectors for FILES table, 2 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FATSECTORS        32  // Number of Sectors for FAT Table, 2 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FIRSTFILESECTOR   34  // Address of First Sector of Storage Memory, 4 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FILES_A           38  // Address of First Sector of FILES_A Table, 4 Bytes ( always 0 )
#define PYGMY_FILE_VOLUME_FIELD_FILES_B           42  // Address of First Sector of FILES_B Table, 4 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FAT_A             46  // Address of First Sector of FAT_A Table, 4 Bytes
#define PYGMY_FILE_VOLUME_FIELD_FAT_B             50  // Address of First Sector of FAT_B Table, 4 Bytes
#define PYGMY_FILE_VOLUME_FIELD_RESERVED          54  // 10 Reserved Bytes  

#define READ                    BIT0
#define WRITE                   BIT1
#define APPEND                  BIT2
#define MEDIARAM                BIT2 // Volume Level, covers all memory that allows random read/write access
#define MEDIAFLASH              BIT3 // Volume Level, covers flash memory ( write cycle limitations )
#define HIDDEN                  BIT3 
#define FOLDER                  BIT4
#define SYSTEM                  BIT5
#define BURIED                  BIT6 // Any folder or file that is non-root
#define EOF                     BIT7
#define VOLUMEFULL              BIT7
#define VOLUMEERROR             BIT6

#define PYGMY_FILE_ERROR_SUCCESS                  BIT0
#define PYGMY_FILE_ERROR_MEMFULL                  BIT2

#define PYGMY_FILE_FILETABLESECTORS                 1
#define PYGMY_FILE_FATSECTORS                       1
#define PYGMY_FILE_SECTORSIZE                       4096
#define PYGMY_FILE_TOTALSECTORS                     256
#define PYGMY_FILE_ALLOCATEDSECTORS                 252
#define PYGMY_FILE_MAXFILES                         252
#define PYGMY_FILE_FILES_A                          0x00000000
#define PYGMY_FILE_FILES_B                          0x00001000
#define PYGMY_FILE_FAT_A                            0x00002000
#define PYGMY_FILE_FAT_B                            0x00003000
#define PYGMY_FILE_DATA                             0x00004000



#define SST_ID                      0xBF
#define SST_ID_MEMTYPE              0x25 // FLASH
#define SST_ID_32M                  0x4A // SST25VF032B
#define SST_ID_16M                  0x41 // SST25VF016B
#define SST_ID_8M                   0x8E // SST25VF080B
#define SST_ID_4M                   0x8D // SST25VF040B

#ifndef __PYGMY_BOOT
    //#define FLASH_CS_LOW    pygmyFlashSPI.PortCS->BRR = pygmyFlashSPI.PinCS
    //#define FLASH_CS_HIGH   pygmyFlashSPI.PortCS->BSRR = pygmyFlashSPI.PinCS
#endif


typedef struct PYGMYFILEVOLUME_TYPEDEF {
                u8 Name[ PYGMY_FILE_MAXVOLUMENAMELEN + 1 ];
                u8 Attributes;
                u32 ActiveFiles;
                u32 ActiveFAT;
                u16 Sectors; // was u32
                u16 SectorSize;
                u32 MaxFiles;
                u32 MediaSize; // Sectors * SectorSize
                u16 EntriesPerFAT;
                u16 FilesSectors;
                u16 FATSectors;
                u32 FirstFileSector;
                //u32 Files_A; // Files_A has to be 0
                u32 Files_B;
                u32 FAT_A;
                u32 FAT_B;
                } PYGMYFILEVOLUME;

typedef struct PYGMYFILE_TYPEDEF { 
                u32 Sector;
                u32 Length;
                u32 Index;
                u16 ID_Sector;
                u16 ID_File;
                u8 Attributes;
                u8 *Resource;
                u8 Name[ PYGMY_FILE_MAXFILENAMELEN + 1 ];
                } PYGMYFILE;

// ------------------ End Defines for SST25VF080B SPI Flash and compatible -----------------

// ----------------------------- Platform specific defines ---------------------------------

// ----------------------------- End Platform specific defines -----------------------------

extern PYGMYFILEVOLUME pygmyRootVolume;     
//extern PYGMYSPIPORT pygmyFlashSPI;
       
u16 fileAllocateContiguousFATEntries( u16 uiID, u16 uiEntries );
u32 filePreAllocate( PYGMYFILE *pygmyFile, u32 ulSize );
void fileCopyHandle( PYGMYFILE *pygmyFrom, PYGMYFILE *pygmyTo );  
u8 fileOpenResource( PYGMYFILE *pygmyFile, u8 *ucResource );            
u8 fileOpen( PYGMYFILE *pygmyFile, u8 *ucName, u8 ucAttrib );
u8 fileSetPosition( PYGMYFILE *pygmyFile, u8 ucOrigin, s32 lIndex );
u32 fileGetPosition( PYGMYFILE *pygmyFile );
u8 fileClose( PYGMYFILE *pygmyFile );
u8 fileDelete( u8 *ucName );
u8 fileRename( u8 *ucCurrentName, u8 *ucName );
u8 filePutChar( PYGMYFILE *pygmyFile, u8 ucChar );
void filePutWord( PYGMYFILE *pygmyFile, u16 uiData, u8 ucEndian );
void filePutLong( PYGMYFILE *pygmyFile, u32 ulData, u8 ucEndian );
u8 filePutString( PYGMYFILE *pygmyFile, u8 *ucString );
u8 filePutBuffer( PYGMYFILE *pygmyFile, u16 uiLen, u8 *ucBuffer );  
u16 fileGetBuffer( PYGMYFILE *pygmyFile, u16 uiLen, u8 *ucBuffer );
u16 fileGetWord( PYGMYFILE *pygmyFile, u8 ucEndian );
u32 fileGetLong( PYGMYFILE *pygmyFile, u8 ucEndian );
u8 fileGetChar( PYGMYFILE *pygmyFile );
u8 fileGetCharBuffered( PYGMYFILE *pygmyStartFile );
u32 fileGetActiveFiles( void );
u16 fileGetName( u16 uiFileEntry, u8 *ucName );
u32 fileGetLength( u32 ulID_File );
u16 fileGetMaxFiles( void );
u32 fileGetSector( u32 ulSector );
u16 fileGetFreeFATEntry( void );
u16 fileGetFreeFileEntry( void );
u16 fileGetFAT( u16 uiID, u16 uiIndex );
u32 fileGetFreeSpace( void );
u8 *fileGetVolumeName( u8 *ucName );
u32 fileGetActiveFAT( void );
u8 fileMountRoot( void );
u8 fileMountVolume( void );//PYGMYFILEVOLUME *pygmyFileVolume );
u32 fileFormat( u8 *ucName );
u8 fileIsValidName( u8 *ucName );
u8 fileIsEOF( PYGMYFILE *pygmyFile );
u8 fileIsRootFull( void );
u8 fileWriteEntry( PYGMYFILE *pygmyFile );            
u8 fileWriteLength( PYGMYFILE *pygmyFile, u32 ulLength );
u8 fileDeleteFATEntry( u16 uiID_File, u16 uiID_Sector );
u16 fileAllocateFAT( u16 uiID_File, u16 uiID_Sector );
u16 fileSeekName( u8 *ucName );

void filePrintDebug( u8 ucStream );
void filePrintList( u8 ucStream );

u8 flashReadByte( u32 ulAddress );
u16 flashReadWord( u32 ulAddress );
u32 flashReadLong( u32 ulAddress );
void flashReadBuffer( u32 ulAddress, u8 *ucBuffer, u16 uiLen );
void flashWriteWord( u32 ulAddress, u16 uiWord );
void flashWriteLong( u32 ulAddress, u32 ulLong );
void flashWriteBuffer( u32 ulAddress, u16 uiLen , u8 *ucBuffer );
void flashWriteString( u32 ulAddress, u8 ucTerminate, u8 *ucBuffer );
void flashWriteByte( u32 ulAddress, u8 ucByte );            
void flashWriteEnable( void );
u8 flashReadStatus( void );
u8 flashReadID( void );
void flashWaitForBusy( void );
void flashSectorErase( u32 ulAddress );
u8 flashChipErase( void );
/*
#ifdef __PYGMY_BOOT
void spiWriteByte( void *, u8 ucByte );
void spiWriteWord( void *, u16 uiWord );
void spiWriteLong( void *, u32 ulLong );
u8 spiReadByte( void * );
#endif
*/

#endif // __PYGMY_HEADER_FILE
