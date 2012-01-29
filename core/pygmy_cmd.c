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

const PYGMYCMD PYGMYSTDCOMMANDS[] = { 
                                    {(u8*)"reset",      cmd_reset},
                                    {(u8*)"peek",       cmd_peek},
                                    {(u8*)"poke",       cmd_poke},
                                    {(u8*)"ps",         cmd_ps},
                                    {(u8*)"kill",       cmd_kill},
                                    //{(u8*)"recv",       cmd_recv}, // XModem
                                    //{(u8*)"send",       cmd_send}, // XModem
                                    
                                    {(u8*)"pinevent",   cmd_pinevent},
                                    {(u8*)"pinset",     cmd_pinset},
                                    {(u8*)"pinget",     cmd_pinget},
                                    {(u8*)"pinanalog",  cmd_pinanalog},
                                    {(u8*)"pinpwm",     cmd_pinpwm},
                                    {(u8*)"pinconfig",  cmd_pinconfig},
                                    #ifdef __PYGMYSTREAMFILE
                                    {(u8*)"mnt",        cmd_mnt},
                                    {(u8*)"format",     cmd_format},
                                    {(u8*)"rm",         cmd_rm},
                                    {(u8*)"ls",         cmd_ls},
                                    {(u8*)"mv",         cmd_mv},
                                    {(u8*)"open",       cmd_open},
                                    {(u8*)"read",       cmd_read},
                                    {(u8*)"write",      cmd_write},
                                    {(u8*)"cp",         cmd_cp},
                                    #endif    
                                
                                    {(u8*)"port",       cmd_port},
                                    
                                    {(u8*)"rfopen",     cmd_rfopen},
                                    {(u8*)"rfsend",     cmd_rfsend},
                                    #ifdef __PYGMYMODEMSHIELD
                                    {(u8*)"modem",      cmd_modem},
                                    #endif
                                    #ifdef __PYGMYHUMIDITYSHIELD
                                    {(u8*)"humidity",   cmd_humidity},
                                    #endif
                                    #ifdef __PYGMYVOLTAGESHIELD
                                    {(u8*)"voltshield", cmd_voltshield},
                                    #endif
                                    
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
    u8 i;

    #ifndef __PYGMYCMDMAXLISTS
        #define __PYGMYCMDMAXLISTS 1
    #endif // __PYGMYCMDMAXLISTS
    for( i = 0; i < __PYGMY_MAXCOMMANDPORTS; i++ ){
        globalCMDPorts[ i ].SPI = 0xFF;
        globalCMDPorts[ i ].I2C = 0xFF;
    } // for
    for( i = 0; i < __PYGMY_MAXCOMMANDSPIPORTS; i++ ){
        globalSPIPorts[ i ].CR = 0xFF;
    } // for
    for( i = 0; i < __PYGMY_MAXCOMMANDI2CPORTS; i++ ){
        globalI2CPorts[ i ].CR = 0xFF;
    } // for
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

u8 cmd_modem( u8 *ucBuffer )
{
    //print( COM2, "%s\r", getNextSubString( ucBuffer, NEWLINE ) );
    
    return( cmdExecute( ucBuffer, PYGMYMODEMCMDS ) );
}

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
    PYGMYTASK pygmyTask;
    u16 i, ii;
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE );
    if( ucParam ){
        if( isStringSame( ucParam, "-s" ) || isStringSame( ucParam, "--sockets" ) ){
            rfListSockets();
        } // if
    } // if
    print( STDIO, "\rTasks\r\r" );
    for( i = 0; i < PYGMY_MAXTASKS; i++ ){
        taskList( &pygmyTask, i );
        if( pygmyTask.ID ){
            println( STDIO, "%4d %s %t", pygmyTask.ID, pygmyTask.Name, pygmyTask.TimeStamp );
        }
    } // for
    
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

    return( 1 );
}

u8 cmd_kill( u8 *ucBuffer )
{

    return( 0 );
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
        pinInterrupt( cmdPinEvent, ucPin, 0 );
    } else{
        pinInterrupt( cmdPinEvent, ucPin, TRIGGER_RISING|TRIGGER_FALLING );
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

u8 cmd_cp( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ];
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE );
    return( fileCopy( ucParams[ 0 ], ucParams[ 1 ] ) );
}

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
                        spiConfig( &globalSPIPorts[ globalCMDPorts[ i ].SPI ] , ucPin0, ucPin1, ucPin2, ucPin3 );
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
        spiWriteBuffer( &globalSPIPorts[ globalCMDPorts[ ucCOM ].SPI ], ucBuffer, uiLen );
        return( 1 );
    } else if( globalCMDPorts[ ucCOM ].I2C != 0xFF ){
        i2cWriteBuffer( &globalI2CPorts[ globalCMDPorts[ ucCOM ].I2C ], (u8)ulAddress, ucBuffer, uiLen );
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
        i2cReadBuffer( &globalI2CPorts[ globalCMDPorts[ ucCOM ].I2C ], (u8)ulAddress, ucBuffer, uiLen );
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
    PYGMYSPIPORT pygmySPI;
    u16 uiPressure, uiTemp;

    pinConfig( A4, OUT );
    pinSet( A4, HIGH );
    spiConfig( &pygmySPI, D0, D1, D3, D2 );
    pinSet( D0, LOW );
    spiWriteByte( &pygmySPI, 0x24 ); // Start Conversion
    delay( 100 );
    spiWriteByte( &pygmySPI, 0x00 ); // 
    pinSet( D0, HIGH );

    delay( 100 );

    pinSet( D0, LOW );
    spiWriteByte( &pygmySPI, 0x80 );
    delay( 100 );
    uiPressure = spiReadByte( &pygmySPI );
    spiWriteByte( &pygmySPI, 0x82 );
    delay( 100 );
    uiPressure |= spiReadByte( &pygmySPI );
    spiWriteByte( &pygmySPI, 0x84 );
    delay( 100 );
    uiTemp = (u16)spiReadByte( &pygmySPI )<<8;
    spiWriteByte( &pygmySPI, 0x86 );
    delay( 100 );
    uiTemp |= spiReadByte( &pygmySPI );
    pinSet( D0, HIGH );
    
    print( COM3, "\rPressure %d Temp %d\r> ", uiPressure, uiTemp );
    pinSet( A4, LOW );
}

u8 cmd_psend( u8 *ucBuffer )
{
    

    return( 1 );
}

//--------------------------------------End Port Commands-------------------------------------
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
    u8 ucSocket;

    ulID = rfGetID();
    if( ulID == 0x41B40AA8 ){
        ucSocket = rfOpenSocket( 0x1FC60435, RF_COMLINK );
    } else {
        ucSocket = rfOpenSocket( 0x41B40AA8, RF_COMLINK );
    } // else
    if( ucSocket == 0xFF ){
        return( 0 );
    } // if
    rfSendOpenCommand( ucSocket );

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

