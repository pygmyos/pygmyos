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
#include <math.h>

extern const u8 PYGMY_HEXCHARS[];

PYGMYGUI globalGUI;

#ifndef __PYGMYMAXSPRITES
    #define __PYGMYMAXSPRITES 32
#endif
PYGMYSPRITE globalSprites[ __PYGMYMAXSPRITES ];
u32 globalSpriteStatus = 0;

void guiSetCursor( u16 uiX, u16 uiY )
{
    globalGUI.Cursor.X = uiX;
    globalGUI.Cursor.Y = uiY;
}

u16 guiGetCursorX( void )
{
    return( globalGUI.Cursor.X );
}

u16 guiGetCursorY( void )
{
    return( globalGUI.Cursor.Y ); 
}

void *guiGetCursor( void )
{
    return( &globalGUI.Cursor );
}

void guiApplyColor( void )
{
    lcdSetColor( globalGUI.Color.R, globalGUI.Color.G, globalGUI.Color.B );
}

void guiApplyBackColor( void )
{
    lcdSetColor( globalGUI.BackColor.R, globalGUI.BackColor.G, globalGUI.BackColor.B );
}

void guiApplyFontColor( void )
{
    lcdSetColor( globalGUI.Font->Color.R, globalGUI.Font->Color.G, globalGUI.Font->Color.B );
}

void guiApplyFontBackColor( void )
{
    lcdSetColor( globalGUI.Font->BackColor.R, globalGUI.Font->BackColor.G, 
        globalGUI.Font->BackColor.B );
}

void guiSetColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalGUI.Color.R = ucR;
    globalGUI.Color.G = ucG;
    globalGUI.Color.B = ucB;
    lcdSetColor( ucR, ucG, ucB );
}

void guiSetBackColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalGUI.BackColor.R = ucR;
    globalGUI.BackColor.G = ucG;
    globalGUI.BackColor.B = ucB;
}

void guiSetAlphaColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalGUI.AlphaColor.R = ucR;
    globalGUI.AlphaColor.G = ucG;
    globalGUI.AlphaColor.B = ucB;
}

void guiSetFontColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB )
{
    pygmyFont->Color.R = ucR;
    pygmyFont->Color.G = ucG;
    pygmyFont->Color.B = ucB;
}

void guiSetFontBackColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB )
{
    pygmyFont->BackColor.R = ucR;
    pygmyFont->BackColor.G = ucG;
    pygmyFont->BackColor.B = ucB;
}

u8 guiSetFont( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont )
{
	u16 uiPygmyInfo, uiEntries;
    
    globalGUI.Font                  = pygmyFont;
    globalGUI.Font->File            = pygmyFile;
    globalGUI.Font->Color.R         = globalGUI.Color.R;
    globalGUI.Font->Color.G         = globalGUI.Color.G;
    globalGUI.Font->Color.B         = globalGUI.Color.B;
    globalGUI.Font->BackColor.R     = globalGUI.BackColor.R;
    globalGUI.Font->BackColor.G     = globalGUI.BackColor.G;
    globalGUI.Font->BackColor.B     = globalGUI.BackColor.B;
    
    uiPygmyInfo = guiGetHeader( pygmyFile );
    uiEntries = guiGetEntries( pygmyFile );
    if( uiEntries < 96 || !( uiPygmyInfo & ( PYGMY_PBM_IMAGESTRIP | PYGMY_PBM_FONT ) ) ){
		return( FALSE );
    } // if
    
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
        fileSetPosition( pygmyFile, CURRENT, 3 );
        pygmyFont->Height = fileGetChar( pygmyFile ) * 0x0100;
	} else{
        fileSetPosition( pygmyFile, CURRENT, 1 );
        pygmyFont->Height = 0;
    } // else
	
    pygmyFont->Height |= fileGetChar( pygmyFile );
  
    return( TRUE );
}

void drawChar( u8 ucChar )
{   
    globalGUI.Cursor.X += 1 + drawImage( globalGUI.Cursor.X, globalGUI.Cursor.Y, 
        globalGUI.Font->File, ucChar - 32 ); 
}

void drawString( u8 *ucBuffer )
{
    for( ; *ucBuffer; ){
        globalGUI.Cursor.X += 1 + drawImage( globalGUI.Cursor.X, globalGUI.Cursor.Y, 
            globalGUI.Font->File, *(ucBuffer++) - 32 ); 
    } // for
}

u8 putsLCD( u8 *ucBuffer )
{
    drawString( ucBuffer );

    return( TRUE );
}

void guiClearArea( u16 uiX0, u16 uiY0, u16 uiX1, u16 uiY1 )
{
    guiApplyBackColor();
    lcdClearArea( uiX0, uiY0, uiX1, uiY1 );
}

void guiClearScreen( void )
{
    guiApplyBackColor();
    lcdClear( );
}

void guiInitSprites( void )
{
    u16 i;

    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
        globalSprites[ i ].ID = 0;
    } // for
    globalSpriteStatus = BIT0;
}

void *guiGetSprite( u16 uiID )
{
    u16 i;

    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
       if( globalSprites[ i ]. ID == uiID ){
            return( &globalSprites[ i ] );
       } //if 
    } // for

    return( NULL );
}

