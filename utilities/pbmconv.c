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

/**************************************************************************
	Pygmy Resource Format V1
	Mode BIT 12-15:	0 = Image, 1 = ImageStrip, 2 = Animation, 3 = Video, 4 = Audio, 5 = Raw Binary, 6 = Raw Text		
	Mode BIT 10-11:	0 = 8 bit table, 1 = 16 bit table, 2 = 32 bit table	
	Mode BIT 9: 	0 = Static Dimensions, 1 = Dimensions per slide
	Mode BIT 8: 	0 = 8 bit Height and Width, 1 = 16 bit Height and Width	
	Mode BIT 7-6: 	0 = Raw, 1 = RLE, 2 = Scatter Draw, 3 = Vector
	Mode BIT 5:		0 = no CRC, 1 = 32bit CRC at end of file
	Mode BIT 4-0:	0=1bpp, 1=4bpp, 2=8bpp, 3=12bpp, 4=16bpp, 5=18bpp, 6=24bpp, 7=32bpp
	Width and Height are stored Big Endian
	Bitmap origin (0,0) is Upper Left
	DATA is stored in a bit stream if Raw
	DATA is stored in packets if RLE compressed. Packet is structured as follows:
	DATA is not aligned with either width or height
	RLE packet will not extend past bitmap boundary, but Raw may
	1bpp
		BIT 7: 	Raw = 0, RLE = 1
		BIT 6: 	Pixel Value, 0 = OFF, 1 = ON
		if ( RLE )
			BIT 0-5: Repetition, 0 - 63 ( store as count -1, since count is reps it is always > 0 )
		else 
			BIT 0-6: Pixel Values 
	4bpp
		BIT 7: 	Raw = 0, RLE = 1
		BIT 6: 	Pixel Value, 0 = Transparent, 1 = ON
        BIT 0-5: Repetition, 0 - 63 
        Pixels are "nibble packed" into following bytes
        Pixels are left aligned on odd boundaries
	8bpp
        BIT 7: 	Raw = 0, RLE = 1
		BIT 6: 	Pixel Value, 0 = Transparent, 1 = ON
        BIT 5-0: Repetition, 0 - 63
		Bytes following store pixel data in format RRRGGBBB
    12bpp
	16bpp  
    18bpp
	24bpp
	32bpp
		BIT 7: Raw = 0, RLE = 1
		BIT 6: Pixel Value, 0 = Transparent, 1 = ON
		if ( RLE )
			BIT 0-5: Repetition, 0 - 63, one raw pixel following
		else 
			BIT 0-5: Number of raw pixels following
			
	Pygmy Bitmaps .pbm are defined as follows:
	
	Offset 0	MODE	16 bits
				Width	8 or 16 bits
				Height	8 or 16 bits
				If Entries > 0, 16 bit entry length followed by entry, data follows
				If NOT Image, 16 bit number of slides, 
				Width
				Height
	
	Offset END	32 bit CRC if selected
***********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"
/*
#define PBM_RLE             BIT6
#define PBM_1BPP            1
#define PBM_4BPP            2
#define PBM_8BPP            3
#define PBM_12BPP           4
#define PBM_16BPP           5
#define PBM_24BPP           6
#define PBM_32BPP           7
#define PBM_ARBITRARY       8*/

#define MAXIMAGEWIDTH       160
#define MAXIMAGEHEIGHT      160
#define MAXBYTESPERPIXEL    4

#define PYGMY_PBM_TYPEMASK				0xF000
#define PYGMY_PBM_IMAGE				    0x0000
#define PYGMY_PBM_IMAGESTRIP			0x1000
#define PYGMY_PBM_ANIMATION			    0x2000
#define PYGMY_PBM_VIDEO				    0x3000
#define PYGMY_PBM_AUDIO				    0x4000
#define PYGMY_PBM_BINARY				0x5000
#define PYGMY_PBM_TEXT					0x6000
#define PYGMY_PBM_EXECUTABLE			0x7000
#define PYGMY_PBM_BYTECODE				0x8000
#define PYGMY_PBM_LIBRARY				0x9000
#define PYGMY_PBM_GUI					0xA000
#define PYGMY_PBM_SCRIPT				0xB000
#define PYGMY_PBM_DRIVER				0xC000
#define PYGMY_PBM_ARCHIVE				0xD000
#define PYGMY_PBM_STREAM				0xE000
#define PYGMY_PBM_FONT  				0xF000	

