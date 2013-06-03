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
//#include "profiles/nebula/shields/gasSensor.h"
//#include "profiles/digipots/mcp443x.h"
//#include "profiles/eeprom/at24hc02b.h"

extern u8 PYGMY_confirm[];
extern u8 globalHumidity[];
extern void volumeSet( void );

#ifdef __PYGMYCOMMANDS
u8 *globalCMDPrompt, *globalCMDError, *globalCMDUnsupported;
#ifndef __PYGMY_MAXCOMMANDSPIPORTS
    #define __PYGMY_MAXCOMMANDSPIPORTS  2
#endif
#ifndef __PYGMY_MAXCOMMANDI2CPORTS
    #define __PYGMY_MAXCOMMANDI2CPORTS  2
#endif
#ifndef __PYGMY_MAXCOMMANDPORTS
    #define __PYGMY_MAXCOMMANDPORTS __PYGMY_MAXCOMMANDSPIPORTS + __PYGMY_MAXCOMMANDI2CPORTS
#endif
PYGMYSPIPORT globalSPIPorts[ __PYGMY_MAXCOMMANDSPIPORTS ];
PYGMYI2CPORT globalI2CPorts[ __PYGMY_MAXCOMMANDI2CPORTS ];
PYGMYPORTINDEX globalCMDPorts[ __PYGMY_MAXCOMMANDPORTS ];

const PYGMYCMD PYGMYCORECOMMANDS[] = {
                                    {(u8*)"set",        cmd_set},
                                    {(u8*)"",           cmdNull}
                                    };

