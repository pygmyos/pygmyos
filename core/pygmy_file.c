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

#include "pygmy_profile.h"

/******************************************************************************************* 
    This file includes limited software SPI port functionallity
    The SPI_SCK, SPI_MOSI, and SPI_MISO must be defined in "platform specific" header
    Filesytem is similar to FAT16, but isn't compatible directly due to nature of
    FLASH memory and limited write cycles    
    Filename must be NULL ( '\0' ) terminated and may be up to 12 bytes long + NULL
    Filename may include extension, but extension isn't required
    Readonly files will normally have a static length and that length will be stored in LEN
    Read/Write files will normally have a LEN of 0xFFFFFFFF and store LEN in first 256 bytes
        of first sector. When file length changes the current LENx will be changed to 
        0x00000000 and the next LENx will be written with the new length. When all 64 LENs
        have been used, the sector will be marked as 0x0000 in FAT and a new sector will be
        allocated with the original sector being copied into it will new length stored in 
        LEN0 and all remaining LENx slots left blank( 0xFFFFFFFF ). 
        If no FAT entries are remaining then FILE_ERROR_MEMFULL must be returned and no 
        change made to the file.
    The first byte of a file entry describes it's possible use: 0xFF means available, 0x00
        means deleted, renamed, or otherwise unavailable. Any other value is the first 
        byte of a valid filename.
    The filesystem must ensure that two files with the same name are not created
    When a file is deleted or renamed, the first byte of the filename will be set to NULL
        ( '\0' or 0x00 )
    When renaming, a filename check must be run to avoid duplicates as when creating a new file
    When formatting the name must be a minimun of one alphanumeric char, NULL is how the name
        will be marked if the alternate FILES table is used
    The first file entry is the storage descriptor which is formatted as follows:
        [ NAME 13B ][ ATTRIB 1B ][ SECTORS 2B ][ SECTORSIZE 2B ][ MAXFILES 4B ][ MEDIALEN 4B ]
    The second file entry is the table pointers, which is formatted as follows
        [ FATPERSECTOR 2B ][ FILESSECTORS 2B ][ FATSECTORS 2B ][ FIRSTFILESECTOR 4B ][ FILES_A 4B ][ FILES_B 4B ][ FAT_A 4B ][ FAT_B 4B ] 
    When creating or adding a sector the limit set in SECTORS must not be exceeded
    At the volume level, ATTRIB contains read/write and also the media type:
        #define MEDIARAM                BIT2 // Volume Level, covers all memory that allows random read/write access
        #define MEDIAFLASH              BIT3 // Volume Level, covers flash memory ( write cycle limitations )
    FILE = [ FILENAME 13B ][ ATTRIB 1B ][ ID 2B ]
        if LEN = 0xFFFFFFFF then LEN is stored in first 256 bytes of first sector of file
        [ LEN0 ][ LEN1 ]...[ LEN63 ]
    Sectors used for file entries ( 1MB example ) are: 
    [ FILES_A1         - SECTOR0 ]   0x000000   
    [ FILES_B1         - SECTOR2 ]   0x001000
    [ FAT_A            - SECTOR4 ]   0x002000
    [ FAT_B            - SECTOR5 ]   0x003000
    [ Storage          - SECTOR6 ]   0x004000

    FAT Table
    [ ALLOCATION 1B ][ SEQUENCE 1B ][ FILE 2B ]
    ...
    [ ALLOCATION 1B ][ SEQUENCE 1B ][ FILE 2B ]

    ALLOCATION 0x00000000 = Bad Sector or No Sector
    ALLOCATION 0xFFFFFFFF = Sector Available
    SEQUENCE N = Sector in File, i.e. first sector, second sector, etc.  
    FILE N = File ID
**********************************************************************************************/

// ToDo: Add directory support
// ToDo: Add support for bad sector rejection

PYGMYFILEVOLUME pygmyRootVolume;
PYGMYSPIPORT pygmyFlashSPI;

u8 fileOpen( PYGMYFILE *pygmyFile, u8 *ucName, u8 ucAttrib )
{
    u32 ulFileEntry;
    u32 ulEntryAddress, ulNameMatch;

    if( !(ucAttrib & (WRITE|READ) ) ){
        return( 0 );
    } // if
    
    if( ucAttrib & APPEND ){
        // Append must always have write 
        ucAttrib |= WRITE; 
    } // if
    pygmyFile->Attributes = ucAttrib;
    //if( ucAttrib ){
    //    pygmyFile->Attributes = ucAttrib;
    //} else{ 
    //    pygmyFile->Attributes = READ;
    //} // else
    
    fileMountRoot( );
    
    if( !fileIsValidName( ucName ) ){
       return( 0 );
    } // if
    
    ulNameMatch = fileSeekName( ucName );
    if( !ulNameMatch && ( pygmyFile->Attributes & READ ) ){
        return( 0 );
    } // if
    copyString( ucName, pygmyFile->Name );
    pygmyFile->Resource = NULL;
    if( ( pygmyFile->Attributes & WRITE ) && 
        !( ulNameMatch && ( pygmyFile->Attributes & APPEND ) ) ){
        if ( ulNameMatch ){ // File exists and we are in write mode, delete
            fileDelete( pygmyFile->Name ); // If file exists and not in append mode, file must be deleted
            ulNameMatch = 0;
        } // if    
        ulFileEntry = fileGetFreeFileEntry( );
        ulNameMatch = ulFileEntry;
        if( !ulFileEntry ){
            return( 0 ); // No Sectors left to allocate     
        } // if
        //[ FILENAME 13B ][ ATTRIB 1B ][ ID_File 2B ] ( 16B total )
        ulFileEntry = pygmyRootVolume.ActiveFiles + ( ulFileEntry * 16); // Each file Entry exactly 16 bytes
        
        flashWriteBuffer( ulFileEntry, len( pygmyFile->Name )+1, pygmyFile->Name );
        flashWriteByte( ulFileEntry+13, (WRITE|READ) );
        pygmyFile->ID_File = ulNameMatch;
        pygmyFile->ID_Sector = 1;
        flashWriteWord( ulFileEntry+14, pygmyFile->ID_File );
        fileAllocateFAT( pygmyFile->ID_File, pygmyFile->ID_Sector );    
    } // if 
    
    ulEntryAddress = pygmyRootVolume.ActiveFiles + ( ulNameMatch * 16); // Each file Entry exactly 16 bytes
    
    pygmyFile->ID_File = flashReadWord( ulEntryAddress + 14 ); // last two bytes store ID
    pygmyFile->Length = fileGetLength( pygmyFile->ID_File );  
    pygmyFile->Sector = (u16)((u32)pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize);
    pygmyFile->ID_Sector = 1;
    pygmyFile->Index = 0;
    if( pygmyFile->Attributes & APPEND ){
        pygmyFile->Index = pygmyFile->Length;
        pygmyFile->ID_Sector += ( ( pygmyFile->Length + 128 ) / pygmyRootVolume.SectorSize );
    } // if
    pygmyFile->Sector += fileGetFAT( pygmyFile->ID_File, pygmyFile->ID_Sector );
    
    return( 1 );
}

u8 fileOpenResource( PYGMYFILE *pygmyFile, u8 *ucResource )
{
    //u32 ulLen; 

    /*ulLen = (u32)((u8)*(ucResource)) << 24;
    ulLen |= (u32)((u8)*(ucResource+1)) << 16;
    ulLen |= (u32)((u8)*(ucResource+2)) << 8;
    ulLen |= (u32)((u8)*(ucResource+3));
    */
    pygmyFile->Length = (u32)((u8)*(ucResource)) << 24;
    pygmyFile->Length |= (u32)((u8)*(ucResource+1)) << 16;
    pygmyFile->Length |= (u32)((u8)*(ucResource+2)) << 8;
    pygmyFile->Length |= (u32)((u8)*(ucResource+3));
    pygmyFile->Resource = (u8*)(ucResource+4);
    pygmyFile->Name[ 0 ] = '\0';
    //pygmyFile->Length = ulLen;
    pygmyFile->Index = 0;
    pygmyFile->Attributes = READ;

    return( 1 );
}