u8 guiCreateSprite( PYGMYFILE *pygmyFile, u16 uiID, u16 *uiCoords, u16 uiLen, u32 ulTimer, u32 ulExpire ) 
{
    u16 i;

    if( !uiID ){
        return( FALSE );
    } // if
    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
        if( !globalSprites[ i ].ID || globalSprites[ i ]. ID == uiID ){
            break;
        } //if
    } // for
    if( i == __PYGMYMAXSPRITES ){
        return( FALSE );
    } // if

    fileCopyHandle( pygmyFile, &globalSprites[ i ].File );
    globalSprites[ i ].Timer        = 1;
    globalSprites[ i ].Index        = 0;
    globalSprites[ i ].Reload       = ulTimer;
    globalSprites[ i ].TimeStamp    = timeGet();
    globalSprites[ i ].Expire       = ulExpire;
    globalSprites[ i ].Coords       = uiCoords;
    globalSprites[ i ].Len          = uiLen;
    globalSprites[ i ].ID           = uiID;
    
    return( TRUE );
}

u8 guiDeleteSprite( u16 uiID )
{
    u16 i;
    
    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
        if( globalSprites[ i ].ID == uiID ){
            globalSprites[ i ].ID = 0;
            return( TRUE );
        } // if
    } // for

    return( FALSE );
}

u8 guiRemoveSpriteCoords( u16 uiID, u16 uiX, u16 uiY )
{
    u16 i, ii, uiCopy = 0;

    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
        if( globalSprites[ i ].ID == uiID ){
            for( ii = 0; ii < globalSprites[ i ].Len; ){
                if( globalSprites[ i ].Coords[ ii ] == uiX && globalSprites[ i ].Coords[ ii + 1 ] == uiY ){
                    uiCopy = 1;
                } // if
                if( uiCopy ){
                    globalSprites[ i ].Coords[ ii ] = globalSprites[ i ].Coords[ ii + 2 ];
                    globalSprites[ i ].Coords[ ii + 1 ] = globalSprites[ i ].Coords[ ii + 3 ];
                } // if
                ii += 2;
            } // for
            globalSprites[ i ].Len -= 2;
        
            return( TRUE );
        } // if
    } // for

    return( FALSE );
}

void guiSpriteProcess( void )
{
    u32 ulTime;
    u16 i;

    ulTime = timeGet();
    for( i = 0; i < __PYGMYMAXSPRITES; i++ ){
        if( globalSprites[ i ].ID != PYGMY_BLANKID ){
            if( globalSprites[ i ].Expire ){
                if( globalSprites[ i ].Expire == ulTime ){
                    guiDeleteSprite( globalSprites[ i ].ID );
                    continue; // Do Not finish processing task after it has ended
                } // if
            } // if
            if( globalSprites[ i ].Timer != 0  ){            // Zero means no timer function
                if( globalSprites[ i ].Timer == 1 ){         // One is the reload point
                    globalSprites[ i ].Timer = globalSprites[ i ].Reload;
                    //if( globalSprites[i].Busy == 0 ){
                    //    globalSprites[ i ].Busy = 1;         // Lock thread until execution is complete
                        drawSprite( &globalSprites[ i ] );   // EventHandler is called on every reload point
                    //    globalSprites[ i ].Busy = 0;         // Unlock Thread
                    //} // if
                } else {
                    --globalSprites[ i ].Timer;              // Else countdown the timer
                } // else  
            } // else
        } // if
    } // for i
}

void guiOutputBMHeader( PYGMYFILE *pygmyFile, u32 ulWidth, u32 ulHeight, u8 ucBPP )
{
    u32 ulBMDataSize, ulFileSize, ulPixelSize;

    if( ucBPP & PYGMY_PBM_8BPP ){
        ucBPP = 8;
        ulPixelSize = 1;
    } else if( ucBPP & PYGMY_PBM_16BPP ){
        ucBPP = 16;
        ulPixelSize = 2;
    } else if( ucBPP & PYGMY_PBM_24BPP ){
        ucBPP = 24;
        ulPixelSize = 3;
    } else{
        ucBPP = 32;
        ulPixelSize = 4;
    }
    ulBMDataSize = ( ( ulWidth + ( ulWidth % 4 ) ) * ulHeight ) * ulPixelSize;
    ulFileSize = 54 + ulBMDataSize;
    
    //WORD   FileType;     // File type, always 4D42h ("BM") */
	//DWORD  FileSize;     // Size of the file in bytes */
	//WORD   Reserved1;    // Always 0 */
	//WORD   Reserved2;    // Always 0 */
	//DWORD  BitmapOffset; // Starting position of image data in bytes */
    //DWORD Size;             
	//LONG  Width;           
	//LONG  Height;          
	//WORD  Planes;          
	//WORD  BitsPerPixel;    
	//DWORD Compression;      
	//DWORD SizeOfBitmap;     
	//LONG  HorzResolution;  
	//LONG  VertResolution;   
	//DWORD ColorsUsed;      
	//DWORD ColorsImportant; 
    filePutChar( pygmyFile, 'B' );
    filePutChar( pygmyFile, 'M' );
    filePutLong( pygmyFile, ulFileSize, LITTLEENDIAN );
    filePutLong( pygmyFile, 0, LITTLEENDIAN );
    filePutLong( pygmyFile, 54, LITTLEENDIAN );
    filePutLong( pygmyFile, 40, LITTLEENDIAN ); // Size of this header in bytes
    filePutLong( pygmyFile, ulWidth, LITTLEENDIAN ); // Image width in pixels 
    filePutLong( pygmyFile, ulHeight, LITTLEENDIAN ); // Image height in pixels
    filePutWord( pygmyFile, 1, LITTLEENDIAN ); // Number of color planes 
    filePutWord( pygmyFile, ucBPP, LITTLEENDIAN ); // Number of bits per pixel 
    filePutLong( pygmyFile, 0, LITTLEENDIAN ); // Compression methods used
    filePutLong( pygmyFile, ulBMDataSize, LITTLEENDIAN ); // Size of bitmap in bytes
    filePutLong( pygmyFile, 0, LITTLEENDIAN ); // Vertical Resolution
    filePutLong( pygmyFile, 0, LITTLEENDIAN ); // Horizontal Resolution
    filePutLong( pygmyFile, 0, LITTLEENDIAN ); // Number of Colors ( 0 = MAX )
    filePutLong( pygmyFile, 0, LITTLEENDIAN ); // Number of Important Colors ( 0 = ALL )
    
    
    // Ready to Save BMP Data
}

