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

PYGMYFILEVOLUME *globalMountPointCurrent = NULL, *globalMountPoints = NULL;
u8 globalMountPointCount = 0;

u8 fileChangeCurrentPath( u8 *ucBuffer )
{
    // Case1: Path is root
    // Case2: Path is .., roll back on folder
    // Case3: Path is absolute, seek path
    // Case4: Path is relative, add path to current path and seek path
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILEPROPERTIES pygmyEntry;
    u16 i, uiPathLen, uiMemLen;
    u8 *ucPath, ucPathBool, ucParamCount, *ucParams[ 32 ];

    uiPathLen = len( ucBuffer );

    if( isStringSame( ucBuffer, "/" ) ){
        // Case 1: Path is root
        print( COM3, "\rChange Path Case1" );
        fileSetCurrentPath( NULL );
        /*ucPath = malloc( 2 + len( pygmyVolume->Properties.Name ) );
        if( ucPath ){
            copyString( "/", ucPath );
            appendString( pygmyVolume->Properties.Name, ucPath );
            fileSetCurrentPath( ucPath );
            free( ucPath );
        } // if*/
        fileSetCurrentMountPoint( NULL );
        return( TRUE );
    } else if( isStringSame( ucBuffer, ".." ) ){
        // Case2: Path is special case "..", roll back one folder
        print( COM3, "\rChange Path Case2" );
        pygmyVolume = fileGetCurrentMountPoint( );
        if( pygmyVolume ){
            if( len( pygmyVolume->Properties.Path ) > 1 + ( len( pygmyVolume->Properties.Name ) ) ){
                // Do not modify path if already at root
                // otherwise, clip off last path segment and test for root condition
                replaceLastChar( pygmyVolume->Properties.Path, '/', '\0' );
                if( len( pygmyVolume->Properties.Path ) < 1 + ( len( pygmyVolume->Properties.Name ) ) ){
                    print( COM3, "\rReached root" );
                    fileSetCurrentMountPoint( NULL );
                } //
            } else{
                // We are at volume segment in the path, going down one level further reaches root
                if( len( pygmyVolume->Properties.Path ) == 1 + ( len( pygmyVolume->Properties.Name ) ) ){
                    print( COM3, "\rReached root" );
                    fileSetCurrentMountPoint( NULL );
                } //
            } // else
        } // if

        return( TRUE );
    } else if( *ucBuffer == '/' ){
        // Case3: Path is absolute, seek path
        print( COM3, "\rChange Path Case3" );
        // First seek path to validate path exists and to retrieve correct mount point
        ucPathBool = fileSeekPath( ucBuffer, &pygmyVolume, &pygmyEntry );
        
        fileSetCurrentMountPoint( pygmyVolume );
        if( !ucPathBool ){
            return( FALSE );
        } // if
        fileSetCurrentPath( ucBuffer );

        return( TRUE );
    } else{
        // Case4: Path is relative,  add path to current path and seek path
        print( COM3, "\rChange Path Case4" );
        pygmyVolume = fileGetCurrentMountPoint();
        if( pygmyVolume ){
            // We are currently somewhere in a path
            uiMemLen = 2 + len( ucBuffer ) + len( pygmyVolume->Properties.Path );
            ucPath = malloc( uiMemLen );
            memset( ucPath, 0, uiMemLen );
            if( ucPath ){
                copyString( pygmyVolume->Properties.Path, ucPath );
                appendString( "/", ucPath );
                appendString( ucBuffer, ucPath );
            } // if
        } else{
            // We are currently at root
            uiMemLen = 2 + len( ucBuffer );
            ucPath = malloc( uiMemLen );
            if( ucPath ){
                copyString( "/", ucPath );
                appendString( ucBuffer, ucPath );
            } // if
        } // else
        if( ucPath ){
            if( fileSeekPath( ucPath, &pygmyVolume, &pygmyEntry ) ){
                fileSetCurrentMountPoint( pygmyVolume );
                fileSetCurrentPath( ucPath );
            } // if
            free( ucPath );
        } // if
        return( TRUE );
    } // else

    return( FALSE );
}

u8 fileSetCurrentPath( u8 *ucBuffer )
{
    PYGMYFILEVOLUME *pygmyVolume;
    u16 uiMemLen;

    pygmyVolume = fileGetCurrentMountPoint();
    if( !pygmyVolume ){
        print( COM3, "\rNo MountPoint for setpath!" );
        return( FALSE );
    } // if
    if( pygmyVolume->Properties.Path ){
        print( COM3, "\rFreeing path..." );
        free( pygmyVolume->Properties.Path );
        print( COM3, "\rDone" );
    } // if
    if( !ucBuffer ){
        // Default mode used for reseting the path to "/volumename"
        uiMemLen = 2 + len ( pygmyVolume->Properties.Name );
        pygmyVolume->Properties.Path = malloc( uiMemLen );
        memset( pygmyVolume->Properties.Path, 0, uiMemLen );
        if( !pygmyVolume->Properties.Path || !pygmyVolume->Properties.Name ){
            print( COM3, "\rPath allocation failed" );
            return( FALSE );
        } // is
        copyString( "/", pygmyVolume->Properties.Path );
        appendString( pygmyVolume->Properties.Name, pygmyVolume->Properties.Path );
    } else{
        uiMemLen = 1 + len ( ucBuffer );
        pygmyVolume->Properties.Path = malloc( uiMemLen );
        memset( pygmyVolume->Properties.Path, 0, uiMemLen );
        if( !pygmyVolume->Properties.Path ){
            print( COM3, "\rPath allocation failed" );
            return( FALSE );
        } // is
        print( COM3, "\rfileSetCurrentPath(%s)", ucBuffer );
        copyString( ucBuffer, pygmyVolume->Properties.Path );
    } // else

    return( TRUE );
}

u8 *fileGetCurrentPath( void )
{
    // This function handles compositing the current path used by the command line
    //    and relative file access

    if( globalMountPointCurrent ){
        if( globalMountPointCurrent->Properties.Path ){
            return( globalMountPointCurrent->Properties.Path );
        } else{
            return( "/" );
        } // else
    } else{
        return( "/" );
    } // else
}

u8 fileSetCurrentMountPoint( PYGMYFILEVOLUME *pygmyVolume )
{
    globalMountPointCurrent = pygmyVolume;

    return( TRUE );
}

u8 fileMount( PYGMYMEMIO *pygmyMEMIO, u8 ulAddress, u8 *ucName, ... )
{
    // ulAddress is used as device address for hardware abstraction in
    //      this function only
    PYGMYFILEVOLUME *pygmyVolume;
    va_list vaList;
    u16 uiMemLen;

    if( globalMountPointCount == 0 ){
        uiMemLen = sizeof( PYGMYFILEVOLUME );
        pygmyVolume = malloc( uiMemLen );
        memset( pygmyVolume, 0, uiMemLen );
    } else{
        pygmyVolume = realloc( globalMountPoints, sizeof( PYGMYFILEVOLUME ) * ( globalMountPointCount + 1 ) );
    } // else
    if( pygmyVolume ){
        globalMountPoints = pygmyVolume;
        pygmyVolume = &globalMountPoints[ globalMountPointCount ];
        va_start( vaList, ucName );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rOpening Port" );
        #endif
        pygmyMEMIO->VOpen( &pygmyVolume->Port, ulAddress, vaList );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "..." );
        #endif
        va_end( vaList );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rva_end" );
        #endif
        pygmyVolume->IO = pygmyMEMIO;
        pygmyMEMIO->Desc( pygmyVolume->Port, &pygmyVolume->Desc );

        pygmyVolume->Properties.ID = pdiaEncodeString( ucName );
        uiMemLen = sizeof( PYGMYFILEADDRESS );
        pygmyVolume->Properties.Sectors = malloc( uiMemLen );
        memset( pygmyVolume->Properties.Sectors, 0, uiMemLen );
        if( !pygmyVolume->Properties.Sectors ){
            return( FALSE );
        } // if
        pygmyVolume->Properties.Sectors[ 0 ].BaseAddress = 0;
        pygmyVolume->Properties.Sectors[ 0 ].SectorCount = 1;
        pygmyVolume->Properties.SectorGroups = 1;
        pygmyVolume->Properties.SectorCount = 1;
        pygmyVolume->Properties.Length = 0;

        //fileFormat( pygmyVolume, "nebula" );
        //pygmyVolume->Properties.Sectors[ 0 ].BaseAddress = 0;
        //pygmyVolume->Properties.Sectors[ 0 ].SectorCount = 1;
        //pygmyVolume->Properties.SectorGroups = 1;
        //pygmyVolume->Properties.SectorCount = 1;
        //pygmyVolume->Properties.Length = 0;
        if( fileLoadEntry( pygmyVolume, &pygmyVolume->Properties ) ){
            //filePrintProperties( &pygmyVolume->Properties );
        } else{
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rFailed to Mount Volume" );
            #endif
        } // else

        ++globalMountPointCount;
        if( !globalMountPointCurrent ){
            globalMountPointCurrent = pygmyVolume;
        } // if
        fileSetCurrentPath( NULL );

        return( TRUE );
    } // if

    return( FALSE );
}

void fileCopyProperties( PYGMYFILEPROPERTIES *fromEntry, PYGMYFILEPROPERTIES *toEntry )
{
    toEntry->Sectors      = fromEntry->Sectors;
    toEntry->SectorGroups = fromEntry->SectorGroups;
    toEntry->SectorCount  = fromEntry->SectorCount;
    toEntry->Time         = fromEntry->Time;
    toEntry->ID           = fromEntry->ID;
    toEntry->Length       = fromEntry->Length;
    toEntry->Path         = fromEntry->Path;
    toEntry->Name         = fromEntry->Name;
    toEntry->Attributes   = fromEntry->Attributes;
}

void filePrintProperties( PYGMYFILEPROPERTIES *pygmyEntry )
{
    u16 i;  

    print( COM3, "\rFileEntry\r\tPath: %s", pygmyEntry->Path );
    print( COM3, "\r\tName: %s", pygmyEntry->Name );
    print( COM3, "\r\tAttrib: %d", pygmyEntry->Attributes );
    print( COM3, "\r\tID: 0x%08X", pygmyEntry->ID );
    print( COM3, "\r\tTime: %t", pygmyEntry->Time );
    print( COM3, "\r\tLength: %d", pygmyEntry->Length );
    print( COM3, "\r\tSectorGroups: %d", pygmyEntry->SectorGroups );
    print( COM3, "\r\tTotalSectors: %d", pygmyEntry->SectorCount );
    for( i = 0; i < pygmyEntry->SectorGroups && i < 10; i++ ){
        print( COM3, "\r\t\tSectors: %d BaseAddress: 0x%012llX", i, pygmyEntry->Sectors[ i ].SectorCount, pygmyEntry->Sectors[ i ].BaseAddress );
    } // if
}

u16 fileGenerateDirectAddress( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILESECTORGROUPS *pygmySG, u32 ulIndex, u64 *ullAddress )
{
    // This function decodes base-address and count pairs and calculates a physical
    //  address off this information and the index
    // The number of bytes left in the current sector is returned, ullAddress is loaded with address
    u32 i, ulGroup, ulSectors;
    u16 uiOffset, uiBufLen;

    ulSectors = ulIndex / ( pygmyVolume->Desc.SectorSize - 2 );
    if( ulSectors > pygmySG->SectorCount  ) { //|| pygmyFile->Index > pygmyFile->Properties.Length ){
        *ullAddress = INVALIDADDRESS;
        return( FALSE );
    } // if
    for( i = 0, ulGroup = 0; i < ulSectors && ulGroup < pygmySG->SectorGroups; ulGroup++ ){
        if( i + pygmySG->Sectors[ ulGroup ].SectorCount > ulSectors ){
            break;
        } // if
        i += pygmySG->Sectors[ ulGroup ].SectorCount;
    } // for
   
    uiOffset = ( ( ulIndex + ( ( 1 + ulSectors ) * 2 ) ) );
    uiBufLen = ( pygmyVolume->Desc.SectorSize ) - uiOffset;
    
    *ullAddress = pygmySG->Sectors[ ulGroup ].BaseAddress + (( ulSectors - i ) *
        pygmyVolume->Desc.SectorSize ) + uiOffset;
    
    return( uiBufLen ); // Return number of bytes remaining in current sector
}

