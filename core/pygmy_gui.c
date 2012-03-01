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


#ifdef __PYGMYLCDSHIELD
/*#ifndef LCD_HEIGHT
    #define LCD_HEIGHT 0
#endif // LCD_HEIGHT
#ifndef LCD_WIDTH
    #define LCD_WIDTH 0
#endif // LCD_WIDTH
*/

extern const u8 PYGMY_HEXCHARS[];

PYGMYGUI globalGUI;

#ifndef __PYGMYMAXSPRITES
    #define __PYGMYMAXSPRITES 32
#endif
PYGMYSPRITE globalSprites[ __PYGMYMAXSPRITES ];
u32 globalSpriteStatus = 0;
//PYGMYTHEME globalTheme;
PYGMYWIDGET globalWidgets;
PYGMYFORM *globalForms;
u8 globalFormsStatus = 0, globalFormsLen = 0;

void guiSetStyle( u32 ulStyle )
{
    globalGUI.Style = ulStyle;
}

u32 guiGetStyle( void )
{
    return( globalGUI.Style );
}

void guiSetRadius( u8 ucRadius )
{
    globalGUI.Radius = ucRadius;
}

u8 guiGetRadius( u8 ucRadius )
{
    return( globalGUI.Radius );
}

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

void guiSetFonts( PYGMYFONT *fontSmall, PYGMYFONT *fontMedium, PYGMYFONT *fontLarge )
{
    globalGUI.Font          = fontSmall;
    globalGUI.SmallFont     = fontSmall;
    globalGUI.MediumFont    = fontMedium;
    globalGUI.LargeFont     = fontLarge;
}

