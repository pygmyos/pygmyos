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
/*
The first sector contains a pointer to the ROOT folder in the first 4 bytes
The format of the first sector is as follows:
[ ROOT 4B ][ SECTORS 4B ][ MAXFILES 4B ]
The remainder of the sector may contain optional markers for bad sectors
Files and folders are stored identically
Memory in the FileTables is stored in 32 byte chunks
If a filename exceeds 22 bytes, additional chunks will store the name
Each chunk will start with the file's ID
Chunks need not be contiguous, but must be ordered largest to smallest
Deleted Chunks should be marked with an ID of 0x00000000
Unused Chunks should be marked with an ID 0xFFFFFFFF
Valid IDs are defined as follows:
    ID must not equal 0x00000000 or 0xFFFFFFFF
    ID is only guaranteed to be unique within the current folder
    ID will be generated using PDIA on the filename and entire path ( if any )
[ FILE 32B ] = [ ID 4B ][ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME LenB ]
Parent is the address of the parent folder
    For a folder, this is the folder in which it resides
    For a file, it is the current folder
The Len chunk always begins with 0x00000000 immediately following ID, this is an invalid
    value for any other section or chunk
ID Format: [ PDIA of NAME 3B][ CHUNK 1B ]
Chunk Format:
    Sectors 0-200
    SectorGroups 201-250
    SumSector: 253
    LEN 254
    Reserved: 250-252,255
Example:
FILE =  [ 32B ][ ID ][ TIME ][ ATTRIB ][ 23 ][ thisisaverylongtestnam ]
        [ 32B ][ ID ][ e ][ Pad 3B ][ S1 ][ S2 ][ S3 ][ S4 ][ S5 ][ S6 ]
        [ 32B ][ ID ][ S7 ][ ... ] ... Next is LenChunk ...
        [ 32B ][ ID ][ 0x00000000 ][ L1 ][ L2 ][ L3 ][ L4 ][ L5 ][ L6 ]
        [ 32B ][ ID ][ 0xFFFFFFFF ][ C1 ][ C2 ][ C3 ][ C4 ][ C5 ][ C6 ]

The first 4 bytes of each Sector contain the SectorIndex ( order within file )
    The SectorIndex Tag may not be 0x00000000 or 0xFFFFFFFF
    0x00000000 is Reserved for Bad Sector
    0xFFFFFFFF is Reserved for Unused, marking with SectorIndex allocates the sector
    There is not value for deleted, when a sector becomes unused it should be immediately
    deleted and marked as 0xFFFFFFFF
*/

#include "pygmy_profile.h"
#include "pygmy_type.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

PYGMYFILE *globalFiles;
//PYGMYFOLDER *globalFolders;
PYGMYFILEVOLUME *globalVolumeCurrent = NULL, **globalVolumes;
//u8 globalPathCount = 0, globalNameCount = 0; 
//u8 **globalPaths, **globalNames;
u8 globalVolumeCount = 0, globalFileCount = 0;//, globalFolderCount = 0;

u8 globalPFATVersionTag[] = "PFAT V0.1   ";

PYGMYFILEVOLUME *fileGetVolumeFromFullPath( u8 *Path )
{
    PYGMYFILEVOLUME *Volume;
    u32 i, ID;
    u8 MaxVolumes, *VolumePath, *VolumeName;

    VolumePath = malloc( len( Path ) );
    if( !VolumePath ){
        return( FALSE );
    } // if
    strcpy( VolumePath, Path );
    VolumeName = strtok( VolumePath, "/" );
    MaxVolumes = fileGetVolumeCount( );
    ID = pdiaEncodeString( VolumeName );
    for( i = 0; i < MaxVolumes; i++ ){
        Volume = fileGetVolume( i );
        if( Volume->Properties.ID == ID ){
            
            return( Volume );
        } // if
    } // for
    free( VolumePath );

    return( NULL );
}

u8 fileChangeCurrentPath( u8 *Buffer )
{
    // Case1: Path is root
    // Case2: Path is .., roll back on folder
    // Case3: Path is absolute, seek path
    // Case4: Path is relative, add path to current path and seek path
    PYGMYFILEVOLUME *Volume;
    PYGMYFILEPROPERTIES Properties;
    PYGMYFOLDER *Folder;
    u32 i, PathLen, MemLen;
    u8 *Path, PathBool, ParamCount, *Params[ 32 ];

    PathLen = len( Buffer );
    print( COM3, "\rChanging directory" );
    if( !strcmp( Buffer, "/" ) ){
        // Case 1: Path is root
        fileSetCurrentPath( NULL );
        fileSetCurrentVolume( NULL );
        return( TRUE );
    } else if( !strcmp( Buffer, ".." ) ){
        // Case2: Path is special case "..", roll back one folder
        print( COM3, "\rCase 2" );
        Volume = fileGetCurrentVolume( );
        if( Volume ){
            if( len( Volume->Properties.Path ) > 1 + ( len( Volume->Properties.Name ) ) ){
                // Do not modify path if already at root
                // otherwise, clip off last path segment and test for root condition
                replaceLastChar( Volume->Properties.Path, '/', '\0' );
                if( len( Volume->Properties.Path ) < 1 + ( len( Volume->Properties.Name ) ) ){
                    fileSetCurrentVolume( NULL );
                } // if
            } else{
                // We are at volume segment in the path, going down one level further reaches root
                if( len( Volume->Properties.Path ) == 1 + ( len( Volume->Properties.Name ) ) ){
                    fileSetCurrentVolume( NULL );
                } // if
            } // else
        } // if

        return( TRUE );
    } else if( *Buffer == '/' ){
        // Case3: Path is absolute, seek path
        // First seek path to validate path exists and to retrieve correct mount point
        print( COM3, "\rCase 3" );
        PathBool = fileTestPath( Buffer );
        if( !PathBool ){
            return( FALSE );
        } // if
        print( COM3, "\rfileTestPath(): TRUE" );
        fileSetCurrentVolume( fileGetVolumeFromFullPath( Buffer ) );
        fileSetCurrentPath( Buffer );
        return( TRUE );
    } else{
        // Case4: Path is relative, add path to current path and seek path
        print( COM3, "\rCase 4\rCalling fileGetCurrentVolume()" );
        Volume = fileGetCurrentVolume();
        if( Volume ){
            print( COM3, " Found Volume" );
            // We are currently somewhere in a path
            MemLen = 2 + len( Buffer ) + len( Volume->Properties.Path );
            Path = malloc( MemLen );
            if( Path ){
                strcpy( Path, Volume->Properties.Path );
                strcat( Path, "/" );
                strcat( Path, Buffer );
            } // if
        } else{
            // We are currently at root
            print( COM3, " At Root" );
            MemLen = 2 + len( Buffer );
            Path = malloc( MemLen );
            if( Path ){
                strcpy( Path, "/" );
                strcat( Path, Buffer );
            } // if
        } // else
        if( Path ){
            print( COM3, "\rHave Path: %s", Path );
            Folder = fileAllocateFoldersFromFullPath( Path );
            if( !Folder ){
                return( FALSE );
            } // if

    
            //if( fileTestPath( Path ) ){
                print( COM3, "\rPath tested good" );
                fileSetCurrentVolume( fileGetVolumeFromFullPath( Path ) );
                print( COM3, "\rSet current volume" );
                fileSetCurrentPath( Path );
                print( COM3, "\rSet current path" );
            //} // if
            print( COM3, "\rCalling fileFreeFolder()..." );
            fileFreeFolder( Folder );
            print( COM3, "\rDone." );
        } // if
        return( TRUE );
    } // else

    return( FALSE );
}

u8 fileSetCurrentPath( u8 *Buffer )
{
    PYGMYFILEVOLUME *Volume;
    u32 MemLen;

    Volume = fileGetCurrentVolume();
    if( !Volume ){
        return( FALSE );
    } // if
    if( Volume->Properties.Path ){
        free( Volume->Properties.Path );
    } // if
    if( !Buffer ){
        // Default mode used for reseting the path to "/volumename"
        MemLen = 2 + len ( Volume->Properties.Name );
        Volume->Properties.Path = malloc( MemLen );
        memset( Volume->Properties.Path, 0, MemLen );
        if( !Volume->Properties.Path || !Volume->Properties.Name ){
            return( FALSE );
        } // is
        copyString( "/", Volume->Properties.Path );
        appendString( Volume->Properties.Name, Volume->Properties.Path );
    } else{
        MemLen = 1 + len ( Buffer );
        Volume->Properties.Path = malloc( MemLen );
        memset( Volume->Properties.Path, 0, MemLen );
        if( !Volume->Properties.Path ){
            return( FALSE );
        } // is
        copyString( Buffer, Volume->Properties.Path );
    } // else

    return( TRUE );
}

u8 *fileGetCurrentPath( void )
{
    // This function returns the current path used by the command line and relative file access
    
    if( globalVolumeCurrent && globalVolumeCurrent->Properties.Path ){
            return( globalVolumeCurrent->Properties.Path );
    } // if
    
    return( "/" );
}

u8 fileSetCurrentVolume( PYGMYFILEVOLUME *Volume )
{
    globalVolumeCurrent = Volume;

    return( TRUE );
}

PYGMYFILEVOLUME *fileIsMounted( u8 *Name )
{
    // This function returns a pointer to the volume that matches name, or NULL if not found
    u32 i;

    for( i = 0; i < globalVolumeCount; i++ ){
        if( !strcmp( globalVolumes[ i ]->Properties.Name, Name ) ){
            
            return( globalVolumes[ i ] );
        } // if
    } // for
    
    return( NULL );
}

u8 fileAddVolume( PYGMYFILEVOLUME *Volume )
{
    // This function adds an existing volume to the global array of volumes
    // If the volume exists, it's updated, otherwise, it will be appended to the current array
    // If no array exists, an array will be allocated and the volume passed will become the first entry
    PYGMYFILEVOLUME **TmpVolumes;
    u32 i;

    // First verify that there is an allocated array to add to
    if( globalVolumeCount == 0 || !globalVolumes ){
        globalVolumes = malloc( sizeof( PYGMYFILEVOLUME*) ); // There was no array, allocate a new one
        if( !globalVolumes ){
            return( FALSE ); // RAM full
        } // if
        globalVolumes[ 0 ] = Volume; // We know that the volume passed is the only volume, assign and return
        globalVolumeCount = 1;

        return( TRUE );
    } // if

    // Append the volume to the end of the array
    // First verify that the volume isn't already part of the array
    for( i = 0; i < globalVolumeCount; i++ ){
        if( !strcmp( globalVolumes[ i ]->Properties.Name, Volume->Properties.Name ) ){
            // The volume is already mounted but may have been modified, if the pointer has changed, update, otherwise they are identical
            if( Volume != globalVolumes[ i ] ){
                // ToDo: Add code to update affected volumess in file handles
                free( globalVolumes[ i ] ); // Free the old pointer
                globalVolumes[ i ] = Volume;
            } // if

            return( TRUE ); // Volume has been updated
        } // if
    } // for
    TmpVolumes = realloc( globalVolumes, ( globalVolumeCount + 1 ) * sizeof( PYGMYFILEVOLUME* ) );
    if( !TmpVolumes ){
        return( FALSE ); // RAM full
    } // if
    globalVolumes = TmpVolumes;
    globalVolumes[ globalVolumeCount++ ] = Volume;

    return( TRUE );
}

PYGMYFILEVOLUME *fileNewVolume( PYGMYMEMIO *MemIO, u8 *Name, ... )
{
    // This function creates a new volume on the memory interface passed
    // If there are no free sectors, the function will return NULL, otherwise, it will return a pointer to the new volume
    PYGMYFILEVOLUME *Volume, *TmpVolumes;
    va_list vaList;
    u64 TmpAddress;
    u32 i;
    u8 *NameString;

    // Name must be allocated as a dynamic copy
    NameString = malloc( 1 + len( Name ) );
    if( !NameString ){
        return( NULL ); // RAM is full
    } // if
    strcpy( NameString, Name ); 
    Volume = malloc( sizeof( PYGMYFILEVOLUME ) );
    if( !Volume ){
        return( NULL ); // RAM is full
    } // if
    Volume->ParentFolder = malloc( sizeof( PYGMYFOLDER ) );
    if( !Volume->ParentFolder ){
        return( NULL ); // RAM is full
    } // if
    Volume->ParentFolder->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !Volume->ParentFolder->Properties.Sectors ){
        return( NULL ); // RAM is full
    } // if
    Volume->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !Volume->Properties.Sectors ){
        return( NULL ); // RAM is full
    } // if
    
    // Open memory interface
    va_start( vaList, Name );
    MemIO->VOpen( &Volume->Port, 0, vaList );
    va_end( vaList );
    Volume->IO = MemIO;
    
    MemIO->Desc( Volume->Port, &Volume->Desc ); // Request the interface descriptor

    // Make a ParentFolder framework that allows writing to the first sector
    Volume->Desc.ChunksPerSector = ( Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    Volume->ParentFolder->Volume = Volume;
    //Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress = fileFindContiguousEmptySectors( Volume, 1 );
    Volume->ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    Volume->ParentFolder->Properties.SectorCount = 1;
    Volume->ParentFolder->Properties.SectorGroups = 1;
    Volume->ParentFolder->Properties.Path = NULL; 
    Volume->ParentFolder->Properties.Name = NULL; 
    Volume->ParentFolder->Properties.Attributes = (READ|WRITE|FOLDER);
    Volume->ParentFolder->Properties.Length = 0;
    Volume->ParentFolder->IsRoot = TRUE; // All 
    // Find a free sector to use to create the volume entry
    Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress = fileFindContiguousEmptySectors( Volume, 1 ); // A Volume entry only uses 1 sector, so we seek for 1 empty sector
    if( Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress == PYGMYFILE_INVALIDADDRESS ){
        // This condition occurs if there are no sectors free to allocate
        return( NULL );
    } // if
    // Write the volume information to the sector
    // Volume sectors are unique and cannot be allocated by the file allocation functions
    // Allocate sector 0 manually
    Volume->IO->PutWord( Volume->Port, Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress, PYGMYFILE_VOLUMESECTOR ); // Write the tag to mark the sector as a volume
    Volume->IO->PutBuffer( Volume->Port, Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress + 2, globalPFATVersionTag, 12 ); // Write the filesystem version
    Volume->Properties.ID = pdiaEncodeString( NameString );
    Volume->Properties.Name = NameString;
    Volume->Properties.Length = 0;
    Volume->Properties.Path = NULL;
    Volume->Properties.SectorCount = 1;
    Volume->Properties.SectorGroups = 1;
    Volume->Status = 0;
    if( !fileAllocateSectors( Volume->ParentFolder, 1, &Volume->Properties ) ){
        return( NULL ); // Allocation Failed 
    } // if
    fileWriteEntry( Volume->ParentFolder, &Volume->Properties );
    if( !fileAddVolume( Volume ) ){
        // ToDo: Add clean up code to handle the RAM full error
        return( FALSE );
    } // if

    return( Volume );
}