u8 fileAllocateChunks( PYGMYFILE *pygmyFile, PYGMYFILESECTORGROUPS *pygmyParentSG, u8 ucChunks, u64 **ullAddresses )
{
    // !!! Understanding the intended operation of this function is critical !!!
    // fileAllocateChunks() will allocate chunks by finding a run ( if possible ) and writing the ID
    // Since this functions doesn't know what chunk range the chunks are being requested for, the chunk field will be left blank
    // It is important to note that if there is insufficient space in the current ParentSG, an additional sector will be allocated and 
    //    inserted into pygmyParentSG. If this sector is not contiguous with a SectorGroup contained within the ParentSG, an additional
    //    SectorGroup will be allocated and appended to the array. This means that the pointer returned should always replace the pointer
    //    passed. If the pointer is NULL, the operation failed.
    // ullAddressess will be allocated by fileAllcoateChunks and will contain pointers to every chunk allocated.
    // !!! This function does not modify pygmyFile !!!
    u64 ullTmpAddress;
    u32 i;

    *ullAddresses = malloc( sizeof( u64 ) * ucChunks );
    if( !(*ullAddresses) ){
        print( COM3, "\rNot enough memory to complete operation" );

        return( NULL );
    } // if
    // First, check for available contiguous chunks
    if( fileFindContiguousChunks( pygmyFile, pygmyParentSG, ucChunks, &ullTmpAddress ) ){
        // We have chunks, if ullTmpAddress is set to PYGMYFILE_INVALIDADDRESS we must recycle ParentSG
        if( ullTmpAddress == PYGMYFILE_INVALIDADDRESS ){
            fileRecyleParentChunks( pygmyFile );
            return( fileAllocateChunks( pygmyFile, pygmyParentSG, ucChunks, &(*ullAddresses) ) ); // recursive call
        } else{
            for( i = 0; i < ucChunks; i++ ){
                // generate addresses for each chunk
                *ullAddresses[ i ] = ullTmpAddress + ( i * PYGMYFILE_CHUNKLEN );
                // Write ID to each chunk to reserve it
                pygmyFile->MountPoint->IO->PutLong( &pygmyFile->MountPoint->Port, *ullAddresses[ i ], ulID );
            } // for
            return( TRUE );
        } // else
    } // if
    // If we dropped through to this point, we have insufficient space in the currently allocated parent sector groups
    if( fileAppendSectors( &pygmyFile, 1, PYGMYFILE_PARENT ) ){ // append to parent
            return( fileAllocateChunks( pygmyFile, pygmyParentSG, ucChunks, &(*ullAddresses) ) );
    } // if

    return( FALSE );
}