u8 guiSaveScreen( void )
{
    PYGMYFILE pygmyFile;
    u16 uiX, uiY, uiPixel, uiPadding;
    u8 ucName[ 13 ], ucR, ucG, ucB;

    convertIntToString( timeGet(), "%8d", ucName );
    stringCopy( ".bmp", ucName+8 );
    print( COM3, "\rCreating Screenshot %s", ucName );
    if( !fileOpen( &pygmyFile, ucName, WRITE ) ){
        print( COM3, "\rFile Fail!" );
        return( FALSE );
    } // if
    guiOutputBMHeader( &pygmyFile, 128, 128, PYGMY_PBM_24BPP );
    for( uiY = 0; uiY < LCD_HEIGHT; uiY++ ){
        for( uiX = 0; uiX < LCD_WIDTH; uiX++ ){
            uiPixel = lcdGetPixel( uiX, LCD_HEIGHT - uiY );
            lcdGetRGB( uiPixel, &ucR, &ucG, &ucB );
            fileWriteChar( &pygmyFile, ucR );
            fileWriteChar( &pygmyFile, ucG );
            fileWriteChar( &pygmyFile, ucB );
        } // for
        for( uiX = 0; uiX < uiPadding; uiX++ ){
            fileWriteChar( &pygmyFile, 0 );
        } // for
    } // for
    fileClose( &pygmyFile );

    return( TRUE );
}

void drawBitmap( u16 uiX, u16 uiY, PYGMYFILE *pygmyFile )
{
    u16 i, ii, iPadding;
    u32 iWidth, iHeight, iBPP, iRLE, iData;
    u8 ucR, ucG, ucB;

    if( !(fileGetChar( pygmyFile ) == 'B' && fileGetChar( pygmyFile ) == 'M') ){
        return;
    } // else    
    
    fileSetPosition( pygmyFile, CURRENT, 8);
    iData = fileGetChar( pygmyFile );
    iData |= (u32)fileGetChar( pygmyFile ) << 8;
    iData |= (u32)fileGetChar( pygmyFile ) << 16;
    iData |= (u32)fileGetChar( pygmyFile ) << 24;
    fileSetPosition( pygmyFile, CURRENT, 4 );
    iWidth = fileGetChar( pygmyFile );
    iWidth |= (u32)fileGetChar( pygmyFile ) << 8;
    iWidth |= (u32)fileGetChar( pygmyFile ) << 16;
    iWidth |= (u32)fileGetChar( pygmyFile ) << 24;
    iHeight = fileGetChar( pygmyFile );
    iHeight |= (u32)fileGetChar( pygmyFile ) << 8;
    iHeight |= (u32)fileGetChar( pygmyFile ) << 16;
    iHeight |= (u32)fileGetChar( pygmyFile ) << 24;
    fileGetChar( pygmyFile ); // Skip 2 bytes for Planes used
    fileGetChar( pygmyFile ); //
    iBPP = fileGetChar( pygmyFile );
    iBPP |= (u32)fileGetChar( pygmyFile ) << 8;
    iRLE = fileGetChar( pygmyFile ); // Ignore the next three bytes

    fileSetPosition( pygmyFile, START, iData );
    iPadding = iWidth % 4;
   
    for( i = 0; i < iHeight; i++ ){
        for( ii = 0; ii < iWidth; ii++ ){
            ucR = fileGetChar( pygmyFile );
            ucG = fileGetChar( pygmyFile );
            ucB = fileGetChar( pygmyFile );
            lcdSetColor( ucR, ucG, ucB );
            lcdDrawPixel( uiX+ii, uiY + ( ( ( iHeight - 1 ) - i ) * iWidth ) );
        } // for  
        for( ii = 0; ii < iPadding; ii++ ){
            fileGetChar( pygmyFile );
        } // for
    } // for
    fileSetPosition( pygmyFile, START, 0 ); 
}

u16 guiGetEntries( PYGMYFILE *pygmyFile )
{
	u16 uiEntries, uiPygmyInfo;
    
    uiPygmyInfo = guiGetHeader( pygmyFile );
	
	if( !( uiPygmyInfo & ( PYGMY_PBM_IMAGESTRIP | PYGMY_PBM_FONT ) ) ){
		return( 0 );
    } // if
	// uiGetHeader leaves the file at the frame count
	uiEntries = (u16)  fileGetChar( pygmyFile ) * 0x0100 ;
	uiEntries |= (u16) fileGetChar( pygmyFile );
    
    return( uiEntries );
}