const PYGMYCMD PYGMYSTDCOMMANDS[] = { 
                                    {(u8*)"play",       cmd_play},
                                    {(u8*)"volume",     cmd_volume},
                                    {(u8*)"reset",      cmd_reset},
                                    {(u8*)"peek",       cmd_peek},
                                    {(u8*)"poke",       cmd_poke},
                                    #ifdef __PYGMYTASKS
                                    {(u8*)"ps",         cmd_ps},
                                    {(u8*)"kill",       cmd_kill},
                                    #endif // __PYGMYTASKS
                                    //{(u8*)"recv",       cmd_recv}, // XModem
                                    //{(u8*)"send",       cmd_send}, // XModem
                                    
                                    {(u8*)"pinevent",   cmd_pinevent},
                                    {(u8*)"pinset",     cmd_pinset},
                                    {(u8*)"pinget",     cmd_pinget},
                                    #ifdef __PYGMYANALOG
                                    {(u8*)"pinanalog",  cmd_pinanalog},
                                    #endif // __PYGMYANALOG
                                    {(u8*)"pinpwm",     cmd_pinpwm},
                                    {(u8*)"pinconfig",  cmd_pinconfig},
                                    #ifdef __PYGMYSTREAMFILE
                                    {(u8*)"mnt",        cmd_mnt},
                                    {(u8*)"format",     cmd_format},
                                    {(u8*)"rm",         cmd_rm},
                                    {(u8*)"ls",         cmd_ls},
                                    {(u8*)"mv",         cmd_mv},
                                    {(u8*)"open",       cmd_open},
                                    {(u8*)"close",      cmd_close},
                                    {(u8*)"read",       cmd_read},
                                    {(u8*)"write",      cmd_write},
                                    {(u8*)"cp",         cmd_cp},
                                    #endif    
                                
                                    {(u8*)"port",       cmd_port},
                                    #ifdef __PYGMYSOCKETS
                                    {(u8*)"rflist",     cmd_rflist},
                                    {(u8*)"rfget",      cmd_rfget},
                                    {(u8*)"rfput",      cmd_rfput},
                                    {(u8*)"rfopen",     cmd_rfopen},
                                    {(u8*)"rfsend",     cmd_rfsend},
                                    {(u8*)"rfscan",     cmd_rfscan},
                                    #endif //__PYGMYSOCKETS
                                    #ifdef __PYGMYMODEMSHIELD
                                    {(u8*)"modem",      cmd_modem},
                                    #endif
                                    #ifdef __PYGMYHUMIDITYSHIELD
                                    {(u8*)"humidity",   cmd_humidity},
                                    #endif
                                    #ifdef __PYGMYVOLTAGESHIELD
                                    {(u8*)"voltshield", cmd_voltshield},
                                    #endif
                                    #ifdef __PYGMYEEPROM
                                    {(u8*)"eeprom",     cmd_eeprom},
                                    #endif //__PYGMYEEPROM
                                    #ifdef __PYGMYGASSENSORSHIELD
                                    {(u8*)"gassensor",  cmd_gassensor},
                                    #endif // __PYGMYGASSENSORSHIELD
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

#ifdef __PYGMYSTREAMSOCKET
void cmdGetsSocket( void )
{
    u8 pygmySTDIO, *ucBuffer;
    
    ucBuffer = sysGetPrintSocketBuffer( );
    print( COM3, ucBuffer );
    pygmySTDIO = streamGetSTDIO();
    streamSetSTDIO( SOCKET );
    cmdExecute( ucBuffer, (PYGMYCMD*)PYGMYSTDCOMMANDS );
    streamSetSTDIO
}
#endif // __PYGMYSTREAMSOCKET

void cmdInit( void )
{
    u8 i;

    #ifndef __PYGMYCMDMAXLISTS
        #define __PYGMYCMDMAXLISTS 1
    #endif // __PYGMYCMDMAXLISTS
    /*for( i = 0; i < __PYGMY_MAXCOMMANDPORTS; i++ ){
        globalCMDPorts[ i ].SPI = 0xFF;
        globalCMDPorts[ i ].I2C = 0xFF;
    } // for
    for( i = 0; i < __PYGMY_MAXCOMMANDSPIPORTS; i++ ){
        globalSPIPorts[ i ].CR = 0xFF;
    } // for
    for( i = 0; i < __PYGMY_MAXCOMMANDI2CPORTS; i++ ){
        globalI2CPorts[ i ].CR = 0xFF;
    } // for
    */
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
        //if( isStringSame( 0, pygmyCmds[ ii ].Name ) ){
        if( isStringSame( NULL, pygmyCmds[ ii ].Name ) ){
            print( STDIO, globalCMDUnsupported);
            return( 0 );
        } // if
        if( isStringSame( ucCommand, pygmyCmds[ ii ].Name ) ){
            // Parameters should not be passed here, they are passed using getNextSubString
            if( pygmyCmds[ ii ].Call( ucBuffer + len(ucCommand) ) ){
            //if( pygmyCmds[ ii ].Call( (u8*)"" ) ){
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
u8 cmd_play( u8 *ucBuffer )
{
    //dac2Output( (u8*)PYGMY_confirm, 46065, 32000, 0 );
    //dac1Output( (u8*)PYGMY_confirm, 46065, 16000, 0 );

    return( TRUE );
}

u8 cmd_volume( u8 *ucBuffer )
{
    /*
    PYGMYI2CPORT pygmyI2C;
    u8 ucVol, ucAddress, ucLen, *ucParams[ 2 ];
    
    ucBuffer = removeLeadingWhitespace( ucBuffer );
    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );   

    if( ucLen != 2 ){
        if( isStringSame( "--run", ucParams[ 0 ] ) ){
            if( taskIsRunning( "volume" ) ){
                print( STDIO, "\rTask killed\rTask spawned" );
                taskDelete( "volume" );
                taskNew( "volume", 5000, 20000, 0, (void*)volumeSet );
            } else{
                print( STDIO, "\rTask spawned" );
                taskNew( "volume", 5000, 20000, 0, (void*)volumeSet );
            } // else
            return( TRUE );
        } // if
        print( STDIO, "\rEnter address and volume" );
        return( 0 );
    } // if 
    ucVol = convertStringToInt( ucParams[ 1 ] );
    if( ucVol > 0x3F ){
        print( STDIO, "\rMax volume is 63" );
        return( FALSE );
    } // if
    ucAddress = convertStringToInt( ucParams[ 0 ] );
    if( ucAddress != 0x4B && ucAddress != 0x4A && ucAddress != 0x49 ) {
        print( STDIO, "\rValid addresses are 0x4B, 0x4A, 0x49" );
        return( FALSE );
    } // if
    i2cConfig( &pygmyI2C, TX1, RX1, ucAddress, I2CSPEEDFAST );
    i2cStart( &pygmyI2C );
    i2cWriteByte( &pygmyI2C, pygmyI2C.Address );
    i2cWriteByte( &pygmyI2C, ucVol );
    i2cStop( &pygmyI2C );
    */
    return( TRUE );
}

u8 cmd_set( u8 *ucBuffer )
{
    u8 ucLen, *ucParams[8];
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 8, WHITESPACE );
    //if( isStringSame( 

    return( TRUE );
}

u8 cmd_set_mcuid( u8 *ucBuffer )
{

}

u8 cmd_set_adcvoltage( u8 *ucBuffer )
{

}

u8 cmd_set_xtal( u8 *ucBuffer )
{

}

u8 cmd_set_platform( u8 *ucBuffer )
{

}

u8 cmd_set_delaytimer( u8 *ucBuffer )
{

}

u8 cmd_set_pwmtimer( u8 *ucBuffer )
{

}


//--------------------------------------------------------------------------------------------
//------------------------------------End Standard Commands-----------------------------------


//--------------------------------------Standard Commands-------------------------------------
//--------------------------------------------------------------------------------------------
#ifdef __PYGMYVOLTAGESHIELD
u8 cmd_voltshield( u8 *ucBuffer )
{
    //u16 uiSample;
    u8 *ucParams[ 4 ], ucLen, ucChannel, ucGain;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 4, WHITESPACE );

    
    if( isStringSame( ucParams[ 0 ], "set" ) ){
        if( ucLen < 4 ){
            return( FALSE );
        } // if 
        ucChannel = convertStringToInt( ucParams[ 2 ] );
        if( isStringSame( ucParams[ 1 ], "gain" ) ){
            ucGain = convertStringToInt( ucParams[ 3 ] );
            voltShieldSetGain( ucChannel, ucGain );
        } else if( isStringSame( ucParams[ 1 ], "coupling" ) || isStringSame( ucParams[ 1 ], "couple" ) ){
            if( isStringSameIgnoreCase( ucParams[ 3 ], "ac" ) ){
                voltShieldSetCoupling( ucChannel, AC );
            }else if(isStringSameIgnoreCase( ucParams[ 3 ], "dc" ) ){
                voltShieldSetCoupling( ucChannel, DC );
            } else{
                return( FALSE );
            } // else
        } else{
            return( FALSE );
        } // else
    } else if( isStringSame( ucParams[ 0 ], "get" ) ){
        if( ucLen < 3 ){
            return( FALSE );
        } // if
        ucChannel = convertStringToInt( ucParams[ 2 ] );
        if( isStringSame( ucParams[ 1 ], "resistance" ) || isStringSame( ucParams[ 1 ], "res" ) ){
            //voltShieldSetCoupling( ucChannel, RESISTANCE );
            print( STDIO, "\r%d", voltShieldGetResistance( ucChannel ) );
        } else if( isStringSame( ucParams[ 1 ], "voltage" ) || isStringSame( ucParams[ 1 ], "volt" ) ){
            print( STDIO, "\r%d", voltShieldGetVoltage( ucChannel ) );
        } else{
            return( FALSE );
        } // else
    } else{
        return( FALSE );
    } // else

    return( TRUE );
}
#endif // __PYGMYVOLTAGESHIELD

#ifdef __PYGMYMODEMSHIELD
u8 cmd_modem( u8 *ucBuffer )
{
    //print( COM2, "%s\r", getNextSubString( ucBuffer, NEWLINE ) );
    
    return( cmdExecute( ucBuffer, PYGMYMODEMCMDS ) );
}
#endif // __PYGMYMODEMSHIELD

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
    PYGMYCOMMANDQUEUE *pygmyQueue;
    PYGMYMESSAGE pygmyMsg;
    PYGMYTASK *pygmyTask;
    u16 i, ii;
    /*u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|NEWLINE );
    
    if( ucParam ){
        if( isStringSame( ucParam, "-s" ) || isStringSame( ucParam, "--sockets" ) ){
            //rfListSockets();
        } else if( isStringSame( ucParam, "-m" ) || isStringSame( ucParam, "--messages" ) ){
            print( COM3, "\rMessages\r" );
            for( i = 0; i < PYGMY_MAXTASKS; i++ ){
                msgList( &pygmyMsg, i );
                if( pygmyMsg.DestID != PYGMY_BLANKID || pygmyMsg.DestName[ 0 ] ){
                    print( COM3, "\rMessage To: %s", pygmyMsg.DestName );
                    print( COM3, "\r\tMessage: %s", pygmyMsg.Message );
                    print( COM3, "\r\tValue: 0x%08X", pygmyMsg.Value );
                    print( COM3, "\r\tTime: %t", pygmyMsg.TimeStamp );
                } // if
            } // for    
        } // else if
    } // if
    */
    print( STDIO, "\rTasks:\r\r" );
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        pygmyTask = taskGetByIndex( i );
        if( pygmyTask ){
            println( STDIO, "%s %t", pygmyTask->Name, pygmyTask->TimeStamp );
        }
    } // for
    
    /*
    for( i = 0; i < PYGMY_MAXQUEUES; i++ ){
        
        pygmyQueue = cmdListQueue( i );
        if( !pygmyQueue ){
            continue;
        } // if
        print( STDIO, "\rCommand Queue\r\r" );
        for( ii = 0; ii < PYGMY_MAXCOMMANDS; ii++ ){
            if( pygmyQueue->Commands[ ii ].Status ){
                print( STDIO, "\r%s %d %t", pygmyQueue->Commands[ ii ].Name, 
                    pygmyQueue->Commands[ ii ].Retry, pygmyQueue->Commands[ ii ].TimeStamp );
            } // if
        } // for
         
    } // for
    */

    return( TRUE );
}