PYGMYFILEVOLUME *fileSeekVolumes( PYGMYMEMIO *MemIO, ... )
{
    // This function starts at the base address of an IO device and scans to the end looking for Volumes
    // Each volume will start with a 32bit ID PYGMYFILE_VOLUMESECTOR
    // Each volume entry will consist of 1 sector, with an ID entry ( always first ) and multiple option entries
    // A separate Volume will be allocated and added to the global volume table as they are found
    PYGMYFILEVOLUME RootVolume, *Volume, *Volumes;
    PYGMYFILEADDRESS Sectors;
    PYGMYFILEPROPERTIES Properties;
    PYGMYFOLDER ParentFolder;
    u64 Address;
    u32 Index, i, ii;
    u16 SectorMarker;
    u64 BusAddress = 0;
    va_list vaList;
    
    Volumes = malloc( sizeof( PYGMYFILEVOLUME *) );
    if( !Volumes ){
        // RAM is full
        return( NULL );
    } // if
    va_start( vaList, MemIO );
    MemIO->VOpen( &RootVolume.Port, BusAddress, vaList );
    va_end( vaList );
    RootVolume.IO = MemIO;
    RootVolume.ParentFolder = &ParentFolder;
    RootVolume.ParentFolder->Properties.Sectors = &Sectors;
    MemIO->Desc( RootVolume.Port, &RootVolume.Desc );
    RootVolume.Desc.ChunksPerSector = ( Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    RootVolume.ParentFolder->Volume = &RootVolume;

    RootVolume.Desc.ChunksPerSector = ( RootVolume.Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    RootVolume.ParentFolder->Properties.Sectors[ 0 ].BaseAddress = 0;
    RootVolume.ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    RootVolume.ParentFolder->Properties.SectorCount = 1;
    RootVolume.ParentFolder->Properties.SectorGroups = 1;
    RootVolume.ParentFolder->Properties.Path = NULL; 
    RootVolume.ParentFolder->Properties.Name = NULL; 
    RootVolume.ParentFolder->Properties.Attributes = (READ|WRITE|FOLDER);
    RootVolume.ParentFolder->Properties.Length = 0;
    RootVolume.ParentFolder->IsRoot = TRUE;

    for( i = 0; i < RootVolume.Desc.Sectors; i++ ){
        SectorMarker = RootVolume.IO->GetWord( RootVolume.Port, i * RootVolume.Desc.SectorSize );
        if( SectorMarker == PYGMYFILE_VOLUMESECTOR ){
            // The ID is unknown at this point, we can only look for what we know it's not
            // The ID will not be PYGMYFILE_ID_ERASED or PYGMYFILE_ID_UNUSED
            // The Entry index will always be 0
            RootVolume.ParentFolder->Properties.Sectors[ 0 ].BaseAddress = Address;
            // Now fill out the properties structure with the data we wish to write to the first sector
            for( ii = 0, Index = PYGMYFILE_CHUNKLEN; ii < RootVolume.Desc.ChunksPerSector - 1; ii++ ){
                Index = fileFindNextID( RootVolume.ParentFolder, Index, &Properties );
                if( !Index ){
                    break;
                } else{
                    break;
                } // else
            } // for
            
        } // if
    } // for

    return( NULL );
}

u8 fileFormat( PYGMYMEMIO *MemIO, u8 Force, ...  )
{
    // This function formats a device, not a volume, volumes are not partitions
    // After formatting, a volume will be created with Name
    PYGMYFILEVOLUME *Volume;
    u64 Address;
    u32 i;
    u16 SectorMarker;
    va_list vaList;
    
    Volume = malloc( sizeof( PYGMYFILEVOLUME ) );
    if( !Volume ){
        return( 0 ); // RAM is full
    } // if
  
    va_start( vaList, Force );
    MemIO->VOpen( &Volume->Port, 0, vaList );
    va_end( vaList );
    Volume->IO = MemIO;
    MemIO->Desc( Volume->Port, &Volume->Desc );
    // Only erase sectors that haven't been marked as bad
    
    if( Force ){
        // Use force to mass erase the media, this is useful if the media was previously formatted with another format
        Volume->IO->EraseAll( Volume->Port );
    } else{
        for( i = 0, Address = 0; i < Volume->Desc.Sectors; i++) {
            PYGMY_WATCHDOG_REFRESH;
            SectorMarker = Volume->IO->GetWord( Volume->Port, Address );
            if( SectorMarker != PYGMYFILE_BADSECTOR ){
                // Don't erase a sector that has been marked bad
                Volume->IO->EraseSector( Volume->Port, Address );
            } // if
            Address += Volume->Desc.SectorSize;
        } // for
    } // if
    free( Volume->Port );
    free( Volume );

    return( TRUE );
}

PYGMYFILEVOLUME *fileLoadVolume( PYGMYMEMIO *MemIO, u8 *Name, ... )
{
    // This function starts at the base address of an IO device and scans to the end looking for Volumes
    // Each volume will start with a 32bit ID PYGMYFILE_VOLUMESECTOR
    // Each volume entry will consist of 1 sector, with an ID entry ( always first ) and multiple option entries
    PYGMYFILEVOLUME *Volume;
    PYGMYFOLDER ParentFolder;
    u64 Address, TmpAddress, BusAddress = 0;
    u32 Index, i, ii;
    u16 SectorMarker;
    u8 VolumeIDBuffer[13];
    va_list vaList;
    
    Volume = malloc( sizeof( PYGMYFILEVOLUME ) );
    if( !Volume ){
        return( NULL ); // RAM is full
    } // if
    Volume->ParentFolder = malloc( sizeof( PYGMYFOLDER ) );
    if( !Volume->ParentFolder ){
        return( NULL ); // RAM is full
    } // if
    Volume->ParentFolder->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !Volume->ParentFolder->Properties.Sectors ){
        return( NULL ); // RAM is full
    } // if
    va_start( vaList, Name );
    MemIO->VOpen( &Volume->Port, BusAddress, vaList );
    va_end( vaList );
    Volume->IO = MemIO;
    MemIO->Desc( Volume->Port, &Volume->Desc );
    Volume->Desc.ChunksPerSector = ( Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    Volume->ParentFolder->Volume = Volume;

    Volume->Desc.ChunksPerSector = ( Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress = 0;
    Volume->ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    Volume->ParentFolder->Properties.SectorCount = 1;
    Volume->ParentFolder->Properties.SectorGroups = 1;
    Volume->ParentFolder->Properties.Path = NULL; 
    Volume->ParentFolder->Properties.Name = NULL; 
    Volume->ParentFolder->Properties.Attributes = (READ|WRITE|FOLDER);
    Volume->ParentFolder->Properties.Length = 0;
    Volume->ParentFolder->IsRoot = TRUE;
    Volume->ParentFolder->Parent = NULL;
    
    if( !Name ){
        // Mount memory interface in debug mode if Name is NULL
        // debug mode mounts the volume as DEBUG with the base address
        Volume->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
        if( !Volume->Properties.Sectors ){
            fileFreeVolume( Volume );
            return( NULL ); // Insufficient RAM
        } // if
        Volume->Properties.Name = malloc( 1 + len( "DEBUG" ) );
        strcpy( Volume->Properties.Name, "DEBUG" );
        if( !Volume->Properties.Name ){
            fileFreeVolume( Volume );
            return( NULL ); // Insufficient RAM
        } // if
        Volume->Properties.Sectors[ 0 ].BaseAddress = 0;
        Volume->Properties.Sectors[ 0 ].SectorCount = 1;
        Volume->Properties.SectorCount = 1;
        Volume->Properties.SectorGroups = 1;
        Volume->Properties.Path = NULL; 
        Volume->Properties.Attributes = (READ|WRITE|FOLDER);
        Volume->Properties.ID = pdiaEncodeString( Volume->Properties.Name ); // Generate a valid ID for later operations
        Volume->Properties.Length = 0;
        Volume->Status = 0;
        fileAddVolume( Volume );
        if( globalVolumeCount == 1 ){
              fileSetCurrentPath( NULL ); // Set path to root level and return
        } // if
        
        return( Volume );
    } // if

    for( i = 0; i < Volume->Desc.Sectors; i++ ){
        SectorMarker = Volume->IO->GetWord( Volume->Port, i * Volume->Desc.SectorSize );
        if( SectorMarker == PYGMYFILE_VOLUMESECTOR ){
            // The ID is unknown at this point, we can only look for what we know it's not
            // The ID will not be PYGMYFILE_ID_ERASED or PYGMYFILE_ID_UNUSED
            // The Entry index will always be 0
            Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress = Address;
            // Now fill out the properties structure with the data we wish to write to the first sector
            for( ii = 0, Index = PYGMYFILE_CHUNKLEN; ii < Volume->Desc.ChunksPerSector - 1; ii++ ){
                Index = fileFindNextID( Volume->ParentFolder, Index, &Volume->Properties );
                if( !Index ){
                    break;
                } else {
                    if( !strcmp( Volume->Properties.Name, Name ) ){
                        Volume->IO->GetBuffer( Volume->Port, Address + PYGMYFILE_SECTORMARKERLEN, VolumeIDBuffer, 12 );
                        VolumeIDBuffer[ 12 ] = '\0';
                
                        // Next test the filesystem is valid
                        if( !strcmp( globalPFATVersionTag, VolumeIDBuffer ) ){
                            // We have a match, set the Properties, Add the Volume, and return
                            // Set status to clear
                            Volume->Status = 0;
                            Volume->Properties.Length = 0;
                            Volume->Properties.Path = NULL;
                            Volume->Properties.ID = pdiaEncodeString( Name ); // Generate a valid ID for later operations
                            fileAddVolume( Volume );
                            if( globalVolumeCount == 1 ){
                                fileSetCurrentPath( NULL ); // Set path to root level and return
                            } // if

                            return( Volume );
                        } // if
                    } // if
                    break;
                } // else
                // SectorGroups are allocated each time fileFindNextID is called
                // We must free the allocated SectorGroups and Name if the volume was not the one we were looking for
                if( Volume->Properties.Sectors ){
                    free( Volume->Properties.Sectors );
                } // if
                if( Volume->Properties.Name ){
                    free( Volume->Properties.Name );
                } // if
            } // for
            
        } // if
    } // for
    // There was no volume to mount, free allocated RAM
    //fileFreeVolume( Volume );

    return( NULL );
}

u8 fileDeleteVolume( PYGMYMEMIO *MemIO, u8 *Name, ... )
{ 
    u64 Address;
    void *Port;
    va_list vaList;

    va_start( vaList, Name );
    Address = fileSeekVolumeAddress( MemIO, Name, vaList );
    va_end( vaList );

    if( Address == PYGMYFILE_INVALIDADDRESS ){
        return( FALSE );
    } // if
    va_start( vaList, Name );
    MemIO->VOpen( Port, 0, vaList );
    va_end( vaList );
    MemIO->PutWord( Port, Address, PYGMYFILE_ERASEDSECTOR );

    return( TRUE );
}

u64 fileSeekVolumeAddress( PYGMYMEMIO *MemIO, u8 *Name, ... )
{
    PYGMYFILEVOLUME Volume;
    PYGMYFILEADDRESS ParentSectors, Sectors;
    PYGMYFILEPROPERTIES Properties;
    PYGMYFOLDER ParentFolder;
    u64 Address, TmpAddress, BusAddress = 0;
    u32 Index, i, ii;
    u16 SectorMarker;
    u8 *NameString;
    va_list vaList;
    
    // Name must be allocated as a dynamic copy
    Volume.ParentFolder = &ParentFolder;
    Volume.ParentFolder->Properties.Sectors = &ParentSectors;
    Volume.Properties.Sectors = &Sectors;

    va_start( vaList, Name );
    MemIO->VOpen( &Volume.Port, BusAddress, vaList );
    va_end( vaList );
    Volume.IO = MemIO;
    Volume.ParentFolder = &ParentFolder;
    Volume.ParentFolder->Properties.Sectors = &Sectors;
    MemIO->Desc( Volume.Port, &Volume.Desc );
    Volume.Desc.ChunksPerSector = ( Volume.Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    Volume.ParentFolder->Volume = &Volume;

    Volume.Desc.ChunksPerSector = ( Volume.Desc.SectorSize / PYGMYFILE_CHUNKLEN );
    Volume.ParentFolder->Properties.Sectors[ 0 ].BaseAddress = 0;
    Volume.ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    Volume.ParentFolder->Properties.SectorCount = 1;
    Volume.ParentFolder->Properties.SectorGroups = 1;
    Volume.ParentFolder->Properties.Path = NULL; 
    Volume.ParentFolder->Properties.Name = NULL; 
    Volume.ParentFolder->Properties.Attributes = (READ|WRITE|FOLDER);
    Volume.ParentFolder->Properties.Length = 0;
    Volume.ParentFolder->IsRoot = TRUE;

    for( i = 0; i < Volume.Desc.Sectors; i++ ){
        SectorMarker = Volume.IO->GetWord( Volume.Port, i * Volume.Desc.SectorSize );
        if( SectorMarker == PYGMYFILE_VOLUMESECTOR ){
            // The ID is unknown at this point, we can only look for what we know it's not
            // The ID will not be PYGMYFILE_ID_ERASED or PYGMYFILE_ID_UNUSED
            // The Entry index will always be 0
            Volume.ParentFolder->Properties.Sectors[ 0 ].BaseAddress = Address;
            // Now fill out the properties structure with the data we wish to write to the first sector
            for( ii = 0, Index = PYGMYFILE_CHUNKLEN; ii < Volume.Desc.ChunksPerSector - 1; ii++ ){
                Index = fileFindNextID( Volume.ParentFolder, Index, &Properties );
                if( !Index ){
                    break;
                } else {
                    if( !strcmp( Properties.Name, NameString ) ){
                        return( Address + Index );
                    } // if
                    break;
                } // else
            } // for
            
        } // if
    } // for

    return( PYGMYFILE_INVALIDADDRESS );
}

u8 fileFreeVolume( PYGMYFILEVOLUME *Volume )
{
    // This function tests and frees all RAM allocated to store a Volume structure
    if( Volume->ParentFolder->Properties.Sectors ){
        free( Volume->ParentFolder->Properties.Sectors );
    } // if
    if( Volume->ParentFolder->Properties.Name ){
        free( Volume->ParentFolder->Properties.Name );
    } // if
    if( Volume->ParentFolder->Properties.Path ){
        free( Volume->ParentFolder->Properties.Path );
    } // if
    if( Volume->ParentFolder ){
        free( Volume->ParentFolder );
    } // if

    if( Volume->Properties.Name ){
        free( Volume->Properties.Name );
    } // if
    if( Volume->Properties.Path ){
        free( Volume->Properties.Path );
    } // if
    if( Volume->Properties.Sectors ){
        free( Volume->Properties.Sectors );
    } // if
    if( Volume->Port ){
        free( Volume->Port );
    } // if
    if( Volume ){
        free( Volume );
    } // if

    return( TRUE );
}

u8 fileUnmount( u8 *Name )
{
    // This function searches for a match to Name in the global volume array an free the memory if found
    // Return is TRUE if the volume was found and FALSE otherwise
    PYGMYFILEVOLUME **Volumes, *CurrentVolume;
    u32 i, ii, Index, Deleted, Found;

    CurrentVolume = fileGetCurrentVolume( ); // Use this to test against volume we are unmounting to prevent path errors
    for( i = 0, Deleted = 0, Found = FALSE; i < globalVolumeCount; i++ ){
        if( !strcmp( globalVolumes[ i ]->Properties.Name, Name ) ){
            // We have found the volume, free the memory used and restack the volume array
            // First allocate memory needed to regenerate global array
            Deleted = i;
            Found = TRUE;

            Volumes = malloc( sizeof( PYGMYFILEVOLUME * ) * ( globalVolumeCount - 1 ) );
            if( !Volumes ){
                return( FALSE ); // RAM full
            } // if
            if( globalVolumes[ i ] == CurrentVolume ){
                // Set path to a safe state if we are unmounting the currently mounted volume
                globalVolumeCurrent = NULL;
                fileSetCurrentPath( NULL );
            } // if
            fileFreeVolume( globalVolumes[ i ] );
            
            break;
        } // if
    } // for
    // Now that the volume is freed, it's time to regenerate the global array
    // We have successfully regenerated the array and freed the volume we wished to unmount
    // Now free the global array and replace the pointer with the pointer to the regenerated array
    if( Found ){
        for( i = 0, Index = 0; i < globalVolumeCount; i++ ){
            if( i != Deleted ){
                // We don't want to copy the Volume we unmounted
                Volumes[ Index++ ] = globalVolumes[ i ];
            } // if
        } // for
        free( globalVolumes ); 
        // Decrement globalVolumeCount and return
        --globalVolumeCount;
        if( globalVolumeCount ){
            globalVolumes = Volumes;
        } else{ 
            globalVolumes = NULL;
        } // else
    } // if
  
    return( Found );
}

u8 fileMountAll( PYGMYMEMIO *MemIO, u8 Address, ... )
{
    // This function mounts all volumes found on the memory interface requested

}

void fileCopyProperties( PYGMYFILEPROPERTIES *EntryFrom, PYGMYFILEPROPERTIES *EntryTo )
{
    EntryTo->Sectors      = EntryFrom->Sectors;
    EntryTo->SectorGroups = EntryFrom->SectorGroups;
    EntryTo->SectorCount  = EntryFrom->SectorCount;
    EntryTo->Time         = EntryFrom->Time;
    EntryTo->ID           = EntryFrom->ID;
    EntryTo->Length       = EntryFrom->Length;
    EntryTo->Path         = EntryFrom->Path;
    EntryTo->Name         = EntryFrom->Name;
    EntryTo->Attributes   = EntryFrom->Attributes;
}

void fileConvertBufferToAddress( PYGMYFILEADDRESS *Address, u8 *Buffer )
{
    u8 i;
	
    Address->SectorCount =  (u16)( *(Buffer++) ) << 8;
    Address->SectorCount |= (u16)( *(Buffer++) );
    Address->BaseAddress = 0LL;
    for( i = 0; i < 7; i++ ){
        Address->BaseAddress |= (*(Buffer++)) << ( (6 - i) * 8 );
    } // for  
}

void fileConvertAddressToBuffer( PYGMYFILEADDRESS *Address, u8 *Buffer )
{
    // Address is stored in big-endian format
    u64 BaseAddress;
    u16 SectorCount;
    u8 i;

    BaseAddress = Address->BaseAddress; // Don't operate on the original address
    SectorCount = Address->SectorCount; // 
    *(Buffer++) = (u8)((u16)SectorCount >> 8);
    *(Buffer++) = (u8)((u16)SectorCount );
    // BaseAddresses are 7 bytes, or 56bit
    for( i = 0; i < 7; i++ ){
        Buffer[ 6 - i ] = (u8)((u64)BaseAddress);
        BaseAddress >>= 8LL;
    } // for
}

u16 fileGenerateDirectAddress( PYGMYFILEVOLUME *Volume, PYGMYFILEPROPERTIES *Properties, u32 Index, u64 *Address )
{
    // This function decodes base-address and count pairs and calculates a physical
    //  address off this information and the index
    // The number of bytes left in the current sector is returned, Address is loaded with address
    u32 i, Group, Sectors, SectorCapacity;
    u16 Offset, BufLen;

    SectorCapacity = Volume->Desc.SectorSize - PYGMYFILE_SECTORMARKERLEN;
    Sectors = Index / SectorCapacity;

    if( Sectors > Properties->SectorCount  ) { 
        *Address = PYGMYFILE_INVALIDADDRESS;
        return( FALSE );
    } // if
    // Sectors is treated as zero-base
    for( i = 0, Group = 0; i < Sectors && Group < Properties->SectorGroups; Group++ ){
        if( i + Properties->Sectors[ Group ].SectorCount > Sectors ){
            break;
        } // if
        i += Properties->Sectors[ Group ].SectorCount;
    } // for

    // Calculate the offset into the current sector
    if( Sectors ){
        Offset = PYGMYFILE_SECTORMARKERLEN + ( Index % ( Sectors * SectorCapacity ) );
    } else{
        Offset = PYGMYFILE_SECTORMARKERLEN + Index;
    } // else
    BufLen = ( Volume->Desc.SectorSize ) - Offset; // Remaining bytes in the current sector
    
    *Address = Properties->Sectors[ Group ].BaseAddress + (( Sectors - i ) * Volume->Desc.SectorSize ) + Offset;
    if( *Address > Properties->Sectors[ Properties->SectorGroups - 1 ].BaseAddress + ( Properties->Sectors[ Properties->SectorGroups - 1 ].SectorCount * Volume->Desc.SectorSize ) ){
        // Past SG boundary
        *Address = PYGMYFILE_INVALIDADDRESS;
        return( 0 );
    } // if
    
    return( BufLen ); // Return number of bytes remaining in current sector
}

/*
u16 fileGenerateDirectAddress( PYGMYFILEVOLUME *Volume, PYGMYFILEPROPERTIES *Properties, u32 Index, u64 *Address )
{
    // This function decodes base-address and count pairs and calculates a physical
    //  address off this information and the index
    // The number of bytes left in the current sector is returned, Address is loaded with address
    u32 i, Group, Sectors;
    u16 Offset, BufLen;

    Sectors = Index / ( Volume->Desc.SectorSize - PYGMYFILE_IDLEN );
    if( Sectors > Properties->SectorCount  ) { 
        *Address = PYGMYFILE_INVALIDADDRESS;
        return( FALSE );
    } // if
    for( i = 0, Group = 0; i < Sectors && Group < Properties->SectorGroups; Group++ ){
        if( i + Properties->Sectors[ Group ].SectorCount > Sectors ){
            break;
        } // if
        i += Properties->Sectors[ Group ].SectorCount;
    } // for
   
    Offset = ( ( Index + ( ( 1 + Sectors ) * PYGMYFILE_SECTORMARKERLEN ) ) );
    BufLen = ( Volume->Desc.SectorSize ) - Offset;
    
    *Address = Properties->Sectors[ Group ].BaseAddress + (( Sectors - i ) *
        Volume->Desc.SectorSize ) + Offset;
    
    return( BufLen ); // Return number of bytes remaining in current sector
}*/

u8 fileMergeSectorGroups( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *PropertiesFrom, PYGMYFILEPROPERTIES *PropertiesTo )
{
    // WARNING: PropertiesTo will be reallocated and must be passed allocated. PropertiesFrom will not be freed.
    // This function merges in sectors from another sector group, there must never be identical sectors shared by the sector groups
    // Normally, a merge operation would take place after allocating new sectors, but before writing the updated sector groups
    // This merge operation will reduce memory usage in the parent folder
    // Example:
    // BaseAddress: 0
    //    Sectors:  2
    // BaseAddress: 8192
    //    Sectors:  1
    // = BaseAddress: 0
    // =  Sectors:  3
    PYGMYFILEADDRESS *TmpSectors;
    u64 AddressTo, AddressFrom;
    u32 i, ii, Address;
    
    if( PropertiesFrom->SectorGroups == 0 ){
        return( FALSE ); // Invalid Sector Groups, 0 not allowed
    } // if
    
    for( i = 0; i < PropertiesTo->SectorGroups; i++ ){
        // Calculate the address 1 sector past the end of the sectors included in the group 
        AddressTo = PropertiesTo->Sectors[ i ].BaseAddress + ( ( PropertiesTo->Sectors[ i ].SectorCount ) * ParentFolder->Volume->Desc.SectorSize );
        // Iterate through the sectorgroups we are merging in, look for a BaseAddress that matches our calculated address
        for( ii = 0; ii < PropertiesFrom->SectorGroups; ii++ ){
            if( AddressTo == PropertiesFrom->Sectors[ ii ].BaseAddress ){
                // We have a match, which means the sectors are contiguous
                // The base address remains the same, we only need to add the sector count
                PropertiesTo->Sectors[ i ].SectorCount += PropertiesFrom->Sectors[ ii ].SectorCount;
                // Now set the BaseAddress to 0 so we don't include the sector group in the last step
                PropertiesFrom->Sectors[ i ].BaseAddress = 0;
            } // if
        } // for
    } // for
    // We have merged in all contiguous groups, now we will merge in any non contiguous groups
    for( i = 0; i < PropertiesFrom->SectorGroups; i++ ){
        if( PropertiesFrom->Sectors[ i ].BaseAddress != 0 ){
            // We just found a SectorGroup that we can append to PropertiesTo
            TmpSectors = realloc( PropertiesTo->Sectors, ( 1 + PropertiesTo->SectorGroups ) * sizeof( PYGMYFILEADDRESS ) );
            if( !TmpSectors ){
                return( FALSE ); // Memory failure
            } // if
            PropertiesTo->Sectors = TmpSectors; 
            PropertiesTo->Sectors[ PropertiesTo->SectorGroups ].BaseAddress = PropertiesFrom->Sectors[ i ].BaseAddress;
            PropertiesTo->Sectors[ PropertiesTo->SectorGroups ].SectorCount = PropertiesFrom->Sectors[ i ].SectorCount;
            ++PropertiesTo->SectorGroups;
        } // if
    } // for
    PropertiesTo->SectorCount += PropertiesFrom->SectorCount;
    
    return( TRUE );
}

u8 fileAppendSectors( PYGMYFOLDER *ParentFolder, u32 RequestedSectors, PYGMYFILEPROPERTIES *Properties )
{
    // WARNING: This function reallocates Sectors, if Sectors is not previously allocated, this will cause a fault
    // This function seeks a contiguous run of sectors
    // Since the number of groups required to allocate the requested number of sectors is
    //  unknown until allocation is finished,the function must allocate the sector groups array
    PYGMYFILEPROPERTIES PropertiesFrom;
    PYGMYFILEADDRESS *Address;
    u64 TmpAddress, Addresses[ 33 ];
    u32 i, ii, Count;
    u16 MemLen;
    u8 Chunks;
  
    if( ParentFolder->Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    PropertiesFrom.Sectors = malloc( sizeof( PYGMYFILEPROPERTIES ) );
    if( !PropertiesFrom.Sectors ){
        return( FALSE ); // Memory error
    } // if
    // We are going to start a sector group in PropertiesFrom that we can merge with the sector groups in Properties
    PropertiesFrom.SectorCount = 0;
    PropertiesFrom.SectorGroups = 0;
    // Subdivide number of requested sectors until suitable level of granularity is reached
    for( i = 0; i < RequestedSectors; ){
        Count = RequestedSectors / ( 1 + i ); // Test for
        TmpAddress = fileFindContiguousEmptySectors( ParentFolder->Volume, Count );// - PYGMYFILE_SECTORMARKERLEN;
        
        if( TmpAddress ){
            // If return is non-zero, add sector group and continue
            PropertiesFrom.Sectors = realloc( PropertiesFrom.Sectors, sizeof( PYGMYFILEADDRESS ) *
                ( PropertiesFrom.SectorGroups + 1 ) );
            if( !PropertiesFrom.Sectors ){
                return( FALSE ); // Memory Full
            } // if
            PropertiesFrom.Sectors[ PropertiesFrom.SectorGroups ].BaseAddress = TmpAddress;
            PropertiesFrom.Sectors[ PropertiesFrom.SectorGroups ].SectorCount = Count;
    
            // Tag Sectors from this group
            
            for( ii = 0; ii < Count; ii++ ){
                ParentFolder->Volume->IO->PutWord( ParentFolder->Volume->Port, PropertiesFrom.Sectors[ PropertiesFrom.SectorGroups ].BaseAddress +
                    ( ii * ParentFolder->Volume->Desc.SectorSize ), PYGMYFILE_ACTIVESECTOR );
            } // for
            ++PropertiesFrom.SectorGroups;
            PropertiesFrom.SectorCount += Count;
            RequestedSectors -= Count;
        } else{
            ++i;
        } // else
    } //for
    
    if( ParentFolder->Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    // To prevent orphaned sectors, we must immediately write the updated sector groups
    fileMergeSectorGroups( ParentFolder, &PropertiesFrom, Properties );
    // Find and delete all the sector group chunks
    for( i = PYGMYFILE_SGCHUNK; i < PYGMYFILE_LASTSGCHUNK; i++ ){
        if( !fileFindChunkAddress( Properties->ID, i, ParentFolder, &TmpAddress ) ){
            break;
        } // if
        fileDeleteChunk( ParentFolder->Volume, TmpAddress );
    } // for
    Chunks = 1 + ( Properties->SectorGroups / 2 );
    if( !fileAllocateChunks( Properties->ID, ParentFolder, Chunks, Addresses ) ){
        return( FALSE );
    } // if
    fileWriteSectorGroups( ParentFolder, Addresses, Properties );
    fileUpdateLength( ParentFolder, Properties ); 

    return( TRUE );
}

u32 fileGetFreeChunks( PYGMYFOLDER *ParentFolder, u32 *ErasedChunks )
{
    u64 SrcAddress;
    u32 i, SrcIndex, ID, EmptyChunks;
    u32 ChunksPerSector;

    SrcIndex = 0;
    *ErasedChunks = 0;
    EmptyChunks = 0;
    ChunksPerSector = ParentFolder->Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN;
    // Increment through each sector, the address will be generated off an Index, so we can ignore the specifics of each sector
    for( i = 0; i < ParentFolder->Properties.SectorCount; i++ ){
        // Increment through each chunk of each sector
        for( ; SrcIndex < ChunksPerSector; SrcIndex++ ){
            // Generate the address, The SectorGroup information in Properties is used for this purpose
            fileGenerateDirectAddress( ParentFolder->Volume, &ParentFolder->Properties, SrcIndex * PYGMYFILE_CHUNKLEN, &SrcAddress );
            if( SrcAddress == PYGMYFILE_INVALIDADDRESS ){
                return( 0 );
            } // if
            SrcAddress -= PYGMYFILE_SECTORMARKERLEN; // Address is always returned indexed past the sector marker
            // Load the Chunk ID 
            ID = ParentFolder->Volume->IO->GetLong( ParentFolder->Volume->Port, SrcAddress );
            if( ID == PYGMYFILE_ID_ERASED ){
                *ErasedChunks += 1;
            } else if( ID == PYGMYFILE_ID_UNUSED ){
                ++EmptyChunks;
            } // else if
        } // for
    } // for

    return( EmptyChunks );
}

u8 fileRecycleChunks( PYGMYFOLDER *ParentFolder )
{
    // Working, current
    // This function allocates new SectorGroups, copies the active chunks to the new sectors and deletes the old sectors
    PYGMYFILEPROPERTIES Properties;
    u64 SrcAddress, DestAddress;
    u32 ID, i, SrcIndex, DestIndex, Chunk;
    u32 ChunksPerSector, ChunksToAlloc;
    u8 Buffer[ PYGMYFILE_CHUNKLEN ];

    // Allocate the new SectorGroups
    ChunksPerSector = ( ParentFolder->Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN ) - 1;
    // Always allocate one more sector than we are recycling, this helps reduce erase cycles
    if( fileAllocateSectors( ParentFolder, ParentFolder->Properties.SectorCount + 1, &Properties ) ){
        SrcIndex = 1; // ChangedFrom: 0
        DestIndex = 1;
        // Increment through each sector, the address will be generated off an Index, so we can ignore the specifics of each sector
        for( i = 0; i < ParentFolder->Properties.SectorCount; i++ ){
        
            // Increment through each chunk of each sector
            for( ; SrcIndex < ChunksPerSector; SrcIndex++ ){
                // Generate the address, The SectorGroup information in Properties is used for this purpose
                fileGenerateDirectAddress( ParentFolder->Volume, &ParentFolder->Properties, SrcIndex * PYGMYFILE_CHUNKLEN, &SrcAddress );
                if( SrcAddress == PYGMYFILE_INVALIDADDRESS ){
                    return( FALSE );
                } // if
                SrcAddress -= PYGMYFILE_SECTORMARKERLEN; // Address is always returned indexed past the sector marker
                
                // Load the Chunk ID 
                ID = ParentFolder->Volume->IO->GetLong( ParentFolder->Volume->Port, SrcAddress );
                if( ID != PYGMYFILE_ID_ERASED && ID != PYGMYFILE_ID_UNUSED ){
                    // We have a Chunk to transfer
                    fileGenerateDirectAddress( ParentFolder->Volume, &Properties, DestIndex * PYGMYFILE_CHUNKLEN, &DestAddress );
                    if( DestAddress == PYGMYFILE_INVALIDADDRESS ){
                        ParentFolder->Volume->Status |= PYGMYFILE_FULL;
                        return( FALSE );
                    } // if
                    DestAddress -= PYGMYFILE_SECTORMARKERLEN; // Address is always returned indexed past the sector marker
                    ParentFolder->Volume->IO->GetBuffer( ParentFolder->Volume->Port, SrcAddress, Buffer, PYGMYFILE_CHUNKLEN );
                    ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, DestAddress, Buffer, PYGMYFILE_CHUNKLEN );
                    // Erase the Chunk
                    // Was erasing chunk, decided to erase only sector, test code
                    SrcAddress = ( SrcAddress / ParentFolder->Volume->Desc.SectorSize ) * ParentFolder->Volume->Desc.SectorSize;
                    ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, SrcAddress, 0 );
                    ++DestIndex;
                } // if 
            } // for
        } // for
        
        if( i == Properties.SectorCount && SrcIndex == ChunksPerSector ){
            // There are no free chunks remaining, this is a serious error
            return( FALSE );
        } // if

        // Free the old SectorGroups
        
        // Move the pointer for the new SectorGroups
        free( ParentFolder->Properties.Sectors );
        ParentFolder->Properties.Sectors = Properties.Sectors;
        ParentFolder->Properties.SectorCount = Properties.SectorCount;
        ParentFolder->Properties.SectorGroups = Properties.SectorGroups;
        
        fileUpdateSectorGroups( ParentFolder, &ParentFolder->Properties );
        //print( COM3, "..." );
        

        return( TRUE );
    } // if

    return( FALSE );
}



u8 fileAllocateChunks( u32 ID, PYGMYFOLDER *ParentFolder, u8 Chunks, u64 *Addresses )
{
    // Working
    // fileAllocateChunks() will allocate chunks by finding a run ( if possible ) and writing the ID to each allocated chunk
    // *Address will be filled with the addresses of the chunks allocated
    // Case 1: If there is sufficient space in the current ParentFolder:
    //    Attempt to allocate a contiguous run of chunks.
    //    If unable to allocate a contiguous run of chunks, allocate individual chunks
    // Case 2: If there is insufficient space in the current ParentFolder: 
    //    Append one or more additional sectors to the ParentFolder.
    //    If there is insufficient space in the ParentFolders Parent, allocate recursively with a call to AppendSectors
    //    Go to Case 1 and complete.
    // Case 3: If there is insufficient space to allocate additional chunks:
    //    Return FALSE;
    PYGMYFILEVOLUME *Volume;
    u64 Address, TmpAddress;
    u32 Index, TmpIndex, ErasedChunks, EmptyChunks, SectorsNeeded;
    u32 i, ii;
    u8 CurrentChunk;

    Volume = ParentFolder->Volume;
    if( Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    
    // Check for available contiguous chunks, this will tell us if we have sufficient space
    EmptyChunks = fileGetFreeChunks( ParentFolder, &ErasedChunks );
    //( Case 2: 
    if( EmptyChunks < Chunks ){
        if( ( EmptyChunks + ErasedChunks ) >= Chunks ){
            // Attempt to free available memory by recycling chunks, if there are enough chunks, we will use them, otherwise recycle and allocate
            //print( COM3, "\rCalling fileRecycleChunks()" );
            if( !fileRecycleChunks( ParentFolder ) ){
               
                if( !fileAppendSectors( ParentFolder->Parent, SectorsNeeded, &ParentFolder->Properties ) ){
                    return( FALSE );
                } // if
                return( FALSE );
            } // if
        } else {
            // We must append another sector to the ParentFolder's Parent
            if( !ParentFolder->Parent ){
                // The code in this if statement is under test, the original is in the else below
                // If the folder that needs sectors appended is Root, we have to replace the sector, root only supports one sector
                // First find a free sector
                Address = fileFindContiguousEmptySectors( Volume, 1 );
                if( Address == PYGMYFILE_INVALIDADDRESS ){
                    return( FALSE );
                } // if
                // Now mark the sector as a volume (root) sector
                Volume->IO->PutWord( Volume->Port, Address, PYGMYFILE_VOLUMESECTOR );
                Volume->IO->PutBuffer( Volume->Port, Address + 2, globalPFATVersionTag, 12 ); // Write the filesystem version
                
                if( !fileWriteEntry( ParentFolder->Parent, &ParentFolder->Properties ) ){
                    return( FALSE );
                } // if
            } else{
                // The code in this else statement is the original
                // The folder that needs sectors appended is not root, append to Parent
                if( !fileAppendSectors( ParentFolder->Parent, SectorsNeeded, &ParentFolder->Properties ) ){
                    return( FALSE );
                } // if
            } // else
        } // else
        EmptyChunks = fileGetFreeChunks( ParentFolder, &ErasedChunks );
    }
    // Case 1: 
    // Contiguous chunks are not available, we must subdivide and allocate until we capture all the sectors we need
    if( EmptyChunks >= Chunks ){
        CurrentChunk = 0;
        for( i = 0; i < Chunks; ){
            Index = fileFindContiguousChunks( ParentFolder, Chunks - i ); // Returns an index into ParentFolder
            if( Index ){ // And index of 0 indicates failure 
                for( ii = 0; ii < Chunks - i; ii++ ){
                    fileGenerateDirectAddress( ParentFolder->Volume, &ParentFolder->Properties, Index + ( ii * PYGMYFILE_CHUNKLEN), &Address );
                    if( Address == PYGMYFILE_INVALIDADDRESS ){
                        return( FALSE );
                    } // if
                    Address -= PYGMYFILE_SECTORMARKERLEN; // Address is always returned indexed past the sector marker
                    Addresses[ CurrentChunk++ ] = Address;
                    Volume->IO->PutLong( Volume->Port, Address, ID );
                } // for
                i += ( Chunks - i ); // We will allocate the number of chunks remaining in following iterations
            } else{
                
                ++i;
            } // else
        } // for
        return( TRUE );
    } // if
       
    // Case 3:
    return( FALSE );
}


u32 fileFindContiguousChunks( PYGMYFOLDER *ParentFolder, u8 Chunks )
{
    // Count the number of erased and unused chunks
    // Return the number of contiguous unused chunks found and set ullStartAddress to start address OR
    // Return the number of erased chunks found and set ullStartAddress to PYGMYFILE_INVALIDADDRESS
    u64 Address;
    u32 i, StartAddressIndex, EndIndex, EntryID;
    u8 Unused;

    Unused = 0;
    EndIndex = ( ParentFolder->Properties.SectorCount * ParentFolder->Volume->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;
    for( i = 0; i < EndIndex; i++ ){
        fileGenerateDirectAddress( ParentFolder->Volume, &ParentFolder->Properties, i * PYGMYFILE_CHUNKLEN, &Address );
        if( Address == PYGMYFILE_INVALIDADDRESS ){
            return( 0 );
        } // if
        Address -= PYGMYFILE_SECTORMARKERLEN;
        EntryID = ParentFolder->Volume->IO->GetLong( ParentFolder->Volume->Port, Address );
        if( EntryID == PYGMYFILE_ID_ERASED ){
            // We have found an erased chunk
            Unused = 0; // An erased sector requires recycling before use, and is counted as "used"
        } else if( EntryID == PYGMYFILE_ID_UNUSED ){
            // We have found an unused chunk
            if( Unused == 0 ){
                // Recapture the start address each time the counter resets to zero
                StartAddressIndex = i * PYGMYFILE_CHUNKLEN;
            } //
            ++Unused;
            if( Unused == Chunks ){
                // We are done with our search, ucChunk unused chunks have been found, return
                return( StartAddressIndex );
            } // if
        } // else if
    } // for

    return( 0 ); // Index of 0 is invalid, valid index would be a minimum of SectorMarkerLen
}


u8 fileWriteEntry( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // !!! WARNING !!! fileWriteEntry() writes the properties contained in Properties
    // !!! WARNING !!! Do not call fileWriteProperties() directly
    // This function writes the Name, Sector Groups and Length
    u64 *Addresses, Address;
    u32 i, ID;
    u8 NameLen, Chunks, SGChunks, NameChunks, Buffer[ 27 ];
	
   
    if( ParentFolder->Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    NameLen = len( Properties->Name ); 
    ID = pdiaEncodeString( Properties->Name );
    if( ID == 0x00000000 || ID == 0xFFFFFFFF ){
        return( FALSE ); // These are the only two restricted ID values
    } // if
    
    NameChunks = 1 + ( ( NameLen + 6 ) / PYGMYFILE_CHUNKPAYLEN );
    SGChunks = 1 + (Properties->SectorGroups / 3); // There are 3 9byte Sector Groups Nodes stored per chunk
    Chunks = 1 + NameChunks + SGChunks; // This stands for LengthChunk + NameChunks + SGChunks
    Addresses = malloc( 8 * Chunks );
    if( !Addresses ){
        return( FALSE );
    } // if
    
    // Check for existing entry with same ID before continuing
    if( fileFindChunk( ID, 0, ParentFolder, &Address, Buffer ) ){
        return( FALSE ); // Files exists
    } // if
    // We allocate all the chunks we will need before we begin to write
    // We don't want to find out that we have insufficient memory after beginning write operations
    if( fileAllocateChunks( ID, ParentFolder, Chunks, Addresses ) ){
        if( fileWriteProperties( ParentFolder, Addresses, Properties ) ){
            if( Properties->SectorGroups ){
                fileWriteSectorGroups( ParentFolder, &Addresses[ NameChunks], Properties );
            } // if
            fileWriteLength( ParentFolder, Addresses[NameChunks+SGChunks], SGChunks, Properties->Length );
            free( Addresses );
            return( TRUE );
        } // if
    } // if

    free( Addresses );

    return( FALSE );
}

u8 fileWriteProperties( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties )
{
    // When writing Attributes, will not write WRITE if Volume does not have WRITE
    // Writes Time stamp, filename length and Name ( may span chunks 0-9 )
    // ID must have been generated by the calling function
    u32 i, ii, NameLen;
    u8 Chunk, NameChunks;

    
    NameLen = len( Properties->Name );
    if( NameLen > 255 || NameLen == 0 ){
        return( TRUE ); // Bad FileName Len
    } // if
    Chunk = PYGMYFILE_NAMECHUNK;
    NameChunks = 1 + ( ( NameLen + 6 ) / PYGMYFILE_CHUNKPAYLEN );
    // Write Chunk 0, starting with ID 
    ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, Addresses[ 0 ], Properties->ID ); // ID written at allocation
    // Write Chunk Count, always 0 for first Properties ( Name ) Chunk
    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ 0 ] + 4, Chunk++ );
    // Write 32bit Time ( stores file creation time )
    ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, Addresses[ 0 ] + 5, timeGet() );
    // Write Attributes, Mask properties not supported by the volume
    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ 0 ] + 9, ( Properties->Attributes & ( READ|WRITE|FOLDER ) ) );
    // Write length of file name
    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ 0 ] + 10, NameLen );
    if( NameLen < 22 ){
        // Entire file name will fit in Chunk 0
        
        ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, Addresses[ 0 ] + 11, Properties->Name, NameLen );
    } else{
       
        ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, Addresses[ 0 ] + 11, Properties->Name, 21 );
	for( i = 21, ii = 1; i < NameLen; ii++ ){
            // Write ID and Chunk number before each file name payload ( up to 27 bytes )
            ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ ii ] + 4, Chunk++ );
            if( ( NameLen - i ) > 27 ){
              
                ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, Addresses[ ii ] + 5, Properties->Name + i, 27 );
                i += 27;
            } else{
                // This is the last chunk
            
		ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, Addresses[ ii ] + 5, Properties->Name + i, NameLen - i );
		break;
            } // else
	} // for
    } // else
	
    return( TRUE );
}

