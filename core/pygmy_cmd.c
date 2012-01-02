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

u8 *globalCMDPrompt, *globalCMDError, *globalCMDUnsupported;

const PYGMYCMD PYGMYSTDCOMMANDS[] = { 
                                    {(u8*)"reset",      cmd_reset},
                                    {(u8*)"peek",       cmd_peek},
                                    {(u8*)"poke",       cmd_poke},
                                    {(u8*)"ps",         cmd_ps},
                                    {(u8*)"kill",       cmd_kill},
                                    //{(u8*)"recv",       cmd_recv}, // XModem
                                    //{(u8*)"send",       cmd_send}, // XModem
                                    
                                    {(u8*)"pinset",     cmd_pinset},
                                    {(u8*)"pinget",     cmd_pinget},
                                    {(u8*)"pinanalog",  cmd_pinanalog},
                                    {(u8*)"pinpwm",     cmd_pinpwm},
                                    {(u8*)"pinconfig",  cmd_pinconfig},
                                    
                                    {(u8*)"format",     cmd_format},
                                    {(u8*)"rm",         cmd_rm},
                                    {(u8*)"ls",         cmd_ls},
                                    {(u8*)"mv",         cmd_mv},
                                    {(u8*)"open",       cmd_open},
                                    {(u8*)"read",       cmd_read},
                                    {(u8*)"write",      cmd_write},
                                    
                                    
                                    {(u8*)"rfopen",     cmd_rfopen},
                                    {(u8*)"rfsend",     cmd_rfsend},
                                    
                                    
                                    {(u8*)"", cmdNull} // No Commands after NULL
                                    }; 


//--------------------------------------------------------------------------------------------
//------------------------------------Pygmy CMD Interface-------------------------------------
#ifdef __PYGMYSTREAMCOM1
void cmdGetsCOM1( void )
{
    u8 pygmySTDIO, ucBuffer[ __PYGMYCOM1BUFFERLEN ];

    pygmySTDIO = streamGetSTDIO( );
    streamSetSTDIO( COM1 );
    streamFIFOToString( COM1, ucBuffer );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO( pygmySTDIO ); 
}
#endif // __PYGMYSTREAMCOM1

#ifdef __PYGMYSTREAMCOM2
void cmdGetsCOM2( void )
{
    u8 pygmySTDIO, ucBuffer[ __PYGMYCOM2BUFFERLEN ];

    pygmySTDIO = streamGetSTDIO( );
    streamSetSTDIO( COM2 );
    streamFIFOToString( COM2, ucBuffer );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO( pygmySTDIO ); 
}
#endif // __PYGMYSTREAMCOM2

#ifdef __PYGMYSTREAMCOM3
void cmdGetsCOM3( void )
{
    u8 pygmySTDIO, ucBuffer[ __PYGMYCOM3BUFFERLEN ];

    pygmySTDIO = streamGetSTDIO( );
    streamSetSTDIO( COM3 );
    streamFIFOToString( COM3, ucBuffer );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO( pygmySTDIO ); 
}
#endif // __PYGMYSTREAMCOM3

#ifdef __PYGMYSTREAMCOM4
void cmdGetsCOM4( void )
{
    u8 pygmySTDIO, ucBuffer[ __PYGMYCOM4BUFFERLEN ];

    pygmySTDIO = streamGetSTDIO( );
    streamSetSTDIO( COM4 );
    streamFIFOToString( COM4, ucBuffer );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO( pygmySTDIO ); 
}
#endif // __ PYGMYSTREAMCOM4

#ifdef __PYGMYSTREAMCOM5
void cmdGetsCOM5( void )
{
    u8 pygmySTDIO, ucBuffer[ __PYGMYCOM5BUFFERLEN ];

    pygmySTDIO = streamGetSTDIO( );
    streamSetSTDIO( COM5 );
    streamFIFOToString( COM5, ucBuffer );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO( pygmySTDIO ); 
}
#endif // __PYGMYSTREAMCOM5

void cmdInit( void )
{
    u16 i;

    #ifndef __PYGMYCMDMAXLISTS
        #define __PYGMYCMDMAXLISTS 1
    #endif // __PYGMYCMDMAXLISTS
    
    globalCMDPrompt = "\r> ";
    globalCMDError = "\rerror\r> ";
    globalCMDUnsupported = "\runsupported\r> ";
    print( STDIO, globalCMDPrompt );
}