u8 cmd_kill( u8 *ucBuffer )
{
    /*PYGMYTASK *pygmyTask;
    u16 i;
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|NEWLINE );
    if( !ucParam ){
        return( FALSE );
    } // if
    ucBuffer = removeLeadingWhitespace( ucParam );
    print( STDIO, "\rucBuffer(%s)", ucParam );
    if( isStringSame( "--all", ucParam ) ){
        for( i = 0; i < PYGMY_MAXTASKS; i++ ){
            pygmyTask = taskGetByIndex( i );
            if( pygmyTask ){
                print( STDIO, "\rTerminating %s", pygmyTask->Name );
                taskDelete( pygmyTask->Name );
            } // if
        } // for
    } else{
        taskDelete( ucParam );
    } // else
    */
    return( TRUE );
}

u8 cmd_time( u8 *ucBuffer )
{

    return( 0 );
}

u8 cmd_pipe( u8 *ucBuffer )
{
    
    return( 0 );
}

//------------------------------------End Standard Commands-----------------------------------
//--------------------------------------------------------------------------------------------

//---------------------------------------Debug Commands-------------------------------------
//--------------------------------------------------------------------------------------------

void cmdPinEvent( void )
{
    u16 uiPR;
    u8 i, ucPin;

    uiPR = pinGetIRQPR();
    for( i = 0; i < 16; i++ ){
        if( uiPR & ( BIT0 << i ) ){
            ucPin = pinGetIRQPin( i );
            if( pinGet( ucPin ) ){
                print( STDIO, "\r%s HIGH%s", convertPinToString( ucPin ),  globalCMDPrompt );
            } else{
                print( STDIO, "\r%s LOW%s", convertPinToString( ucPin ), globalCMDPrompt );
            } // else
        } // if
    } // for
}