u8 fileWriteLength( PYGMYFOLDER *ParentFolder, u64 Address, u8 SGChunks, u32 Length )
{
    // !!! WARNING !!! This function can only be used to write a new chunk
    // find chunk 254 and load the length, also load Sector Groups Count if supported
    // the length is stored as: [ SG0 1B ][ SG1 1B ][ SG2 1B ][ LEN0 4B ][ LEN1 4B ][ LEN2 4B ][ LEN3 4B ][ LEN4 4B ][ LEN5 4B ]
    // All 1s is unused, all 0s is erased, any other number for SG is number of Sector Groups or LEN is Length of file in bytes
    // compare the Sector Groups Count and Length, only write if changed
    // If changed, find the first unused ( all 1s ) section, erase the current value, and write the new value
    // If either Sector Groups or Length have changed and there are no unused spots, write a new chunk and erase current chunk
    u64 TmpAddress;

  
    // Write the Chunk number after the ID ( already written at allocation )
    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Address + 4, PYGMYFILE_LENGTHCHUNK );
    // Write the number of Sector Group Chunks ( this reduces load time later )
    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Address + 5, SGChunks );
    // Write the 32bit file length into LEN0
    ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, Address + 8, Length );
	
    return( TRUE );
}

u8 fileUpdateLength( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties  )
{
    u64 Address;//, TmpAddress;
    u32 Length;
    u8 i, SectorGroups, CurrentChunks, ChunkFound, Buffer[ 27 ], ClearToWrite;

    if( ParentFolder->Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    ChunkFound = fileFindChunk( Properties->ID, PYGMYFILE_LENGTHCHUNK, ParentFolder, &Address, Buffer );
    if( ChunkFound ){
        SectorGroups = 1 + ( Properties->SectorGroups / 3 );
        // The first 3 bytes contain the Sector Group Counts
        // SectorGroups can never be 0, unless it's been deleted
        for( i = 0, ClearToWrite = 0; i < 3; i++ ){
            if( Buffer[ i ] != 0 && Buffer[ i ] != 0xFF ){
                if( Buffer[ i ] != SectorGroups ){
                    ClearToWrite = TRUE;
                    ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Address + 5 + i, 0 ); // erase
                } // if
                break;
            } else if( Buffer[ i ] == 0xFF && ClearToWrite ){ 
                ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Address + 5 + i, SectorGroups ); // write
                break;
             } // else if
        } // for
        for( i = 0, ClearToWrite = 0; i < 6; i++ ){
            Length = convertBufferToU32( Buffer + 3 + ( i * 4 ), BIGENDIAN );
            if( Length != 0xFFFFFFFF ){
                if( Length && Length != Properties->Length ){
                    ClearToWrite = TRUE; // We don't want to write an update unless we had a value to update
                    ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, Address + 8 + ( i * 4 ), 0 ); // erase
                } // if
            } else if( Length == 0xFFFFFFFF && ClearToWrite ){
                ParentFolder->Volume->IO->PutLong( ParentFolder->Volume->Port, Address + 8 + ( i * 4 ), Properties->Length ); // increment and write
                return( TRUE );
            }  // else if
        } // for
        fileDeleteChunk( ParentFolder->Volume, Address );
    } // if
    
    // if we dropped through to here, we must create a new length entry
    // first delete the current length chunk
    if( fileAllocateChunks( Properties->ID, ParentFolder, 1, &Address ) ){
        if( fileWriteLength( ParentFolder, Address, 1 + ( Properties->SectorGroups / 3 ) , Properties->Length ) ){
            return( TRUE );
        } // if
    } // if

    return( FALSE );
}


