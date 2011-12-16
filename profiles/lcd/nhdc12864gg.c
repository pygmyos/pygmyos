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

//************************************************************************************
//*******************************New Haven nhd-c12864gg*******************************
//**********************************ST7565 Controller*********************************

// ST7565
#define LCD_PAGES			8
#define LCD_BYTESPERPAGE	128
#define LCD_BITSPERPIXEL	1
#define LCD_WIDTH			128
#define LCD_HEIGHT			64
#define LCD_CMD_ON			0xAF
#define LCD_CMD_OFF			0xAE
#define LCD_CMD_SCANREVERSE	0xC8
#define LCD_CMD_SCANNORMAL	0xC0
#define LCD_CMD_ADCNORMAL	0xA0
#define LCD_CMD_ADCREVERSE	0xA1
#define LCD_CMD_BIAS17		0xA2
#define LCD_CMD_BIAS19		0xA3
#define LCD_CMD_DISPALLOFF	0xA4
#define LCD_CMD_DISPALLON	0xA5	
#define LCD_CMD_DISPNORMAL	0xA6
#define LCD_CMD_DISPINVERT	0xA7
#define LCD_CMD_ADDRROW		0x40	// Lower 6 bits is Line Address
#define LCD_CMD_ADDRPAGE	0xB0	// Lower Nibble is Page Address
#define LCD_CMD_ADDRCOLMSB	0x10	// Lower Nibble is Most Significant Nibble of Column Address
#define LCD_CMD_ADDRCOLLSB	0x00	// Lower Nibble is Least Significant Nibble of Column Address


PYGMYPARALLELPORT pygmyLCD;
u8 pygmyGlobalVideoBuffer[ LCD_PAGES * LCD_BYTESPERPAGE ];
PYGMYGUI pygmyGlobalColors;

void lcdInit( void )
{
    // LCD_RESET, LCD_CS, LCD_A0, LCD_WR, LCD_RD, LCD_D0 ( LSB of data bus ), 
    //  must all be defined in board profile included in pygmy_profile.h
    
    parallelConfig( &pygmyLCD, 8, LCD_CS, LCD_A0, LCD_WR, LCD_RD, LCD_D0 );
    
    pinConfig( LCD_RESET, OUT );
    setPin( LCD_RESET, LOW );
    delay( 100 );
    setPin( LCD_RESET, HIGH );
    delay( 500 );
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADCNORMAL );
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_OFF );
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_SCANREVERSE );
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_BIAS17 );
    parallelWrite( &pygmyLCD, COMMAND, 0x2F ); // Operating mode
    parallelWrite( &pygmyLCD, COMMAND, 0x26 ); // Resistor ratio
    parallelWrite( &pygmyLCD, COMMAND, 0x81 ); // Volume
    parallelWrite( &pygmyLCD, COMMAND, 0x0C ); // Volume value
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ON );
    lcdSetBPP( PYGMY_PBM_1BPP );
    lcdSetColor( 0xFF, 0xFF, 0xFF );
    lcdBackColor( 0x00, 0x00, 0x00 );
    lcdClear();
}

void lcdOn( void )
{
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ON );
}

void lcdOff( void )
{
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_OFF );
}


void lcdDrawPixel( u16 uiX, u16 uiY )
{
    u16 uiAddress;
    
    
	uiAddress = ( ( uiY/LCD_PAGES ) * LCD_BYTESPERPAGE ) + uiX;
    if( pygmyGlobalColors.Color.R ){ 
        pygmyGlobalVideoBuffer[ uiAddress ] |= PYGMY_BITMASKS[ uiY % 8 ];
    } else{
        pygmyGlobalVideoBuffer[ uiAddress ] &= ~PYGMY_BITMASKS[ uiY % 8 ];
    } // else

    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRPAGE | ( uiY / 8 ) ); 	    // Page
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLMSB | ( uiX >> 4 ) ); 	// MSB of address goes into LSB of command, part1
	parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLLSB | ( uiX & 0x0F ) ); 	// LSB of address goes into LSB of command, part2

    parallelWrite( &pygmyLCD, DATA, pygmyGlobalVideoBuffer[ uiAddress ] );
}

void lcdClearPixel( u16 uiX, u16 uiY )//, u8 ucBPP, u8 ucR, u8 ucG, u8 ucB )
{
    u16 uiAddress;
    
    
	uiAddress = ( ( uiY/LCD_PAGES ) * LCD_BYTESPERPAGE ) + uiX;
    if( pygmyGlobalColors.BackColor.R ){ 
        pygmyGlobalVideoBuffer[ uiAddress ] |= PYGMY_BITMASKS[ uiY % 8 ];
    } else{
        pygmyGlobalVideoBuffer[ uiAddress ] &= ~PYGMY_BITMASKS[ uiY % 8 ];
    } // else

    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRPAGE | ( uiY / 8 ) ); 	    // Page
    parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLMSB | ( uiX >> 4 ) ); 	// MSB of address goes into LSB of command, part1
	parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLLSB | ( uiX & 0x0F ) ); 	// LSB of address goes into LSB of command, part2

    parallelWrite( &pygmyLCD, DATA, pygmyGlobalVideoBuffer[ uiAddress ] );
}

