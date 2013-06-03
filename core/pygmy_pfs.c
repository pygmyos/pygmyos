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

u16 fileGenerateDirectAddress( PYGMYFILEVOLUME *pygmyVolume, PYGMYFILEPROPERTIES *pygmyProperties, u32 ulIndex, u64 *ullAddress )
{
    // This function decodes base-address and count pairs and calculates a physical
    //  address off this information and the index
    // The number of bytes left in the current sector is returned, ullAddress is loaded with address
    u32 i, ulGroup, ulSectors;
    u16 uiOffset, uiBufLen;

    ulSectors = ulIndex / ( pygmyVolume->Desc.SectorSize - 2 );
    if( ulSectors > pygmyProperties->SectorCount  ) { 
        *ullAddress = INVALIDADDRESS;
        return( FALSE );
    } // if
    for( i = 0, ulGroup = 0; i < ulSectors && ulGroup < pygmyProperties->SectorGroups; ulGroup++ ){
        if( i + pygmyProperties->Sectors[ ulGroup ].SectorCount > ulSectors ){
            break;
        } // if
        i += pygmyProperties->Sectors[ ulGroup ].SectorCount;
    } // for
   
    uiOffset = ( ( ulIndex + ( ( 1 + ulSectors ) * 2 ) ) );
    uiBufLen = ( pygmyVolume->Desc.SectorSize ) - uiOffset;
    
    *ullAddress = pygmyProperties->Sectors[ ulGroup ].BaseAddress + (( ulSectors - i ) *
        pygmyVolume->Desc.SectorSize ) + uiOffset;
    
    return( uiBufLen ); // Return number of bytes remaining in current sector
}

