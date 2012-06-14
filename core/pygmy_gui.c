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

void colorSetRGB( PYGMYCOLOR *pygmyColor, u8 ucR, u8 ucG, u8 ucB )
{
    pygmyColor->R = ucR;
    pygmyColor->G = ucG;
    pygmyColor->B = ucB;
}

void colorCopy( PYGMYCOLOR *colorFrom, PYGMYCOLOR *colorTo )
{
    colorTo->R = colorFrom->R;
    colorTo->G = colorFrom->G;
    colorTo->B = colorFrom->B;
}

void colorApply( PYGMYCOLOR *pygmyColor )
{
    lcdSetColor( pygmyColor->R, pygmyColor->G, pygmyColor->B );
}

PYGMYCOLOR *colorGetRootColor( void )
{
    return( &globalGUI.Color );
}

PYGMYCOLOR *colorGetRootClearColor( void )
{
    return( &globalGUI.ClearColor );
}

PYGMYCOLOR *colorGetRootFocusColor( void )
{
    return( &globalGUI.FocusColor );
}

PYGMYCOLOR *colorGetRootFocusBackColor( void )
{
    return( &globalGUI.FocusBackColor );
}


PYGMYCOLOR *colorGetRootAlphaColor( void )
{
    return( &globalGUI.AlphaColor );
}

PYGMYCOLOR *colorGetRootBackColor( void )
{
    return( &globalGUI.BackColor );
}

void fontSetColor( PYGMYFONT *pygmyFont, PYGMYCOLOR *pygmyColor )
{
    colorCopy( pygmyColor, &pygmyFont->Color );
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rFont Color Changed To: " );
    print( COM3, "0x%02X 0x%02X 0x%02X", globalGUI.Font->Color.R, globalGUI.Font->Color.G, globalGUI.Font->Color.B );
    #endif // __PYGMY_DEBUG_GUI
}

void fontSetBackColor( PYGMYFONT *pygmyFont, PYGMYCOLOR *pygmyColor )
{
    colorCopy( pygmyColor, &pygmyFont->BackColor );
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rFont BackColor Changed To: " );
    print( COM3, "0x%02X 0x%02X 0x%02X", globalGUI.Font->BackColor.R, globalGUI.Font->BackColor.G, 
        globalGUI.Font->BackColor.B );
    #endif // __PYGMY_DEBUG_GUI
}

PYGMYFONT *fontGetActive( void )
{
    return( globalGUI.Font );
}

PYGMYFONT *fontGetSmall( void )
{
    return( globalGUI.SmallFont );
}

PYGMYFONT *fontGetMedium( void )
{
    return( globalGUI.MediumFont );
}

PYGMYFONT *fontGetLarge( void )
{
    return( globalGUI.LargeFont );
}

void fontSetActive( PYGMYFONT *pygmyFont )
{
    globalGUI.Font = pygmyFont;
}

void fontSetSmall( PYGMYFONT *pygmyFont )
{
    globalGUI.SmallFont = pygmyFont;
}

void fontSetMedium( PYGMYFONT *pygmyFont )
{
    globalGUI.MediumFont = pygmyFont;
}

void fontSetLarge( PYGMYFONT *pygmyFont )
{
    globalGUI.LargeFont = pygmyFont;
}

void fontSetAll( PYGMYFONT *pygmyFont )
{
    globalGUI.Font          = pygmyFont;
    globalGUI.SmallFont     = pygmyFont;
    globalGUI.MediumFont    = pygmyFont;
    globalGUI.LargeFont     = pygmyFont;
}

/*
void guiSetFonts( PYGMYFONT *fontSmall, PYGMYFONT *fontMedium, PYGMYFONT *fontLarge )
{
    globalGUI.Font          = fontSmall;
    globalGUI.SmallFont     = fontSmall;
    globalGUI.MediumFont    = fontMedium;
    globalGUI.LargeFont     = fontLarge;
}*/