PYGMYFONT *lcdGetFont( void )
{
    return( pygmyGlobalColors.Font );
}

void lcdSetFont( PYGMYFONT *pygmyFont )
{
   pygmyGlobalColors.Font = pygmyFont;   
}

void lcdSetCursor( u16 uiX, u16 uiY )
{
    pygmyGlobalColors.Cursor.X = uiX;
    pygmyGlobalColors.Cursor.Y = uiY;
}

u16 lcdGetCursorX( void )
{
    return( pygmyGlobalColors.Cursor.X );
}

u16 lcdGetCursorY( void )
{
    return( pygmyGlobalColors.Cursor.Y );
}

void lcdSetBPP( u8 ucBPP )
{
    pygmyGlobalColors.BPP = ucBPP;
}

void lcdSetColor( u8 ucR, u8 ucG, u8 ucB )
{
    if( pygmyGlobalColors.BPP == PYGMY_PBM_8BPP ){
        ucR *= 2;
        ucG *= 3;
        ucB *= 2;
    } else if( pygmyGlobalColors.BPP == PYGMY_PBM_16BPP ){
        ucR >>= 1;
        ucG >>= 2;
        ucB >>= 1;
    } else if( pygmyGlobalColors.BPP == PYGMY_PBM_24BPP ){
        ucR >>= 4;
        ucG >>= 4;
        ucB >>= 4;
    } // if
    pygmyGlobalColors.Color.R = ucR;
    pygmyGlobalColors.Color.G = ucG;
    pygmyGlobalColors.Color.B = ucB;
}

void lcdBackColor( u8 ucR, u8 ucG, u8 ucB )
{
    if( pygmyGlobalColors.BPP == PYGMY_PBM_8BPP ){
        ucR *= 2;
        ucG *= 3;
        ucB *= 2;
    } else if( pygmyGlobalColors.BPP == PYGMY_PBM_16BPP ){
        ucR >>= 1;
        ucG >>= 2;
        ucB >>= 1;
    } else if( pygmyGlobalColors.BPP == PYGMY_PBM_24BPP ){
        ucR >>= 4;
        ucG >>= 4;
        ucB >>= 4;
    } // if
    pygmyGlobalColors.BackColor.R = ucR;
    pygmyGlobalColors.BackColor.G = ucG;
    pygmyGlobalColors.BackColor.B = ucB;
}

void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{
    u16 i, uiAddress;
    u8 ucMSB, ucLSB;

    ucMSB = LCD_CMD_ADDRCOLMSB |( uiX1 >> 4 );
    ucLSB = LCD_CMD_ADDRCOLLSB |( uiX1 & 0x0F );
    for( ; uiY1 < uiY2; ){
        uiAddress = ( ( uiY1/LCD_PAGES ) * LCD_BYTESPERPAGE ) + uiX1;
        parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRPAGE | ( uiY1 / 8 ) ); 	    // Page
        parallelWrite( &pygmyLCD, COMMAND, ucMSB  ); 	// MSB of address goes into LSB of command, part1
        parallelWrite( &pygmyLCD, COMMAND, ucLSB  ); 	// LSB of address goes into LSB of command, part2

        for( i = uiY1 % 8; i < 8 && uiY1 < uiY2; i++, uiY1++ ){
            if( pygmyGlobalColors.BackColor.R ){ 
                pygmyGlobalVideoBuffer[ uiAddress ] |= PYGMY_BITMASKS[ i ];
            } else{
                pygmyGlobalVideoBuffer[ uiAddress ] &= ~PYGMY_BITMASKS[ i ];
            } // else
        } // for
        
        for( i = uiX1; i < uiX2; i++ ){
            parallelWrite( &pygmyLCD, DATA, pygmyGlobalVideoBuffer[ uiAddress ] );
        } // for
    } // for

}

void lcdClear( void )
{
    lcdClearArea( 0, 0, 127, 63 );
    
    /*u32 i, ii, iii;
    u8 ucColor;

    if( pygmyGlobalColors.BackColor.R ){
        ucColor = 0xFF;
    } else{
        ucColor = 0;
    } // else
    for( ii = 0, iii = 0; ii < LCD_PAGES; ii++ ){
        parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRPAGE | ii ); 	// Increment through all 8 pages of 128 bytes 
        parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLMSB ); 	// MSB of address goes into LSB of command, part1
        parallelWrite( &pygmyLCD, COMMAND, LCD_CMD_ADDRCOLLSB ); 	
        for( i = 0; i < LCD_BYTESPERPAGE; i++ ){
            parallelWrite( &pygmyLCD, DATA, ucColor );
            
            pygmyGlobalVideoBuffer[ iii++ ] = ucColor;
		} // for 
    } // for
    */
}