u16 guiGetHeader( PYGMYFILE *pygmyFile )
{
    u16  uiPygmyInfo;

    fileSetPosition( pygmyFile, START, 0 ); 
	uiPygmyInfo = (u16)  fileGetChar( pygmyFile ) * 0x0100;
	uiPygmyInfo |= (u16) fileGetChar( pygmyFile );
    
    return( uiPygmyInfo );
}

u32 guiGetImage( PYGMYFILE *pygmyFile, u16 uiIndex )
{
    // This function seeks for image in imagestrip or font based on provided index
	u32 ulPixelIndex;
	u16 uiEntries, uiPygmyInfo;

    uiPygmyInfo = guiGetHeader( pygmyFile );
    uiEntries = guiGetEntries( pygmyFile );
    if( !( uiPygmyInfo & ( PYGMY_PBM_IMAGESTRIP | PYGMY_PBM_FONT ) ) ){
		return( 0 );
    } // if
	if( !uiEntries || uiIndex > uiEntries) {
		return( 0 ); // index past entries
    } // if
	if( uiPygmyInfo & PYGMY_PBM_TABLE32 ){				// Table Entries are 32bit
        fileSetPosition( pygmyFile, CURRENT, uiIndex * 4 );
        ulPixelIndex =  (u32)fileGetChar( pygmyFile ) << 24;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile ) << 16;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile ) << 8;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile );
	} else{ 										// Table Entries are 16bit
        fileSetPosition( pygmyFile, START, 4 + (uiIndex * 2 ) );
        ulPixelIndex =  (u32)fileGetChar( pygmyFile ) << 8;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile );
	} // else
		
    // The index returned is the absolute offset from the start of file
	return( ulPixelIndex );	
}

u16 drawSprite( PYGMYSPRITE *pygmySprite )
{
    // u16 *Coords;
    // u16 Len;
    // u16 ID;
    // u16 Index;
    // PYGMYFILE File;
    u16 i, uiEntries, uiWidth;

    uiEntries = guiGetEntries( &pygmySprite->File );
    if( ++pygmySprite->Index == uiEntries ){
        pygmySprite->Index = 0;
    } // if
    for( i = 0; i < pygmySprite->Len ; ){
        uiWidth = drawImage( pygmySprite->Coords[ i++ ], pygmySprite->Coords[ i++ ], &pygmySprite->File, pygmySprite->Index );
    } // for
    
    return( uiWidth );
}

