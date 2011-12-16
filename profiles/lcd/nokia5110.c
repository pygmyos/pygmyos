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
//****************************Nokia 84x48*****************************
//*************************Philips PCD8544************************
//********************************************************************
// Standard Instruction Set
#define LCD_NOP                 0
#define LCD_FUNCSET             BIT5
    #define LCD_FUNCSET_H       BIT0
    #define LCD_FUNCSET_V       BIT1
    #define LCD_FUNCSET_PD      BIT2
#define LCD_DISPCONTROL         BIT3
    #define LCD_DISPCONTROL_E   BIT0
    #define LCD_DISPCONTROL_D   BIT2
#define LCD_SETY                BIT6
#define LCD_SETX                BIT7
// Extended Instruction Set
#define LCD_TEMPCONTROL         BIT2
    #define LCD_TEMPCONTROL_TC0 BIT0
    #define LCD_TEMPCONTROL_TC1 BIT1
#define LCD_BIAS                BIT4
    #define LCD_BIAS_BS0        BIT0
    #define LCD_BIAS_BS1        BIT1
    #define LCD_BIAS_BS2        BIT2
#define LCD_SETVOP              BIT7
    #define LCD_SETVOP_VOP0     BIT0
    #define LCD_SETVOP_VOP1     BIT1
    #define LCD_SETVOP_VOP2     BIT2
    #define LCD_SETVOP_VOP3     BIT3
    #define LCD_SETVOP_VOP4     BIT4
    #define LCD_SETVOP_VOP5     BIT5
    #define LCD_SETVOP_VOP6     BIT6


#define LCD_ROWS	            48
#define LCD_COLS	            84
#define LCD_SCREENBUFFER_LEN    504
#define LCD_WIDTH               LCD_COLS
#define LCD_HEIGHT              LCD_ROWS
// !!! Note, LCD_CS, LCD_DC, LCD_RESET, LCD_SCK, LCD_MOSI
// !!!   must be defined in the local copy of pygmy_profile.h

PYGMYGUI pygmyGlobalColors;
PYGMYSPIPORT pygmyLCD;
u8 globalScreenBuffer[ LCD_SCREENBUFFER_LEN ];

void lcdWriteCommand( u16 uiCommand )
{
    pinSet( LCD_A0, LOW );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    pygmyLCD.PortSCK->BRR = pygmyLCD.PinSCK; // verify SCK low
    spiWriteByte( &pygmyLCD, (u8)uiCommand );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdWriteData( u16 uiData )
{
    pinSet( LCD_A0, HIGH );
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    pygmyLCD.PortSCK->BRR = pygmyLCD.PinSCK; // verify SCK low
    spiWriteByte( &pygmyLCD, (u8)uiData );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
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
   
    pinSet( LCD_RESET, LOW );
    delay( 2000 ); // microseconds
    pinSet( LCD_RESET, HIGH );
    delay( 2000 ); // microseconds
    lcdWriteCommand( LCD_FUNCSET | LCD_FUNCSET_H ); 
    lcdWriteCommand( LCD_SETVOP | 55 );   
    lcdWriteCommand( LCD_TEMPCONTROL ); 
    lcdWriteCommand( LCD_BIAS | LCD_BIAS_BS1|LCD_BIAS_BS0 );
    lcdWriteCommand( LCD_FUNCSET ); 
    lcdWriteCommand( LCD_DISPCONTROL | LCD_DISPCONTROL_D ); 
    lcdSetBPP( PYGMY_PBM_1BPP );
    lcdSetColor( 0xFF, 0xFF, 0xFF );
    lcdBackColor( 0x00, 0x00, 0x00 );
    lcdClear();
    drawAll();
}

void lcdDrawPixel( u16 uiX, u16 uiY )
{
    u16 i, ii;
    
    if( uiX >= LCD_WIDTH || uiY >= LCD_HEIGHT ){
        return;
    } // if
    if( pygmyGlobalColors.Color.R ){
        globalScreenBuffer[ ( uiX * 6 ) + ( uiY / 8 ) ] |= PYGMY_BITMASKS[ uiY % 8 ];
    } else{
        globalScreenBuffer[ ( uiX * 6 ) + ( uiY / 8 ) ] &= ~PYGMY_BITMASKS[ uiY % 8 ];
    } // else
}

void lcdClearPixel( u16 uiX, u16 uiY )
{
    if( uiX >= LCD_WIDTH || uiY >= LCD_HEIGHT ){
        return;
    } // if
    if( pygmyGlobalColors.BackColor.R ){
        globalScreenBuffer[ ( uiX * 6 ) + ( uiY / 8 ) ] |= PYGMY_BITMASKS[ uiY % 8 ];
    } else{
        globalScreenBuffer[ ( uiX * 6 ) + ( uiY / 8 ) ] &= ~PYGMY_BITMASKS[ uiY % 8 ];
    } // else
}

void lcdWriteScreenBuffer( void )
{
    u16 i, ii;

    for( i = 0; i < 6; i++ ){
        lcdWriteCommand( LCD_SETY | i );
        lcdWriteCommand( LCD_SETX );
        for( ii = 0; ii < 84; ii++ ){
            lcdWriteData( globalScreenBuffer[ ( ( ii ) * 6 ) + i ] );
        } // for
    } // for

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
    lcdWriteCommand( LCD_FUNCSET );
}

void lcdOff( void )
{
    lcdWriteCommand( LCD_FUNCSET | LCD_FUNCSET_PD );
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
            if( pygmyGlobalColors.BackColor.R ){
                globalScreenBuffer[ ( uiX1 * 6 ) + ( uiY1 / 8 ) ] |= PYGMY_BITMASKS[ uiY1 % 8 ];
            } else{
                globalScreenBuffer[ ( uiX1 * 6 ) + ( uiY1 / 8 ) ] |= PYGMY_BITMASKS[ uiY1 % 8 ];
            } // else
        } // for
    } // for
}

void lcdClear( void )
{
    u16 i;
  
    for( i = 0; i < LCD_SCREENBUFFER_LEN; i++ ){
        if( pygmyGlobalColors.BackColor.R ){
            globalScreenBuffer[ i ] = 0xFF;
        } else{
            globalScreenBuffer[ i ] = 0;
        } // else
    } // for
}