u8 fileDelete( u8 *ucName )
{
    u16 uiFileEntry;

    fileMountRoot( );
    uiFileEntry = fileSeekName( ucName );
    if( !uiFileEntry || !( pygmyRootVolume.Attributes & WRITE ) ){
        return( 0 );
    } // if
    flashWriteByte( pygmyRootVolume.ActiveFiles + ( uiFileEntry * 16 ), 0 ); // First byte 0 marks as deleted
    // Next find all FAT entries with the ID uiFleEntry and Delete
    fileDeleteFATEntry( uiFileEntry, 0 ); // 0 erases all FAT Entries with ID uiFileEntry
    
    return( 1 );
}

void fileCopyHandle( PYGMYFILE *pygmyFrom, PYGMYFILE *pygmyTo )
{
    pygmyTo->Sector     = pygmyFrom->Sector;
    pygmyTo->Length     = pygmyFrom->Length;
    pygmyTo->Index      = pygmyFrom->Index;
    pygmyTo->ID_Sector  = pygmyFrom->ID_Sector;
    pygmyTo->ID_File    = pygmyFrom->ID_File;
    pygmyTo->Attributes = pygmyFrom->Attributes;
    pygmyTo->Resource   = pygmyFrom->Resource;
    copyString( pygmyFrom->Name, pygmyTo->Name );
}

u32 fileGetPosition( PYGMYFILE *pygmyFile )
{
    return( pygmyFile->Index );
}

u8 fileSetPosition( PYGMYFILE *pygmyFile, u8 ucOrigin, s32 lIndex )
{
    if( ucOrigin == START ){ 
        if( lIndex >= 0 && lIndex < pygmyFile->Length ){
            pygmyFile->Index = lIndex;
            return( 1 );
        } // if
    } else if( ucOrigin == CURRENT ){
        if( ( pygmyFile->Index + lIndex ) >= 0 && ( pygmyFile->Index + lIndex ) < pygmyFile->Length ){
            pygmyFile->Index += lIndex;
            return( 1 );
        } // if
    } else if( ucOrigin == END ){
        if( ( pygmyFile->Length + lIndex ) >= 0 && ( pygmyFile->Length + lIndex ) < pygmyFile->Length ){
                pygmyFile->Length += lIndex;
            return( 1 );
        } // if
    } // else
    
    return( 0 );
}

u8 fileIsEOF( PYGMYFILE *pygmyFile )
{
    return( pygmyFile->Attributes & EOF );
}

u8 fileIsRootFull( void )
{
    return( pygmyRootVolume.Attributes & VOLUMEFULL );
}

u8 *fileGetVolumeName( u8 *ucName )
{
    u8 i, ucByte;

    for( i = 0, ucName[ 0 ] = 0; i < 13; i++ ){
        ucByte = flashReadByte( pygmyRootVolume.ActiveFiles + i );
        ucName[ i ] = ucByte;
        if( !ucByte ){
            break;
        } // if
    } // for
    
    return( ucName );
}

u32 fileGetFreeSpace( void )
{
    u32 ulFreeSpace;
    u16 i;
    u8 ucByte;

    // Count unallocated and erased tags
    for( i = 0, ulFreeSpace = 0; i < pygmyRootVolume.Sectors; i++ ){
        ucByte = flashReadByte( pygmyRootVolume.ActiveFAT + ( i * 4 ) );
        ucByte >>= 6;
        //if( !ucByte || ucByte == 0xFF ){
        if( ucByte != 1 ){ // Updated for 2 bit allocation
            ulFreeSpace += pygmyRootVolume.SectorSize;
        } // if
    } // for

    return( ulFreeSpace );
}

u8 fileIsValidName( u8 *ucName )
{
    u8 i;   

    // alpha upper and lower allowed, 35-38 allowed, 45-57 allowed ( numeric plus - and . )
    // 95 ( _ ) and 126 ( ~ ) also allowed 
    for( i = 0; i < PYGMY_FILE_MAXFILENAMELEN+1 && ucName[ i ]; i++ ){    
        if( i == PYGMY_FILE_MAXFILENAMELEN || ( !isAlpha( ucName[ i ] ) && 
            !( ucName[ i ] > 34 && ucName[ i ] < 39 ) && !( ucName[ i ] > 44 && ucName[ i ] < 58 ) &&
            !(ucName[ i ] == 95 ) && !(ucName[ i ] == 126 ) ) ){
            return( 0 );
        } // if
    } // for

    return( 1 );
}

u8 fileCopy( u8 *ucCurrentName, u8 *ucName )
{
    PYGMYFILE pygmyFileTo, pygmyFileFrom;    
    u8 ucByte;

    //if( fileIsValidName( ucCurrentName ) && fileIsValidName( ucName ) ){  
    //if( fileIsValidName( ucName ) ){
        if( !fileOpen( &pygmyFileFrom, ucCurrentName, READ ) || 
            !fileOpen( &pygmyFileTo, ucName, WRITE ) ){
            return( 0 );
        } // if
        for( ; !(pygmyFileFrom.Attributes & EOF ); ){
            ucByte = fileGetChar( &pygmyFileFrom );
            if( !(pygmyFileFrom.Attributes & EOF ) ){
                filePutChar( &pygmyFileTo, ucByte );
            } // if
        } // for
        fileClose( &pygmyFileTo );
        return( 1 );
    //} // if

    return( 0 );
}

u8 fileRename( u8 *ucCurrentName, u8 *ucName )
{
    u32 ulFileEntry, ulFileID;
    //u16 uiFileID;
    //u8 ucAttrib;
 
    /*if( !fileIsValidName( ucName ) || !fileIsValidName( ucCurrentName ) ){
        return( 0 );
    } // if
    */
    if( !fileIsValidName( ucName ) ){
        return( 0 );
    } // if
    ulFileEntry = fileSeekName( ucCurrentName );
    if( !ulFileEntry || fileSeekName( ucName ) ){
        return( 0 );
    } // if

    ulFileEntry = pygmyRootVolume.ActiveFiles + ( ulFileEntry * 16); // Each file Entry exactly 16 bytes
    ulFileID = (u32)flashReadByte( ulFileEntry + 13 ) << 24;
    ulFileID |= (u32)flashReadWord( ulFileEntry + 14 );
    //ucAttrib = flashReadByte( ulFileEntry + 13 );
    //uiFileID = flashReadWord( ulFileEntry + 14 );
    flashWriteByte( ulFileEntry, 0x00 ); // Delete old entry by setting first byte to NULL
    
    ulFileEntry = fileGetFreeFileEntry( );
    if( !ulFileEntry ){
        return( 0 ); // This can only happen in the case of memory failure
    } // if

    ulFileEntry = pygmyRootVolume.ActiveFiles + ( ulFileEntry * 16); // Each file Entry exactly 16 bytes
    flashWriteBuffer( ulFileEntry, len( ucName ), ucName );
    //ulFileID = ( (u32)ucAttrib << 24 ) | uiFileID;
    
    flashWriteLong( ulFileEntry + 12, ulFileID );
    //flashWriteBuffer( ulFileEntry, len( ucName )+1, ucName );
    //flashWriteByte( ulFileEntry+13, ucAttrib );
    // Next we write the ID, this starts as the physical offset of the entry
    // This tag cannot be used to identify the entry location since the tag will follow the
    // file through name changes, etc. If ID changes, all associated FAT Entries are lost
    //flashWriteWord( ulFileEntry+14, uiFileID );

    return( 1 );
}