u8 fileFindContiguousChunks( PYGMYFILE *pygmyFile, PYGMYFILESECTORGROUPS *pygmyParentSG, u8 ucChunks, u64 *ullStartAddress )
{
    // Count the number of erased and unused chunks
    // Return the number of contiguous unused chunks found  ans set ullStartAddress to start address OR
    // Return the number of erased chunks found and set ullStartAddress to PYGMYFILE_INVALIDADDRESS
    PYGMYFILESECTORGROUPS *pygmyParentSG;
    u64 ullAddress;
    u32 i, ulEndIndex, ulID;
    u8 ucErased, ucUnused;

    //pygmyParentSG
    ucErased = 0;
    ucUnused = 0;
    ulEndIndex = ( pygmyParentSG->SectorCount * pygmyVolume->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;
    for( i = 0; i < ulEndIndex; i++ ){
        fileGenerateAddress( pygmyVolume, pygmyParentSG, i * PYGMYFILE_CHUNKLEN, &ullAddress );
        ulID = pygmyVolume->IO->GetLong( &pygmyVolume->Port, ullAddress );
        if( ulID == 0 ){
            // We have found an erased chunk
            if( ucErased < 255 ){
                ++ucErased;
            } // if
            ucUnused = 0; // An erased sector requires recycling before use, and is counted as "used"
        } else if( ulID == 0xFFFFFFFF ){
            // We have found an unused chunk
            if( ucUnused == 0 ){
                // Recapture the start address each time the counter resets to zero
                ullStartAddress = ullAddress;
            } //
            ++ucUnused;
            if( ucUnused == ucChunks ){
                // We are done with our search, ucChunk unused chunks have been found, return
                return( ucUnused );
            } // if
        } // else if
    } // for

    *ullStartAddress = PYGMYFILE_INVALIDADDRESS; // We don't have the requested run of contiguous chunks

    if( ucErased >= ucChunks ){
        return( ucErased );
    } // if 

    return( 0 ); // 0 chunks found
}

u8 fileWriteEntry( PYGMYFILE *pygmyFile )
{
    // !!! WARNING !!! fileWriteEntry() writes the properties contained in the Properties of the file passed
    
    
}

u8 fileWriteProperties( PYGMYFILE *pygmyFile, PYGMYFILESECTORGROUPS *pygmyParentSG )
{
    // Writes Proeprties of the passed file 
    // When writing Attributes, will not write WRITE if pygmyFile->MountPoint does not have WRITE
    // Writes Time stamp, filename length and pygmyFile->Name ( may span chunks 0-9 )

}

u8 fileWriteLength( PYGMYFILE *pygmyFile )
{
    // find chunk 254 and load the length, also load Sector Groups Count if supported
    // the length is stored as: [ SG0 1B ][ SG1 1B ][ SG2 1B ][ LEN0 4B ][ LEN1 4B ][ LEN2 4B ][ LEN3 4B ][ LEN4 4B ][ LEN5 4B ]
    // All 1s is unused, all 0s is erased, any other number for SG is number of Sector Groups or LEN is Length of file in bytes
    // compare the Sector Groups Count and Length, only write if changed
    // If changed, find the first unused ( all 1s ) section, erase the current value, and write the new value
    // If either Sector Groups or Length have changed and there are no unused spots, write a new chunk and erase current chunk
    
}

u8 fileWriteSectorGroups( PYGMYFILE *pygmyFile )
{

}

u8 fileLoadEntry( u32 ulID, PYGMYFILEVOLUME *pygmyVolume, PYGMYFILESECTORGROUPS *pymgySG, PYGMYFILEPROPERTIES *pygmyEntry )
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
    u8 ucSGChunks;
    
    if( fileLoadProperties( ulID, pygmyVolume, pygmySG, pygmyEntry ){
        uiSGChunks = fileLoadLength( ulID, pygmyVolume, pygmySG, pygmyEntry );
        if( uiSGChunks != 0 ){
            if( fileLoadSectorGroups( ulID, ucSGChunks, pygmyvolume, pygmySG, pygmyEntry );
                print( COM3, "\rEntry Loaded" );
                return( TRUE );
            } // if
        } // if
    } // if
    
    return( FALSE );
}

u8 fileLoadLength( u32 ulID, PYGMYFILEVOLUME *pygmyVolume, PYGMYFILESECTORGROUPS *pymgySG, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // find chunk 254 and load the length into pygmyEntry->Length, also load pygmyEntry->SectorGroups if supported
    // the length is stored as: [ SG0 1B ][ SG1 1B ][ SG2 1B ][ LEN0 4B ][ LEN1 4B ][ LEN2 4B ][ LEN3 4B ][ LEN4 4B ][ LEN5 4B ]
    // All 1s is unused, all 0s is erased, any other number for SG is number of Sector Groups or LEN is Length of file in bytes
    // return the number of Sector Groups recorded in the first 3 bytes
    u64 ullAddress;
    u32 ulLength;
    u8 i, ucSG, ucBuffer[ 27 ];

    ucSG = 1; // There is always at least 1 Sector Group count
    ulLength = 0;
    if( fileFindChunk( ulID, PYGMYFILE_LENGTHCHUNK, pygmyVolume, pygmySG, ucBuffer ) ){
        // We have found the length chunk, now load the Sector Group Count and the Length
        
        for( i = 0; i < 3; i++ ){
            if( ucBuffer[ i ] != 0xFF && ucBuffer[ i ] != 0x00 ){
                ucSG = ucBuffer[ 0 ]; // Found a 
                break;
            } // if
        } // for
        ucBuffer += 3; // Increment to the first Length tag
       // There are six tags stored in each chunk, when a file has been written to and then closed, 
       //   the current tag is changed to 0 and the next erased ( 0xFFFFFFFF ) tag is updated with the current file length
       //   When all 6 tags have been used, the ID is changed to 0 and a new Length chunk is allocated
        for( i = 0; i < 6; i++ ){ 
            ulLength = convertBufferToU32( ucBuffer + ( 4 * i ), BIGENDIAN );
            if( ulLength != 0xFFFFFFFF && ulLength != 0 ){
                // We found the current Length Tag, time to return
                break;
            } // if
        } // for
    } // if

    // If there is no length chunk, that means the file hasn't had any write operations performed yet
    // This is not an error condition
    pygmyEntry->Length = ulLength;

    return( ucSG );
}

u8 fileFindChunk( u32 ulID, u8 ucChunk, PYGMYFILEVOLUME *pygmyVolume,  PYGMYFILESECTORGROUPS *pymgySG, u8 *ucBuffer )
{
    // fileFindChunk() does not allocate ucBuffer, 27 bytes must be allocated by the calling functions, static allocation is recommended
    // pygmySG is a pointer to the parent sector group array, this array must be filled out by the calling function
    u32 i, ii, iii, uiEntryCount, ulTmpID;
    u8 ucTmpChunk;

    uiEntryCount = pygmyVolume->Desc.SectorSize / 32; // 32 is length of chunks
    
    for( i = 0; i < pygmySGs->SectorGroups; i++ ){ // This is the number of parent sectors containing chunks to sort
        for( ii = 0; ii < pygmySG->Sectors[ i ].SectorCount; ii++ ){ // Iterate through Chunks and process matches
            for( iii = 1; iii < uiEntryCount; iii++ ){
                *ullAddress = pygmySG->Sectors[ i ].BaseAddress +
                    ( ii * pygmyVolume->Desc.SectorSize ) + ( iii * 32 );
                // Load the first 5 bytes of the chunk, this contains the ID and the chunk number
                pygmyParent->MountPoint->IO->GetBuffer( pygmyVolume->Port, *ullAddress, ucBuffer, 5 );
                ulTmpID = convertBufferToU32( ucBuffer, BIGENDIAN );
                ucTmpChunk = ucBuffer[ 4 ];
                
                if( ulTmpID == ulID && ucTmpChunk == ucChunk ){
                    // We found the chunk requested, load the 27 byte payload and return
                    pygmyParent->MountPoint->IO->GetBuffer( pygmyParent->MountPoint->Port, *ullAddress + 5, ucBuffer, 27 );
                    return( TRUE );
                } // if
            } // for
        } // for
    } // for
    
    return( FALSE ); // The requested chunk doesn't exist in the current folder
}

u8 fileLoadProperties( u32 ulID, PYGMYFILEVOLUME *pygmyVolume, PYGMYSECTORGROUPS *pygmySG, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // The Properties ( Name ) chunk is formatted as follows:
    // [ ID 4B ][ CHUNK 1B ][ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // Since fileFindChunk() fills ucBuffer with the 27 bytes following CHUNK, we will treat the format as follows:
    // [ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // If the file name length exceeds the 21 bytes available in the first Chunk, additional chunks will be loaded to
    // retrieve the remainder of the file name. All additional Properties ( Name ) chunks contain up to 27 bytes of name each,
    // spanning up to 10 chunks ( 0 - 9 )
    u16 i, ii;
    u8 ucBuffer[ 27 ], ucNameLen, ucNameIndex, ucChunk;

    if( !fileFindChunk( ulID, 0, pygmyVolume, pymgySG, ucBuffer ) ){
        // No Name Chunk means the file doesn't exist
        return( FALSE );
    } // if
    pygmyEntry->Time = convertBufferToU32( ucBuffer, BIGENDIAN );
    pygmyEntry->Atrributes = ucBuffer[ 4 ];
    ucNameLen = ucBuffer[ 5 ];
    pygmyEntry->Name = malloc( ucNameLen );
    if( !pygmyEntry->Name ){
        print( COM3, "\rMemory full!" );
        return( FALSE );
    } // if
    // Now load the filename
    for( i = 0, ii = 6, ucNameIndex = 0; i < ucNameLen; i++, ii++ ){
        if( !( ii % 27 ) ){
            // load the next Properties ( Name ) chunk id we are out of data and haven't reached the end of the name field
            if( !fileFindChunk( ulID, ii / 27, pygmyVolume, pygmySG, ucBuffer ) ){
                // This indicates file system corruption and is a serious failure
                print( COM3, "\rRequired Properties ( Name ) chunk missing." );
                return( FALSE );
            } // if
            ucNameIndex = 0; // Reset ucNameIndex to lock within range 0-26
        } // if
        if( ucBuffer[ ucNameIndex ] == 0xFF ){
            print( COM3, "\rEnd of Properties ( Name ) found at %d bytes", ii );
            ucBuffer[ ucNameIndex ] = '\0'; // Terminate string
            break;
        } // if
        pygmyEntry->Name[ i ] = ucBuffer[ ucNameIndex++ ];
    } // for

    return( TRUE );
}

u8 fileLoadSectorGroups( u32 ulID, u8 ucSGChunks, PYGMYFILEVOLUME *pygmyVolume, PYGMYSECTORGROUPS *pygmySG, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Sector Groups are stored 3 per chunk at 9 bytes each
    // A Sector Groups is formatted as follows: [ SECTORCOUNT 2B ][ BASEADDRESS 7B ]
    PYGMYSECTORGROUPS pygmyTmpSG;
    u64 ullAddress;
    u16 i, ii, iii;
    u16 uiSectorCount;
    u8 ucBuffer[ 27 ];

    pygmyTmpSG.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !pygmyTmpSG.Sectors ){
        return( FALSE );
    } // if
    pygmyTmpSG.SectorGroups = 0;
    pygmyTmpSG.SectorCount = 0;
    for( i = 0; i < ucSGChunks; i++ ){
        if( fileFindChunk( ulID, PYGMYFILE_SGCHUNK + i, pygmyVolume, pymgySG, uycBuffer ) ){
            for( ii = 0; ii < 3; ii++ ){
                // Load the 16bit Sector Count
                uiSectorCount = convertBufferToU16( ucBuffer, BIGENDIAN );
                ucBuffer += 2; // Increment to the Base Address and load the 56bit address
                for( iii = 0, ullAddress = 0; iii < 7; iii++ ){
                    ullAddress |= ( (u64) (++ucBuffer) << ( ( 7 - iii ) * 8 ) ); 
                } // iii
                // Now check the address for 0 length group or unused memory
                if( ullAddress == 0x00FFFFFFFFFFFFFFLL {
                    // If data is unused, we are past the last sector group
                    break;
                } else if( uiSectorCount ){
                    // If uiSectorCount is 0, then the sector group entry was erased, ignore and continue
                    // otherwise, allocate memory for a new sector group and append to the list
                    pygmyTmpSG.SectorCount += uiSectorCount;
                    ++pygmyTmpSG.SectorGroups;
                    pygmyTmpSG.Sectors = realloc( pygmyTmpSG.Sectors, ( 1 + pygmyTmpSG.Sectors ) * pygmysizeof( PYGMYFILEADDRESS ) );
                    if( !pygmyTmpSG.Sectors ){
                        return( FALSE );
                    } // if
                    pygmyTmpSG.Sectors[ pygmyTmpSG.SectorGroups - 1 ].SectorCount = uiSectorCount;
                    pygmyTmpSG.Sectors[ pygmyTmpSG.SectorGroups - 1 ].BaseAddress = ullAddress;
                    print( COM3, "\rAdded Sector Group, Count: %d, Base Address: %012llX", uiSectorCount, ullAddress );
                } // if
            } // for
        } else{
            // Sector Group chunks are returned in order.
            print( COM3, "\rProcessed all address chunks." );
            break;
        } // else
    } // for
}
/*
u8 fileLoadEntry( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Usage: Preload pygmyEntry with ID to load and parent Sector and SectorCount
    // Function will seek File Entry with ID in pygmyEntry and load into pygmyEntry
    // A local copy will be made to save the Sectors and SectorCount values
    // Function will allocate the Sectors and Name fields, calling function must free
    // Address info must be for the Parent,
    PYGMYFILEPROPERTIES tmpEntry;
    PYGMYFILEADDRESS *tmpAddress;
    PYGMYFILE pygmyFile;
    u64 ullTmpAddress;
    u32 i, ulEntryLen, ulID;
    u16 uiMemLen;
    //, ii, ulSP, ulSPLen, *ulPtr
    u8 *ucBuffer;

    pygmyFile.MountPoint = pygmyVolume;
    fileCopyProperties( pygmyEntry, &pygmyFile.Properties );
    pygmyFile.Properties.Name = "";
    pygmyFile.Properties.Path = "";
    pygmyFile.Properties.Length = 0;
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rCalling fileGetEntry from fileLoadEntry: " );
    //#endif
    ulEntryLen = fileGetEntry( &pygmyFile, &ucBuffer );
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntry dump of %d bytes", ulEntryLen );
        for( i = 0; i < ulEntryLen; i++ ){
            print( COM3, "(%02X)", ucBuffer[ i ] );
        } // for
    //#endif
    if( !ulEntryLen ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rEntry Len of 0" );
        #endif
        return( FALSE );
    } // if
    
    if( ucBuffer[ 4 ] == 0xFF || ucBuffer[ 5 ] == 0xFF || ucBuffer[ 6 ] == 0xFF ){
        // Entry slot is erased
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rLoaded erased entry" );
        #endif
        return( FALSE );
    } // if
    
    uiMemLen = 1 + ucBuffer[ 5 ];
    pygmyEntry->Name = malloc( uiMemLen );
    memset( pygmyEntry->Name, 0, uiMemLen );
    pygmyEntry->SectorCount = 0;
    uiMemLen = sizeof( PYGMYFILEADDRESS );
    pygmyEntry->Sectors = malloc( uiMemLen );
    memset( pygmyEntry->Name, 0, uiMemLen );
    pygmyEntry->Time = convertBufferToU32( ucBuffer, BIGENDIAN );
    pygmyEntry->Attributes = ucBuffer[ 4 ];
    copyBuffer( ucBuffer + 6, pygmyEntry->Name, ucBuffer[ 5 ] );
    pygmyEntry->Name[ ucBuffer[ 5 ] ] = '\0';
    i = 6 + ( ( 1 + ( ( 6 + ucBuffer[ 5 ] ) / 27 ) ) * 27 );
    print( COM3, "\ri = %d", i );
    //i = ( 1 + ( i / 27 ) ) * 27; // 27 is the 32 byte for the chunk minus the 5 bytes for ID and 1 byte for chunk number
    //i += ( ( i + 4 ) % 7 );
    //print( COM3, "\rStarting at: %d", i );
    //i = 6 + ucBuffer[ 5 ];
    //i = ( i / 7 ) * 7;
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSorting Sector Groups..." );
    //#endif
    for( pygmyEntry->SectorGroups = 0; i < ulEntryLen; ){
        if( !( i % 27 ) ){
            i += 6; // Each 27 byte chunk payload contains the sector groups offset by 6 bytes
        } // if
        fileLoadAddress( &pygmyEntry->Sectors[ pygmyEntry->SectorGroups ], ucBuffer+i );
        //#ifdef __PYGMY_DEBUG_FILE
            filePrintAddress( &pygmyEntry->Sectors[ pygmyEntry->SectorGroups ] );
        //#endif
        if( pygmyEntry->Sectors[ pygmyEntry->SectorGroups ].BaseAddress == 0x0000FFFFFFFFFFFFLL
            || !pygmyEntry->Sectors[ pygmyEntry->SectorGroups ].SectorCount ){
            // If stackpointer is not valid, then we have reached end of pointers
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rFound End" );
            #endif
            break;
        } // if
        tmpAddress = realloc( pygmyEntry->Sectors, sizeof( PYGMYFILEADDRESS ) * ( pygmyEntry->SectorGroups + 1 ) );
        if( !tmpAddress ){
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rMem Fail!" );
            #endif
            free( pygmyEntry->Sectors );
            free( ucBuffer );
            return( FALSE );
        } // if
        pygmyEntry->Sectors = tmpAddress;
        pygmyEntry->SectorCount += pygmyEntry->Sectors[ pygmyEntry->SectorGroups ].SectorCount;
        ++pygmyEntry->SectorGroups;
        i += 7;
    } // for
    // Buffer is allocated by fileGetEntry() and must be freed before calling again or returning
   
    free( ucBuffer );

    // Now load length, stored in chunk 254
   // pygmyFile.Properties.ID = ( ( pygmyFile.Properties.ID & 0xFFFFFF00 ) | 254 );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rCalling fileGetEntry() from fileLoadEntry() for Length Chunk" );
    #endif
    //ulEntryLen = fileGetEntry( &pygmyFile, &ucBuffer );
    //pygmyFile.MountPoint = pygmyVolume;
    //fileCopyProperties( pygmyEntry, &pygmyFile.Properties );
    if( pygmyEntry->Attributes & FOLDER ){
        return( TRUE );
    } // if
    pygmyFile.MountPoint = pygmyVolume;
    print( COM3, "\rMountPoint: %08X", &pygmyFile.MountPoint );
    if( fileSeekChunk( &pygmyFile, &ullTmpAddress, 254 ) ){
        
        ucBuffer = malloc( 24 );
        if(!(*ucBuffer) ){
            return( FALSE );
        } // if
        //memset( ucBuffer, 0xFF, 27 ); // clear memory to the Flash erased state to prevent write errors
        print( COM3, "\rLoading buffer from %012llX", ullTmpAddress );
        print( COM3, "\rMountPoint: %08X", pygmyVolume );
        pygmyVolume->IO->GetBuffer( pygmyVolume->Port, ullTmpAddress + 8, ucBuffer, 24);
        print( COM3, "\rLength Buffer:" );
        for( i = 0; i < 24; i++ ){
            print( COM3, " %02X", ucBuffer[ i ] );
        } // for
        // We have an entry, time to seek the length tag.
        // There are 6 length tags in a Length Entry, stored as one chunk.
        // A chunk is 32 bytes with ID, but only the 28 bytes after the ID are returned.
        // Only one Length tag will be valid, all others will be 0xFFFFFFFF or 0x00000000
        
        for( i = 0; i < 24; ){
            // Look for the first 4 byte entry not deleted ( 0xFFFFFFFF ) or erased ( 0x00000000 )
            // If there are no such entries, then the file hasn't had a length saved yet and must be 0
            pygmyEntry->Length = convertBufferToU32( ucBuffer+i, BIGENDIAN );
            //#ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rTesting Length: %d", pygmyEntry->Length );
            //#endif
            if( pygmyEntry->Length != 0xFFFFFFFF && pygmyEntry->Length != 0 ){
                //#ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rFound Length: %d", pygmyEntry->Length );
                //#endif
                break;
            } else{
                pygmyEntry->Length = 0;
            } // else
            i+= 4;
        } // for
    } else{
        // No length chunk is not an error, it is normal for new files
        // Set length to 0 and return
        pygmyEntry->Length = 0;
    } // else
    // Free buffer allocated by fileGetEntry() before returning
    free( ucBuffer );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rLength not found: %d", pygmyEntry->Length );
    #endif
    return( TRUE );
}*/

u8 fileSeekPath( u8 *ucPath, PYGMYFILEVOLUME **pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Path Rules:
    // 1. If the first char is '/', first substring must be name of mountpoint
    // 2. If the first char is '/' and there are no substrings, path points to root of current mountpoint
    // 3. If the first char is not '/' and there are no substrings, path points to path of current mountpoint
    // 4. If the first char is not '/' and there are substrings, path becomes path of current mountpoint
    // Note that no permanent changes are made to mountpoint or it's path

    PYGMYFILEVOLUME *pygmyLocalVolume;
    u32 ulID;
    u16 i, uiMemLen;
    u8 ucMax, ucLen, *ucFullPath, *ucParams[ 32 ];

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntering fileSeekPath(%s)", ucPath );
    #endif
    ucPath = removeLeadingWhitespace( ucPath );
    // All paths will be created equal, start by detecting path type
    if( ucPath[ 0 ] != '/' ){
        // Path is relative to current path, so get and composite with current path
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath is Relative" );
        #endif
        uiMemLen = 2 + len( ucPath ) + len( fileGetCurrentPath() );
        ucFullPath = malloc( uiMemLen );

        if( !ucFullPath ){
            return( FALSE );
        } // if
        memset( ucFullPath, 0, uiMemLen );
        print( COM3, "\rSplicing:");
        copyString( fileGetCurrentPath(), ucFullPath );
        print( COM3, "\r\t1:(%s)", ucFullPath );
        appendString( "/", ucFullPath );
        print( COM3, "\r\t2:(%s)", ucFullPath );
        appendString( ucPath, ucFullPath );
        print( COM3, "\r\t3:(%s)", ucFullPath );
    } else{
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath is Absolute" );
        #endif
        uiMemLen = 1 + len( ucPath );
        ucFullPath = malloc( uiMemLen );

        if( !ucFullPath ){
            return( FALSE );
        } // if
        memset( ucFullPath, 0, uiMemLen );
        copyString( ucPath, ucFullPath );

    } // else
    // Now we have a full path, time to separate volume name and step through folders
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rComposite path: %s", ucFullPath );
    #endif
    print( COM3, "\rGetting SubStrings from (%s)", ucFullPath );
    ucLen = getAllSubStrings( ucFullPath, ucParams, 32, FILESEPARATORS|NEWLINE );
    print( COM3, "..." );
    if( ucLen ){
        //ulID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ 0 ] ) );
        ulID = pdiaEncodeString( ucParams[ 0 ] );
        ucMax = fileGetMountPointCount();
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rSearching %d volumes for %s ", ucMax, ucParams[ 0 ] );
        #endif
        for( i = 0; i < ucMax; i++ ){
            *pygmyVolume = fileGetMountPoint( i );
            pygmyLocalVolume = *pygmyVolume;
            if( pygmyLocalVolume->Properties.ID == ulID ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "Done." );
                #endif
                break;
            } // if
        } // for
        if( i == ucMax ){
            return( FALSE );
        } // if
    } else{
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath stops at root" );
        #endif
        *pygmyVolume = fileGetCurrentMountPoint();
        pygmyLocalVolume = *pygmyVolume;
        fileCopyProperties( &pygmyLocalVolume->Properties, pygmyEntry );
        return( TRUE );
    } // else
    fileCopyProperties( &pygmyLocalVolume->Properties, pygmyEntry );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rBranching through %d nested entries", ucLen-1 );
    #endif
    for( i = 1; i < ucLen; i++ ){
        //ulID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ i ] ) );
        ulID = pdiaEncodeString( ucParams[ i ] );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rSearching for %s", ucParams[ i ] );
        #endif
        pygmyEntry->ID = ulID;
        if( !fileLoadEntry( pygmyLocalVolume, pygmyEntry ) ){
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rError: Entry %s not found!", ucParams[ i ] );
            #endif
            return( FALSE ); // Invalid path
        } else if( !(pygmyEntry->Attributes & FOLDER) && (i + 1) != ucLen ){
            // If loaded entry isn't a folder, then you can't branch any deeper
            // If this is the last param, it doesn't matter, otherwise it's a path error
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rError: Found file in path!" );
            #endif
            return( FALSE );
        } // else if
    } // for
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rDone" );
    #endif
    //print( COM3, "\rLoaded the following properties:" );
    //filePrintProperties( pygmyEntry );
    if( !pygmyLocalVolume ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rVolume NULL" );
        #endif
    } // if

    return( TRUE );
}