#define PYGMY_PBM_TABLEMASK			    0x0300
#define PYGMY_PBM_TABLE16				0x0000
#define PYGMY_PBM_TABLE32				0x0400
#define PYGMY_PBM_DIMENSIONSIZEMASK	    0x0100
#define PYGMY_PBM_8BITD				    0x0000
#define PYGMY_PBM_16BITD				0x0100
#define PYGMY_PBM_DRAWSTYLEMASK		    0x00C0
#define PYGMY_PBM_RAW					0x0000
#define PYGMY_PBM_ALPHA				    0x0040
#define PYGMY_PBM_SCATTER				0x0080
#define PYGMY_PBM_VECTOR				0x00C0
#define PYGMY_PBM_CRCMASK				0x0020
#define PYGMY_PBM_CRC					0x0020
#define PYGMY_PBM_VERSIONMASK			0x0010
#define PYGMY_PBM_VERSION				0x0010
#define PYGMY_PBM_BPPMASK				0x000F
#define PYGMY_PBM_1BPP					0x0000
#define PYGMY_PBM_4BPP					0x0001
#define PYGMY_PBM_8BPP					0x0002
#define PYGMY_PBM_12BPP                 0x0003
#define PYGMY_PBM_16BPP				    0x0004
#define PYGMY_PBM_24BPP				    0x0005
#define PYGMY_PBM_32BPP				    0x0006

#define PYGMY_PBM_RLE                   BIT7
#define PYGMY_PBM_PIXELON               BIT6

#define PIXELON             1

const u32 PYGMY_BITMASKS[] = { BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11,BIT12,BIT13,BIT14,BIT15,
                              BIT16,BIT17,BIT18,BIT19,BIT20,BIT21,BIT22,BIT23,BIT24,BIT25,BIT26,BIT27,BIT28,BIT29,BIT30,BIT31};


typedef struct {
                u8 Buffer[ MAXIMAGEWIDTH * MAXIMAGEHEIGHT ][ MAXBYTESPERPIXEL ];
                u8 BPP;
                u8 RLE;
                u16 Width;
                u16 Height;
                } IMAGE_BM;

void loadBitmap( FILE *fileImage, IMAGE_BM *image );
void putPixelRaw( FILE *fileImage, u32 ulPixel, u16 uiFromBPP, u16 uiToBPP );
void putPixelRep( FILE *fileImage, u32 ulPixel, u16 uiFromBPP, u16 uiToBPP );                
void exportPBM( FILE *fileOut, IMAGE_BM *image, u16 uiBPP );
void convertBPP( IMAGE_BM *imageIn, IMAGE_BM *imageOut, u16 uiBPP );
void applyThreshhold( IMAGE_BM *image, u8 ucR, u8 ucG, u8 ucB );
void apply4BPP( IMAGE_BM *image );