u16 fileGetFreeFileEntry( void )
{
    u32 ulAddress;
    u32 i, ii, iii, uiErased;// ulFilesTable
    u8 ucBuffer[ 16 ];

    // Read MSB of each Entry until 0xFF is found
    // First four entries are always reserved for File Volume Descriptor

    pygmyRootVolume.ActiveFiles = fileGetActiveFiles( );

    if( pygmyRootVolume.ActiveFiles == 0xFFFFFFFF ){ 
        return( 0 );
    } // if
    
    for( i = 0, uiErased = 0; i < pygmyRootVolume.MaxFiles; i++ ){
        ii = flashReadByte( pygmyRootVolume.ActiveFiles + ( ( i + 4 ) * 16 ) );
        if( ii == 0xFF ){
            return( 4 + i );
        } // if
        if( !uiErased && !ii ){ // We have a deleted entry
            uiErased = 4 + i; // Store deleted entry, if no entries left we will free up and use
        } // if
    } // for
    // If we exit the loop then we are out of memory
    if( uiErased ){
        // Create pointer to unused Table set
        if( pygmyRootVolume.ActiveFiles == pygmyRootVolume.Files_B ){ 
            ulAddress = PYGMY_FILE_FILES_A;
        } else{
            ulAddress = pygmyRootVolume.Files_B;
        } // else
        for( i = 0; i < pygmyRootVolume.FilesSectors; i++ ){
            flashSectorErase( ulAddress + ( i * pygmyRootVolume.SectorSize ) );
        } // for
        
        for( i = 0; i < 64; i++ ){ // Copy first 64 bytes of Volume descriptor to new sector
            flashWriteByte( ulAddress + i, flashReadByte( pygmyRootVolume.ActiveFiles + i ) );
        } // for
        
        for( i = 0; i < pygmyRootVolume.MaxFiles; i++ ){
            ii = ( i + 4 ) * 16;
            // Test first byte, if marked deleted ( 0 ), then skip entry since 0xFF cannot be written to 0xFF in flash
            ucBuffer[ 0 ] = flashReadByte( pygmyRootVolume.ActiveFiles + ii );
            if( !ucBuffer[ 0 ] ){
                continue;
            } // if
            for( iii = 0; iii < 16; iii++ ){
                ucBuffer[ iii ] = flashReadByte( pygmyRootVolume.ActiveFiles + ii + iii );  
                if( ucBuffer[ iii ] != 0xFF ){
                    flashWriteByte( ulAddress + ii + iii, ucBuffer[ iii ] );
                } // if
            } // for
        } // for
        // Now set first byte of ActiveFiles to 0 to delete
        flashWriteByte( pygmyRootVolume.ActiveFiles, 0x00 );
        pygmyRootVolume.ActiveFiles = ulAddress;
    
        return( uiErased ); // This was the location of the first known deleted entry, now free
    } // if
    
    // If we drop through, there are no free sectors remaining, mark volume as full
    pygmyRootVolume.Attributes |= VOLUMEFULL;

    return( 0 );
}

u16 fileGetFAT( u16 uiID, u16 uiIndex )
{
    u8 ucAlloc;
    u16 i, uiTmpID, uiID_Sector;

    for( i = 0; i < pygmyRootVolume.Sectors; i++ ){
        uiID_Sector = flashReadWord( pygmyRootVolume.ActiveFAT + ( i * 4 ) );
        ucAlloc = uiID_Sector >> 14;
        uiID_Sector <<= 2;
        uiID_Sector >>= 2; 
        
        uiTmpID = flashReadWord( pygmyRootVolume.ActiveFAT + ( i * 4 ) + 2 );
        if(  ucAlloc == 0x01 && uiID_Sector == uiIndex && uiID == uiTmpID ){
            return( i );
        } // if
    } // for
    #ifndef __PYGMY_BOOT
        pygmyRootVolume.Attributes |= VOLUMEERROR;
    #endif // __PYGMY_BOOT
    return( 0xFFFF ); // 0xFFFF indicates error
}

u16 fileGetFreeFATEntry( void )
{
    u32 i, ii, ulFAT, ulAddress, ulEntry;
    u8 ucErased = 0;

    // Read MSB of each Entry until 0xFF is found 
    // Return 0xFF for failure, last entry is invalid for a FAT Entry
    // Last Entry would always be reserved for Active Table Byte
    // [ ALLOCATION 4bits MSb ][ SEQUENCE 4 bits LSb ][ SEQUENCE 1B ][ FILE 2B ]
    // Note that sequence is a 12 bit field, allocation should be shifted 4 bits before testing 
    for( i = 0; i < pygmyRootVolume.Sectors; i++ ){
        ii = flashReadWord( pygmyRootVolume.ActiveFAT + ( i * 4 ) ); 
        ii >>= 14;
        if( ii == 3 ){
            return( i );
        } // if
        if( ii == 1 ){
            ucErased = 1;
        } // else if
    } // for
    
    // We have deleted entries in the FAT that can be recycled, since we have no free entries
    // To recycle we must swap the tables A to B or B to A
    if( ucErased ){
        pygmyRootVolume.Attributes &= ~VOLUMEFULL;
        pygmyRootVolume.ActiveFAT = fileGetActiveFAT( );
        if( pygmyRootVolume.ActiveFAT == pygmyRootVolume.FAT_B ){
            ulAddress = pygmyRootVolume.FAT_A;
        } else{
            ulAddress = pygmyRootVolume.FAT_B;
        } //else
        // Now that alternate table location is known clear, copy while omitting deleted entries
        // Also, the Active FAT and Files must be updated after copy, at this point ulAddress contains the
        //     destination for sector copy and the ActiveFAT contains 
        for( i = 0, ulFAT = 0xFFFFFFFF; i < pygmyRootVolume.Sectors; i++ ){
            ii = i * 4;
            ulEntry = flashReadLong( pygmyRootVolume.ActiveFAT + ii );
            if( ulEntry & 0xC0000000 ){ // ToDo: Test
            //if( ulEntry & 0xFF000000 ){
                flashWriteLong( ulAddress + ii, ulEntry );
            } else if( ulFAT == 0xFFFFFFFF ){
                ulFAT = i; // Only assign ulFAT on first recycled Entry
            } // else if
        } // for 
        // Now delete original table and make updated table active
        // This will set the Active/Inactive byte to 0xFF, or inactive
        for( i = 0; i < pygmyRootVolume.FATSectors; i++ ){ // If more then 1 sector reserved for each A and B
            flashSectorErase( pygmyRootVolume.ActiveFAT + ( i * pygmyRootVolume.SectorSize ) );
        } // for
        // Finally clear last byte of FAT table to mark as unused
        // Warning! Changed to mark ACTIVE table with 0!
        pygmyRootVolume.ActiveFAT = ulAddress; // Now update the ActiveFAT to the updated table
        flashWriteByte( pygmyRootVolume.ActiveFAT + (pygmyRootVolume.FATSectors * pygmyRootVolume.SectorSize) - 1, 0x00 );
        
        return( (u16)ulFAT ); // Return the recycled value
    } // if 
    
    return( 0xFFFF ); // No memory left
}

