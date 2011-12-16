/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

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

u32 getImage( PYGMYFILE *pygmyFile, u16 uiIndex )
{
    // This function seeks for image in imagestrip or font based on provided index
	u32 ulPixelIndex;
	u16 uiEntries, uiPygmyInfo;
	
	uiPygmyInfo = (u16)  fileGetChar( pygmyFile ) * 0x0100;
	uiPygmyInfo |= (u16) fileGetChar( pygmyFile );
	if( !( uiPygmyInfo & ( PYGMY_PBM_IMAGESTRIP | PYGMY_PBM_FONT ) ) ){
		return( 0 );
    } // if
	
	uiEntries = (u16)  fileGetChar( pygmyFile ) * 0x0100 ;
	uiEntries |= (u16) fileGetChar( pygmyFile );
	if( uiIndex > uiEntries) {
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

u16 drawImage( u16 uiXPos, u16 uiYPos, PYGMYFILE *pygmyFile, u16 uiIndex )
{
	u32 i, ii, ulPixelIndex, ulLen;
	u16 uiCount, uiPygmyInfo, uiX = 0, uiY = 0, uiWidth = 0, uiHeight = 0;
	u8 ucPacket, ucBPP, ucByte1, ucByte2, ucByte3, ucR, ucG, ucB;

    ulPixelIndex = getImage( pygmyFile, uiIndex );
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
    //lcdSetDrawArea( uiXPos, uiYPos, uiXPos + uiWidth, uiYPos + uiHeight );
    
	ulLen = (u32) (uiWidth * uiHeight);
    ucBPP = ( uiPygmyInfo & 0x000F ); 
    //print( COM3, "\rLoaded Image: X %d, Y %d, BPP %d", uiWidth, uiHeight, ucBPP );
    lcdSetBPP( ucBPP ); // Must be set BEFORE setting colors
    ucR = 0xFF;
    ucG = 0xFF;
    ucB = 0xFF;
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
                if( ucBPP != PYGMY_PBM_1BPP ){
                    lcdSetColor( ucR, ucG, ucB );
                } // if
            } // if
            if( ucPacket & BIT6 ){
                //lcdSetDrawArea( uiXPos+uiX, uiYPos+uiY, uiXPos+uiX, +uiYPos+uiY );
                //lcdDrawPixel( );
                lcdDrawPixel( uiXPos+uiX, uiYPos+uiY );
            } else {// if( !( uiPygmyInfo & PYGMY_PBM_ALPHA  ) ){
                //lcdSetDrawArea( uiXPos+uiX, uiYPos+uiY, uiXPos+uiX, +uiYPos+uiY );
                //lcdClearPixel( );
                lcdClearPixel( uiXPos+uiX, uiYPos+uiY );
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
/*
void drawPixel( u16 uiX, u16 uiY )
{
    //lcdSetColor( uiColor );
    lcdSetDrawArea( uiX, uiY, uiX, uiY );
    lcdDrawPixel( );
}

void drawClearPixel( u16 uiX, u16 uiY )
{
    lcdSetDrawArea( uiX, uiY, uiX, uiY );
    lcdClearPixel();
}*/


void drawWidget( PYGMYWIDGET *pygmyWidget )
{
    PYGMYFONT *pygmyFont;
    u16 uiX, uiY;

    pygmyFont = lcdGetFont( );
    //lcdSetpygmyWidget->
    if( !( pygmyWidget->Style & PYGMY_VISIBLE ) ){
        return;
    } // if
    lcdSetColor( pygmyWidget->Color.B, pygmyWidget->Color.G, pygmyWidget->Color.R );
    lcdSetBackColor( pygmyWidget->BackColor.B, pygmyWidget->BackColor.G, pygmyWidget->BackColor.R );
    if( pygmyWidget->Style & PYGMY_BORDER ){
        
        drawRect( pygmyWidget->X, pygmyWidget->Y, pygmyWidget->X + pygmyWidget->Width,
            pygmyWidget->Y+pygmyWidget->Height, pygmyWidget->Style, 8 );
        
        if( pygmyWidget->String[0] ){
            if( pygmyWidget->Style & PYGMY_CENTERED ){
                uiX = ( ( pygmyWidget->Width - ( pygmyFont->Height * len( pygmyWidget->String ) ) ) / 2 );
                if( uiX > pygmyWidget->Width ){
                    uiX = 8;
                } // if
                uiY = ( pygmyWidget->Height - pygmyFont->Height ) / 2;
                if( uiY > pygmyWidget->Height ){
                    uiY = 8;
                } // if
            } // if
            lcdSetCursor( pygmyWidget->X + uiX, pygmyWidget->Y + uiY  );
            print( LCD, pygmyWidget->String );
        } else{
            
            lcdSetCursor( pygmyWidget->X, pygmyWidget->Y );
            print( LCD, "%d", pygmyWidget->Value );
        } // else
        //drawLine( pygmyWidget->X, pygmyWidget->Y, pygmyWidget
    } // if
}

/*void guiSetColor( u16 uiColor )
{
    
}

void guiSetBackColor( u16 uiColor )
{
    
}*/

void drawFill( u16 x, u16 y )
{
    if( lcdGetPixel(x,y) != 0 ){
        lcdDrawPixel(x,y);
        drawFill(x+1,y);
        drawFill(x-1,y);
        drawFill(x,y+1);
        drawFill(x,y-1);
    } 
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

    if( ulStyle & PYGMY_FILLED ){
        if( uiRadius && ( ulStyle & ( PYGMY_ROUNDED | PYGMY_CHAMFER ) ) ){
            for( i = iY1+uiRadius; i < ( iY2 - uiRadius) + 1; i++ ){
                drawLine( iX1, i, iX2, i, ulStyle ); 
            } // for
            for( i = 0, ii = 0; i < uiRadius; i++ ){
                drawLine( iX1 + uiRadius - ii, iY1 + i, iX2 - uiRadius + ii, iY1 + i, ulStyle );
                drawLine( iX1 + uiRadius - ii, iY2 - i, iX2 - uiRadius + ii, iY2 - i, ulStyle );
                if( ulStyle & PYGMY_CHAMFER ){
                    ++ii;
                } // if
            } // for 
        } else{
            for( i = iY1; i < iY2; i++ ){
                drawLine( iX1 + 1, i, iX2 - 1, i, ulStyle );
            } // for
        } // else    
    } 
    if( ( ulStyle & PYGMY_CHAMFER ) ){
        drawLine( iX2-uiRadius, iY1, iX2, iY1+uiRadius, ulStyle ); // top right
        drawLine( iX2-uiRadius, iY2, iX2, iY2-uiRadius, ulStyle ); // bottom right
        drawLine( iX1+uiRadius, iY2, iX1, iY2-uiRadius, ulStyle ); // bottom left
        drawLine( iX1, iY1+uiRadius, iX1+uiRadius, iY1, ulStyle ); // top left
    } // if
    if( ( ulStyle & PYGMY_ROUNDED ) ){ 
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
    
    if( ulStyle & PYGMY_FILLED ){
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
        if( ulStyle & PYGMY_FILLED ){
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

void setFontColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB )
{
    pygmyFont->Color.R = ucR;
    pygmyFont->Color.G = ucG;
    pygmyFont->Color.B = ucB;
}

void setFontBackColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB )
{
    pygmyFont->BackColor.R = ucR;
    pygmyFont->BackColor.G = ucG;
    pygmyFont->BackColor.B = ucB;
}

u8 setFont( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont )
{
    u32 ulPixelIndex;
	u16 uiPygmyInfo;
    
	pygmyFont->File.Sector      = pygmyFile->Sector;
    pygmyFont->File.Length      = pygmyFile->Length;
    pygmyFont->File.Index       = pygmyFile->Index;
    pygmyFont->File.ID_Sector   = pygmyFile->ID_Sector;
    pygmyFont->File.ID_File     = pygmyFile->ID_File;
    pygmyFont->File.Attributes  = pygmyFile->Attributes;
    pygmyFont->File.Resource    = pygmyFile->Resource;
    copyString( pygmyFile->Name, pygmyFont->File.Name );
    // Default White foreground color
    pygmyFont->Color.R = 0xFF;
    pygmyFont->Color.G = 0xFF;
    pygmyFont->Color.B = 0xFF;
    // Default Black background color
    pygmyFont->BackColor.R = 0x00;
    pygmyFont->BackColor.G = 0x00;
    pygmyFont->BackColor.B = 0x00;

    ulPixelIndex = getImage( pygmyFile, 0 );
    if( !ulPixelIndex ){			// Image format of index error, font unusable
		return( -1 );	            //
    } // if
    fileSetPosition( pygmyFile, START, ulPixelIndex );
    
	uiPygmyInfo = (u16) fileGetChar( pygmyFile ) * 0x0100;
	uiPygmyInfo |= fileGetChar( pygmyFile );
	
	if( uiPygmyInfo & PYGMY_PBM_16BITD  ){		// Determine 8 or 16 bit Width and Height fields
        fileSetPosition( pygmyFile, CURRENT, 3 );
        pygmyFont->Height = fileGetChar( pygmyFile ) * 0x0100;
	} else{
        fileSetPosition( pygmyFile, CURRENT, 1 );
        pygmyFont->Height = 0;
    } // else
	
    pygmyFont->Height |= fileGetChar( pygmyFile );
    lcdSetFont( pygmyFont );
  
    return( 1 );
}

/*void drawChar( u16 uiXPos, u16 uiYPos, PYGMYFONT *pygmyFont, u8 ucChar )
{
    lcdSetBPP( PYGMY_PBM_1BPP );
    lcdSetColor( pygmyFont->Color.R, pygmyFont->Color.G, pygmyFont->Color.B );
    lcdBackColor( pygmyFont->BackColor.R, pygmyFont->BackColor.G, pygmyFont->BackColor.B );
    drawImage( uiXPos, uiYPos, &pygmyFont->File, ucChar - 32 ); 
}*/

void drawChar( u8 ucChar )
{
    PYGMYFONT *pygmyFont;
    u16 uiX, uiY;
    
    uiX = lcdGetCursorX();
    uiY = lcdGetCursorY();
    pygmyFont = (PYGMYFONT*)lcdGetFont();
    uiX += 1 + drawImage( uiX, uiY, &pygmyFont->File, ucChar - 32 ); 
    
    lcdSetCursor( uiX, uiY );
}

void drawString( u8 *ucBuffer )
{
    PYGMYFONT *pygmyFont;
	u16 i, uiX, uiY;
	
    uiX = lcdGetCursorX();
    uiY = lcdGetCursorY();
    pygmyFont = (PYGMYFONT *)lcdGetFont();
	for( ; *ucBuffer; ){
		i = 1 + drawImage( uiX, uiY, &pygmyFont->File, *(ucBuffer++) - 32 );
        uiX += i;
	} // for
    lcdSetCursor( uiX, uiY );
}

/*void drawString( u16 uiXPos, u16 uiYPos, PYGMYFONT *pygmyFont, u8 *ucStr )
{
	u16 uiXOffset;
	
	for( uiXOffset = 0; *ucStr; ){
		uiXOffset += 1 + drawImage( uiXPos + uiXOffset, uiYPos, &pygmyFont->File, *(ucStr++) - 32 );
	} // for
}*/


/*void drawHex( u16 uiX, u16 uiY, PYGMYFONT *pygmyFont, u32 ulData, u16 uiDigits )
{
    u16 i;

    uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, '0' - 32 );
    uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, 'x' - 32 );
    for( i = uiDigits; i > 0; ){
        --i;
        //uiX += 1 + Pygmy_GUI_DrawChar( uiX, uiY, LCD_HEXCHARS[ ( ulData >> ( i * 4) ) & 0x0000000F ] );
        uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, PYGMY_HEXCHARS[ ( ulData >> ( i * 4) ) & 0x0000000F ] - 32 ); 
    } // for
	
}*/

/*void drawChar( u8 *ucBuffer, PYGMYFONT *pygmyFont, u16 uiXPos, u16 uiYPos, u8 ucChar )
{
  int i;
  //unsigned char *ucpFontChars;
  
  drawImage( ucBuffer, uiXPos, uiYPos, pygmyFont->ucpFont, ucChar - 32 ); 
  
}

void drawString( u8 *ucBuffer, PYGMYFONT *pygmyFont, u16 uiXPos, u16 uiYPos, u8 *ucStr )
{
	unsigned int uiXOffset;
	
	for( uiXOffset = 0; *ucStr; ){
		uiXOffset += 1 + drawImage( ucBuffer, uiXPos + uiXOffset, uiYPos, pygmyFont->ucpFont, *(ucStr++) - 32 );
	}
}

void drawHex( u8 *ucBuffer, PYGMYFONT *pygmyFont, u16 uiX, u16 uiY, u32 ulData, u16 uiDigits )
{
    u16 i;

    uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, '0' - 32 );
    uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, 'x' - 32 );
    for( i = uiDigits; i > 0; ){
        --i;
        //uiX += 1 + Pygmy_GUI_DrawChar( uiX, uiY, LCD_HEXCHARS[ ( ulData >> ( i * 4) ) & 0x0000000F ] );
        uiX += 1 + drawImage( ucBuffer, uiX, uiY, pygmyFont->ucpFont, PYGMY_HEXCHARS[ ( ulData >> ( i * 4) ) & 0x0000000F ] - 32 ); 
    } // for
	
}*/
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

/*void lcdDrawArea( u8 *ucBuffer, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{
	unsigned int uiAddress, i;
	unsigned char ucCOLMSB, ucCOLLSB;
	
	ucCOLMSB = uiX1 >> 4;
	ucCOLLSB = uiX1 & 0x0F;
	
	uiY1 =  uiY1 / 8;
	uiY2 =  1+ ( uiY2 / 8 );
	for( ; uiY1 < uiY2; uiY1++ ){
		lcdCommandWrite( LCD_CMD_ADDRPAGE | ( uiY1 ) ); 	// Increment through all 8 pages of 128 bytes 
		lcdCommandWrite( LCD_CMD_ADDRCOLMSB | ucCOLMSB ); 	// MSB of address goes into LSB of command, part1
		lcdCommandWrite( LCD_CMD_ADDRCOLLSB | ucCOLLSB ); 	// LSB of address goes into LSB of command, part2
		uiAddress = ( uiY1 * LCD_BYTESPERPAGE ) + uiX1;
		for( i = uiX1; i < uiX2; i++ ){
            lcdDataWrite( ucBuffer[ uiAddress++ ] );
		} // for
	} // for
}*/

void drawAll( u8 *ucBuffer )
{
    lcdWriteScreenBuffer();
}
/*	unsigned int i, ii, iii;
	
	// This is one of the few functions that relies directly on the driver interface functions
	// This function is expecting to see and SED memory layout and 128 x 64 1BPP
    
	for( ii = 0, iii = 0; ii < LCD_PAGES; ii++ ){
		lcdCommandWrite( LCD_CMD_ADDRPAGE | ii ); 	// Increment through all 8 pages of 128 bytes 
		lcdCommandWrite( LCD_CMD_ADDRCOLMSB ); 	// MSB of address goes into LSB of command, part1
		lcdCommandWrite( LCD_CMD_ADDRCOLLSB ); 	// LSB of address goes into LSB of command, part2
		for( i = 0; i < LCD_BYTESPERPAGE; i++ ){
            //LCD_Data_Write( ucVideoBuffer[ iii++ ] );
            lcdDataWrite( ucBuffer[ iii++ ] );
		} // for
	} // for
}*/
/*
void setPixel( u8 *ucBuffer, u16 uiX, u16 uiY, u16 uiColor )
{
	unsigned int uiAddress;
	
    lcdSetPixel( uiColor, uiX, uiY );
    
	//This function only implements the 1BPP at this time, color was stripped for simplicity
    // Stencil buffer removed to free RAM, stencil not used effectively with this GUI lib
	uiAddress = ( ( uiY/LCD_PAGES ) * LCD_BYTESPERPAGE ) + uiX;
    //if( ucBuffer ){
        //if( ucStencilBuffer[ uiAddress ] & LCD_BITMASKS[ uiY % 8 ] ) {
            if( uiColor ) 
                //ucVideoBuffer[ uiAddress ] |= LCD_BITMASKS[ uiY % 8 ];
                ucBuffer[ uiAddress ] |= LCD_BITMASKS[ uiY % 8 ];
            else
                ucBuffer[ uiAddress ] &= ~LCD_BITMASKS[ uiY % 8 ];
                //ucVideoBuffer[ uiAddress ] &= ~LCD_BITMASKS[ uiY % 8 ];
       // }
    
}*/

void drawObject( PYGMYGUIOBJECT *pygmyGUIObject )
{
   
}

void drawLine( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle )
{   
    s16 dy = iY2 - iY1;
    s16 dx = iX2 - iX1;
    s16 stepx, stepy, iStepSkip, iToggle, fraction;

    iStepSkip = 1;
    iToggle = 1;
    if( ulStyle & PYGMY_DOT ){
        iStepSkip = 2;
    } else if( ulStyle & PYGMY_DASH ){
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
    //drawRadius( uiX0, uiY0, 1, uiRadius, ulStyle );
    //drawRadius( uiX0, uiY0, 2, uiRadius, ulStyle );
    //drawRadius( uiX0, uiY0, 3, uiRadius, ulStyle );
    
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