PYGMYFILEPROPERTIES *fileList( PYGMYFILELIST *pygmyFileList )
{
    PYGMYFILE pygmyFile;
    u64 ullAddress;
    u32 i, ii, ulID;
    u16 uiSectorGroup, uiSector, uiEntry;

    if( !pygmyFileList->MountPoint ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rNo Mountpoint Defined" );
        #endif
        return( NULL );
    } // if
    pygmyFile.MountPoint = pygmyFileList->MountPoint;
    fileCopyProperties( &pygmyFileList->ParentProperties, &pygmyFileList->Properties );
    fileCopyProperties( &pygmyFileList->ParentProperties, &pygmyFile.Properties );
    pygmyFile.Properties.Length = 4096*4; //fileReadLength( &pygmyFile ); //4096*4;

    for( i = 0; i < 100; i++ ){
        pygmyFile.Index = pygmyFileList->Index + 30;

        fileGenerateAddress( &pygmyFile, &ullAddress );

        ulID = pygmyFileList->MountPoint->IO->GetLong( pygmyFileList->MountPoint->Port, ullAddress );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\ri %d Index %d ID 0x%08X from 0x%012llX", i, pygmyFile.Index, ulID, ullAddress );
        #endif
        if( ulID != 0x00000000 && pygmyFileList->MountPoint->IO->GetChar( pygmyFileList->MountPoint->Port, ullAddress+4 ) == 0 ){//(ulID & 0x000000FF ) == 0 ){
            pygmyFileList->Properties.ID = ulID;
            fileLoadEntry( pygmyFileList->MountPoint, &pygmyFileList->Properties );
            ii = (( len( pygmyFileList->Properties.Name ) + ( pygmyFileList->Properties.SectorGroups * 7 ) ) ) ;
            pygmyFileList->Index += 32;// ii;
           
            return( &pygmyFileList->Properties );
        } // if
        pygmyFileList->Index += 32;
    } // for

    return( NULL );
}

u8 fileUnmount( PYGMYFILEVOLUME *pygmyVolume )
{

}

u8 fileFormat( PYGMYFILEVOLUME *pygmyVolume, u8 *ucName )
{
    //PYGMYFILEVOLUME *pygmyTmp
    PYGMYFILE pygmyFile, pygmyParent ;
    u32 i;
    u16 uiMemLen;
    u8 ucRemount = 0;

    if( !pygmyVolume ){
        pygmyVolume = fileGetCurrentMountPoint();
        if( !pygmyVolume ){
            return( FALSE );
        } // if
    } // if

    pygmyParent.MountPoint = pygmyVolume;
    //pygmyParent.Properties = pygmyParentEntry;
    //pygmyParent.Properties.Sectors = sysAllocate( sizeof( PYGMYFILEADDRESS ) );
    uiMemLen = sizeof( PYGMYFILEADDRESS );
    pygmyParent.Properties.Sectors = malloc( uiMemLen );
    memset( pygmyParent.Properties.Sectors, 0, uiMemLen );
    pygmyParent.Properties.Sectors[ 0 ].BaseAddress = 0;
    pygmyParent.Properties.Sectors[ 0 ].SectorCount = 1;
    pygmyParent.Properties.SectorCount = 1;
    pygmyParent.Properties.SectorGroups = 1;
    pygmyParent.Properties.Path = NULL; 
    pygmyParent.Properties.Name = NULL; 
    
    pygmyFile.MountPoint = pygmyVolume;
    pygmyFile.Properties.Sectors = NULL;
    pygmyFile.Properties.SectorGroups = 0;
    pygmyFile.Properties.SectorCount = 0;
    //pygmyFile.Properties.Path = "";
    //pygmyFile.Properties.Name = ucName;
    uiMemLen = 2 + len( ucName );
    pygmyFile.Properties.Path = malloc( uiMemLen );
    pygmyFile.Properties.Name = malloc( uiMemLen );
    memset( pygmyFile.Properties.Path, 0, uiMemLen );
    memset( pygmyFile.Properties.Name, 0, uiMemLen );
    if( pygmyFile.Properties.Path == NULL || pygmyFile.Properties.Name == NULL ){
        return( FALSE );
    } // if
    copyString( "/", pygmyFile.Properties.Path );
    appendString( ucName, pygmyFile.Properties.Path );
    copyString( ucName, pygmyFile.Properties.Name );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rErasing All" );
    #endif
    pygmyVolume->IO->EraseAll( pygmyVolume->Port );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "..." );
    #endif
    // Allocate sector 0 manually
    pygmyVolume->IO->PutWord( pygmyVolume->Port, 0, 0 );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rAllocating Sectors" );
    #endif

    fileAllocateSectors( &pygmyFile, 1 );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rWriting Entry..." );
    #endif
    pygmyFile.Properties.Attributes = (READ|WRITE|FOLDER);
    pygmyFile.Properties.Name = ucName;
    fileWriteEntry( &pygmyParent, &pygmyFile.Properties );

    free( pygmyParent.Properties.Sectors );
       
    fileCopyProperties( &pygmyFile.Properties, &pygmyVolume->Properties );
}

u64 fileFindNextEmptySector( PYGMYFILEVOLUME *pygmyVolume, u64 ullAfter )
{
    u32 i;
    u16 uiData;

    if( !pygmyVolume ){
        return( FALSE );
    } // if
    i = ullAfter / pygmyVolume->Desc.SectorSize ;
    if( ullAfter % pygmyVolume->Desc.SectorSize ){
        ++i;
    } // if
    if( i == 0 ){
        i = 1;
    } // if
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSector Size %d", pygmyVolume->Desc.SectorSize );
        print( COM3, "\rStarting at Sector %d of %d sectors", i, pygmyVolume->Desc.Sectors );
    #endif
    for( ; i < pygmyVolume->Desc.Sectors && i < 10; i++ ){
        uiData = pygmyVolume->IO->GetWord( pygmyVolume->Port, i * pygmyVolume->Desc.SectorSize );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rTesting %d at %d", uiData, i * pygmyVolume->Desc.SectorSize );
        #endif
        if( uiData == 0xFFFF ){
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rFound %d", i * pygmyVolume->Desc.SectorSize );
            #endif
            return( i * pygmyVolume->Desc.SectorSize );
        } // else if
    } // for
    // Memory full
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rMem Full" );
    #endif

    return( FALSE );
}

u64 fileFindContiguousEmptySectors( PYGMYFILEVOLUME *pygmyVolume, u32 ulCount )
{
    u64 ullAddress;
    u32 i, ulFound;

    for( i = 1, ulFound = 0; i < pygmyVolume->Desc.Sectors;  i++){
            ullAddress = fileFindNextEmptySector( pygmyVolume, i * pygmyVolume->Desc.SectorSize );
            if( !ullAddress ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "Fail" );
                #endif
                ulFound = 0;
            } else{
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rFound" );
                #endif
                ++ulFound;
            } // else
        if( ulFound == ulCount ){
            return( ullAddress );
        } // if
    } // for

    return( FALSE );
}

u8 fileAllocateSectors( PYGMYFILE *pygmyFile, u32 ulRequestedSectors )
{
    // This function seeks a contiguous run of sectors
    // Since the number of groups required to allocate the requested number of sectors is
    //  unknown until allocation is finished,the function must allocate the sector groups array
    PYGMYFILEADDRESS *pygmyAddress;
    u64 ullAddress;
    u32 i, ii, ulCount;
    u16 uiMemLen;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rStart Allocating" );
    #endif
    if( !pygmyFile->Properties.Sectors ){
        uiMemLen = sizeof( PYGMYFILEADDRESS );
        pygmyAddress = malloc( uiMemLen );
        memset( pygmyAddress, 0, uiMemLen );
        if( !pygmyAddress ){
            return( FALSE ); // Memory Full
        } // if
    } // if
    // Subdivide number of requested sectors until suitable level of granularity is reached

    for( i = 0; i < ulRequestedSectors; ){
        ulCount = ulRequestedSectors / ( 1 + i ); // Test for
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rCalling Contiguous Seek" );
            print( COM3, "\r\tulRequestedSectors: %d", ulRequestedSectors );
            print( COM3, "\r\tulCount: %d", ulCount );
            print( COM3, "\r\ti %d", i );
        #endif
        ullAddress = fileFindContiguousEmptySectors( pygmyFile->MountPoint, ulCount );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rRequested %d", ulRequestedSectors );
            print( COM3, "\rFound %d", ulCount );
            print( COM3, "\rSectors at %d", ullAddress );
        #endif
        if( ullAddress ){
            // If return is non-zero, add sector group and continue
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rReallocating memory" );
            #endif
            pygmyAddress = realloc( pygmyFile->Properties.Sectors, sizeof( PYGMYFILEADDRESS ) *
                ( pygmyFile->Properties.SectorGroups + 1 ) );
            if( !pygmyAddress ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rMem Fail in Sector Alloc" );
                #endif
                return( FALSE ); // Memory Full
            } // if
            pygmyFile->Properties.Sectors = pygmyAddress;
            pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups ].BaseAddress = ullAddress;
            pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups ].SectorCount = ulCount;

            // Tag Sectors from this group
            for( ii = 0; ii < ulCount; ii++ ){
                pygmyFile->MountPoint->IO->PutWord( pygmyFile->MountPoint->Port, ullAddress +
                    ( ii * pygmyFile->MountPoint->Desc.SectorSize ), pygmyFile->Properties.SectorCount++ );
            } // for
            ++pygmyFile->Properties.SectorGroups;
            ulRequestedSectors -= ulCount;
        } else{
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rNo sectors free" );
            #endif
            ++i;
        } // else
    } //for
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rDone Allocating" );
        print( COM3, "\r\tSectorGroups: %d", pygmyFile->Properties.SectorGroups );
        print( COM3, "\r\tSectors: %d", pygmyFile->Properties.SectorCount );
    #endif
    return( TRUE );
}