u8 cmd_pinevent( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], ucLen, ucPin;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    ucPin = convertStringToPin( ucParams[ 0 ] );
    if( ucLen > 1 && isStringSameIgnoreCase( ucParams[ 1 ], "off" ) ){
        // if second parameter is included and is "off"
        pinInterrupt( cmdPinEvent, ucPin, 0, 5 );
    } else{
        pinInterrupt( cmdPinEvent, ucPin, TRIGGER_RISING|TRIGGER_FALLING, 5 );
    } // else

    return( TRUE );
}

u8 cmd_pinset( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], ucPin;
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    ucPin = convertStringToPin( ucParams[ 0 ] );
    if( isStringSameIgnoreCase( ucParams[ 1 ], "high" ) || isStringSame( ucParams[ 1 ], "1" )
        || isStringSameIgnoreCase( ucParams[ 1 ], "on" ) ){
        pinSet( ucPin, 1 );
    } else if( isStringSameIgnoreCase( ucParams[ 1 ], "low" ) || isStringSame( ucParams[ 1 ], "0" )
        || isStringSameIgnoreCase( ucParams[ 1 ], "off" ) ){
        pinSet( ucPin, 0 );
    } else{
        return( FALSE );
    } // else

    return( TRUE );
}


u8 cmd_pinget( u8 *ucBuffer )
{
    print( STDIO, "\r%d", pinGet( convertStringToPin( getNextSubString( ucBuffer, WHITESPACE ) ) ) );

    return( 1 );
}

u8 cmd_pinanalog( u8 *ucBuffer )
{
    if( !adcGetStatus( ) ){
        adcSingleSampleInit();
    } // if
    print( STDIO, "\r%d", adcSingleSample( convertStringToPin( getNextSubString( ucBuffer, WHITESPACE ) ) ) );

    return( TRUE );
}

u8 cmd_pinpwm( u8 *ucBuffer )
{
    u8 *ucParams[ 3 ], ucPin, ucDutyCycle, ucFreq;

    getAllSubStrings( ucBuffer, ucParams, 3, WHITESPACE );
    ucPin = convertStringToPin( ucParams[ 0 ] );
    ucFreq = convertStringToInt( ucParams[ 1 ] );
    ucDutyCycle = convertStringToInt( ucParams[ 2 ] );
    pinPWM( ucPin, ucFreq, ucDutyCycle );

    return( TRUE );
}

