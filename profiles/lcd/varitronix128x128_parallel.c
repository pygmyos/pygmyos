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

#ifdef __PYGMYLCDSHIELD
//********************************************************************
//*************************Varitronix 128x128*************************
//*************************Samsung S6B3306X11*************************
//********************************************************************
// Standard Instruction Set
#define LCD_NOP                 0x00
#define LCD_SWRESET             0x01
#define LCD_RDDST               0x09
#define LCD_RDDPM               0x0A
#define LCD_RDDMADCTR           0x0B
#define LCD_RDDCOLMOD           0x0C
#define LCD_RDDIM               0x0D
#define LCD_RDDSM               0x0E
#define LCD_SLPIN               0x10
#define LCD_SLPOUT              0x11
#define LCD_PTLON               0x12
#define LCD_NORON               0x13
#define LCD_INVOFF              0x20
#define LCD_INVON               0x21
#define LCD_APOFF               0x22
#define LCD_APON                0x23
#define LCD_WRCNTR              0x25
#define LCD_DISPOFF             0x28
#define LCD_DISPON              0x29
#define LCD_CASET               0x2A
#define LCD_RASET               0x2B
#define LCD_RAMWR               0x2C
#define LCD_RAMRD               0x2E
#define LCD_PTLAR               0x30
#define LCD_SCRLAR              0x33
#define LCD_TEOFF               0x34
#define LCD_TEON                0x35
#define LCD_MADCTR              0x36
#define LCD_VSCSAD              0x37
#define LCD_IDMOFF              0x38
#define LCD_IDMON               0x39
#define LCD_COLMOD              0x3A
#define LCD_RDID                0xDA
#define LCD_DUTYSET             0xB0
#define LCD_FIRSTCOM            0xB1
#define LCD_OSCDIV              0xB3
#define LCD_NLINVSET            0xB5
#define LCD_COMSCANDIR          0xB7
#define LCD_RMWIN               0xB8
#define LCD_RMWOUT              0xB9
#define LCD_DISPCOMPSET         0xBD
#define LCD_VOPSET              0xC0
#define LCD_VOPOFFSETINC        0xC1
#define LCD_VOPOFFSETDEC        0xC2
#define LCD_BIASSEL             0xC3
#define LCD_BSTBMPXSEL          0xC4
#define LCD_BSTEFFSEL           0xC5
#define LCD_VGSORCSEL           0xCB
#define LCD_IDSET               0xCC
#define LCD_ANASET              0xD0
#define LCD_AUTOLOADSET         0xD7
#define LCD_RDTSTSTATUS         0xDE
#define LCD_EEPCIN              0xE0
#define LCD_EEPCOUT             0xE1
#define LCD_EEPWR               0xE2
#define LCD_EEPRD               0xE3
#define LCD_ROMSET              0xE5
#define LCD_FRMSEL              0xF0
#define LCD_FRM8SEL             0xF1
#define LCD_TMPRNG              0xF2
#define LCD_TMPHYS              0xF3
#define LCD_TMPSEL              0xF4
#define LCD_THYS                0xF7
#define LCD_FRAMESET            0xF9
#define LCD_EEPANFSEL           0xFA
#define LCD_EEPERS              0xFB

#define LCD_ROWS	            128
#define LCD_COLS	            128
#define LCD_SCREENBUFFER_LEN    0
#define LCD_WIDTH               LCD_COLS
#define LCD_HEIGHT              LCD_ROWS

// !!! Note, LCD_CS, LCD_A0, LCD_RESET, LCD_SCK, LCD_MOSI be predefined

u8 globalPixel[ 2 ], globalBPP, globalContrast;

void lcdWriteCommand( u16 uiCommand )
{  
    LCD_CS_LOW;
    LCD_A0_LOW;
    LCD_DATA_WR( uiCommand );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}

void lcdWriteData( u16 uiData )
{
    
    LCD_CS_LOW;
    LCD_A0_HIGH;  
    LCD_DATA_WR( uiData );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}

u16 lcdReadData( void )
{
    /*u8 ucByte;

    LCD_DATA_RD_INIT;
    LCD_A0_HIGH;
    LCD_CS_LOW;
    LCD_RD_LOW;
    LCD_RD_HIGH;
    ucByte = LCD_DATA_RD;
    LCD_CS_HIGH;
    LCD_DATA_WR_INIT;
*/
    return( 0 );
}

