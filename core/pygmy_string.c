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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "pygmy_profile.h"

#define EPSILON 1.0e-7
#define floatIsEqual(a, b) (fabs((a)-(b)) < EPSILON)

const u8 PYGMYHEXCHARS[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
const u8 PYGMYBASE64CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

//#ifdef __PYGMYNEBULA
    #define PYGMYMAXPINS  31
    const PYGMYPAIR PYGMYNEBULAPINS[] = {
                                        { (u8*)"LED0",        LED0 },
                                        { (u8*)"LED1",        LED1 },
                                        { (u8*)"DAC1",        DAC1 },
                                        { (u8*)"DAC2",        DAC2 },
                                        { (u8*)"MCO",         MCO },
                                        { (u8*)"TX1",         TX1 },
                                        { (u8*)"RX1",         RX1 },
                                        { (u8*)"TX2",         TX2 },
                                        { (u8*)"RX2",         RX2 },
                                        { (u8*)"TA0",         TA0 },
                                        { (u8*)"TA1",         TA1 },
                                        { (u8*)"T0",          T0 },
                                        { (u8*)"T1",          T1 },
                                        { (u8*)"T2",          T2 },
                                        { (u8*)"T3",          T3 },
                                        { (u8*)"A0",          A0 },
                                        { (u8*)"A1",          A1 },
                                        { (u8*)"A2",          A2 },
                                        { (u8*)"A3",          A3 },
                                        { (u8*)"A4",          A4 },
                                        { (u8*)"A5",          A5 },
                                        { (u8*)"A6",          A6 },
                                        { (u8*)"A7",          A7 },
                                        { (u8*)"D0",          D0 },
                                        { (u8*)"D1",          D1 },
                                        { (u8*)"D2",          D2 },
                                        { (u8*)"D3",          D3 },
                                        { (u8*)"FLASH_CS",    FLASH_CS },
                                        { (u8*)"FLASH_SCK",   FLASH_SCK },
                                        { (u8*)"FLASH_MISO",  FLASH_MISO },
                                        { (u8*)"FLASH_MOSI",  FLASH_MOSI },
                                        { (u8*)NULL,    0 } };
//#endif // __PYGMYNEBULA

u8 *convertPinToString( u8 ucPin )
{
    u8 i;

    for( i = 0; i < PYGMYMAXPINS; i++ ){
        if( ucPin == PYGMYNEBULAPINS[ i ].Value ){
            return( PYGMYNEBULAPINS[ i ].String );
        } // if
    } // for

    return( " " );
}
                                    
u8 convertStringToPin( u8 *ucBuffer )
{
    // This function is constant string safe
    u8 i, ucPin;

    #ifdef __PYGMYNEBULA
        for( i = 0; i < PYGMYMAXPINS; i++ ){
            if( isStringSame( PYGMYNEBULAPINS[ i ].String, ucBuffer ) ){
                return( PYGMYNEBULAPINS[ i ].Value );
            } // if     
        } // for
    #endif // __PYGMYNEBULA
    // Pin isn't an alias, check for port letter and pin number
    if( replaceChars( ucBuffer, "pP", ' ' ) ){
        if( replaceChars( ucBuffer, "aA", ' ' ) ){
            ucPin = 0;
        } else if( replaceChars( ucBuffer, "bB", ' ' ) ){
            ucPin = 16;
        } else if( replaceChars( ucBuffer, "cC", ' ' ) ){
            ucPin = 32;
        } else if( replaceChars( ucBuffer, "dD", ' ' ) ){
            ucPin = 48;
        } else if( replaceChars( ucBuffer, "eE", ' ' ) ){
            ucPin = 64;
        } else if( replaceChars( ucBuffer, "fF", ' ' ) ){
            ucPin = 80;
        } else{
            return( 0xFF );
        } // else
        return( ucPin + convertStringToInt( ucBuffer ) );
    } // if

    return( 0xFF );
}

u8 convertStringToPort( u8 *Buffer )
{
    Buffer = removeLeadingWhitespace( Buffer );
    
    #ifdef __PYGMYSTREAMCOM1
    if( !strcmp( Buffer, "COM1" ) ){
        return( COM1 );
    } // if
    #endif // __PYGMYSTREAMCOM1
    #ifdef __PYGMYSTREAMCOM2
    if( !strcmp( Buffer, "COM2" ) ){
        return( COM2 );
    } // if
    #endif // __PYGMYSTREAMCOM2
    #ifdef __PYGMYSTREAMCOM3
    if( !strcmp( Buffer, "COM3" ) ){
        return( COM3 );
    } // if
    #endif // __PYGMYSTREAMCOM3
    #ifdef __PYGMYSTREAMCOM4
    if( !strcmp( Buffer, "COM4" ) ){
        return( COM4 );
    } // if
    #endif // __PYGMYSTREAMCOM4
    #ifdef __PYGMYSTREAMCOM5
    if( !strcmp( Buffer, "COM5" ) ){
        return( COM5 );
    } // if
    #endif // __PYGMYSTREAMCOM5
    #ifdef __PYGMYSTREAMCOM6
    if( !strcmp( Buffer, "COM6" ) ){
        return( COM6 );
    } // if
    #endif // __PYGMYSTREAMCOM6

    return( STDIO );
}

u32 convertDateStringToSeconds( u8 *Buffer )
{
    // This function converts a Date/Time string to seconds
    //"11/20/2003 12:48:00"
    //u16 Year;
    //u8 Month;
    //u8 Day;
    //u8 Hour;
    //u8 Minute;
    //u8 Second;
    PYGMYTIME Time;
    u32 Seconds;
    u8 *TmpString, TmpBuffer[8];

    TmpString = TmpBuffer;
    TmpString = (u8*)strtok ( Buffer, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Month = convertStringToInt( TmpString );
    TmpString = strtok ( NULL, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Day = convertStringToInt( TmpString );
    TmpString = strtok ( NULL, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Year = convertStringToInt( TmpString );
    TmpString = strtok ( NULL, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Hour = convertStringToInt( TmpString );
    TmpString = strtok ( NULL, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Minute = convertStringToInt( TmpString );
    TmpString = strtok ( NULL, ": /" );
    if( !TmpString ){
        return( FALSE );
    } // if
    Time.Second = convertStringToInt( TmpString );

    Seconds = convertSystemTimeToSeconds( &Time );
    
    return( Seconds );
}

void copyBuffer( u8 *ucFrom, u8 *ucTo, u16 uiLen )
{
    u32 i;

    for( i = 0; i < uiLen; i++ ){
        *(ucTo++) = *(ucFrom++);
    } // for
}

u8 *stripLeadingChars( u8 *ucString, u8 *ucChars )
{    
    for( ; *ucString; ucString++){
       if(!isCharInString( *ucString, ucChars ) ){
            break;
        } // if
    } // for
    
    return( ucString );
}

u8 *splitString( u8 *ucString, u8 ucChar, s16 sCount )
{
    // Returns pointer to second half of split string
    u16 i, uiLen;
    s16 sInc;
    //u8 *ucStart;

    uiLen = len( ucString );
    //ucStart = ucString;
    sInc = -1;
    if( sCount < 0 ){
        sInc = 1;
        ucString = ucString + uiLen;
    } // if
    for( i = 0; i < uiLen && sCount != 0; i++ ){
        if( *ucString == ucChar ){
            sCount += sInc;
        } // if
        if( sInc < 0 ){
            --ucString;
        } else{
            ++ucString;
        } // else
    } // for
    
    return( ucString );
}

void convertU16ToBuffer( u16 uiData, u8 *ucBuffer, u8 ucEndian )
{
    if( ucEndian == BIGENDIAN ){
        *(ucBuffer++) = (u8)((u16) uiData >> 8 );
        *(ucBuffer++) = (u8)uiData;
    } else {
        *(ucBuffer++) = (u8)uiData;
        *(ucBuffer++) = (u8)((u16) uiData >> 8 );
    } // else
}

void convertU32ToBuffer( u32 ulData, u8 *ucBuffer, u8 ucEndian )
{
    if( ucEndian == BIGENDIAN ){
        *(ucBuffer++) = (u8)((u32) ulData >> 24 );
        *(ucBuffer++) = (u8)((u32) ulData >> 16 );
        *(ucBuffer++) = (u8)((u32) ulData >> 8 );
        *(ucBuffer++) = (u8) ulData;
    } else {
        *(ucBuffer++) = (u8) ulData;
        *(ucBuffer++) = (u8)((u32) ulData >> 8 );
        *(ucBuffer++) = (u8)((u32) ulData >> 16 );
        *(ucBuffer++) = (u8)((u32) ulData >> 24 );
    } // else
}

u16 convertBufferToU16( u8 *ucBuffer, u8 ucEndian )
{
    u16 uiData;

    if( ucEndian == BIGENDIAN ){
        uiData = (u16)ucBuffer[ 0 ] << 8;
        uiData |= (u16)ucBuffer[ 1 ];
    } else {
        uiData = (u16)ucBuffer[ 0 ];
        uiData |= (u16)ucBuffer[ 1 ] << 8;
    } // else

    return( uiData );
}

u32 convertBufferToU32( u8 *ucBuffer, u8 ucEndian )
{
    u32 ulData;

    if( ucEndian == BIGENDIAN ){
        ulData = (u32)ucBuffer[ 0 ] << 24;
        ulData |= (u32)ucBuffer[ 1 ] << 16;
        ulData |= (u32)ucBuffer[ 2 ] << 8;
        ulData |= (u32)ucBuffer[ 3 ]; 
    } else{
        ulData = (u32)ucBuffer[ 0 ];
        ulData |= (u32)ucBuffer[ 1 ] << 8;
        ulData |= (u32)ucBuffer[ 2 ] << 16;
        ulData |= (u32)ucBuffer[ 3 ] << 24;
    } // else

    return( ulData );
}

u32 countCharInString( u8 Char, u8 *String )
{
    u32 i, Matches;

    for( i = 0, Matches = 0; String[ i ]; i++ ){
        if( String[ i ] == Char ){
            ++Matches;
        } // if
    } // for

    return( Matches );
}

u8 *removeLeadingWhitespace( u8 *ucString )
{
    for( ; *ucString; ){
        if( isWhitespace( *ucString ) ){
            ++ucString;
        } else{
            break;
        } // else
    }

    return( ucString );
}

u8 isPrintable( u8 ucChar )
{
    if( ucChar > 31 && ucChar < 127 ){
        return( TRUE );
    } // if

    return( FALSE );
}

s8 isCharInString( u8 ucChar, u8 *ucChars )
{
    for( ; *ucChars; ) {
        if( ucChar == *(ucChars++) ){
            return( TRUE );
        } // if
    } // for
    
    return( FALSE );
}

s8 isAlpha( u8 ucChar )
{
    if( ( ucChar > 64 && ucChar < 91 ) || ( ucChar > 96 && ucChar < 123 ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

s8 isNumeric( u8 ucChar )
{
    if( ucChar > 47 && ucChar < 58 ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isAlphaOrNumeric( u8 ucChar )
{
    if( ( ucChar > 64 && ucChar < 91 ) || ( ucChar > 96 && ucChar < 123 )
        || ( ucChar > 47 && ucChar < 58 ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

s8 isHex( u8 ucChar )
{
    if( ( ucChar > 47 && ucChar < 58 ) || ( ucChar > 64 && ucChar < 71 ) ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isBinary( u8 ucChar )
{
    if( ucChar == '0' || ucChar == '1' ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isOctal( u8 ucChar )
{
    if( ucChar > 47 && ucChar < 56 ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isNewline( u8 ucChar )
{
    if( ucChar == 10 || ucChar == 12 || ucChar == 13 ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isWhitespace( u8 ucChar )
{
    if( ( ucChar > 7 && ucChar < 33 ) ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

s8 isQuote( u8 ucChar )
{
    if( ( ucChar == 34 ) || ( ucChar == 39 ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
    
}

s8 isMath( u8 ucChar )
{
    if( isCharInString( ucChar, "*/%&^|+=<>()%~" ) ){
        return( 1 );
    } // if
        
    return( FALSE );
}

u8 isFileSeparator( u8 ucChar )
{
    if( ucChar == '/' ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

s8 isSeparator( u8 ucChar )
{
    if( isCharInString( ucChar, "/\\{}[]-_+=@`|<>'\"" ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

s8 isPunctuation( u8 ucChar )
{
    if( isCharInString( ucChar, "!?,.:;" ) ){
        return( TRUE );
    } // if
        
    return( FALSE );
}

u16 len( u8 *ucString )
{
    u16 i;
    
    for( i = 0; *(ucString++); i++ ){
        if( i == 0xFFFF ){
            break;
        } // if
    } // for
    
    return( i );
}

u8 convertCharToUpper( u8 ucChar )
{
    if( ucChar > 64 && ucChar < 91 ){
        ucChar += 32;
    } // if

    return( ucChar );
}

u8 convertCharToLower( u8 ucChar )
{
    if( ucChar > 96 && ucChar < 123 ){
        ucChar -= 32;
    } // if

    return( ucChar );
}

u8 isCharSameIgnoreCase( u8 ucChar1, u8 ucChar2 )
{
    if( convertCharToUpper( ucChar1 ) == convertCharToUpper( ucChar2 ) ){
       return( 1 );
    } // if

    return( 0 );
}

u8 isStringSameIgnoreCase( u8 *ucString1, u8 *ucString2 )
{
    if( len( ucString1 ) != len( ucString2 ) )
        return ( 0 );
    
    for( ; *ucString1; ){
        if( !isCharSameIgnoreCase( *(ucString1++), *(ucString2++) ) ){
            return( 0 );
        } // if
    } // for
    
    return( 1 );
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

u8 getAllParameters( u8 *Buffer, PYGMYPARAMLIST *Parameters ) 
{
    // This function breaks a buffer of chars into an array of strings
    // ParamArray is allocated by this function and must be freed by the calling function
    // parameters are separated by double quotes and spaces
    u32 i, ii, BufferLen;
    u8 Quotes;
    
    Parameters->SwitchCount = 0;
    Parameters->ParamCount = 0;
    Quotes = FALSE;
    BufferLen = len( Buffer );

    if( BufferLen < 1 ){
        return( FALSE );
    } // if
    Parameters->Switches = malloc( sizeof( void * ) );
    if( !Parameters->Switches ){
        return( FALSE );
    } // if
    Parameters->Params = malloc( sizeof( void * ) );
    if( !Parameters->Params ){
        free( Parameters->Switches );
        return( FALSE );
    } // if
    
    for( i = 0; i < BufferLen; i++ ){
        // Skip spaces until a parameter or name is found
        // Parameters start with a - or --
        if( Buffer[ i ] == '-' ){
            // This is a parameter
            ++i;
            if( Buffer[ i ] == '-' ){
                ++i; // This parameter is prefixed with --
            } // if
            Parameters->Switches = realloc( Parameters->Switches, ( Parameters->SwitchCount + 1 ) * 4 );
            if( !Parameters->Switches ){
                freeParameterList( Parameters );
                return( FALSE );
            } // if
            
            Parameters->Switches[ Parameters->SwitchCount ] = malloc( 2 );
            if( !Parameters->Switches[ Parameters->SwitchCount ] ){
                // If memory allcoation failed, cleanup and return
                freeParameterList( Parameters );
                return( FALSE );
            } // if
            
            // copy the parameter, break on whitespace or end of buffer
            for( ii = 0; ii < BufferLen && Buffer[ i ] != ' ' && Buffer[ i ] != ','; ii++, i++ ){
            //for( ii = 0; ii < BufferLen && Buffer[ i ] != ' '; ii++, i++ ){
                // allocate another byte for the next char
                Parameters->Switches[ Parameters->SwitchCount ] = realloc( Parameters->Switches[ Parameters->SwitchCount ], 1 + ( ii + 2 ) );
                // append the char
                Parameters->Switches[ Parameters->SwitchCount ][ ii ] = Buffer[ i ];
            } // for
            Parameters->Switches[ (Parameters->SwitchCount)++ ][ ii ] = '\0'; // Terminate string and increment parameter counter
        } else if( Buffer[ i ] != ' ' &&  Buffer[ i ] != ',' ){
            // This is a name
            if( Buffer[ i ] == '\"' ){
                Quotes = TRUE;
                ++i;
            } else{
                Quotes = FALSE;
            } // else
            Parameters->Params = realloc( Parameters->Params, ( Parameters->ParamCount + 1 ) * 4 );
            Parameters->Params[ Parameters->ParamCount ] = malloc( 2 );
            if( !Parameters->Params || !Parameters->Params[ Parameters->ParamCount ] ){
                // memory allocation failed, cleanup and return with error
                freeParameterList( Parameters );
                return( FALSE );
            } // if
            // Copy the name
            for( ii = 0; ii < BufferLen; ii++, i++ ){
                Parameters->Params[ Parameters->ParamCount ] = realloc( Parameters->Params[ Parameters->ParamCount ], ii + 3 );
                if( !Parameters->Params[ Parameters->ParamCount ] ){
                    freeParameterList( Parameters );
                    return( FALSE );
                } // if
                
                if( Quotes ){
                    if( Buffer[ i ] == '\"' ){
                        ++i;
                        Quotes = FALSE;
                        break; // The end of string has been reached, terminate the string and break
                    } // if
                    if( Buffer[ i ] == '\\' ){
                        ++i;
                        if( Buffer[ i ] == 'r'){
                            print( COM3, "\rFound Escape Sequence: Carriage Return" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x0D;  // carriage return escape sequence
                        } else if( Buffer[ i ] == 't' ){ 
                            print( COM3, "\rFound Escape Sequence: Tab" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x09;  // horizontal tab escape sequence
                        } else if( Buffer[ i ] == 'n' ){          
                            print( COM3, "\rFound Escape Sequence: Newline" );
                            Parameters->Params[ Parameters->ParamCount ][ ii++ ] = 0x0D;// For a newline, add a carriage return followed by a linefeed
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x0A;
                            ++BufferLen; // Increase BufferLen to accommodate the additional character
                        } else if( Buffer[ i ] == 'l' ){
                            print( COM3, "\rFound Escape Sequence: Line Feed" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x0A;  // line feed escape sequence
                        } else if( Buffer[ i ] == 'f' ){
                            print( COM3, "\rFound Escape Sequence: Form Feed" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x0C;  // form feed/page break escape sequence
                        } else if( Buffer[ i ] == '\"' ){
                            print( COM3, "\rFound Escape Sequence: Double Quote" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x22;  // double quotes escape sequence
                        } else if( Buffer[ i ] == '\'' ){
                            print( COM3, "\rFound Escape Sequence: Single Quote" );
                            Parameters->Params[ Parameters->ParamCount ][ ii ] = 0x27;  // single quotes escape sequence
                        } // else if
                        continue;
                    } // if
                } else if( Buffer[ i ] == ' ' ){
                    break; // The end of string has been reached, terminate the string and break
                } // else
                Parameters->Params[ Parameters->ParamCount ][ ii ] = Buffer[ i ];
            } // for
            Parameters->Params[ Parameters->ParamCount++ ][ ii ] = '\0'; // Terminate string
        } // else if
    } // for

    return( TRUE );
}

void freeParameterList( PYGMYPARAMLIST *Parameters )
{
    u32 i;
  
    return;
    if( Parameters->Params ){
        for( i = 0; i < Parameters->ParamCount; i++ ){ 
            if( Parameters->Params[ i ] ){
                free( Parameters->Params[ i ] );
            } // if
        } // for
        free( Parameters->Params );
    } // if
    if( Parameters->Switches ){
        for( i = 0; i < Parameters->SwitchCount; i++ ){
            if( Parameters->Switches[ i ] ){
                free( Parameters->Switches[ i ] );
            } // if
        } // for
        free( Parameters->Switches );
    } // if
    
}

u16 getAllSubStrings( u8 *ucBuffer, u8 *ucStrings[], u16 uiLen, u8 ucMode )
{
    u16 i;
    u8 *ucSub;

    // First clear string buffer to prevent memory errors later
    for( i = 0; i < uiLen; i++ ){
        ucStrings[ i ] = NULL;
    } // for
    ucSub = getNextSubString( ucBuffer, ucMode );
    for( i = 0; i < uiLen && ucSub; i++ ){
        ucStrings[ i ] = ucSub;
        ucSub = getNextSubString( NULL, ucMode );
    } // for

    return( i );
}

u8 *getNextSubString( u8 *ucBuffer, u8 ucMode )
{
    static u8 *ucIndex = NULL;
    static u8 *ucBufferCopy = NULL;
    u8 *ucStart;

    if( ucBuffer ){
        if( ucBufferCopy ){
            //sysFree( ucBufferCopy );
            free( ucBufferCopy );
        } // if
        //ucBufferCopy = sysAllocate( 1 + len( ucBuffer ) );
        ucBufferCopy = malloc( 1 + len( ucBuffer ) );
        if( !ucBufferCopy ){
            return( NULL );
        } // if
        
        copyString( ucBuffer, ucBufferCopy );
        ucIndex = ucBufferCopy;
        
    } // if
    if( ucIndex == NULL ){
        return( NULL );
    } // if
    
    for( ; *ucIndex;  ){
        if( ( isWhitespace( *ucIndex ) ) || 
            ( (ucMode & FILESEPARATORS) && isFileSeparator( *ucIndex ) ) ||
            ( (ucMode & PUNCT) && isPunctuation( *ucIndex ) ) ||
            ( (ucMode & SEPARATORS) && isSeparator( *ucIndex ) ) ||
            ( (ucMode & QUOTES) && isQuote( *ucIndex ) ) ||
            ( (ucMode & COMMA) && *ucIndex == ',' ) ){
            ++ucIndex;
            continue;
            } // if
        ucStart = ucIndex;
        
        for( ; *ucIndex ; ){
            if( ( (ucMode & WHITESPACE) && isWhitespace( *ucIndex ) ) || 
                ( (ucMode & FILESEPARATORS) && *ucIndex == '/' ) ||
                ( (ucMode & PUNCT) && isPunctuation( *ucIndex ) ) ||
                //( (ucMode & PUNCT) && isFileSeparator( *ucIndex ) ) ||
                ( (ucMode & SEPARATORS) && isSeparator( *ucIndex ) ) ||
                ( (ucMode & QUOTES) && isQuote( *ucIndex ) ) ||
                ( (ucMode & NEWLINE) && isNewline( *ucIndex ) ) ||
                ( (ucMode & COMMA) && *ucIndex == ',' ) ||
                ( (ucMode & ALPHA) && isAlpha( *ucIndex ) ) ){
                break;
            } // if
            ++ucIndex;
        } // for
        if( *ucIndex ){
            *(ucIndex++) = '\0';
        } else{
            ucIndex = NULL;
        } // else
    
        return( ucStart );
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

double convertStringToFloat( u8 *ucBuffer )
{
    // supports 20 chars precision for numerator and denominator plus terminator
    // ucBuffer must be copied before modifying, ucBuffer may be const
    double dNum, dDenom, dSign = 1.0;
    u8 i, ucNum[21], ucDenom[21]; 

    if( *ucBuffer == '-' ){
        dSign = -1.0;
        ++ucBuffer;
    } else if( *ucBuffer == '+' ){
        ++ucBuffer;
    } // else if

    for( i = 0; i < 21 && *ucBuffer && *ucBuffer != '.'; i++ ){
        ucNum[ i ] = *(ucBuffer++);
    } // for
    ucNum[ i ] = '\0'; // Null Terminate
    if( *ucBuffer == '.' ){
        ++ucBuffer;
    } // if
    for( i = 0; i < 21 && *ucBuffer; i++ ){
        ucDenom[ i ] = *(ucBuffer++);
    } // for
    ucDenom[ i ] = '\0'; // Null Terminate
    dNum    = (double)convertStringToInt( ucNum );
    dDenom  = (double)convertStringToInt( ucDenom );
    
    for( ; i; i-- ){
        dDenom /= 10.0;
    } // for
    dNum = ( dNum + dDenom ) * dSign; 

    return( dNum );
}

void convertFloatToString( double fData, u8 *ucFormat, u8 *ucBuffer )
{
    // Function only supports decimal floating point representations
    double dNum, dDenom;
	s32 lNum; 
    u32 lDenom;
	u8 ucPlaces, ucRPlaces, ucTmpFormat[16], ucIntFormat[8], ucLen, *ucParams[ 2 ];

    dDenom = modf( fData, &dNum ); 
    // First split float 
    lNum = (s32)fData;
    dDenom = fabs( modf( fData, &dNum ) ); 
    // Next generate format strings for integer representation of numerator"%4.4f"
    copyString( ucFormat, ucTmpFormat );
    copyString( "%", ucIntFormat );
    ucFormat = ucTmpFormat;
    if( *ucFormat == '%' ){
        ucFormat += 1;
    } // if
    ucLen = getAllSubStrings( ucFormat, ucParams, 2, PUNCT|ALPHA );
    if( ucLen ){
        appendString( ucParams[ 0 ], ucIntFormat );
    } // if
    appendString( "d", ucIntFormat );
    // Output numerator
    convertIntToString( lNum, ucIntFormat, ucBuffer );
    appendString( ".", ucBuffer );
    // Find places for denomiator and scale
    if( ucLen == 2 ){
        copyString( ucParams[ 1 ], ucIntFormat+2 );
        ucPlaces = convertStringToInt( ucParams[ 1 ] );
        ucLen = ucPlaces;
    } else{
        ucLen = DBL_DIG;
    } // else
    for( ucRPlaces = 0, lDenom = 0; ucRPlaces < ucLen; ucRPlaces++ ){
        dDenom *= 10;
        dDenom = modf( dDenom, &dNum );
        if( floatIsEqual( dDenom, 0 ) ){
            break;  
        } // if
        lDenom = ( lDenom * 10 ) + (u32)dNum;
    } // for
    if( ucLen == DBL_DIG ){
        ucRPlaces = ucPlaces;
    } // if
    // Modify format string for denominator 
    copyString( "%0", ucIntFormat );
    convertIntToString( ucRPlaces, "%d", ucIntFormat+2 );
    appendString( "d", ucIntFormat );
    // Output denominator
    convertIntToString( lDenom, ucIntFormat, ucBuffer+len( ucBuffer ) );
}

void convertIntToString( s64 lData, u8 *ucFormat, u8 *ucBuffer )
{
    u8 ucTmpFormat[ 9 ];
    s64 i, iType, iLen, iMagnitude, iValue;
    u8 ucPadding;

    //print( COM3, "(%s)", ucFormat );
    for( i = 0; i < 8 && ucFormat[ i ]; i++ ){
        ucTmpFormat[ i ] = ucFormat[ i ];
    } // for
    ucTmpFormat[ i ] = '\0';
    ucFormat = ucTmpFormat;
    
    if( lData < 0 ){
        *(ucBuffer++) = '-';
        lData = ~((u64)lData) + 1; // reverse 2s complement
    } // if
    if( *ucFormat == '%' ){
        ++ucFormat;
    } // if
   
    if( *ucFormat == '0' ){
        ucPadding = *ucFormat;
        ++ucFormat;
    } else{
        ucPadding = ' ';
    } // if
    
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
    if( iLen < 0 ){
        iLen = 0;
    } // if
    for( i = 0, iMagnitude = 1; ( iMagnitude * iType ) <= lData; i++ ){
        iMagnitude *= iType;
    } // for
    for( ; iLen > i; iLen-- ){
        *(ucBuffer++) = ucPadding; //'0';
    } // for
    /*putcUSART3( '(' );
    putcUSART3( 48 + i );
    putcUSART3( ')' );
    putcUSART3( '(' );*/
    for( ; i>=0; i-- ){
        iValue = lData / iMagnitude;
        *(ucBuffer++) = PYGMYHEXCHARS[ iValue ];
        //putcUSART3( PYGMYHEXCHARS[ iValue ] );
        lData -= ( iValue * iMagnitude );
        iMagnitude /= iType;
    } // for
    //putcUSART3( ')' );
    *ucBuffer = '\0';
}

u8 replaceLastChar( u8 *ucString, u8 ucChar, u8 ucNewChar )
{
    // Warning! Do not use with constant strings!
    // Replaces last occurance of ucChar with ucNewChar
    // Returns location of char
    u16 i;

    i = len( ucString );
    for( ; i; ){
        if( ucString[ --i ] == ucChar ){
            ucString[ i ] = ucNewChar;
            break;
        } // if
    } // for

    return( i );
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


