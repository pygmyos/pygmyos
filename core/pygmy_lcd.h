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

// !!! Note: The defines in the file are intended to prived a seamless
// !!!   interface to the pygmy_gui lib. All functions declared should
// !!!   be inplemented by the hardware specific LCD source file

#pragma once
    
#include "pygmy_profile.h"   

extern const u8 LCD_BITMASKS[];

void lcdInit( void );
void lcdClear( void );
void lcdWriteCommand( u16 uiCommand );
void lcdWriteCommandAndParam( u16 uiCommand, u16 uiParam );
void lcdWriteAddress( u8 ucX1, u8 ucY1, u8 ucX2, u8 ucY2 );
void lcdWriteData( u16 uiData );
u16 lcdReadData( void );
void lcdBacklightOn( void );
void lcdBacklightOff( void );
void lcdWriteScreenBuffer( void );
void lcdSetDrawArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
void lcdDrawPixel( u16 uiX, u16 uiY );
void lcdClearPixel( u16 uiX, u16 uiY );
u32 lcdGetPixel( u16 uiX, u16 uiY );

u16 lcdGetCursorX( void );
u16 lcdGetCursorY( void );
u16 lcdGetWidth( void );
u16 lcdGetHeight( void );
u8 lcdGetContrast( void );
void lcdSetContrast( u8 ucContrast);
void lcdSetCursor( u16 uiX, u16 uiY );
void *lcdGetFont( void );
void lcdSetFont( void *pygmyFont );
void lcdSetCursor( u16 uiX, u16 uiY );
u8 putsLCD( u8 *ucBuffer );
//u16 lcdGetCursorX( void );
//u16 lcdGetCursorY( void );
u32 lcdColorToData( void *pygmyColor );
void lcdSetBPP( u8 ucBPP );
void lcdSetColor( u8 ucR, u8 ucG, u8 ucB );
void lcdSetBackColor( u8 ucR, u8 ucG, u8 ucB );
void *lcdGetColor( void );
void *lcdGetBackColor( void );
void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