u32 fileGetActiveFAT( void )
{  
    // Warning! Changed to mark ACTIVE table with 0!
    // Note: Sectors may never be an even value such as 1024, 
    //  normally the file and FAT sectors will see to this, but the last 32 bit field should always be
    //  left for use by the Active/Inactive tag below
    if( flashReadByte( pygmyRootVolume.FAT_A + (pygmyRootVolume.FATSectors * pygmyRootVolume.SectorSize) - 1 ) == 0x00 ){
        return( pygmyRootVolume.FAT_A ); // Table A Active, last byte is set to 0x00 for active, 0xFF for inactive
    } // if
 
    if( flashReadByte( pygmyRootVolume.FAT_B + (pygmyRootVolume.FATSectors * pygmyRootVolume.SectorSize) - 1 ) == 0x00 ){
        return( pygmyRootVolume.FAT_B ); // Table B Active, last byte is set to 0x00 for active, 0xFF for inactive
    } // if
    
    flashWriteByte( pygmyRootVolume.FAT_A + (pygmyRootVolume.FATSectors * pygmyRootVolume.SectorSize) - 1, 0x00 );

    return( pygmyRootVolume.FAT_A );
}

u32 fileGetActiveFiles( void )
{
    if( flashReadByte( PYGMY_FILE_FILES_A ) ){
        return( PYGMY_FILE_FILES_A );
    } // if
    if( flashReadByte( pygmyRootVolume.Files_B ) ){
        return( pygmyRootVolume.Files_B );
    } // if
    #ifndef __PYGMY_BOOT
        pygmyRootVolume.Attributes |= VOLUMEERROR;
    #endif // __PYGMY_BOOT
    
    return( 0xFFFFFFFF ); // both Files tables inactive means memory failure
}

u8 fileDeleteFATEntry( u16 uiID_File, u16 uiID_Sector )
{
    u32 i, ii;
    u16 uiTmpID, uiDeleted, uiSector;
    u8 ucAlloc;

    // ucID_Sector == 0 calls for a complete erase
    for( i = 0, uiDeleted = 0; i < pygmyRootVolume.Sectors; i++ ){ // There are as many entries as Sectors
        ii = i * 4;
        uiSector = flashReadWord( pygmyRootVolume.ActiveFAT + ii );
        ucAlloc = uiSector >> 14;
        uiSector &= 0x3FFF;
        uiTmpID = flashReadByte( pygmyRootVolume.ActiveFAT + ii + 2 ) << 8;
        uiTmpID |= flashReadByte( pygmyRootVolume.ActiveFAT + ii + 3 );
        if( uiTmpID == uiID_File ){
            pygmyRootVolume.Attributes &= ~VOLUMEFULL;
            if( !uiID_Sector ){
                flashWriteByte( pygmyRootVolume.ActiveFAT + ii, 0x0F );
                uiDeleted = 1;
            } else if( uiID_Sector == uiSector ){
                if ( ucAlloc ){
                    flashWriteByte( pygmyRootVolume.ActiveFAT + ii, 0x0F );
                    return( 1 );
                } // if
            } //else if
        } // if
    } // for
 
    return( uiDeleted );
}

u16 fileAllocateFAT( u16 uiID_File, u16 uiID_Sector )
{
    u32 ulFAT, ulAddress;

    // BIT15 set, BIT14 set is empty
    // BIT15 clear, BIT14 set is allocated
    // BIT15 clear, BIT14 clear is deleted
    // An ID_Sector of 0x00 and 0xFF reserved for error
    // If these IDs are requested, return error ( 0xFFFF )
    if( !uiID_Sector || uiID_Sector > 0x3FFE ){
        #ifndef __PYGMY_BOOT
            pygmyRootVolume.Attributes |= VOLUMEERROR;
        #endif //__PYGMY_BOOT
        return( 0xFFFF );
    } // if
    ulFAT = fileGetFreeFATEntry( );
    if( ulFAT != 0xFFFF ){
        flashSectorErase( pygmyRootVolume.FirstFileSector + ( ulFAT * pygmyRootVolume.SectorSize ) );
        ulAddress = ulFAT * 4;
        //flashWriteByte( pygmyRootVolume.ActiveFAT + ulAddress++, 0x7F ); // 0x07 = Allocated, shifted 4 bits, F leaves bits unaffected
        flashWriteWord( pygmyRootVolume.ActiveFAT + ulAddress, BIT14 | uiID_Sector ); 
        flashWriteWord( pygmyRootVolume.ActiveFAT + ulAddress + 2, uiID_File );  
    } // if

    return( ulFAT );
}
 
u16 fileGetMaxFiles( void )
{
    return( pygmyRootVolume.MaxFiles );
}

u16 fileGetName( u16 uiFileEntry, u8 *ucName )
{
    // This function is incremental in nature and designed to provided
    // listing information
    u32 ulAddress;
    u16 uiID;
    u8 i, ucBuffer[ 16 ];

    if( uiFileEntry > pygmyRootVolume.MaxFiles ){
        return( 0 );
    } // if
    
    flashReadBuffer( pygmyRootVolume.ActiveFiles + ( uiFileEntry * 16 ), ucBuffer, 16 );
    uiID = ucBuffer[ 14 ];
    uiID = uiID << 8 | ucBuffer[ 15 ];
    
    for( i = 0; i < 12; i++ ){
        if( !ucBuffer[ i ] || ucBuffer[ i ] == 0xFF ){
            break;
        } else{
            ucName[ i ] = ucBuffer[ i ];
        } // else
    } // for
    ucName[ i ] = '\0';
    if( !i ){
        return( 0 );
    } // if
            
    /*ulAddress = pygmyRootVolume.ActiveFiles + 64 + ( uiFileEntry * 16 );
    for( i = 0; i < PYGMY_FILE_MAXFILENAMELEN; i++ ){
        ucName[ i ] = flashReadByte( ulAddress + i );
        if( ucName[ i ] == 0xFF ){
            ucName[ i ] = '\0';
            if( !i ){
                return( 0 );
            } else{
                break;
            } // else
        } // if
    } // for
    */
    // return the ID of the file to use to load length
    return( uiID );//flashReadWord( ulAddress + 14 ) );
}

u16 fileSeekName( u8 *ucName )
{
    u32 i, ii;
    u8 ucBuffer[ 13 ];
    
    for( i = 0; i < pygmyRootVolume.MaxFiles; i++ ){
        /*for( ii = 0; ii < PYGMY_FILE_MAXFILENAMELEN; ii++ ){
            ucBuffer[ ii ] = flashReadByte( pygmyRootVolume.ActiveFiles + ( ( i + 4 ) * 16 ) + ii );
            if( !ucBuffer[ ii ] || ucBuffer[ ii ] == 0xFF ){
                ucBuffer[ ii ] = '\0';
                break;
            } // if
        } // for
        */
        flashReadBuffer( pygmyRootVolume.ActiveFiles + ( ( i + 4 ) * 16 ), ucBuffer, 13 );
        if( isStringSame( ucName, ucBuffer ) ){
            return( 4 + i );
        } // if
    } // for

    return( 0 );
}

u8 fileClose( PYGMYFILE *pygmyFile )
{
    // Close only flushes file. Write operations may continue on closed file 
    // until PYGMYFILE is reused. Because of this, Sector MUST be updated
    // ONLY if ID_Sector is currently 1. This is due to the fact that the
    // file length is stored in ID_Sector 1 and moving this while still in
    // ID_Sector 1 moves the current Sector to the newly allocated address

    u32 i, ulAddress, ulAddress2;
    u8 ucByte;
    
    ulAddress = pygmyRootVolume.FirstFileSector + ( fileGetFAT( pygmyFile->ID_File, 1 ) * pygmyRootVolume.SectorSize );
    if( fileWriteLength( pygmyFile, pygmyFile->Length ) ){
        return( 1 );
    } // if
    // Out of length slots in Sector, Handle Sector copy and re-allocation
    fileDeleteFATEntry( pygmyFile->ID_File, 1 ); // Deletes on first sequential FAT entry
    ulAddress2 = fileAllocateFAT( pygmyFile->ID_File, 1 ); // Allocates new sector to replace first FAT entry
    if( ulAddress2 == 0xFFFF ){
        return( 0 ); // Time to panic, there is no room left to complete operation and we have run out of options
    } // if
    ulAddress2 += pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize;
    if( pygmyFile->ID_Sector == 1 ){
        pygmyFile->Sector = ulAddress2; // Sector must be updated if we are still in sector 1
    } // if
    ulAddress2 *= pygmyRootVolume.SectorSize;
    
    for( i = 128; i < pygmyRootVolume.SectorSize;  ){
        ucByte = flashReadByte( ulAddress + i );
        if( ucByte != 0xFF ) {// Don't attempt to erase the erased
            flashWriteByte( ulAddress2 + i, ucByte ); 
        } // if
        ++i;
    } // for
    
    flashSectorErase( ulAddress );// Pre-erase to speed next write operation
    if( fileWriteLength( pygmyFile, pygmyFile->Length ) ){
        return( 1 ); 
    } // if
    
    return( 0 ); 
}