u8 fontLoad( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont )
{
	u32 ulImageStart, uiPygmyInfo;
    
    globalGUI.Font                  = pygmyFont;
    globalGUI.Font->File            = pygmyFile;
    globalGUI.Font->File            = pygmyFile;
    colorCopy( colorGetRootColor(), &pygmyFont->Color );
    colorCopy( colorGetRootBackColor(), &pygmyFont->BackColor );
    
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

void guiClearArea( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiX1, u16 uiY1 )
{
    //guiApplyClearColor();
    colorApply( pygmyColor );
    lcdClearArea( uiX0, uiY0, uiX1, uiY1 );
}

void guiClearScreen( PYGMYCOLOR *pygmyColor )
{
    //guiApplyBackColor();
    colorApply( pygmyColor );
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
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rCreating Screenshot: %s X: %d Y: %d", ucName, uiWidth, uiHeight );
    #endif // __PYGMY_DEBUG_GUI
    if( !fileOpen( &pygmyFile, "screenshot", WRITE ) ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rFile Fail!" );
        #endif // __PYGMY_DEBUG_GUI
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
        if( ucPacket == PYGMY_VECTOR_BRUSH ){
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
	u16 uiCount, uiPygmyHeader, uiPygmyInfo, uiX = 0, uiY = 0, uiWidth = 0, uiHeight = 0;
	u8 ucPacket, ucStartBPP, ucBPP, ucByte1, ucByte2, ucByte3, ucR, ucG, ucB;

    uiPygmyHeader = fileGetWord( pygmyFile, BIGENDIAN );
    fileSetPosition( pygmyFile, START, 0 );
    ulPixelIndex = guiGetImage( pygmyFile, uiIndex );
    fileSetPosition( pygmyFile, START, ulPixelIndex );
    // Warning! ulPixelIndex is reused below
  
    uiPygmyInfo = fileGetWord( pygmyFile, BIGENDIAN );
	
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
        uiWidth = fileGetWord( pygmyFile, BIGENDIAN );
        uiHeight = fileGetWord( pygmyFile, BIGENDIAN );
	} else{
        uiWidth = fileGetChar( pygmyFile );
        uiHeight = fileGetChar( pygmyFile );
    } // else
    
	ulLen = (u32) (uiWidth * uiHeight);
    ucBPP = ( uiPygmyInfo & 0x000F ); 
    ucStartBPP = lcdGetBPP();
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
            } // if
            if( ucPacket & BIT6 ){
                if( ( uiPygmyHeader & PYGMY_PBM_FONT ) && ( ucBPP == PYGMY_PBM_1BPP ) ){
                    colorApply( &globalGUI.Font->Color );
                    //print( COM3, "\rFont && 1BPP" );
                    //guiApplyFontColor(  );
                    //guiApplyColor();
                } else if( ucBPP != PYGMY_PBM_1BPP ){
                    lcdSetColor( ucR, ucG, ucB );
                } else{
                    colorApply( colorGetRootColor() );
                } // else
                lcdDrawPixel( uiXPos+uiX, uiYPos+uiY );
            } else if( !( uiPygmyInfo & PYGMY_PBM_ALPHA  ) ){
                if( ( uiPygmyHeader & PYGMY_PBM_FONT ) && ( ucBPP == PYGMY_PBM_1BPP ) ){
                    colorApply( &globalGUI.Font->BackColor );
                    //guiApplyFontBackColor( );
                    //guiApplyBackColor();
                // } else if( ucBPP != PYGMY_PBM_1BPP ){
                //    lcdSetColor( ucR, ucG, ucB );
                } else{
                    //guiApplyBackColor( );
                    colorApply( colorGetRootBackColor() );
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
    lcdSetBPP( ucStartBPP );
	return( uiWidth );
}
/*
void drawVector( PYGMYFILE &pygmyFile, u16 uiX, u16 uiY )
{
    u32 i, ii, ulPixelIndex, ulLen;
	u16 uiCount, uiPygmyInfo, uiPointX, uiPointY, uiWidth = 0, uiHeight = 0;
	u8 ucPacket, ucLen, ucBPP, ucByte1, ucByte2, ucByte3, ucR, ucG, ucB;

    ulPixelIndex = guiGetImage( pygmyFile, uiIndex );
    fileSetPosition( pygmyFile, START, ulPixelIndex );
  
    uiPygmyInfo = fileGetWord( pygmyFile, BIGENDIAN );
	
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
        uiWidth = fileGetWord( pygmyFile, BIGENDIAN );
        uiHeight = fileGetWord( pygmyFile, BIGENDIAN );
	} else{
        uiWidth = fileGetChar( pygmyFile );
        uiHeight = fileGetChar( pygmyFile );
    } // else

    for( i = 0; ; i++ ){
        ucPacket = fileGetChar( pygmyFile );
        ucLen = ucPacket & ~PYGMY_VECTOR_MASK;
        ucPacket &= PYGMY_VECTOR_MASK;
        if( ucPacket == PYGMY_VECTOR_END ){
            break;
        } else if( ucPacket == PYGMY_VECTOR_POLY ){
            
            for( ii = 0; ii < ucLen; ii++ ){
                uiPointX1 = fileGetWord( pygmyFile, BIGENDIAN );
                uiPointY1 = fileGetWord( pygmyFile, BIGENDIAN );
                uiPointX2 = fileGetWord( pygmyFile, BIGENDIAN );
                uiPointY2 = fileGetWord( pygmyFile, BIGENDIAN );
                drawLine( uiPointX1, uiPointY1, uiPointX2, uiPointY2, VISIBLE );
            } // for
        } else if( ucPacket == PYGMY_VECTOR_ARC ){
            
        } else if( ucPacket == PYGMY_VECTOR_SPLINE ){
        
        } else if( ucPacket == PYGMY_VECTOR_RASTER ){
        
        } else if( ucPacket == PYGMY_VECTOR_TEXT ){
        
        } else if( ucPacket == PYGMY_VECTOR_FONT ){
        
        } // else if
    } // for
}
*/


void drawPixel( PYGMYCOLOR *pygmyColor, u16 uiX, u16 uiY )
{
    //guiApplyColor();
    colorApply( pygmyColor );
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
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rSOI: 0x%X APP0: 0x%X APP0Len: 0x%X", uiSOI, uiAPP0, uiAPP0Len );
    print( COM3, "\rID: %s Version: %d", ucID, uiVersion );
    print( COM3, "\rWidth: %d Height: %d", uiWidth, uiHeight );
    print( COM3, "\rThumbWidth: %d ThumbHeight: %d", uiThumbWidth, uiThumbHeight );
    #endif // __PYGMY_DEBUG_GUI
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
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rChunk: %s, Len: %d", ucChunkType, ulLen );
            print( COM3, "\rWidth: %d Height: %d", ulWidth, ulHeight );
            print( COM3, "\rBPP: %d", ucBPP );
            print( COM3, "\rColor: %d", ucColorType );
            print( COM3, "\rCompression: %d", ucCompression );
            print( COM3, "\rFilter: %d", ucFilter );
            print( COM3, "\rInterlace: %d", ucInterlace );
            #endif // __PYGMY_DEBUG_GUI
        } else if( isStringSame( "PLTE", ucChunkType ) ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rPalette: %d", ulLen );
            #endif // __PYGMY_DEBUG_GUI
            fileSetPosition( pygmyFile, CURRENT, ulLen );
            
        } else if( isStringSame( "IDAT", ucChunkType ) ){
            // Image data chunk, there may be multiple
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rData: %d", ulLen );
            #endif // __PYGMY_DEBUG_GUI
            fileSetPosition( pygmyFile, CURRENT, ulLen );
        } else if( isStringSame( "IEND", ucChunkType ) ){
            // End of image reached,
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rEnd" );
            #endif // __PYGMY_DEBUG_GUI
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
/*
u8 widgetDelete( )
{

}

void formCurrentSetColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalForms[ globalFormsLen ].Color.R = ucR;
    globalForms[ globalFormsLen ].Color.G = ucG;
    globalForms[ globalFormsLen ].Color.B = ucB;
}

void formCurrentSetBackColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalForms[ globalFormsLen ].BackColor.R = ucR;
    globalForms[ globalFormsLen ].BackColor.G = ucG;
    globalForms[ globalFormsLen ].BackColor.B = ucB;
}

void formCurrentSetAlphaColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalForms[ globalFormsLen ].AlphaColor.R = ucR;
    globalForms[ globalFormsLen ].AlphaColor.G = ucG;
    globalForms[ globalFormsLen ].AlphaColor.B = ucB;
}

void formCurrentSetClearColor( u8 ucR, u8 ucG, u8 ucB )
{
    globalForms[ globalFormsLen ].ClearColor.R = ucR;
    globalForms[ globalFormsLen ].ClearColor.G = ucG;
    globalForms[ globalFormsLen ].ClearColor.B = ucB;
}
*/

u8 formNew( u16 uiX, u16 uiY, u16 uiWidth, u16 uiHeight )
{
    PYGMYFORM *pygmyForms = NULL;
    PYGMYWIDGET *newWidget;

    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rCreating Form..." );
    #endif // __PYGMY_DEBUG_GUI
    if( globalFormsStatus ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\r\tReallocating Form Data" );
        #endif // __PYGMY_DEBUG_GUI
        pygmyForms = sysReallocate( globalForms, sizeof( PYGMYFORM ) * globalFormsLen + 1 ); 
    } else{
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\r\tAllocating Form Data" );
        #endif // __PYGMY_DEBUG_GUI
        pygmyForms = sysAllocate( sizeof( PYGMYFORM ) );
        globalFormsStatus = 1;
    } // else
    if( !pygmyForms ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rMemory Fail on Form Creation!" );
        #endif // __PYGMY_DEBUG_GUI
        return( 0 );
    } // if
    globalForms = pygmyForms;
    globalForms[ globalFormsLen ].Len = 0;
    newWidget = sysAllocate( sizeof( PYGMYWIDGET ) );
    if( !newWidget ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rMemory Full!" );
        #endif // __PYGMY_DEBUG_GUI
        return( FALSE );
    } // if
    globalForms[ globalFormsLen ].X             = uiX;
    globalForms[ globalFormsLen ].Y             = uiY;
    globalForms[ globalFormsLen ].Width         = uiWidth;
    globalForms[ globalFormsLen ].Height        = uiHeight;
    globalForms[ globalFormsLen ].Widgets       = newWidget;
    globalForms[ globalFormsLen ].Selected      = 0;
    colorCopy( &globalGUI.AlphaColor, &globalForms[ globalFormsLen ].AlphaColor );
    colorCopy( &globalGUI.BackColor, &globalForms[ globalFormsLen ].BackColor );
    colorCopy( &globalGUI.ClearColor, &globalForms[ globalFormsLen ].ClearColor );
    colorCopy( &globalGUI.Color, &globalForms[ globalFormsLen ].Color );
    colorCopy( &globalGUI.FocusColor, &globalForms[ globalFormsLen ].FocusColor );
    colorCopy( &globalGUI.FocusBackColor, &globalForms[ globalFormsLen ].FocusBackColor );
    ++globalFormsLen; 
    
    return( 1 );
}

void formRemove( void )
{
    PYGMYFORM *pygmyForms = NULL;

    if( globalFormsLen ){
        if( globalForms[ globalFormsLen - 1 ].Widgets ){
            sysFree( globalForms[ globalFormsLen - 1 ].Widgets );
        } // if
        pygmyForms = sysReallocate( globalForms, sizeof( PYGMYFORM ) * ( globalFormsLen - 1 ) );
        if( pygmyForms ){
            globalForms = pygmyForms;
            --globalFormsLen;
        } // if
    } // if
    
}

void formDrawAll( void )
{
    
}

void formAddEventHandler( void *pygmyFunc, u8 ucEvent, u8 ucPin, u8 ucTrigger )
{
    // Any pin interrupts or "low-level" events are handled by the form
    // The form then generates widget level events as appropriate
    u16 uiIndex = globalFormsLen - 1;
    if( ucPin != NONE ){
        // NONE is valid when event handler isn't pin driven
        pinInterrupt( pygmyFunc, ucPin, ucTrigger, 1 );
    } // if
    if( ucEvent == CREATED ){
        globalForms[ uiIndex ].Created = pygmyFunc;
    } else if( ucEvent == DESTROYED ){
        globalForms[ uiIndex ].Destroyed = pygmyFunc;
    } else if( ucEvent == DRAW ){
        globalForms[ uiIndex ].Draw = pygmyFunc;
    } else if( ucEvent == SELECTION ){  
        globalForms[ uiIndex ].Selection = pygmyFunc;
    } else if( ucEvent == MOUSE_MOVE ){
        globalForms[ uiIndex ].MouseMove = pygmyFunc;
    } else if( ucEvent == MOUSE_CLICK ){
        globalForms[ uiIndex ].MouseClick = pygmyFunc;
    } // else if
}

PYGMYWIDGET *widgetGet( u8 *ucName )
{
    u16 i;
    
    for( i = 0; i < globalForms[ globalFormsLen - 1 ].Len; i++ ){
        if( isStringSame( globalForms[ globalFormsLen - 1 ].Widgets[ i ].String, ucName ) ){
            return( &globalForms[ globalFormsLen - 1 ].Widgets[ i ] );
        } // if
    } // for

    return( NULL );
}

PYGMYWIDGET *widgetGetFocused( void )
{
    // Returns widget with focus from current form
    
    return ( &globalForms[ globalFormsLen - 1 ].Widgets[ globalForms[ globalFormsLen - 1 ].Selected ] );
}

PYGMYWIDGET *widgetGetCurrent( void )
{
    // Returns last created widget on last created form
    
    return ( &globalForms[ globalFormsLen - 1 ].Widgets[ globalForms[ globalFormsLen - 1 ].Len - 1 ] );
}

void widgetRun( PYGMYWIDGET *pygmyWidget )
{
    if( pygmyWidget->Selected ){
        pygmyWidget->Selected();
    } // if
}

u8 widgetAddEventHandler( PYGMYWIDGET *pygmyWidget, void *pygmyFunc, u8 ucEvent )
{
    
    if( ucEvent == DRAW ){
        pygmyWidget->Draw = pygmyFunc;
    } else if( ucEvent == GOTFOCUS ){
        pygmyWidget->GotFocus = pygmyFunc;
    } else if( ucEvent == LOSTFOCUS ){
        pygmyWidget->LostFocus = pygmyFunc;
    } else if( ucEvent == SELECTED ){
        pygmyWidget->Selected = pygmyFunc;
    } else if( ucEvent == CREATED ){
        pygmyWidget->Created = pygmyFunc;
    } else if( ucEvent == DESTROYED ){
        pygmyWidget->Destroyed = pygmyFunc;
    } // else if
}

void formEventHandler( u8 ucEvent )
{
    PYGMYWIDGET *pygmyWidget;
    
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rEvent 0x%02X ", ucEvent );
    #endif // __PYGMY_DEBUG_GUI
    pygmyWidget = widgetGetFocused();
    if( !pygmyWidget ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rWidget Failed!" );
        #endif // __PYGMY_DEBUG_GUI
        return;
    } // if
    if( ucEvent == DRAW ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, " DRAW" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->Draw ){
            pygmyWidget->Draw();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
            drawWidget( pygmyWidget );
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Completed Widget Draw->Event" );
            #endif // __PYGMY_DEBUG_GUI
        } // else
    } else if( ucEvent == MOUSE_MOVE ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "MOUSE_MOVE" );
        #endif // PYGMY_DEBUG_GUI
        if( pygmyWidget->MouseMove ){
            pygmyWidget->MouseMove();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
            if( globalGUI.EventValue == MOUSE_UP || globalGUI.EventValue == MOUSE_LEFT ){
                formFocusPrevious();
            } else{
                formFocusNext();
            } // else
        } // else
    } else if( ucEvent == MESSAGE ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "MESSAGE" );
        #endif // __PYGMY_DEBUG_GUI
    } else if( ucEvent == MOUSE_CLICK ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "MOUSE_CLICK" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->MouseClick ){
            pygmyWidget->MouseClick();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rDraw Invert" );
            #endif // __PYGMY_DEBUG_GUI
            pygmyWidget->Style |= INVERT;
            //formEventHandler( DRAW );
            if( pygmyWidget->Draw ){
                pygmyWidget->Draw();
            } else{
                drawWidget( pygmyWidget );
            } // else
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rClear Invert" );
            //delay( 5000 );
            #endif // __PYGMY_DEBUG_GUI
            pygmyWidget->Style &= ~INVERT;
            //formEventHandler( DRAW );
            if( pygmyWidget->Draw ){
                pygmyWidget->Draw();
            } else{
                drawWidget( pygmyWidget );
            } // else
        } // else
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rPassing control to SELECTED" );
        #endif // __PYGMY_DEBUG_GUI
        formEventHandler( SELECTED );
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, " Done." );
        #endif // __PYGMY_DEBUG_GUI
    } else if( ucEvent == GOTFOCUS ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "GOTFOCUS" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->GotFocus ){
            pygmyWidget->GotFocus();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
            pygmyWidget->Style |= ACTIVE;
            formEventHandler( DRAW );
        } // else
    } else if( ucEvent == LOSTFOCUS ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "LOSTFOCUS" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->LostFocus ){
            pygmyWidget->LostFocus();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
            pygmyWidget->Style &= ~ACTIVE;
            formEventHandler( DRAW );
        } // else
    } else if( ucEvent == SELECTED ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rSELECTED" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->Selected ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rCalling Handler" );
            #endif // __PYGMY_DEBUG_GUI
            pygmyWidget->Selected();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
        } // else
    } else if( ucEvent == CREATED ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "CREATED" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->Created ){
            pygmyWidget->Created();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
        } // else
    } else if( ucEvent == DESTROYED ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "DESTROYED" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->Destroyed ){
            pygmyWidget->Destroyed();
        } else{
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Generic" );
            #endif // __PYGMY_DEBUG_GUI
        } // else
    } // else if
}

