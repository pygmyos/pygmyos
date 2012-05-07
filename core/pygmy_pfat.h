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
#define INVALIDADDRESS          0xFFFFFFFFFFFFFFFF
#define READ                    BIT0
#define WRITE                   BIT1
#define APPEND                  BIT2
//#define MEDIARAM                BIT2 // Volume Level, covers all memory that allows random read/write access
//#define MEDIAFLASH              BIT3 // Volume Level, covers flash memory ( write cycle limitations )
#define HIDDEN                  BIT3 
#define FOLDER                  BIT4
#define SYSTEM                  BIT5
#define ROOT                    BIT6 // Any folder or file that is non-root
#define EOF                     BIT7
//#define VOLUMEFULL              BIT7
//#define VOLUMEERROR             BIT6
// Address is 48 grouped with a leading 1B sector count
// Each address group is 7B, first byte denoting number of
//  contiguous sectors included in group, the next 6B contain
//  the start address
typedef struct {
                u64 BaseAddress;
                u8 SectorCount;
                } PYGMYFILEADDRESS;

typedef struct {
                //u32 *Sectors;
                //u32 SectorCount;
                PYGMYFILEADDRESS *Sectors; // Each address group contains a group sector count
                u32 SectorGroups; // pairs of base address-sector count pairs
                u32 SectorCount; // Total Sectors Allocated for file
                u32 ID;
                u32 Time;
                u32 Length;
                u8 *Path;
                u8 *Name;
                u8 Attributes;
                } PYGMYFILEPROPERTIES;
                

typedef struct {
                PYGMYMEMIO *IO;
                PYGMYMEMDESC Desc;
                PYGMYFILEPROPERTIES Properties;
                u32 FreeSectors;
                u32 RootFolder;
                //u32 *Sectors;
                //u32 SectorCount;
                //u8 *Path;
                //u8 *Name;
                //u32 FileTable1;
                //u32 FileTable2;
                //u32 FAT1;
                //u32 FAT2;
                void*Port;
                } PYGMYFILEVOLUME;
            
typedef struct PYGMYFILE_TYPEDEF { 
                PYGMYFILEVOLUME *MountPoint;
                PYGMYFILEPROPERTIES Properties;
                //u8 *Name;
                //u8 *Path;
                //u8 *Buffer;
                //u32 *Sectors;
                //u32 SectorCount;
                //u32 Length;
                u32 Index;
                //u16 ID_Sector;
                //u16 ID_File;
                //u8 Attributes;
                u8 *Resource;
                
                //u8 Name[ PYGMY_FILE_MAXFILENAMELEN + 1 ];
                } PYGMYFILE;
     
extern PYGMYFILEVOLUME *globalMountPoints;

u16 fileGenerateAddress( PYGMYFILE *pygmyFile, u64 *ullAddress );
void fileLoadAddress( PYGMYFILEADDRESS *pygmyAddress, u8 *ucBuffer );
void fileConvertAddressToBuffer( PYGMYFILEADDRESS *pygmyAddress, u8 *ucBuffer );
//u8 fileWriteChunk( PYGMYFILEVOLUME *pygmyVolume, u32 *ulAddress, u32 ulLen, u8 *ucBuffer );
u8 fileWriteChunk( PYGMYFILE *pygmyFile, u8 *ucChunk );
//u64 fileSeekChunk( PYGMYFILEVOLUME *pygmyVolume, u32 ulID, u32 *ulAddress, u32 ulLen );
u8 fileSeekChunk( PYGMYFILE *pygmyFile, u64 *ullAddress );
//u16 fileGetEntry( PYGMYFILEVOLUME *pygmyVolume, u32 ulID, u32 *ulAddress, u32 ulLen, u8 **ucBuffer );
u16 fileGetEntry( PYGMYFILE *pygmyFile, u8 **ucBuffer );
u8 fileWriteEntry( PYGMYFILE *pygmyParent, PYGMYFILEPROPERTIES *pygmyEntry, u8 ucAttrib, u8 *ucName );
            