u8 fileAllocateChunks( PYGMYFILE *ParentFolder, u8 Chunks, u64 *Addresses )
{
    // fileAllocateChunks() will allocate chunks by finding a run ( if possible ) and writing the ID
    // Since this functions doesn't know what chunk range the chunks are being requested for, the chunk field will be left blank
    // It is important to note that if there is insufficient space in the current ParentSG, an additional sector will be allocated and 
    //    inserted into pygmyParentSG. If this sector is not contiguous with a SectorGroup contained within the ParentSG, an additional
    //    SectorGroup will be allocated and appended to the array. This means that the pointer returned should always replace the pointer
    //    passed. If the pointer is NULL, the operation failed.
    // ullAddressess will be allocated by fileAllcoateChunks and will contain pointers to every chunk allocated.
    // !!! This function does not modify pygmyFile !!!
    u64 TmpAddress;
    u32 i;

    // First, check for available contiguous chunks
    if( fileFindContiguousChunks( ParentFolder, Chunks, &TmpAddress ) ){
        // We have chunks, if ullTmpAddress is set to PYGMYFILE_INVALIDADDRESS we must recycle ParentSG
        if( TmpAddress == PYGMYFILE_INVALIDADDRESS ){
            fileRecyleParentChunks( ParentFolder );
            return( fileAllocateChunks( ParentFolder, Chunks, &(*Addresses) ) ); // recursive call
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
	// ToDo: Add step to allocate based on non-contiguous chunks before allocating another sector
    // If we dropped through to this point, we have insufficient space in the currently allocated parent sector groups
    if( fileAppendSectors( &pygmyFile, 1, PYGMYFILE_PARENT ) ){ // append to parent
            return( fileAllocateChunks( pygmyFile, pygmyParentSG, ucChunks, &(*ullAddresses) ) );
    } // if

    return( FALSE );
}

u8 fileFindContiguousChunks( PYGMYFOLDER *ParentFolder, u8 Chunks, u64 *StartAddress )
{
    // Count the number of erased and unused chunks
    // Return the number of contiguous unused chunks found  ans set ullStartAddress to start address OR
    // Return the number of erased chunks found and set ullStartAddress to PYGMYFILE_INVALIDADDRESS
    u64 Address;
    u32 i, EndIndex, EntryID;
    u8 Erased, Unused;

    Erased = 0;
    Unused = 0;
    EndIndex = ( ParentFolder->Properties->SectorCount * ParentFolder->MountPoint->Desc.SectorSize ) / PYGMYFILE_CHUNKLEN;
    for( i = 0; i < EndIndex; i++ ){
		// fileGenerateAddress() takes a mount point and properties ( for sector group data ) and an index and returns fills ullAddress
        fileGenerateAddress( &ParentFolder->MountPoint, &ParentFolder->Properties, i * PYGMYFILE_CHUNKLEN, &Address );
        EntryID = ParentFolder->IO->GetLong( &ParentFolder->MountPoint->Port, Address );
        if( EntryID == 0 ){
            // We have found an erased chunk
            if( Erased < 255 ){
                ++Erased;
            } // if
            Unused = 0; // An erased sector requires recycling before use, and is counted as "used"
        } else if( EntryID == 0xFFFFFFFF ){
            // We have found an unused chunk
            if( Unused == 0 ){
                // Recapture the start address each time the counter resets to zero
                StartAddress = Address;
            } //
            ++Unused;
            if( Unused == Chunks ){
                // We are done with our search, ucChunk unused chunks have been found, return
                return( Unused );
            } // if
        } // else if
    } // for

    *StartAddress = PYGMYFILE_INVALIDADDRESS; // We don't have the requested run of contiguous chunks

    if( Erased >= Chunks ){
        return( Erased );
    } // if 

    return( 0 ); // 0 chunks found
}

u8 fileWriteEntry( PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // !!! WARNING !!! fileWriteEntry() writes the properties contained in Properties
	// !!! WARNING !!! Do not call fileWriteProeprties() directly
	// This function writes the Name, Sector Groups and Length
    u64 *Addresses;
	u8 NameLen, Chunks, SGChunks, NameChunks;
	
	NameLen = len( Properties->Name ); 
	NameChunks = 1 + ( ( NameLen + 6 ) / PYGMYFILE_CHUNKPAYLEN );
	SGChunks = Properties->SectorGroups / 3; // There are 3 9byte Sector Groups Nodes stored per chunk
	Chunks = 1 + NameChunks + SGChunks; // This stands for LengthChunk + NameChunks + SGChunks
	Addresses = malloc( sizeof( u64 ) * Chunks );
    if( !Addresses ){
        print( COM3, "\rNot enough memory for %d chunk pointers", Chunks );
        return( FALSE );
    } // if
	// We allocate all the chunks we will need before we begin to write
	// We don't want to find out that we have insufficient memory after beginning write operations
	if( fileAllocateChunks( ParentFolder, Chunks, Addresses ) ){
		if( fileWriteProperties( ParentFolder, Addresses, Properties ) ){
			if( fileWriteSectorGroups( ParentFolder, Addresses+NameChunks, Properties ) ){
				// Sector Group chunks start after the last Name Chunk
				if( fileWriteLength( ParentFolder, *(Addresses+NameChunks+SGChunks), SGChunks, Properties->ID ) ){
					// The Length Chunk is last, following the Name and Sector Group Chunks
					free( Addresses );
					return( TRUE );
				} // if
			} // if
		} // if
	} // if
	
	free( Addresses );
	return( FALSE );
}

u8 fileWriteProperties( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties )
{
    // When writing Attributes, will not write WRITE if MountPoint does not have WRITE
    // Writes Time stamp, filename length and Name ( may span chunks 0-9 )
	// ID must have been generated by the calling function
	u16 i, ii, NameLen;
	u8 Chunk, NameChunks;
	
	NameLen = len( Properties->Name );
	if( NameLen > 255 || NameLen == 0 ){
		print( COM2, "\rfileWriteProeprties() -> Bad file name" );
	} // if
	Chunk = PYGMYFILE_NAMECHUNK;
	NameChunks = 1 + ( ( NameLen + 6 ) / PYGMYFILE_CHUNKPAYLEN );
	// Write Chunk 0, starting with ID
	//ParentFolder->MountPoint->IO->PutLong( &ParentFolder->MountPoint->Port, Addresses[ 0 ], Properties->ID ); // ID written at allocation
	// Write Chunk Count, always 0 for first Properties ( Name ) Chunk
	ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 4, Chunk++ );
	// Write 32bit Time ( stores file creation time )
	ParentFolder->MountPoint->IO->PutLong( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 5, timeGet() );
	// Write Attributes, Mask properties not supported by the volume
	ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 9, 
		( ParentFolder->MountPoint->Properties.Attributes & ( READ|WRITE) ) & Properties->Attributes ); 
	// Write length of file name
	ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 10, NameLen );
	if( NameLen < 22 ){
		// Entire file name will fit in Chunk 0
		ParentFolder->MountPoint->IO->PutBuffer( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 11, Properties->Name, NameLen );
	} else{
		ParentFolder->MountPoint->IO->PutBuffer( &ParentFolder->MountPoint->Port, Addresses[ 0 ] + 11, Properties->Name, 21 );
	
		for( i = 21, ii = 1; i < NameLen; ii++ ){
			// Write ID and Chunk number before each file name payload ( up to 27 bytes )
			//ParentFolder->MountPoint->IO->PutLong( &ParentFolder->MountPoint->Port, Addresses[ ii ], Properties->ID ); // ID written at allocation
			ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Addresses[ ii ] + 4, Chunk++ );
			if( ( NameLen - i ) > 27 ){
				ParentFolder->MountPoint->IO->PutBuffer( &ParentFolder->MountPoint->Port, Addresses[ ii ] + 5, Properties->Name + i, 27 );
				i += 27;
			} else{
				// This is the last chunk
				ParentFolder->MountPoint->IO->PutBuffer( &ParentFolder->MountPoint->Port, Addresses[ ii ] + 5, Properties->Name + i, NameLen - i );
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
	
	// Write the Chunk number after the ID ( already written at allocation )
	ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Address + 4, PYGMYFILE_LENGTHCHUNK );
	// Write the number of Sector Group Chunks ( this reduces load time later )
    ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Address + 5, SGChunks );
	// Write the 32bit file length into LEN0
	ParentFolder->MountPoint->IO->PutLong( &ParentFolder->MountPoint->Port, Address + 8, Length );
	
	return( TRUE );
}

