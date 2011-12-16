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
//****************************Nokia 132x132***************************
//*************************Epson S1D15G10D08BE************************
//********************************************************************
#define LCD_DISON       0xAF
#define LCD_DISOFF      0xAE
#define LCD_DISNOR      0xA6
#define LCD_DISINV      0xA7
#define LCD_SLPIN       0x95
#define LCD_SLPOUT      0x94
#define LCD_COMSCN      0xBB
#define LCD_DISCTL      0xCA
#define LCD_PAGESET     0x75
#define LCD_COLSET      0x15
#define LCD_DATCTL      0xBC
#define LCD_RGBSET8     0xCE
#define LCD_RAMWR       0x5C
#define LCD_RAMRD       0x5D
#define LCD_PTLIN       0xA8
#define LCD_PTLOUT      0xA9
#define LCD_RMWIN       0xE0
#define LCD_RMWOUT      0xEE
#define LCD_ASCSET      0xAA
#define LCD_SCSTART     0xAB
#define LCD_OSCON       0xD1
#define LCD_OSCOFF      0xD2
#define LCD_PWRCTR      0x20
#define LCD_VOLCTR      0x81
#define LCD_VOLUP       0xD6
#define LCD_VOLDOWN     0xD7
#define LCD_TMPGRD      0x82
#define LCD_EPCTIN      0xCD
#define LCD_EPCOUT      0xCC
#define LCD_EPMWR       0xFC
#define LCD_EPMRD       0xFD
#define LCD_EPSRRD1     0x7C
#define LCD_EPSRRD2     0x7D
#define LCD_NOP         0x25
#define LCD_ENDCOL      131
#define LCD_ENDROW      131

#define LCD_ROWS	132
#define LCD_COLS	132
// !!! Note, LCD_CS, LCD_RESET, LCD_SCK_, LCD_MISO, LCD_MOSI
// !!!   must be defined in the local copy of pygmy_profile.h

PYGMYGUI pygmyGlobalColors;
PYGMYSPIPORT pygmyLCD;