u8 guiSetFont( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont )
{
	u32 ulImageStart, uiPygmyInfo;//, uiEntries;
    
    pygmyFont->Color.R         = globalGUI.Color.R;
    pygmyFont->Color.G         = globalGUI.Color.G;
    pygmyFont->Color.B         = globalGUI.Color.B;
    pygmyFont->BackColor.R     = globalGUI.BackColor.R;
    pygmyFont->BackColor.G     = globalGUI.BackColor.G;
    pygmyFont->BackColor.B     = globalGUI.BackColor.B;
    globalGUI.Font                  = pygmyFont;
    globalGUI.Font->File            = pygmyFile;
    globalGUI.Font->Color.R         = globalGUI.Color.R;
    globalGUI.Font->Color.G         = globalGUI.Color.G;
    globalGUI.Font->Color.B         = globalGUI.Color.B;
    globalGUI.Font->BackColor.R     = globalGUI.BackColor.R;
    globalGUI.Font->BackColor.G     = globalGUI.BackColor.G;
    globalGUI.Font->BackColor.B     = globalGUI.BackColor.B;
    
    ulImageStart = guiGetImage( pygmyFile, 0 );
    if( !ulImageStart ){
        return( FALSE );
    } // if
    fileSetPosition( pygmyFile, START, ulImageStart );
    uiPygmyInfo = fileGetWord( pygmyFile, BIGENDIAN );
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
        fileSetPosition( pygmyFile, CURRENT, 3 );
        pygmyFont->Height = fileGetWord( pygmyFile, BIGENDIAN );
	} else{
        fileSetPosition( pygmyFile, CURRENT, 1 );
        pygmyFont->Height |= fileGetChar( pygmyFile );
    } // else
	
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

    /*if( ucBPP == 8 ){
        ulPixelSize = 1;
    } else if( ucBPP == 16 ){
        ulPixelSize = 2;
    } else if( ucBPP == 24 ){
        ulPixelSize = 3;
    } else{
        ucBPP = 32;
        ulPixelSize = 4;
    }*/
    if( ucBPP > 32 ){
        return;
    } // if
    ulPixelSize = ucBPP / 8;
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
    u16 uiX, uiY, uiPixel, uiPadding, uiWidth, uiHeight;
    u8 ucName[ 13 ], ucR, ucG, ucB;

    uiWidth = lcdGetWidth();
    uiHeight = lcdGetHeight();
    convertIntToString( timeGet(), "%8d", ucName );
    copyString( ".bmp", ucName+8 );
    print( COM3, "\rCreating Screenshot: %s X: %d Y: %d", ucName, uiWidth, uiHeight );
    if( !fileOpen( &pygmyFile, "screenshot", WRITE ) ){
        print( COM3, "\rFile Fail!" );
        return( FALSE );
    } // if
    guiOutputBMHeader( &pygmyFile, 128, 128, 24 );
    uiPadding = uiWidth % 4;
    for( uiY = 0; uiY < uiHeight; uiY++ ){
        //print( COM3, "\r" );
        for( uiX = 0; uiX < uiWidth; uiX++ ){
            uiPixel = lcdGetPixel( uiX, ( uiHeight - 1 ) - uiY );
            //filePutWord( &pygmyFile, uiPixel, LITTLEENDIAN );
            lcdGetRGB( uiPixel, 24, &ucR, &ucG, &ucB );
            filePutChar( &pygmyFile, ucR );
            filePutChar( &pygmyFile, ucG );
            filePutChar( &pygmyFile, ucB );
            //print( COM3, "(%X %X %X)", ucR, ucG, ucB );
        } // for
        for( uiX = 0; uiX < uiPadding; uiX++ ){
            filePutChar( &pygmyFile, 0 );
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
    /*iData = fileGetChar( pygmyFile );
    iData |= (u32)fileGetChar( pygmyFile ) << 8;
    iData |= (u32)fileGetChar( pygmyFile ) << 16;
    iData |= (u32)fileGetChar( pygmyFile ) << 24;*/
    iData = fileGetLong( pygmyFile, LITTLEENDIAN );
    fileSetPosition( pygmyFile, CURRENT, 4 );
    iWidth = fileGetLong( pygmyFile, LITTLEENDIAN );
    /*iWidth = fileGetChar( pygmyFile );
    iWidth |= (u32)fileGetChar( pygmyFile ) << 8;
    iWidth |= (u32)fileGetChar( pygmyFile ) << 16;
    iWidth |= (u32)fileGetChar( pygmyFile ) << 24;*/
    iHeight = fileGetLong( pygmyFile, LITTLEENDIAN );
    /*iHeight = fileGetChar( pygmyFile );
    iHeight |= (u32)fileGetChar( pygmyFile ) << 8;
    iHeight |= (u32)fileGetChar( pygmyFile ) << 16;
    iHeight |= (u32)fileGetChar( pygmyFile ) << 24;*/
    
    fileSetPosition( pygmyFile, CURRENT, 2); // Skip 2 bytes for Planes used
    iBPP = fileGetWord( pygmyFile, LITTLEENDIAN );
    //iBPP = fileGetChar( pygmyFile );
    //iBPP |= (u32)fileGetChar( pygmyFile ) << 8;
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
        ulPixelIndex = fileGetLong( pygmyFile, BIGENDIAN );
        /*ulPixelIndex =  (u32)fileGetChar( pygmyFile ) << 24;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile ) << 16;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile ) << 8;
        ulPixelIndex |= (u32)fileGetChar( pygmyFile );*/
	} else{ 										// Table Entries are 16bit
        fileSetPosition( pygmyFile, START, 4 + (uiIndex * 2 ) );
        ulPixelIndex = fileGetWord( pygmyFile, BIGENDIAN );
        //ulPixelIndex =  (u32)fileGetChar( pygmyFile ) << 8;
        //ulPixelIndex |= (u32)fileGetChar( pygmyFile );
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

u16 drawVector( PYGMYFILE *pygmyFile, u16 uiX, u16 uiY, s8 cScale, u16 uiIndex )
{
    u32 i, ii, ulIndex;
    u16 uiPygmyInfo;
    u8 ucBPP, ucPacket, ucByte1, ucPoints, ucR, ucG, ucB, ucBuffer[ 64 ];
    
    ulIndex = guiGetImage( pygmyFile, uiIndex );
    fileSetPosition( pygmyFile, START, ulIndex );
    uiPygmyInfo = fileGetWord( pygmyFile, BIGENDIAN );
    ucBPP = ( uiPygmyInfo & 0x000F ); 
    lcdSetBPP( ucBPP ); // Must be set BEFORE setting colors
    for( ; !fileEOF( pygmyFile ); ){
        ucPoints = fileGetChar( pygmyFile );
        ucPacket = ucPoints & PYGMY_VECTOR_MASK;
        if( ucPacket == PYGMY_VECTOR_COLOR ){
            if( ucBPP == PYGMY_PBM_1BPP ){
                ucR = fileGetChar( pygmyFile );
                ucG = ucR;
                ucB = ucR;
            } else if( ucBPP == PYGMY_PBM_4BPP ){
                ucR = fileGetChar( pygmyFile );
            } else if( ucBPP == PYGMY_PBM_8BPP ){
                ucByte1 = fileGetChar( pygmyFile );
                ucR = ucByte1 >> 5;
                ucG = ( ucByte1 & 0x18 ) >> 3;
                ucB = (ucByte1 & 0x07 );
            } else if( ucBPP == PYGMY_PBM_12BPP ){
                
            } else if( ucBPP == PYGMY_PBM_16BPP ){
            
            } else if( ucBPP == PYGMY_PBM_24BPP ){
            
            } // else if
        } else if( ucPacket == PYGMY_VECTOR_POLY ){
        
        } else if( ucPacket == PYGMY_VECTOR_ARC ){
        
        } else if( ucPacket == PYGMY_VECTOR_SPLINE ){
        
        } else if( ucPacket == PYGMY_VECTOR_RASTER ){
        
        } else{
            break;
        }
    } // for
}

u16 drawImage( u16 uiXPos, u16 uiYPos, PYGMYFILE *pygmyFile, u16 uiIndex )
{
	u32 i, ii, ulPixelIndex, ulLen;
	u16 uiCount, uiPygmyInfo, uiX = 0, uiY = 0, uiWidth = 0, uiHeight = 0;
	u8 ucPacket, ucBPP, ucByte1, ucByte2, ucByte3, ucR, ucG, ucB;

    ulPixelIndex = guiGetImage( pygmyFile, uiIndex );
    fileSetPosition( pygmyFile, START, ulPixelIndex );
    // Warning! ulPixelIndex is reused below
  
    uiPygmyInfo = fileGetWord( pygmyFile, BIGENDIAN );
	//uiPygmyInfo = (u16) fileGetChar( pygmyFile ) * 0x0100;
	//uiPygmyInfo |= fileGetChar( pygmyFile );
	
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
		//uiWidth = fileGetChar( pygmyFile ) * 0x0100;
        uiWidth = fileGetWord( pygmyFile, BIGENDIAN );
        uiHeight = fileGetWord( pygmyFile, BIGENDIAN );
	} else{
        uiWidth = fileGetChar( pygmyFile );
        uiHeight = fileGetChar( pygmyFile );
    } // else
	//uiWidth |= fileGetChar( pygmyFile );
	//if( uiPygmyInfo & PYGMY_PBM_16BITD ){		// Determine 8 or 16 bit Width and Height fields
	//	uiHeight = fileGetChar( pygmyFile ) * 0x0100;
	//} // if
	//uiHeight |= fileGetChar( pygmyFile );
    
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
            } else if( !( uiPygmyInfo & PYGMY_PBM_ALPHA  ) ){
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

void drawJPEG( PYGMYFILE *pygmyFile, u16 uiX, u16 uiY )
{
    u16 uiWidth, uiHeight, uiThumbWidth, uiThumbHeight;
    u16 uiVersion, uiUnits, uiSOI, uiAPP0, uiAPP0Len;
    u8 ucID[ 5 ];
  /*   BYTE SOI[2];          // 00h  Start of Image Marker  
  BYTE APP0[2];         // 02h  Application Use Marker  
  BYTE Length[2];       // 04h  Length of APP0 Field     
  BYTE Identifier[5];   // 06h  "JFIF" (zero terminated) Id String 
  BYTE Version[2];      // 07h  JFIF Format Revision      
  BYTE Units;           // 09h  Units used for Resolution 
  BYTE Xdensity[2];     // 0Ah  Horizontal Resolution     
  BYTE Ydensity[2];     // 0Ch  Vertical Resolution       
  BYTE XThumbnail;      // 0Eh  Horizontal Pixel Count    
  BYTE YThumbnail;      // 0Fh  Vertical Pixel Count      */
    uiSOI = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiAPP0 = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiAPP0Len = fileGetWord( pygmyFile, LITTLEENDIAN );
    fileGetBuffer( pygmyFile, 5, ucID );
    uiVersion = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiUnits = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiWidth = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiHeight = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiThumbWidth = fileGetWord( pygmyFile, LITTLEENDIAN );
    uiThumbHeight = fileGetWord( pygmyFile, LITTLEENDIAN );
    print( COM3, "\rSOI: 0x%X APP0: 0x%X APP0Len: 0x%X", uiSOI, uiAPP0, uiAPP0Len );
    print( COM3, "\rID: %s Version: %d", ucID, uiVersion );
    print( COM3, "\rWidth: %d Height: %d", uiWidth, uiHeight );
    print( COM3, "\rThumbWidth: %d ThumbHeight: %d", uiThumbWidth, uiThumbHeight );
}

void drawPNG( PYGMYFILE *pygmyFile, u16 uiX, u16 uiY )
{
    u32 ulWidth, ulHeight, ulLen, ulCRC;
    u8 ucBPP, ucColorType, ucCompression, ucFilter, ucInterlace;
    u8 ucHeader[ 12 ], ucChunkType[ 5 ];
    
    // First load PNG Header
    fileGetBuffer( pygmyFile, 8, ucHeader );
    ucHeader[ 7 ] = '\0';
    // Load each chunk and process supported types
    while( !fileIsEOF( pygmyFile ) ){
        ulLen = fileGetLong( pygmyFile, BIGENDIAN );
        fileGetBuffer( pygmyFile, 4, ucChunkType );
        ucChunkType[ 4 ] = '\0';
        if( isStringSame( "IHDR", ucChunkType ) ){
            ulWidth = fileGetLong( pygmyFile, BIGENDIAN );
            ulHeight = fileGetLong( pygmyFile, BIGENDIAN );
            ucBPP = fileGetChar( pygmyFile );
            ucColorType = fileGetChar( pygmyFile );
            ucCompression = fileGetChar( pygmyFile );
            ucFilter = fileGetChar( pygmyFile );
            ucInterlace = fileGetChar( pygmyFile );
            print( COM3, "\rChunk: %s, Len: %d", ucChunkType, ulLen );
            print( COM3, "\rWidth: %d Height: %d", ulWidth, ulHeight );
            print( COM3, "\rBPP: %d", ucBPP );
            print( COM3, "\rColor: %d", ucColorType );
            print( COM3, "\rCompression: %d", ucCompression );
            print( COM3, "\rFilter: %d", ucFilter );
            print( COM3, "\rInterlace: %d", ucInterlace );
        } else if( isStringSame( "PLTE", ucChunkType ) ){
            print( COM3, "\rPalette: %d", ulLen );
            fileSetPosition( pygmyFile, CURRENT, ulLen );
            
        } else if( isStringSame( "IDAT", ucChunkType ) ){
            // Image data chunk, there may be multiple
            print( COM3, "\rData: %d", ulLen );
            fileSetPosition( pygmyFile, CURRENT, ulLen );
        } else if( isStringSame( "IEND", ucChunkType ) ){
            // End of image reached, 
            print( COM3, "\rEnd" );
            return;
        } else{
            // If chunk is ancilliary it's not supported, skip
            fileSetPosition( pygmyFile, CURRENT, ulLen );
        } // else if
        // Get the CRC for the last chunk read to keep data aligned
        ulCRC = fileGetLong( pygmyFile, BIGENDIAN );
    } // while
}



u8 widgetAdd( u8 ucType, u16 uiX, u16 uiY, u16 uiWidth, u16 uiHeight, u32 ulValue, u8 *ucBuffer )
{
    
}

u8 widgetDelete( )
{

}

u8 formNew( u16 uiX, u16 uiY, u16 uiWidth, u16 uiHeight )
{
    PYGMYFORM *pygmyForms;
    PYGMYWIDGET *newWidget;

    if( globalFormsStatus ){
        pygmyForms = sysReallocate( globalForms, sizeof( PYGMYFORM ) * globalFormsLen ); 
    } else{
        pygmyForms = sysAllocate( sizeof( PYGMYFORM ) );
    } // else
    if( !pygmyForms ){
        return( 0 );
    } // if
    globalForms = pygmyForms;
    globalForms[ globalFormsLen ].Len = 0;
    newWidget = sysAllocate( sizeof( PYGMYWIDGET ) );
    if( !newWidget ){
        //print( COM3, "\rMemory Full!" );
        return( FALSE );
    } // if
    globalForms[ globalFormsLen ].Widgets = newWidget;
    globalForms[ globalFormsLen ].AlphaColor.R = globalGUI.AlphaColor.R;
    globalForms[ globalFormsLen ].AlphaColor.G = globalGUI.AlphaColor.G;
    globalForms[ globalFormsLen ].AlphaColor.B = globalGUI.AlphaColor.B;
    globalForms[ globalFormsLen ].BackColor.R = globalGUI.BackColor.R;
    globalForms[ globalFormsLen ].BackColor.G = globalGUI.BackColor.G;
    globalForms[ globalFormsLen ].BackColor.B = globalGUI.BackColor.B;
    globalForms[ globalFormsLen ].Color.R = globalGUI.Color.R;
    globalForms[ globalFormsLen ].Color.G = globalGUI.Color.G;
    globalForms[ globalFormsLen ].Color.B = globalGUI.Color.B;
    ++globalFormsLen;
    globalFormsStatus = 1;
    
    return( 1 );
}

void formRemove( void )
{
    PYGMYFORM *pygmyForms;

    if( globalFormsLen ){
        pygmyForms = sysReallocate( globalForms, sizeof( PYGMYFORM ) * ( globalFormsLen - 1 ) );
    } // if
    if( pygmyForms ){
        globalForms = pygmyForms;
        --globalFormsLen;
    } // if
}

void formDrawAll( void )
{
    
}

u8 formAddWidget( PYGMYWIDGET *pygmyWidget )
{
    PYGMYWIDGET *newWidget;
    u32 ulLen;
    u16 uiIndex = globalFormsLen - 1;
    
    ulLen = sizeof( PYGMYWIDGET ) * ( globalFormsLen + 1 );
    newWidget = sysReallocate( globalForms[ uiIndex ].Widgets, ulLen );
    if( !newWidget ){
        print( COM3, "\rMemory Full!" );
        return( FALSE );
    } // if
    globalForms[ uiIndex ].Widgets = newWidget;
    
    
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Value    = pygmyWidget->Value;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].X        = pygmyWidget->X;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Y        = pygmyWidget->Y;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Width    = pygmyWidget->Width;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Height   = pygmyWidget->Height;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Style    = pygmyWidget->Style;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Type     = pygmyWidget->Type;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].String   = pygmyWidget->String;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Action   = pygmyWidget->Action;

    ++globalForms[ uiIndex ].Len;
    print( COM3, "\rWidgets: %d", globalForms[ uiIndex ].Len );
    return( TRUE );
}