u8 fileWriteLength( PYGMYFILE *pygmyFile, u32 ulLength )
{
    u32 i, ulTmp, ulAddress;

    ulAddress = pygmyRootVolume.FirstFileSector + ( fileGetFAT( pygmyFile->ID_File, 1 ) * pygmyRootVolume.SectorSize );
    
    for( i = 0; i < 128; ){
        ulTmp = flashReadLong( ulAddress + i );
        
        if( ulTmp == 0xFFFFFFFF ){
            flashWriteLong( ulAddress + i, ulLength );
            return( 1 );
        } else if( ulTmp ){ //ulTmp != 0xFFFFFFFF && ulTmp != 0x00000000 ){
            if( ulTmp == ulLength ){ // Don't waste a write cycle if Length hasn't changed
                return( 1 );
            } // if
            flashWriteLong( ulAddress + i, 0x00000000 );
        } // if
        i += 4;
    } // if
    
    return( 0 );
}

u32 fileGetLength( u32 ulID_File )
{
    u32 i, ulLen, ulAddress;

    ulAddress = fileGetFAT( ulID_File, 1 ); // This returns sector index, not address
    if( ulAddress == 0xFFFF ){
        return( 0 );
    } // if
    // Sector index retrieved from FAT is 0 based, add index to first file sector and mult by
    // sector size to get address
    //ulAddress += ( pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize );
    //ulAddress *= pygmyRootVolume.SectorSize;
    ulAddress = pygmyRootVolume.FirstFileSector + ( ulAddress * pygmyRootVolume.SectorSize );
    for( i = 0; i < 128; ){
        ulLen = flashReadLong( ulAddress + i );
        i += 4;
        if( ulLen != 0x00000000 && ulLen != 0xFFFFFFFF ){
            return( ulLen );
        } // if
    } // for

    return( 0 ); 
}

u8 filePutBuffer( PYGMYFILE *pygmyFile, u16 uiLen, u8 *ucBuffer )
{
    for( ; uiLen--; ){
        if( !filePutChar( pygmyFile, *(ucBuffer++) ) ){
            return( 0 );
        } // if
    } // for

    return( 1 );
}

u8 filePutString( PYGMYFILE *pygmyFile, u8 *ucString )
{   
    for( ; *ucString; ){
        if( !filePutChar( pygmyFile, *(ucString++) ) ){
            return( 0 );
        } // if
    } // for

    return( 1 );
}

