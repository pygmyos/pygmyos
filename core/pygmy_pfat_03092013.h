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
#ifndef __PYGMY_HEADER_PFAT
	#define __PYGMY_HEADER_PFAT
    #define __PYGMY_HEADER_FILE

#define PYGMYFILE_INVALIDADDRESS        0xFFFFFFFFFFFFFFFF
#define PYGMYFILE_INVALIDBASEADDRESS    0x0000FFFFFFFFFFFF
#define PYGMYFILE_ADDRESSLEN            9
#define READ                            BIT0
#define WRITE                           BIT1
#define APPEND                          BIT2
#define HIDDEN                          BIT3 
#define FOLDER                          BIT4
#define SYSTEM                          BIT5
#define ROOT                            BIT6 // Any folder or file that is non-root
#define EOF                             BIT7

#define PYGMYFILE_CURRENT               0
#define PYGMYFILE_PARENT                1

#define PYGMYFILE_ACTIVESECTOR          0xAAAA
#define PYGMYFILE_VOLUMESECTOR          0xBBBB
#define PYGMYFILE_UNUSEDSECTOR          0xFFFF
#define PYGMYFILE_ERASEDSECTOR          0x0000
#define PYGMYFILE_BADSECTOR             0x1111

#define PYGMYFILE_CHUNKLEN              32
#define PYGMYFILE_CHUNKPAYLEN           27
#define PYGMYFILE_ID_ERASED             0x00000000
#define PYGMYFILE_ID_UNUSED             0xFFFFFFFF

#define PYGMYFILE_LENGTHCHUNK           254
#define PYGMYFILE_NAMECHUNK             0
#define PYGMYFILE_PROPERTIESCHUNK       0
#define PYGMYFILE_HASHCHUNK             233
#define PYGMYFILE_SGCHUNK               33
#define PYGMYFILE_LASTSGCHUNK           232
#define PYGMYFILE_SGCHUNKS              PYGMYFILE_LASTSGCHUNK - PYGMYFILE_SGCHUNK
#define PYGMYFILE_OWNERCHUNK            253
#define PYGMYFILE_PERMISSIONSCHUNK      253
#define PYGMYFILE_METACHUNK             251
#define PYGMYFILE_JOURNALCHUNK          252

#define PYGMYFILE_SECTORMARKERLEN       2
#define PYGMYFILE_IDLEN                 4
#define PYGMYFILE_MAXFOLDERDEPTH        16 // The larger the folder count, the higher the RAM usage

#define PYGMYFILE_ENABLED               BIT8
#define PYGMYFILE_FULL                  BIT9
#define PYGMYFILE_LOCKED                BIT10
#define PYGMYFILE_BUSY                  BIT11

//#define START                           1
//#define CURRENT                         2
//#define END                             4

// Address is 48 grouped with a leading 1B sector count
// Each address group is 7B, first byte denoting number of
//  contiguous sectors included in group, the next 6B contain
//  the start address
typedef struct {
                u64 BaseAddress;
                u16 SectorCount;
                } PYGMYFILEADDRESS;

typedef struct {
                PYGMYFILEADDRESS *Sectors; // Each address group contains a group sector count
                u16 SectorGroups; // pairs of base address-sector count pairs
                u32 SectorCount; // Total Sectors Allocated for file
                u32 ID;
                u32 Time;
                u32 Length;
                u8 *Path;
                u8 *Name;
                u8 Attributes;
                //u8 Access;
                } PYGMYFILEPROPERTIES;

typedef struct { 
                PYGMYFILEADDRESS *Sectors;
                u32 SectorCount;
                u8 SectorGroups;
                } PYGMYFILESECTORGROUPS;                
typedef struct PYGMYFOLDER_TYPEDEF PYGMYFOLDER;
typedef struct {
                PYGMYMEMIO *IO;
                PYGMYMEMDESC Desc;
                PYGMYFILEPROPERTIES Properties;
                PYGMYFOLDER *ParentFolder;
                u32 FreeSectors;
                u32 RootFolder;
                u32 Status;
                void *Port;
                } PYGMYFILEVOLUME;

struct PYGMYFOLDER_TYPEDEF {
                PYGMYFILEVOLUME *Volume;
                PYGMYFILEPROPERTIES Properties;
                struct PYGMYFOLDER_TYPEDEF *Parent; // Parent Folder, this allows a Volume->...->Folder->File relationship
                u8 IsRoot;
                };