u8 cmd_pinconfig( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], ucPin, ucMode;

    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    ucPin = convertStringToPin( ucParams[ 0 ] );
    if( isStringSameIgnoreCase( ucParams[ 1 ], "in" ) ){
        ucMode = IN;
    } else if( isStringSameIgnoreCase( ucParams[ 1 ], "out" ) ){
        ucMode = OUT;
    } else if( isStringSameIgnoreCase( ucParams[ 1 ], "analog" ) ){
        ucMode = ANALOG;
    } else if( isStringSameIgnoreCase( ucParams[ 1 ], "pullup" ) ){
        ucMode = PULLUP;
    } else if( isStringSameIgnoreCase( ucParams[ 1 ], "pulldown" ) ){
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
#ifdef __PYGMYSTREAMFILE
u8 cmd_mnt( u8 *ucBuffer )
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
    filePrintList( STDIO );
    return( 1 );
}

u8 cmd_rm( u8 *ucBuffer )
{
    return( fileDelete( getNextSubString( ucBuffer, WHITESPACE ) ) );
}

u8 cmd_close( u8 *ucBuffer )
{
    PYGMYFILE *pygmyFile;
    
    pygmyFile = ( PYGMYFILE *)sysGetFile();
    fileClose( pygmyFile );

    return( TRUE );
}

u8 cmd_open( u8 *ucBuffer )
{
    PYGMYFILE *pygmyFile;
    u8 *ucParams[ 2 ], ucMode;
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    
    if( ucParams[ 1 ][ 0 ] == '-' ){
        ++ucParams[ 1 ];
    } // if
    if( isCharInString( *ucParams[ 1 ], "rR" ) ){
        ucMode = READ;
    } else if( isCharInString( *ucParams[ 1 ], "aA" ) ){
        ucMode = APPEND;
    } else if( isCharInString( *ucParams[ 1 ], "wW" ) ){
        ucMode = WRITE;
    } // else if
    pygmyFile = ( PYGMYFILE *)sysGetFile();

    return( fileOpen( pygmyFile, ucParams[ 0 ], ucMode ) );
}

u8 cmd_mv( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], *ucParam2;

    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );

    return( fileRename( ucParams[ 1 ], ucParams[ 2 ] ) );
}

u8 cmd_cp( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ];
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    return( fileCopy( ucParams[ 0 ], ucParams[ 1 ] ) );
}
#endif // __PYGMYSTREAMFILE

//----------------------------------End Basic File Commands-----------------------------------
//--------------------------------------------------------------------------------------------

//----------------------------------------Port Commands---------------------------------------
//--------------------------------------------------------------------------------------------
u8 cmdOpenPort( u8 ucProtocol, u8 ucPin0, u8 ucPin1, u8 ucPin2, u8 ucPin3, u8 ucAddress )
{
    u8 i, ii;

    if( ucProtocol ){
        for( i = 0; i < __PYGMY_MAXCOMMANDPORTS; i++ ){
            if( globalCMDPorts[ i ].SPI == 0xFF && globalCMDPorts[ i ].I2C == 0xFF ){
                for( ii = 0; ii < __PYGMY_MAXCOMMANDSPIPORTS; ii++ ){
                    if( globalSPIPorts[ ii ].CR == 0xFF ){
                        globalCMDPorts[ i ].SPI = ii;
                        spiConfig( &globalSPIPorts[ globalCMDPorts[ i ].SPI ] , ucPin0, ucPin1, ucPin2, ucPin3, 0 );
                        return( i );
                    } // if
                } // for
            } // 
        } // if  
    } else{
        for( i = 0; i < __PYGMY_MAXCOMMANDPORTS; i++ ){
            if( globalCMDPorts[ i ].SPI == 0xFF && globalCMDPorts[ i ].I2C == 0xFF ){
                for( ii = 0; ii < __PYGMY_MAXCOMMANDI2CPORTS; ii++ ){
                    if( globalI2CPorts[ ii ].CR == 0xFF ){
                        globalCMDPorts[ i ].I2C = ii;
                        //print( STDIO, "\rOpening i2c on %d %d @ %d", ucPin0, ucPin1, ucAddress );
                        i2cConfig( &globalI2CPorts[ globalCMDPorts[ i ].I2C ] , ucPin0, ucPin1, ucAddress, 0 );
                        return( i );
                    } // if
                } // for
            } // 
        } // if
        //print( STDIO, "\rPort opened for address %d", ucAddress );
        //i2cConfig( &globalI2CPort, ucPin0, ucPin1, ucAddress, 0  );
    } // else

    return( 0xFF );
}

