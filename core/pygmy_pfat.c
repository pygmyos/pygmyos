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
    u8 *ucPath, ucParamCount, *ucParams[ 32 ];

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
        pygmyVolume = fileSeekPath( ucBuffer, 0, &pygmyEntry );
        if( !pygmyVolume ){
            return( FALSE );
        } // if
        fileSetCurrentMountPoint( pygmyVolume );
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
            pygmyVolume = fileSeekPath( ucPath, 0, &pygmyEntry );
            if( pygmyVolume ){
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
    /*static u8 *globalCurrentPath = NULL;

    if( globalCurrentPath ){
        free( globalCurrentPath );
        globalCurrentPath = NULL;
    } // if
    if( globalMountPointCurrent ){
        globalCurrentPath = malloc( 3 + len( globalMountPointCurrent->Properties.Path ) + len( globalMountPointCurrent->Properties.Name ) );
        if( globalCurrentPath ){
            copyString( "/", globalCurrentPath );
            appendString( globalMountPointCurrent->Properties.Name, globalCurrentPath );
            if( globalMountPointCurrent->Properties.Path != NULL && len( globalMountPointCurrent->Properties.Path ) > 0 ){
                appendString( "/", globalCurrentPath );
                appendString( globalMountPointCurrent->Properties.Path, globalCurrentPath );
            } // if
        } // if

    } else{
        globalCurrentPath = malloc( 2 );
        if( globalCurrentPath ){
            copyString( "/", globalCurrentPath );
        } // if
    } //else

    return( globalCurrentPath );*/

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
    //u32 ulID, ulLen, ulAddrData, *ulAddrPtr;
    va_list vaList;
    u16 uiMemLen;
    //u8 *ucBuffer;

    //if( globalMountPoints || globalMountPointCount == 0 ){
    if( globalMountPointCount == 0 ){
        uiMemLen = sizeof( PYGMYFILEVOLUME );
        pygmyVolume = malloc( uiMemLen );
        memset( pygmyVolume, 0, uiMemLen );
        //pygmyVolume = sysAllocate( sizeof( PYGMYFILEVOLUME ) );
    } else{
        //pygmyVolume = sysReallocate( globalMountPoints, sizeof( PYGMYFILEVOLUME ) * ( globalMountPointCount + 1 ) );
        pygmyVolume = realloc( globalMountPoints, sizeof( PYGMYFILEVOLUME ) * ( globalMountPointCount + 1 ) );
    } // else
    if( pygmyVolume ){
        globalMountPoints = pygmyVolume;
        pygmyVolume = &globalMountPoints[ globalMountPointCount ];
        va_start( vaList, ucName );
        print( COM3, "\rOpening Port" );
        pygmyMEMIO->VOpen( &pygmyVolume->Port, ulAddress, vaList );
        print( COM3, "..." );
        va_end( vaList );
        print( COM3, "\rva_end" );
        pygmyVolume->IO = pygmyMEMIO;
        pygmyMEMIO->Desc( pygmyVolume->Port, &pygmyVolume->Desc );

        //fileFormat( pygmyVolume, "nebula" );

        pygmyVolume->Properties.ID = 0xFFFFFF00 & pdiaEncodeString( ucName );
        //pygmyVolume->Properties.Sectors = sysAllocate( sizeof( PYGMYFILEADDRESS ) );
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

        if( fileLoadEntry( pygmyVolume, &pygmyVolume->Properties ) ){
            //filePrintProperties( &pygmyVolume->Properties );
        } else{
            //putstr( "\rFailed to Mount Volume" );
            print( COM3, "\rFailed to Mount Volume" );
            //return( FALSE );
        } // else

        //pygmyVolume->Properties.Path = sysAllocate( 1 ); // Path is always freed and allocated
        /*pygmyVolume->Properties.Path = malloc( 2 + len( pygmyVolume->Properties.Name ) ); // Path is always freed and allocated
        //pygmyVolume->Properties.Path[ 0 ] = '\0';
        if( pygmyVolume->Properties.Path ){
            copyString( "/", pygmyVolume->Properties.Path );
            appendString( pygmyVolume->Properties.Name, pygmyVolume->Properties.Path );
        } // if*/
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
    u32 i, ulEntryLen;
    u16 uiMemLen;
    //, ii, ulSP, ulSPLen, *ulPtr
    u8 *ucBuffer;

    pygmyFile.MountPoint = pygmyVolume;
    fileCopyProperties( pygmyEntry, &pygmyFile.Properties );
    pygmyFile.Properties.Name = "";
    pygmyFile.Properties.Path = "";
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rCalling fileGetEntry from fileLoadEntry: " );
    #endif
    ulEntryLen = fileGetEntry( &pygmyFile, &ucBuffer );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntry dump of %d bytes", ulEntryLen );
        for( i = 0; i < ulEntryLen; i++ ){
            print( COM3, "(%02X)", ucBuffer[ i ] );
        } // for
    #endif
    //ulEntryLen = fileGetEntry( pygmyVolume, pygmyEntry->ID, pygmyEntry->Sectors, pygmyEntry->SectorCount, &ucBuffer );
    if( !ulEntryLen ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rEntry Len of 0" );
        #endif
        return( FALSE );
    } // if
    //tmpEntry.Name = sysAllocate( 1 + ucBuffer[ 5 ] );
    if( ucBuffer[ 4 ] == 0xFF || ucBuffer[ 5 ] == 0xFF || ucBuffer[ 6 ] == 0xFF ){
        // Entry slot is erased
        print( COM3, "\rLoaded erased entry" );
        return( FALSE );
    } // if
    uiMemLen = 1 + ucBuffer[ 5 ];
    pygmyEntry->Name = malloc( uiMemLen );
    memset( pygmyEntry->Name, 0, uiMemLen );
    //tmpEntry.Sectors = sysAllocate( sizeof( PYGMYFILEADDRESS ) );
    pygmyEntry->SectorCount = 0;
    uiMemLen = sizeof( PYGMYFILEADDRESS );
    pygmyEntry->Sectors = malloc( uiMemLen );
    memset( pygmyEntry->Name, 0, uiMemLen );
    pygmyEntry->Time = convertBufferToU32( ucBuffer, BIGENDIAN );
    pygmyEntry->Attributes = ucBuffer[ 4 ];
    copyBuffer( ucBuffer + 6, pygmyEntry->Name, ucBuffer[ 5 ] );
    pygmyEntry->Name[ ucBuffer[ 5 ] ] = '\0';
    i = 6 + ucBuffer[ 5 ];
    i += ( ( i + 4 ) % 7 );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSorting Sector Groups..." );
    #endif
    for( pygmyEntry->SectorGroups = 0; i < ulEntryLen; ){
        fileLoadAddress( &pygmyEntry->Sectors[ pygmyEntry->SectorGroups ], ucBuffer+i );
        #ifdef __PYGMY_DEBUG_FILE
          filePrintAddress( &pygmyEntry->Sectors[ pygmyEntry->SectorGroups ] );
        #endif
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

    /*pygmyEntry->Sectors         = tmpEntry.Sectors;
    pygmyEntry->SectorGroups    = tmpEntry.SectorGroups;
    pygmyEntry->SectorCount     = tmpEntry.SectorCount;
    pygmyEntry->Time            = tmpEntry.Time;

    pygmyEntry->Path            = tmpEntry.Path;
    pygmyEntry->Name            = tmpEntry.Name;
    pygmyEntry->Attributes      = tmpEntry.Attributes;
  */
    //sysFree( ucBuffer );
    free( ucBuffer );

    return( TRUE );
}

PYGMYFILEVOLUME *fileSeekPath( u8 *ucPath, s8 sNest, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Path Rules:
    // 1. If the first char is '/', first substring must be name of mountpoint
    // 2. If the first char is '/' and there are no substrings, path points to root of current mountpoint
    // 3. If the first char is not '/' and there are no substrings, path points to path of current mountpoint
    // 4. If the first char is not '/' and there are substrings, path becomes path of current mountpoint
    // Note that no permanent changes are made to mountpoint or it's path

    PYGMYFILEVOLUME *pygmyVolume;
    u32 ulID;
    u16 i, uiMemLen;
    u8 ucMax, ucLen, *ucFullPath, *ucParams[ 32 ];

    print( COM3, "\rEntering fileSeekPath(%s)", ucPath );
    if( ucPath == NULL || len( ucPath ) == 0 ){
        print( COM3, "\rucPath is NULL" );
        pygmyVolume = fileGetCurrentMountPoint( );
        //
        fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
        //
        return( pygmyVolume );
    } // if
    ucPath = removeLeadingWhitespace( ucPath );
    // All paths will be created equal, start by detecting path type
    if( ucPath[ 0 ] != '/' ){
        // Path is relative to current path, so get and composite with current path
        print( COM3, "\rPath is Relative" );
        uiMemLen = 2 + len( ucPath ) + len( fileGetCurrentPath() );
        ucFullPath = malloc( uiMemLen );

        if( !ucFullPath ){
            return( NULL );
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
        print( COM3, "\rPath is Absolute" );
        uiMemLen = 1 + len( ucPath );
        ucFullPath = malloc( uiMemLen );

        if( !ucFullPath ){
            return( NULL );
        } // if
        memset( ucFullPath, 0, uiMemLen );
        copyString( ucPath, ucFullPath );

    } // else
    // Now we have a full path, time to separate volume name and step through folders
    print( COM3, "\rComposite path: %s", ucFullPath );
    ucLen = getAllSubStrings( ucFullPath, ucParams, 32, FILESEPARATORS|NEWLINE );
    if( ucLen ){
        ulID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ 0 ] ) );
        ucMax = fileGetMountPointCount();
        print( COM3, "\rSearching %d volumes for %s ", ucMax, ucParams[ 0 ] );
        for( i = 0; i < ucMax; i++ ){
            pygmyVolume = fileGetMountPoint( i );
            if( pygmyVolume->Properties.ID == ulID ){
                print( COM3, "Done." );
                break;
            } // if
        } // for
        if( i == ucMax ){
            return( NULL );
        } // if
    } else{
        print( COM3, "\rPath stops at root" );
        pygmyVolume = fileGetCurrentMountPoint();
        fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
        return( pygmyVolume );
    } // else
    fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
    print( COM3, "\rBranching through %d nested entries", ucLen );
    for( i = 1; i < ucLen; i++ ){
        ulID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ i ] ) );
        print( COM3, "\rSearching for %s", ucParams[ i ] );
        pygmyEntry->ID = ulID;
        if( !fileLoadEntry( pygmyVolume, pygmyEntry ) ){
            print( COM3, "\rError: Entry %s not found!", ucParams[ i ] );
            return( NULL ); // Invalid path
        } else if( !(pygmyEntry->Attributes & FOLDER) && (i + 1) != ucLen ){
            // If loaded entry isn't a folder, then you can't branch any deeper
            // If this is the last param, it doesn't matter, otherwise it's a path error
            print( COM3, "\rError: Found file in path!" );
            return( NULL );
        } // else if
    } // for
    print( COM3, "\rDone" );
    print( COM3, "\rLoaded the following properties:" );
    filePrintProperties( pygmyEntry );
    return( pygmyVolume );
}
/*
PYGMYFILEVOLUME *fileSeekPath( u8 *ucPath, PYGMYFILEPROPERTIES *pygmyEntry )
{
    // Path Rules:
    // 1. If the first char is '/', first substring must be name of mountpoint
    // 2. If the first char is '/' and there are no substrings, path points to root of current mountpoint
    // 3. If the first char is not '/' and there are no substrings, path points to path of current mountpoint
    // 4. If the first char is not '/' and there are substrings, path becomes path of current mountpoint
    // Note that no permanent changes are made to mountpoint or it's path
    //PYGMYFILEPROPERTIES *pygmyParentEntry;
    PYGMYFILEVOLUME *pygmyVolume;
    u32 i, ulMax;
    u8 ucLen, *ucParams[ 32 ], ucParamIndex;

    print( COM3, "\rEntering fileSeekPath(%s)", ucPath );
    if( ucPath == NULL || len( ucPath ) == 0 ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rDefault Relative Path" );
        //#endif
        pygmyVolume = fileGetCurrentMountPoint( );
        fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
        return( pygmyVolume );
    } // if
    ucLen = getAllSubStrings( ucPath, ucParams, 32, FILESEPARATORS|NEWLINE );
    ucParamIndex = 0;
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSubstrings: %d", ucLen );
        for( i = 0; i < ucLen; i++ ){
            print( COM3, "\rSubString %d: %s", i, ucParams[ i ] );
        } // for
    //#endif
    ucPath = removeLeadingWhitespace( ucPath );
    if( ucPath[ 0 ] == '/' ){
        //#ifdef __PYGMY_DEBUG_FILE
          print( COM3, "\rCase1: Absolute" );
        //#endif
        // Absolute path, resolve mountpoint first
        if( ucLen ){ // Case 1
            pygmyEntry->ID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ ucParamIndex++ ] ) );
            //#ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rCase1" );
                print( COM3, "\rSeeking SubString %d: %s", ucParamIndex-1, ucParams[ ucParamIndex-1 ] );
                print( COM3, "\rSeeking ID: 0x%08X", pygmyEntry->ID );
            //#endif
            ulMax = fileGetMountPointCount();
            for( i = 0; i < ulMax; i++ ){
                pygmyVolume = fileGetMountPoint( i );
                if( pygmyVolume->Properties.ID == pygmyEntry->ID ){
                    //#ifdef __PYGMY_DEBUG_FILE
                        print( COM3, "\rFound MountPoint @ %d", pygmyEntry->ID );
                    //#endif
                    // Found mountpoint, time to continue
                    break;
                } // if
            } // for
            if( i == ulMax ){
                //#ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rNo MountPoint" );
                //#endif
                return( NULL ); // There is no match for requested mountpoint
            } // if
        } else{ // Case 2
            //#ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rCase2: Absolute Local" );
            //#endif
            pygmyVolume = fileGetCurrentMountPoint( );
            if( !pygmyVolume ){
               // #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rNo MountPoint" );
               // #endif
                return( NULL ); // There is no current mountpoint
            } // if
        } // else
    } else{
        // Relative path, mountpoint is current mountpoint
        pygmyVolume = fileGetCurrentMountPoint( );
        if( !pygmyVolume ){
            #ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rNo MountPoint" );
            #endif
            return( NULL ); // There is no current mountpoint
        } // if
        // Case 4 is effectively default and drops through
        if( !ucLen ){ // Case 3
            ucLen = getAllSubStrings( pygmyVolume->Properties.Path, ucParams, 32, FILESEPARATORS|NEWLINE );
        } // if
    } // else
    // We now have a mountpoint and path
    if( !ucLen || ucLen == ucParamIndex ){
        // In this case the mountpoint file entry is what we are looking for
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rMountPoint found" );
        #endif
              //filePrintProperties( &pygmyVolume->Properties );
        fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
    } else{
        // In this case we must resolve remainder of path and load file entry for last parameter
        for( ; ucParamIndex < ucLen && pygmyVolume; ucParamIndex++ ){
            fileCopyProperties( &pygmyVolume->Properties, pygmyEntry );
            pygmyEntry->ID = ( 0xFFFFFF00 & pdiaEncodeString( ucParams[ ucParamIndex++ ] ) );
            if( !fileLoadEntry( pygmyVolume, pygmyEntry ) ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rNo MountPoint" );
                #endif
                return( NULL ); // Invalid path
            } else if( !(pygmyEntry->Attributes & FOLDER) && ucParamIndex + 1 != ucLen ){
                // If loaded entry isn't a folder, then you can't branch any deeper
                // If this is the last param, it doesn't matter, otherwise it's a path error
                //#ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rNot Folder" );
                //#endif
                return( NULL );
            } // else if
           // } // else
        } // for
    } // else

    return( pygmyVolume );
}*/

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
    pygmyFile.Properties.Length = 4096*4;

    for( i = 0; i < 5; i++ ){
        pygmyFile.Index = pygmyFileList->Index + 30;

        fileGenerateAddress( &pygmyFile, &ullAddress );

        ulID = pygmyFileList->MountPoint->IO->GetLong( pygmyFileList->MountPoint->Port, ullAddress );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\ri %d Index %d ID 0x%08X from 0x%012llX", i, pygmyFile.Index, ulID, ullAddress );
        #endif
        if( ulID != 0x00000000 && (ulID & 0x000000FF ) == 0 ){
            pygmyFileList->Properties.ID = ulID;
            fileLoadEntry( pygmyFileList->MountPoint, &pygmyFileList->Properties );
            ii = (( len( pygmyFileList->Properties.Name ) + ( pygmyFileList->Properties.SectorGroups * 7 ) ) ) ;
            pygmyFileList->Index += 32;// ii;

            return( &pygmyFileList->Properties );
        } // if
        pygmyFileList->Index += 32;
    } // for
    //ulMax = ulIndex + 255;//( pygmyParent->SectorCount * pygmyVolume->Desc.SectorSize ) / 32;
    /*for( ii = 0; ii < 255; ulIndex++ ){
        i =  ( ulIndex * 32 ) / pygmyVolume->Desc.SectorSize;
        if( i >= pygmyParent->SectorGroups ){
            return( FALSE );
        } // if
        ullAddress = pygmyParent->Sectors[ i ].BaseAddress + ( ( ulIndex * 32 ) % pygmyVolume->Desc.SectorSize );
        ulID = pygmyVolume->IO->GetLong( pygmyVolume->Port, ullAddress );
        if( (ulID & 0x000000FF ) == 0 ){
            pygmyEntry->ID = ulID;
            pygmyEntry->Sectors         = pygmyParent->Sectors;
            pygmyEntry->SectorGroups    = pygmyParent->SectorGroups;
            pygmyEntry->SectorCount     = pygmyParent->SectorCount;
            if( fileLoadEntry( pygmyVolume, pygmyEntry ) ){
                return( ++ulIndex );
            } // if
        } // if
    } // for
    */
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
    pygmyParent.Properties.Path = NULL; //malloc( 2 + len( ucName ) );
    pygmyParent.Properties.Name = NULL; //malloc( 1 + len( ucName ) );
    /*if( pygmyParent.Properties.Path == NULL || pygmyParent.Properties.Name == NULL ){
        return( FALSE );
    } // if
    copyString( "/", pygmyParent.Properties.Path );
    appendString( ucName, pygmyParent.Properties.Path );
    copyString( ucName, pygmyParent.Properties.Name );*/
    pygmyFile.MountPoint = pygmyVolume;
    pygmyFile.Properties.Sectors = NULL;
    pygmyFile.Properties.SectorGroups = 0;
    pygmyFile.Properties.SectorCount = 0;
    pygmyFile.Properties.Path = "";
    pygmyFile.Properties.Name = ucName;
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
    //fileAllocateSectors( pygmyVolume, ulSecPtr, 1, 1 );

    fileAllocateSectors( &pygmyFile, 1 );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rWriting Entry..." );
    #endif
    fileWriteEntry( &pygmyParent, &pygmyFile.Properties, (READ|WRITE|FOLDER), ucName );

    /*
    pygmyVolume->Properties.ID = 0xFFFFFF00 & pdiaEncodeString( ucName );
        pygmyVolume->Properties.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
        if( !pygmyVolume->Properties.Sectors ){
            return( FALSE );
        } // if
        pygmyVolume->Properties.Sectors[ 0 ].BaseAddress = 0;
        pygmyVolume->Properties.Sectors[ 0 ].SectorCount = 1;
        pygmyVolume->Properties.SectorGroups = 1;
        pygmyVolume->Properties.SectorCount = 1;
        pygmyVolume->Properties.Length = 0;

        if( fileLoadEntry( pygmyVolume, &pygmyVolume->Properties ) ){
        */
    free( pygmyParent.Properties.Sectors );
        /*pygmyParent.Properties.Path = malloc( 2 + len( ucName ) );
        if( pygmyParent.Properties.Path ){
            copyString( "/", pygmyParent.Properties.Path );
            appendString( ucName, pygmyParent.Properties.Path );
        } // if
        */
    /*
    if( pygmyVolume->Properties.Path ){
        free( pygmyVolume->Properties.Path );
    } // if
    if( pygmyVolume->Properties.Name ){
        free( pygmyVolume->Properties.Name );
    } // if*/
    fileCopyProperties( &pygmyFile.Properties, &pygmyVolume->Properties );

        //fileSetCurrentMountPoint( pygmyVolume );
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
    print( COM3, "\rMem Full" );
    return( FALSE );
}