int main( int argc, char* argv[] )
{
    FILE *fileIn, *fileOut;
    IMAGE_BM imageIn, imageOut;
    
    if( argc != 4 ){
        printf( "\nCorrect usage: filefrom fileto BitsPerPixel( 1bpp, 4bpp, 8bpp, 12bpp, 16bpp, 24bpp )" );
        exit( 0 );
    } //
    printf( "\nLoading: %s for output to: %s", argv[ 1 ], argv[ 2 ] );
    //exit( 0 );
    fileIn = fopen( argv[ 1 ], "rb" );
    if( !fileIn ){
        printf("\nError opening %s", argv[ 1 ] );
        exit( 0 );
    } // if
    fileOut = fopen( argv[ 2 ], "wb" );
    if( !fileOut ){
        printf("\nError opening %s", argv[ 2 ] );
        exit( 0 );
    } // if
    
    loadBitmap( fileIn, &imageIn );
    
    if( !strcmp( argv[ 3 ], "1bpp" ) || !strcmp( argv[ 3 ], "1BPP" ) ){
        applyThreshhold( &imageIn, 0x7F, 0x7F, 0x7F );
        exportPBM( fileOut, &imageIn, PYGMY_PBM_1BPP );
    } else if( !strcmp( argv[ 3 ], "4bpp" ) || !strcmp( argv[ 3 ], "4BPP" ) ){
        apply4BPP( &imageIn );
        exportPBM( fileOut, &imageIn, PYGMY_PBM_4BPP );
    } else if( !strcmp( argv[ 3 ], "8bpp" ) || !strcmp( argv[ 3 ], "8BPP" ) ){
        exportPBM( fileOut, &imageIn, PYGMY_PBM_8BPP );
    } else if( !strcmp( argv[ 3 ], "12bpp" ) || !strcmp( argv[ 3 ], "12BPP" ) ){
        exportPBM( fileOut, &imageIn, PYGMY_PBM_12BPP );
    } else if( !strcmp( argv[ 3 ], "16bpp" ) || !strcmp( argv[ 3 ], "16BPP" ) ){
        exportPBM( fileOut, &imageIn, PYGMY_PBM_16BPP );
    } else if( !strcmp( argv[ 3 ], "24bpp" ) || !strcmp( argv[ 3 ], "24BPP" ) ){
        exportPBM( fileOut, &imageIn, PYGMY_PBM_24BPP );
    } else{
        printf( "\nError: Bits Per Pixel not supported, or not specified" );
    }
    
    fclose( fileIn );
    fclose( fileOut );
}

void apply4BPP( IMAGE_BM *image )
{
    u32 i, ulLen, ulColor;
    
    ulLen = image->Height * image->Width;
    for( i = 0; i < ulLen; i++ ){
        ulColor = image->Buffer[ i ][ 0 ] + image->Buffer[ i ][ 1 ] + image->Buffer[ i ][ 2 ];
        image->Buffer[ i ][ 0 ] = ulColor / 48;
        image->Buffer[ i ][ 1 ] = image->Buffer[ i ][ 0 ];
        image->Buffer[ i ][ 2 ] = image->Buffer[ i ][ 0 ];
    } //for
}

void applyThreshhold( IMAGE_BM *image, u8 ucR, u8 ucG, u8 ucB )
{
    u32 i, ulLen;
    
    ulLen = image->Height * image->Width;
    for( i = 0; i < ulLen; i++ ){
        if( image->Buffer[ i ][ 0 ] > ucR || image->Buffer[ i ][ 1 ] > ucG || image->Buffer[ i ][ 2 ] > ucB ){
            image->Buffer[ i ][ 0 ] = 0xFF;
            image->Buffer[ i ][ 1 ] = 0xFF;
            image->Buffer[ i ][ 2 ] = 0xFF;
        } else{
            image->Buffer[ i ][ 0 ] = 0;
            image->Buffer[ i ][ 1 ] = 0;
            image->Buffer[ i ][ 2 ] = 0;
        } // else
    } // for
}

void convertBPP( IMAGE_BM *imageIn, IMAGE_BM *imageOut, u16 uiBPP )
{
    u32 i, ii;
    
    imageOut->BPP = uiBPP;
    imageOut->Height = imageIn->Height;
    imageOut->Width = imageIn->Width;
    for( i = 0, ii = 0; i < imageIn->Height * imageIn->Width; ){
            if( imageIn->BPP == 24 ){
                if( imageOut->BPP == 12 ){
                    imageOut->Buffer[ ii ][ 0 ] = 0xF0 & imageIn->Buffer[ i ][ 0 ];                
                    imageOut->Buffer[ ii ][ 0 ] |= 0x0F & ( imageIn->Buffer[ i ][ 1 ] >> 4 );
                    imageOut->Buffer[ ii ][ 1 ] = 0xF0 & imageIn->Buffer[ i ][ 2 ];                
                    imageOut->Buffer[ ii ][ 1 ] |= 0xF0 & imageIn->Buffer[ i + 1 ][ 0 ];
                    imageOut->Buffer[ ii ][ 2 ] = 0x0F & ( imageIn->Buffer[ i + 1 ][ 1 ] >> 4 );                
                    imageOut->Buffer[ ii ][ 2 ] |= 0x0F & imageIn->Buffer[ i + 1 ][ 2 ];
                    ++ii;
                    i+=2;
                } // if
            } // if
        
    } // for
}