void lcdBacklightOn( void )
{
    LCD_BL_HIGH;
}

void lcdBacklightOff( void )
{
    LCD_BL_LOW;
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
    SHIELD_INIT; // For Clocks, must be defined, even if no shield used
    LCD_CS_INIT;
    LCD_A0_INIT; 
    LCD_WR_INIT;
    LCD_RD_INIT;
    LCD_BL_INIT;
    LCD_BL_HIGH;
    LCD_DATA_WR_INIT;
    LCD_RESET_INIT;
   
    LCD_RESET_LOW;
    delay( 900 ); // microseconds
    LCD_RESET_HIGH;
    delay( 8000 ); // microseconds
    
    lcdWriteCommand( LCD_AUTOLOADSET );     // Autoread control		
        lcdWriteData( 0x9F );               // Disable Autoread		
	lcdWriteCommand( LCD_EEPCIN );          // Control OTP/MTP		
        lcdWriteData(0x00);                 // Read Mode				
	delay(500);                            // microseconds				
    lcdWriteCommand( LCD_EEPRD );           // Control OTP/MTP						
	delay(500);                            // microseconds						
    lcdWriteCommand( LCD_EEPCOUT );         // Close Read Mode	
							    
    lcdWriteCommand( LCD_DISPOFF );         // Display OFF								
    lcdWriteCommand( LCD_SLPOUT );          // Sleep Out								
    delay(500);                            // microseconds								
   
    lcdWriteCommand( LCD_VOPSET );          // V0 Voltage Set								
        lcdWriteData( 0xFF ); // 0xF0                					
        lcdWriteData( 0x00 );                							
					
    lcdWriteCommand( LCD_BIASSEL );              							
        lcdWriteData( 0x03 );   // 0x03              							
	lcdWriteCommand( LCD_BSTBMPXSEL );      // BOOST     							
		lcdWriteData( 0x07 );               //06-->7X; 07-->8x							
	lcdWriteCommand( LCD_BSTEFFSEL );       //Booster Efficiency Set							
		lcdWriteData( 0x02 );               // 0x01 Low 0x02 High 						
	lcdWriteCommand( LCD_VGSORCSEL );       // VG Booster Set							
		lcdWriteData( 0x01 ); 	// 0x01						

	lcdWriteCommand( LCD_COMSCANDIR );      //Glass Direction						
        lcdWriteData( 0x00 );              					
							
	//lcdWriteCommand( LCD_NLINVSET );        // N-line Set						
	//	lcdWriteData(0x8B);                 // 0x8B					
    lcdWriteCommand( LCD_ANASET );
        lcdWriteData( 0x1D );
	//lcdWriteCommand(0xEC);              	// ???						
	//	lcdWriteData(0x04);                 // ???					
    lcdWriteCommand( LCD_DISPCOMPSET );           								
		lcdWriteData(0x02);                 // 0x02
              			
    lcdWriteCommand( LCD_FRMSEL );          // Frame Frequency Select	
	    lcdWriteData( 0x07 );               // 45Hz (-30^C ~ -10^C)		
        lcdWriteData( 0x0C );               // 60Hz (-10^C ~ 0^C)			
        lcdWriteData( 0x0C );               // 72Hz (0^C ~ 10^C)			
        lcdWriteData( 0x12 );			    // 77HZ
		
	lcdWriteCommand( LCD_INVOFF );          // Normal Display	
    
	lcdWriteCommand( LCD_COLMOD );          // Pixel Format Set	
		lcdWriteData( 0x05 );               // 64K 	

	lcdWriteCommand( LCD_MADCTR );          // Memory Access Control	
		lcdWriteData( 0x00 );  // BIT4|BIT3               // 0xC8 Clear BIT3

	lcdWriteCommand( LCD_DUTYSET );                  //Duty Set		
		lcdWriteData( 0x7F );                 //0x7F 1/128	

    lcdWriteCommand( LCD_DISPON );          	
    
	lcdWriteCommand( LCD_FRAMESET);         // Gamma
		lcdWriteData(0x00);                 // 
		lcdWriteData(0x02);                 //
		lcdWriteData(0x04);                 //
		lcdWriteData(0x06);                 //
		lcdWriteData(0x08);                 //
		lcdWriteData(0x0A);                 //
		lcdWriteData(0x0C);                 //
		lcdWriteData(0x0E);                 //
		lcdWriteData(0x10);                 //
		lcdWriteData(0x12);                 //
		lcdWriteData(0x14);                 //
		lcdWriteData(0x16);                 //
		lcdWriteData(0x18);                 //
		lcdWriteData(0x1A);                 //
		lcdWriteData(0x1C);                 //
       
    lcdSetContrast( 47 );
    lcdSetBPP( PYGMY_PBM_16BPP );
    //lcdSetColor( 0x00, 0x00, 0x00 );
    lcdSetColor( 0xFF, 0xFF, 0xFF );
    lcdClear();
}

