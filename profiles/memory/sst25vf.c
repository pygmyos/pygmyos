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
#include "pygmy_type.h"
#include <stdarg.h>
#include <stdlib.h>

//const PYGMYMEMIO __attribute__ ((section ("PYGMY_FILEVECTORS"))) = { };

//#include "pygmy_profile.h"
//#include "pygmy_type.h"
#include "profiles/memory/sst25vf.h"

const PYGMYMEMIO SST25VF = {
    (void*)sst25vfGetChar,
    (void*)sst25vfPutChar,
    (void*)sst25vfGetWord,
    (void*)sst25vfPutWord,
    (void*)sst25vfGetLong,
    (void*)sst25vfPutLong,
    (void*)sst25vfGetBuffer,
    (void*)sst25vfPutBuffer,
    (void*)sst25vfOpen,
    (void*)sst25vfVOpen,
    (void*)sst25vfClose,
    (void*)sst25vfDesc,
    (void*)sst25vfStatus,
    (void*)sst25vfEraseAll,
    (void*)sst25vfEraseSector
};

u16 sst25vfGetWord( PYGMYSPIPORT *pygmySPI, u64 ulAddress )
{
    u16 uiData;

    ulAddress |=  0x0B000000;
    //uiData = spiGetWord( pygmySPI, (u32)ulAddress );
    uiData =  (u16)spiGetChar( pygmySPI, ulAddress ) << 8;
    uiData |= (u16)spiGetChar( pygmySPI, ulAddress+1);
    return( uiData );
}

u32 sst25vfGetLong( PYGMYSPIPORT *pygmySPI, u64 ulAddress )
{
    u32 ulData;

    ulAddress |= 0x0B000000;
    //ulData = spiGetLong( pygmySPI, (u32)ulAddress );
    ulData =  (u32)spiGetChar( pygmySPI, ulAddress++ ) << 24;
    ulData |= (u32)spiGetChar( pygmySPI, ulAddress++ ) << 16;
    ulData |= (u32)spiGetChar( pygmySPI, ulAddress++ ) << 8;
    ulData |= (u32)spiGetChar( pygmySPI, ulAddress );

    return( ulData );
}

u8 sst25vfGetChar( PYGMYSPIPORT *pygmySPI, u64 ulAddress )
{   
    u8 ucChar;

    ulAddress |= 0x0B000000;// 80MHz Read //0x03000000; 25MHz Read
	ucChar = spiGetChar( pygmySPI, (u32)ulAddress );
    //pygmySPI->PortCS->BSRR = pygmySPI->PinCS;

	return( ucChar );    
}

u8 sst25vfGetBuffer( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    u32 i;

    ulAddress = ulAddress | 0x0B000000;
    for( i = 0; i < uiLen; i++ ){
        ucBuffer[ i ] = spiGetChar( pygmySPI, ulAddress + i );
    } // for
    //spiGetBuffer( pygmySPI, ulAddress, ucBuffer, uiLen );
    //pygmySPI->PortCS->BSRR = pygmySPI->PinCS;;

    return( TRUE );
}

u32 sst25vfStatus( PYGMYSPIPORT *pygmySPI )
{
    u8 ucByte;
    
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteByte( pygmySPI, SST25VF_RDSR );
    ucByte = spiReadByte( pygmySPI );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;

    return( ucByte );
}

u32 sst25vfID( PYGMYSPIPORT *pygmySPI )
{
    u32 ulID;
    
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteByte( pygmySPI, SST25VF_JEDECID );
    ulID =  (u32)spiReadByte( pygmySPI ) << 16; // Manufacturer
    ulID |= (u32)spiReadByte( pygmySPI ) << 8; // Type
    ulID |= (u32)spiReadByte( pygmySPI ); // Capacity
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;

    return( ulID); 
}

u8 sst25vfEraseSector( PYGMYSPIPORT *pygmySPI, u64 ulSector )
{
    ulSector |= 0x20000000; // 4KB Sector Erase
    spiPutCommand( pygmySPI, SST25VF_WREN );
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteAddress( pygmySPI, ulSector );
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
    sst25vfWaitForBusy( pygmySPI );
    
    return( TRUE );
}