void formCallFocused( void )
{
    if( globalForms[ globalFormsLen - 1 ].Widgets[ globalForms[ globalFormsLen - 1 ].Selected ].Selected ){
        globalForms[ globalFormsLen - 1 ].Widgets[ globalForms[ globalFormsLen - 1 ].Selected ].Selected();
    } // if
}

u8 formSetFocus( u16 uiSelected )
{
    // Not the same as the SELECTED event, this selects the active widget
    // 
    formEventHandler( LOSTFOCUS );
    if( uiSelected <= globalForms[ globalFormsLen - 1 ].Len ){
        globalForms[ globalFormsLen - 1 ].Selected = uiSelected;
    } // if
    formEventHandler( GOTFOCUS );

    return( FALSE );
}

u16 formGetFocus( void )
{
    return( globalForms[ globalFormsLen - 1 ].Selected );
}

u16 formGetWidgetCount( void )
{
    return( globalForms[ globalFormsLen - 1 ].Len );
}

void formFocusNext( void )
{
    u16 i, uiSelected;

    //if( globalForms[ globalFormsLen - 1 ].Len == 0 ){
    //    return;
    //} // if
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rFocusNext" );
    #endif // __PYGMY_DEBUG_GUI
    uiSelected = globalForms[ globalFormsLen - 1 ].Selected;
    for( i = 0; i < globalForms[ globalFormsLen - 1 ].Len && i < 255; i++ ){
        if( uiSelected < ( globalForms[ globalFormsLen - 1 ].Len ) ){
            ++uiSelected;
        } else{
            uiSelected = 0;
        } // else
        if( globalForms[ globalFormsLen - 1 ].Widgets[ uiSelected ].Type == BUTTON ){
            formEventHandler( LOSTFOCUS );
            globalForms[ globalFormsLen - 1 ].Selected = uiSelected;
            /*if( globalForms[ globalFormsLen - 1 ].Selected < ( globalForms[ globalFormsLen - 1 ].Len - 1 ) ){
                ++globalForms[ globalFormsLen - 1 ].Selected;
            } else{
                globalForms[ globalFormsLen - 1 ].Selected = 0;
            } // else
            */
            formEventHandler( GOTFOCUS );
            return;
        } // if
    } // for
}