u8 fileGetSectorGroupCount( u32 ID, PYGMYFOLDER *ParentFolder )
{
    // Returns the number of Chunks used by the sector groups, NOT the number of sector groups
    u64 Address;
    u8 i, SGChunks, Buffer[ 27 ];

    // Search for SGChunks from the first SGChunk until we get a false result
    for( i = PYGMYFILE_SGCHUNK; i < PYGMYFILE_LASTSGCHUNK; i++ ){
        if( !fileFindChunk( ID, i, ParentFolder, &Address, Buffer ) ){
            // Sector Groups are arranged sequentially
            // The first false result marks the end of the sequence
            break;
        } // if
    } // for

    return( i - PYGMYFILE_SGCHUNK );
}

u8 fileUpdateSectorGroups( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // Working
    // This function replaces SectorGroups in ParentFolder with the SectorGroups in Properties
    // The old SectorGroups are deleted 
    // fileWriteSectorGroups is only intended for new files
    u64 Address, *Addresses;
    u32 ChunksPerSector;
    u8 i, Chunks, Buffer[ PYGMYFILE_CHUNKLEN ];

    if( ParentFolder->Volume->Status & PYGMYFILE_FULL ){
        return( FALSE );
    } // if
    Addresses = malloc( sizeof( u64 ) * ( Properties->SectorGroups / 2 ) );//( PYGMYFILE_CHUNKPAYLEN / PYGMYFILE_ADDRESSLEN ) ) );
    if( !Addresses ){
        return( FALSE );
    } // if
    ChunksPerSector = PYGMYFILE_CHUNKPAYLEN / PYGMYFILE_ADDRESSLEN;
    Chunks = 1 + ( ParentFolder->Properties.SectorGroups / ChunksPerSector ) + ( ParentFolder->Properties.SectorGroups % ChunksPerSector );
    //Chunks += ParentFolder->Properties.SectorGroups % ( PYGMYFILE_CHUNKPAYLEN / PYGMYFILE_ADDRESSLEN );
    //print( COM3, "\r\r\r\rAllocating %d Chunks\r\r\r\r", Chunks );
    //Chunks = 1 + ( Properties->SectorGroups / 2 );//( PYGMYFILE_CHUNKPAYLEN / PYGMYFILE_ADDRESSLEN ) );
    //Chunks += Properties->SectorGroups % 2;//( PYGMYFILE_CHUNKPAYLEN / PYGMYFILE_ADDRESSLEN ) );

    fileDeleteSectorGroups( ParentFolder->Properties.ID, ParentFolder->Parent );
    if( !fileAllocateChunks( ParentFolder->Properties.ID, ParentFolder->Parent, Chunks, Addresses ) ){
        free( Addresses );
        return( FALSE );
    } // if
    
    fileWriteSectorGroups( ParentFolder->Parent, Addresses, Properties );
    free( Addresses );

    return( TRUE );
}