u64 fileFindContiguousEmptySectors( PYGMYFILEVOLUME *pygmyVolume, u32 ulCount )
{
    u64 ullAddress;
    u32 i, ulFound;

    //filePrintProperties( &pygmyVolume->Properties );
    for( i = 1, ulFound = 0; i < pygmyVolume->Desc.Sectors;  i++){
        //for( ulFound = 0; ulFound < ulCount; ulFound++ ){
            ullAddress = fileFindNextEmptySector( pygmyVolume, i * pygmyVolume->Desc.SectorSize );
            if( !ullAddress ){
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "Fail" );
                #endif
                ulFound = 0;
                //break;
            } else{
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rFound" );
                #endif
                ++ulFound;
            } // else
        //} // for
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
    //  unknown until allocation is finished,
    PYGMYFILEADDRESS *pygmyAddress;
    u64 ullAddress;
    u32 i, ii, ulCount;
    u16 uiMemLen;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rStart Allocating" );
    #endif
    if( !pygmyFile->Properties.Sectors ){
        //pygmyAddress = sysAllocate( sizeof( PYGMYFILEADDRESS ) );
        uiMemLen = sizeof( PYGMYFILEADDRESS );
        pygmyAddress = malloc( uiMemLen );
        memset( pygmyAddress, 0, uiMemLen );
        if( !pygmyAddress ){
            return( FALSE ); // Memory Full
        } // if
    } // if
    // Subdivide number of requested sectors until suitable level of granularity is reached

    //for( i = 0; i < ulRequestedSectors && !(ulRequestedSectors & BIT31 ); ){
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
            //pygmyAddress = sysReallocate( pygmyFile->Properties.Sectors, sizeof( PYGMYFILEADDRESS ) *
            //    ( pygmyFile->Properties.SectorGroups + 1 ) );
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
    pygmyParent->Properties.ID = 0xFFFFFFFF;
    //ullAddress = 0;
    if( fileSeekChunk( pygmyParent, &ullAddress )){
        pygmyParent->MountPoint->IO->PutBuffer( pygmyParent->MountPoint->Port, ullAddress, ucChunk, 32 );
        pygmyParent->Properties.ID = ulID;
        return( TRUE );
    } // if
    pygmyParent->Properties.ID = ulID;

    return( FALSE );
}