typedef struct { 
                PYGMYFILEVOLUME *Volume;
                PYGMYFILEPROPERTIES Properties;
                PYGMYFOLDER *ParentFolder;
                u32 Index;
                u8 *Resource;
                } PYGMYFILE;

typedef struct {
                PYGMYFILEVOLUME *Volume;
                //PYGMYFILEPROPERTIES ParentProperties; // For Parent Folder
                PYGMYFOLDER *ParentFolder;
                PYGMYFILEPROPERTIES Properties; // For currently listed file
                u32 Index;
                } PYGMYFILELIST;

//extern PYGMYFILEVOLUME **globalVolumes;

// Function declarations
// Free and allocate
//u8 fileFreeFolder( PYGMYFOLDER *Folder );
u8 fileFreeFolder( PYGMYFOLDER *Folder );
u8 fileFreeFile( PYGMYFILE *File );
u8 fileFreeVolume( PYGMYFILEVOLUME *Volume );

// Print
void filePrintDescriptor( PYGMYMEMDESC *Descriptor );
void filePrintVolume( PYGMYFILEVOLUME *Volume );
void filePrintFolder( PYGMYFOLDER *Folder );
void filePrintProperties( PYGMYFILEPROPERTIES *Properties );

// Address
u8 fileMergeSectorGroups( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *PropertiesFrom, PYGMYFILEPROPERTIES *PropertiesTo );
u16 fileGenerateDirectAddress( PYGMYFILEVOLUME *Volume, PYGMYFILEPROPERTIES *Properties, u32 Index, u64 *Address );
void fileConvertBufferToAddress( PYGMYFILEADDRESS *Address, u8 *Buffer );
void fileConvertAddressToBuffer( PYGMYFILEADDRESS *Address, u8 *Buffer );
u16 fileGenerateAddress( PYGMYFILE *File, u64 *Address );