u8 fileWriteSectorGroups( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties )
{
    // Working Active
    u64 TmpAddress;
    u32 i, ii, AddressIndex, SectorCount, FreeChunks, ErasedChunks;
    u8 Buffer[ 9 ]; // Storage for processed Sector Group Address

    if( Properties->SectorGroups == 0 ){
        return( FALSE ); // Nothing to write
    } // if
    PYGMY_WATCHDOG_REFRESH;
    
    SectorCount = 1 + ( Properties->SectorGroups/2); // This is the number of sector chunks that will be required
    FreeChunks = fileGetFreeChunks( ParentFolder, &ErasedChunks );
    if( FreeChunks < SectorCount ){
          if( ErasedChunks ){
              // If there are any erased chunks, free them before appending
              fileRecycleChunks( ParentFolder );
          } // if
          if( ( ErasedChunks + FreeChunks ) < SectorCount ){
              // There are sufficient chunks if we recycle the erased chunks
              if( !fileAppendSectors( ParentFolder, 1, &ParentFolder->Properties ) ){
                  // There is no memory left on the physical device
              } //if
          } //if
    } 
    //SectorCount = Properties->SectorCount;
   
    for( i = 0, ii = 0, AddressIndex = 0; ii < Properties->SectorGroups; ii++ ){
        if( ii == Properties->SectorGroups ){
            // All groups have been written
            return( TRUE );
        } // if
        fileConvertAddressToBuffer( &Properties->Sectors[ ii ], Buffer );
        //ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ AddressIndex ] + 4 + ( i * 9 ), PYGMYFILE_SGCHUNK + ii );
        if( i == 0 ){
            ParentFolder->Volume->IO->PutChar( ParentFolder->Volume->Port, Addresses[ AddressIndex ] + 4, PYGMYFILE_SGCHUNK + AddressIndex );
        } // if

        ParentFolder->Volume->IO->PutBuffer( ParentFolder->Volume->Port, Addresses[ AddressIndex ] + 5 + ( i * 9 ), Buffer, 9 );
        SectorCount -= Properties->Sectors[ ii ].SectorCount; // Decrement the number of sectors in the last SG       
        
        if( (++i) == 2 ){
            ++AddressIndex;
            i = 0;
	} // if
    } // for

    return( FALSE );
}

u8 fileLoadEntry( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // fileLoadEntry() pieces together all the chunks for the requested file ID.
    // fileLoadLength() finds and returns the file length
    // fileLoadProperties() finds and loads all the chunks containing the file name and attributes
    // fileLoadSectorGroups() finds and loads all the chunks containing the sector allocation groups
    // fileLoadHash() finds and loads the file hash, or checksum
    // fileLoadOwner() finds and loads the file owner and permissions data
    // the chunks are grouped as follows:
    //    0-31    Properties ( Creation Time, Attributes, Name )  
    //    32-232  Sector Groups
    //    233-250 Hash
    //    251     Meta Data
    //    252     Journal 
    //    253     Permissions
    //    254     Length
    //    255     Reserved
    // Always load the Name ( Properties ) and Length chunks first, fileLoadLength() returns the Sector Group Count
    u8 Status, SGChunks;
    
    if( ID == 0 || ID == 0xFFFFFFFF ){
        return( FALSE ); // 0 is an invalid ID, as is 0xFFFFFFFF
    } // if
    Status = fileLoadProperties( ID, ParentFolder, Properties );
    if( Status ){
        SGChunks = fileLoadLength( ID, ParentFolder, Properties );
        if( SGChunks ){
            if( fileLoadSectorGroups( ID, SGChunks, ParentFolder, Properties ) ){
                return( TRUE );
            } else{
                print( COM3, "\rfileLoadSectorGroups() Failed" );
            } //else
        } else{
            print( COM3, "\rfileLoadSectorGroups() Failed" );
        } // else
    } else{
        print( COM3, "\rfileLoadProperties() Failed" );
    } // else

    return( FALSE );
}

u8 fileLoadLength( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // find chunk 254 and load the length into pygmyEntry->Length, also load pygmyEntry->SectorGroups if supported
    // the length is stored as: [ SG0 1B ][ SG1 1B ][ SG2 1B ][ LEN0 4B ][ LEN1 4B ][ LEN2 4B ][ LEN3 4B ][ LEN4 4B ][ LEN5 4B ]
    // All 1s is unused, all 0s is erased, any other number for SG is number of Sector Groups or LEN is Length of file in bytes
    // return the number of Sector Groups recorded in the first 3 bytes
    u64 Address;
    u32 Length;
    u8 i, SG, Buffer[ 27 ];

    SG = 1; // There is always at least 1 Sector Group count
    Length = 0;
    if( fileFindChunk( ID, PYGMYFILE_LENGTHCHUNK, ParentFolder, &Address, Buffer ) ){
        // We have found the length chunk, now load the Sector Group Count and the Length
        for( i = 0; i < 3; i++ ){
            if( Buffer[ i ] != 0xFF && Buffer[ i ] != 0x00 ){
                SG = Buffer[ i ]; // Found SG Count // was 0
                break;
            } // if
        } // for
       // There are six tags stored in each chunk, when a file has been written to and then closed, 
       //   the current tag is changed to 0 and the next erased ( 0xFFFFFFFF ) tag is updated with the current file length
       //   When all 6 tags have been used, the ID is changed to 0 and a new Length chunk is allocated
        for( i = 0; i < 6; i++ ){ 
            Length = convertBufferToU32( Buffer + 3 + ( 4 * i ), BIGENDIAN );
            if( Length != 0xFFFFFFFF && Length != 0 ){
                // We found the current Length Tag, time to return
                Properties->Length = Length;
                return( SG );
                //break;
            } // if
        } // for
    } // if

    // If there is no length chunk, that means the file hasn't had any write operations performed yet
    // This is not an error condition
    Properties->Length = 0;

    return( SG );
}

u8 fileFindChunkAddress( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u64 *Address )
{
    u32 i, ii, iii, j, EntryCount, TmpID;
    u8 TmpChunk, Buffer[ 5 ];

    EntryCount = ParentFolder->Volume->Desc.SectorSize / PYGMYFILE_CHUNKLEN; // 32 is length of chunks
    
    for( i = 0; i < ParentFolder->Properties.SectorGroups; i++ ){ // This is the number of parent sectors containing chunks to sort
        for( ii = 0; ii < ParentFolder->Properties.Sectors[ i ].SectorCount; ii++ ){ // Iterate through Chunks and process matches
            for( iii = 1; iii < EntryCount; iii++ ){
                *Address = ParentFolder->Properties.Sectors[ i ].BaseAddress +
                    ( ii * ParentFolder->Volume->Desc.SectorSize ) + ( iii * PYGMYFILE_CHUNKLEN );
                // Load the first 5 bytes of the chunk, this contains the ID and the chunk number
                ParentFolder->Volume->IO->GetBuffer( ParentFolder->Volume->Port, *Address, Buffer, 5 ); 
                TmpID = convertBufferToU32( Buffer, BIGENDIAN );
                TmpChunk = Buffer[ 4 ];
                
                if( TmpID == ID && TmpChunk == Chunk ){
                    // We found the chunk requested, load the 27 byte payload and return
                    return( TRUE );
                } // if
            } // for
        } // for
    } // for
    *Address = PYGMYFILE_INVALIDADDRESS;

    return( FALSE );
}

u8 fileFindActiveChunk( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u64 *Address, u8 *Buffer )
{
    // fileFindChunk() does not allocate ucBuffer, 27 bytes must be allocated by the calling functions, static allocation is recommended
    // pygmySG is a pointer to the parent sector group array, this array must be filled out by the calling function

    if( fileFindChunkAddress( ID, Chunk, ParentFolder, Address ) ){
        ParentFolder->Volume->IO->GetBuffer( ParentFolder->Volume->Port, *Address + 5, Buffer, 27 );
        return( TRUE );
    } // if
    *Address = PYGMYFILE_INVALIDADDRESS;

    return( FALSE ); // The requested chunk doesn't exist in the current folder
}


u8 fileFindChunk( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u64 *Address, u8 *Buffer )
{
    // fileFindChunk() does not allocate ucBuffer, 27 bytes must be allocated by the calling functions, static allocation is recommended
    // pygmySG is a pointer to the parent sector group array, this array must be filled out by the calling function

    if( fileFindChunkAddress( ID, Chunk, ParentFolder, Address ) ){
        ParentFolder->Volume->IO->GetBuffer( ParentFolder->Volume->Port, *Address + 5, Buffer, 27 );
        return( TRUE );
    } // if
    *Address = PYGMYFILE_INVALIDADDRESS;

    return( FALSE ); // The requested chunk doesn't exist in the current folder
}

u8 fileLoadProperties( u32 ID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // The Properties ( Name ) chunk is formatted as follows:
    // [ ID 4B ][ CHUNK 1B ][ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // Since fileFindChunk() fills ucBuffer with the 27 bytes following CHUNK, we will treat the format as follows:
    // [ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // If the file name length exceeds the 21 bytes available in the first Chunk, additional chunks will be loaded to
    // retrieve the remainder of the file name. All additional Properties ( Name ) chunks contain up to 27 bytes of name each,
    // spanning up to 10 chunks ( 0 - 9 )
    u64 Address;
    u32 i, ii;
    u8 Buffer[ 27 ], NameLen, NameIndex, NullTerminator, Chunks;

    if( !fileFindChunk( ID, 0, ParentFolder, &Address, Buffer ) ){
        // No Name Chunk means the file doesn't exist
        
        return( FALSE );
    } // if
    Properties->Time = convertBufferToU32( Buffer, BIGENDIAN );
    Properties->Attributes = Buffer[ 4 ];
    NameLen = Buffer[ 5 ];
    Properties->Name = malloc( NameLen );
    if( !Properties->Name ){
        return( FALSE );
    } // if
    Chunks = 1 + ( ( NameLen + 6 ) / 27 );
    NullTerminator = NameLen;
    if( NameLen <= 21 ){
        // The entire name will be in the first buffer
        memcpy( Properties->Name, Buffer+6, NameLen );
        Properties->Name[ NullTerminator ] = '\0'; // Null terminate string
        
        return( TRUE );
    } else{
        // The first 21 chars will be in the first buffer, load and then seek the next buffer
        memcpy( Properties->Name, Buffer+6, 21 );
        NameLen -= 21;
    } // else
    // Now load the remainder of the filename
    for( i = 1, NameIndex = 21; i < Chunks && i < PYGMYFILE_SGCHUNK; i++ ){
        if( !fileFindChunk( ID, i, ParentFolder, &Address, Buffer ) ){
            // This indicates file system corruption and is a serious failure
            free( Properties->Name );
            return( FALSE );
        } // if
        memcpy( Properties->Name + NameIndex, Buffer, NameLen );
        if( NameLen > 27 ){
            NameLen -= 27;
        } else{
            break;
        } // else
        NameIndex += 27;
    } // for
    Properties->Name[ NullTerminator ] = '\0';

    return( TRUE );
}

u8 fileLoadSectorGroups( u32 ID, u8 SGChunks, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // Sector Groups are stored 3 per chunk at 9 bytes each
    // A Sector Groups is formatted as follows: [ SECTORCOUNT 2B ][ BASEADDRESS 7B ]
    PYGMYFILESECTORGROUPS TmpSG;
    PYGMYFILEADDRESS TmpAddress;
    u64 Address;
    u32 i, ii, iii, SectorCount;
    u8 *BufferPtr, Buffer[ 27 ];

    TmpSG.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !TmpSG.Sectors ){
        return( FALSE );
    } // if
    TmpSG.SectorGroups = 0;
    TmpSG.SectorCount = 0;
    
    for( i = 0; i < SGChunks; i++ ){
        if( fileFindChunk( ID, PYGMYFILE_SGCHUNK + i, ParentFolder, &Address, Buffer ) ){
            for( ii = 0; ii < 3; ii++ ){
                // Load the 16bit Sector Count
                fileConvertBufferToAddress( &TmpAddress, Buffer + ( ii * PYGMYFILE_ADDRESSLEN ) );
                // Now check the address for 0 length group or unused memory
                if( TmpAddress.BaseAddress >= PYGMYFILE_INVALIDBASEADDRESS || TmpAddress.BaseAddress < 0 ){
                    // If data is unused, we are past the last sector group
                    break;
                } else if( TmpAddress.SectorCount ){
                    // If uiSectorCount is 0, then the sector group entry was erased, ignore and continue
                    // otherwise, allocate memory for a new sector group and append to the list
                    TmpSG.SectorCount += TmpAddress.SectorCount;
                    ++TmpSG.SectorGroups;
                    TmpSG.Sectors = realloc( TmpSG.Sectors, ( 1 + TmpSG.SectorCount ) * sizeof( PYGMYFILEADDRESS ) );
                    if( !TmpSG.Sectors ){
                        return( FALSE );
                    } // if
                    TmpSG.Sectors[ TmpSG.SectorGroups - 1 ].SectorCount = TmpAddress.SectorCount;
                    TmpSG.Sectors[ TmpSG.SectorGroups - 1 ].BaseAddress = TmpAddress.BaseAddress;
                } // if
            } // for
        } else{
            // Sector Group chunks are returned in order.
            break;
        } // else
    } // for
    Properties->Sectors = TmpSG.Sectors;
    Properties->SectorGroups = TmpSG.SectorGroups;
    Properties->SectorCount = TmpSG.SectorCount;

    return( TRUE );
}


u8 fileDeleteChunk( PYGMYFILEVOLUME *Volume, u64 Address )
{
    u8 Buffer[] = {0,0,0,0,0};
    // Warning! This function performs no sanity checks, it simply deletes!
    //print( COM3, "\rDeleting sector @ 0x%012llX", Address );
    Volume->IO->PutBuffer( Volume->Port, Address, Buffer, 5 );
}

u8 fileDeleteEntry( u32 ID, PYGMYFOLDER *ParentFolder )
{
    u64 Address;
    u32 i;
    
    if( !fileDeleteProperties( ID, ParentFolder ) ){
        return( FALSE );
    } // if
    if( !fileDeleteSectorGroups( ID, ParentFolder ) ){
        return( FALSE );
    } // if
    if( !fileDeleteLength( ID, ParentFolder ) ){
        return( FALSE );
    } // if

    return( TRUE );
}

u8 fileDeleteProperties( u32 ID, PYGMYFOLDER *ParentFolder )
{
    u64 Address;
    u32 i;

    for( i = PYGMYFILE_PROPERTIESCHUNK; i < PYGMYFILE_SGCHUNK; i++ ){
        if( !fileFindChunkAddress( ID, i, ParentFolder, &Address ) ){
            return( TRUE );
        } // else
        fileDeleteChunk( ParentFolder->Volume, Address );
    } // for

    return( FALSE );
}

u8 fileDeleteLength( u32 ID, PYGMYFOLDER *ParentFolder )
{
    u64 Address;

    if( fileFindChunkAddress( ID, PYGMYFILE_LENGTHCHUNK, ParentFolder, &Address ) ){
        if( fileDeleteChunk( ParentFolder->Volume, Address ) ){
            return( TRUE );
        } // if
    } // if
   
    return( FALSE );
}

u8 fileDeleteSectorGroups( u32 ID, PYGMYFOLDER *ParentFolder )
{
    u64 Address;
    u32 i;

    for( i = PYGMYFILE_SGCHUNK; i < PYGMYFILE_LASTSGCHUNK; i++ ){
        if( !fileFindChunkAddress( ID, i, ParentFolder, &Address ) ){
            return( TRUE );
        } // else
        fileDeleteChunk( ParentFolder->Volume, Address );
    } // for

    return( FALSE );
}

PYGMYFILE *fileOpen( u8 *FileName, u8 Attributes, u32 Length )
{
    PYGMYFILE *File;
    u64 Address;
    u32 i;
    u8 Buffer[ 21 ], *TmpName, *TmpPath, *Separator;
    
    FileName = removeLeadingWhitespace( FileName );
    TmpName = malloc( 1 + strlen( FileName ) );
    if( !TmpName ){
        return( NULL );
    } // if
    strcpy( TmpName, FileName );
    
    File = malloc( sizeof( PYGMYFILE ) );
    if( !File ){
        return( NULL );
    } // if
    //FileName = removeLeadingWhitespace( FileName );
    if( *FileName == '/' ){
        // Absolute
        Separator = strchr( TmpName + 1, '/' );
        if( !Separator ){
            return( NULL );
        }
        *Separator = '\0'; // break string to capture the volume name only
        File->Volume = fileFindVolume( TmpName + 1 ); // start past the / char
        *Separator = '/'; // replace file separator and split path into Path and Name
        Separator = strrchr( TmpName, '/' );
        if( !Separator ){
            return( NULL );
        } // if
        *Separator = '/'; // split path in to name and path
        File->Properties.Name = Separator + 1;
        File->Properties.Path = TmpName;
    } else{
        File->Volume = fileGetCurrentVolume();
        if( !File->Volume ){
            return( NULL ); // No volume mounted
        } // if
        File->Properties.Name = TmpName;
        File->Properties.Path = malloc( 1 + strlen( File->Volume->Properties.Path ) );
        if( !File->Properties.Path ){
            return( NULL );
        } // if
        strcpy( File->Properties.Path, File->Volume->Properties.Path );
    } // else
    //File->ParentFolder = fileAllocateFoldersFromPath( File->Volume, File->Properties.Path );
    File->ParentFolder = fileAllocateFoldersFromFullPath( File->Properties.Path );
    File->Properties.Length = 0;
    File->Properties.ID = pdiaEncodeString( File->Properties.Name );
    File->Properties.Attributes = Attributes & ( READ|WRITE|FOLDER );
    // Generate ID
    // We now have a volume, Path and Name, time to verify path and load entry
    // Test for an existing file
    
    if( ( Attributes & WRITE ) && !( Attributes & APPEND ) ){ 
        // Write mode, erase existing file
        if( fileFindChunk( File->Properties.ID, 0, File->ParentFolder, &Address, Buffer )){
            fileDeleteEntry( File->Properties.ID, File->ParentFolder );
        } // if
        if( !fileAllocateSectors( File->ParentFolder, 1 + ( Length / File->Volume->Desc.SectorSize ), &File->Properties ) ){
            return( NULL );
        } // if
        //File->Properties.Attributes &= ( FOLDER|READ|WRITE );
        if( !fileWriteEntry( File->ParentFolder, &File->Properties ) ){
            return( NULL );
        } // if
        File->Index = 0;
    } else if( ( Attributes & WRITE ) && ( Attributes & APPEND ) ){
        //File->Properties.Attributes = (WRITE|READ);
        if( !fileLoadEntry( File->Properties.ID, File->ParentFolder, &File->Properties ) ){
            //return( NULL );
            if( !fileAllocateSectors( File->ParentFolder, 1 + ( Length / File->Volume->Desc.SectorSize ), &File->Properties ) ){
                return( NULL );
            } // if
            //File->Properties.Attributes &= ~APPEND; //( FOLDER|READ|WRITE );
            if( !fileWriteEntry( File->ParentFolder, &File->Properties ) ){
                return( NULL );
            } // if
                
        } // if
        File->Index = File->Properties.Length;
    } else{
        // Read mode
        if( !fileLoadEntry( File->Properties.ID, File->ParentFolder, &File->Properties ) ){
            return( NULL );
        } // if
        File->Index = 0;
    } // else
    
   
    return( File );
}

