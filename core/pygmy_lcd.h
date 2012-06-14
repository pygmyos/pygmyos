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

// !!! Note: The defines in this file are intended to provide a seamless
// !!!   interface to the pygmy_gui lib. All functions declared should
// !!!   be inplemented by the hardware specific LCD source file

//#pragma once
#ifndef __PYGMY_HEADER_LCD
	#define __PYGMY_HEADER_LCD

#include "pygmy_profile.h"   

extern const u8 LCD_BITMASKS[];

void lcdWriteCommand( u16 uiCommand );
void lcdWriteData( u16 uiData );
u16 lcdReadData( void );
void lcdBacklightOn( void );
void lcdBacklightOff( void );
u16 lcdGetWidth( void );
u16 lcdGetHeight( void );
void lcdInit( void );
u8 lcdGetContrast( void );
void lcdSetContrast( u8 ucContrast );
u32 lcdGetPixel( u16 uiX, u16 uiY );
void lcdGetRGB( u32 ulPixel, u8 ucBPP, u8 *ucR, u8 *ucG, u8 *ucB );
void lcdDrawPixel( u16 uiX, u16 uiY );
void lcdSetDrawArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
void lcdWriteScreenBuffer( void );
void lcdOn( void );
void lcdOff( void );
void lcdSetBPP( u8 ucBPP );
u8 lcdGetBPP( void );
void lcdSetColor( u8 ucR, u8 ucG, u8 ucB );
void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
void lcdClear( void );

#endif // __PYGMY_HEADER_LCD
