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

const u8 PYGMYBASE64CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const u8 PYGMYHEXCHARS[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

u16 bufferToU16( u8 *ucBuffer )
{
    u16 ii;
    u8 i;

    for ( i = 0, ii = *(ucBuffer++); i < 3; i++ ){
        ii = ( ii << 8 ) | *(ucBuffer++);
    } // if

    return( ii );
}

u32 bufferToU32( u8 *ucBuffer )
{
    u32 ii;
    u8 i;

    for ( i = 0, ii = *(ucBuffer++); i < 3; i++ ){
        ii = ( ii << 8 ) | *(ucBuffer++);
    } // if

    return( ii );
}

s8 isCharInString( u8 ucChar, u8 *ucChars )
{
    for( ; *ucChars; ) {
        if( ucChar == *(ucChars++) ){
            return( 1 );
        } // if
    } // for
    
    return( 0 );
}

s8 isAlpha( u8 ucChar )
{
    if( ( ucChar > 64 && ucChar < 91 ) || ( ucChar > 96 && ucChar < 123 ) ){
        return( 1 );
    } // if
    
    return( 0 );
}

s8 isNumeric( u8 ucChar )
{
    if( ucChar > 47 && ucChar < 58 ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isAlphaOrNumeric( u8 ucChar )
{
    if( ( ucChar > 64 && ucChar < 91 ) || ( ucChar > 96 && ucChar < 123 )
        || ( ucChar > 47 && ucChar < 58 ) ){
        return( 1 );
    } // if

    return( 0 );
}

s8 isHex( u8 ucChar )
{
    if( ( ucChar > 47 && ucChar < 58 ) || ( ucChar > 64 && ucChar < 71 ) ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isBinary( u8 ucChar )
{
    if( ucChar == '0' || ucChar == '1' ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isOctal( u8 ucChar )
{
    if( ucChar > 47 && ucChar < 56 ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isNewline( u8 ucChar )
{
    if( ucChar == 10 || ucChar == 12 || ucChar == 13 ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isWhitespace( u8 ucChar )
{
    if( ( ucChar > 7 && ucChar < 33 ) ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isQuote( u8 ucChar )
{
    if( ( ucChar == 34 ) || ( ucChar == 39 ) ){
        return( 1 );
    } // if
    
    return( 0 );
    
}

s8 isMath( u8 ucChar )
{
    if( isCharInString( ucChar, "*/%&^|+=<>()%~" ) ){
        return( 1 );
    } // if
        
    return( 0 );
}

s8 isSeparator( u8 ucChar )
{
    if( isCharInString( ucChar, "/\\{}[]-_+=@`|<>'\"" ) ){
        return( 1 );
    } // if
    
    return( 0 );
}

s8 isPunctuation( u8 ucChar )
{
    if( isCharInString( ucChar, "!?,.:;" ) ){
        return( 1 );
    } // if
        
    return( 0 );
}

u16 len( u8 *ucString )
{
    u16 i;
    
    for( i = 0; *(ucString++) ; i++ ){
        if( i == 0xFFFF ){
            break;
        } // if
    } // for
    
    return( i );
}

s8 isStringSame( u8 *ucBuffer, u8 *ucString )
{
    if( len( ucBuffer ) != len( ucString ) )
        return ( 0 );
        
    for( ; *ucBuffer ; ){
        if( *(ucBuffer++) != *(ucString++) ){
            return( 0 );
        } // if
    } // for
    
    return( 1 );
}

u8 *getNextSubString( u8 *ucBuffer, u8 ucMode )
{
    static u8 *ucIndex = NULL;
    
    if( *ucBuffer ){
        ucIndex = ucBuffer;
    } // if
    if( ucIndex == NULL ){
        return( NULL );
    } // if
    
    for( ; *ucIndex;  ){
        if( ( isWhitespace( *ucIndex ) ) || 
            ( (ucMode & PUNCT) && isPunctuation( *ucIndex ) ) ||
            ( (ucMode & SEPARATORS) && isSeparator( *ucIndex ) ) ||
            ( (ucMode & QUOTES) && isQuote( *ucIndex ) ) ||
            ( (ucMode & COMMA) && *ucIndex == ',' ) ){
            ++ucIndex;
            continue;
            } // if
        ucBuffer = ucIndex;
        //for( ; *ucIndex && !Pygmy_TestWhiteSpace( *ucIndex ) && !Pygmy_TestPunctuation( *ucIndex ) ; ){
        for( ; *ucIndex ; ){
            if( ( (ucMode & WHITESPACE) && isWhitespace( *ucIndex ) ) || 
                ( (ucMode & PUNCT) && isPunctuation( *ucIndex ) ) ||
                ( (ucMode & SEPARATORS) && isSeparator( *ucIndex ) ) ||
                ( (ucMode & QUOTES) && isQuote( *ucIndex ) ) ||
                ( (ucMode & NEWLINE) && isNewline( *ucIndex ) ) ||
                ( (ucMode & COMMA) && *ucIndex == ',' ) ){
                break;
            } // if
            ++ucIndex;
        } // for
        if( *ucIndex ){
            *(ucIndex++) = '\0';
        } else{
            ucIndex = NULL;
        } // else
    
        return( ucBuffer );
    } // for
    
    return( NULL );
}

u8 replaceChars( u8 *ucBuffer, u8 *ucChars, u8 ucChar )
{
    u16 i, uiReplaced;
    
    for( uiReplaced = 0; *ucBuffer ; ){
        for( i = 0; *(ucChars+i); i++ ){
            if( *ucBuffer == *(ucChars+i) ){
                ++uiReplaced; 
                *ucBuffer = ucChar;
                break;
            } // if
        } // for
        ++ucBuffer;
    } // for
    
    return( uiReplaced );
}

u8 convertCharToInt( u8 ucChar )
{
    if( ucChar > 47 && ucChar < 58 ){
        return( ucChar - 48 );
    } else if( ucChar > 64 && ucChar < 71 ){
        return( ucChar - 55 );
    } else if( ucChar > 96 && ucChar < 103 ){
        return( ucChar - 87);
    }
    
    return( 0 );   
}

u8 convertIntToChar( u8 ucData )
{
    
}

u8 *seekChar( u8 ucChar, u8 *ucString )
{
    // ToDo: Add this function

    return( NULL );
}

u8 *seekString( u8 *ucSeekString, u8 *ucInString )
{
    u16 i, ii, uiLen, uiStop;
    
    uiLen = len( ucSeekString );
    uiStop = 1 + len( ucInString );
    if( uiStop < uiLen ){ // can't search in a string for a longer string match
        return( NULL );
    }
    uiStop -= uiLen; // don't search for a match past end of InString
    for( i = 0; i < uiStop; i++ ){
        for( ii = 0; ii < uiLen; ii++ ){
            if( ucSeekString[ ii ] != ucInString[ i + ii ] ){
                break; // break on first mismatch, increment and test again
            }
        }
        if( ii == uiLen ){
            return( (u8*)ucInString+i ); // this is the start of the match
        }
    }
    
    return( NULL );
}

s32 seekStringInBuffer( u8 *ucString, u8 *ucBuffer, u32 ulLen  )
{
    // ToDo: Add this function
    
    return( 0 );
}

u8 convertIntToString( s64 lData, u8 *ucFormat, u8 *ucBuffer )
{
    u8 ucTmpFormat[ 9 ];
    s64 i, iType, iLen, iMagnitude, iValue;
    u8 ucPadding;

    for( i = 0; i < 8 && ucFormat[ i ]; i++ )
        ucTmpFormat[ i ] = ucFormat[ i ];
    ucTmpFormat[ i ] = '\0';
    ucFormat = ucTmpFormat;
    
    if( lData < 0 ){
        *(ucBuffer++) = '-';
        lData = ~((u32)lData) + 1; // reverse 2s complement
    }
    if( *ucFormat == '%' ){
        ++ucFormat;
    }
    if( !isAlphaOrNumeric( *ucFormat ) || *ucFormat == '0' ){
        ucPadding = *ucFormat;
        ++ucFormat;
    } else{
        ucPadding = '0';
    }
    
    if( replaceChars( ucFormat, "xXHh", ' ' ) ){
        iType = 16;
    } else if( replaceChars( ucFormat, "oO", ' ' ) ){
        iType = 8;
    } else if( replaceChars( ucFormat, "bB", ' ' ) ){
        iType = 2;
    } else{
        iType = 10; // Decimal
    } // else
    
    iLen = convertStringToInt( ucFormat )-1;
    if( iLen < 0 )
        iLen = 0;
    for( i = 0, iMagnitude = 1; ( iMagnitude * iType ) <= lData; i++ )
        iMagnitude *= iType;
    for( ; iLen > i; iLen-- ){
        *(ucBuffer++) = ucPadding;//'0';
    }
    
    for( ; i>=0; i-- ){
        iValue = lData / iMagnitude;
        *(ucBuffer++) = PYGMYHEXCHARS[ iValue ];
        lData -= ( iValue * iMagnitude );
        iMagnitude /= iType;
    }
    *ucBuffer = '\0';
}

void appendString( u8 *ucFrom, u8 *ucTo )
{
    for( ; *ucTo; ){
        ++ucTo;
    }
    for( ; *ucFrom; ){
        *(ucTo++) = *(ucFrom++);
    }
    *ucTo = '\0';
}

void copyString( u8 *ucFrom, u8 *ucTo )
{
    for( ; *ucFrom; ){
        *(ucTo++) = *(ucFrom++); 
    }
    *ucTo = '\0';
}

s32 convertStringToInt( u8 *ucBuffer )
{
    u8 ucScratch[35]; // 32 + sign char and format char
    s32 iValue, iType, iSign;
    
    // ucBuffer is moved to scratch buffer to make function constant safe
    for( iValue = 0; iValue < 35 && ucBuffer[ iValue ]; iValue++ )
        ucScratch[ iValue ] = ucBuffer[ iValue ];
    ucScratch[ iValue ] = '\0';
    ucBuffer = ucScratch;    
    
    iType = 10; // Decimal
    if( replaceChars( ucBuffer, "-", ' ' ) )
        iSign = -1;
    else
        iSign = 1;
    // HEX must be tested first to avoid stripping valid HEX digit chars
    if( replaceChars( ucBuffer, "xXhH", ' ' ) ){//"xXh"
        iType = 16; // HEX
    }else if( replaceChars( ucBuffer, "bB", ' ' ) ){//"bB"
        iType = 2; // Binary
    }else if( replaceChars( ucBuffer, "oO", ' ' ) ){//"oO"
        iType = 8; // Octal
    } else{
        replaceChars( ucBuffer, "dD", ' ' );//"dD"
    }
   
    for( iValue = 0; *ucBuffer ; ){
        if( isAlphaOrNumeric( *ucBuffer ) ){
            iValue = ( iValue * iType ) + convertCharToInt( *ucBuffer );
        }
        ++ucBuffer;
    }
    
    return( iValue * iSign );
}

u8 convertBase64ToCode( u8 ucChar )
{
    // This function intentionally avoids the normal for loop
    // since there are two common alphabets for Base64 and a
    // for loop to handle both is significantly less efficient
    // Alphabet 1 = A-Z,a-z,0-9,+,/,=
    // Alphabet 2 = A-Z,a-z,0-9,-,_,=
    // The = char signifies padding, input len must be divisible by 3
    // and output len must be divisible by 4
    if( ucChar > 64 && ucChar < 91 ){ // A-Z
        return( ucChar - 65 );
    } else if( ucChar > 96 && ucChar < 123 ){ // a-z
        return( ucChar - 71 );
    } else if( ucChar > 47 && ucChar < 58 ){ // 0-9
        return( ucChar + 4 );
    } else if( ucChar == '+' || ucChar == '-' ){
        return( 62 );
    } else if( ucChar == '/' || ucChar == '_' ){
        return( 63 );
    } 

    return( 64 );
}

u16 convertHexEncodedStringToBuffer( u8 *ucString, u8 *ucBuffer )
{
    // HEX Encoded ASCII string to binary eqivalent
    // Use: ( "01020A", ptr ) , ptr[ 0 ] = 1, ptr[ 1 ] = 2, ptr[ 3 ] = 10
    // Note that input buffer may be used as output buffer since input is always
    //  twice the length of the output
    u16 uiLen;
    u8 ucByte;

    for( uiLen = 0; *ucString; ){
        ucByte = convertHexCharToInteger( *(ucString++) ) * 16;
        ucByte += convertHexCharToInteger( *(ucString++) );
        ucBuffer[ uiLen++ ] = ucByte;
    }
        
    return( uiLen );
}



u16 convertHexCharToInteger( u8 ucChar )
{
    u16 i;

    for( i = 0; i < 16 && ucChar != PYGMYHEXCHARS[ i ]; i++ )
        ;
    
    return( i );
}

u16 convertBase64ToBuffer( u8 *ucBufferIn, u8 *ucBufferOut )
{
    u16 i, uiPad, uiLen;
    u32 ulValue;
    
    for( uiLen = 0; *ucBufferIn ; ){
        for( i = 0, ulValue = 0, uiPad = 0; i < 4 && *ucBufferIn; i++ ){
            if( convertBase64ToCode( *ucBufferIn ) == 64 )
                ++uiPad;
            else
                ulValue +=  ( u32)convertBase64ToCode( *ucBufferIn ) << ( ( 3 - i ) * 6 ) ;
            ++ucBufferIn;
        }
        for( i = 0; i < 3-uiPad; i++, uiLen++ ){
            *(ucBufferOut++) = PYGMYBASE64CHARS[ (u8)(0x000000FF & ( ulValue >> ( ( 2 - i ) * 8 ) ) ) ];
        }
    }
    
    return( uiLen );
}

void convertBase64ToString( u8 *ucBuffer, u8 *ucString )
{
    u16 i, uiPad;
    u32 ulValue;
    
    for( ; *ucBuffer ; ){
        for( i = 0, ulValue = 0, uiPad = 0; i < 4 && *ucBuffer; i++ ){
            if( convertBase64ToCode( *ucBuffer ) == 64 )
                ++uiPad;
            else
                ulValue +=  ( u32)convertBase64ToCode( *ucBuffer ) << ( ( 3 - i ) * 6 ) ;
            ++ucBuffer;
        }
        for( i = 0; i < 3-uiPad; i++){
            *(ucString++) = PYGMYBASE64CHARS[ (u8)(0x000000FF & ( ulValue >> ( ( 2 - i ) * 8 ) ) ) ];
        }
    }
    *ucString = '\0';
}

void convertStringToBase64( u8 *ucString, u8 *ucBase64 )
{
    u16 i, uiPad, uiLen;
    u32 ulValue;
    
    uiLen = len( ucString );
    for( ; *ucString; ){
        for( i = 0, ulValue = 0, uiPad = 0; i < 3; i++){
            if( !(*ucString) ){
                uiPad = 1;
                break;
            }
            ulValue += ((u32)*(ucString++) << ( ( 2 - i ) * 8 ) );
        }
        if( uiPad )
            uiPad = 3 - ( uiLen % 3 );
        for( i = 0; i < 4 - uiPad; i++ ){
            *(ucBase64++) = PYGMYBASE64CHARS[ (u8)(0x0000003F & ( ulValue >> ( ( 3 - i ) * 6 ) ) ) ];
        }
        for( ; i < 4; i++ ){
            *(ucBase64++) = '=';
        }
    }
    *ucBase64 = '\0';
}