u16 drawImage( u16 uiXPos, u16 uiYPos, PYGMYFILE *pygmyFile, u16 uiIndex )
{
	u32 i, ii, ulPixelIndex, ulLen;
	u16 uiCount, uiPygmyInfo, uiX = 0, uiY = 0, uiWidth = 0, uiHeight = 0;
	u8 ucPacket, ucBPP, ucByte1, ucByte2, ucByte3, ucR, ucG, ucB;

    ulPixelIndex = guiGetImage( pygmyFile, uiIndex );
    fileSetPosition( pygmyFile, START, ulPixelIndex );
    // Warning! ulPixelIndex is reused below
  
	uiPygmyInfo = (u16) fileGetChar( pygmyFile ) * 0x0100;
	uiPygmyInfo |= fileGetChar( pygmyFile );
	
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
		uiWidth = fileGetChar( pygmyFile ) * 0x0100;
	} // if
	uiWidth |= fileGetChar( pygmyFile );
	if( uiPygmyInfo & PYGMY_PBM_16BITD ){		// Determine 8 or 16 bit Width and Height fields
		uiHeight = fileGetChar( pygmyFile ) * 0x0100;
	} // if
	uiHeight |= fileGetChar( pygmyFile );
    
	ulLen = (u32) (uiWidth * uiHeight);
    ucBPP = ( uiPygmyInfo & 0x000F ); 
    lcdSetBPP( ucBPP ); // Must be set BEFORE setting colors
    ucR = globalGUI.Color.R;
    ucG = globalGUI.Color.G;
    ucB = globalGUI.Color.B;
	for( ulPixelIndex = 0; ulPixelIndex < ulLen; ){
		ucPacket = fileGetChar( pygmyFile );
        uiCount = ( ucPacket & 0x3F ) + 1;
        if( ucBPP == PYGMY_PBM_1BPP && !(ucPacket & BIT7) ){
            uiCount = 7;
        } // if
        
        ulPixelIndex += uiCount;
            
        for( i = 0, ii = 0; i < uiCount; i++  ){ 
            if( !i || !(ucPacket & BIT7) ){
                if( ucBPP == PYGMY_PBM_1BPP ){
                    if( !(ucPacket & BIT7) ){
                        if( (ucPacket & ( BIT6 >> i ) ) ){
                            ucPacket |= BIT6;
                        } else{
                            ucPacket &= ~BIT6;
                        } // else
                        
                    } // if
                } else if( ucBPP == PYGMY_PBM_4BPP ){
                    if( !ii ){
                        ucByte1 = fileGetChar( pygmyFile );
                        ucR = ucByte1 >> 4;
                        ii = 1;
                    } else{
                        ucR = ucByte1 & 0x0F;
                        ii = 0;
                    } // else
                    ucG = ucR;
                    ucB = ucR;
                } else if( ucBPP == PYGMY_PBM_12BPP ){
                    if( !ii ){
                        ucByte1 = fileGetChar( pygmyFile );
                        ucByte2 = fileGetChar( pygmyFile );
                        ucR = ucByte1;
                        ucG = ucByte2 >> 4;
                        ucB = ucByte2 & 0x0F;
                    } else{
                        ucByte3 = fileGetChar( pygmyFile );
                        ucR = ucByte2 & 0x0F;
                        ucG = ucByte3 >> 4;
                        ucB = ucByte3 & 0x0F;
                    } // else
                } else if( ucBPP == PYGMY_PBM_8BPP ){
                    ucByte1 = fileGetChar( pygmyFile );
                    ucR = ucByte1 >> 5;
                    ucG = ( ucByte1 & 0x18 ) >> 3;
                    ucB = (ucByte1 & 0x07 );
                } else if( ucBPP == PYGMY_PBM_16BPP ){
                    ucByte1 = fileGetChar( pygmyFile );
                    ucByte2 = fileGetChar( pygmyFile );
                    ucR = ucByte1 >> 3;
                    ucG = (( ucByte1 & 0x07 ) << 3 ) | (ucByte2 >> 5);
                    ucB = ucByte2 & 0x1F;
                } else if( ucBPP == PYGMY_PBM_24BPP ){
                    ucR = fileGetChar( pygmyFile );
                    ucG = fileGetChar( pygmyFile );
                    ucB = fileGetChar( pygmyFile );
                } // else if
                //if( ucBPP != PYGMY_PBM_1BPP ){
                //    lcdSetColor( ucR, ucG, ucB );
                //} // if            
            } // if
            if( ucPacket & BIT6 ){
                if( ( uiPygmyInfo & PYGMY_PBM_FONT ) && ( uiPygmyInfo & PYGMY_PBM_1BPP ) ){
                    guiApplyFontColor(  );
                } else if( ucBPP != PYGMY_PBM_1BPP ){
                    lcdSetColor( ucR, ucG, ucB );
                } else{
                    guiApplyColor( );
                } // else
                lcdDrawPixel( uiXPos+uiX, uiYPos+uiY );
            } else {// if( !( uiPygmyInfo & PYGMY_PBM_ALPHA  ) ){
                if( ( uiPygmyInfo & PYGMY_PBM_FONT ) && ( uiPygmyInfo & PYGMY_PBM_1BPP ) ){
                    guiApplyFontBackColor(  );
                } else{
                    guiApplyBackColor( );
                } // else
                lcdDrawPixel( uiXPos+uiX, uiYPos+uiY );
            } // else
            if( ++uiX == uiWidth  ){
                uiX = 0;
                ++uiY;
            } // if
        } // for
	} // for
	fileSetPosition( pygmyFile, START, 0 );

	return( uiWidth );
}

void drawPixel( u16 uiX, u16 uiY )
{
    guiApplyColor();
    lcdDrawPixel( uiX, uiY );
}

void drawClearPixel( u16 uiX, u16 uiY )
{
    guiApplyBackColor();
    lcdDrawPixel( uiX, uiY);
}


void drawWidget( PYGMYWIDGET *pygmyWidget )
{
    PYGMYFONT *pygmyFont;
    u16 uiX, uiY;

    if( !( pygmyWidget->Style & VISIBLE ) ){
        return;
    } // if
    guiSetColor( pygmyWidget->Color.B, pygmyWidget->Color.G, pygmyWidget->Color.R );
    guiSetBackColor( pygmyWidget->BackColor.B, pygmyWidget->BackColor.G, pygmyWidget->BackColor.R );
    //lcdSetColor( pygmyWidget->Color.B, pygmyWidget->Color.G, pygmyWidget->Color.R );
    //lcdSetBackColor( pygmyWidget->BackColor.B, pygmyWidget->BackColor.G, pygmyWidget->BackColor.R );
    if( pygmyWidget->Style & BORDER ){
        
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, 8 );
        
        if( pygmyWidget->String[0] ){
            if( pygmyWidget->Style & CENTERED ){
                uiX = ( ( pygmyWidget->Width - ( pygmyWidget->Font->Height * len( pygmyWidget->String ) ) ) / 2 );
                if( uiX > pygmyWidget->Width ){
                    uiX = 8;
                } // if
                uiY = ( pygmyWidget->Height - pygmyWidget->Font->Height ) / 2;
                if( uiY > pygmyWidget->Height ){
                    uiY = 8;
                } // if
            } // if
            guiSetCursor( pygmyWidget->X + uiX, pygmyWidget->Y + uiY  );
            #ifdef __PYGMYSTREAMLCD
                print( LCD, pygmyWidget->String );
            #endif
        } else{
            guiSetCursor( pygmyWidget->X, pygmyWidget->Y );
            #ifdef __PYGMYSTREAMLCD
                print( LCD, "%d", pygmyWidget->Value );
            #endif
        } // else
        //drawLine( pygmyWidget->X, pygmyWidget->Y, pygmyWidget
    } // if
}



void drawFill( u16 x, u16 y )
{
    if( lcdGetPixel(x,y) != 0 ){
        lcdDrawPixel(x,y);
        drawFill(x+1,y);
        drawFill(x-1,y);
        drawFill(x,y+1);
        drawFill(x,y-1);
    } // if
}


