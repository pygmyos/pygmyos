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

u16 *globalBaseAddress;

u16 fpecGetID( void )
{   
    // This function is for use with initial run on JTAG
    // The degug registers are not available outside JTAG on some silicon revisions ( ST bugs )
    // If JTAG registers are unavailable and descriptor is un written, the return will be invalid
    // Note that under normal operation this is not a possible scenario
    u32 *ulID;

    if ( SIZEREG->Pages >= 512 ){
        globalBaseAddress = 0x08000000 + ( ( 510 ) * 2048 );
    } else{
        globalBaseAddress = 0x08000000 + ( ( SIZEREG->Pages - 2 ) * 1024 );
    } // else
    // First Check the decriptor page in internal flash
    
    ulID = (u32*)globalBaseAddress;
    if( *ulID == 0xFFFFFFFF ){
        // Well, the descriptor is empty, time to load from debug
        ulID = (u32*)0xE0042000; // Address of 32bit ID and revision code in DBG regs
    } // if    
    return( *ulID & 0x00000FFF );
}

u8 fpecProcessIHEX( u8 *ucBuffer )
{
    static u32 ulAddress, ulAddrOffset;
    static u8 ucDummy = 0;
    u16 i, uiData;
    u8 ucShortInts[64], ucLen;

    // Dummy write must be to erased area, 0x08002000 is selected as the first sector past the
    // valid bootloader range which is limited to 8KB max
    
    convertHexEncodedStringToBuffer( ucBuffer, ucShortInts );
    
    ucLen = ucShortInts[ 0 ];
    switch( ucShortInts[ 3 ] ){ // Location three in ShortInts is Record Type
        case IHEX_DATA:
            ulAddrOffset = ( (u16)ucShortInts[ 1 ] << 8 ) + ucShortInts[ 2 ];
            if( !ucDummy ){ // one dummy write to valid erased flash address required after erase
                fpecWriteWord( (u16*)( ulAddress + ulAddrOffset ), 0xFFFF );
                ucDummy = 1;
            } // if
            for( i = 0; i < ucLen; ){
                uiData =  (u16)( ucShortInts[ 4 + i ] );
                uiData |= (u16)( ucShortInts[ 4 + i + 1 ] )<<8;
                
                if( !fpecWriteWord( (u16*)(ulAddress + ulAddrOffset + i), uiData) ){
                    return( 0 ); // Flash failed
                } // if
                i+= 2;
                
            }
            break;
        case IHEX_EXTENDEDLINEARADDR: // 16 MSb of 32 bit address stored in data section
            ulAddress = (u32)(ucShortInts[ 4 ]) << 24;
            ulAddress |= (u32)(ucShortInts[ 5 ]) << 16;
            break;
        case IHEX_STARTLINEARADDR: // Full 32 byte address stored in data section
            ulAddress = (u32)ucShortInts[ 4 ] << 24;
            ulAddress |= (u32)ucShortInts[ 5 ] << 16;
            ulAddress |= (u32)ucShortInts[ 6 ] << 8;
            ulAddress |= (u32)ucShortInts[ 7 ];
            break;
        case IHEX_EOF:
            return( 0xFF );
            break;
    }

    return( 1 );
}

u8 fpecUnlock( u8 ucBank )
{
    if( ucBank ){
        FPEC->KEYR2 = FPEC_KEY1;
        FPEC->KEYR2 = FPEC_KEY2;
    } else{
        FPEC->KEYR = FPEC_KEY1;
        FPEC->KEYR = FPEC_KEY2;
    } // else
}

u8 fpecLock( void )
{
    FPEC->CR |= FPEC_CR_LOCK;
}

u8 fpecWriteLong( u16 *uiAddress, u32 ulData )
{
    u8 ucStatus;

    ucStatus = fpecWriteWord( uiAddress++, (u16)ulData );
    ucStatus &= fpecWriteWord( uiAddress, (u16)( (u32)ulData >> 16 ) );

    return( ucStatus );
}