u8 cmdWritePort( u8 ucCOM, u32 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    ucCOM -= __PYGMY_FIRSTUSERCOMPORT;
    if( globalCMDPorts[ ucCOM ].SPI != 0xFF ){
        spiPutBuffer( &globalSPIPorts[ globalCMDPorts[ ucCOM ].SPI ], ulAddress, ucBuffer, uiLen );
        return( 1 );
    } else if( globalCMDPorts[ ucCOM ].I2C != 0xFF ){
        i2cPutBuffer( &globalI2CPorts[ globalCMDPorts[ ucCOM ].I2C ], (u8)ulAddress, ucBuffer, uiLen );
        return( 1 );
    } // else if
    
    return( 0 );
}

u8 cmdReadPort( u8 ucCOM, u32 ulAddress, u8 *ucBuffer, u16 uiLen )
{
    ucCOM -= __PYGMY_FIRSTUSERCOMPORT;
    if( globalCMDPorts[ ucCOM ].SPI != 0xFF ){
        spiReadBuffer( &globalSPIPorts[ globalCMDPorts[ ucCOM ].SPI ], ucBuffer, uiLen );
        return( 1 );
    } else if( globalCMDPorts[ ucCOM ].I2C != 0xFF ){
        i2cGetBuffer( &globalI2CPorts[ globalCMDPorts[ ucCOM ].I2C ], (u8)ulAddress, ucBuffer, uiLen );
        return( 1 );
    } // else if
    
    return( 0 );
}

u8 cmd_port( u8 *ucBuffer )
{
    u8 ucData[4], ucAction, ucAddress, ucCOM, ucProtocol, ucPin0, ucPin1, ucPin2, ucPin3, *ucParam;
    u16 i, uiLen;
    u8 *ucStrings[ 64 ];
    
    uiLen = getAllSubStrings( ucBuffer, ucStrings, 64, WHITESPACE ); 
    print( STDIO, "\r%d strings: ", uiLen );
    for( i = 0; i < uiLen; i++ ){
        print( STDIO, " (%s)", ucStrings[ i ] );
    } // for
    print( STDIO, "\rTesting action..." );
    if( isStringSameIgnoreCase( ucStrings[ 0 ], "open" ) ){
        print( STDIO, "Open detected\rTesting protocol..." );
        if( isStringSameIgnoreCase( ucStrings[ 1 ], "i2c" ) ){
            print( STDIO, "i2c detected" );
            ucProtocol = 0;
            ucPin0 = convertStringToPin( ucStrings[ 2 ] );
            ucPin1 = convertStringToPin( ucStrings[ 3 ] );
            ucAddress = convertStringToInt( ucStrings[ 4 ] );
        } else if( isStringSameIgnoreCase( ucStrings[ 1 ], "spi" ) ){
            print( STDIO, "spi detected" );
            ucProtocol = 1;
            ucPin0 = convertStringToPin( ucStrings[ 2 ] );
            ucPin1 = convertStringToPin( ucStrings[ 3 ] );
            ucPin2 = convertStringToPin( ucStrings[ 4 ] );
            ucPin3 = convertStringToPin( ucStrings[ 5 ] );
            print( STDIO, "\rCS=%d SCK=%d MISO=%d MOSI=%d", ucPin0, ucPin1, ucPin2, ucPin3 );
        } // else if
        print( STDIO, "\rOpening port..." );
        ucCOM = cmdOpenPort( ucProtocol, ucPin0, ucPin1, ucPin2, ucPin3, ucAddress );
        if( ucCOM != 0xFF ){
            print( STDIO, "\rCOM%d", __PYGMY_FIRSTUSERCOMPORT + ucCOM );
            return( 1 );
        } // else
    } else if( isStringSameIgnoreCase( ucStrings[ 0 ], "close" ) ){
        ucCOM = convertStringToInt( ucStrings[ 1 ] );
    } else if( isStringSameIgnoreCase( ucStrings[ 0 ], "send" ) || isStringSameIgnoreCase( ucStrings[ 0 ], "write" ) ){
        if( uiLen < 3 ){
            return( FALSE );
        } // if
        replaceChars( ucStrings[ 1 ], "COM", ' ' );
        ucCOM = convertStringToInt( ucStrings[ 1 ] );
        ucAddress = convertStringToInt( ucStrings[ 2 ] );
        print( STDIO, "\rSending %d Data Bytes to COM: %d, Address: %d, \r", uiLen - 3, ucCOM, ucAddress );
        for( i = 0; i < uiLen - 3; i++ ){
            ucData[ i ] = convertStringToInt( ucStrings[ i + 3 ] );
            print( STDIO, "%d (%d)\r", i, ucData[ i ] );
        } // for
        cmdWritePort( ucCOM, ucAddress, ucData, uiLen - 3 );
           
        return( 1 );
    } else if( isStringSameIgnoreCase( ucStrings[ 0 ], "read" ) ){
        if( uiLen < 4 ){
            return( FALSE );
        } // if
        replaceChars( ucStrings[ 1 ], "COM", ' ' );
        ucCOM = convertStringToInt( ucStrings[ 1 ] );
        
        ucAddress = convertStringToInt( ucStrings[ 2 ] );
        print( STDIO, "\rAddress: %d", ucAddress );
        uiLen = convertStringToInt( ucStrings[ 3 ] );
        print( STDIO, "\rLen: %d", uiLen );
            
        cmdReadPort( ucCOM, ucAddress, ucData, uiLen );
        print( STDIO, "\rData: " );
        for( i = 0; i < uiLen; i++ ){
            print( STDIO, " (%d)", ucData[ i ] );
        } //for
        return( 1 );
    } // else if
    
    return( 0 );
}

