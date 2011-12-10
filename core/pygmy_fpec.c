/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "pygmy_profile.h"

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

u8 fpecUnlock( void )
{
    FPEC->KEYR = FPEC_KEY1;
    FPEC->KEYR = FPEC_KEY2;
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
    // location is FLASH, is required before writing data after an
    // erase operation!!!
    if( FPEC->CR & FPEC_CR_LOCK ){
        fpecUnlock( );
    } // if
    
    FPEC->CR |= FPEC_CR_PG;
    *uiAddress = uiData;
    while( FPEC->SR & FPEC_SR_BSY )
        ;
   
    if( *uiAddress == uiData ){
        return( 1 );
    } // if
    
    return( 0 );
}

u8 fpecEraseProgramMemory( u16 uiStart, u16 uiEnd )
{
    u32 i, ucFail;
    
    fpecUnlock();
    for( i = uiStart; i < uiEnd; i++ ){
        for( ucFail = 0; !fpecErasePage( 0x08000000 + ( i * 1024 ) ) ; ucFail++ ){
            if( ucFail > 3 ) // Acceptable limit of retries before we know something is wrong
                return( 0 );
        }
    }

    return( 1 );
}

u8 fpecErasePage( u32 ulAddress )
{
    u32 *ulData;
    u16 i;

    ulData = (u32*)ulAddress;
    
    FPEC->CR |= FPEC_CR_PER;            // Page Erase
    FPEC->AR = ulAddress;               // Load Address
    FPEC->CR |= FPEC_CR_STRT;           // Start Erase
    while( FPEC->SR & FPEC_SR_BSY )    // Wait until complete
        ;
    FPEC->CR &= ~(FPEC_CR_PER|FPEC_CR_STRT);           // Start Erase
    for( i = 0; i < 256; i++ ){         // Verify the 1KB Page is empty
        if( *(ulData+i) != 0xFFFFFFFF ){
            return( 0 );
        } // if
    } // for
    
    return( 1 );
}