u8 fpecWriteWord( u16 *uiAddress, u16 uiData )
{
    // A dummy write, calling this function and pointing to a valid
    // location in FLASH, is required before writing data after an
    // erase operation!!!
    
    if( (u32)uiAddress > FPEC_MAXBANK1 ){
        if( FPEC->CR2 & FPEC_CR_LOCK ){
            fpecUnlock( 1 );
        } // if
        FPEC->CR2 |= FPEC_CR_PG;
        *uiAddress = uiData;
        while( FPEC->SR2 & FPEC_SR_BSY )
            ;
    } else{
        if( FPEC->CR & FPEC_CR_LOCK ){
            fpecUnlock( 0 );
        } // if
        FPEC->CR |= FPEC_CR_PG;
        *uiAddress = uiData;
        while( FPEC->SR & FPEC_SR_BSY )
            ;
    } // else
    
    if( *uiAddress == uiData ){
        return( 1 );
    } // if
    
    return( 0 );
}

u8 fpecWriteDescriptor( u16 uiDescriptor, u32 ulValue )
{
    // Descriptor entries are 32 bits each
    u16 *uiAddress;

    uiAddress = (u16*)globalBaseAddress; 
    uiAddress += uiDescriptor * 2;
    
    if( *((u32*)uiAddress) == 0xFFFFFFFF ){ 
        //putstr( "\rDesc: " );
        //putIntUSART3( uiAddress );
        return( fpecWriteLong( uiAddress, ulValue ) );
    } // if
    
    return( 0 );
}

u8 fpecEraseDescriptor( void )
{

}

u8 fpecEraseMemory( u16 uiStart, u16 uiEnd )
{

}

u8 fpecEraseProgramMemory( void ) 
{
    u32 i;
    u16 uiLen, uiPageSize;
    //u8 ucFail;
    
    // SIZEREG changes from pages to KB at 512
    if ( SIZEREG->Pages >= 512 ){
        uiLen = ( SIZEREG->Pages / 2 ) - 2;
        uiPageSize = 2048;
        i = 4;
    } else{
        uiLen = SIZEREG->Pages - 2;
        uiPageSize = 1024;
        i = 8;
    } // else
    
    for( ; i < uiLen; i++ ){ 
        PYGMY_WATCHDOG_REFRESH;
        fpecErasePage( 0x08000000 + ( i * uiPageSize ) ); 
    } // for

    return( 1 );
}

u8 fpecErasePage( u32 ulAddress )
{
    u32 *ulData;
    u16 i, uiLen;

    ulData = (u32*)ulAddress;
    if ( SIZEREG->Pages >= 512 ){
        uiLen = 512;
    } else{
        uiLen = 256;
    } // else
    
    for( i = 0; i < uiLen; i++ ){         // Verify the 1KB Page is empty
        if( *(ulData+i) != 0xFFFFFFFF ){
            break;
        } // if
    } // for
    if( i == uiLen ){
        // Don't erase a page that is already erased
        return( 1 );
    } // if
    if( (u32)ulAddress >= 0x08080000 ){ 
        fpecUnlock( 1 );
        FPEC->CR2 |= FPEC_CR_PER;            // Page Erase
        FPEC->AR2 = ulAddress;               // Load Address
        FPEC->CR2 |= FPEC_CR_STRT;           // Start Erase
        i = 0; // At least one command must be executed before checking status to avoid F103XL silicon bug
        while( FPEC->SR2 & FPEC_SR_BSY )    // Wait until complete
            ;
        FPEC->CR2 &= ~(FPEC_CR_PER|FPEC_CR_STRT);           // Start Erase
    } else{
        fpecUnlock( 0 );
        FPEC->CR |= FPEC_CR_PER;            // Page Erase
        FPEC->AR = ulAddress;               // Load Address
        FPEC->CR |= FPEC_CR_STRT;           // Start Erase
        i = 0; // At least one command must be executed before checking status to avoid F103XL silicon bug
        while( FPEC->SR & FPEC_SR_BSY )    // Wait until complete
            ;
        FPEC->CR &= ~(FPEC_CR_PER|FPEC_CR_STRT);           // Start Erase
    } // else
    
    for( ; i < uiLen; i++ ){         // Verify the 1KB Page is empty
        if( *(ulData+i) != 0xFFFFFFFF ){
            return( 0 );
        } // if
    } // for
       
    return( 1 );
}