/*
void drawFill( u16 uiX, u16 uiY  )
{
    PYGMYCOLOR *pygmyColor;
    u32 ulColor;
    u16 uiRepeat, uiLastX, uiLastY, uiDirection = 1;
    u8 ucPixels, ucLastPixels;

    pygmyColor = lcdGetColor( );
    ulColor = lcdColorToData( pygmyColor ); 
    for( ; lcdGetPixel( uiX, uiY ) != ulColor && uiY; uiY-- );
    ++uiY;
    for( uiRepeat = 0; uiRepeat < 100 && uiY < 128 && uiX < 128; ){
        ucPixels = guiGetPixelOpening( uiX, uiY, ulColor );
        uiLastX = uiX;
        uiLastY = uiY;
        //print( COM3, "\rBorderColor: 0x%4X, %d, %d, 0x%2X", ulColor, uiX, uiY, ucPixels );
        if( !(ucPixels & BIT1) && ucPixels & BIT3 ){
            lcdDrawPixel( --uiX, uiY );
        } else if( ucPixels & BIT0 && ucPixels & BIT3 ){
            lcdDrawPixel( uiX, --uiY );
            continue;
        } else if( ucPixels & BIT0 ){
            lcdDrawPixel( ++uiX, uiY );
            continue;
        } else if( ucPixels & BIT2 ){
            lcdDrawPixel( uiX, ++uiY );
            continue;
        } else if( ucPixels & BIT1 ){
            lcdDrawPixel( --uiX, uiY );
            continue;
        } else if( ucPixels & BIT3 ){
            lcdDrawPixel( uiX, --uiY );
            continue;
        }  
        //ucLastPixels = ucPixels;
        uiX = uiLastX;
        uiY = uiLastY;
        ++uiRepeat;
        //ucPixels = lcdGetPixelOpening( uiX, uiY, ulColor );
    
        
       // return;
    } // for
}*/
/*
void drawFill( u16 uiX, u16 uiY, u32 ulColor )
{
    if( ulColor != lcdGetPixel( uiX, uiY ) ) { // sample pixel color

        drawFill( uiX, uiY, ulColor );
        drawFill( uiX-1, uiY, ulColor);
        drawFill( uiX+1, uiY, ulColor);
        drawFill( uiX, uiY-1, ulColor);
        drawFill( uiX, uiY+1, ulColor);
    }
}*/

u8 guiGetPixelOpening( u16 uiX, u16 uiY, u32 ulColor )
{
    // Looks for any color on any of the four side of the specified
    // location that is not the specified color
    u8 ucPixels = 0;

    if( lcdGetPixel( uiX + 1, uiY ) != ulColor ){
        ucPixels |= BIT0;
    } else{
        //print( COM3, "\rFound: 0x%4X Not: 0x%4X", lcdGetPixel( uiX + 1, uiY ), ulColor ); 
    }
    if( lcdGetPixel( uiX - 1, uiY ) != ulColor ){
        ucPixels |= BIT1;
    } else{
        //print( COM3, "\rFound: 0x%4X Not: 0x%4X", lcdGetPixel( uiX - 1, uiY ), ulColor ); 
    }
    if( lcdGetPixel( uiX, uiY + 1 ) != ulColor ){
        ucPixels |= BIT2;
    } else{
        //print( COM3, "\rFound: 0x%4X Not: 0x%4X", lcdGetPixel( uiX, uiY + 1 ), ulColor ); 
    }
    if( lcdGetPixel( uiX, uiY - 1 ) != ulColor ){
        ucPixels |= BIT3;
    } else{
        //print( COM3, "\rFound: 0x%4X Not: 0x%4X", lcdGetPixel( uiX, uiY - 1 ), ulColor ); 
    }

    return( ucPixels );
}

void drawRect( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle, u16 uiRadius )
{      
    u16 i, ii;

    guiApplyColor();

    if( ulStyle & FILLED ){
        if( uiRadius && ( ulStyle & ( ROUNDED | CHAMFER ) ) ){
            for( i = iY1+uiRadius; i < ( iY2 - uiRadius) + 1; i++ ){
                drawLine( iX1, i, iX2, i, ulStyle ); 
            } // for
            for( i = 0, ii = 0; i < uiRadius; i++ ){
                drawLine( iX1 + uiRadius - ii, iY1 + i, iX2 - uiRadius + ii, iY1 + i, ulStyle );
                drawLine( iX1 + uiRadius - ii, iY2 - i, iX2 - uiRadius + ii, iY2 - i, ulStyle );
                if( ulStyle & CHAMFER ){
                    ++ii;
                } // if
            } // for 
        } else{
            for( i = iY1; i < iY2; i++ ){
                drawLine( iX1 + 1, i, iX2 - 1, i, ulStyle );
            } // for
        } // else    
    } 
    if( ( ulStyle & CHAMFER ) ){
        drawLine( iX2-uiRadius, iY1, iX2, iY1+uiRadius, ulStyle ); // top right
        drawLine( iX2-uiRadius, iY2, iX2, iY2-uiRadius, ulStyle ); // bottom right
        drawLine( iX1+uiRadius, iY2, iX1, iY2-uiRadius, ulStyle ); // bottom left
        drawLine( iX1, iY1+uiRadius, iX1+uiRadius, iY1, ulStyle ); // top left
    } // if
    if( ( ulStyle & ROUNDED ) ){ 
        drawRadius( iX1+uiRadius, iY1+uiRadius, BIT0, uiRadius, ulStyle );
        drawRadius( iX2-uiRadius, iY1+uiRadius, BIT1, uiRadius, ulStyle );
        drawRadius( iX1+uiRadius, iY2-uiRadius, BIT3, uiRadius, ulStyle );
        drawRadius( iX2-uiRadius, iY2-uiRadius, BIT2, uiRadius, ulStyle );
    } else{
        drawLine( iX1+uiRadius, iY1, iX2-uiRadius, iY1, ulStyle ); // top
        drawLine( iX1+uiRadius, iY2, iX2-uiRadius, iY2, ulStyle ); // bottom
        drawLine( iX1, iY1+uiRadius, iX1, iY2-uiRadius, ulStyle ); // left
        drawLine( iX2, iY1+uiRadius, iX2, iY2-uiRadius, ulStyle ); // right
    } // else 
    
}