void formFocusPrevious( void )
{
    u16 i, uiSelected;

    uiSelected = globalForms[ globalFormsLen - 1 ].Selected;
    for( i = 0; i < globalForms[ globalFormsLen - 1 ].Len && i < 255; i++ ){
        if( uiSelected > 0 ){
            --uiSelected;
        } else{
            uiSelected = globalForms[ globalFormsLen - 1 ].Len - 1;
        } // else
        if( globalForms[ globalFormsLen - 1 ].Widgets[ uiSelected ].Type == BUTTON ){
            formEventHandler( LOSTFOCUS );
            globalForms[ globalFormsLen - 1 ].Selected = uiSelected;
            /*if( globalForms[ globalFormsLen - 1 ].Selected < ( globalForms[ globalFormsLen - 1 ].Len - 1 ) ){
                ++globalForms[ globalFormsLen - 1 ].Selected;
            } else{
                globalForms[ globalFormsLen - 1 ].Selected = 0;
            } // else
            */
            formEventHandler( GOTFOCUS );
            return;
        } // if
    } // for

    /*if( globalForms[ globalFormsLen - 1 ].Len == 0 ){
        return;
    } // if
    print( COM3, "\rFocus Previous" );
    formEventHandler( LOSTFOCUS );
    if( globalForms[ globalFormsLen - 1 ].Selected > 0 ){
        --globalForms[ globalFormsLen - 1 ].Selected;
    } else{
        globalForms[ globalFormsLen - 1 ].Selected = globalForms[ globalFormsLen - 1 ].Len - 1;
    } // else
    formEventHandler( GOTFOCUS );*/
}