u8 fileSeekChunk( PYGMYFILE *pygmyParent, u64 *ullAddress )
{
    u32 i, ulTmpID;
    u16 ii, iii, uiEntryCount;
    // This function seeks for an ID match ( 0xFFFFFFFF for unused, or [ ID 3B | CHUNK 1B ] )
    // If found, the address for the chunk is returned, else 0xFFFFFFFF ( since 0x00000000 is
    //  valid for the root folder
    // A special case is provided for list and search functions: The ID|CHUNK 0xFFFFFF00 will
    // return on the first occurance
    uiEntryCount = pygmyParent->MountPoint->Desc.SectorSize / 32; // 32 is length of chunks
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rSeeking 0x%08X", pygmyParent->Properties.ID );
        print( COM3, "\ruiEntryCount: %d", uiEntryCount );
    #endif
    for( i = 0; i < pygmyParent->Properties.SectorGroups; i++ ){ // This is the number of parent sectors containing chunks to sort
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\r\tIn Group %d", i );
            print( COM3, "\r\t\tSectorCount: %d", pygmyParent->Properties.Sectors[ i ].SectorCount );
            print( COM3, "\r\t\tBaseAddress: 0x%012llX", pygmyParent->Properties.Sectors[ i ].BaseAddress );
        #endif
        for( ii = 0; ii < pygmyParent->Properties.Sectors[ i ].SectorCount; ii++ ){ // Iterate through Chunks and process matches
            for( iii = 1; iii < uiEntryCount; iii++ ){
                *ullAddress = pygmyParent->Properties.Sectors[ i ].BaseAddress +
                    ( ii * pygmyParent->MountPoint->Desc.SectorSize ) + ( iii * 32 );
                ulTmpID = pygmyParent->MountPoint->IO->GetLong( pygmyParent->MountPoint->Port, *ullAddress );
                #ifdef __PYGMY_DEBUG_FILE
                    print( COM3, "\rID 0x%08X @ 0x%012llX", ulTmpID, (u64) *ullAddress );
                #endif
                if( ulTmpID == pygmyParent->Properties.ID ){
                    #ifdef __PYGMY_DEBUG_FILE
                        print( COM3, "\rFound Chunk" );
                    #endif
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
    //uiChunks = 32 + ( 32 * (( 10 + len( pygmyFile->Properties.Name ) + ( pygmyFile->Properties.SectorGroups * sizeof( PYGMYFILEADDRESS ) )) / 32 ) );
    uiChunks = 1 + ( 32 * (( 10 + len( pygmyFile->Properties.Name ) + ( pygmyFile->Properties.SectorGroups * sizeof( PYGMYFILEADDRESS ) )) / 32 ) );
    for( i = 0; i < uiChunks; i++ ){
        pygmyParent.Properties.ID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | (u8)i;
        if( fileSeekChunk( &pygmyParent, &ullAddress ) ){
            pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, 0x00000000 );
        } // if
    } // for
    // Finally, seek and delete length chunk if present
    pygmyParent.Properties.ID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | (u8)254;
    if( fileSeekChunk( &pygmyParent, &ullAddress ) ){
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
    *ucBuffer = malloc( 28 );
    memset( *ucBuffer, 0, 28 );
    if( !(*ucBuffer ) ){
        return( FALSE );
    } // if
    for( i = 0; i < 256; i++ ){
        pygmyFile->Properties.ID = ( ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | i );
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rCalling fileSeekChunk" );
            filePrintProperties( &pygmyFile->Properties );
        #endif
        if( !fileSeekChunk( pygmyFile, &ullTmpAddress ) ){
            break;
        } // if
        *ucBuffer = realloc( *ucBuffer, ((i+1)*28) );
        if(!(*ucBuffer) ){
            return( FALSE );
        } // if
        pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullTmpAddress + 4, *ucBuffer+(i*28), 28 );
    } // for

    return( i * 28 );
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
        //ullTmp = ucBuffer[ i ];
        //ullTmp << ( i * 8LL );
        pygmyAddress->BaseAddress <<= 8LL;
        pygmyAddress->BaseAddress |= ucBuffer[ i ];
    } // for
    // pygmyAddress->BaseAddress <<= 40;
    //print( COM3, "\r0x%012X", pygmyAddress->BaseAddress );
    //ullTmp = ( ulltmp | (u64)ucBuffer[ 1 ] ) << 8;
    //print( COM3, "\r0x%012X", pygmyAddress->BaseAddress );
    //ullTmp = ( ullTmp | (u64)ucBuffer[ 2 ] ) << 24;
    //print( COM3, "\r0x%012X", pygmyAddress->BaseAddress );
    //ullTmp |= (u64)ucBuffer[ 3 ] << 16;
    //print( COM3, "\r0x%012X", pygmyAddress->BaseAddress );
    //ullTmp |= (u64)ucBuffer[ 4 ] << 8;
   // print( COM3, "\r0x%012X", pygmyAddress->BaseAddress );
    //ullTmp |= (u64)ucBuffer[ 5 ];
    //pygmyAddress->BaseAddress = ullTmp;
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

    uiOffset = ( ( pygmyFile->Index + ( ( 1 + ulSectors ) * 2 ) ) );//- pygmyFile->Index );
    uiBufLen = ( pygmyFile->MountPoint->Desc.SectorSize ) - uiOffset;
    //print( COM3, "\ruiOffset %d\ruiBufLen %d", uiOffset, uiBufLen );
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rCalculated BufLen: %d", uiBufLen );
    #endif
    *ullAddress = pygmyFile->Properties.Sectors[ ulGroup ].BaseAddress + (( ulSectors - i ) *
        pygmyFile->MountPoint->Desc.SectorSize ) + uiOffset;
    #ifdef __PYGMY_DEBUG_FILE
      print( COM3, "\rCalculated Address: 0x%012llX", *ullAddress );
    #endif
    return( uiBufLen ); // Return number of bytes remaining in current sector
}

u8 fileAppendSectors( PYGMYFILE *pygmyFile, PYGMYFILEADDRESS *pygmyAddress ) //u32 *ulSectors, u32 ulSectorCount )
{
    // Sector Tag [ Count 1B ][ Start Address 6B ]
    // [ 32B ][ ID 4B ][ 7B ][ 7B ][ 7B ][ 7B ]
    // This function finds the first available slot for a sector tag and writes ulSector
    // If no slots are available in the currently allocated chunks, allocate a new one
    PYGMYFILE pygmyParent;
    PYGMYFILEADDRESS pygmySP;
    PYGMYFILEPROPERTIES pygmyEntry;
    u32 ulSPLen;
    u16 i, uiEntryLen;
    u8 *ucBuffer;

    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rAppending Sectors" );
    #endif
    if( !fileSeekPath( pygmyFile->Properties.Path, 0, &pygmyEntry ) ){
        return( FALSE );
    } // if
    pygmyParent.MountPoint      = pygmyFile->MountPoint;
    fileCopyProperties( &pygmyEntry, &pygmyParent.Properties );
    pygmyParent.Properties.ID   = pygmyFile->Properties.ID;
    uiEntryLen = fileGetEntry( &pygmyParent, &ucBuffer );
    sysFree( pygmyEntry.Sectors );
    if( !uiEntryLen ){
        return( FALSE );
    } // if
    // Seek for first empty sector tag pair,
    i = 6 + ucBuffer[ 5 ];
    i += ( i % 7 );
    for( ulSPLen = 0; i < uiEntryLen; ){
        fileLoadAddress( &pygmySP, ucBuffer );
        if( pygmySP.BaseAddress >= 0x0000FFFFFFFFFFFFLL ){
            // If sector pointer is not valid, then we have reached end of pointers
            break;
        } // if
        i += 7;
    } // for
    //( i % 28 )
    sysFree( ucBuffer );
    return( TRUE );
}

u8 fileWriteEntry( PYGMYFILE *pygmyParent, PYGMYFILEPROPERTIES *pygmyEntry, u8 ucAttrib, u8 *ucName )
{
    u32 ulSectorIndex;
    u16 i, ii, uiBufferSize, uiMemLen;
    u8 ucChunks, ucChunk, ucNameLen, *ucBuffer;

    pygmyEntry->ID = 0xFFFFFF00 & pdiaEncodeString( ucName );
    ucNameLen = len( ucName );

    //uiBufferSize = 32 + ( 32 * (( 10 + ucNameLen + ( pygmyEntry->SectorGroups * sizeof( PYGMYFILEADDRESS ) )) / 32 ) );
    uiBufferSize = ( 10 + ucNameLen + ( pygmyEntry->SectorGroups * sizeof( PYGMYFILEADDRESS ) ) );
    ucChunks = uiBufferSize / 32;
    if( uiBufferSize % 32 ){
        ++ucChunks;
    } // if
    //ucBuffer = sysAllocate( ucChunks * 32 );
    uiMemLen = ucChunks * 32;
    ucBuffer = malloc( uiMemLen );
    memset( ucBuffer, 0, uiMemLen );
    for( i = 0; i < uiBufferSize; i++ ){
        ucBuffer[ i ] = 0xFF;
    } // for
    if( !ucBuffer ){
        return( FALSE );
    } // if
    #ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntry:" );
        print( COM3, "\r\tID: 0x%08X", pygmyEntry->ID );
        print( COM3, "\r\tBufferSize: %d", uiBufferSize );
        print( COM3, "\r\tChunks: %d", ucChunks );
        print( COM3, "\r\tAttrib: %d", ucAttrib );
        print( COM3, "\r\tSectorGroups: %d", pygmyEntry->SectorGroups);
    #endif
    // First chunk always contains [ID][TIME][ATTRIB][STRLEN][NAME], Name may overflow into next chunk(s)
    convertU32ToBuffer( pygmyEntry->ID, ucBuffer, BIGENDIAN );
    convertU32ToBuffer( timeGet(), ucBuffer + 4, BIGENDIAN );
    ucBuffer[ 8 ] = ucAttrib;
    ucBuffer[ 9 ] = ucNameLen;
    ucChunk = 0; // Chunks start at 0, 0 always containing the name
    for( i = 10; *ucName; i++ ){
        if( !( i % 32 ) ){
            ++ucChunk;
            convertU32ToBuffer( pygmyEntry->ID|ucChunk, ucBuffer+i, BIGENDIAN );
            i += 4;
        } // if
        ucBuffer[ i ] = *(ucName++);
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "%c", ucBuffer[ i ] );
        #endif
    } // for
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
        //convertU32ToBuffer( pygmyAddress->BaseAddress[ ulSectorIndex ], ucBuffer+ii, BIGENDIAN );
        ii += 7;
    } // for
    // Finally, write the Entry as Chunks
    for( i = 0; i < ucChunks; i++ ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rEntry: " );
            for( ii = 0; ii < 32; ii++ ){
                print( COM3, "(%d)", (u32)(( u8 )*(ucBuffer + ii + ( i * 32 )) ) );
            } // for
        #endif
        fileWriteChunk( pygmyParent, ucBuffer + ( i * 32 ) );
    } // for
    //sysFree( ucBuffer );
    free( ucBuffer );

    return( ucChunk );
}

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
    PYGMYFILE *pygmyFile;
    u64 ullAddress;
    u16 i, ii;

    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rEntering fileDelete(%s)", ucName );

    pygmyVolume = fileSeekPath( ucName, 0, &pygmyEntry ); // Now we attempt to open the entire path, including name
    if( !pygmyVolume ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath Seek Failed!" );
        //#endif
        return( FALSE );
    } else{
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rPath found" );
        //#endif
    } // else
    for( i = 0; i < pygmyFile->Properties.SectorGroups; i++ ){
        for( ii = 0; ii < pygmyFile->Properties.Sectors[ i ].SectorCount; ii++ ){
            ullAddress = pygmyFile->Properties.Sectors[ i ].BaseAddress + ( ii * pygmyFile->MountPoint->Desc.SectorSize );
            pygmyFile->MountPoint->IO->EraseSector( pygmyFile->MountPoint->Port, ullAddress );
        } // for
    } // for
    // Todo: Change to a mark as deleted
    fileDeleteEntry( pygmyFile, &pygmyEntry );
    //fileClose( pygmyFile );
}