void drawPixelInArea( u16 uiX0, u16 uiY0, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{
    if( ( uiX0 >= uiX1 && uiX0 <= uiX2 ) && (  uiY0 >= uiY1 && uiY0 <= uiY2 ) ){
        lcdDrawPixel( uiX0, uiY0 ); 
    }
}

void drawRadius( u16 uiCenterX, u16 uiCenterY, u16 uiCorner, u16 uiRadius, u32 ulStyle )
{
    s16 f = 1 - uiRadius;
    s16 ddF_x = 1;
    s16 ddF_y = -2 * uiRadius;
    u16 uiX = 0;
    u16 uiY = uiRadius;
    
    guiApplyColor();
    if( ulStyle & FILLED ){
        drawLine( uiCenterX - uiRadius, uiCenterY, uiCenterX + uiRadius, uiCenterY, ulStyle );
        drawLine( uiCenterX, uiCenterY - uiRadius, uiCenterX, uiCenterY + uiRadius, ulStyle );
    } else{
        lcdDrawPixel( uiCenterX, uiCenterY + uiRadius );
        lcdDrawPixel( uiCenterX, uiCenterY - uiRadius );
        lcdDrawPixel( uiCenterX + uiRadius, uiCenterY );
        lcdDrawPixel( uiCenterX - uiRadius, uiCenterY );
        
    } // else

    while( uiX < uiY ){
        if(f >= 0){
            uiY--;
            ddF_y += 2;
            f += ddF_y;
        } // if
        uiX++;
        ddF_x += 2;
        f += ddF_x;  
        if( ulStyle & FILLED ){
            if( uiCorner & BIT0 ){
                drawLine( uiCenterX, uiCenterY-uiY, uiCenterX - uiX, uiCenterY - uiY, ulStyle ); // 7
                drawLine( uiCenterX, uiCenterY-uiX, uiCenterX - uiY, uiCenterY - uiX, ulStyle ); // 6
            } // if
            if( uiCorner & BIT1 ){
                drawLine( uiCenterX, uiCenterY-uiY, uiCenterX + uiX, uiCenterY - uiY, ulStyle ); // 0
                drawLine( uiCenterX, uiCenterY-uiX, uiCenterX + uiY, uiCenterY - uiX, ulStyle ); // 1
            }
            if( uiCorner & BIT2 ){
                drawLine( uiCenterX, uiCenterY+uiY, uiCenterX + uiX, uiCenterY + uiY, ulStyle ); // 3
                drawLine( uiCenterX, uiCenterY+uiX, uiCenterX + uiY, uiCenterY + uiX, ulStyle ); // 2
            } // if
            if( uiCorner & BIT3 ){
                drawLine( uiCenterX, uiCenterY+uiY, uiCenterX - uiX, uiCenterY + uiY, ulStyle ); // 4
                drawLine( uiCenterX, uiCenterY+uiX, uiCenterX - uiY, uiCenterY + uiX, ulStyle ); // 5
            } // if
        } else{
            if( uiCorner & BIT0 ){
                lcdDrawPixel( uiCenterX - uiX, uiCenterY - uiY ); // 7
                lcdDrawPixel( uiCenterX - uiY, uiCenterY - uiX ); // 6
            } // if
            if( uiCorner & BIT1 ){
                lcdDrawPixel( uiCenterX + uiX, uiCenterY - uiY ); // 0
                lcdDrawPixel( uiCenterX + uiY, uiCenterY - uiX ); // 1
            }
            if( uiCorner & BIT2 ){
                lcdDrawPixel( uiCenterX + uiX, uiCenterY + uiY ); // 3
                lcdDrawPixel( uiCenterX + uiY, uiCenterY + uiX ); // 2
            } // if
            if( uiCorner & BIT3 ){
                lcdDrawPixel( uiCenterX - uiX, uiCenterY + uiY ); // 4
                lcdDrawPixel( uiCenterX - uiY, uiCenterY + uiX ); // 5
            } // if
        } // else
    } // while
}

/*
void clearAll( u8 *ucBuffer, u16 uiColor )
{
    lcdClear();
	u16 i;
	
    if( uiColor ) // flatten to 1bpp
        uiColor = 0xFF;
    else
        uiColor = 0x00;
	for( i = 0; i < ( LCD_PAGES * LCD_BYTESPERPAGE ); i++ ){
        ucBuffer[ i ] = uiColor;
		//ucVideoBuffer[ i ] = 0x00;
	}
}*/


void drawAll( u8 *ucBuffer )
{
    lcdWriteScreenBuffer();
}

void drawObject( PYGMYGUIOBJECT *pygmyGUIObject )
{
   
}

void drawPoly( u16 *uiPoints, u16 uiLen, u32 ulStyle )
{
    u16 i, uiX, uiY;

    for( i = 0; i < uiLen-2; ){
        uiX = uiPoints[ i++ ];
        uiY = uiPoints[ i++ ];
        drawLine( uiX, uiY, uiPoints[ i ], uiPoints[ i + 1 ], ulStyle );
    } // i
}

void drawLine( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle )
{   
    s16 dy = iY2 - iY1;
    s16 dx = iX2 - iX1;
    s16 stepx, stepy, iStepSkip, iToggle, fraction;

    guiApplyColor();
    iStepSkip = 1;
    iToggle = 1;
    if( ulStyle & DOT ){
        iStepSkip = 2;
    } else if( ulStyle & DASH ){
        iStepSkip = 4;
    } // else if
    if( dy < 0 ){
        dy = -dy;  
        stepy = -1; 
    } else{ 
        stepy = 1; 
    } // else
    
    if( dx < 0 ){
        dx = -dx;  
        stepx = -1; 
    } else{ 
        stepx = 1; 
    } // else
    dy <<= 1;                                                  // dy is now 2*dy
    dx <<= 1;                                                  // dx is now 2*dx

    lcdDrawPixel( iX1, iY1 );
    if (dx > dy) {
        fraction = dy - (dx >> 1);                         // same as 2*dy - dx
        while( iX2 != iX1 ) {
            if(fraction >= 0) {
                iY1 += stepy;
                fraction -= dx;                                // same as fraction -= 2*dx
            } // if
            iX1 += stepx;
            fraction += dy;                                    // same as fraction -= 2*dy
            /*if( ( ulStyle & ( PYGMY_DOT | PYGMY_DASH ) ) && iX1 % iStepSkip ){
                if( iToggle ){
                    iToggle = 0;
                    --iStepSkip;
                } else {
                    iToggle = 1;
                    ++iStepSkip;
                } // else
                continue;
            }*/
            lcdDrawPixel( iX1, iY1 );
        } // while
    } else {
        fraction = dx - (dy >> 1);
        while( iY2 != iY1 ) {
            if(fraction >= 0) {
                iX1 += stepx;
                fraction -= dy;
            } // if
            iY1 += stepy;
            fraction += dx;
            /*if( ( ulStyle & ( PYGMY_DOT | PYGMY_DASH ) ) && iY1 % iStepSkip ){
                if( iToggle ){
                    iToggle = 0;
                    --iStepSkip;
                } else {
                    iToggle = 1;
                    ++iStepSkip;
                } // else
                continue;
            }*/
            lcdDrawPixel( iX1, iY1 );
        } // while
    } // else
} 

void drawCircle( u16 uiX0, u16 uiY0, u16 uiRadius, u32 ulStyle )
{
    drawRadius( uiX0, uiY0, BIT0|BIT1|BIT2|BIT3, uiRadius, ulStyle );
    
    /*s16 f = 1 - uiRadius;
    s16 ddF_x = 1;
    s16 ddF_y = -2 * uiRadius;
    u16 uiX = 0;
    u16 uiY = uiRadius;
 
    lcdDrawPixel( uiX0, uiY0 + uiRadius );
    lcdDrawPixel( uiX0, uiY0 - uiRadius );
    lcdDrawPixel( uiX0 + uiRadius, uiY0 );
    lcdDrawPixel( uiX0 - uiRadius, uiY0 );
 
    while( uiX < uiY ){
        if(f >= 0){
            uiY--;
            ddF_y += 2;
            f += ddF_y;
        } // if
        uiX++;
        ddF_x += 2;
        f += ddF_x;    
        lcdDrawPixel( uiX0 + uiX, uiY0 + uiY );
        lcdDrawPixel( uiX0 - uiX, uiY0 + uiY );
        lcdDrawPixel( uiX0 + uiX, uiY0 - uiY );
        lcdDrawPixel( uiX0 - uiX, uiY0 - uiY );
        lcdDrawPixel( uiX0 + uiY, uiY0 + uiX );
        lcdDrawPixel( uiX0 - uiY, uiY0 + uiX );
        lcdDrawPixel( uiX0 + uiY, uiY0 - uiX );
        lcdDrawPixel( uiX0 - uiY, uiY0 - uiX );
    } // while
    */
}


void drawSine( u16 uiX0, u16 uiY0, u16 uiFrequency, s8 a )
{
    /*s16 i;
    s16 y = uiY0;
    //float a = uiAmplitude;
    float piDouble = 2 * 3.14159265;
    float factor = piDouble / uiFrequency;

    for( i = 0; i < uiFrequency; i++ ){ 
		lcdDrawPixel( uiX0+i,  y + ( sin( i * factor) * a ) );
    } // for
	*/
}
/*
int getNormalizedSine(int x, int halfY, int maxX) {

      float piDouble = 2 * PI;
      float factor = piDouble / maxX;

      return (int) ( sin( x * factor) * halfY + halfY );
}
*/