u8 formAddWidget( PYGMYWIDGET *pygmyWidget )
{
    PYGMYWIDGET *newWidget;
    u32 ulLen;
    u16 uiIndex = globalFormsLen - 1;
    
    ulLen = sizeof( PYGMYWIDGET ) * ( globalFormsLen + 1 );
    newWidget = sysReallocate( globalForms[ uiIndex ].Widgets, ulLen );
    if( !newWidget ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rMemory Full!" );
        #endif // __PYGMY_DEBUG_GUI
        return( FALSE );
    } // if
    globalForms[ uiIndex ].Widgets = newWidget;
    
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Value    = pygmyWidget->Value;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].X        = pygmyWidget->X;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Y        = pygmyWidget->Y;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Width    = pygmyWidget->Width;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Height   = pygmyWidget->Height;
    if( globalForms[ uiIndex ].Len == 0 ){
        globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Style    = pygmyWidget->Style | (ACTIVE);
    } else{
        globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Style    = pygmyWidget->Style;
    } // else
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Type     = pygmyWidget->Type;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].String   = pygmyWidget->String;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Created    = NULL;//pygmyWidget->Created;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Destroyed  = NULL;//pygmyWidget->Destroyed;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].MouseMove  = NULL;    
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].GotFocus  = NULL;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].LostFocus  = NULL;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].MouseClick = NULL;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Draw       = NULL;
    //globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Deselected = NULL;//pygmyWidget->Deselected;
    globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Selected   = NULL;//pygmyWidget->Selected;
    colorCopy( &globalForms[ uiIndex ].Color, &globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].Color );
    colorCopy( &globalForms[ uiIndex ].BackColor, &globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].BackColor );
    colorCopy( &globalForms[ uiIndex ].FocusColor, &globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].FocusColor );
    colorCopy( &globalForms[ uiIndex ].FocusBackColor, &globalForms[ uiIndex ].Widgets[ globalForms[ uiIndex ].Len ].FocusBackColor );
    ++globalForms[ uiIndex ].Len;
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rWidgets: %d", globalForms[ uiIndex ].Len );
    #endif // __PYGMY_DEBUG_GUI
    return( TRUE );
}

void formFreeAll( void )
{
    u16 i;

    if( globalForms ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rFreeing form data" );
        #endif // __PYGMY_DEBUG_GUI
        for( i = 0; i < globalFormsLen; i++ ){
            if( globalForms[ i ].Widgets ){
                sysFree( globalForms[ i ].Widgets );
            } // if
        } // form
        sysFree( globalForms );
    } // if
    globalForms = NULL;
    globalFormsStatus = 0;
    globalFormsLen = 0;
}

void drawCurrentForm( void )
{ 
    PYGMYCOLOR *pygmyColor;
    u16 i, ii;
   
    for( i = 0; i < globalFormsLen; i++ ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rDrawing Form: %d", i );
        print( COM3, "\r\tLen: %d", globalForms[ i ].Len );
        print( COM3, "\r\tX,Y: %d,%d", globalForms[ i ].X, globalForms[ i ].Y );
        print( COM3, "\r\tWidth,Height: %d,%d", globalForms[ i ].Width, globalForms[ i ].Height );
        #endif // __PYGMY_DEBUG_GUI
        //colorCopy( &globalForms[ i ].ClearColor, &globalGUI.ClearColor );
        //guiSetBackColor( globalForms[ i ].BackColor.R, globalForms[ i ].BackColor.G, globalForms[ i ].BackColor.B );
        guiClearArea( &globalForms[ i ].ClearColor, globalForms[ i ].X, globalForms[ i ].Y, 
            globalForms[ i ].X + globalForms[ i ].Width, 
            globalForms[ i ].Y + globalForms[ i ].Height );
        for( ii = 0; ii < globalForms[ i ].Len; ii++ ){
            PYGMY_WATCHDOG_REFRESH;
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\r\tDrawing Widget %d %s", ii, globalForms[ i ].Widgets[ ii ].String );
            #endif // __PYGMY_DEBUG_GUI
            drawWidget( &globalForms[ i ].Widgets[ ii ] );
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Completed Drawing Widget->CurrentForm" );
            #endif // __PYGMY_DEBUG_GUI
        } // for
    } // for
}

void drawForms( void )
{ 
    PYGMYCOLOR *pygmyColor;
    u16 i, ii;
   
    for( i = 0; i < globalFormsLen; i++ ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rDrawing Form: %d", i );
        #endif // __PYGMY_DEBUG_GUI
        //print( COM3, "\r\tLen: %d", globalForms[ i ].Len );
        //print( COM3, "\r\tX,Y: %d,%d", globalForms[ i ].X, globalForms[ i ].Y );
        //print( COM3, "\r\tWidth,Height: %d,%d", globalForms[ i ].Width, globalForms[ i ].Height );
        
        guiClearArea( &globalForms[ i ].ClearColor, globalForms[ i ].X, globalForms[ i ].Y, 
            globalForms[ i ].X + globalForms[ i ].Width, 
            globalForms[ i ].Y + globalForms[ i ].Height );
        for( ii = 0; ii < globalForms[ i ].Len; ii++ ){
            PYGMY_WATCHDOG_REFRESH;
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\r\tDrawing Widget %d %s", ii, globalForms[ i ].Widgets[ ii ].String );
            #endif // __PYGMY_DEBUG_GUI
            drawWidget( &globalForms[ i ].Widgets[ ii ] );
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, " Completed Drawing Widget->Form" );
            #endif // __PYGMY_DEBUG_GUI
        } // for
    } // for
}