u8 filePutChar( PYGMYFILE *pygmyFile, u8 ucChar )
{
    u32 i, ulAddress;
    
    if( !( pygmyFile->Attributes & WRITE ) || pygmyRootVolume.Attributes & VOLUMEFULL ) {
        return( 0 ); // In Write mode, Length and Index must match or memory corruption will occur
    } // if
    if( !( (( pygmyFile->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ){      
        i = fileAllocateFAT( pygmyFile->ID_File, pygmyFile->ID_Sector + 1 );
        if( pygmyRootVolume.Attributes & VOLUMEFULL ){
            return( 0 );// memory is full!
        } // if
        pygmyFile->Sector = i + ( pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize );
        ++pygmyFile->ID_Sector;     
    } // if
    ulAddress = ( pygmyFile->Sector * pygmyRootVolume.SectorSize ) + ((( pygmyFile->Index + 128 ) ) % pygmyRootVolume.SectorSize );
    if( ulAddress < ( pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize ) ) {//|| ulAddress > 0x03FFF000 ){
        #ifndef __PYGMY_BOOT
            pygmyRootVolume.Attributes |= VOLUMEERROR;
        #endif // __PYGMY_BOOT
        return( 0 );
    } // if 
    flashWriteByte( ulAddress, ucChar );
    
    ++pygmyFile->Index;
    pygmyFile->Length = pygmyFile->Index;

    return( 1 );
}

void filePutWord( PYGMYFILE *pygmyFile, u16 uiData, u8 ucEndian )
{
    if( ucEndian == BIGENDIAN ){
        filePutChar( pygmyFile, (u8)((u16) uiData >> 8 ) );
        filePutChar( pygmyFile, (u8)uiData );
    } else {
        filePutChar( pygmyFile, (u8)uiData );
        filePutChar( pygmyFile, (u8)((u16) uiData >> 8 ) );
    } // else
}

void filePutLong( PYGMYFILE *pygmyFile, u32 ulData, u8 ucEndian )
{
    if( ucEndian == BIGENDIAN ){
        filePutChar( pygmyFile, (u8)((u32) ulData >> 24 ) );
        filePutChar( pygmyFile, (u8)((u32) ulData >> 16 ) );
        filePutChar( pygmyFile, (u8)((u32) ulData >> 8 ) );
        filePutChar( pygmyFile, (u8) ulData );
    } else {
        filePutChar( pygmyFile, (u8) ulData );
        filePutChar( pygmyFile, (u8)((u32) ulData >> 8 ) );
        filePutChar( pygmyFile, (u8)((u32) ulData >> 16 ) );
        filePutChar( pygmyFile, (u8)((u32) ulData >> 24 ) );
    } // else
}

u16 fileGetBuffer( PYGMYFILE *pygmyFile, u16 uiLen, u8 *ucBuffer )
{
    u16 i;

    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = fileGetChar( pygmyFile );
        if( fileIsEOF( pygmyFile ) ){
            return( i );
        } // if
    } // for

    return( i );
}

u16 fileGetWord( PYGMYFILE *pygmyFile, u8 ucEndian )
{
    u16 uiData;
    u8 i;

    if( ucEndian == BIGENDIAN ){
        uiData = (u16)fileGetChar( pygmyFile ) << 8;
        uiData |= (u16)fileGetChar( pygmyFile );
    } else {
        uiData = (u16)fileGetChar( pygmyFile );
        uiData |= (u16)fileGetChar( pygmyFile ) << 8;
    } // else

    return( uiData );
}

u32 fileGetLong( PYGMYFILE *pygmyFile, u8 ucEndian )
{
    u32 ulData;

    if( ucEndian == BIGENDIAN ){
        ulData = (u32)fileGetChar( pygmyFile ) << 24;
        ulData |= (u32)fileGetChar( pygmyFile ) << 16;
        ulData |= (u32)fileGetChar( pygmyFile ) << 8;
        ulData |= (u32)fileGetChar( pygmyFile ); 
    } else{
        ulData = (u32)fileGetChar( pygmyFile );
        ulData |= (u32)fileGetChar( pygmyFile ) << 8;
        ulData |= (u32)fileGetChar( pygmyFile ) << 16;
        ulData |= (u32)fileGetChar( pygmyFile ) << 24;
    } // else

    return( ulData );
}

#ifdef __FILE_SMALLGETCHAR
u8 fileGetChar( PYGMYFILE *pygmyFile )
{ 
    // This function only remains for use with bootloaders
    u32 i, ulAddress;
    
    if( pygmyFile->Index >= pygmyFile->Length ){
        pygmyFile->Attributes |= EOF;
        return( 0 );
    } // if
    if( !( (( pygmyFile->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ){
        i = fileGetFAT( pygmyFile->ID_File, pygmyFile->ID_Sector+1 );
        if( i == 0xFFFF ){
            pygmyFile->Attributes |= EOF;
            return( 0 ); 
        } // if
        pygmyFile->Sector = i + ( pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize );
        ++pygmyFile->ID_Sector;
    } // if
    
    ulAddress = ( pygmyFile->Sector * pygmyRootVolume.SectorSize ) + ((( pygmyFile->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ;
    if( pygmyFile->Index < pygmyFile->Length ){
        ++pygmyFile->Index;
    } // if

    return( flashReadByte( ulAddress ) );
}
#else
u8 fileGetChar( PYGMYFILE *pygmyFile )
{
    #define PYGMY_FILE_BUFLEN   128 
    static PYGMYFILE *pygmyFilePtr;
    static u32 ulIndex;
    u32 i, ulAddress;
    static u8 ucIndex, ucChar, ucBuffer[ PYGMY_FILE_BUFLEN ];

    if( pygmyFile != pygmyFilePtr || ulIndex != pygmyFile->Index ){
        pygmyFilePtr = pygmyFile;
        ucIndex = PYGMY_FILE_BUFLEN;
    } // if
    
    if( pygmyFilePtr->Index >= pygmyFilePtr->Length ){
        pygmyFilePtr->Attributes |= EOF;
        return( 0 );
    } // if
    if( pygmyFilePtr->Resource ){ // If resource use index to return value
        ucChar = pygmyFilePtr->Resource[ pygmyFilePtr->Index++ ];
        return( ucChar );
    } else if( ucIndex == PYGMY_FILE_BUFLEN || !( (( pygmyFilePtr->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ){
        ucIndex = 0;
        if( !( (( pygmyFilePtr->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ) {
            
            i = fileGetFAT( pygmyFilePtr->ID_File, pygmyFilePtr->ID_Sector+1 );
            if( i == 0xFFFF ){
                pygmyFilePtr->Attributes |= EOF;
                return( 0 ); 
            } // if
            pygmyFilePtr->Sector = i + ( pygmyRootVolume.FirstFileSector / pygmyRootVolume.SectorSize );
            ++pygmyFilePtr->ID_Sector;    
        } // if
    
        ulAddress = ( pygmyFilePtr->Sector * pygmyRootVolume.SectorSize ) + ((( pygmyFilePtr->Index + 128 ) ) % pygmyRootVolume.SectorSize ) ;
        
        ulAddress = ulAddress | 0x03000000;
        //spiGetBuffer( &pygmyFlashSPI, ulAddress, ucBuffer, PYGMY_FILE_BUFLEN );
        FLASH_CS_LOW;
        spiWriteLong( &pygmyFlashSPI, ulAddress );
        for( i = 0; i < PYGMY_FILE_BUFLEN; i++ ){
            ucBuffer[ i ] = spiReadByte( &pygmyFlashSPI );
        } // for   
        FLASH_CS_HIGH;
    } // if
      
    if( pygmyFilePtr->Index < pygmyFilePtr->Length ){
        ++pygmyFilePtr->Index;
        ulIndex = pygmyFilePtr->Index;
    } // if
    
    return( ucBuffer[ ucIndex++ ] );
}
#endif

u8 fileMountRoot( void )//Volume( void )//PYGMYFILEVOLUME *pygmyFileVolume )
{
    u32 ulTableOffset;
    
    flashWriteEnable( );
    // Note that the Files_A and Files_B values are static and will not change, both Files A/B will be same in both tables 
    //pygmyRootVolume.Files_A = PYGMY_FILE_FILES_A;//Pygmy_FLASH_ReadLong( PYGMY_FILE_VOLUME_FIELD_FILES_A );
    pygmyRootVolume.Files_B = flashReadLong( PYGMY_FILE_VOLUME_FIELD_FILES_B );
    
    ulTableOffset = fileGetActiveFiles();
    if( ulTableOffset == 0xFFFFFFFF ){ // May not have been formatted
        fileFormat( "" );
        ulTableOffset = fileGetActiveFiles();
        if( ulTableOffset == 0xFFFFFFFF ){ // After format this means memory failure
            return( 0 );
        } // if
    } // if
    
    // Add code to copy volume name to structure here, if needed
     
    pygmyRootVolume.Attributes          = flashReadByte( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_ATTRIB );
    pygmyRootVolume.Sectors             = flashReadWord( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_SECTORS );
    pygmyRootVolume.SectorSize          = flashReadWord( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_SECTORSIZE );
    pygmyRootVolume.MaxFiles            = flashReadLong( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_MAXFILES );
    pygmyRootVolume.MediaSize           = flashReadLong( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_MEDIASIZE );
    pygmyRootVolume.EntriesPerFAT       = flashReadWord( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_ENTRIESPERFAT );
    pygmyRootVolume.FilesSectors        = flashReadWord( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_FILESSECTORS );
    pygmyRootVolume.FATSectors          = flashReadWord( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_FATSECTORS );
    pygmyRootVolume.FirstFileSector     = flashReadLong( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_FIRSTFILESECTOR );
    pygmyRootVolume.FAT_A               = flashReadLong( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_FAT_A );
    pygmyRootVolume.FAT_B               = flashReadLong( ulTableOffset + PYGMY_FILE_VOLUME_FIELD_FAT_B );
    pygmyRootVolume.ActiveFAT           = fileGetActiveFAT();

    return( 1 );
}

#ifdef __PYGMYSTREAMS
void filePrintList( u8 ucStream )
{
    u16 i, ii, uiLen, uiID;
    u8 ucName[ 16 ];

    for( i = 0; i < pygmyRootVolume.MaxFiles; i++ ){
        flashReadBuffer( pygmyRootVolume.ActiveFiles + ( ( i + 4 ) * 16 ), ucName, 16 );
        PYGMY_WATCHDOG_REFRESH;
        if( ucName[ 0 ] && ucName[ 0 ] != 0xFF && !(ucName[ 13 ] & (HIDDEN|BURIED) ) ){
            uiID = ucName[ 14 ];
            uiID = ( uiID << 8 ) | ucName[ 15 ]; //fileGetName( i+4, ucName );
            //print( ucStream, "\r%s", ucName ); 
            //uiLen = 16 - len( ucName );
            //for( ii = 0; ii < uiLen; ii++ ){
            //    print( ucStream, " " );
            //} // for
            //print( ucStream, "%d", fileGetLength( uiID ) );
            print( ucStream, "\r%8d %s", fileGetLength( uiID ), ucName ); 
        } // if
    } // for
    //print( ucStream, "\rFree:           %d\r", fileGetFreeSpace() );
    print( ucStream, "\r%8d Free", fileGetFreeSpace() );
}

void filePrintDebug( u8 ucStream )
{
    u32 i, ulA, ulB;
    u8 ucBuffer1[ 16 ], ucBuffer2[ 16 ], ucTerm[] = { 0xFF, 0x00 };

    print( ucStream, "\rRoot:" );
    print( ucStream, "\r\tAttrib: 0x%2X", pygmyRootVolume.Attributes );
    print( ucStream, "\r\tSectors: %d", pygmyRootVolume.Sectors );
    print( ucStream, "\r\tSectorSize: %d", pygmyRootVolume.SectorSize );
    print( ucStream, "\r\tMaxFiles: %d", pygmyRootVolume.MaxFiles );
    print( ucStream, "\r\tMediaSize: %d", pygmyRootVolume.MediaSize );
    print( ucStream, "\r\tEntriesPerFAT: %d", pygmyRootVolume.EntriesPerFAT );
    print( ucStream, "\r\tFilesSectors: %d", pygmyRootVolume.FilesSectors );
    print( ucStream, "\r\tFATSectors: %d", pygmyRootVolume.FATSectors );
    print( ucStream, "\r\tFirstFileSector: %d", pygmyRootVolume.FirstFileSector );
    print( ucStream, "\r\tFAT_A: %d", pygmyRootVolume.FAT_A );
    print( ucStream, "\r\tFAT_B: %d", pygmyRootVolume.FAT_B );
    print( ucStream, "\r\tActiveFAT: %d", pygmyRootVolume.ActiveFAT );
    print( ucStream, "\r\tActiveFiles: %d", pygmyRootVolume.ActiveFiles );

    print( ucStream, "\r\rFAT_A         FAT_B" );
    for( i = 0; i < ( pygmyRootVolume.EntriesPerFAT * pygmyRootVolume.FATSectors ); i++ ){
        ulA = flashReadLong( pygmyRootVolume.FAT_A + ( i * 4 ) );
        ulB = flashReadLong( pygmyRootVolume.FAT_B + ( i * 4 ) );
        if( ulA == 0xFFFFFFFF && ulB == 0xFFFFFFFF ){
            continue;
        } // if
        print( ucStream, "\r0x%8X    0x%8X", ulA, ulB);
    } // for
    PYGMY_WATCHDOG_REFRESH;
    print( ucStream, "\r\rFiles_A               Files_B" );
    for( i = 4; i < pygmyRootVolume.MaxFiles+4; i++ ){
        flashReadBuffer( i * 16, ucBuffer1, 16 );
        flashReadBuffer( pygmyRootVolume.Files_B + ( i * 16 ), ucBuffer2, 16 );
        
        if( !ucBuffer1[ 0 ] ){
            copyString( "DELETED", ucBuffer1 );
        } // if
        
        if( ucBuffer1[ 0 ] == 0xFF ){
            copyString( "EMPTY", ucBuffer1 );
        } else{
            replaceChars( ucBuffer1, ucTerm, 0x00 );
        } // else
        if( !ucBuffer2[ 0 ] ){
            copyString( "DELETED", ucBuffer1 );
        } // if
        
        if( ucBuffer2[ 0 ] == 0xFF ){
            copyString( "EMPTY", ucBuffer2 );
        } else{
            replaceChars( ucBuffer2, ucTerm, 0x00 );
        } // else
        print( ucStream, "\r%10s %10s", ucBuffer1, ucBuffer2 );
        PYGMY_WATCHDOG_REFRESH;
    } // for
}
#endif // __PYGMYSTREAMS

u32 fileFormat( u8 *ucName )
{
    u32 ulCapacity, ulFATBase;
    u16 uiSectorSize, uiSectors, uiSecPerFAT;
    u8 ucAttribs;

    #ifndef __PYGMY_BOOT
        // Verify name isn't too long 
        if( ucName[ 0 ] && !fileIsValidName( ucName ) ){ 
            return( 0 );
        } // if
    #endif
    // Start by erasing chip
    
    flashWriteEnable( );
    flashChipErase( );
    ulCapacity = flashReadID( );
    uiSecPerFAT = 1;
    ucAttribs = 0;
    // If ID is for FLASH, use the following settings:
        ucAttribs = READ|WRITE|MEDIAFLASH;
        uiSectorSize = 0x1000;
    // Flash ICs up to 32Mbit use 1 4KB sector for FAT_A and 1 for FAT_B
    if( ulCapacity == SST_ID_32M ){
        uiSectors = 1024;
        uiSecPerFAT = 4;
    } else if( ulCapacity == SST_ID_16M ){
        uiSectors = 512;
        uiSecPerFAT = 2;
    } else if( ulCapacity == SST_ID_8M ){
        uiSectors = 256;
    } else if( ulCapacity == SST_ID_4M ){
        uiSectors = 128;
    } // else if
    
    #ifndef __PYGMY_BOOT
        // Next write optional volume name, if needed
        if( ucName[ 0 ] ){ // As long as first char of name field is non-null, name is optional
            flashWriteString( PYGMY_FILE_VOLUME_FIELD_NAME, TRUE, ucName );
        } // if
    #endif
    flashWriteByte( PYGMY_FILE_VOLUME_FIELD_ATTRIB, ucAttribs );
    flashWriteWord( PYGMY_FILE_VOLUME_FIELD_SECTORS, uiSecPerFAT * 252 ); 
    flashWriteWord( PYGMY_FILE_VOLUME_FIELD_SECTORSIZE, uiSectorSize ); // hard coded to SST family sectors size
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_MAXFILES, uiSecPerFAT * 252 );
    // Media size is defined as total sectors minus FILE and FAT sectors * SECTOR SIZE
    ulCapacity = ( uiSectors - ( uiSecPerFAT * 2 ) ) * uiSectorSize;
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_MEDIASIZE, ulCapacity );
    flashWriteWord( PYGMY_FILE_VOLUME_FIELD_ENTRIESPERFAT, uiSecPerFAT * 252 ); // 252
    flashWriteWord( PYGMY_FILE_VOLUME_FIELD_FILESSECTORS, uiSecPerFAT );
    flashWriteWord( PYGMY_FILE_VOLUME_FIELD_FATSECTORS, 1 );
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_FIRSTFILESECTOR, ( ( ( uiSecPerFAT * 2 ) + 2 )* uiSectorSize ) );
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_FILES_A, PYGMY_FILE_FILES_A );
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_FILES_B, uiSecPerFAT * uiSectorSize );
    ulFATBase = ( uiSecPerFAT * 2 ) * uiSectorSize;
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_FAT_A, ulFATBase );
    flashWriteLong( PYGMY_FILE_VOLUME_FIELD_FAT_B, ulFATBase + uiSectorSize );
    flashWriteByte( ( ulFATBase - uiSectorSize ) - 1, 0x00 ); // Mark FAT_A as active
    
    fileMountRoot();
    
    return( ulCapacity );
}       

#ifdef __PYGMY_BOOT
    //#define FLASH_CS_LOW    
    //#define FLASH_CS_HIGH   
void spiWriteByte( PYGMYSPIPORT *pymgyPort, u8 ucByte )
{
    // Clocks out 8 bits
        u16 i;
        
        FLASH_SCK_LOW;                              // Clock starts low, low-high-low clocks data in or out
        for( i = 0; i < 8; i++ ){                       // Clock out Write CMD
                if( ucByte & ( BIT7 >> i ) ){       // SRAM clocks MSB first, test each bit in sequence from 7 to 0             
            FLASH_MOSI_HIGH;                  // SlaveInMasterOut high if bit set
                } else{                             //
            FLASH_MOSI_LOW;                   // SlaveInMasterOut low if bit clear
        } // else                           //
                FLASH_SCK_HIGH;                                 // clock must start low, transition high
                FLASH_SCK_LOW;                              // Low transition finishes clock sequence
    } // for
} 

void spiWriteWord( PYGMYSPIPORT *pymgyPort,  u16 uiWord )
{
    // Clocks out 16 bits
        u16 i;
        
        FLASH_SCK_LOW;                                          // Clock starts low, low-high-low clocks data in or out
        for( i = 0; i < 16; i++ ){                          // 
                if( uiWord & ( 0x8000 >> i ) ){     // 
            FLASH_MOSI_HIGH;                  // SlaveInMasterOut high if bit set
                } else{                                     //
            FLASH_MOSI_LOW;                   // SlaveInMasterOut low if bit clear
        } // else                           // 
                FLASH_SCK_HIGH;                                 // clock must start low, transition high
                FLASH_SCK_LOW;                                  // Low transition finishes clock sequence
    } // for
} 

void spiWriteLong( PYGMYSPIPORT *pymgyPort, u32 ulLong )
{
    // Clocks out 32 bits
        u16 i;
        
        FLASH_SCK_LOW;                                          // Clock starts low, low-high-low clocks data in or out
        for( i = 0; i < 32; i++ ){                          // 
                if( ulLong & ( 0x80000000 >> i ) ){     // 
            FLASH_MOSI_HIGH;                  // SlaveInMasterOut high if bit set
                } else{                                     //        
            FLASH_MOSI_LOW;                   // SlaveInMasterOut low if bit clear
        } // else                           //     
                FLASH_SCK_HIGH;                                 // clock must start low, transition high
                FLASH_SCK_LOW;                                  // Low transition finishes clock sequence
    } // for
} 

u8 spiReadByte( PYGMYSPIPORT *pymgyPort )
{
    // Clocks in 8 bits
    u16 i;
        u8 ucByte;
        
    FLASH_SCK_LOW;                            // Clock starts low, low-high-low clocks data in or out
        for( i = 0, ucByte = 0; i < 8; i++ ){   // 
                FLASH_SCK_HIGH;                                 // clock must start low, transition high                        
        if( FLASH_MISO_STATE ){               // Test port input for high and set bit in ucByte
                        ucByte |= ( BIT7 >> i );        //
        } // if                             //
                FLASH_SCK_LOW;                                  // Low transition finishes clock sequence
    } // for
        
        return( ucByte );
}

#endif

u16 flashReadWord( u32 ulAddress )
{
    u16 uiWord;

    ulAddress |=  0x03000000;
    //uiWord = spiGetWord( &pygmyFlashSPI, ulAddress );
    FLASH_CS_LOW;               // CS active low to enable chip  
    spiWriteLong( &pygmyFlashSPI, ulAddress );
        uiWord = (u16)spiReadByte( &pygmyFlashSPI ) << 8;
    uiWord |= (u16)spiReadByte( &pygmyFlashSPI );
    FLASH_CS_HIGH;
    
    return( uiWord );
}

u32 flashReadLong( u32 ulAddress )
{
    u32 ulLong;

    ulAddress |= 0x03000000;
    //ulLong = spiGetLong( &pygmyFlashSPI, ulAddress );
    FLASH_CS_LOW; 
    spiWriteLong( &pygmyFlashSPI, ulAddress );
    ulLong = (u32)spiReadByte( &pygmyFlashSPI ) << 24;
    ulLong |= (u32)spiReadByte( &pygmyFlashSPI ) << 16;
    ulLong |= (u32)spiReadByte( &pygmyFlashSPI ) << 8;
    ulLong |= (u32)spiReadByte( &pygmyFlashSPI );
    FLASH_CS_HIGH;

    return( ulLong );
}

u8 flashReadByte( u32 ulAddress )
{
    u8 ucByte;
    
    ulAddress |= 0x03000000;
    //ucByte = spiGetChar( &pygmyFlashSPI, ulAddress );
    FLASH_CS_LOW;
    spiWriteLong( &pygmyFlashSPI, ulAddress );
        ucByte = spiReadByte( &pygmyFlashSPI );
    FLASH_CS_HIGH;
        
        return( ucByte );    
}

void flashReadBuffer( u32 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    u16 i;

    ulAddress = ulAddress | 0x03000000;
    //spiGetBuffer( &pygmyFlashSPI, ulAddress, ucBuffer, uiLen );
    FLASH_CS_LOW; 
    spiWriteLong( &pygmyFlashSPI, ulAddress );
    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = spiReadByte( &pygmyFlashSPI );
    } // for
    FLASH_CS_HIGH;   
}

u8 flashReadStatus( void )
{
    u8 ucByte;
    
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_RDSR );
    ucByte = spiReadByte( &pygmyFlashSPI );
    FLASH_CS_HIGH;

    return( ucByte );
}

u8 flashReadID( void )
{
    u8 ucByte;
    
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_JEDECID );
    ucByte = spiReadByte( &pygmyFlashSPI ); // Manufacturer
    ucByte = spiReadByte( &pygmyFlashSPI ); // Type
    ucByte = spiReadByte( &pygmyFlashSPI ); // Capacity
    FLASH_CS_HIGH;

    return( ucByte );
    
}

void flashSectorErase( u32 ulSector )
{
    ulSector |= 0x20000000; // 4KB Sector Erase
    //spiPutCommand( &pygmyFlashSPI, PYGMY_FLASH_WREN );
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN );
    FLASH_CS_HIGH;
    //spiWriteAddress( &pygmyFlashSPI, ulSector );
    FLASH_CS_LOW;
    spiWriteLong( &pygmyFlashSPI, ulSector );
    FLASH_CS_HIGH;
    flashWaitForBusy();
}

u8 flashChipErase( void )
{
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN );
    FLASH_CS_HIGH;
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_ERASE_ALL );
    FLASH_CS_HIGH;
    flashWaitForBusy( );
    
    return( 1 );
}

void flashWaitForBusy( void )
{
    for( ; PYGMY_FLASH_STATUS_BUSY & flashReadStatus( ); );    
}

void flashWriteWord( u32 ulAddress, u16 uiWord )
{
    flashWriteByte( ulAddress, (u8)( uiWord >> 8 ) );
    flashWriteByte( ulAddress + 1, (u8) uiWord );    
}

void flashWriteLong( u32 ulAddress, u32 ulLong )
{
    u8 i;
    
    for( i = 0; i < 4; i++ ){
        flashWriteByte( ulAddress + i, (u8)( ulLong >> ( ( 3 - i ) * 8 ) ) );
    } // for
}

void flashWriteString( u32 ulAddress, u8 ucTerminate, u8 *ucBuffer )
{

    for( ; *ucBuffer; ){
        flashWriteByte( ulAddress++, *(ucBuffer++) );
    } // for
    if( ucTerminate ){
        flashWriteByte( ulAddress++, 0 ); // Terminate string with NULL if true
    } // if
}

void flashWriteBuffer( u32 ulAddress, u16 uiLen , u8 *ucBuffer )
{
    // Function returns 1 for all bytes good, 0 for failure of one or more bytes
    u32 i;
    
    for( i = 0; i < uiLen; i++ ){
        flashWriteByte( ulAddress + i, ucBuffer[ i ] );
    }
}


void flashWriteByte( u32 ulAddress, u8 ucByte )
{
    // Mask 32 bit long to make sure MSB is clear for instruction
    ulAddress |= 0x02000000;
    //spiPutCommand( &pygmyFlashSPI, PYGMY_FLASH_WREN );
    //spiPutChar( &pygmyFlashSPI, ulAddress, ucByte );
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN ); // WREN unlocks write capability for one operation
    FLASH_CS_HIGH;
    FLASH_CS_LOW;
    spiWriteLong( &pygmyFlashSPI, ulAddress ); // Write instruction and 3 address bytes
    spiWriteByte( &pygmyFlashSPI, ucByte ); // Write data byte
    FLASH_CS_HIGH;
    flashWaitForBusy(); // Busy must be clear before next write cycle may start
}

void flashWriteEnable( void )
{
    // Must be called before any write operations to FLASH
    #ifndef __PYGMY_BOOT
        spiConfig( &pygmyFlashSPI, FLASH_CS, FLASH_SCK, FLASH_MISO, FLASH_MOSI, 0 );
    #endif
    #ifdef __PYGMY_BOOT
        FLASH_CS_INIT; // To be defined in profile
        FLASH_SCK_INIT;
        FLASH_MISO_INIT;
        FLASH_MOSI_INIT; 
    #endif
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN ); // WREN must be written before status register may be modified
    FLASH_CS_HIGH;
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WRSR );
    spiWriteByte( &pygmyFlashSPI, 0 );
    FLASH_CS_HIGH;
}