u8 lcdGetContrast( void )
{
    return( globalContrast );
}

void lcdSetContrast( u8 ucContrast )
{
    globalContrast = ucContrast;
    lcdWriteCommand( 0x25 );                // Contrast							
    lcdWriteData( globalContrast ); 	
}

u32 lcdGetPixel( u16 uiX, u16 uiY )
{
    u16 uiColor, uiByte;
    
    
    LCD_CS_LOW;
    LCD_A0_LOW;
    LCD_DATA_WR( LCD_CASET );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    
    LCD_A0_HIGH;  
    LCD_DATA_WR( uiX );
    LCD_WR_LOW;
    LCD_WR_HIGH; 
    LCD_WR_LOW;
    LCD_WR_HIGH;
   
    LCD_A0_LOW;
    LCD_DATA_WR( LCD_RASET );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    
    LCD_A0_HIGH;  
    LCD_DATA_WR( uiY );
    LCD_WR_LOW;
    LCD_WR_HIGH; 
    LCD_WR_LOW;
    LCD_WR_HIGH;
    //LCD_CS_HIGH;
    LCD_RD_HIGH;
    LCD_A0_LOW;
    LCD_DATA_WR( LCD_RAMRD );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    delay( 60 );
    LCD_A0_HIGH;
    LCD_DATA_RD_INIT;
    
    LCD_RD_LOW; // dummy read
    LCD_RD_HIGH; // dummy read
    LCD_RD_LOW;
    LCD_RD_HIGH;
    delay( 60 );
    uiColor = LCD_DATA_RD; 
    
    LCD_RD_LOW;
    LCD_RD_HIGH;
    delay( 60 );
    uiByte = LCD_DATA_RD;
    //print( COM3, "(%X %X)", uiColor, uiByte );
    uiColor = ( uiColor << 8) | uiByte;
    LCD_CS_HIGH;
    //LCD_RD_LOW;
    LCD_DATA_WR_INIT;

    return( uiColor );
}

void lcdDrawPixel( u16 uiX, u16 uiY )
{   
    if( uiX >= LCD_WIDTH || uiY >= LCD_HEIGHT ){
        return;
    } // if
    
    LCD_CS_LOW;
    LCD_A0_LOW;
    LCD_DATA_WR( LCD_CASET );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    
    LCD_A0_HIGH;  
    LCD_DATA_WR( uiX );
    LCD_WR_LOW;
    LCD_WR_HIGH; 
    //LCD_DATA_WR( uiX );
    LCD_WR_LOW;
    LCD_WR_HIGH;
   
    LCD_A0_LOW;
    LCD_DATA_WR( LCD_RASET );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    
    LCD_A0_HIGH;  
    LCD_DATA_WR( uiY );
    LCD_WR_LOW;
    LCD_WR_HIGH; 
    //LCD_DATA_WR( uiY );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_CS_HIGH;
    
    lcdWriteCommand( LCD_RAMWR );
    LCD_CS_LOW;
    LCD_A0_HIGH;  
    LCD_DATA_WR( globalPixel[ 0 ] );
    //LCD_DATA_WR( (u8)( ( pygmyGlobalColors.Color.R << 3 ) | ( ( pygmyGlobalColors.Color.G >> 3 ) & 0x07 ) ) );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_DATA_WR( globalPixel[ 1 ] );
    //LCD_DATA_WR( (u8)( ( ( pygmyGlobalColors.Color.G << 5 ) & 0xE0 ) | ( pygmyGlobalColors.Color.B & 0x1F ) ) );
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}

void lcdSetDrawArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{
    lcdWriteCommand( LCD_CASET );
        lcdWriteData( uiX1 );
        lcdWriteData( uiX2 );
    lcdWriteCommand( LCD_RASET );
        lcdWriteData( uiY1 );
        lcdWriteData( uiY2 );
    lcdWriteCommand( LCD_RAMWR );
}

void lcdWriteScreenBuffer( void )
{
   // Screen buffer internal to display
}

void lcdOn( void )
{
    lcdWriteCommand( LCD_DISPON );
}

void lcdOff( void )
{
    lcdWriteCommand( LCD_DISPOFF );
}

void lcdSetBPP( u8 ucBPP )
{
    globalBPP = ucBPP;
}

u8 lcdGetBPP( void )
{
    return( globalBPP );
}

void lcdSetColor( u8 ucR, u8 ucG, u8 ucB )
{
    if( globalBPP == PYGMY_PBM_8BPP ){
        ucR <<= 3; 
        ucG <<= 2;
        ucB <<= 3;
    } else if( globalBPP == PYGMY_PBM_16BPP || globalBPP == PYGMY_PBM_1BPP ){
        //ucR = ucByte1 >> 3;
        //ucG = (( ucByte1 & 0x07 ) << 3 ) | (ucByte2 >> 5);
        //ucB = ucByte2 & 0x1F;
        //globalPixel[ 0 ] = (u8)( ( ucR << 3 ) | ( ( ucG >> 3 ) & 0x07 ) );
        //globalPixel[ 1 ] = (u8)( ( ucG << 5 ) | ( ucB & 0x1F ) ) ;
        globalPixel[ 1 ] = (u8)( ( ucR << 3 ) | ( ( ucG >> 3 ) & 0x07 ) );
        globalPixel[ 0 ] = (u8)( ( ucG << 5 ) | ( ucB & 0x1F ) ) ;
    } else if( globalBPP == PYGMY_PBM_24BPP ){
        ucR >>= 3; 
        ucG >>= 2;
        ucB >>= 3;
        globalPixel[ 0 ] = (u8)( ( ucR  << 3 ) | ( ( ucG >> 3 ) & 0x07 ) );
        globalPixel[ 1 ] = (u8)( ( ( ucG << 5 ) & 0xE0 ) | ( ucB & 0x1F ) ) ;
    } // else if
      
    
    //print( COM3, "\rR: %d G: %d B: %d", ( ucR << 3 )     
} 

void lcdGetRGB( u32 ulPixel, u8 ucBPP, u8 *ucR, u8 *ucG, u8 *ucB )
{
    if( ucBPP == 16 ){
        
    } else if( ucBPP == 24 ){
        *ucR = (u8)((u16)( ulPixel & 0xF800 ) >> 8 );//11 );
        *ucG = (u8)((u16)( ulPixel & 0x07E0 ) >> 3 );//5 );
        *ucB = (u8)( ulPixel & 0x001F ) << 3;
    } // else if
}

void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{ 
    u32 ulLen;
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
    
    lcdWriteCommand( LCD_CASET );
        lcdWriteData( uiX1 );
        lcdWriteData( uiX2 );
    lcdWriteCommand( LCD_RASET );
        lcdWriteData( uiY1 );
        lcdWriteData( uiY2 );
    lcdWriteCommand( LCD_RAMWR );
    LCD_CS_LOW;   
    for( i = 0, ulLen = ( uiX2 - uiX1 ) * ( 1 + ( uiY2 - uiY1 ) ); i < ulLen; i++ ){
        LCD_A0_HIGH;
        LCD_DATA_WR( globalPixel[ 1 ] );
        //LCD_DATA_WR( (u8)( ( pygmyGlobalColors.BackColor.R << 3 ) |  ( ( pygmyGlobalColors.BackColor.G >> 3 ) & 0x07 ) ) );
        LCD_WR_LOW;
        LCD_WR_HIGH;
        LCD_DATA_WR( globalPixel[ 0 ] );
        //LCD_DATA_WR( (u8)( ( ( pygmyGlobalColors.BackColor.G << 5 ) & 0xE0 ) | ( pygmyGlobalColors.BackColor.B & 0x1F ) ) );
        LCD_WR_LOW;
        LCD_WR_HIGH;    
    } // for
    LCD_CS_HIGH;
}

void lcdClear( void )
{
    lcdClearArea( 0, 0, LCD_WIDTH, LCD_HEIGHT );
}
#endif // __PYGMYLCDSHIELD