void drawWidget( PYGMYWIDGET *pygmyWidget )
{
    PYGMYCOLOR pygmyColor, pygmyBackColor;
    u32 ulStyle;
    u16 uiX, uiY, uiPos, uiPoly[ 8 ];
    u8 ucRadius;

    if( !(pygmyWidget->Style & VISIBLE ) ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\rWidget not visible" );
        #endif // __PYGMY_DEBUG_GUI
        return;
    } // if
    //print( COM3, "\rWidget: %s", pygmyWidget->String );
    
    ucRadius = globalGUI.Radius;
    
    if( pygmyWidget->Style & INVERT ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\r\tInvert" );
        #endif // __PYGMY_DEBUG_GUI
        if( pygmyWidget->Style & ACTIVE ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\r\tActive" );
            #endif // __PYGMY_DEBUG_GUI
            colorCopy( &pygmyWidget->FocusColor, &pygmyBackColor );
            colorCopy( &pygmyWidget->FocusBackColor, &pygmyColor );
            //fontSetColor( fontGetActive(), &pygmyWidget->BackColor );
        } else{
            colorCopy( &pygmyWidget->Color, &pygmyBackColor );
            colorCopy( &pygmyWidget->BackColor, &pygmyColor );
            //fontSetColor( fontGetActive(), &pygmyBackColor );
        } // else
    } else{    
        if( pygmyWidget->Style & ACTIVE ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\r\tActive" );
            #endif // __PYGMY_DEBUG_GUI
            colorCopy( &pygmyWidget->FocusColor, &pygmyColor );
            colorCopy( &pygmyWidget->FocusBackColor, &pygmyBackColor );
            //fontSetColor( fontGetActive(), &pygmyWidget->BackColor );
        } else{ 
            colorCopy( &pygmyWidget->Color, &pygmyColor );
            colorCopy( &pygmyWidget->BackColor, &pygmyBackColor );
            //
        } // else
    } // else
    fontSetColor( fontGetActive(), &pygmyColor );
    fontSetBackColor( fontGetActive(), &pygmyBackColor );
    if( pygmyWidget->Style & BORDER ){
        //print( COM3, "\r\tBorder" );
        
        drawRect( &pygmyColor, pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style & ~FILLED, ucRadius );
    } // if 
    if( pygmyWidget->Type == BUTTON || pygmyWidget->Type == LABEL ){
        /*if ( pygmyWidget->Type == BUTTON ){
            print( COM3, "\r\tButton" );
        } else{
            print( COM3, "\r\tLabel" );
            //guiClearArea( colorGetRootClearColor(), pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            //    pygmyWidget->Y+pygmyWidget->Height );
        } // else*/
        if( pygmyWidget->Style & FILLED ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\r\tFilled with:" );
            print( COM3, " R: 0x%02X", pygmyBackColor.R );
            print( COM3, " G: 0x%02X", pygmyBackColor.G );
            print( COM3, " B: 0x%02X", pygmyBackColor.B );
            #endif // __PYGMY_DEBUG_GUI
            drawRect( &pygmyBackColor, pygmyWidget->X+1, pygmyWidget->Y+1, pygmyWidget->X + ( pygmyWidget->Width - 1 ),
                pygmyWidget->Y + ( pygmyWidget->Height - 1 ), pygmyWidget->Style, ucRadius );
        } // if
        
        if( pygmyWidget->Style & CENTERED ){
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\rCentered" );
            #endif // __PYGMY_DEBUG_GUI
            uiY = ( pygmyWidget->Height - globalGUI.Font->Height ) / 2;
            uiX = 2 + ( ( pygmyWidget->Width - ( ( globalGUI.Font->Height - 7 ) * len( pygmyWidget->String ) ) ) / 2 );
            #ifdef __PYGMY_DEBUG_GUI
            print( COM3, "\ruiX: %d", uiX );  
            #endif // __PYGMY_DEBUG_GUI
        } else{
            uiX = 0;
            uiY = ( pygmyWidget->Height - globalGUI.Font->Height ) / 2;
        } // else
    } else if( pygmyWidget->Type == CHECKBOX ){
        //print( COM3, "\r\tCheckbox" );
        //guiApplyColor();
        drawRect( &pygmyColor, pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        if( pygmyWidget->Value ){ // Draw Check if not zero
            drawLine( &pygmyWidget->Color, pygmyWidget->X+1, pygmyWidget->Y+(pygmyWidget->Height/2), pygmyWidget->X+(pygmyWidget->Height/2),
                pygmyWidget->Y + ( pygmyWidget->Height - 1 ), pygmyWidget->Style );
            
            drawLine( &pygmyColor, pygmyWidget->X+(pygmyWidget->Height/2), pygmyWidget->Y + ( pygmyWidget->Height - 1 ), 
                pygmyWidget->X+(pygmyWidget->Height - 1), pygmyWidget->Y - 1, pygmyWidget->Style );
        } // if
        uiX = 2 + pygmyWidget->Height;
        uiY = 2 + ( pygmyWidget->Height - globalGUI.Font->Height ) / 2;
        
        //print( COM3, "\rWidget Height: %d", pygmyWidget->Height );
        //print( COM3, "\rFont Height: %d", globalGUI.Font->Height );
        //print( COM3, "\ruiX: %d uiY: %d", uiX, uiY );
    } else if( pygmyWidget->Type == VSCROLLBAR ){
        //print( COM3, "\r\tVScroll" );
        //guiApplyColor();
        drawRect( &pygmyColor, pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawRect( &pygmyColor, pygmyWidget->X + ( pygmyWidget->Width - pygmyWidget->Height ), pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
    } else if( pygmyWidget->Type == HSCROLLBAR ){
        #ifdef __PYGMY_DEBUG_GUI
        print( COM3, "\r\tHScroll" );
        #endif // __PYGMY_DEBUG_GUI
        //guiApplyColor();
        drawRect( &pygmyColor, pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Height,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawRect( &pygmyColor, pygmyWidget->X + ( pygmyWidget->Width - pygmyWidget->Height ), pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        // Left Arrow
        drawLine( &pygmyColor, pygmyWidget->X + 2, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->Style );
        drawLine( &pygmyColor, pygmyWidget->X + 2, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        drawLine( &pygmyColor, pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->X + ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        // Right Arrow
        drawLine( &pygmyColor, pygmyWidget->X + ( pygmyWidget->Width - 4 ), pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->Style );
        drawLine( &pygmyColor, pygmyWidget->X + pygmyWidget->Width - 4, pygmyWidget->Y + ( pygmyWidget->Height / 2 ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        drawLine( &pygmyColor, pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( pygmyWidget->Height - ( ucRadius / 2 ) ),
            pygmyWidget->X + pygmyWidget->Width - ( pygmyWidget->Height - 4 ), pygmyWidget->Y + ( ucRadius / 2 ),
            pygmyWidget->Style );
        if( pygmyWidget->Value ){
            uiPos = pygmyWidget->Width / ( 100 / ( pygmyWidget->Value ) ); 
        } else{
            uiPos = pygmyWidget->Height;
        } // else
        drawRect( &pygmyColor, uiPos - ( pygmyWidget->Height / 2 ), pygmyWidget->Y, uiPos + ( pygmyWidget->Height / 2 ), 
            pygmyWidget->Y + pygmyWidget->Height, pygmyWidget->Style, ucRadius );
        drawLine( &pygmyColor, uiPos, pygmyWidget->Y + 4, uiPos, pygmyWidget->Y + ( pygmyWidget->Height - 4 ), pygmyWidget->Style );
        drawLine( &pygmyColor, uiPos - 4, pygmyWidget->Y + 6, uiPos - 4, pygmyWidget->Y + ( pygmyWidget->Height - 6 ), pygmyWidget->Style );
        drawLine( &pygmyColor, uiPos + 4, pygmyWidget->Y + 6, uiPos + 4, pygmyWidget->Y + ( pygmyWidget->Height - 6 ), pygmyWidget->Style );
    } 
    if( pygmyWidget->Style & CAPTION ){
        if( pygmyWidget->String ){ 
            guiSetCursor( pygmyWidget->X + uiX, pygmyWidget->Y + uiY  );
            /*if( !(pygmyWidget->Style & INVERT) ){
                guiSetBackColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
                guiSetColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
            } else{
                guiSetColor( globalGUI.Font->Color.B, globalGUI.Font->Color.G, globalGUI.Font->Color.R );
                guiSetBackColor( globalGUI.Font->BackColor.B, globalGUI.Font->BackColor.G, globalGUI.Font->BackColor.R );
            } // else
            */
            #ifdef __PYGMYSTREAMLCD
                print( LCD, pygmyWidget->String );
            #endif
        } else{
            guiSetCursor( pygmyWidget->X, pygmyWidget->Y );
            #ifdef __PYGMYSTREAMLCD
                print( LCD, "%d", pygmyWidget->Value );
            #endif
        } // else
    } 
    #ifdef __PYGMY_DEBUG_GUI
    print( COM3, "\rDone." );
    #endif // __PYGMY_DEBUG_GUI
}
/*
void eventGotFocus( void )
{
    PYGMYWIDGET *pygmyWidget;
    
    pygmyWidget = widgetGetFocused();
    pygmyWidget->Style |= INVERT;
    formEventHandler( DRAW );
}

void eventLostFocus( void )
{
    PYGMYWIDGET *pygmyWidget;
    
    pygmyWidget = widgetGetFocused();
    pygmyWidget->Style &= ~INVERT;
    formEventHandler( DRAW );
}*/

void eventMouseMove( void )
{
    // Generic Event Handler for Forms
    formEventHandler( MOUSE_MOVE );
}

void eventMouseMoveUp( void )
{
    // Generic Event Handler for Mouse/Pointer
    if( globalGUI.Mouse.Y > 0 ){
        --globalGUI.Mouse.Y;
    } //
    globalGUI.Event = MOUSE_MOVE;
    globalGUI.EventValue = MOUSE_UP;
    formEventHandler( MOUSE_MOVE );
}

void eventMouseMoveDown( void )
{
    // Generic Event Handler for Mouse/Pointer
    if( globalGUI.Mouse.Y < lcdGetHeight() - 1 ){
        ++globalGUI.Mouse.Y;
    } // if
    globalGUI.Event = MOUSE_MOVE;
    globalGUI.EventValue = MOUSE_DOWN;
    formEventHandler( MOUSE_MOVE );
}

void eventMouseMoveLeft( void )
{
    // Generic Event Handler for Mouse/Pointer
    if( globalGUI.Mouse.X > 0 ){
        --globalGUI.Mouse.X;
    } // if
    globalGUI.Event = MOUSE_MOVE;
    globalGUI.EventValue = MOUSE_LEFT;
    formEventHandler( MOUSE_MOVE );
}

void eventMouseMoveRight( void )
{
    // Generic Event Handler for Mouse/Pointer
    if( globalGUI.Mouse.X < lcdGetWidth() ){
        ++globalGUI.Mouse.X;
    } // if
    globalGUI.Event = MOUSE_MOVE;
    globalGUI.EventValue = MOUSE_RIGHT;
    formEventHandler( MOUSE_MOVE );
}

void eventMouseClickLeft( void )
{
    // Generic Event Handler for Mouse/Pointer
    globalGUI.Event = MOUSE_CLICK;
    globalGUI.EventValue = BUTTON_LEFT;
    formEventHandler( MOUSE_CLICK );
}

void eventMouseClickCenter( void )
{
    // Generic Event Handler for Mouse/Pointer
    globalGUI.Event = MOUSE_CLICK;
    globalGUI.EventValue = BUTTON_CENTER;
    formEventHandler( MOUSE_CLICK );
}

void eventMouseClickRight( void )
{
    // Generic Event Handler for Mouse/Pointer
    globalGUI.Event = MOUSE_CLICK;
    globalGUI.EventValue = BUTTON_RIGHT;
    formEventHandler( MOUSE_CLICK );
}

void eventGotFocus( void )
{
    // Generic Event Handler for Widgets
    //widgetRun( widgetGetFocused() );
    globalGUI.Event = GOTFOCUS;
    formEventHandler( GOTFOCUS );
}

void eventLostFocus( void )
{
    // Generic Event Handler for Widgets
    globalGUI.Event = LOSTFOCUS;
    formEventHandler( LOSTFOCUS );
}

void eventSelected( void )
{
    // Generic Event Handler for Forms/Widgets
    globalGUI.Event = SELECTED;
    formEventHandler( SELECTED );
}

void drawFill( PYGMYCOLOR *pygmyColor, u16 x, u16 y )
{
    if( lcdGetPixel(x,y) != 0 ){
        lcdDrawPixel(x,y);
        drawFill( pygmyColor, x+1,y);
        drawFill(pygmyColor, x-1,y);
        drawFill(pygmyColor, x,y+1);
        drawFill(pygmyColor, x,y-1);
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

void drawRect( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle, u16 uiRadius )
{      
    u16 i, ii;

    colorApply( pygmyColor );
    if( ulStyle & FILLED ){
        if( uiRadius && ( ulStyle & ( ROUNDED | CHAMFER ) ) ){
            for( i = iY1+uiRadius; i < ( iY2 - uiRadius) + 1; i++ ){
                drawLine( pygmyColor, iX1, i, iX2, i, ulStyle ); 
            } // for
            for( i = 0, ii = 0; i < uiRadius; i++ ){
                drawLine( pygmyColor, iX1 + uiRadius - ii, iY1 + i, iX2 - uiRadius + ii, iY1 + i, ulStyle );
                drawLine( pygmyColor, iX1 + uiRadius - ii, iY2 - i, iX2 - uiRadius + ii, iY2 - i, ulStyle );
                if( ulStyle & CHAMFER ){
                    ++ii;
                } // if
            } // for 
        } else{
            for( i = iY1; i < iY2; i++ ){
                drawLine( pygmyColor, iX1 + 1, i, iX2 - 1, i, ulStyle );
            } // for
        } // else    
    } 
    if( ( ulStyle & CHAMFER ) ){
        drawLine( pygmyColor, iX2-uiRadius, iY1, iX2, iY1+uiRadius, ulStyle ); // top right
        drawLine( pygmyColor, iX2-uiRadius, iY2, iX2, iY2-uiRadius, ulStyle ); // bottom right
        drawLine( pygmyColor, iX1+uiRadius, iY2, iX1, iY2-uiRadius, ulStyle ); // bottom left
        drawLine( pygmyColor, iX1, iY1+uiRadius, iX1+uiRadius, iY1, ulStyle ); // top left
    } else if( ( ulStyle & ROUNDED ) ){ 
        drawRadius( pygmyColor, iX1+uiRadius, iY1+uiRadius, BIT0, uiRadius, ulStyle );
        drawRadius( pygmyColor, iX2-uiRadius, iY1+uiRadius, BIT1, uiRadius, ulStyle );
        drawRadius( pygmyColor, iX1+uiRadius, iY2-uiRadius, BIT3, uiRadius, ulStyle );
        drawRadius( pygmyColor, iX2-uiRadius, iY2-uiRadius, BIT2, uiRadius, ulStyle );
    } // else if
        drawLine( pygmyColor, iX1+uiRadius, iY1, iX2-uiRadius, iY1, ulStyle ); // top
        drawLine( pygmyColor, iX1+uiRadius, iY2, iX2-uiRadius, iY2, ulStyle ); // bottom
        drawLine( pygmyColor, iX1, iY1+uiRadius, iX1, iY2-uiRadius, ulStyle ); // left
        drawLine( pygmyColor, iX2, iY1+uiRadius, iX2, iY2-uiRadius, ulStyle ); // right
    
}

void drawPixelInArea( u16 uiX0, u16 uiY0, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{
    if( ( uiX0 >= uiX1 && uiX0 <= uiX2 ) && (  uiY0 >= uiY1 && uiY0 <= uiY2 ) ){
        lcdDrawPixel( uiX0, uiY0 ); 
    }
}

void drawRadius( PYGMYCOLOR *pygmyColor, u16 uiCenterX, u16 uiCenterY, u16 uiCorner, u16 uiRadius, u32 ulStyle )
{
    s16 f = 1 - uiRadius;
    s16 ddF_x = 1;
    s16 ddF_y = -2 * uiRadius;
    u16 uiX = 0;
    u16 uiY = uiRadius;
    
    colorApply( pygmyColor );
    //guiApplyColor();
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
                drawLine( pygmyColor, uiCenterX, uiCenterY-uiY, uiCenterX - uiX, uiCenterY - uiY, ulStyle ); // 7
                drawLine( pygmyColor, uiCenterX, uiCenterY-uiX, uiCenterX - uiY, uiCenterY - uiX, ulStyle ); // 6
            } // if
            if( uiCorner & BIT1 ){
                drawLine( pygmyColor, uiCenterX, uiCenterY-uiY, uiCenterX + uiX, uiCenterY - uiY, ulStyle ); // 0
                drawLine( pygmyColor, uiCenterX, uiCenterY-uiX, uiCenterX + uiY, uiCenterY - uiX, ulStyle ); // 1
            }
            if( uiCorner & BIT2 ){
                drawLine( pygmyColor, uiCenterX, uiCenterY+uiY, uiCenterX + uiX, uiCenterY + uiY, ulStyle ); // 3
                drawLine( pygmyColor, uiCenterX, uiCenterY+uiX, uiCenterX + uiY, uiCenterY + uiX, ulStyle ); // 2
            } // if
            if( uiCorner & BIT3 ){
                drawLine( pygmyColor, uiCenterX, uiCenterY+uiY, uiCenterX - uiX, uiCenterY + uiY, ulStyle ); // 4
                drawLine( pygmyColor, uiCenterX, uiCenterY+uiX, uiCenterX - uiY, uiCenterY + uiX, ulStyle ); // 5
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

void drawPoly( PYGMYCOLOR *pygmyColor, u16 *uiPoints, u16 uiLen, u32 ulStyle )
{
    u16 i, uiX, uiY;

    for( i = 0; i < uiLen-2; ){
        uiX = uiPoints[ i++ ];
        uiY = uiPoints[ i++ ];
        drawLine( pygmyColor, uiX, uiY, uiPoints[ i ], uiPoints[ i + 1 ], ulStyle );
    } // i
}

void drawThickLine( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u8 ucThickness, u32 ulStyle )
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
            drawLine( pygmyColor, iX1-iX, iY1-iY, iX2-iX, iY2-iY, ulStyle );
            
        } else{
            drawLine( pygmyColor, iX1+iX, iY1+iY, iX2+iX, iY2+iY, ulStyle );
            iX += iXStep;
            iY += iYStep; 
        } // else
        
    } // for
}

void drawLine( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle )
{   
    s16 dy = iY2 - iY1;
    s16 dx = iX2 - iX1;
    s16 stepx, stepy, iStepSkip, iToggle, fraction;

    colorApply( pygmyColor );
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

void drawCircle( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiRadius, u32 ulStyle )
{
    drawRadius( pygmyColor, uiX0, uiY0, BIT0|BIT1|BIT2|BIT3, uiRadius, ulStyle );
    
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


void drawSine( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiFrequency, s8 a )
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