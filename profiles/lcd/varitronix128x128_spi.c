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

//********************************************************************
//*************************Varitronix 128x128*************************
//*************************Samsung S6B3306X11*************************
//********************************************************************
// Standard Instruction Set
#define LCD_NOP                 0x00
#define LCD_OSCILLATION         0x02
#define LCD_DRIVEROUTPUT        0x10
#define LCD_MONITORSIGNAL       0x18
#define LCD_TEMPCOMPENSATION    0x28
#define LCD_CONTRAST            0x2A
#define LCD_STANDBYOFF          0x2C
#define LCD_STANDBYON           0x2D
#define LCD_ADDRESSMODE         0x30
#define LCD_ROWVECTOR           0x32
#define LCD_NBLOCKINVERSION     0x34
#define LCD_DRIVINGMODE         0x36
#define LCD_ENTRYMODE           0x40
#define LCD_ROWAREA             0x42
#define LCD_COLUMNAREA          0x43
#define LCD_RAMSKIP             0x45
#define LCD_DISPLAYOFF          0x50
#define LCD_DISPLAYON           0x51
#define LCD_DISPLAYPATTERN      0x53
#define LCD_PARTIALDISPLAYMODE  0x55
#define LCD_PARTIALDISPLAYSTART 0x56
#define LCD_PARTIALDISPLAYEND   0x57
#define LCD_BOOSTRATIO          0x70
#define LCD_FRAMEFREQUENCY      0x7F
#define LCD_PRELIMINARY         0x8C
#define LCD_CIDREADON           0xDA
#define LCD_CIDREADOFF          0xDB
#define LCD_MTPLOAD             0xE5
#define LCD_MTPREAD             0xE6
#define LCD_MTPINITIALDISABLE   0xE8
#define LCD_MTPINITIALENABLE    0xE9
#define LCD_MTPSELECTOFF        0xEA
#define LCD_MTPSELECTON         0xEB
#define LCD_OFFSETVOLUME        0xED
#define LCD_MTPWRITEENABLE      0xEE
#define LCD_MTPWRITEDISABLE     0xEF

#define LCD_ROWS	            128
#define LCD_COLS	            128
#define LCD_SCREENBUFFER_LEN    0
#define LCD_WIDTH               LCD_COLS
#define LCD_HEIGHT              LCD_ROWS
// !!! Note, LCD_CS, LCD_A0, LCD_RESET, LCD_SCK, LCD_MOSI
// !!!   must be defined in the local copy of pygmy_profile.h

PYGMYGUI pygmyGlobalColors;
PYGMYSPIPORT pygmyLCD;