u8 fileGetMountPointCount( void );
PYGMYFILEVOLUME *fileGetMountPoint( u8 ucIndex );
PYGMYFILEVOLUME *fileGetCurrentMountPoint( void );
PYGMYFILEVOLUME *fileSeekPath( u8 *ucPath, PYGMYFILEPROPERTIES *pygmyEntry );
u8 fileLoadEntry( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry );
void filePrintProperties( PYGMYFILEPROPERTIES *pygmyEntry );
//u32 fileReadLength( PYGMYFILEVOLUME *pygmyVolume, u32 ulID, u32 *ulSectors, u32 ulSectorCount );
u32 fileReadLength( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry );
//u8 fileAllocateSectors( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEADDRESS *pygmyAddress, u32 ulCount, u32 ulStartSector );

u8 fileReadBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen );
u8 fileWriteBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen );

void filePrintAddress( PYGMYFILEADDRESS *pygmyAddress );
u8 fileChangeCurrentPath( u8 *ucBuffer );
u8 fileSetCurrentPath( u8 *ucBuffer );
u8 *fileGetCurrentPath( void );
u8 fileSetCurrentMountPoint( PYGMYFILEVOLUME *pygmyVolume );
u8 fileList( u8 *ucPath, PYGMYFILEPROPERTIES *pygmyEntry );            
//u8 fileWriteEntry( PYGMYFILEVOLUME *pygmyVolume, u32 *ulAddress, u32 ulLen, u32 *ulSectors, u32 ulSectorLen, u8 ucAttrib, u8 *ucName  );
u32 fileGetSectorPointers( PYGMYFILEVOLUME *pygmyVolume, u32 ulID, u32 *ulAddress, u32 ulLen, u32 **ulSectors );
//u8 fileAllocateSectors( PYGMYFILEVOLUME *pygmyVolume, u32 *ulSectors, u32 ulCount, u32 ulStartSector );
u8 fileAllocateSectors( PYGMYFILE *pygmyFile, u32 ulRequestedSectors );
u64 fileFindNextEmptySector( PYGMYFILEVOLUME *pygmyVolume, u64 ullAfter );
u64 fileFindContiguousEmptySectors( PYGMYFILEVOLUME *pygmyVolume, u32 ulCount );
 void fileCopyProperties( PYGMYFILEPROPERTIES *fromEntry, PYGMYFILEPROPERTIES *toEntry );
            
u8 fileMount( PYGMYMEMIO *pygmyMEMIO, u8 ulAddress, u8 *ucName, ... );
u8 fileUnmount( PYGMYFILEVOLUME *pygmyVolume );
u8 fileFormat( PYGMYFILEVOLUME *pygmyVolume, u8 *ucName );
PYGMYFILE *fileOpen( u8 *ucName, u8 ucAttrib, u32 ulFileLen );
u8 fileSetPath( PYGMYFILE *pygmyFile, u8 *ucPath );
u32 fileGetFreeSpace( PYGMYFILEVOLUME *pygmyVolume );
u8 fileClose( PYGMYFILE *pygmyFile );
u8 fileFlush( PYGMYFILE *pygmyFile );
u8 fileRename( u8 *ucName, u8 *ucNewName );
u8 fileDelete( u8 *ucName ); //PYGMYFILE *pygmyFile  );
u8 fileCopy( u8 *ucFrom, u8 *ucTo );
u8 fileGetChar( PYGMYFILE *pygmyFile );
u8 filePutChar( PYGMYFILE *pygmyFile, u8 ucData );
u16 fileGetWord( PYGMYFILE *pygmyFile, u8 ucEndian );
u8 filePutWord( PYGMYFILE *pygmyFile, u16 uiData, u8 ucEndian );
u32 fileGetLong( PYGMYFILE *pygmyFile, u8 ucEndian );
u8 filePutLong( PYGMYFILE *pygmyFile, u32 ulData, u8 ucEndian );
u8 fileGetBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen );
u8 filePutBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen );
            