u8 cmdExecute( u8 *ucBuffer, PYGMYCMD *pygmyCmds )
{
    u16 ii;
    u8 *ucCommand;
    
    ucCommand = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( !ucCommand ){
        print( STDIO, globalCMDPrompt );
        return( 0 );
    } // if
        
    for( ii = 0; 1; ii++ ){
        if( isStringSame( NULL, pygmyCmds[ ii ].Name ) ){
            print( STDIO, globalCMDUnsupported);
            return( 0 );
        } // if
        if( isStringSame( ucCommand, pygmyCmds[ ii ].Name ) ){
            // Parameters should not be passed here, they are passed using getNextSubString
            if( pygmyCmds[ ii ].Call( (u8*)"" ) ){
                print( STDIO, globalCMDPrompt );
                return( 1 );
            }else{
                print( STDIO, globalCMDError );
                return( 0 );
            } // else
        } // if
    } // for
}

//----------------------------------End Pygmy CMD Interface-----------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------Standard Commands-------------------------------------
//--------------------------------------------------------------------------------------------
u8 cmdNull( u8 *ucBuffer )
{
    return( 0 ); // No command, return error
}

u8 cmd_reset( u8 *ucBuffer )
{
    PYGMY_RESET;

    return( 0 ); // If reached, error has occurred
}

u8 cmd_peek( u8 *ucBuffer )
{

    return( 0 );
}

u8 cmd_poke( u8 *ucBuffer )
{

    return( 0 );
}

u8 cmd_ps( u8 *ucBuffer )
{
    
    return( 0 );
}

u8 cmd_kill( u8 *ucBuffer )
{

    return( 0 );
}

u8 cmd_time( u8 *ucBuffer )
{

    return( 0 );
}

//------------------------------------End Standard Commands-----------------------------------
//--------------------------------------------------------------------------------------------

//---------------------------------------Debug Commands-------------------------------------
//--------------------------------------------------------------------------------------------

u8 cmd_pinset( u8 *ucBuffer )
{
    u8 *ucParam, ucPin, ucState;
    
    ucPin = convertStringToPin( ucBuffer );
    ucParam = getNextSubString( NULL, WHITESPACE );
    if( isStringSame( ucParam, "high" ) || isStringSame( ucParam, "HIGH" ) || isStringSame( ucParam, "1" ) ){
        ucState = 1;
    } else{
        ucState = 0;
    } // else
    pinSet( ucPin, ucState );

    return( 1 );
}


u8 cmd_pinget( u8 *ucBuffer )
{
    print( STDIO, "\r%d", pinGet( convertStringToPin( ucBuffer ) ) );

    return( 1 );
}

u8 cmd_pinanalog( u8 *ucBuffer )
{
    if( !adcGetStatus( ) ){
        adcSingleSampleInit();
    } // if
    print( STDIO, "\r%d", adcSingleSample( convertStringToPin( getNextSubString( ucBuffer, WHITESPACE ) ) ) );

    return( 1 );
}

u8 cmd_pinpwm( u8 *ucBuffer )
{
    u8 ucPin, ucDutyCycle, ucFreq;

    ucPin = convertStringToPin( ucBuffer );
    ucFreq = convertStringToInt( getNextSubString( NULL, WHITESPACE ) );
    ucDutyCycle = convertStringToInt( getNextSubString( NULL, WHITESPACE ) );
    pinPWM( ucPin, ucFreq, ucDutyCycle );

    return( 1 );
}

u8 cmd_pinconfig( u8 *ucBuffer )
{
    u8 *ucParam, ucPin, ucMode;

    ucPin = convertStringToPin( ucBuffer );
    ucParam = getNextSubString( NULL, WHITESPACE );
    if( isStringSame( ucParam, "IN" ) || isStringSame( ucParam, "in" ) ){
        ucMode = IN;
    } else if( isStringSame( ucParam, "OUT" ) || isStringSame( ucParam, "out" ) ){
        ucMode = OUT;
    } else if( isStringSame( ucParam, "ANALOG" ) || isStringSame( ucParam, "analog" ) ){
        ucMode = ANALOG;
    } else if( isStringSame( ucParam, "PULLUP" ) || isStringSame( ucParam, "pullup" ) ){
        ucMode = PULLUP;
    } else if( isStringSame( ucParam, "PULLDOWN" ) || isStringSame( ucParam, "pulldown" ) ){
        ucMode = PULLDOWN;
    } else{
        return( 0 );
    } // else
    pinConfig( ucPin, ucMode );

    return( 1 );
}

//-------------------------------------End Debug Commands-------------------------------------
//--------------------------------------------------------------------------------------------