u8 sst25vfEraseAll( PYGMYSPIPORT *pygmySPI )
{
    //u16 i;
    //u8 ucStatus;

    spiPutCommand( pygmySPI, SST25VF_WREN );
    spiPutCommand( pygmySPI, SST25VF_ERASE_ALL );
    sst25vfWaitForBusy( pygmySPI );

    return( TRUE );
}

void sst25vfWaitForBusy( PYGMYSPIPORT *pygmySPI )
{
    u16 i;
    u8 ucStatus;

    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteByte( pygmySPI, SST25VF_RDSR );
    for( i = 0 ; i < 60000; i++ ){
        ucStatus = spiReadByte( pygmySPI );
        if( !(SST25VF_STATUS_BUSY & ucStatus) ){
            break;
        //if( !(SST25VF_STATUS_BUSY & sst25vfStatus( pygmySPI ) ) ){
        //    return;
        } // if
    } // for
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
}

u8 sst25vfPutWord( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u16 uiWord )
{
    //spiPutCommand( &pygmyFlashSPI, PYGMY_FLASH_WREN );// WREN unlocks write capability for one operation
    //spiPutWord( &pygmyFlashSPI, ulAddress, uiWord );
    sst25vfPutChar( pygmySPI, ulAddress++, (u8)( uiWord >> 8 ) );
    sst25vfPutChar( pygmySPI, ulAddress, (u8) uiWord ); 
}

u8 sst25vfPutLong( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u32 ulLong )
{
    //spiPutCommand( &pygmyFlashSPI, PYGMY_FLASH_WREN );// WREN unlocks write capability for one operation
    //spiPutLong( &pygmyFlashSPI, ulAddress, ulLong );
    //u8 i;
    
    //for( i = 0; i < 4; i++ ){
    //    sst25vfPutChar( ulAddress + i, (u8)( ulLong >> ( ( 3 - i ) * 8 ) ) );
    //} // for
    sst25vfPutChar( pygmySPI, ulAddress++, (u8)((u32)ulLong>>24) );
    sst25vfPutChar( pygmySPI, ulAddress++, (u8)((u32)ulLong>>16) );
    sst25vfPutChar( pygmySPI, ulAddress++, (u8)((u32)ulLong>>8) );
    sst25vfPutChar( pygmySPI, ulAddress, (u8)((u32)ulLong) );
    
    return( TRUE );
}

u8 sst25vfPutBuffer( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    u32 i;
    
    for( i = 0; i < uiLen; i++ ){
        sst25vfPutChar( pygmySPI, ulAddress + i, ucBuffer[ i ] );
    } // for
}


u8 sst25vfPutChar( PYGMYSPIPORT *pygmySPI, u64 ulAddress, u8 ucByte )
{
    // Mask 32 bit long to make sure MSB is clear for instruction
    ulAddress |= 0x02000000;
    spiPutCommand( pygmySPI, SST25VF_WREN );// WREN unlocks write capability for one operation
    spiPutChar( pygmySPI, ulAddress, ucByte );
    //FLASH_CS_LOW;
    //spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN ); // WREN unlocks write capability for one operation
    //FLASH_CS_HIGH;
    //FLASH_CS_LOW;
    //spiWriteLong( &pygmyFlashSPI, ulAddress ); // Write instruction and 3 address bytes
    //spiWriteByte( &pygmyFlashSPI, ucByte ); // Write data byte
    //FLASH_CS_HIGH;
    sst25vfWaitForBusy( pygmySPI ); // Busy must be clear before next write cycle may start
    //putstr( "\rPutChar" );
    return( TRUE );
}

u8 sst25vfOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, ... )
{
    u8 ucResult;
    va_list vaList;
    
    va_start( vaList, ulAddress );
    ucResult = sst25vfVOpen( pygmySPI, ulAddress, vaList );
    va_end( vaList );

    return( ucResult );
}