PYGMYFILEVOLUME *fileFindVolume( u8 *VolumeName )
{
    // This functions searches for a 
    PYGMYFILEVOLUME *Volume;
    u32 i, ID, VolumeCount;

    ID = pdiaEncodeString( VolumeName );
    VolumeCount = fileGetVolumeCount();
    for( i = 0; i < VolumeCount; i++ ){
        Volume = fileGetVolume( i );
        if( Volume->Properties.ID == ID ){
            return( Volume );
        } // if
    } // for
    
    return( NULL );
}

PYGMYFOLDER *fileAllocateFoldersFromFullPath( u8 *Path )
{
    PYGMYFILEVOLUME *Volume;
    PYGMYFOLDER *ParentFolder, *Folder;
    u32 ID, i, FolderCount;
    u8 *FolderName, *FullPath, MaxVolumes;
    
    i = len( Path );
    FullPath = malloc( i + 2 );
    if( !FullPath ){
        return( NULL );
    } // if
    strcpy( FullPath, Path );
    if( FullPath[ i - 1 ] != '/' ){
        FullPath[ i ] = '/';
        FullPath[ i + 1 ] = '\0';
        print( COM3, "\rFullPath[ i - 1 ] != '/'" );
    } // if
    print( COM3, "\rEntering fileAllocateFoldersFromFullPath(): %s", FullPath );
    
    ParentFolder = malloc( sizeof( PYGMYFOLDER ) );
    if( !ParentFolder ){
        return( NULL );
    } // if
    Folder = malloc( sizeof( PYGMYFOLDER ) );
    if( !Folder ){
        free( ParentFolder );
        return( NULL );
    } // if
    FolderName = strtok( FullPath, "/" );
    // Get a volume
    MaxVolumes = fileGetVolumeCount( );
    //ID = pdiaEncodeString( FolderName );
    for( i = 0; i < MaxVolumes; i++ ){
        Volume = fileGetVolume( i );
        //if( Volume->Properties.ID == ID ){
        if( !strcmp( Volume->Properties.Name, FolderName ) ){
            //print( COM3, "\rFound Volume: %s", Volume->Properties.Name );
            break;
        } // if
    } // for
    if( i == MaxVolumes ){
        return( NULL ); // No matches, volume name must be invalid
    } // if
    print( COM3, "\rFound Volume: %s", Volume->Properties.Name );
    ParentFolder->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !ParentFolder->Properties.Sectors ){
        return( NULL );
    } // if
    // Manually create a base folder for the media entry point
    ParentFolder->Volume = Volume;
    fileCopyProperties( &Volume->ParentFolder->Properties, &ParentFolder->Properties );
    ParentFolder->Properties.Sectors[ 0 ].BaseAddress = Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress;
    ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    
    FolderCount = countCharInString( '/', Path ) - 1;
    if( FolderCount == 0 ){
        return( ParentFolder );
    } // if
    for( i = 0; i < FolderCount; i++ ){
        FolderName = strtok( NULL, "/" );
        if( !FolderName ){
            print( COM3, "\rFailed to allocate FolderName" );
            return( Folder );
        } // if
            // Allocate the next folder, but only if there will be another entry
            Folder = malloc( sizeof( PYGMYFOLDER ) );
            if( !Folder ){
                return( NULL );
            } // if
            Folder->Properties.Name = FolderName;
            Folder->Properties.Path = NULL;
        
        ID = pdiaEncodeString( FolderName );
        print( COM3, "\rCalling fileLoadEntry(): %s, 0x%08X", FolderName, ID );
        if( !fileLoadEntry( ID, ParentFolder, &Folder->Properties ) ){
            // We may not able to load the entry because the path contains a file
            // Attempt to load the properties and check the attributes for a file
            if( fileLoadProperties( ID, ParentFolder, &Folder->Properties ) ){
                if( !( Folder->Properties.Attributes & FOLDER ) ){
                    // The entry exists and is not a folder
                    if( i + 1 == FolderCount ){
                        // This is a file included as the end-point in the path
                        return( ParentFolder );
                    } else{
                        // This is an error
                        return( NULL );
                    } // else
                } // if
            }// if
            print( COM3, " Failed on: %s", FolderName );
            return( NULL );
            // ID doesn't exist
        }  // else
        if( !( Folder->Properties.Attributes & FOLDER ) ){
            if( i + 1 == FolderCount ){
                return( ParentFolder );
            } else{
                return( NULL );
            } // else
        } // if
        Folder->Properties.ID = ID;
        Folder->Parent = ParentFolder; // Set ParentFolder
        Folder->Volume = Volume;
        ParentFolder = Folder; // Promote Folder
    } // for
    print( COM3, "\rReturning Folder" );

    return( Folder );
}

/*PYGMYFOLDER *fileAllocateFoldersFromFullPath( u8 *Path )
{
    // Folder structures contain the parent/child relationships used by the file system to allocate sectors
    // Start at the root and allocate each folder and link
    // Return pointer to the last Folder, this can be linked to a file in this way: file->ParentFolder = Folder;
    // Return NULL if the Path is invalid
    PYGMYFILEVOLUME *Volume;
    PYGMYFOLDER *ParentFolder, *Folder;
    u32 ID, i, FolderCount;
    u8 MaxVolumes, *FullPath, *FolderName;
    
    // First allocate an initial pair of folders
    ParentFolder = malloc( sizeof( PYGMYFOLDER ) );
    Folder = malloc( sizeof( PYGMYFOLDER ) );
    if( !ParentFolder || !Folder ){
        return( NULL );
    } // if
    ParentFolder->Volume = NULL;
    ParentFolder->Properties.Name = NULL;
    ParentFolder->Properties.Path = NULL;
    ParentFolder->Parent; // Parent Folder, this allows a Volume->...->Folder->File relationship
    Folder->Volume = NULL;
    Folder->Properties.Name = NULL;
    Folder->Properties.Path = NULL;
    Folder->Parent = NULL; // Parent Folder, this allows a Volume->...->Folder->File relationship
    Path = removeLeadingWhitespace( Path );
    //print( COM3, "\rAllocating folders from path: %s", Path );
    // All paths will be created equal, start by detecting path type
    if( Path[ 0 ] != '/' ){
        // Path is relative to current path, so get and composite with current path
        FullPath = malloc( 2 + len( Path ) + len( fileGetCurrentPath() ) );
        if( !FullPath ){
            return( NULL );
        } // if
        strcpy( FullPath, fileGetCurrentPath() );
        strcat( FullPath, "/" );
        strcat( FullPath, Path );
    } else{
        FullPath = malloc( 1 + len( Path ) );
        if( !FullPath ){
            return( NULL );
        } // if
        strcpy( FullPath, Path );
    } // else
    print( COM3, "\rAllocating from Full Path: %s", FullPath );
    // Now we have a full path, time to separate volume name and step through folders
    FolderName = strtok( FullPath, "/" );
    MaxVolumes = fileGetVolumeCount( );
    //ID = pdiaEncodeString( FolderName );
    for( i = 0; i < MaxVolumes; i++ ){
        Volume = fileGetVolume( i );
        //if( Volume->Properties.ID == ID ){
        if( !strcmp( Volume->Properties.Name, FolderName ) ){
            //print( COM3, "\rFound Volume: %s", Volume->Properties.Name );
            break;
        } // if
    } // for
    if( i == MaxVolumes ){
        return( NULL ); // No matches, volume name must be invalid
    } // if
    print( COM3, "\rFound Volume: %s", Volume->Properties.Name );
    ParentFolder->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !ParentFolder->Properties.Sectors ){
        return( NULL );
    } // if
    // Manually create a base folder for the media entry point
    ParentFolder->Volume = Volume;
    fileCopyProperties( &Volume->ParentFolder->Properties, &ParentFolder->Properties );
    ParentFolder->Properties.Sectors[ 0 ].BaseAddress = Volume->ParentFolder->Properties.Sectors[ 0 ].BaseAddress;
    ParentFolder->Properties.Sectors[ 0 ].SectorCount = 1;
    print( COM3, "\rSeeking through folders" );
    FolderCount = countCharInString( '\\', FullPath );
    for( i = 0; i < PYGMYFILE_MAXFOLDERDEPTH && FolderName; i++ ){
        ID = pdiaEncodeString( FolderName );

        if( !fileLoadEntry( ID, ParentFolder, &Folder->Properties ) ){
            print( COM3, "\rfileLoadEntry() Failed on: %s", FolderName );
            return( ParentFolder );
            // ID doesn't exist
        }  // else
        if( !( Folder->Properties.Attributes & FOLDER ) ){
            if( i == FolderCount ){
                return( ParentFolder );
            } else{
                return( NULL );
            } // if
        } // if
        // ToTest: The following statement should prevent accessing a file as a folder
        //if( !( Folder->Properties.Attributes & FOLDER ) && ){
        //    //print( COM3, "\rCalling fileFreeFolder()" );
        //    //fileFreeFolder( Folder );
        //    print( COM3, "\rFreed folder, returning" );
        //    return( NULL );
        //} // if
        
        Folder->Properties.ID = ID;
        Folder->Parent = ParentFolder; // Set ParentFolder
        Folder->Volume = Volume;
        ParentFolder = Folder; // Promote Folder
        FolderName = strtok( NULL, "/" );
        if( FolderName ){
            // Allocate the next folder, but only if there will be another entry
            Folder = malloc( sizeof( PYGMYFOLDER ) );
            if( !Folder ){
                return( NULL );
            } // if
            Folder->Properties.Name = FolderName;
            Folder->Properties.Path = NULL;
        } // if
        //} // if
    } // for
    print( COM3, "\rReturning Folder" );

    return( Folder );
}*/

u8 fileTestPath( u8 *Path )
{
    // Folder structures contain the parent/child relationships used by the file system to allocate sectors
    // Start at the root and allocate each folder and link
    // Return pointer to the last Folder, this can be linked to a file in this way: file->ParentFolder = Folder;
    // Return FALSE if the Path is invalid
    PYGMYFILEVOLUME *Volume;
    PYGMYFOLDER *ParentFolder, *Folder;
    u32 ID, MaxVolumes, i;
    u8 *FullPath, *FolderName;

    Volume = NULL;
    // First allocate an initial pair of folders
    ParentFolder = malloc( sizeof( PYGMYFOLDER ) );
    Folder = malloc( sizeof( PYGMYFOLDER ) );
    if( !ParentFolder || !Folder ){
        return( FALSE );
    } // if
    Path = removeLeadingWhitespace( Path );
    // All paths will be created equal, start by detecting path type
    if( Path[ 0 ] != '/' ){
        // Path is relative to current path, so get and composite with current path
        FullPath = malloc( 2 + len( Path ) + len( fileGetCurrentPath() ) );
        if( !FullPath ){
            return( FALSE );
        } // if
        strcpy( FullPath, fileGetCurrentPath() );
        strcat( FullPath, "/" );
        strcat( FullPath, Path );
        strcat( FullPath, "/" );
    } else{
        FullPath = malloc( 1 + len( Path ) );
        if( !FullPath ){
            return( FALSE );
        } // if
        strcpy( FullPath, Path );
        strcat( FullPath, "/" );
    } // else
    // Now we have a full path, time to separate volume name and step through folders
    //print( COM3, "\rTesting Path: %s", FullPath );
    print( COM3, "\rAllocating Folders From Path: %s", FullPath );
    Folder = fileAllocateFoldersFromFullPath( FullPath );
    if( !Folder ){
        return( FALSE );
    } // if

    fileFreeFolder( Folder );
    return( TRUE );
}


u8 fileList( PYGMYFILELIST *FileList )
{
    // The return structure will only contain the filename, attributes, and length
    u32 Index;

    Index = fileFindNextID( FileList->ParentFolder, FileList->Index, &FileList->Properties );
    
    if( Index ){
        return( TRUE ); // ID will be FALSE if fileFindNexID() failed
    } // if

    return( FALSE );
}

u32 fileFindNextID( PYGMYFOLDER *ParentFolder, u32 Index, PYGMYFILEPROPERTIES *Properties )
{
    u64 Address, TmpAddress;
    u32 i, ID, ChunkCount;
    u8 Result, Buffer[ 5 ];

    //ChunkCount = ParentFolder->Properties.SectorCount * ( ParentFolder->MountPoint->Desc.SectorSize / PYGMYFILE_CHUNKLEN ); 
               
    //for( i = 0; i < ChunkCount; i++ ){
    for( i = 0; i < ParentFolder->Volume->Desc.ChunksPerSector; i++ ){
        fileGenerateDirectAddress( ParentFolder->Volume, &ParentFolder->Properties, Index, &Address );
        if( Address == PYGMYFILE_INVALIDADDRESS ){
            return( 0 );
        } // if
        Index += PYGMYFILE_CHUNKLEN; // Index to next chunk
        Address -= PYGMYFILE_SECTORMARKERLEN; // The address generated is past the SectorMarker for data sectors, decrement to correct
         // Load the first 5 bytes of the chunk, this contains the ID and the chunk number
        
        ParentFolder->Volume->IO->GetBuffer( ParentFolder->Volume->Port, Address, Buffer, 5 );
        ID = convertBufferToU32( Buffer, BIGENDIAN );
        //if( ID != 0xFFFFFFFF && ID != 0x00000000 && Buffer[ 4 ] == 0 ){
        if( ID != PYGMYFILE_ID_UNUSED && ID != PYGMYFILE_ID_ERASED && Buffer[ 4 ] == 0 ){
            // We found the chunk requested, load the properties and length and return
            
            Result = fileLoadEntry( ID, ParentFolder, Properties ); 
            
            if( Result ){
                if( !( Properties->Attributes & FOLDER ) && fileLoadLength( ID, ParentFolder, Properties ) ){
                    // Found and loaded properties and length
                    return( Index );
                } // if
                Properties->Length = 0;
 
                return( Index );
            } // if
            break; // Failed to load properties or length
        } // if
    } // for

    return( 0 );
}


/*

u8 fileFormat( PYGMYFILEVOLUME *Volume, u8 *Name )
{
    PYGMYFILEVOLUME *TmpVolume;
    PYGMYFILEADDRESS Sectors;
    u64 Addresses[] = { 32, 64, 96, 128 };
    u32 i;
    u16 MemLen;

    if( !Volume ){
        Volume = fileGetCurrentVolume();
        if( !Volume ){
            return( FALSE );
        } // if
    } // if

    Volume->IO->EraseAll( Volume->Port );
    // Allocate sector 0 manually
    Volume->IO->PutWord( Volume->Port, 0, PYGMYFILE_VOLUMESECTOR );
    Volume->IO->PutBuffer( Volume->Port, 2, globalPFATVersionTag, 12 );
    
    Volume->Properties.Name = Name;
    Volume->Properties.Length = 0;
    Volume->Properties.Path = NULL;
    Volume->Properties.SectorCount = 1;
    Volume->Properties.SectorGroups = 1;
    if( !fileAllocateSectors( Volume->ParentFolder, 1, &Volume->Properties ) ){
        // Allocation Failed 
        return( FALSE );
    } // if
   
    fileWriteEntry( Volume->ParentFolder, &Volume->Properties );

    return( TRUE );
}
*/