PYGMYFILE *fileOpen( u8 *ucName, u8 ucAttrib, u32 ulFileLen )
{
    PYGMYFILEVOLUME *pygmyVolume;
    PYGMYFILE *pygmyFile, pygmyParent;
    u32 ulLen, *ulSectors;
    u16 uiNameLen, uiPathLen;
    u8 i, *ucFileName, *ucFilePath, ucLen, *ucParams[32];

    /*#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rucName: (%s)", ucName );
    #endif
    ucLen = getAllSubStrings( ucName, ucParams, 32, FILESEPARATORS|NEWLINE );
    #ifdef __PYGMY_DEBUG_FILE
        for( i = 0; i < ucLen; i++ ){
            print( COM3, "\r\t%d: (%s)", i,  ucParams[ i ] );
        } // for
    #endif
    if( !ucLen ){
        print( COM3, "\rInvalid filename!" );
        return( NULL ); // if there is no valid filename, bail now
    } // if*/
    //uiNameLen = len( ucParams[ ucLen - 1 ] );
    //uiPathLen = len( ucName ) - uiNameLen;
    /*uiPathLen = replaceLastChar( ucName, '/', '/' );
    uiNameLen = len( ucName + uiPathLen + 1 );
    ucFileName = malloc( uiNameLen + 1 );
    ucFilePath = malloc( uiPathLen + 1 );
    memset( ucFileName, 0, uiNameLen + 1 );
    memset( ucFilePath, 0, uiPathLen + 1 );*/
    pygmyFile = malloc( sizeof( PYGMYFILE ) );
    /*memset( pygmyFile, 0, sizeof( PYGMYFILE ) );
    if( !ucFileName ){
        free( ucFileName );
        return( NULL );
    } // if
    if( !ucFilePath ){
        free( ucFilePath );
        return( NULL );
    } // if*/
    if( !pygmyFile ){
        free( pygmyFile );
        return( NULL );
    } // if
    //copyBuffer( ucName, ucFilePath, uiPathLen );
    //ucFilePath[ uiPathLen ] = '\0';
    //copyString( ucParams[ ucLen - 1 ], ucFileName );
    //copyString( (u8*)(ucName + uiPathLen + 1), ucFileName );
    //copyBuffer( ucName, ucFilePath, uiPathLen );
    //ucFilePath[ uiPathLen ] = '\0';

    pygmyFile->Properties.Sectors = NULL;
    pygmyFile->Properties.SectorGroups = 0;
    pygmyFile->Properties.SectorCount = 0;
    pygmyFile->Index = 0;
    //#ifdef __PYGMY_DEBUG_FILE
        print( COM3, "\rName: %s", ucFileName );
        print( COM3, "\rPath: %s", ucFilePath );
    //#endif
    if( ( ucAttrib & WRITE ) && !(ucAttrib & APPEND ) ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rWRITE" );
        //#endif
        //fileDelete( ucName );

        pygmyVolume = fileSeekPath( ucName, -1, &pygmyParent.Properties ); // Seek after as a safety measure
        if( pygmyVolume ){
            pygmyParent.MountPoint = pygmyVolume;
            pygmyFile->MountPoint = pygmyVolume;
            ulLen = 1 + ( ulLen / ( pygmyVolume->Desc.SectorSize - 2 ) );
            fileAllocateSectors( pygmyFile, ulLen ); // Loads Sectors, SectorGroups and SectorCount
            fileWriteEntry( &pygmyParent, &pygmyFile->Properties, ucAttrib, ucFileName );
        } else{
            //#ifdef __PYGMY_DEBUG_FILE
                print( COM3, "\rPath seek failed!" );
            //#endif
        } // else
    } // if
    pygmyVolume = fileSeekPath( ucName, 0, &pygmyFile->Properties ); // Now we attempt to open the entire path, including name
    if( pygmyVolume ){
        // File is valid and loaded into pygmyFile
        pygmyFile->MountPoint = pygmyVolume;
        if( ucAttrib & APPEND ){
            pygmyFile->Index = pygmyFile->Properties.Length;
        } else{
            pygmyFile->Index = 0;
        } // else
    } else{
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rName seek failed!" );
        #endif
        free( pygmyFile );
        free( ulSectors );
        free( ucFileName );
        free( ucFilePath );
        return( NULL );
    } // else
    free( ulSectors );
    free( ucFileName );
    free( ucFilePath );

    return( pygmyFile );
}