void lcdWriteCommand( u16 uiCommand )
{
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    pygmyLCD.PortSCK->BRR = pygmyLCD.PinSCK; // verify SCK low
    pygmyLCD.PortMOSI->BRR = pygmyLCD.PinMOSI; // LOW MSB ( 9th bit ) is command 
    pygmyLCD.PortSCK->BSRR= pygmyLCD.PinSCK; // Clock out 9th bit
    spiWriteByte( &pygmyLCD, (u8)uiCommand );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdWriteData( u16 uiData )
{
    pygmyLCD.PortCS->BRR = pygmyLCD.PinCS; // Enables chip
    pygmyLCD.PortSCK->BRR = pygmyLCD.PinSCK; // verify SCK low
    pygmyLCD.PortMOSI->BSRR = pygmyLCD.PinMOSI; // HIGH MSB ( 9th bit ) is data 
    pygmyLCD.PortSCK->BSRR = pygmyLCD.PinSCK; // Clock out 9th bit
    spiWriteByte( &pygmyLCD, (u8)uiData );
    pygmyLCD.PortCS->BSRR = pygmyLCD.PinCS; // Disables chip
}

void lcdInit( void )
{	
    spiConfig( &pygmyLCD, LCD_CS, LCD_SCK, LCD_MISO, LCD_MOSI );
    pinConfig( LCD_RESET, OUT ); // We will use USART1_RTS as CS
   
    setPin( LCD_RESET, LOW );
    delay( 2000 ); // microseconds
    setPin( LCD_RESET, HIGH );
    delay( 2000 ); // microseconds
    lcdWriteCommand( LCD_DISCTL );
    lcdWriteData( 0x0C );   		
	lcdWriteData( 0x20 );    
	lcdWriteData( 0x00 );
	lcdWriteData( 0x01 );
    
    lcdWriteCommand( LCD_OSCON );  	
    lcdWriteCommand( LCD_SLPOUT );  
    lcdWriteCommand( LCD_PWRCTR );  // all on
    lcdWriteData( 0x0F );    		// 
	 
    lcdWriteCommand( LCD_DATCTL );
    lcdWriteData(0x00);			    // P1
	lcdWriteData(0x01);   		    // P2, Alternate RGB arrangement
	lcdWriteData(0x04);			    // P3, 16-bit Grayscale Type B
	
    lcdWriteCommand( LCD_VOLCTR ); 
	lcdWriteData( 0x24 );   		// fine
    lcdWriteData( 0x03 );   		// coarse

    lcdWriteCommand( LCD_NOP );  	
    lcdWriteCommand( LCD_DISON ); 
    lcdSetColor( 0xFF, 0xFF, 0xFF );
    lcdBackColor( 0x00, 0x00, 0x00 );
    lcdClear();
}

void lcdDrawPixel( u16 uiX, u16 uiY )
{
    u8 ucColor;

    lcdWriteCommand( LCD_PAGESET );   // page start/end ram
	lcdWriteData( uiY );
    lcdWriteData( uiY );
	lcdWriteCommand( LCD_COLSET );   // column start/end ram
	lcdWriteData( uiX );
    lcdWriteData( uiX );
    lcdWriteCommand( LCD_RAMWR );    // write
    
    lcdWriteData( pygmyGlobalColors.Color.R );
    // 0x00B0 | 0x000R = 0x00B
    ucColor = ( pygmyGlobalColors.Color.G << 4 ) | pygmyGlobalColors.Color.B;
    lcdWriteData( ucColor );
	//lcdWriteData( 0 ); // dummy write   
    //lcdWriteCommand( LCD_NOP ); //
    //lcdWriteData( pygmyGlobalColors.Color.R << 4 | pygmyGlobalColors.Color.G );
    //lcdWriteData( pygmyGlobalColors.Color.B << 4  );
		
}

void lcdClearPixel( u16 uiX, u16 uiY )
{
    u8 ucColor;

    lcdWriteCommand( LCD_PAGESET );   // page start/end ram
	lcdWriteData( uiY );
    lcdWriteData( uiY );
	lcdWriteCommand( LCD_COLSET );   // column start/end ram
	lcdWriteData( uiX );
    lcdWriteData( uiX );
    lcdWriteCommand( LCD_RAMWR );    // write
    
    lcdWriteData( pygmyGlobalColors.BackColor.R );
    // 0x00B0 | 0x000R = 0x00B
    ucColor = ( pygmyGlobalColors.BackColor.G << 4 ) | pygmyGlobalColors.BackColor.B;
    lcdWriteData( ucColor );
	lcdWriteData( 0 ); // dummy write   

    //lcdWriteCommand( LCD_NOP ); //
    //lcdWriteData( pygmyGlobalColors.Color.R << 4 | pygmyGlobalColors.Color.G );
    //lcdWriteData( pygmyGlobalColors.Color.B << 4  );
		
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
    lcdWriteCommand( LCD_DISON );
}

void lcdOff( void )
{
    lcdWriteCommand( LCD_DISOFF );
}

void lcdSetBPP( u8 ucBPP )
{
    pygmyGlobalColors.BPP = ucBPP;
}

void lcdBackColor( u8 ucR, u8 ucG, u8 ucB )
{
    /*if( pygmyGlobalColors.BPP == PYGMY_PBM_1BPP ){
        if( ucR || ucG || ucB ){
            ucR = 0x0F;
            ucG = 0x0F;
            ucB = 0x0F;
        } // if
    } else */
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
   /* if( pygmyGlobalColors.BPP == PYGMY_PBM_1BPP ){
        if( ucR || ucG || ucB ){
            ucR = 0x0F;
            ucG = 0x0F;
            ucB = 0x0F;
        } // if
    } else */
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
    u32 i;
    u8 ucColor;

    
    ucColor = ( pygmyGlobalColors.BackColor.G << 4 ) | pygmyGlobalColors.BackColor.B;
    lcdWriteCommand( LCD_PAGESET );  
    lcdWriteData( uiY1 );
    lcdWriteData( uiY2 );
    lcdWriteCommand( LCD_COLSET );  
    lcdWriteData( uiX1 );
    lcdWriteData( uiX2 );
    lcdWriteCommand( LCD_RAMWR );    
    for( ; uiY1 < uiY2; uiY1++ ){
        for( i = uiX1; i < uiX2; i++ ){ 
            lcdWriteData( pygmyGlobalColors.BackColor.R );   
            // 0x00B0 | 0x000R = 0x00B
            lcdWriteData( ucColor );
        } // for
    } // for
    
}

void lcdClear( void )
{
    u32 i, ulLen;
  
    lcdWriteCommand( LCD_PAGESET );
	lcdWriteData( 0 );
	lcdWriteData( LCD_ENDROW );
	
	lcdWriteCommand( LCD_COLSET );
	lcdWriteData( 0 );
	lcdWriteData( LCD_ENDCOL );

	lcdWriteCommand( LCD_RAMWR );
	ulLen = ( LCD_ROWS * LCD_COLS ) / 2;
   
	for( i = 0; i < ulLen; i++ ) {
		lcdWriteData( pygmyGlobalColors.BackColor.R );
		lcdWriteData( pygmyGlobalColors.BackColor.G );
		lcdWriteData( pygmyGlobalColors.BackColor.B );
	} // for
}