u8 fileWriteChunk( PYGMYFILE *pygmyParent, u8 *ucChunk )
{
    // This function scans for next unused entry and writes 32 byte ucBuffer
    // ucBuffer must be correctly formatted, starting with ID|CHUNK
    u64 ullAddress;
    u32 ulID;

    ulID = pygmyParent->Properties.ID;
    pygmyParent->Properties.ID = 0xFFFFFFFF; // Set the address to all bits set to seek for erased chunk
  
    if( fileSeekChunk( pygmyParent, &ullAddress, 0xFF )){
        print( COM3, "\rfileWriteChunk()->MountPoint: %08X", (u32)pygmyParent->MountPoint );
        print( COM3, "\rfileWriteChunk()->Address: %012llX", ullAddress );
        pygmyParent->MountPoint->IO->PutBuffer( pygmyParent->MountPoint->Port, ullAddress, ucChunk, 32 );
        pygmyParent->Properties.ID = ulID;
        print( COM3, "\rtest" ); //"\rfileWriteChunk()->Finished write of 32 bytes" );
        return( TRUE );
    } // if
    pygmyParent->Properties.ID = ulID;

    return( FALSE );
}

u8 fileSeekChunk( PYGMYFILE *pygmyParent, u64 *ullAddress, u8 ucChunk )
{
    u32 i, ulTmpID;
    u16 ii, iii, uiEntryCount;
    u8 ucBuffer[ 5 ], ucTmpChunk;
    // This function seeks for an ID match ( 0xFFFFFFFF for unused, or [ ID 3B | CHUNK 1B ] )
    // If found, the address for the chunk is returned, else 0xFFFFFFFF ( since 0x00000000 is
    //  valid for the root folder
    // A special case is provided for list and search functions: The ID|CHUNK 0xFFFFFF00 will
    // return on the first occurance
    uiEntryCount = pygmyParent->MountPoint->Desc.SectorSize / 32; // 32 is length of chunks
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSeeking 0x%08X", pygmyParent->Properties.ID );
        print( COM3, "\ruiEntryCount: %d", uiEntryCount );
    //#endif
    for( i = 0; i < pygmyParent->Properties.SectorGroups; i++ ){ // This is the number of parent sectors containing chunks to sort
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\r\tIn Group %d", i );
            print( COM3, "\r\t\tSectorCount: %d", pygmyParent->Properties.Sectors[ i ].SectorCount );
            print( COM3, "\r\t\tBaseAddress: 0x%012llX", pygmyParent->Properties.Sectors[ i ].BaseAddress );
        //#endif
        for( ii = 0; ii < pygmyParent->Properties.Sectors[ i ].SectorCount; ii++ ){ // Iterate through Chunks and process matches
            for( iii = 1; iii < uiEntryCount; iii++ ){
                *ullAddress = pygmyParent->Properties.Sectors[ i ].BaseAddress +
                    ( ii * pygmyParent->MountPoint->Desc.SectorSize ) + ( iii * 32 );
                pygmyParent->MountPoint->IO->GetBuffer( pygmyParent->MountPoint->Port, *ullAddress, ucBuffer, 5 );
                ulTmpID = convertBufferToU32( ucBuffer, BIGENDIAN );
                ucTmpChunk = ucBuffer[ 4 ];
                
                if( ulTmpID == pygmyParent->Properties.ID && ucTmpChunk == ucChunk ){
                    print( COM3, "\rulTmpID: %04X, ucTmpChunk: %02X", ulTmpID, ucTmpChunk );
                    //#ifdef __PYGMY_DEBUG_FILE
                        print( COM3, "\rID 0x%08X @ 0x%012llX", ulTmpID, (u64) *ullAddress );
                    //#endif
                    // *ullAddress += 4;
                    // ucTmpChunk = pygmyParent->MountPoint->IO->GetChar( pygmyParent->MountPoint->Port, *ullAddress );
                   
                    // ID is a match, next check chunk number 
                    /*/if( ucTmpChunk != 0 ){
                        #ifdef __PYGMY_DEBUG_FILE
                            print( COM3, "\rFound Chunk" );
                        #endif
                        return( TRUE );
                    } // if
                    */
                    return( TRUE );
                } // if
            } // for
        } // for
    } // for

    return( FALSE );
}

u8 fileDeleteEntry( PYGMYFILE *pygmyFile, PYGMYFILEPROPERTIES *pygmyParentEntry )
{
    PYGMYFILE pygmyParent;
    u64 ullAddress;
    u16 i, uiChunks;

    pygmyParent.MountPoint = pygmyFile->MountPoint;
    fileCopyProperties( pygmyParentEntry, &pygmyParent.Properties );
 
    uiChunks = 1 + ( 32 * (( 10 + len( pygmyFile->Properties.Name ) + ( pygmyFile->Properties.SectorGroups * sizeof( PYGMYFILEADDRESS ) )) / 32 ) );
    for( i = 0; i < uiChunks; i++ ){
        //pygmyParent.Properties.ID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | (u8)i;
        if( fileSeekChunk( &pygmyParent, &ullAddress, i ) ){
            pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, 0x00000000 );
        } // if
    } // for
    // Finally, seek and delete length chunk if present
    //pygmyParent.Properties.ID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | (u8)254;
    if( fileSeekChunk( &pygmyParent, &ullAddress, 254 ) ){
        pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, 0x00000000 );
    } // if

    return( TRUE );
}

u16 fileGetEntry( PYGMYFILE *pygmyFile, u8 **ucBuffer )
{
    // Pass this function parent address
    u64 ullTmpAddress;
    u16 i, ii;

    // !!!Warning!!! The calling function must free ucBuffer!
    // If not found, function returns false, else the 28 bytes following ID are loaded
    //      into ucBuffer and returned.
    *ucBuffer = malloc( 27 );

    if( !(*ucBuffer ) ){
        return( FALSE );
    } // if
    // Clear buffer to erased state before loading data
    memset( *ucBuffer, 0xFF, 27 );
    /*if( ( pygmyFile->Properties.ID & 0x000000FF ) == 254 ){
        // Length chunk, start by looking for chunk and retrieve address
        if( !fileSeekChunk( pygmyFile, &ullTmpAddress ) ){
            print( COM3, "\rNo chunk" );
            return( 0 );
        } // if
        pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullTmpAddress + 4, *ucBuffer, 28 );
        return( 28 );
    } else{*/
        for( i = 0; i < 250; i++ ){
            //pygmyFile->Properties.ID = ( ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | i );
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rCalling fileSeekChunk" );
                filePrintProperties( &pygmyFile->Properties );
            #endif
            if( !fileSeekChunk( pygmyFile, &ullTmpAddress, i ) ){
                print( COM3, "\rChunk break" );
                break;
            } // if
            *ucBuffer = realloc( *ucBuffer, ((i+1)*27) );
            if(!(*ucBuffer) ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rMemory failure!" );
                #endif
                return( FALSE );
            } // if
            //memset( *ucBuffer+(i*27), 0xFF, 27 ); // clear memory to the Flash erased state to prevent write errors
            pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullTmpAddress + 5, *ucBuffer+(i*27), 27 );
        } // for
    //} // else
    return( i * 27 );
}

void filePrintAddress( PYGMYFILEADDRESS *pygmyAddress )
{
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rAddress" );
        print( COM3, "\r\tSectorCount: 0x%02X", pygmyAddress->SectorCount );
        print( COM3, "\r\tBaseAddress: 0x%012llX", pygmyAddress->BaseAddress );
    #endif
}

void fileLoadAddress( PYGMYFILEADDRESS *pygmyAddress, u8 *ucBuffer )
{
    u8 i;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\r" );
        for( i = 0; i < 7; i++ ){
            print( COM3, "(%02X)", ucBuffer[ i ] );
        } // for
        print( COM3, "\rBuilding Address:" );
    #endif
    pygmyAddress->SectorCount = *(ucBuffer++);

    pygmyAddress->BaseAddress = 0LL;
    for( i = 0; i < 6; i++ ){
        pygmyAddress->BaseAddress <<= 8LL;
        pygmyAddress->BaseAddress |= ucBuffer[ i ];
    } // for
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\r0x%012llX", pygmyAddress->BaseAddress );
    #endif
}

void fileConvertAddressToBuffer( PYGMYFILEADDRESS *pygmyAddress, u8 *ucBuffer )
{
    *(ucBuffer++) = (u8)pygmyAddress->SectorCount;
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress >> 40 );
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress >> 32 );
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress >> 24 );
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress >> 16 );
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress >> 8 );
    *(ucBuffer++) = (u8)((u64)pygmyAddress->BaseAddress );
}

u16 fileGenerateAddress( PYGMYFILE *pygmyFile, u64 *ullAddress )
{
    // This function decodes base-address and count pairs and calculates a physical
    //  address off this information and the file index.
    // The number of bytes left in the current sector is returned, ullAddress is loaded with address
    u32 i, ulGroup, ulSectors;
    u16 uiOffset, uiBufLen;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rGenerating Address" );
        filePrintProperties( &pygmyFile->Properties );
    #endif // __PYGMY_DEBUG_FILE
    ulSectors = pygmyFile->Index / ( pygmyFile->MountPoint->Desc.SectorSize - 2 );
    if( ulSectors > pygmyFile->Properties.SectorCount || pygmyFile->Index > pygmyFile->Properties.Length ){
        *ullAddress = INVALIDADDRESS;
       
        return( FALSE );
    } // if
    for( i = 0, ulGroup = 0; i < ulSectors && ulGroup < pygmyFile->Properties.SectorGroups; ulGroup++ ){
        if( i + pygmyFile->Properties.Sectors[ ulGroup ].SectorCount > ulSectors ){
            break;
        } // if
        i += pygmyFile->Properties.Sectors[ ulGroup ].SectorCount;
    } // for
   
    uiOffset = ( ( pygmyFile->Index + ( ( 1 + ulSectors ) * 2 ) ) );
    uiBufLen = ( pygmyFile->MountPoint->Desc.SectorSize ) - uiOffset;
    
    *ullAddress = pygmyFile->Properties.Sectors[ ulGroup ].BaseAddress + (( ulSectors - i ) *
        pygmyFile->MountPoint->Desc.SectorSize ) + uiOffset;
    
    return( uiBufLen ); // Return number of bytes remaining in current sector
}

u8 fileAppendSectors( PYGMYFILE *pygmyFile, u32 ulSectorCount )
{
    // Sector Tag [ Count 1B ][ Start Address 6B ]
    // [ 32B ][ ID 4B ][ 7B ][ 7B ][ 7B ][ 7B ]
    // This function finds the first available slot for a sector tag and writes ulSector
    // If no slots are available in the currently allocated chunks, allocate a new one
    PYGMYFILE pygmyParent;
    PYGMYFILEADDRESS pygmySP, pygmyAddress;
    PYGMYFILEPROPERTIES pygmyEntry;
    PYGMYFILEVOLUME *pygmyVolume;
    u64 ullAddress, ullAfter;
    u32 ulSPLen;
    u16 i, uiEntryLen;
    u8 *ucBuffer;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rAppending Sectors" );
    #endif
    
    if( pygmyFile->Properties.SectorGroups == 0 ){
        // We cannot append sectors without at least one sector group, file is corrupt without 1 or more sector groups
        print( COM3, "\Error: rNo SectorGroups in %s", pygmyFile->Properties.Name );
        return( FALSE );
    } // if
    // Always attempt to allocate contiguous sectors
    // Calculate the address of the last sector in the last sector group, we will use this below
    ullAfter = pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups - 1 ].BaseAddress;
    ullAfter += pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups - 1 ].SectorCount * pygmyFile->MountPoint->Desc.SectorSize;

    if( ulSectorCount == 0 ){
        // We cannot append 0 sectors, there is no work to be done
        print( COM3, "\rError: Empty Sector Append for %s", pygmyFile->Properties.Name );
        return( FALSE );
    } else if( ulSectorCount == 1 && pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups - 1 ].SectorCount < 254 ){
        // Scenario 1: We are only appending one sector, we will attempt to append to current group
        // If the sector count in the last group is already reached the limit, we will continue on to Scenario 2
        ullAddress =  fileFindNextEmptySector( pygmyFile->MountPoint, ullAfter );
        // Check address to make sure it's contiguous, if not, simply skip to Scenario 2
        // Allocate sectors, if the address is contiguous
        if( ullAddress > ullAfter && ullAddress < ( ullAfter + pygmyFile->MountPoint->Desc.SectorSize ) ){
            pygmyFile->MountPoint->IO->PutWord( pygmyFile->MountPoint->Port, ullAddress, pygmyFile->Properties.SectorCount );
            ++pygmyFile->Properties.SectorCount;
            ++pygmyFile->Properties.Sectors[ pygmyFile->Properties.SectorGroups - 1 ].SectorCount;
        } else{
            fileAllocateSectors( pygmyFile, 1 );
        } // else
    } else{
        // Scenario 2: We are appending multiple sectors, we will add a new group
        // Allocate sectors
        fileAllocateSectors( pygmyFile, ulSectorCount );
    } // else
    // ToDo: Add fileUpdateEntry() to minimize write cycles
    // Create a parentFile then delete the current entry and write the updated entry

    pygmyParent.MountPoint = pygmyFile->MountPoint;      
    //fileWriteEntry( &pygmyParent, &pygmyFile->Properties );

    fileCopyProperties( &pygmyFile->MountPoint->Properties, &pygmyParent.Properties );
    filePrintProperties( &pygmyFile->Properties );
    fileDeleteEntry( pygmyFile, &pygmyFile->MountPoint->Properties ); 
    
    fileWriteEntry( &pygmyParent, &pygmyFile->Properties );
    filePrintProperties( &pygmyFile->Properties );

    return( TRUE );
}