u8 fileWriteLength( PYGMYFILE *pygmyFile )
{
    // Each Len packet is marked as a special Chunk: [ ID 3B ][ 254 ]
    // Each Len chunk contains 6 4 byte bigendian tags
    // After a tag is used, it is set to 0x00000000
    // After all 6 tags are used, ID is set to 0x00000000 and a new chunk is created
    PYGMYFILE pygmyParent;
    u64 ullAddress;
    u32 ulID, ulLength;
    u8 i, ucBuffer[ 28 ];

    if( !fileSeekPath( pygmyFile->Properties.Path, 0, &pygmyParent.Properties ) ){
        // This is a serious error, file path is corrupt, was freed, or is simply incorrect
        return( FALSE );
    } // if
    ulID = ( pygmyFile->Properties.ID & 0xFFFFFF00 ) | 254;
    pygmyParent.MountPoint = pygmyFile->MountPoint;
    pygmyParent.Properties.ID = ulID;
    if( !fileSeekChunk( &pygmyParent, &ullAddress )){ //ulID, pygmyEntry.Sectors, pygmyEntry.SectorCount );
    //if( ulAddress == 0xFFFFFFFF ){
        return( FALSE );
    } // if
    // Now skip ID and load buffer
    pygmyFile->MountPoint->IO->GetBuffer( pygmyFile->MountPoint->Port, ullAddress + 4, ucBuffer, 28 );
    for( i = 0; i < 6; i++ ){
        // Iterate through Length Tags
        ulLength = convertBufferToU32( ucBuffer + ( i * 4 ), BIGENDIAN );
        if( ulLength == 0xFFFFFF ){
            ullAddress += ( ( i + 1 ) * 4 );
            pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, pygmyFile->Properties.Length );
            return( TRUE );
        } // if
    } // for
    // If we fell through, we need to delete and write a new chunk
    // First Delete current chunk
    pygmyFile->MountPoint->IO->PutLong( pygmyFile->MountPoint->Port, ullAddress, 0x00000000 );
    // Next write ID and file Length to buffer
    convertU32ToBuffer( ulID, ucBuffer, BIGENDIAN );
    convertU32ToBuffer( pygmyFile->Properties.ID, ucBuffer + 4, BIGENDIAN );
    // Finally, write buffer as new chunk
    fileCopyProperties( &pygmyFile->Properties, &pygmyParent.Properties );
    fileWriteChunk( &pygmyParent, ucBuffer );

    return( TRUE );
}