u64 fileFindNextEmptySector( PYGMYFILEVOLUME *Volume, u64 After )
{
    u64 Address;
    u32 i;
    u16 Data;

    if( !Volume ){
        return( 0 );
    } // if
    i = After / Volume->Desc.SectorSize ;
    if( After % Volume->Desc.SectorSize ){
        ++i;
    } // if
    if( i == 0 ){
        i = 1;
    } // if
    //print( COM3, "\rSeeking Empty Sector: i == %d, SectorSize == %d", i, Volume->Desc.SectorSize ); 
    for( Address = 0LL; i < Volume->Desc.Sectors; i++ ){
        Data = Volume->IO->GetWord( Volume->Port, Address );
        //print( COM3, "\rSectorMarker: 0x%08X @ 0x%012llX", Data, Address );
        if( Data == PYGMYFILE_UNUSEDSECTOR ){
            return( Address );
        } // else if
        Address += Volume->Desc.SectorSize;
    } // for
    // Memory full
    
    return( 0 );
}

u64 fileFindContiguousEmptySectors( PYGMYFILEVOLUME *Volume, u32 Count )
{
    u64 Address;
    u32 i, Found;
    
    for( i = 1, Found = 0; i < Volume->Desc.Sectors;  i++){
        Address = fileFindNextEmptySector( Volume, i * Volume->Desc.SectorSize );
        if( !Address ){
            Found = 0;
        } else{
            ++Found;
        } // else
        if( Found == Count ){
            return( Address );
        } // if
    } // for

    return( 0 );
}

u8 fileAllocateSectors( PYGMYFOLDER *ParentFolder, u32 RequestedSectors, PYGMYFILEPROPERTIES *Properties )
{
    // This function seeks a contiguous run of sectors
    // Since the number of groups required to allocate the requested number of sectors is
    //  unknown until allocation is finished,the function must allocate the sector groups array
    PYGMYFILEVOLUME *Volume;
    PYGMYFILEADDRESS *TmpAddress;
    u64 Address, i, ii;
    u32 Count, MemLen, SectorSize;
  
    Volume = ParentFolder->Volume;
    SectorSize = Volume->Desc.SectorSize;
    
    Properties->SectorGroups = 0;
    Properties->SectorCount = 0;
    MemLen = sizeof( PYGMYFILEADDRESS );
    Properties->Sectors = malloc( MemLen );
    if( !Properties->Sectors ){
        return( FALSE ); // Memory Full
    } // if
    // Subdivide number of requested sectors until suitable level of granularity is reached
    for( i = 0; i < RequestedSectors; ){
        Count = RequestedSectors / ( 1 + i ); // Test for
        Address = fileFindContiguousEmptySectors( Volume, Count );
        if( Address != 0 ){
            // If return is non-zero, add sector group and continue
            TmpAddress = realloc( Properties->Sectors, sizeof( PYGMYFILEADDRESS ) * ( Properties->SectorGroups + 1 ) );
            if( !TmpAddress ){
                free( Properties->Sectors );
                return( FALSE ); // Memory Full
            } // if
            Properties->Sectors = TmpAddress;
            Properties->Sectors[ Properties->SectorGroups ].BaseAddress = Address;
            Properties->Sectors[ Properties->SectorGroups ].SectorCount = Count;
    
            // Tag Sectors from this group
            for( ii = 0; ii < Count; ii++ ){
                Volume->IO->PutWord( Volume->Port, Address + (ii * SectorSize), PYGMYFILE_ACTIVESECTOR );
            } // for
            ++Properties->SectorGroups; // Increment after marking sectors to save on arithmetic operations
            Properties->SectorCount += Count;
            RequestedSectors -= Count;
        } else{
            ++i;
        } // else
    } //for
    
    return( TRUE );
}

u8 fileSetPath( PYGMYFILE *File, u8 *Path )
{

}

u32 fileGetFreeSpace( PYGMYFILEVOLUME *Volume )
{
    u32 i, Sectors;

    for( i = 0, Sectors = 0; i < Volume->Desc.Sectors; i++ ){
        if( Volume->IO->GetWord( Volume->Port, i * Volume->Desc.SectorSize ) == 0xFFFF ){
            ++Sectors;
        } // if
    } // for
    Sectors *= Volume->Desc.SectorSize ;

    return( Sectors );
}

u8 fileGetVolumeCount( void )
{
    return( globalVolumeCount );
}

PYGMYFILEVOLUME *fileGetCurrentVolume( void )
{
    return( globalVolumeCurrent );
}

PYGMYFILEVOLUME *fileGetVolume( u8 Index )
{
    return( globalVolumes[ Index ] );
}

u8 fileDelete( u8 *Name  )
{
    // This function simplifies the use of fileDeleteAdvanced()
    PYGMYFOLDER *ParentFolder;
    u8 *FullPath;
    
    Name = removeLeadingWhitespace( Name );
    if( Name[ 0 ] != '/' ){
        FullPath = malloc( 2 + len( fileGetCurrentPath() ) + len( Name ) );
        if( !FullPath ){
            return( FALSE );
        } // if
        strcpy( FullPath, fileGetCurrentPath( ) );
        strcat( FullPath, "/" );
        strcat( FullPath, Name );
    } else{
        FullPath = malloc( len( Name ) + 1 );
        if( !FullPath ){
            return( FALSE );
        } // if
        strcpy( FullPath, Name );
    } // else
    print( COM3, "\rCalling fileAllocateFoldersFromFullPath(): %s", FullPath );
    ParentFolder = fileAllocateFoldersFromFullPath( FullPath );
    if( !ParentFolder ){
        return( FALSE );
    } // if
    ParentFolder->Properties.Path = FullPath;
    print( COM3, "\rCalling fileDeleteAdvanced()" );
    fileDeleteAdvanced( ParentFolder );

    return( TRUE );
}