void filePrintChunk( u8 *ucBuffer )
{
    u16 i, ii, uiNameLen;

    //#ifdef __PYGMY_DEBUG_FILE
    if( ucBuffer[ 4 ] == 0 ){ 
        print( COM3, "\rName Chunk" );
        print( COM3, "\r\tID:\t" );
        for( ii = 0; ii < 4; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tChunk:\t %02X", ucBuffer[ ii++ ] );
        print( COM3, "\r\tTime:\t" );
        for( ; ii < 9; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tAttrib:  %02X", ucBuffer[ ii++ ] );
        print( COM3, "\r\tNameLen: %02X", ucBuffer[ ii++ ] );
        print( COM3, "\r\tName:\t" );
        for( ; ii < 11 + ucBuffer[ 10 ] && ii < 32; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tFill:\t" );
        for( ; ii < 32; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
    } else if ( ucBuffer[ 4 ] == 254 ){
        print( COM3, "\rLength Chunk" );
        print( COM3, "\r\tID:\t" );
        for( ii = 0; ii < 4; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tChunk:\t%02X", ucBuffer[ ii++ ] );
        print( COM3, "\r\tFill:\t" );
        for( ; ii < 8; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        for( i = 0; i < 6; i++ ){
            print( COM3, "\r\tFill:\t" );
            for( ii = 0; ii < 4; ii++ ){
                print( COM3, " %02X", ucBuffer[ 8 + ( i * 4 ) + ii ] );
            } // for
        } // for
    } else{
        print( COM3, "\rAllocation Chunk" );
        print( COM3, "\r\tID:\t" );
        for( ii = 0; ii < 4; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tChunk:\t %02X", ucBuffer[ ii++ ] );
        print( COM3, "\r\tFill:\t" );
        for( ; ii < 11; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tGroup1:\t" );
        for( ; ii < 18; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tGroup2:\t" );
        for( ; ii < 25; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
        print( COM3, "\r\tGroup3:\t" );
        for( ; ii < 32; ii++ ){
            print( COM3, " %02X", ucBuffer[ ii ] );
        } // for
    } // else
        //#endif
}

u8 fileWriteEntry( PYGMYFILE *pygmyParent, PYGMYFILEPROPERTIES *pygmyEntry )
{
    u32 ulSectorIndex;
    u16 i, ii, uiBufferSize, uiMemLen;
    u8 ucChunks, ucChunk, ucNameLen, *ucBuffer;

    //pygmyEntry->ID = 0xFFFFFF00 & pdiaEncodeString( pygmyEntry->Name );
    pygmyEntry->ID = pdiaEncodeString( pygmyEntry->Name );
    ucNameLen = len( pygmyEntry->Name );

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntering fileWriteEntry" );
    #endif
    //uiBufferSize = ( 10 + ucNameLen + ( pygmyEntry->SectorGroups * sizeof( PYGMYFILEADDRESS ) ) );
    uiBufferSize = ( 1 + ( ( 11 + ucNameLen ) / 32 ) ) * 32; 
    if( pygmyEntry->SectorGroups ){
        uiBufferSize += ( 1 + ( ( ( pygmyEntry->SectorGroups * sizeof( PYGMYFILEADDRESS ) ) ) / 32 ) ) * 32;
    } // if
    ucChunks = uiBufferSize / 32;
    if( uiBufferSize % 32 ){
        ++ucChunks;
    } // if
    print( COM3, "\rSectorGroups: %d", pygmyEntry->SectorGroups );
    print( COM3, "\ruiBufferSize: %d, ucChunks: %d", uiBufferSize, ucChunks );
    uiMemLen = ucChunks * 32;
    ucBuffer = malloc( uiMemLen );
    // Clear buffer contents with 0xFF to prevent writing erroneous bits
    memset( ucBuffer, 0xFF, uiMemLen );
   
    if( !ucBuffer ){
        return( FALSE );
    } // if
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntry:" );
        print( COM3, "\r\tID: 0x%08X", pygmyEntry->ID );
        print( COM3, "\r\tBufferSize: %d", uiBufferSize );
        print( COM3, "\r\tChunks: %d", ucChunks );
        print( COM3, "\r\tAttrib: %d", pygmyEntry->Attributes );
        print( COM3, "\r\tSectorGroups: %d\r\r", pygmyEntry->SectorGroups);
    #endif
    // First chunk always contains [ID 4B][CHUNK 1B[TIME 4B][ATTRIB 1B][NAMELEN 1B][NAME NAMELEN], Name may overflow into next chunk(s)
    ucChunk = 0; // Chunks start at 0, 0 always containing the name
    convertU32ToBuffer( pygmyEntry->ID, ucBuffer, BIGENDIAN );
    ucBuffer[ 4 ] = ucChunk; // First Chunk, 0 is start of entry and hold the name or first chunk of the name
    convertU32ToBuffer( timeGet(), ucBuffer + 5, BIGENDIAN );
    ucBuffer[ 9 ] = pygmyEntry->Attributes;
    ucBuffer[ 10 ] = ucNameLen;
    
    // i starts at 11 to place the name into the buffer starting after the namelen
    for( i = 11; *(pygmyEntry->Name); i++ ){
        if( !( i % 32 ) ){
            ++ucChunk;
            convertU32ToBuffer( pygmyEntry->ID, ucBuffer+i, BIGENDIAN );
            i += 5;
        } // if
        // Add name to buffer
        ucBuffer[ i ] = *(pygmyEntry->Name++);
    } // for
    // Add the sector pointers to the buffer
    // All sector pointers are saved on 32bit boundaries
    /*if( 32 - i > 6 ){
        ii = ( i / 7 ) * 7; //i + ( i % 7 ); // There is space for one or more Sector Pointers at the end of last name chunk
        print( COM3, "\rRoom left for sector groups, %d bytes", 32 - i );
        print( COM3, "\r\tStarting at %d", ii );
    } else{
        ii = 32;//i + ( 32 % i ); // There is not enough space, increment to beginning of next chunk
    } // else
    */
    i = ( ( 1 + ( i / 32 ) ) * 32 ); // Snap to 32 byte boundary, we will ignore any bytes after the name
    //i += 6; // First Sector Group is stored 6 bytes in, or 3 groups of 7 bytes per 
    if( pygmyEntry->SectorGroups ){
        convertU32ToBuffer( pygmyEntry->ID, ucBuffer+i, BIGENDIAN );
        ucBuffer[ i+4 ] = ++ucChunk;
        i += 11;
    } // if
    for( ulSectorIndex = 0; ulSectorIndex < pygmyEntry->SectorGroups; ulSectorIndex++ ){
        if( !( i % 32 ) ){
            ++ucChunk;
            convertU32ToBuffer( pygmyEntry->ID, ucBuffer+i, BIGENDIAN );
            ucBuffer[ i+4 ] = ucChunk;
            i += 11;
        } // if
        fileConvertAddressToBuffer( &pygmyEntry->Sectors[ ulSectorIndex ], ucBuffer+i );
        i += 7;
    } // for
    // Finally, write the Entry as 32 byte chunks
    for( i = 0; i < ucChunks; i++ ){
        filePrintChunk( ucBuffer + ( i * 32 ) );
        fileWriteChunk( pygmyParent, ucBuffer + ( i * 32 ) );
    } // for

    free( ucBuffer );

    return( ucChunk );
}

/*
u8 fileUpdateEntry( PYGMYFILE *pygmyParent, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Use this function to write only the delta
    u32 ulSectorIndex;
    u16 i, ii, uiBufferSize, uiMemLen;
    u8 ucChunks, ucChunk, ucNameLen, *ucBuffer;

    pygmyEntry->ID = 0xFFFFFF00 & pdiaEncodeString( pygmyEntry->Name );
    ucNameLen = len( pygmyEntry->Name );

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntering fileWriteEntry" );
    #endif
    uiBufferSize = ( 10 + ucNameLen + ( pygmyEntry->SectorGroups * sizeof( PYGMYFILEADDRESS ) ) );
    ucChunks = uiBufferSize / 32;
    if( uiBufferSize % 32 ){
        ++ucChunks;
    } // if
    uiMemLen = ucChunks * 32;
    ucBuffer = malloc( uiMemLen );
    // Clear buffer contents with 0xFF to prevent writing erroneous bits
    memset( ucBuffer, 0xFF, uiMemLen );
   
    if( !ucBuffer ){
        return( FALSE );
    } // if
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntry:" );
        print( COM3, "\r\tID: 0x%08X", pygmyEntry->ID );
        print( COM3, "\r\tBufferSize: %d", uiBufferSize );
        print( COM3, "\r\tChunks: %d", ucChunks );
        print( COM3, "\r\tAttrib: %d", pygmyEntry->Attributes );
        print( COM3, "\r\tSectorGroups: %d\r\r", pygmyEntry->SectorGroups);
    #endif
    // First chunk always contains [ID][TIME][ATTRIB][STRLEN][NAME], Name may overflow into next chunk(s)
    convertU32ToBuffer( pygmyEntry->ID, ucBuffer, BIGENDIAN );
    convertU32ToBuffer( timeGet(), ucBuffer + 4, BIGENDIAN );
    ucBuffer[ 8 ] = pygmyEntry->Attributes;
    ucBuffer[ 9 ] = ucNameLen;
    ucChunk = 0; // Chunks start at 0, 0 always containing the name
    // i starts at one to place the name into the buffer starting after the namelen
    for( i = 10; *(pygmyEntry->Name); i++ ){
        if( !( i % 32 ) ){
            ++ucChunk;
            convertU32ToBuffer( pygmyEntry->ID|ucChunk, ucBuffer+i, BIGENDIAN );
            i += 4;
        } // if
        // Add name to buffer
        ucBuffer[ i ] = *(pygmyEntry->Name++);
    } // for
    // Add the sector pointers to the buffer
    // All sector pointers are saved on 32bit boundaries
    if( i % 32 > 6 ){
        ii = i + ( i % 7 ); // There is space for one or more Sector Pointers at the end of last name chunk
    } else{
        ii = i + ( i % 32 ); // There is not enough space, increment to beginning of next chunk
    } // else
    for( ulSectorIndex = 0; ulSectorIndex < pygmyEntry->SectorGroups; ulSectorIndex++ ){
        if( !( ii % 32 ) ){
            ++ucChunk;
            convertU32ToBuffer( pygmyEntry->ID|ucChunk, ucBuffer+ii, BIGENDIAN );
            ii += 4;
        } // if
        fileConvertAddressToBuffer( &pygmyEntry->Sectors[ ulSectorIndex ], ucBuffer+ii );
        ii += 7;
    } // for
    // Finally, write the Entry as 32 byte chunks
    for( i = 0; i < ucChunks; i++ ){
        fileWriteChunk( pygmyParent, ucBuffer + ( i * 32 ) );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rWriting Entry: " );
            for( ii = 0; ii < 32; ii++ ){
                print( COM3, "(%d)", ucBuffer[ ii ] );
            } // for
        #endif
    } // for

    free( ucBuffer );

    return( ucChunk );
}*/

u8 fileSetPath( PYGMYFILE *pygmyFile, u8 *ucPath )
{

}

u32 fileGetFreeSpace( PYGMYFILEVOLUME *pygmyVolume )
{
    u32 i, ulSectors;
    //u16 uiTag;

    for( i = 0, ulSectors = 0; i < pygmyVolume->Desc.Sectors; i++ ){
        if( pygmyVolume->IO->GetWord( pygmyVolume->Port, i * pygmyVolume->Desc.SectorSize ) == 0xFFFF ){
            ++ulSectors;
        } // if
    } // for
    ulSectors *= pygmyVolume->Desc.SectorSize ;

    return( ulSectors );
}

u8 fileGetMountPointCount( void )
{
    return( globalMountPointCount );
}

PYGMYFILEVOLUME *fileGetCurrentMountPoint( void )
{
    return( globalMountPointCurrent );
}

PYGMYFILEVOLUME *fileGetMountPoint( u8 ucIndex )
{
    return( &globalMountPoints[ ucIndex ] );
}

u8 fileDelete( u8 *ucName  )
{
    PYGMYFILEPROPERTIES pygmyEntry;
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILE pygmyFile;
    u64 ullAddress;
    u16 i, ii;

    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntering fileDelete(%s)", ucName );
    //#endif
     // Now we attempt to open the entire path, including name
    if( fileSeekPath( ucName, &pygmyVolume, &pygmyEntry ) == FALSE ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath Seek Failed!" );
        //#endif
        return( FALSE );
    } else{
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath found" );
        //#endif
    } // else
    fileCopyProperties( &pygmyEntry, &pygmyFile.Properties );
    pygmyFile.MountPoint = pygmyVolume;
    for( i = 0; i < pygmyFile.Properties.SectorGroups; i++ ){
        for( ii = 0; ii < pygmyFile.Properties.Sectors[ i ].SectorCount; ii++ ){
            ullAddress = pygmyFile.Properties.Sectors[ i ].BaseAddress + ( ii * pygmyFile.MountPoint->Desc.SectorSize );
            pygmyFile.MountPoint->IO->EraseSector( pygmyFile.MountPoint->Port, ullAddress );
        } // for
    } // for
    // Todo: Change to a mark as deleted
    fileDeleteEntry( &pygmyFile, &pygmyVolume->Properties );
}

u8 *fileGetPathFromPath( u8 *ucPath )
{
    u16 i, uiLen;
    u8 *ucParams[32], *ucSubPath, ucLen, ucAbsolute;

    if( ucPath[ 0 ] == '/' ){
        ucAbsolute = TRUE;
    } else{
        ucAbsolute = FALSE;
    } // else
    uiLen = 1 + len( ucPath );
    ucLen = getAllSubStrings( ucPath, ucParams, 32, FILESEPARATORS );
    if( !ucLen ){
        return( NULL );
    } // if
    ucSubPath = malloc( uiLen - sizeof( ucParams[ ucLen - 1 ] ) );
    if( !ucSubPath ){
        return( NULL );
    } // if
    
    strcpy( ucSubPath, "" );
    for( i = 0; i < ucLen-1; i++ ){
        if( i || ucAbsolute ){
            ucSubPath = strcat( "/", ucSubPath );
        } // else
        ucSubPath = strcat( ucParams[ i ], ucSubPath );
    } // for
}

u8 *fileGetNameFromPath( u8 *ucPath )
{
    u8 *ucParams[32], *ucName, ucLen;

    ucLen = getAllSubStrings( ucPath, ucParams, 32, FILESEPARATORS );
    if( !ucLen ){
        return( NULL );
    } // if
    ucName = malloc( sizeof( ucParams[ ucLen - 1 ] ) );
    if( !ucName ){
        return( NULL );
    } // if

    strcpy( ucName, ucParams[ ucLen - 1 ] );

    return( ucName );
}

PYGMYFILE *fileOpen( u8 *ucName, u8 ucAttrib, u32 ulFileLen )
{
    PYGMYFILEPROPERTIES *pygmyProperties;
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILE *pygmyFile, pygmyParent;
    u32 ulLen;//, *ulSectors;
    u16 uiNameLen, uiPathLen;
    u8 i, *ucFileName, *ucFilePath, ucLen, *ucParams[32];

    print( COM3, "\r\tAttributes" );
    if( ucAttrib & READ ){
        print( COM3, " READ" );
    } // if
    if( ucAttrib & WRITE ){
        print( COM3, " WRITE" );
    } // if
    if( ucAttrib & APPEND ){
        print( COM3, " APPEND" );
    } // if
    
    pygmyFile = malloc( sizeof( PYGMYFILE ) );
    
    if( !pygmyFile ){
        free( pygmyFile );
        return( NULL );
    } // if
    
    ucFileName = fileGetNameFromPath( ucName );
    if( !ucFileName ){
        return( NULL );
    } // if
    ucFilePath = fileGetPathFromPath( ucName );
    if( !ucFilePath ){
        return( NULL );
    } // if
    pygmyFile->Index = 0;
    pygmyFile->Properties.Sectors = NULL;
    pygmyFile->Properties.SectorGroups = 0;
    pygmyFile->Properties.SectorCount = 0;
    pygmyFile->Properties.Length = 0;
    pygmyFile->Properties.Attributes = ucAttrib;
    pygmyFile->Properties.Name = ucFileName;
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rName: %s", ucFileName );
        print( COM3, "\rPath: %s", ucFilePath );
    //#endif
    if( ( ucAttrib & WRITE ) && !(ucAttrib & APPEND ) ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rWRITE" );
        //#endif
        fileDelete( ucName );
        // Seek after as a safety measure
        if( fileSeekPath( ucFilePath, &pygmyVolume, &pygmyParent.Properties ) ){
            print( COM3, "\rfileOpen()->Found Path" );
            pygmyParent.MountPoint = pygmyVolume;
            pygmyFile->MountPoint = pygmyVolume;
            ulLen = 1 + ( ulLen / ( pygmyVolume->Desc.SectorSize - 2 ) );
            print( COM3, "\rAllocating Sectors" );
            fileAllocateSectors( pygmyFile, ulLen ); // Loads Sectors, SectorGroups and SectorCount
            print( COM3, "..." );
            pygmyProperties = &pygmyFile->Properties;
            pygmyFile->Properties.Attributes = ucAttrib;
            print( COM3, "\rfileOpen()->Writing Entry" );
            fileWriteEntry( &pygmyParent, &pygmyFile->Properties );
            print( COM3, "..." );
            pygmyFile->Properties.Length = ulFileLen;
            //if( ulFileLen ){
            //    print( COM3, "\rfileOpen()->Writing Length" );
            //    fileWriteLength( pygmyFile );
            //    print( COM3, "..." );
            //} // if
        } else{
            //#ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rPath seek failed!" );
            //#endif
        } // else
    } // if
    
    /*print( COM3, "\r\tAttributes" );
    if( pygmyFile->Properties.Attributes & READ ){
        print( COM3, " READ" );
    } // if
    if( pygmyFile->Properties.Attributes & WRITE ){
        print( COM3, " WRITE" );
    } // if
    if( pygmyFile->Properties.Attributes & APPEND ){
        print( COM3, " APPEND" );
    } // if
    */
    //print( COM3, "\rfileOpen() -> File Length: %d", pygmyFile->Properties.Length );
     // Now we attempt to open the entire path, including name
    if( fileSeekPath( ucName, &pygmyVolume, &pygmyFile->Properties ) ){
        // File is valid and loaded into pygmyFile
        //print( COM3, "\rfileOpen() -> File Length: %d", pygmyFile->Properties.Length );
        pygmyFile->MountPoint = pygmyVolume;
        if( ucAttrib & APPEND ){
            pygmyFile->Index = pygmyFile->Properties.Length;
        } else{
            pygmyFile->Index = 0;
        } // else
    /*print( COM3, "\r\tAttributes" );
    if( pygmyFile->Properties.Attributes & READ ){
        print( COM3, " READ" );
    } // if
    if( pygmyFile->Properties.Attributes & WRITE ){
        print( COM3, " WRITE" );
    } // if
    if( pygmyFile->Properties.Attributes & APPEND ){
        print( COM3, " APPEND" );
    } // if
    */
    } else{
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rName seek failed!" );
        //#endif
        free( pygmyFile );
        //free( ulSectors );
        free( ucFileName );
        free( ucFilePath );

        return( NULL );
    } // else
    print( COM3, "\r\tAttributes" );
    if( pygmyFile->Properties.Attributes & READ ){
        print( COM3, " READ" );
    } // if
    if( pygmyFile->Properties.Attributes & WRITE ){
        print( COM3, " WRITE" );
    } // if
    if( pygmyFile->Properties.Attributes & APPEND ){
        print( COM3, " APPEND" );
    } // if

    pygmyFile->Properties.Attributes = ucAttrib;
    print( COM3, "\rExiting fileOpen()" );
    print( COM3, "\r\tLength of %d bytes", pygmyFile->Properties.Length );
    print( COM3, "\r\tAttributes" );
    if( pygmyFile->Properties.Attributes & READ ){
        print( COM3, " READ" );
    } // if
    if( pygmyFile->Properties.Attributes & WRITE ){
        print( COM3, " WRITE" );
    } // if
    if( pygmyFile->Properties.Attributes & APPEND ){
        print( COM3, " APPEND" );
    } // if

    return( pygmyFile );
}

u8 fileWriteLength( PYGMYFILE *pygmyFile )
{
    // Each Len packet is marked as a special Chunk: [ ID 3B ][ 254 ]
    // Each Len chunk contains 6 4 byte bigendian tags
    // After a tag is used, it is set to 0x00000000
    // After all 6 tags are used, ID is set to 0x00000000 and a new chunk is created
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILE pygmyParent;
    u64 ullAddress;
    u32 ulID, ulLength;
    u8 i, ucBuffer[ 24 ];
    
    print( COM3, "\rEntering fileWriteLength" );
    memset( ucBuffer, 0xFF, 24 );
    
    ulID = pygmyFile->Properties.ID; //( pygmyFile->Properties.ID & 0xFFFFFF00 ) | 254;
    print( COM3, "\rSeeking ID: 0x%08X", ulID );
    pygmyParent.MountPoint = pygmyFile->MountPoint;
    fileCopyProperties( &pygmyFile->MountPoint->Properties, &pygmyParent.Properties );
    pygmyParent.Properties.ID = ulID;
    if( fileSeekChunk( &pygmyParent, &ullAddress, 254 )){ //ulID, pygmyEntry.Sectors, pygmyEntry.SectorCount );
        // This is a normal condition for a file that has previously been flushed or closed
        // Now skip ID and load buffer
        print( COM3, "\rLoading buffer from Address 0x%012llX\r", ullAddress );
        pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullAddress, ucBuffer, 24 );
        //ullAddress += 3;
        for( i = 0; i < 24; i++ ){
            // Iterate through Length Tags
            ulLength = convertBufferToU32( ucBuffer + i, BIGENDIAN );
            print( COM3, "\t0x%08X", ulLength );
            if( ulLength == 0xFFFFFF ){
                //ullAddress += ( ( i + 1 ) * 4 );
                print( COM3, "\rFound open entry at: 0x%012llX", ullAddress + i );
                print( COM3, "\rWriting length: %d", pygmyFile->Properties.Length );
                pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress + i, pygmyFile->Properties.Length );
                return( TRUE );
            } // if
        } // for
        // If we fell through, we need to delete and write a new chunk
        // First Delete current chunk
        print( COM3, "\rDeleting chunk: 0x%012llX", ullAddress );
        pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, 0x00000000 );
    } else if( pygmyFile->Properties.Length == 0 ){
        // if there is no length entry and length is 0, return without changing anything
        return( TRUE );
    } // else
    // Next write ID and file Length to buffer
    print( COM3, "\rWriting ID: 0x%08X", ulID );
    memset( ucBuffer, 0xFF, 32 );
    convertU32ToBuffer( ulID, ucBuffer, BIGENDIAN );
    ucBuffer[ 4 ] = 254;
    convertU32ToBuffer( pygmyFile->Properties.Length, ucBuffer + 8, BIGENDIAN );
    // Finally, write buffer as new chunk
    print( COM3, "\rWriting Length: %d", pygmyFile->Properties.Length ); 
    print( COM3, "\rWriting Chunk:" );
    for( i = 0; i < 32; i++ ){
        print( COM3, " 0x%02X", ucBuffer[ i ] );
    } // for

    fileWriteChunk( &pygmyParent, ucBuffer );
    print( COM3, "\rfileWriteLength()->Done" );
    return( TRUE );
}
/*
u32 fileReadLength( PYGMYFILE *pygmyFile )
{
    PYGMYFILE pygmyParent;
    u64 ullAddress;
    u32 ulLength, ulID;
    u16 i;
    u8 ucBuffer[ 32 ];

    print( COM3, "\rEntering fileLoadLength" );
    ulID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | 254;
    print( COM3, "\rSeeking ID: 0x%08X", ulID );
    pygmyParent.MountPoint = pygmyFile->MountPoint;
    fileCopyProperties( &pygmyFile->MountPoint->Properties, &pygmyParent.Properties );
    pygmyParent.Properties.ID = ulID;
    //fileCopyProperties( &pygmyFile->MountPoint->Properties, &pygmyParent.Properties );
    //pygmyParent.Properties.ID = ( pygmyParent.Properties.ID & 0xFFFFFF00 | 254 );
    filePrintProperties( &pygmyParent.Properties );
    if( !fileSeekChunk( &pygmyParent, &ullAddress ) ){
        // This isn't technically an error condition, a new file isn't required to have a length entry until
        //    data is first written to the file, so, no entry means length is 0
        print( COM3, "\rNo chunk found" );
        return( 0 );
    } // if
    // Now skip ID and load buffer
    print( COM3, "\r\tullAddress: 0x%012llX", ullAddress );
    pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullAddress + 4, ucBuffer, 28 );
    for( i = 0; i < 6; i++ ){
        // Iterate through Length Tags
        ulLength = convertBufferToU32( ucBuffer + ( i * 4 ), BIGENDIAN );
        if( ulLength != 0xFFFFFF && ulLength != 0x00000000 ){
            print( COM3, "\r\tFile Length Found: %d", ulLength );
            return( ulLength);
        } // if
    } // for

    return( 0 );
}*/

/*u32 fileReadLength( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry )//u32 ulID, u32 *ulSectors, u32 ulSectorCount )
{
    PYGMYFILE pygmyParent;
    u64 ullAddress;
    u32 ulLength;
    u8 i, ucBuffer[ 28 ];

    //ulID = ( ulID & 0xFFFFFF00 ) | 254;
    pygmyParent.MountPoint = pygmyVolume;
    fileCopyProperties( pygmyEntry, &pygmyParent.Properties );
    pygmyParent.Properties.ID = ( pygmyParent.Properties.ID & 0xFFFFFF00 | 254 );
    if( fileSeekChunk( &pygmyParent, &ullAddress ) ){//pygmyVolume, ulID, ulSectors, ulSectorCount ) ){
    //if( ulAddress == 0xFFFFFFFF ){
        // This isn't technically an error condition, a new file isn't required to have a length entry until
        //    data is first written to the file, so, no entry means length is 0
        return( 0 );
    } // if
    // Now skip ID and load buffer
    pygmyVolume->IO->GetBuffer( pygmyVolume->Port, ullAddress + 4, ucBuffer, 28 );
    for( i = 0; i < 6; i++ ){
        // Iterate through Length Tags
        ulLength = convertBufferToU32( ucBuffer + ( i * 4 ), BIGENDIAN );
        if( ulLength != 0xFFFFFF && ulLength != 0x00000000 ){
            return( ulLength);
        } // if
    } // for

    return( 0 );
}*/

u8 fileClose( PYGMYFILE *pygmyFile )
{
    if( pygmyFile->Properties.Attributes & WRITE ){
        // Only write length if the file has write access
        print( COM3, "\rClosing in WRITE mode..." );
        fileWriteLength( pygmyFile );
    } // if
    //return( TRUE );
    if( pygmyFile->Properties.Sectors ){
        print( COM3, "\rFreeing %d Sector Groups", pygmyFile->Properties.SectorGroups );
        free( pygmyFile->Properties.Sectors );
        print( COM3, "..." );
    } // if
    if( pygmyFile->Properties.Name ){
        print( COM3, "\rFreeing %s", pygmyFile->Properties.Name );
        free( pygmyFile->Properties.Name );
        print( COM3, "..." );
    } // if
    /*if( pygmyFile->Properties.Path ){
        print( COM3, "\rFreeing Path" );
        free( pygmyFile->Properties.Path );
        print( COM3, "..." );
    } // if
    */
    if( pygmyFile ){
        print( COM3, "\rFreeing File" );
        free( pygmyFile );
        print( COM3, "..." );
    } // if

    return( TRUE );
}

u8 fileFlush( PYGMYFILE *pygmyFile )
{
    // ToDo: Add code to handle writing of resource buffer, if applicable
    fileWriteLength( pygmyFile );

    return( TRUE );
}

u8 fileRename( u8 *ucName, u8 *ucNewName )
{

}



u8 fileCopy( u8 *ucFrom, u8 *ucTo )
{

}

u8 fileGetChar( PYGMYFILE *pygmyFile )
{
    u8 ucChar;

    fileReadBytes( pygmyFile, &ucChar, 1 );

    return( ucChar );
}

u8 fileGetBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen )
{
    fileReadBytes( pygmyFile, ucBuffer, uiLen );

    return( TRUE );
}

u8 *fileGetString( PYGMYFILE *pygmyFile )
{
    u16 i;
    u8 *ucString, ucChar;
    
    /*ucString = malloc( 13 );
    fileReadBytes( pygmyFile, ucString, 12 );
    ucString[ 12 ] = '\0';
    return( ucString );*/
    // First eliminate non printable characters
    ucString = malloc( 13 );
    /*for( i = 0; i < 12; i++ ){
        fileReadBytes( pygmyFile, &ucChar, 1 );
        //if( !fileReadBytes( pygmyFile, &ucChar, 1 ) ){
        //    break;
        //} // if
        ucString[ i ] = ucChar;
        ucString[ i + 1 ] = '\0';
        print( COM3, "(%s)", ucString );
    } // for
    */
    ucString = malloc( 1 );
    if( !ucString ){
        print( COM3, "\rfileGetString->mem fail" );
        return( NULL );
    } // if
    ucString[ 0 ] = ucChar;

    for( i = 1; i < 1024; i++ ){
        //print( COM3, "+" );
        if( !fileReadBytes( pygmyFile, &ucChar, 1 ) ){
            break;
        } // if
        //print( COM3, "%c", ucChar );
        if( !pygmyFile->Properties.Attributes & EOF ){
            //print( COM3, "\rfileGetString->EOF" );
            return( NULL );
        } // if
        if( isPrintable( ucChar ) ){
            ucString[ i ] = ucChar;
            ucString = realloc( ucString, i + 2 );
        } else{
            ucString[ i+1 ] = '\0';
            return( ucString );
            //return( ucString );
        } // else
    } // for
    
    return( NULL );
}

u8 fileReadBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen )
{
    u64 ullAddress;
    u32 i, ulSectorSize;
    u16 uiBufLen;

    if( ( pygmyFile->Properties.Length + uiLen ) >= ( ( ulSectorSize - 2 ) * pygmyFile->Properties.SectorCount ) ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rAllocating another sector" );
        //#endif
    } // if
    for( i = 0; i < uiLen; ){
        // Calculate parameters for buffer write to flash
        uiBufLen = fileGenerateAddress( pygmyFile, &ullAddress );
       // print( COM3, "\rullAddress = 0x%012llX, uiBufLen = %d, uiLen = %d", ullAddress, uiBufLen, uiLen );
        if( uiBufLen == 0 ){
            //print( COM3, "\ruiBufLen = 0" );
            return( FALSE );
        } // if
        
        if( uiBufLen + i > uiLen ){
            uiBufLen = uiLen - i;
        } // if
        pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port,
            ullAddress, ucBuffer+i, uiLen );
        pygmyFile->Index += uiLen;
        //pygmyFile->Index += uiBufLen;
        i += uiBufLen;
        //print( COM3, "\rpygmyFile->Index = %d", pygmyFile->Index );
    } // for
    
    return( TRUE );
}

u8 fileWriteBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen )
{
    // This function is only for internal use!!!
    // Use filePut functions in user code, fileWriteBytes is optimized for multi-byte write oeprations
    //PYGMYFILEADDRESS *pygmyAddress
    u64 ullAddress;
    u32 i;//, ulSectorSize;
    u16 uiBufLen;
    
    if( ( pygmyFile->Properties.Length + uiLen ) >= ( ( pygmyFile->MountPoint->Desc.SectorSize - 2 ) * pygmyFile->Properties.SectorCount ) ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rAllocating another sector" );
        
        fileAppendSectors( pygmyFile, 1 );
        //#endif
    } else{
        print( COM3, "\rWriting Bytes, Length %d < Allocated Length %d", ( pygmyFile->Properties.Length + uiLen ), ( ( pygmyFile->MountPoint->Desc.SectorSize - 2 ) * pygmyFile->Properties.SectorCount ) );
        print( COM3, "\r\tpygmyFile->Properties.Length = %d", pygmyFile->Properties.Length );
        print( COM3, "\r\tuiLen = %d", uiLen );
        print( COM3, "\r\tpygmyFile->MountPoint->Desc.SectorSize = %d", pygmyFile->MountPoint->Desc.SectorSize );
        print( COM3, "\r\tpygmyFile->Properties.SectorCount = %d", pygmyFile->Properties.SectorCount );
    } // else
    for( i = 0; i < uiLen; ){
        // Calculate parameters for buffer write to flash
        uiBufLen = fileGenerateAddress( pygmyFile, &ullAddress );
        if( uiBufLen + i > uiLen ){
            uiBufLen = uiLen - i;
        } // if
        pygmyFile->MountPoint->IO->PutBuffer( pygmyFile->MountPoint->Port,
            ullAddress, ucBuffer+i, uiBufLen );
        pygmyFile->Index += uiBufLen;
        pygmyFile->Properties.Length += uiBufLen;
        i += uiBufLen;
    } // for
    //print( COM3, "\rDone writing bytes" );
    
    return( TRUE );
}

u8 filePutChar( PYGMYFILE *pygmyFile, u8 ucData )
{
    // Immutable means immutable! Flash files do not support random write access


    // Do not allow write within written space of Flash, allow RAM devices to continue
    if( ( pygmyFile->MountPoint->Desc.Attributes & MEDIAFLASH ) && pygmyFile->Index != pygmyFile->Properties.Length ){
        return( FALSE );
    } // if
    // Check that there is space in the current sectors before writing
    //ulSectorSize = pygmyFile->MountPoint->Desc.SectorSize - 2;

    //if( ( pygmyFile->Properties.Length + 1 ) >= ( ulSectorSize * pygmyFile->Properties.SectorCount ) ){
    //    putstr( "\rAllocating another sector" );
    //} // if
    //ulSector = pygmyFile->Index / ulSectorSize;
    //ulAddress = pygmyFile->Properties.Sectors[ ulSector ] +
    //    ( ( pygmyFile->Index % ulSectorSize ) - ( ulSectorSize * 2 ) );
    //pygmyFile->MountPoint->IO->PutChar( pygmyFile->MountPoint->Port, ulAddress, ucData );
    //++pygmyFile->Index;
    //++pygmyFile->Properties.Length;
    fileWriteBytes( pygmyFile, &ucData, 1 );

    return( TRUE );
}

u16 fileGetWord( PYGMYFILE *pygmyFile, u8 ucEndian )
{

}

u8 filePutWord( PYGMYFILE *pygmyFile, u16 uiData, u8 ucEndian )
{
    u8 ucBuffer[ 2 ];

    if( ucEndian == BIGENDIAN ){
        ucBuffer[ 0 ] = (u8)((u16)uiData >> 8);
        ucBuffer[ 1 ] = (u8)((u16)uiData);
    } else{
        ucBuffer[ 0 ] = (u8)((u16)uiData);
        ucBuffer[ 1 ] = (u8)((u16)uiData >> 8);
    } // else

    return( fileWriteBytes( pygmyFile, ucBuffer, 2 ) );
}

u32 fileGetLong( PYGMYFILE *pygmyFile, u8 ucEndian )
{

}

u8 filePutLong( PYGMYFILE *pygmyFile, u32 ulData, u8 ucEndian )
{
    u8 ucBuffer[ 4 ];

    if( ucEndian == BIGENDIAN ){
        ucBuffer[ 0 ] = (u8)((u32)ulData >> 24);
        ucBuffer[ 1 ] = (u8)((u32)ulData >> 16);
        ucBuffer[ 2 ] = (u8)((u32)ulData >> 8);
        ucBuffer[ 3 ] = (u8)((u32)ulData);
    } else{
        ucBuffer[ 0 ] = (u8)((u32)ulData);
        ucBuffer[ 1 ] = (u8)((u32)ulData >> 8);
        ucBuffer[ 2 ] = (u8)((u32)ulData >> 16);
        ucBuffer[ 3 ] = (u8)((u32)ulData >> 24);
    } // else

    return( fileWriteBytes( pygmyFile, ucBuffer, 4 ) );
}

/*u8 fileGetBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{

}*/

u8 filePutBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{
    return( fileWriteBytes( pygmyFile, ucBuffer, uiLen ) );
}