u32 fileReadLength( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyEntry )//u32 ulID, u32 *ulSectors, u32 ulSectorCount )
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
}

u8 fileClose( PYGMYFILE *pygmyFile )
{
    fileWriteLength( pygmyFile );
    //sysFree( pygmyFile->Properties.Sectors );
    free( pygmyFile->Properties.Sectors );
    //sysFree( pygmyFile->Properties.Path );
    free( pygmyFile->Properties.Path );
    //sysFree( pygmyFile->Properties.Name );
    free( pygmyFile->Properties.Name );

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

}

u8 fileReadBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen )
{

    return( TRUE );
}

u8 fileWriteBytes( PYGMYFILE *pygmyFile, u8 *ucBuffer, u16 uiLen )
{
    // This function is only for internal use!!!
    // Use filePut functions in user code, fileWriteBytes is optimized for multi-byte write oeprations
    u64 ullAddress;
    u32 i, ulSectorSize;
    u16 uiBufLen;

    //if( ( pygmyFile->Properties.Length + 1 ) >= ( ulSectorSize * pygmyFile->Properties.SectorCount ) ){
    if( ( pygmyFile->Properties.Length + uiLen ) >= ( ( ulSectorSize - 2 ) * pygmyFile->Properties.SectorCount ) ){
        #ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rAllocating another sector" );
        #endif
    } // if
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

u8 fileGetBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{

}

u8 filePutBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{
    return( fileWriteBytes( pygmyFile, ucBuffer, uiLen ) );
}