u8 fileWriteSectorGroups( PYGMYFOLDER *ParentFolder, u64 *Addresses, PYGMYFILEPROPERTIES *Properties )
{
	u8 Buffer[ 9 ]; // Storage for processed Sector Group Address
	
	if( Properties->SectorGroups == 0 ){
		return( FALSE );
	} // if
	fileconvertAddressToBuffer( Properties->Sectors[ i ], Buffer );
	for( i = 0, ii = 0; ii < Properties->SectorGroups; ){
		ParentFolder->MountPoint->IO->PutChar( &ParentFolder->MountPoint->Port, Addresses[ ii ] + 4, PYGMYFILE_SGCHUNK + ii );
		ParentFolder->MountPoint->IO->PutBuffer( &ParentFolder->MountPoint->Port, Addresses[ ii ] + 5 + ( i * 9 ), 9, Buffer );
		if( i == 2 ){
			i = 0;
			++ii;
		} else{
			++i;
		} // else
	} // for
}

void fileConvertBufferToAddress( PYGMYFILEADDRESS *Address, u8 *Buffer )
{
	u8 i;
	
	Address->SectorCount =  (u16)( *(Buffer++) ) << 8;
	Address->SectorCount |= (u16)( *(Buffer++) );
	Address->BaseAddress = 0;
	for( i = 0; i < 6; i++ ){
		Address->BaseAddress |= Buffer[ i ];
		Address->BaseAddress <<= 8;
	} // for
	Address->BaseAddress = Buffer[ 6 ];
}