void exportPBM( FILE *fileOut, IMAGE_BM *image, u16 uiBPP )
{
    // ToDo: Add second level of compression by preseeking the number of bytes NOT in an RLE run
    
    u32 i, ii, iii, ulPixel, ulLastPixel, ulLen;
    u16 uiPBMHeader, uiPixelRep, uiRawPixels;
    unsigned char ucBitMask[8] = { BIT7, BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
    
    uiPBMHeader = ( uiBPP );
    fputc( (u8)((u16)uiPBMHeader >> 8), fileOut );
    fputc( (u8)uiPBMHeader, fileOut );
    fputc( (u8)image->Height, fileOut );
    fputc( (u8)image->Width, fileOut );
    
    //image->BPP /= 8;
    ulLen = image->Width*image->Height;
    printf( "\nExporting BPP: %d", uiBPP );
    //for( i = image->Width; i < ulLen; ) {
    for( i = 0; i < ulLen; ) {
        ulLastPixel = (u32)image->Buffer[ i ][ 0 ] << 16;
        ulLastPixel |= (u32)image->Buffer[ i ][ 1 ] << 8;
        ulLastPixel |= (u32)image->Buffer[ i ][ 2 ];
        //++i;
		for( ii = 0, uiPixelRep = 0; i < ulLen; uiPixelRep++, ++i ) {// range 0-63	
            ulPixel = (u32)image->Buffer[ i ][ 0 ] << 16;
            ulPixel |= (u32)image->Buffer[ i ][ 1 ] << 8;
            ulPixel |= (u32)image->Buffer[ i ][ 2 ];
            
            if( ++ii == 65 || ulPixel != ulLastPixel ){
                break;
            } // if 
		} // for
        if( uiBPP == PYGMY_PBM_1BPP ){
            if( uiPixelRep > 7 ) {
                // RLE
                
                if( ulLastPixel  ){
                    printf("\nRLE %d ON", uiPixelRep );
                    fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut ); // BIT7 set for RLE, BIT6 set for pixel ON
                } else{
                    printf("\nRLE %d OFF", uiPixelRep );
                    fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE, fileOut ); //BIT7 set for RLE ON, BIT6 clear for pixel off/transparent
                } // else
            } else {
                // RAW
                for( ii = 1, i-=uiPixelRep, uiRawPixels = 0; ii < 8 && i < ulLen; i++, ii++){
                    ulPixel = (u32)image->Buffer[ i ][ 0 ] << 16;
                    ulPixel |= (u32)image->Buffer[ i ][ 1 ] << 8;
                    ulPixel |= (u32)image->Buffer[ i ][ 2 ];
                    if( ulPixel ){
                        uiRawPixels |= ucBitMask[ ii ];
                    } // if
                } // for
                printf("\nRaw 0x%X", uiRawPixels );
                fputc( ( u8 )uiRawPixels, fileOut ); 
            } // else
        } else if( uiBPP == PYGMY_PBM_4BPP ){
            // ToDo: Add 4BPP Export
            if( uiPixelRep ){
                printf( "\nRLE, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut );
                fputc( image->Buffer[ i-1 ][ 0 ] << 4, fileOut );    
            } else{
                printf( "\nRAW, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_PIXELON, fileOut );
                for( ii = 0, i -= uiPixelRep; ii < ( ( uiPixelRep / 2 ) * 2 ) && i < ulLen; ){
                    fputc( (image->Buffer[ i++ ][ 0 ] << 4) | ( image->Buffer[ i++ ][ 0 ] & 0x0F ), fileOut );
                    ii += 2;
                } // for
                if( ii < uiPixelRep ){
                    fputc( (image->Buffer[ i++ ][ 0 ] << 4 ), fileOut ); 
                } // if 
            } // else
        } else if( uiBPP == PYGMY_PBM_8BPP ){
            if( uiPixelRep  ){
                printf( "\nRLE, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut );
                fputc( ((ulLastPixel >> 16) & 0xE0 ) | (((ulLastPixel >> 8) & 0xC0 ) >> 3) | (( ulLastPixel & 0xE0) >> 5), fileOut );
            } else{ 
                printf( "\nRAW, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_PIXELON, fileOut );
                for( ii = 0, i -= uiPixelRep; ii < uiPixelRep && i < ulLen; ii++, i++ ){
                    //fputc( (image->Buffer[ i ][ 0 ]<<5)|(image->Buffer[ i ][ 1 ]&, fileOut );
                    fputc( (image->Buffer[ i ][ 0 ] & 0xE0 ) | ((image->Buffer[ i ][ 1 ] & 0xC0 ) >> 3) | 
                        (( image->Buffer[ i ][ 0 ] & 0xE0) >> 5), fileOut );
                } // for
            } // else
        } else if( uiBPP == PYGMY_PBM_12BPP ){
            if( uiPixelRep  ){
                printf( "\nRLE, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut );
                fputc( (ulLastPixel>>20)&0x000F , fileOut );
                fputc( ((ulLastPixel>>8)&0x00F0)|((ulLastPixel>>4)&0x000F), fileOut );
            } else{ 
                printf( "\nRAW, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_PIXELON, fileOut );
                for( ii = 0, i -= uiPixelRep; ii < ( ( uiPixelRep / 2 ) * 2 ) && i < ulLen; ){
                    fputc( (image->Buffer[ i ][ 0 ] & 0xF0 ) | ( (image->Buffer[ i ][ 1 ]>>4) & 0x0F ), fileOut );
                    fputc( (image->Buffer[ i ][ 2 ] & 0xF0 ) | ( (image->Buffer[ i+1 ][ 0 ]>>4) & 0x0F ), fileOut ); 
                    fputc( (image->Buffer[ i+1 ][ 1 ] & 0xF0 ) | ( (image->Buffer[ i+1 ][ 2 ]>>4) & 0x0F ), fileOut );
                    ii += 2;
                    i += 2;
                } // for
                if( ii < uiPixelRep ){
                    fputc( (image->Buffer[ i ][ 0 ] & 0xF0 ) | ( (image->Buffer[ i ][ 1 ]>>4) & 0x0F ), fileOut );
                    fputc( (image->Buffer[ i ][ 2 ] & 0xF0 ), fileOut ); 
                    ++i;
                } // if 
            } // else    
        } else if( uiBPP == PYGMY_PBM_16BPP ) {
            if( uiPixelRep   ){
                printf( "\nRLE, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut );// 
                //fputc( ((ulLastPixel >> 16 ) & 0xF8 ) | (ulLastPixel >> 13) , fileOut );
                //fputc( (((ulLastPixel >> 8 ) & 0x1C ) << 3 ) | (( ulLastPixel >> 3 ) & 0x1F ) , fileOut );
                //fputc( ((ulLastPixel>>16) & 0xF8 ) | ((ulLastPixel>>8) >> 5 ) , fileOut );
                //fputc( (((ulLastPixel>>8) & 0x1C ) << 3 ) | (( ulLastPixel >> 3 ) & 0x1F ) , fileOut );
                fputc( (image->Buffer[ i-1 ][ 0 ] & 0xF8 ) | (image->Buffer[ i-1 ][ 1 ] >> 5 ) , fileOut );
                fputc( ((image->Buffer[ i-1 ][ 1 ] & 0x1C ) << 3 ) | (( image->Buffer[ i-1 ][ 2 ] >> 3 ) & 0x1F ) , fileOut );
            } else{ 
                printf( "\nRAW, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_PIXELON, fileOut );//
                for( ii = 0, i -= uiPixelRep; ii < uiPixelRep && i < ulLen; ii++, i++ ){
                    fputc( (image->Buffer[ i ][ 0 ] & 0xF8 ) | (image->Buffer[ i ][ 1 ] >> 5 ) , fileOut );
                    fputc( ((image->Buffer[ i ][ 1 ] & 0x1C ) << 3 ) | (( image->Buffer[ i ][ 2 ] >> 3 ) & 0x1F ) , fileOut );
                    //fputc( ((image->Buffer[ i ][ 0 ] & 0x1F ) << 3 ) | ((image->Buffer[ i ][ 1 ] & 0x3F ) >> 3 ) , fileOut );
                    //fputc( (image->Buffer[ i ][ 1 ]  << 5 ) | ( image->Buffer[ i ][ 2 ] & 0x1F ) , fileOut );
                } // for
            } // else
        } else if( uiBPP == PYGMY_PBM_24BPP ) {
            if( uiPixelRep  ){
                printf( "\nRLE, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_RLE | PYGMY_PBM_PIXELON, fileOut );
                
                
                fputc( ulLastPixel, fileOut );
				fputc( ulLastPixel >> 8, fileOut );
				fputc( ulLastPixel >> 16, fileOut );
            } else{ 
                printf( "\nRAW, Reps: %d", uiPixelRep );
                fputc( (u8)(uiPixelRep-1) | PYGMY_PBM_PIXELON, fileOut );
                for( ii = 0, i -= uiPixelRep; ii < uiPixelRep && i < ulLen; ii++, i++ ){
                    fputc( image->Buffer[ i ][ 2 ], fileOut );
                    fputc( image->Buffer[ i ][ 1 ], fileOut );
                    fputc( image->Buffer[ i ][ 0 ], fileOut );
                } // for
            } // else
        } // else if
            
        
        //} // else
	} // for		
}

void putPixelRep( FILE *fileImage, u32 ulPixel, u16 uiFromBPP, u16 uiToBPP )
{
    
}

void putPixelRaw( FILE *fileImage, u32 ulPixel, u16 uiFromBPP, u16 uiToBPP )
{
    
}

void loadBitmap( FILE *fileImage, IMAGE_BM *image )
{
    u16 i, ii, iii, uiPadding;
    u32 uiData;

    if( fgetc( fileImage ) != 'B' || fgetc( fileImage ) != 'M' ){
        printf( "\nInvalid source image!" );
        return;
    } // if 
    for( i = 0; i < 8; i++ ){
        fgetc( fileImage );
    } // for
    uiData = fgetc( fileImage );
    uiData |= (u32)fgetc( fileImage ) << 8;
    uiData |= (u32)fgetc( fileImage ) << 16;
    uiData |= (u32)fgetc( fileImage ) << 24;
    for( i = 0; i < 4; i++ ){
        fgetc( fileImage );
    } // for
    image->Width = fgetc( fileImage );
    image->Width |= (u32)fgetc( fileImage ) << 8;
    image->Width |= (u32)fgetc( fileImage ) << 16;
    image->Width |= (u32)fgetc( fileImage ) << 24;
    image->Height = fgetc( fileImage );
    image->Height |= (u32)fgetc( fileImage ) << 8;
    image->Height |= (u32)fgetc( fileImage ) << 16;
    image->Height |= (u32)fgetc( fileImage ) << 24;
    fgetc( fileImage ); // Skip 2 bytes for Planes used
    fgetc( fileImage ); //
    image->BPP = fgetc( fileImage );
    image->BPP |= (u32)fgetc( fileImage ) << 8;
    image->RLE = fgetc( fileImage ); // Ignore the next three bytes
    
    

    fseek( fileImage, uiData, SEEK_SET );
    uiPadding = image->Width % 4;
    printf( "\nLoaded BM Height: %d Width: %d BPP: %d Offset: %d Pad: %d", image->Height, image->Width, image->BPP, uiData, uiPadding );
    for( i = 0; i < image->Height; i++ ){
        for( ii = 0; ii < image->Width; ii++ ){
            image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 2 ] = fgetc( fileImage );
            image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 1 ] = fgetc( fileImage );
            image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 0 ] = fgetc( fileImage );
            //printf( "\nLoaded pixel R: %d G: %d B: %d",image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 0 ], 
            //    image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 1 ],image->Buffer[ ( ((image->Height-1)-i) * image->Width ) + ii ][ 2 ]); 
        } // for  
        for( ii = 0; ii < uiPadding; ii++ ){
            fgetc( fileImage );
        } // for
    } // for
}
