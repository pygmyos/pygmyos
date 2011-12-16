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

#pragma once

#include "pygmy_profile.h"      
      
#define PUNCT              BIT0
#define WHITESPACE         BIT1
#define NEWLINE            BIT2
#define SEPARATORS         BIT3
#define QUOTES             BIT4
#define COMMA              BIT5

//extern const u8 PYGMYBASE64CHARS[];
//extern const u8 PYGMYHEXCHARS[];

u16 bufferToU16( u8 *ucBuffer );
u32 bufferToU32( u8 *ucBuffer );
s8 isSeparator( u8 ucChar );
s8 isMathChar( u8 ucChar );
s8 isQuoteChar( u8 ucChar );
s8 isCharInString( u8 ucChar, u8 *ucChars );
s8 isAlpha( u8 ucChar );
s8 isNumeric( u8 ucChar );
s8 isAlphaOrNumeric( u8 ucChar );
s8 isHex( u8 ucChar );
s8 isBinary( u8 ucChar );
s8 isOctal( u8 ucChar );
s8 isWhitespace( u8 ucChar );
s8 isPunctuation( u8 ucChar );
s8 isStringSame( u8 *ucBuffer, u8 *ucString );
u16 len( u8 *ucString );
u8 replaceChars( u8 *ucBuffer, u8 *ucChars, u8 ucChar );
u8 *seekStringInString(  u8 *ucString, u8 *ucBuffer );
u8 *getNextSubString( u8 *ucBuffer, u8 ucMode );
//u8 *getNextSubString( u8 *ucBuffer );
u8 convertIntToString( s64 ulData, u8 *ucFormat, u8 *ucBuffer );
void copyString( u8 *ucFrom, u8 *ucTo );
void appendStringToString( u8 *ucFrom, u8 *ucTo );
s32 convertStringToInt( u8 *ucBuffer );
u8 convertCharToInt( u8 ucChar );
s32 seekStringInBuffer( u8 *ucString, u8 *ucBuffer, u32 ulLen  );
u16 convertHexEncodedStringToBuffer( u8 *ucString, u8 *ucBuffer );
u16 convertHexCharToInteger( u8 ucChar );
u8 getBase64Code( u8 ucChar );
u16 convertBase64ToBuffer( u8 *ucBufferIn, u8 *ucBufferOut );
void convertBase64ToString( u8 *ucBuffer, u8 *ucString );
void convertStringToBase64( u8 *ucString, u8 *ucBase64 );