u8 sst25vfVOpen( PYGMYSPIPORT **pygmySPI, u64 ulAddress, va_list vaList )
{
    // Variadic open
    //va_list vaList;
    PYGMYSPIPORT *ptrSPI;
    u8 ucCS, ucSCK, ucMISO, ucMOSI;
    // This function MUST be passed a pointer to a pointer
    // The PYGMYMEMIO interface assumes that the Open call will allocate memory
    //    for the port needed by the interface. The pointer passed will be
    //    encapsulated by the parent data structer or interface.
    //    Must be called before any write operations to FLASH
    //va_start( vaList, ulAddress );
    ucCS = (u8)va_arg( vaList, int );
    ucSCK = (u8)va_arg( vaList, int );
    ucMISO = (u8)va_arg( vaList, int );
    ucMOSI = (u8)va_arg( vaList, int );
    va_end( vaList );
    //putstr( "\rAllocating Port" );
    print( COM3, "\rAllocating Port" );
    //ptrSPI = sysAllocate( sizeof( PYGMYSPIPORT ) );
    ptrSPI = malloc( sizeof( PYGMYSPIPORT ) );
    if( ptrSPI != NULL ){
        *pygmySPI = ptrSPI;
        spiConfig( ptrSPI, ucCS, ucSCK, ucMISO, ucMOSI, SPILONGADDRESS|SPIDUMMYONREAD );
        spiPutCommand( ptrSPI, SST25VF_WREN ); // WREN must be written before status register may be modified
        ptrSPI->PortCS->BRR = ptrSPI->PinCS;
        spiWriteByte( ptrSPI, SST25VF_WRSR );
        spiWriteByte( ptrSPI, 0 );
        ptrSPI->PortCS->BSRR = ptrSPI->PinCS;
        return( TRUE );
    } // if
    print( COM3, "\rFail" );

    return( FALSE );
}

u8 sst25vfClose( PYGMYSPIPORT **pygmySPI )
{
    //sysFree( *pygmySPI );
    free( *pygmySPI );
    return( TRUE );
}

u8 sst25vfDesc( PYGMYSPIPORT *pygmySPI, PYGMYMEMDESC *pygmyDesc )
{
    pygmySPI->PortCS->BRR = pygmySPI->PinCS;
    spiWriteByte( pygmySPI, SST25VF_JEDECID );
    pygmyDesc->Manufacturer = spiReadByte( pygmySPI ); // Manufacturer
     spiReadByte( pygmySPI ); // Type
    pygmyDesc->Type = spiReadByte( pygmySPI ); // Capacity
    pygmyDesc->SectorSize = 4096;
    if( pygmyDesc->Type == SST25VF_ID_32M ){
        pygmyDesc->Sectors = 1024;
    } else if( pygmyDesc->Type == SST25VF_ID_16M ){
        pygmyDesc->Sectors = 512;
    } else if( pygmyDesc->Type == SST25VF_ID_8M ){
        pygmyDesc->Sectors = 256;
    } else if( pygmyDesc->Type == SST25VF_ID_4M ){
        pygmyDesc->Sectors = 128;
    } // else if
    pygmyDesc->Capacity = pygmyDesc->SectorSize * pygmyDesc->Sectors;
    pygmyDesc->Attributes = READ|WRITE|MEDIAFLASH;
    
    pygmySPI->PortCS->BSRR = pygmySPI->PinCS;
    print( COM3, "\rCapacity: %d", pygmyDesc->Capacity );
    print( COM3, "\rSectors: %d", pygmyDesc->Sectors );
    print( COM3, "\rSectorSize: %d", pygmyDesc->SectorSize );
    print( COM3, "\rAttributes: %d", pygmyDesc->Attributes );
    print( COM3, "\rManufacturer: %d", pygmyDesc->Manufacturer );
    print( COM3, "\rType: %d", pygmyDesc->Type );

    return( TRUE );
}