void drawForms( void )
{ 
    PYGMYCOLOR *pygmyColor;
    u16 i, ii;

    for( i = 0; i < globalFormsLen; i++ ){
        
        guiClearArea( globalForms[ i ].X, globalForms[ i ].Y, 
            globalForms[ i ].X + globalForms[ i ].Width, 
            globalForms[ i ].Y + globalForms[ i ].Height );
        for( ii = 0; ii < globalForms[ i ].Len; ii++ ){
            PYGMY_WATCHDOG_REFRESH;
            drawWidget( &globalForms[ i ].Widgets[ ii ] );
        } // for
    } // for
}

void drawWidget( PYGMYWIDGET *pygmyWidget )
{
    u32 ulStyle;
    u16 uiX, uiY, uiPos, uiPoly[ 8 ];
    u8 ucRadius;

    if( !(pygmyWidget->Style & VISIBLE ) ){
        print( COM3, "\rWidget not visible" );
        return;
    } // if
    //ulStyle = pygmyWidget->Style;
    print( COM3, "\rWidget: %s", pygmyWidget->String );
    ucRadius = globalGUI.Radius;
    if( pygmyWidget->Style & INVERT ){
        guiSetBackColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
        guiSetColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
    } else{
        guiSetColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
        guiSetBackColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
    } // else
    if( pygmyWidget->Style & BORDER ){
        guiApplyColor();
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style & ~FILLED, ucRadius );
    } // if 
    if( pygmyWidget->Type == BUTTON || pygmyWidget->Type == LABEL ){
        if( pygmyWidget->Style & FILLED ){
            guiApplyBackColor();
            drawRect( pygmyWidget->X+1, pygmyWidget->Y+1, pygmyWidget->X + ( pygmyWidget->Width - 1 ),
                pygmyWidget->Y + ( pygmyWidget->Height - 1 ), pygmyWidget->Style, ucRadius );
        } // if
        
        if( pygmyWidget->Style & CENTERED ){
            
            uiY = ( pygmyWidget->Height - globalGUI.Font->Height ) / 2;
            uiX = 2 + ( ( pygmyWidget->Width - ( ( globalGUI.Font->Height - 7 ) * len( pygmyWidget->String ) ) ) / 2 );
            print( COM3, "\ruiX: %d", uiX );  
        } // if
    } else if( pygmyWidget->Type == CHECKBOX ){
        guiApplyColor();
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        if( pygmyWidget->Value ){ // Draw Check if not zero
            drawLine( pygmyWidget->X+1, pygmyWidget->Y+(pygmyWidget->Height/2), pygmyWidget->X+(pygmyWidget->Height/2),
                pygmyWidget->Y + ( pygmyWidget->Height - 1 ), pygmyWidget->Style );
            
            drawLine( pygmyWidget->X+(pygmyWidget->Height/2), pygmyWidget->Y + ( pygmyWidget->Height - 1 ), 
                pygmyWidget->X+(pygmyWidget->Height - 1), pygmyWidget->Y - 1, pygmyWidget->Style );
        } // if
        uiX = 2 + pygmyWidget->Height;
        uiY = 2 + ( pygmyWidget->Height - globalGUI.Font->Height ) / 2;
        
        print( COM3, "\rWidget Height: %d", pygmyWidget->Height );
        print( COM3, "\rFont Height: %d", globalGUI.Font->Height );
        print( COM3, "\ruiX: %d uiY: %d", uiX, uiY );
    } else if( pygmyWidget->Type == VSCROLLBAR ){
        guiApplyColor();
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawRect( pygmyWidget->X + ( pygmyWidget->Width - pygmyWidget->Height ), pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
    } else if( pygmyWidget->Type == HSCROLLBAR ){
        guiApplyColor();
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawRect( pygmyWidget->X + ( pygmyWidget->Width - pygmyWidget->Height ), pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        // Left Arrow
        drawLine( pygmyWidget->X + 2, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->Style );
        drawLine( pygmyWidget->X + 2, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        drawLine( pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        // Right Arrow
        drawLine( pygmyWidget->X + ( pygmyWidget->Width - 4 ), pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->Style );
        drawLine( pygmyWidget->X + pygmyWidget->Width - 4, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        drawLine( pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        if( pygmyWidget->Value ){
            uiPos = pygmyWidget->Width / ( 100 / ( pygmyWidget->Value ) ); 
        } else{
            uiPos = pygmyWidget->Height;
        } // else
        drawRect( uiPos - ( pygmyWidget->Height / 2 ), pygmyWidget->Y, uiPos + ( pygmyWidget->Height / 2 ), 
            pygmyWidget->Y + pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawLine( uiPos, pygmyWidget->Y + 4, uiPos, pygmyWidget->Y + ( pygmyWidget->Height - 4 ), pygmyWidget->Style );
        drawLine( uiPos - 4, pygmyWidget->Y + 6, uiPos - 4, pygmyWidget->Y + ( pygmyWidget->Height - 6 ), pygmyWidget->Style );
        drawLine( uiPos + 4, pygmyWidget->Y + 6, uiPos + 4, pygmyWidget->Y + ( pygmyWidget->Height - 6 ), pygmyWidget->Style );
    } 
    if( pygmyWidget->Style & CAPTION ){
        if( pygmyWidget->String ){ 
            guiSetCursor( pygmyWidget->X + uiX, pygmyWidget->Y + uiY  );
            if( !(pygmyWidget->Style & INVERT) ){
                guiSetBackColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
                guiSetColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
            } else{
                guiSetColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
                guiSetBackColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
            } // else
            #ifdef __PYGMYSTREAMLCD
                print( LCD, pygmyWidget->String );
            #endif
        } else{
            guiSetCursor( pygmyWidget->X, pygmyWidget->Y );
            #ifdef __PYGMYSTREAMLCD
                print( LCD, "%d", pygmyWidget->Value );
            #endif
        } // else
    } // if
        //drawLine( pygmyWidget->X, pygmyWidget->Y, pygmyWidget
    //} // if
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
    } else if( ( ulStyle & ROUNDED ) ){ 
        drawRadius( iX1+uiRadius, iY1+uiRadius, BIT0, uiRadius, ulStyle );
        drawRadius( iX2-uiRadius, iY1+uiRadius, BIT1, uiRadius, ulStyle );
        drawRadius( iX1+uiRadius, iY2-uiRadius, BIT3, uiRadius, ulStyle );
        drawRadius( iX2-uiRadius, iY2-uiRadius, BIT2, uiRadius, ulStyle );
    } // else if
        drawLine( iX1+uiRadius, iY1, iX2-uiRadius, iY1, ulStyle ); // top
        drawLine( iX1+uiRadius, iY2, iX2-uiRadius, iY2, ulStyle ); // bottom
        drawLine( iX1, iY1+uiRadius, iX1, iY2-uiRadius, ulStyle ); // left
        drawLine( iX2, iY1+uiRadius, iX2, iY2-uiRadius, ulStyle ); // right
    
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
    /*if( ulStyle & FILLED ){
        drawLine( uiCenterX - uiRadius, uiCenterY, uiCenterX + uiRadius, uiCenterY, ulStyle );
        drawLine( uiCenterX, uiCenterY - uiRadius, uiCenterX, uiCenterY + uiRadius, ulStyle );
    } else{
        lcdDrawPixel( uiCenterX, uiCenterY + uiRadius );
        lcdDrawPixel( uiCenterX, uiCenterY - uiRadius );
        lcdDrawPixel( uiCenterX + uiRadius, uiCenterY );
        lcdDrawPixel( uiCenterX - uiRadius, uiCenterY );     
    } // else
    */
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
/*
void drawObject( PYGMYGUIOBJECT *pygmyGUIObject )
{
   
}*/

void drawPoly( u16 *uiPoints, u16 uiLen, u32 ulStyle )
{
    u16 i, uiX, uiY;

    for( i = 0; i < uiLen-2; ){
        uiX = uiPoints[ i++ ];
        uiY = uiPoints[ i++ ];
        drawLine( uiX, uiY, uiPoints[ i ], uiPoints[ i + 1 ], ulStyle );
    } // i
}

void drawThickLine( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u8 ucThickness, u32 ulStyle )
{
    s16 iXStep, iYStep, iX, iY;
    u8 i;
    
    iXStep = 0;
    iYStep = 0;
    if( iY1 == iY2 ){
        iYStep = 1;
    } else {//if( iY1 != iY2 ){
        iXStep = 1;
    } // if
    
    for( i = 0, iX = 0, iY = 0; i < ucThickness; i++ ){
        if( i % 2 ){
            drawLine( iX1-iX, iY1-iY, iX2-iX, iY2-iY, ulStyle );
            
        } else{
            drawLine( iX1+iX, iY1+iY, iX2+iX, iY2+iY, ulStyle );
            iX += iXStep;
            iY += iYStep; 
        } // else
        
    } // for
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
#endif //__PYGMYLCDSHIELD