//------------------------------------Basic File Commands-------------------------------------
//--------------------------------------------------------------------------------------------
u8 cmd_mount( u8 *ucBuffer )
{
    fileMountRoot();
    
    return( 0 );
}

u8 cmd_format( u8 *ucBuffer )
{
    u8 *ucParam;
    
    ucParam = getNextSubString( ucBuffer, WHITESPACE );
    fileFormat( ucParam );

    return( 1 );
}

u8 cmd_read( u8 *ucBuffer )
{
    PYGMYFILE pygmyFile;
    u16 i;
    u8 *ucParam, ucChar;
    
    ucParam = getNextSubString( ucParam, WHITESPACE|NEWLINE );
    if( ucParam && fileOpen( &pygmyFile, ucParam, READ ) ){
        for( i = 0; !(pygmyFile.Attributes & EOF ); i++ ){
            ucChar = fileGetChar( &pygmyFile );
            if( !(pygmyFile.Attributes & EOF ) ){
                if( ucChar == '\r' ){
                    i = 0;
                } // if
                if( !( i % 80 ) ){
                    print( STDIO, "\r" );
                } // if
                if( ucChar != '\r' && ( ucChar < 32 || ucChar > 126 ) ){
                    print( STDIO, "(%d)", ucChar );
                } else{
                    print( STDIO, "%c", ucChar );
                } // else
            } // if
        } // 
        return( 1 );
    } // if

    return( 0 );
}

u8 cmd_write( u8 *ucBuffer )
{
    print( FILE, ucBuffer );

    return( 1 );
}

u8 cmd_ls( u8 *ucBuffer )
{
    u16 i, ii, uiLen, uiID;
    u8 ucName[ 16 ];

    for( i = 0; i < pygmyRootVolume.MaxFiles; i++ ){
        uiID = fileGetName( i, ucName );
        if( uiID ){
            print( STDIO, "\r%s", ucName ); 
            uiLen = 16 - len( ucName );
            for( ii = 0; ii < uiLen; ii++ ){
                print( STDIO, " " );
            } // for
            print( STDIO, "%d", fileGetLength( uiID ) );
        } // if
    } // for
    print( STDIO, "\rFree:           %d\r", fileGetFreeSpace() );
  
    return( 1 );
}

u8 cmd_rm( u8 *ucBuffer )
{
    return( fileDelete( getNextSubString( ucBuffer, WHITESPACE ) ) );
}

u8 cmd_open( u8 *ucBuffer )
{
    PYGMYFILE *pygmyFile;
    u8 *ucParam1, *ucParam2, ucMode;
    
    ucParam1 = getNextSubString( ucBuffer, WHITESPACE );
    ucParam2 = getNextSubString( NULL, WHITESPACE );
    if( ucParam2[ 0 ] == '-' ){
        ++ucParam2;
    } // if
    if( isCharInString( *ucParam2, "rR" ) ){
        ucMode = READ;
    } else if( isCharInString( *ucParam2, "aA" ) ){
        ucMode = WRITE;
    } else if( isCharInString( *ucParam2, "wW" ) ){
        ucMode = APPEND;
    } // else if
    pygmyFile = ( PYGMYFILE *)sysGetFile();

    return( fileOpen( pygmyFile, ucParam1, ucMode ) );
}

u8 cmd_mv( u8 *ucBuffer )
{
    u8 *ucParam1, *ucParam2;

    ucParam1 = getNextSubString( ucBuffer, WHITESPACE );
    ucParam2 = getNextSubString( NULL, WHITESPACE );

    return( fileRename( ucParam1, ucParam2 ) );
}

//----------------------------------End Basic File Commands-----------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------Basic RF Commands-------------------------------------
//--------------------------------------------------------------------------------------------
u8 cmd_rfscan( u8 *ucBuffer )
{
    
    return( 0 );
}

u8 cmd_rfopen( u8 *ucBuffer )
{
    u32 ulID;

    ulID = rfGetID();
    if( ulID == 0x41B40AA8 ){
        rfOpenSocket( 0x1FC60435, RF_COMLINK );
    } else {
        rfOpenSocket( 0x41B40AA8, RF_COMLINK );
    } // else
    
    return( 1 );
}

u8 cmd_rfsend( u8 *ucBuffer )
{
    u8 *ucParam;
    
    ucParam = getNextSubString( ucBuffer, NEWLINE );
    if( ucParam ){
        rfPutString( ucParam );
    } // if

    return( 1 );
}

//------------------------------------End Basic RFCommands------------------------------------
//--------------------------------------------------------------------------------------------