u8 cmd_humidity( u8 *ucBuffer )
{
    u8 ucLen, *ucParams[ 2 ];
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    if( ucLen > 1 ){
        socketSendControl( convertStringToInt( ucParams[ 0 ] ), ucParams[ 1 ] );
    
        return( TRUE );
    } // if
    
    /*PYGMYFILE pygmyFile;
    u8 ucBuf[ 20 ];
    
    if( fileOpen( &pygmyFile, "humidity.txt", WRITE ) ){
        convertIntToString( humidityRead(), "%d", ucBuf );
        filePutString( &pygmyFile, ucBuf );
        fileClose( &pygmyFile );
    } // if*/

    return( FALSE );
}

u8 cmd_psend( u8 *ucBuffer )
{
    

    return( 1 );
}

//--------------------------------------End Port Commands-------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------Basic RF Commands-------------------------------------
//--------------------------------------------------------------------------------------------
#ifdef __PYGMYSOCKETS
u8 cmd_rflist( u8 *ucBuffer )
{
    socketList();
    
    return( TRUE );
}

u8 cmd_rfscan( u8 *ucBuffer )
{
    copyString( "Clicked...", globalHumidity );
    drawForms();
    
    print( COM3, "\rLocal ID: %X", socketGetID() );
    //rfSendScanCommand( 0xFF );
    return( TRUE );
}