// Chunk and Entry
void fileCopyProperties( PYGMYFILEPROPERTIES *EntryFrom, PYGMYFILEPROPERTIES *EntryTo );
u8 fileRecycleChunks( PYGMYFOLDER *ParentFolder );
u32 fileGetFreeChunks( PYGMYFOLDER *ParentFolder, u32 *ErasedChunks );
u8 fileDeleteChunk( PYGMYFILEVOLUME *Volume, u64 Address );
u8 fileAllocateChunks( u32 ID, PYGMYFOLDER *ParentFolder, u8 Chunks, u64 *Addresses );
u32 fileFindContiguousChunks( PYGMYFOLDER *ParentFolder, u8 Chunks );
u8 fileWriteEntry( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileDeleteEntry( u32 ID, PYGMYFOLDER *ParentFolder );
u8 fileWriteProperties( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties );
u8 fileDeleteProperties( u32 ID, PYGMYFOLDER *ParentFolder );
u8 fileWriteLength( PYGMYFOLDER *ParentFolder, u64 Address, u8 SGChunks, u32 Length );
u8 fileDeleteLength( u32 ID, PYGMYFOLDER *ParentFolder );
u8 fileUpdateLength( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileGetSectorGroupCount( u32 ID, PYGMYFOLDER *ParentFolder );
u8 fileWriteSectorGroups( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties );
u8 fileUpdateSectorGroups( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileDeleteSectorGroups( u32 ID, PYGMYFOLDER *ParentFolder );
u8 fileLoadEntry( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileLoadLength( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileFindChunkAddress( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u64 *Address );
u8 fileFindChunk( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u64 *Address, u8 *Buffer );
u8 fileLoadProperties( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u8 fileLoadSectorGroups( u32 ID, u8 SGChunks, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties );
u32 fileFindNextID( PYGMYFOLDER *ParentFolder, u32 Index, PYGMYFILEPROPERTIES *Properties );
//void filePrintChunk( u8 *Buffer );

// Sector
u64 fileFindContiguousEmptySectors( PYGMYFILEVOLUME *Volume, u32 Count );
u8 fileAllocateSectors( PYGMYFOLDER *ParentFolder, u32 RequestedSectors, PYGMYFILEPROPERTIES *Properties );
u8 fileAppendSectors( PYGMYFOLDER *ParentFolder, u32 RequestedSectors, PYGMYFILEPROPERTIES *Properties );

// Path
u8 fileTestPath( u8 *Path );
u8 fileChangeCurrentPath( u8 *Buffer );
u8 fileSetCurrentPath( u8 *Buffer );
u8 *fileGetCurrentPath( void );
u8 fileSeekPath( u8 *ucPath, PYGMYFILEVOLUME **pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry );
PYGMYFOLDER *fileAllocateFoldersFromPath( PYGMYFILEVOLUME *Volume, u8 *Path );
PYGMYFOLDER *fileAllocateFoldersFromFullPath( u8 *Path );
u8 fileSetPath( PYGMYFILE *File, u8 *Path );
u8 *fileGetPathFromPath( u8 *Path );
u8 *fileGetNameFromPath( u8 *Path );
u8 *fileAddStringToArray( u16 ArrayLen, u8 *StringArray[], u8 *Str );

// Volume
u8 fileFreeVolume( PYGMYFILEVOLUME *Volume );
u8 fileDeleteVolume( PYGMYMEMIO *MemIO, u8 *Name, ... );
u64 fileSeekVolumeAddress( PYGMYMEMIO *MemIO, u8 *Name, ... );
PYGMYFILEVOLUME *fileLoadVolume( PYGMYMEMIO *MemIO, u8 *Name, ... );
PYGMYFILEVOLUME *fileIsMounted( u8 *Name );
u8 fileAddVolume( PYGMYFILEVOLUME *Volume );
PYGMYFILEVOLUME *fileNewVolume( PYGMYMEMIO *MemIO, u8 *Name, ... );
PYGMYFILEVOLUME *fileSeekVolumes( PYGMYMEMIO *MemIO, ... );
PYGMYFILEVOLUME *fileGetVolumeFromFullPath( u8 *Path );
u8 fileSetCurrentVolume( PYGMYFILEVOLUME *Volume );
u8 fileMount( PYGMYMEMIO *MemIO, u8 Address, u8 *Name, ... );
u8 fileUnmount( u8 *Name );
u8 fileDeleteVolume( PYGMYMEMIO *MemIO, u8 *Name, ... );
u8 fileFormat( PYGMYMEMIO *MemIO, u8 Force, ...  );
u64 fileFindNextEmptySector( PYGMYFILEVOLUME *pygmyVolume, u64 ullAfter );
u32 fileGetFreeSpace( PYGMYFILEVOLUME *Volume );
u8 fileGetVolumeCount( void );
PYGMYFILEVOLUME *fileFindVolume( u8 *VolumeName );
PYGMYFILEVOLUME *fileGetCurrentVolume( void );
PYGMYFILEVOLUME *fileGetVolume( u8 Index );
PYGMYFILEVOLUME *fileAllocateVolume( PYGMYMEMIO *MemIO, u8 Address, u8 *Name, ... );

// File operations
u8 fileDeleteAdvanced( PYGMYFOLDER *ParentFolder );
u8 fileDelete( u8 *Name  );
PYGMYFILE *fileOpen( u8 *Name, u8 Attrib, u32 FileLen );
u8 fileClose( PYGMYFILE *File );
u8 fileList( PYGMYFILELIST *FileList );

u8 fileFlush( PYGMYFILE *File );
u8 fileRename( u8 *Name, u8 *NewName );
u8 fileCopy( u8 *From, u8 *To );
u8 fileGetChar( PYGMYFILE *File );
u8 fileGetBuffer( PYGMYFILE *File, u8 *Buffer, u32 Len );
u8 *fileGetString( PYGMYFILE *File );
u8 fileReadBytes( PYGMYFILE *File, u8 *Buffer, u32 Len );
u8 fileWriteBytes( PYGMYFILE *File, u8 *Buffer, u32 Len );
u8 filePutChar( PYGMYFILE *File, u8 Data );
u16 fileGetWord( PYGMYFILE *File, u8 Endian );
u8 filePutWord( PYGMYFILE *File, u16 Data, u8 Endian );
u32 fileGetLong( PYGMYFILE *File, u8 Endian );
u8 filePutLong( PYGMYFILE *File, u32 Data, u8 Endian );
u8 filePutBuffer( PYGMYFILE *File, u8 *Buffer, u32 Len );
u8 fileSeek( PYGMYFILE *File, u8 Origin, s32 Index );

#endif