void lcdWriteAddress( u8 ucX1, u8 ucY1, u8 ucX2, u8 ucY2 )
{
    pinSet( LCD_A0, LOW );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    spiWriteByte( &pygmyLCD, LCD_ROWAREA );
    spiWriteByte( &pygmyLCD, ucX1 );
    spiWriteByte( &pygmyLCD, ucX2 );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS;
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS;
    spiWriteByte( &pygmyLCD, LCD_COLUMNAREA );
    spiWriteByte( &pygmyLCD, ucY1 );
    spiWriteByte( &pygmyLCD, ucY2 );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdWriteCommand( u16 uiCommand )
{
    pinSet( LCD_A0, LOW );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    spiWriteByte( &pygmyLCD, (u8)uiCommand );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}


void lcdWriteCommandAndParam( u16 uiCommand, u16 uiParam )
{
    pinSet( LCD_A0, LOW );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    spiWriteByte( &pygmyLCD, (u8)uiCommand );
    spiWriteByte( &pygmyLCD, (u8)uiParam );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdWriteData( u16 uiData )
{
    pinSet( LCD_A0, HIGH );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    spiWriteWord( &pygmyLCD, uiData );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdBacklightOn( void )
{
    pinSet( LCD_BL, HIGH );
}

void lcdBacklightOff( void )
{
    pinSet( LCD_BL, LOW );
}

u8 putsLCD( u8 *ucBuffer )
{
    for( ; *ucBuffer ; ){
        drawChar( *(ucBuffer++) );
    } // for

    return( 1 );
}

u16 lcdGetWidth( void )
{
    return( LCD_WIDTH );
}

u16 lcdGetHeight( void )
{
    return( LCD_HEIGHT );
}

void lcdInit( void )
{	
    spiConfig( &pygmyLCD, LCD_CS, LCD_SCK, NONE, LCD_MOSI );
    pinConfig( LCD_RESET, OUT ); // We will use USART1_RTS as CS
    pinConfig( LCD_A0, OUT );
    pinConfig( LCD_BL, OUT );
    pinSet( LCD_BL, HIGH );
   
    pinSet( LCD_RESET, LOW );
    delay( 500 ); // microseconds
    pinSet( LCD_RESET, HIGH );
    delay( 8000 ); // microseconds
    //lcdWriteCommandAndParam( 0x02, 0x00 );// internal clock mode ***
    lcdWriteCommand( 0x2C );// STANDBY OFF
    delay( 500 );
    lcdWriteCommandAndParam( 0x2a, 0x2E );// Contrast Control 1  ***
    lcdWriteCommandAndParam( 0x53, 0x03 );// Specified Display Pattern Set, 0x03 ( original 0x80 )
    lcdWriteCommandAndParam( 0x70, 0x14 ); // Booster Boosting Set  ***
    lcdWriteCommandAndParam( 0x7F, 0x50 );// frame frequency set ***
    lcdWriteCommand( 0x51 );//Display ON
    
    delay( 65500 );
    delay( 65500 );
    delay( 65500 );
    lcdWriteCommandAndParam( 0x10, 0x03 );// 0x03 );// Driver Output Mode Set
    lcdWriteCommandAndParam( 0x28, 0x00 );// Temperature Compensation Set
    
    lcdWriteCommandAndParam( 0x30, 0x00 );// address mode set   ***
    lcdWriteCommandAndParam( 0x32, 0x06 );//Row Vector Mode Set  ***
    lcdWriteCommandAndParam( 0x34, 0x07 );// N-block inversion Set  ***
    lcdWriteCommandAndParam( 0x36, 0x00 );// Driving Mode Set
    lcdWriteCommandAndParam( 0x40, 0x00 );// Entry Mode Set //00:16 bit 80:8bit
    lcdWriteCommandAndParam( 0x45, 0x00 );// RAM Skip Area Set:
    
    lcdWriteCommandAndParam( 0x55, 0x00 );// Partial Display Mode Set, 0x01
    lcdWriteCommandAndParam( 0x56, 0x00 );// Partial Display Start Line Se
    lcdWriteCommandAndParam( 0x57, 0x7F );// Partial Display end Line Set
    lcdWriteCommandAndParam( 0x70, 0x14 ); // Booster Boosting Set  ***
    lcdWriteCommandAndParam( 0x7F, 0x50 );// frame frequency set ***
    //lcdWriteCommand( 0x51 );//Display ON
    /*lcdWriteCommandAndParam( LCD_OSCILLATION, 0x00 ); //(0x02);// internal clock mode
    lcdWriteCommand( LCD_STANDBYOFF );
    lcdWriteCommandAndParam( LCD_DRIVEROUTPUT, 0x03 ); //Driver Output Mode Set
    lcdWriteCommandAndParam( LCD_TEMPCOMPENSATION, 0x00 ); //(0x28);//Temperature Compensation Set
    lcdWriteCommandAndParam( LCD_CONTRAST, 0x2E ); //(0x2a);//Contrast Control 1 
    lcdWriteCommandAndParam( LCD_ADDRESSMODE, 0x0B ); //(0x30);//  address mode set
    lcdWriteCommandAndParam( LCD_ROWVECTOR, 0x06 ); //(0x32);//Row Vector Mode Set 
    lcdWriteCommandAndParam( LCD_NBLOCKINVERSION, 0x07 );//(0x34);//N-block inversion Set
    lcdWriteCommandAndParam( LCD_DRIVINGMODE, 0x00 );//(0x36);// Driving Mode Set
    lcdWriteCommandAndParam( LCD_ENTRYMODE, 0x80 ); //(0x40);// Entry Mode Set
    lcdWriteCommandAndParam( LCD_RAMSKIP, 0x00 ); //(0x45);// RAM Skip Area Set:
    lcdWriteCommandAndParam( LCD_DISPLAYPATTERN, 0x80 ); //(0x53);//Specified Display Pattern Set
    lcdWriteCommandAndParam( LCD_PARTIALDISPLAYMODE, 0x00 ); //(0x55);//Partial Display Mode Set
    lcdWriteCommandAndParam( LCD_PARTIALDISPLAYSTART, 0x00 ); //(0x56);//Partial Display Start Line Set
    lcdWriteCommandAndParam( LCD_PARTIALDISPLAYEND, 0x83 ); //(0x57);// Partial Display end Line Set 0x83
    lcdWriteCommandAndParam( LCD_BOOSTRATIO, 0x14 ); //(0x70); //Booster Boosting Set
    lcdWriteCommandAndParam( LCD_FRAMEFREQUENCY, 0x50 ); //(0x7f);//frame frequency set
    lcdWriteCommand( LCD_DISPLAYON );//(0x51);//Display ON
    */ 
    //lcdSetContrast( 0x2E );
    lcdSetBPP( PYGMY_PBM_16BPP );
    lcdSetColor( 0x00, 0x00, 0x00 );
    lcdBackColor( 0xFF, 0xFF, 0xFF );
    //lcdClear();
}

u8 lcdGetContrast( void )
{
    return( pygmyGlobalColors.Contrast );
}

void lcdSetContrast( u8 ucContrast )
{
    pygmyGlobalColors.Contrast = ucContrast;
    lcdWriteCommandAndParam( LCD_CONTRAST, pygmyGlobalColors.Contrast );
}

void lcdDrawPixel( u16 uiX, u16 uiY )
{
    u16 uiColor;
    
    if( uiX >= LCD_WIDTH || uiY >= LCD_HEIGHT ){
        return;
    } // if
    
    uiColor = pygmyGlobalColors.Color.R << 11;
    uiColor |= pygmyGlobalColors.Color.G << 5;
    uiColor |= pygmyGlobalColors.Color.B;
    lcdWriteAddress( uiX, uiY, uiX, uiY );
    lcdWriteData( uiColor );
    //pinSet( LCD_A0, HIGH );
    //pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    //spiWriteWord( &pygmyLCD, uiColor );
    //pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdClearPixel( u16 uiX, u16 uiY )
{
    u16 uiColor;

    if( uiX >= LCD_WIDTH || uiY >= LCD_HEIGHT ){
        return;
    } // if

    uiColor = pygmyGlobalColors.BackColor.R << 11;
    uiColor |= pygmyGlobalColors.BackColor.G << 5;
    uiColor |= pygmyGlobalColors.BackColor.B;
    lcdWriteAddress( uiX, uiY, uiX, uiY );
    lcdWriteData( uiColor );
    //pinSet( LCD_A0, HIGH );
    //pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    //spiWriteWord( &pygmyLCD, uiColor );
    //pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdDrawArea( u16 uiX1, u16 uiX2, u16 uiY1, u16 uiY2 )
{

}

void lcdWriteScreenBuffer( void )
{
   
}

void *lcdGetFont( void )
{
    return( pygmyGlobalColors.Font );
}

void lcdSetFont( void *pygmyFont )
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

void lcdOn( void )
{
    lcdWriteCommand( LCD_DISPLAYON );
}

void lcdOff( void )
{
    lcdWriteCommand( LCD_DISPLAYOFF );
}

void lcdSetBPP( u8 ucBPP )
{
    pygmyGlobalColors.BPP = ucBPP;
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

void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{ 
    u16 i;

    if( uiX1 > uiX2 ){
        i = uiX1;
        uiX1 = uiX2;
        uiX2 = i;
    } // if
    if( uiY1 > uiY2 ){
        i = uiY1;
        uiY1 = uiY2;
        uiY2 = i;
    } // if
    
    for( ; uiY1 < uiY2; uiY1++ ){
        for( i = uiX1; i < uiX2; i++ ){
            //if( pygmyGlobalColors.BackColor.R ){
            //    globalScreenBuffer[ ( uiX1 * 6 ) + ( uiY1 / 8 ) ] |= PYGMY_BITMASKS[ uiY1 % 8 ];
            //} else{
            //    globalScreenBuffer[ ( uiX1 * 6 ) + ( uiY1 / 8 ) ] |= PYGMY_BITMASKS[ uiY1 % 8 ];
            //} // else
        } // for
    } // for
}

void lcdClear( void )
{
    u16 i, uiColor;

    uiColor = pygmyGlobalColors.BackColor.R << 11;
    uiColor |= pygmyGlobalColors.BackColor.G << 5;
    uiColor |= pygmyGlobalColors.BackColor.B;

    lcdWriteAddress( 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1 );
    //lcdWriteAddress( 0, 0, LCD_WIDTH, LCD_HEIGHT );
    //pinSet( LCD_A0, HIGH );
    //pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip 
    
    for( i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++ ){
        lcdWriteData( uiColor );
        //spiWriteWord( &pygmyLCD, uiColor );
    } // for
    //pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