u8 cmd_rfget( u8 *ucBuffer )
{
    u8 ucLen, *ucParams[ 2 ];
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    if( ucLen > 1 ){
        print( STDIO, "\rRequesting File" );
        socketRequestFile( convertStringToInt( ucParams[ 0 ] ), ucParams[ 1 ] );
        print( COM3, "..." );
        //rfListSockets();
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 cmd_rfput( u8 *ucBuffer )
{
    u8 ucLen, *ucParams[ 2 ];
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    if( ucLen > 1 ){
        print( STDIO, "\rSending File" );
        socketSendFile( convertStringToInt( ucParams[ 0 ] ), ucParams[ 1 ] );
        //rfListSockets();
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 cmd_rfopen( u8 *ucBuffer )
{
    u32 ulID;
    u8 ucSocket;
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|NEWLINE|PUNCT );
    if( !ucParam ){
        return( 0 );
    } // if
    //socketOpenCommandLine( convertStringToInt( ucParam ) );
    

    return( 1 );
}

u8 cmd_rfsend( u8 *ucBuffer )
{
    PYGMYSOCKET *pygmySocket;
    u32 ulSocket;
    u8 ucLen, *ucParams[ 2 ];
    
    ucParams[ 0 ] = getNextSubString( ucBuffer, WHITESPACE );
    ucParams[ 1 ] = getNextSubString( NULL, NEWLINE );
    if( ucParams[ 1 ] ){
        ulSocket = convertStringToInt( ucParams[ 0 ] );
        pygmySocket = socketGet( ulSocket, ulSocket );
        ucLen = len( ucParams[ 1 ] );
        ucParams[ 1 ][ ucLen ] = '\r';
        ucParams[ 1 ][ ucLen + 1 ] = '\0';
        socketSendDataFromString( pygmySocket, ucParams[ 1 ] );
     
        return( TRUE );
    } // if
    
    return( FALSE );
}
#endif // __PYGMYSOCKETS
//------------------------------------End Basic RFCommands------------------------------------
//--------------------------------------------------------------------------------------------

u8 cmd_eeprom( u8 *ucBuffer )
{
    u8 i, ucChar, ucLen, ucMemory[ 128 ], *ucParams[ 4 ], *ucEeproms;
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 6, WHITESPACE );
    if( !ucLen ){
        return( FALSE );
    } // if
    if( isStringSame( ucParams[ 0 ], "list" ) ){
        if( ucLen == 3 ){
            eepromOpen( 0x50, convertStringToPin( ucParams[ 1 ] ), 
                convertStringToPin( ucParams[ 2 ] ), NONE );
        } else if( ucLen == 4 ){
            eepromOpen( convertStringToInt( ucParams[ 1 ] ), convertStringToPin( ucParams[ 2 ] ), 
                convertStringToPin( ucParams[ 3 ] ), NONE );
        } else{
            return( FALSE );
        } // else
        
        print( STDIO, "\rEEPROMs:" );
        ucEeproms = eepromQueryBus();
        for( i = 0; i < 8; i++ ){
            if( ucEeproms[ i ] ){
                print( STDIO, "\r0x%02X", ucEeproms[ i ] );
            } // if
        } // for
    } else if( isStringSame( ucParams[ 0 ], "open" ) ){
        if( ucLen < 4 ){
            return( FALSE );
        } // if
        eepromOpen( convertStringToInt( ucParams[ 1 ] ), convertStringToPin( ucParams[ 2 ] ), 
            convertStringToPin( ucParams[ 3 ] ), NONE );
    } else if( isStringSame( ucParams[ 0 ], "write" ) ){
        if( ucLen < 3 ){
            return( FALSE );
        } // if
        eepromPutString( convertStringToInt( ucParams[ 1 ] ), ucParams[ 2 ] );
        for( i = 0; i < ucLen - 3; i++ ){
            eepromPutString( convertStringToInt( ucParams[ 1 ] ), " " );
            eepromPutString( convertStringToInt( ucParams[ 1 ] ), ucParams[ 2 + i ] );
        } // for
    } else if( isStringSame( ucParams[ 0 ], "read" ) ){
        i = 0;
        if( ucLen == 2 ){
            ucLen = convertStringToInt( ucParams[ 1 ] );
        } else if( ucLen == 3 ){
            i = convertStringToInt( ucParams[ 1 ] );
            ucLen = convertStringToInt( ucParams[ 2 ] );
        } else{
            ucLen = 255;
        } // else
        //eepromGetBuffer( 0, ucMemory, 256 );
        for( ; i < ucLen; i++ ){
            ucChar = eepromGetChar( i );
            if( isPrintable( ucChar ) ){
                print( STDIO, "%c", ucChar );
            } // if
        } // for
    } else if( isStringSame( ucParams[ 0 ], "erase" ) ){
        eepromErase( );
    } else{
        return( FALSE );
    } // else

    return( TRUE );
}

u8 cmd_gassensor( u8 *ucBuffer )
{
    u8 ucWiper, ucLen, *ucParams[2];

    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    if( ucLen ){
        if( isStringSameIgnoreCase( "on", ucParams[ 0 ] ) ){
            print( STDIO, "\rBoost ON" );
            gasSensorBoostEnable();
        } else if( isStringSameIgnoreCase( "off", ucParams[ 0 ] ) ){
            print( STDIO, "\rBoost OFF" );
            gasSensorBoostDisable();
        } else if( isStringSame( "pwm", ucParams[ 0 ] ) ){
            print( STDIO, "\rHeater PWM set" );
            gasSensorSetHeaterPWM( convertStringToInt( ucParams[ 1 ] ) );
        } else if( isStringSame( "gain", ucParams[ 0 ] ) ){
            gasSensorSetGain( convertStringToInt( ucParams[ 1 ] ) );//, convertStringToInt( ucParams[ 2 ] ) );
            //print( STDIO, "\rChanged gain" );
        } else if( isStringSame( "load", ucParams[ 0 ] ) ){
            gasSensorSetLoad( convertStringToInt( ucParams[ 1 ] ) );
        } else if( isStringSame( "wiper", ucParams[ 0 ] ) ){
            ucWiper = convertStringToInt( ucParams[ 1 ] );
            print( STDIO, "\rWiper%d: 0x%02X", ucWiper, gasSensorGetGain( ucWiper ) );
        } // else if
    } // if
    print( COM3, "\rGas Sensor: %1.3f", gasSensorRead() );
    
    return( TRUE );
}

#endif // __PYGMYCOMMANDS