u8 fileDeleteAdvanced( PYGMYFOLDER *ParentFolder )
{
    // !!! WARNING !!! This function requires ParentFolder to include FullPath ( Absolute ) and Name 
    //   FullPath must include 
    // This function handles deleting entire directory structures, or folder trees
    // Name designates the "root" of the folder tree, if recursive deletion is selected, 
    //    then we must seek into all folders of all folders and the return back out, deleting
    //    the files on the way back out.  
    PYGMYFOLDER *Folder;
    u32 i, MaxFiles;
    u8 *FullPath;
    
    MaxFiles = ( ParentFolder->Properties.SectorCount * ParentFolder->Volume->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;
    
    for( i = 0; i < MaxFiles; i++ ){
        // Look to see if the folder contains files or folders
        Folder = malloc( sizeof( PYGMYFOLDER ) );
        if( !Folder ){
            break; // Memory Error
        } // if
        Folder->Volume = ParentFolder->Volume;
        if( fileFindNextID( ParentFolder, 0, &Folder->Properties ) ){
            // The folder is not empty
            if( Folder->Properties.Attributes & FOLDER ){
                // The folder contains another folder, step into and repeat
                FullPath = malloc( 2 + len( ParentFolder->Properties.Path ) + len( Folder->Properties.Name ) );
                if( !FullPath ){
                    // We have run out of RAM
                    return( FALSE );
                } // if
                Folder->Properties.ID = pdiaEncodeString( Folder->Properties.Name );
                strcpy( FullPath, ParentFolder->Properties.Path );
                strcat( FullPath, "/" );
                strcat( FullPath, Folder->Properties.Name );
                Folder->Parent = fileAllocateFoldersFromFullPath( FullPath );
                Folder->Properties.Path = FullPath;
                // Call self recursively to step through
                if( !fileDeleteAdvanced( Folder ) ){
                    break;
                } // if
                print (COM3, "\rCalling fileDeleteEntry()" );
                fileDeleteEntry( Folder->Properties.ID, Folder->Parent->Parent );
                // ToTest: Added following call to fileFreeFolder()
                fileFreeFolder( Folder );
            } else{
                Folder->Properties.ID = pdiaEncodeString( Folder->Properties.Name );
                Folder->Properties.Path = ParentFolder->Properties.Path;
                print (COM3, "\rCalling fileDeleteEntry()" );
                fileDeleteEntry( Folder->Properties.ID, ParentFolder );
            } // if
        } else{
            // There may not be an ID and the Path will have the file or folder name appended to it
            // We must first generate a valid ID and then remove the file or folder name from the path
            ParentFolder->Properties.ID = pdiaEncodeString( ParentFolder->Properties.Name );
            FullPath = strrchr( ParentFolder->Properties.Path, '/' ); // Find last entry, this will be the file or folder name
            *FullPath = '\0'; // Remove Name from Path
            print (COM3, "\rCalling fileDeleteEntry()" );
            fileDeleteEntry( ParentFolder->Properties.ID, ParentFolder->Parent );
            break;
        } // else
    } // for

    return( TRUE );
}

u8 *fileGetPathFromPath( u8 *Path )
{
    u32 i, Len;
    u8 *Params[32], *SubPath, ParamCount, Absolute;

    if( Path[ 0 ] == '/' ){
        Absolute = TRUE;
    } else{
        Absolute = FALSE;
    } // else
    Len = 1 + len( Path );
    ParamCount = getAllSubStrings( Path, Params, 32, FILESEPARATORS );
    if( !ParamCount ){
        return( NULL );
    } // if
    SubPath = malloc( Len - sizeof( Params[ ParamCount - 1 ] ) );
    if( !SubPath ){
        return( NULL );
    } // if
    
    strcpy( SubPath, "" );
    for( i = 0; i < ParamCount-1; i++ ){
        if( i || Absolute ){
            SubPath = strcat( "/", SubPath );
        } // else
        SubPath = strcat( Params[ i ], SubPath );
    } // for

    return( SubPath );
}

u8 *fileGetNameFromPath( u8 *Path )
{
    u8 *Params[32], *Name, Len;

    Len = getAllSubStrings( Path, Params, 32, FILESEPARATORS );
    if( !Len ){
        return( NULL );
    } // if
    Name = malloc( sizeof( Params[ Len - 1 ] ) );
    if( !Name ){
        return( NULL );
    } // if

    strcpy( Name, Params[ Len - 1 ] );

    return( Name );
}

u8 fileFreeSectorGroups( PYGMYFILEADDRESS *SectorGroups )
{
    // SectorGroups is a linear array of address pairs, this may change, so this function should be called for compatibility
    free( SectorGroups );

    return( TRUE );
}

u8 fileFreeFolder( PYGMYFOLDER *Folder )
{
    // A folder may contain up to PYGMY_MAXFOLDERDEPTH parent folders
    // All Parent Folders must be freed before returning
    PYGMYFOLDER *ParentFolder;
    u32 i;
    return( TRUE );
    for( i = 0; i < PYGMYFILE_MAXFOLDERDEPTH; i++ ){
        ParentFolder = Folder->Parent;
        if( ParentFolder->IsRoot ){
            // If this is the last folder, or root folder, free the current and the root
            fileFreeSectorGroups( ParentFolder->Properties.Sectors );
            if( ParentFolder->Properties.Name ){
                free( ParentFolder->Properties.Name );
            } // if
            if( ParentFolder->Properties.Path ){
                free( ParentFolder->Properties.Path );
            } // if
            if( ParentFolder ){
                free( ParentFolder );
            } // if
            if( ParentFolder->Properties.Sectors ){
                fileFreeSectorGroups( ParentFolder->Properties.Sectors );
            } // if
            if( Folder->Properties.Name ){
                free( Folder->Properties.Name );
            } // if
            if( Folder->Properties.Path ){
                free( Folder->Properties.Path );
            } // if
            if( Folder ){
                free( Folder );
            } // if
            return( TRUE );
        } // if
        // To free a folder, we must free the sector groups, name, path and finally the folder
        if(  ParentFolder->Properties.Sectors ){
            fileFreeSectorGroups( ParentFolder->Properties.Sectors );
        } // if
        if( Folder->Properties.Name ){
            free( Folder->Properties.Name );
        } // if
        if( Folder->Properties.Path ){
            free( Folder->Properties.Path );
        } // if
        if( Folder ){
            free( Folder );
        } // if
    } // for

    return( FALSE );
}

u8 fileFreeFile( PYGMYFILE *File )
{
    free( File->Properties.Path );
    free( File->Properties.Name );
    fileFreeFolder( File->ParentFolder );
    fileFreeSectorGroups( File->Properties.Sectors );

    return( TRUE );
}

u8 fileClose( PYGMYFILE *File )
{
    if( File->Properties.Attributes & WRITE ){
        // Only write length if the file has write access
        fileFlush( File );
    } // if
    if( File->Properties.Sectors ){
        free( File->Properties.Sectors );
    } // if
    if( File->Properties.Name ){
        free( File->Properties.Name );
    } // if
    if( File ){
        free( File );
    } // if

    return( TRUE );
}

u8 fileFlush( PYGMYFILE *File )
{
    // ToDo: Add code to handle writing of resource buffer, if applicable
    fileUpdateLength( File->ParentFolder, &File->Properties );

    return( TRUE );
}

void filePrintProperties( PYGMYFILEPROPERTIES *Properties )
{
    u32 i, ii;

    print( COM3, "\rID: 0x%08X", Properties->ID );
    print( COM3, "\rTime: %t", Properties->Time );
    print( COM3, "\rLength: %d", Properties->Length );
    print( COM3, "\rName: %s", Properties->Name );
    print( COM3, "\rPath: %s", Properties->Path );
    print( COM3, "\rAttributes:" );
    if( Properties->Attributes & READ ){
        print( COM3, " READ" ); 
    } //if
    if( Properties->Attributes & WRITE ){
        print( COM3, " WRITE" );
    } // if 
    if( Properties->Attributes & FOLDER ){
        print( COM3, " FOLDER" );
    } // if
    if( Properties->Attributes & APPEND ){
        print( COM3, " APPEND" );
    } // else if
    print( COM3, "\rSectorGroups: %d", Properties->SectorGroups );
    print( COM3, "\rSectorCount: %d", Properties->SectorCount );
    for( i = 0; i < Properties->SectorGroups; i++ ){
        print( COM3, "\r\tBaseAddress: 0x%012llX", Properties->Sectors[ i ].BaseAddress );
        print( COM3, "\r\tSectorCount: %d", Properties->Sectors[ i ].SectorCount );
    } // for
}

u8 *fileCreateNameWithPath( u8 *Name )
{
    PYGMYFILEVOLUME *Volume;
    u8 *NameWithPath;

    if( Name[ 0 ] == '/' ){
        print( COM3, "\rAlready absolute" );
        return( Name );
    } // if
    Volume = fileGetCurrentVolume();
    if( !Volume ){
        return( NULL );
    } // if
    NameWithPath = malloc( len( Volume->Properties.Path ) + len( Name ) + 2 );
    if( !NameWithPath ){
        return( NULL );
    } // if
    
    strcpy( NameWithPath, Volume->Properties.Path );
    strcat( NameWithPath, "/" );
    Name = removeLeadingWhitespace( Name );
    strcat( NameWithPath, Name );
    print( COM3, "\rCreated NameWithPath: %s", NameWithPath );

    return( NameWithPath );
}

u8 *fileMakeFullPath( u8 *Path )
{
    PYGMYFOLDER *ParentFolder;
    u8 *FullPath;
    
    Path = removeLeadingWhitespace( Path );
    if( Path[ 0 ] != '/' ){
        FullPath = malloc( 2 + len( fileGetCurrentPath() ) + len( Path ) );
        if( !FullPath ){
            return( NULL );
        } // if
        strcpy( FullPath, fileGetCurrentPath( ) );
        strcat( FullPath, "/" );
        strcat( FullPath, Path );
    } else{
        FullPath = malloc( len( Path ) + 1 );
        if( !FullPath ){
            return( NULL );
        } // if
        strcpy( FullPath, Path );
    } // else

    return( FullPath );
}

u8 fileRename( u8 *Name, u8 *NewName )
{
    PYGMYFILE *File;
    PYGMYFILEPROPERTIES Properties;
    u8 *FullPathName;//, *FullPathNewName;
    
    print( COM3, "\rEntering fileRename: %s, %s", Name, NewName );
    FullPathName = fileMakeFullPath( Name );
    //FullPathNewName = fileMakeFullPath( NewName );
    NewName = fileGetNameFromPath( NewName );
    if( !NewName ){
        return( FALSE );
    } // if
    File = fileOpen( FullPathName, READ, 0 );
    if( !File ){
        free( FullPathName );
        return( FALSE );
    } // if
    //filePrintProperties( &File->Properties );
    fileCopyProperties( &File->Properties, &Properties );
    if( !fileDeleteEntry( File->Properties.ID, File->ParentFolder ) ){
        return( FALSE );
    } // if
    
    //filePrintProperties( &File->Properties );
    free( File->Properties.Name );
    File->Properties.Name = malloc( strlen( NewName ) + 1 );
    if( !File->Properties.Name ){
        free( FullPathName );
        return( FALSE );
    } // if
   
    strcpy( File->Properties.Name, NewName );
    //File->Properties.Name = NewNameAndPath;
    File->Properties.ID = pdiaEncodeString( File->Properties.Name );
    //filePrintProperties( &File->Properties );

    if( !fileWriteEntry( File->ParentFolder, &File->Properties ) ){
        free( FullPathName );
        return( FALSE );
    } // if 
    fileClose( File );
    free( FullPathName );

    return( TRUE );
}

u8 fileCopy( u8 *FileNameFrom, u8 *FileNameTo )
{
    PYGMYFILE *FileFrom, *FileTo;
    u32 i, ii, len;
    u8 TmpChar, Buffer[ 128 ];

    FileFrom = fileOpen( FileNameFrom, READ, 0 );
    if( !FileFrom ){
        return( FALSE );
    } // if
    FileTo = fileOpen( FileNameTo, WRITE, FileFrom->Properties.Length );
    if( !FileTo ){
        fileClose( FileFrom );
        return( FALSE );
    } // if
    //print( COM3, "\rFileFrom:" );
    filePrintProperties( &FileFrom->Properties );
    //print( COM3, "\rFileTo:" );
    //filePrintProperties( &FileTo->Properties );
    for( i = 0; i < FileFrom->Properties.Length; ){
        len = fileReadBytes( FileFrom, Buffer, 128 );
        /*if( isPrintable( TmpChar ) ){
            print( COM3, "%c", TmpChar );
        } else{
            print( COM3, "(%02X)", TmpChar );
        } // else
        */
        fileWriteBytes( FileTo, Buffer, len );
        /*print( COM3, "\rReading Buffer: " );
        len = fileReadBytes( FileFrom, Buffer, 128 );
        for( ii = 0; ii < len; ii++ ){
            if( isPrintable( Buffer[ ii ] ) ){
                print( COM3, "%c", Buffer[ ii ] );
            } else{
                print( COM3, "(%02X)", Buffer[ ii ] );
            } // else
        } // for
        fileWriteBytes( FileTo, Buffer, len );
        i += len;*/
        i += len;
        if( len == 0 ){
            break;
        } // if
    } // for

    fileClose( FileFrom );
    fileClose( FileTo );

    return( TRUE );
}

u8 fileGetChar( PYGMYFILE *pygmyFile )
{
    u8 ucChar;

    fileReadBytes( pygmyFile, &ucChar, 1 );

    return( ucChar );
}

u8 fileGetBuffer( PYGMYFILE *File, u8 *Buffer, u32 Len )
{
    fileReadBytes( File, Buffer, Len );

    return( TRUE );
}

u8 *fileGetString( PYGMYFILE *File )
{
    u32 i;
    u8 *Buffer, TmpChar;
    
    Buffer = malloc( 2 );
    if( !Buffer ){
        return( NULL ); // 
    } // if
    
    for( i = 0; i < 1024 && !( File->Properties.Attributes & EOF ); i++ ){
        if( !fileReadBytes( File, &TmpChar, 1 ) ){
            // zero bytes remaining
            break;
        } // if
        
        if( isPrintable( TmpChar ) ){
            Buffer = realloc( Buffer, i + 2 );
            if( !Buffer ){
                return( NULL ); // memory error
            } // if
            Buffer[ i ] = TmpChar;
        } else{
            break;
        } // else
    } // for
    Buffer[ i ] = '\0';

    return( Buffer );
}

u32 fileReadBytes( PYGMYFILE *File, u8 *Buffer, u32 Len )
{
    u64 Address;
    u32 i, ii, BufLen;

    // The filesystem must mark the Volume as busy during a transaction to prevent the resource from being accessed in the middle of an operation
    if( File->Volume->Status & (PYGMYFILE_FULL|PYGMYFILE_LOCKED|PYGMYFILE_BUSY) ){
        //print( COM3, "\rError: fileReadBytes()->FileBusy" );
        return( FALSE );
    } // if
    File->Volume->Status |= PYGMYFILE_BUSY;
    if( ( File->Properties.Length - File->Index ) < Len ){
        // There aren't len bytes left in the file
        Len = File->Properties.Length - File->Index;
        File->Properties.Attributes &= EOF;
    } // if
    for( i = 0; i < Len; ){
        BufLen = fileGenerateDirectAddress( File->Volume, &File->Properties, File->Index, &Address ); // BufLen is the number of bytes remaining in the current sector group
        if( ( Len - i ) <= BufLen ){
            BufLen = Len - i;
        } // if
        if( Address == PYGMYFILE_INVALIDADDRESS){
            return( FALSE ); // We attempted to seek past the allocated sectors
        } // if
        File->Volume->IO->GetBuffer( File->Volume->Port, Address, Buffer + i, BufLen );
        //Buffer[ i ] = File->Volume->IO->GetChar( File->Volume->Port, Address );
        File->Index += BufLen;
        i += BufLen;
    } // for
    File->Volume->Status &= ~PYGMYFILE_BUSY; // Clear busy flag before returning
    if( File->Index > File->Properties.Length ){
        // End of File Reached
        File->Properties.Attributes &= EOF;
    } // if

    return( Len ); // return the number of bytes read
}

u8 fileWriteBytes( PYGMYFILE *File, u8 *Buffer, u32 Len )
{
    // This function is only for internal use!!!
    // Use filePut functions in user code, fileWriteBytes is optimized for multi-byte write oeprations
    u64 Address;
    u32 i, BufLen;
    
    if( File->Volume->Status & (PYGMYFILE_FULL|PYGMYFILE_LOCKED|PYGMYFILE_BUSY) ){
        return( FALSE );
    } // if
    File->Volume->Status |= PYGMYFILE_BUSY;
    if( ( File->Properties.Length + Len ) > ( ( File->Volume->Desc.SectorSize - PYGMYFILE_SECTORMARKERLEN ) * File->Properties.SectorCount ) ){
        if( !fileAppendSectors( File->ParentFolder, 1, &File->Properties ) ){
            return( FALSE );
         } // else
    } // if
    for( i = 0; i < Len; ){
        // Calculate parameters for buffer write to flash
        BufLen = fileGenerateDirectAddress( File->Volume, &File->Properties, File->Index, &Address ); 
       // if( ! ){
        /*if( File->Volume->IO->GetWord( File->Volume->Port, ( Address / 4096 ) * 4096 ) == 0 ){
            print( COM3, "\rWriting to Erased Sector @ 0x%012llX", Address );
        } // if
        */
        //} // if
        if( Address < 4096 ){
            
            /*for( i = 0; i < File->Properties.SectorGroups; i++ ){
                print( COM3, "\r\tBaseAddress: 0x%012llX", File->Properties.Sectors[ i ].BaseAddress );
                print( COM3, "\r\tSectorCount: %d", File->Properties.Sectors[ i ].SectorCount );
            } // for
            */
            return( FALSE );
        } // if
        if( Address == PYGMYFILE_INVALIDADDRESS){
            File->Volume->Status &= ~PYGMYFILE_BUSY;
            return( FALSE );
        } // if
        if( BufLen + i > Len ){
            BufLen = Len - i;
        } // if
        //print( COM3, "\rfileWriteBytes()->Address: 0x%012llX", Address );
        File->Volume->IO->PutBuffer( File->Volume->Port, Address, Buffer+i, BufLen );
        File->Index += BufLen;
        File->Properties.Length += BufLen;
        i += BufLen;
        
        //if( BufLen == 0 ){
        
    } // for

    File->Volume->Status &= ~PYGMYFILE_BUSY;

    return( TRUE );
}

u8 filePutChar( PYGMYFILE *File, u8 Data )
{
    // Immutable means immutable! Flash files do not support random write access

    // Do not allow write within written space of Flash, allow RAM devices to continue
    if( ( File->Volume->Desc.Attributes & MEDIAFLASH ) && File->Index != File->Properties.Length ){
        return( FALSE );
    } // if
    // Check that there is space in the current sectors before writing
    
    return( fileWriteBytes( File, &Data, 1 ) );
}

u16 fileGetWord( PYGMYFILE *File, u8 Endian )
{
    u8 Buffer[ 2 ];
    
    fileReadBytes( File, Buffer, 2 );

    return( convertBufferToU16( Buffer, Endian ) );
}

u8 filePutWord( PYGMYFILE *File, u16 Data, u8 Endian )
{
    u8 Buffer[ 2 ];

    convertU16ToBuffer( Data, Buffer, Endian );

    return( fileWriteBytes( File, Buffer, 2 ) );
}

u32 fileGetLong( PYGMYFILE *File, u8 Endian )
{
    u8 Buffer[ 4 ];
    
    fileReadBytes( File, Buffer, 4 );

    return( convertBufferToU32( Buffer, Endian ) );
}

u8 filePutLong( PYGMYFILE *File, u32 Data, u8 Endian )
{
    u8 Buffer[ 4 ];

    convertU32ToBuffer( Data, Buffer, Endian );

    return( fileWriteBytes( File, Buffer, 4 ) );
}

u8 filePutBuffer( PYGMYFILE *File, u8 *Buffer, u32 Len )
{
    u8 Result;

    pinSet( LED0, 1 );
    Result = fileWriteBytes( File, Buffer, Len );
    pinSet( LED0, 0 );
    return( Result );
}

u8 fileSeek( PYGMYFILE *File, u8 Origin, s32 Index )
{
    if( Origin == START ){ 
        if( Index >= 0 && Index < File->Properties.Length ){
            File->Index = Index;
            return( 1 );
        } // if
    } else if( Origin == CURRENT ){
        if( ( File->Index + Index ) >= 0 && ( File->Index + Index ) < File->Properties.Length ){
            File->Index += Index;
            return( 1 );
        } // if
    } else if( Origin == END ){
        if( ( File->Properties.Length + Index ) >= 0 && ( File->Properties.Length + Index ) < File->Properties.Length ){
                File->Properties.Length += Index;
            return( TRUE );
        } // if
    } // else
    
    return( FALSE );
}

u8 *fileFind( PYGMYFILEVOLUME *Volume, u8 *FileName )
{
    // This function searches for a file in the Volume specified
    //ParentFolder = fileAllocateFoldersFromFullPath( fileGetCurrentPath() );
    PYGMYFILEPROPERTIES Properties;
    PYGMYFOLDER *ParentFolder, *Folder;
    u32 i, MaxFiles, Index;
    u8* FullPath;

    print( COM3, "\rFind: %s", FileName );
    FileName = removeLeadingWhitespace( FileName );
    FullPath = malloc( 2 + len( fileGetCurrentPath() ) + len( FileName ) );
    if( !FullPath ){
        print( COM3, "\rMemory allocation error" );
    } // if
    strcpy( FullPath, fileGetCurrentPath( ) );
    strcat( FullPath, "/" );
    strcat( FullPath, FileName );
    print( COM3, "\rCalling fileAllocateFoldersFromFullPath(): %s", FullPath );
    ParentFolder = fileAllocateFoldersFromFullPath( FullPath );
    if( !ParentFolder ){
        print( COM3, "\rFailed to allocate folders" );
        return( NULL );
    } // if
    ParentFolder->Properties.Path = FullPath;

    //ParentFolder = fileAllocateFoldersFromFullPath( Volume->Properties.Path );
    Index = 0;
    MaxFiles = 50; //( ParentFolder->Properties.SectorCount * ParentFolder->Volume->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;

    print( COM3, "\rSearching at: %s", ParentFolder->Properties.Path );

    for( i = 0; i < MaxFiles; i++ ){
        // Look to see if the folder contains files or folders
        Folder = malloc( sizeof( PYGMYFOLDER ) );
        if( !Folder ){
            break; // Memory Error
        } // if
        Folder->Volume = ParentFolder->Volume;
        Index = fileFindNextID( ParentFolder, Index, &Properties );
        if( !Index ){
            break;
        } // if
        
        print( COM3, "\rChecking %d: 0x%08X, %s", Index, Properties.ID, Properties.Name );
        if( Properties.ID != 0xFFFFFFFF && Properties.Attributes & FOLDER ){
            print( COM3, "\rFound Folder" );
            // The folder contains another folder, step into and repeat
            FullPath = malloc( 2 + len( ParentFolder->Properties.Path ) + len( Folder->Properties.Name ) );
            if( !FullPath ){
                // We have run out of RAM
                return( FALSE );
            } // if
            Folder->Properties.ID = pdiaEncodeString( Folder->Properties.Name );
            strcpy( FullPath, ParentFolder->Properties.Path );
            strcat( FullPath, "/" );
            strcat( FullPath, Folder->Properties.Name );
            print( COM3, "\rCalling fileAllocateFoldersFromFullPath() with: %s", FullPath );
            Folder->Parent = fileAllocateFoldersFromFullPath( FullPath );
            Folder->Properties.Path = FullPath;
            print( COM3, "\rCalling fileFind() with: %s", FullPath );
            // Call self recursively to step through
            if( fileFind( Volume, FileName ) ){
                break;
            } // if
            print( COM3, "\rFound %s @ %s", FileName, Volume->Properties.Path );
            // ToTest: Added following call to fileFreeFolder()
            fileFreeFolder( Folder );
        } else{
            // print filename, file length, and size of file on media
        //if( !Properties.Attributes & FOLDER ){
            if( !strcmp( FileName, Properties.Name ) ){
                print( COM3, "\rFound %s @ %s", FileName, Volume->Properties.Path );
                //print( COM3, "\r%t %10d %10d %s", Properties.Time, Properties.Length, Properties.SectorCount * Volume->Desc.SectorSize, Properties.Name );
            } // if
        } // if
     } // for

    /*PYGMYFOLDER *Folder;
    u32 i, MaxFiles;
    u8 *FullPath;
    
    MaxFiles = ( ParentFolder->Properties.SectorCount * ParentFolder->Volume->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;
    
    for( i = 0; i < MaxFiles; i++ ){
        // Look to see if the folder contains files or folders
        Folder = malloc( sizeof( PYGMYFOLDER ) );
        if( !Folder ){
            break; // Memory Error
        } // if
        Folder->Volume = ParentFolder->Volume;
        if( fileFindNextID( ParentFolder, 0, &Folder->Properties ) ){
            // The folder is not empty
            if( Folder->Properties.Attributes & FOLDER ){
                // The folder contains another folder, step into and repeat
                FullPath = malloc( 2 + len( ParentFolder->Properties.Path ) + len( Folder->Properties.Name ) );
                if( !FullPath ){
                    // We have run out of RAM
                    return( FALSE );
                } // if
                Folder->Properties.ID = pdiaEncodeString( Folder->Properties.Name );
                strcpy( FullPath, ParentFolder->Properties.Path );
                strcat( FullPath, "/" );
                strcat( FullPath, Folder->Properties.Name );
                Folder->Parent = fileAllocateFoldersFromFullPath( FullPath );
                Folder->Properties.Path = FullPath;
                // Call self recursively to step through
                if( !fileDeleteAdvanced( Folder ) ){
                    break;
                } // if
                fileDeleteEntry( Folder->Properties.ID, Folder->Parent->Parent );
                // ToTest: Added following call to fileFreeFolder()
                fileFreeFolder( Folder );
            } else{
                Folder->Properties.ID = pdiaEncodeString( Folder->Properties.Name );
                Folder->Properties.Path = ParentFolder->Properties.Path;
                fileDeleteEntry( Folder->Properties.ID, ParentFolder );
            } // if
        } else{
            // There may not be an ID and the Path will have the file or folder name appended to it
            // We must first generate a valid ID and then remove the file or folder name from the path
            ParentFolder->Properties.ID = pdiaEncodeString( ParentFolder->Properties.Name );
            FullPath = strrchr( ParentFolder->Properties.Path, '/' ); // Find last entry, this will be the file or folder name
            *FullPath = '\0'; // Remove Name from Path
            fileDeleteEntry( ParentFolder->Properties.ID, ParentFolder->Parent );
            break;
        } // else
    } // for

    return(  );*/
}

u8 fileEOF( PYGMYFILE *File )
{
    if( File->Properties.Attributes & EOF ){
        return( TRUE );
    } // if
    if( File->Index == File->Properties.Length ){
        return( TRUE );
    } // if

    return( FALSE );
}