void fileConvertAddressToBuffer( PYGMYFILEADDRESS *Address, u8 *Buffer )
{
	u8 i;
	
    *(Buffer++) = (u8)((u16)Address->SectorCount >> 8);
	*(Buffer++) = (u8)((u16)Address->SectorCount );
	Address->BaseAddress = 0;
	for( i = 0; i < 7; i++ ){
		Buffer[ 6 - i ] = (u8)((u64)Address->BaseAddress);
		Address->BaseAddress >> 8LL;
	} // for
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
    u8 SGChunks;
    
    if( fileLoadProperties( ID, ParentFolder, Properties ){
        SGChunks = fileLoadLength( ID, ParentFolder, Properties );
        if( SGChunks != 0 ){
            if( fileLoadSectorGroups( ID, SGChunks, ParentFolder, Proeprties );
                print( COM3, "\rEntry Loaded" );
                return( TRUE );
            } // if
        } // if
    } // if
    
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
    if( fileFindChunk( ID, PYGMYFILE_LENGTHCHUNK, ParentFolder, Buffer ) ){
        // We have found the length chunk, now load the Sector Group Count and the Length
        
        for( i = 0; i < 3; i++ ){
            if( Buffer[ i ] != 0xFF && Buffer[ i ] != 0x00 ){
                SG = Buffer[ 0 ]; // Found a 
                break;
            } // if
        } // for
        Buffer += 3; // Increment to the first Length tag
       // There are six tags stored in each chunk, when a file has been written to and then closed, 
       //   the current tag is changed to 0 and the next erased ( 0xFFFFFFFF ) tag is updated with the current file length
       //   When all 6 tags have been used, the ID is changed to 0 and a new Length chunk is allocated
        for( i = 0; i < 6; i++ ){ 
            Length = convertBufferToU32( Buffer + ( 4 * i ), BIGENDIAN );
            if( Length != 0xFFFFFFFF && Length != 0 ){
                // We found the current Length Tag, time to return
                break;
            } // if
        } // for
    } // if

    // If there is no length chunk, that means the file hasn't had any write operations performed yet
    // This is not an error condition
    Properties->Length = Length;

    return( SG );
}

u8 fileFindChunk( u32 ID, u8 Chunk, PYGMYFOLDER *ParentFolder, u8 *Buffer )
{
    // fileFindChunk() does not allocate ucBuffer, 27 bytes must be allocated by the calling functions, static allocation is recommended
    // pygmySG is a pointer to the parent sector group array, this array must be filled out by the calling function
    u64 Address;
	u32 i, ii, iii, EntryCount, TmpID;
    u8 TmpChunk;

    EntryCount = ParentFolder->MountPoint->Desc.SectorSize / PYGMYFILE_CHUNKLEN; // 32 is length of chunks
    
    for( i = 0; i < ParentFolder->SectorGroups; i++ ){ // This is the number of parent sectors containing chunks to sort
        for( ii = 0; ii < ParentFolder->Sectors[ i ].SectorCount; ii++ ){ // Iterate through Chunks and process matches
            for( iii = 1; iii < EntryCount; iii++ ){
                *Address = ParentFolder->Sectors[ i ].BaseAddress +
                    ( ii * ParentFolder->Desc.SectorSize ) + ( iii * PYGMFILE_CHUNKLEN );
                // Load the first 5 bytes of the chunk, this contains the ID and the chunk number
                ParentFolder->MountPoint->IO->GetBuffer( &ParentFolder->MountPoint->Port, *Address, Buffer, 5 );
                TmpID = convertBufferToU32( Buffer, BIGENDIAN );
                TmpChunk = Buffer[ 4 ];
                
                if( TmpID == ID && TmpChunk == Chunk ){
                    // We found the chunk requested, load the 27 byte payload and return
                    ParentFolder->MountPoint->IO->GetBuffer( &ParentFolder->MountPoint->Port, *Address + 5, Buffer, 27 );
                    return( TRUE );
                } // if
            } // for
        } // for
    } // for
    
    return( FALSE ); // The requested chunk doesn't exist in the current folder
}

u8 fileLoadProperties( u32 ulID, PYGMYFOLDER *ParentFolder, PYGMYFILEPROPERTIES *Properties )
{
    // The Properties ( Name ) chunk is formatted as follows:
    // [ ID 4B ][ CHUNK 1B ][ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // Since fileFindChunk() fills ucBuffer with the 27 bytes following CHUNK, we will treat the format as follows:
    // [ TIME 4B ][ ATTRIB 1B ][ NAMELEN 1B ][ NAME nB ]
    // If the file name length exceeds the 21 bytes available in the first Chunk, additional chunks will be loaded to
    // retrieve the remainder of the file name. All additional Properties ( Name ) chunks contain up to 27 bytes of name each,
    // spanning up to 10 chunks ( 0 - 9 )
    u16 i, ii;
    u8 Buffer[ 27 ], NameLen, NameIndex, Chunk;

    if( !fileFindChunk( ID, 0, ParentFolder, Buffer ) ){
        // No Name Chunk means the file doesn't exist
        return( FALSE );
    } // if
    Properties->Time = convertBufferToU32( Buffer, BIGENDIAN );
    Properties->Attributes = Buffer[ 4 ];
    NameLen = Buffer[ 5 ];
    Properties->Name = malloc( NameLen );
    if( !Properties->Name ){
        print( COM3, "\rMemory full!" );
        return( FALSE );
    } // if
    // Now load the filename
    for( i = 0, ii = 6, NameIndex = 0; i < NameLen; i++, ii++ ){
        if( !( ii % 27 ) ){
            // load the next Properties ( Name ) chunk id we are out of data and haven't reached the end of the name field
            if( !fileFindChunk( ulID, ii / 27, ParentFolder, Buffer ) ){
                // This indicates file system corruption and is a serious failure
                print( COM3, "\rRequired Properties ( Name ) chunk missing." );
                return( FALSE );
            } // if
            NameIndex = 0; // Reset ucNameIndex to lock within range 0-26
        } // if
        if( Buffer[ NameIndex ] == 0xFF ){
            print( COM3, "\rEnd of Properties ( Name ) found at %d bytes", ii );
            ucBuffer[ NameIndex ] = '\0'; // Terminate string
            break;
        } // if
        Properties->Name[ i ] = Buffer[ NameIndex++ ];
    } // for

    return( TRUE );
}

u8 fileLoadSectorGroups( u32 ID, u8 SGChunks, PYGMYFOLDER *ParentFolder PYGMYFILEPROPERTIES *Properties )
{
    // Sector Groups are stored 3 per chunk at 9 bytes each
    // A Sector Groups is formatted as follows: [ SECTORCOUNT 2B ][ BASEADDRESS 7B ]
    PYGMYSECTORGROUPS TmpSG;
    u64 Address;
    u16 i, ii, iii;
    u16 SectorCount;
    u8 Buffer[ 27 ];

    TmpSG.Sectors = malloc( sizeof( PYGMYFILEADDRESS ) );
    if( !TmpSG.Sectors ){
        return( FALSE );
    } // if
    TmpSG.SectorGroups = 0;
    TmpSG.SectorCount = 0;
    for( i = 0; i < SGChunks; i++ ){
        if( fileFindChunk( ID, PYGMYFILE_SGCHUNK + i, ParentFolder, Buffer ) ){
            for( ii = 0; ii < 3; ii++ ){
                // Load the 16bit Sector Count
                SectorCount = convertBufferToU16( Buffer, BIGENDIAN );
                Buffer += 2; // Increment to the Base Address and load the 56bit address
                for( iii = 0, Address = 0; iii < 7; iii++ ){
                    Address |= ( (u64) (++Buffer) << ( ( 7 - iii ) * 8 ) ); 
                } // iii
                // Now check the address for 0 length group or unused memory
                if( Address == 0x00FFFFFFFFFFFFFFLL {
                    // If data is unused, we are past the last sector group
                    break;
                } else if( SectorCount ){
                    // If uiSectorCount is 0, then the sector group entry was erased, ignore and continue
                    // otherwise, allocate memory for a new sector group and append to the list
                    TmpSG.SectorCount += SectorCount;
                    ++TmpSG.SectorGroups;
                    TmpSG.Sectors = realloc( TmpSG.Sectors, ( 1 + TmpSG.Sectors ) * pygmysizeof( PYGMYFILEADDRESS ) );
                    if( !TmpSG.Sectors ){
                        return( FALSE );
                    } // if
                    TmpSG.Sectors[ TmpSG.SectorGroups - 1 ].SectorCount = SectorCount;
                    TmpSG.Sectors[ TmpSG.SectorGroups - 1 ].BaseAddress = Address;
                    print( COM3, "\rAdded Sector Group, Count: %d, Base Address: %012llX", SectorCount, Address );
                } // if
            } // for
        } else{
            // Sector Group chunks are returned in order.
            print( COM3, "\rProcessed all address chunks." );
            break;
        } // else
    } // for
	Properties->Sectors = TmpSG.Sectors;
	Properties->SectorGroups = TmpSG.SectorGroups;
	Properties->SectorCount = TmpSG.SectorCount;
	
	return( TRUE );
}

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

/*
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
}*/

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

PYGMYFILE *fileOpen( u8 *Name, u8 Attrib, u32 FileLen )
{
    PYGMYFILEPROPERTIES ParentProperties, *Properties;
    PYGMYFILEVOLUME *Volume;
	PYGMYFOLDER ParentFolder;
    PYGMYFILE *pygmyFile, pygmyParent;
    u32 Len;
    u16 NameLen, PathLen;
    u8 i, *FileName, *FilePath, ucLen, *ucParams[32];
    
    pygmyFile = malloc( sizeof( PYGMYFILE ) );
    
    if( !pygmyFile ){
        free( pygmyFile );
        return( NULL );
    } // if
    
    FileName = fileGetNameFromPath( Name );
    if( !FileName ){
        return( NULL );
    } // if
    FilePath = fileGetPathFromPath( Name );
    if( !FilePath ){
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
    if( ( Attrib & WRITE ) && !( Attrib & APPEND ) ){
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rOpen for WRITE" );
        //#endif
        fileDelete( Name );
        // Seek after as a safety measure
        if( fileSeekPath( FilePath, &Volume, &ParentProperties ) ){
            print( COM3, "\rfileOpen()->Found Path" );
            ParentFolder.MountPoint = Volume;
			ParentFolder.Sectors = ParentProperties.Sectors;
			ParentFolder.SectorCount = ParentProperties.SectorCount;
			ParentFolder.SectorGroups = ParentProperties.SectorGroups;
            pygmyFile->MountPoint = Volume;
            Len = 1 + ( Len / ( Volume->Desc.SectorSize - 2 ) );
            print( COM3, "\rAllocating Sectors" );
            fileAllocateSectors( pygmyFile, Len ); // Loads Sectors, SectorGroups and SectorCount
            print( COM3, "..." );
            //pygmyProperties = &pygmyFile->Properties;
            pygmyFile->Properties.Attributes = ucAttrib;
            print( COM3, "\rfileOpen()->Writing Entry" );
			pygmyFile->Properties.Length = FileLen;
            fileWriteEntry( &ParentFolder, &pygmyFile->Properties );
            print( COM3, "..." );
            
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
    
    // Now we attempt to open the entire path, including name
    if( fileSeekPath( Name, &Volume, &pygmyFile->Properties ) ){
        // File is valid and loaded into pygmyFile
        //print( COM3, "\rfileOpen() -> File Length: %d", pygmyFile->Properties.Length );
        pygmyFile->MountPoint = Volume;
        if( ucAttrib & APPEND ){
            pygmyFile->Index = pygmyFile->Properties.Length;
        } else{
            pygmyFile->Index = 0;
        } // else
    
    } else{
        //#ifdef __PYGMY_DEBUG_FILE
            print( COM3, "\rName seek failed!" );
        //#endif
        free( pygmyFile );
        //free( ulSectors );
        free( FileName );
        free( FilePath );

        return( NULL );
    } // else

    pygmyFile->Properties.Attributes = Attrib;

    return( pygmyFile );
}

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

    convertU16ToBuffer( uiData, ucBuffer, ucEndian );

    return( fileWriteBytes( pygmyFile, ucBuffer, 2 ) );
}

u32 fileGetLong( PYGMYFILE *pygmyFile, u8 ucEndian )
{

}

u8 filePutLong( PYGMYFILE *pygmyFile, u32 ulData, u8 ucEndian )
{
    u8 ucBuffer[ 4 ];

    convertU32ToBuffer( ulData, ucBuffer, ucEndian );

    return( fileWriteBytes( pygmyFile, ucBuffer, 4 ) );
}

/*u8 fileGetBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{

}*/

u8 filePutBuffer( PYGMYFILE *pygmyFile, u8 *ucBuffer, u32 uiLen )
{
    return( fileWriteBytes( pygmyFile, ucBuffer, uiLen ) );
}

