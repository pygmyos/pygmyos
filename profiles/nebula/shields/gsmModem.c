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

void modemOn( void )
{

}

void modemOff( void )
{

}

void modemSignalStrength( void )
{

}

void modemFormatSMS( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "+CMGF";
    pygmyCmd.PrintHandler = printCMGF;
    pygmyCmd.EventHandler = handlerATCMGF;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 1;

    queueCommand( &pygmyGlobalCmdQueue, &pygmyCmd );
}

void modemSendSMS( void )
{
    PYGMYCOMMAND pygmyCmd1, pygmyCmd2;
    
    pygmyCmd1.Name = ">";   
    pygmyCmd1.Expire = 2;
    pygmyCmd1.Retry = 0;
    pygmyCmd1.EventHandler = handlerATSMSPROMPT;
    pygmyCmd1.PrintHandler = printCMGS;

    pygmyCmd2.Name = "+CMGS";
    pygmyCmd2.Expire = 2;
    pygmyCmd2.Retry = 1;
    pygmyCmd2.EventHandler = AT_CMGS;
    pygmyCmd2.PrintHandler = printCMGSMessage;
       
    cmdQueue( &pygmyGlobalCmdQueue, &pygmyCmd1 );
    cmdQueue( &pygmyGlobalCmdQueue, &pygmyCmd2 );
}

u8 handlerATOK( u8 *ucBuffer )
{
    // Waits for OK response
    // Returns TRUE for "OK" FALSE for anything else
    u8 *ucSubString;

    ucSubString = getNextSubString( ucParams, WHITESPACE|PUNCT );
    if( ucSubString && isStringSame( ucSubString, (u8*)PYGMY_CMD_OK ) ){
        return( TRUE );
    }

    return( FALSE );
}

u8 handlerHTTP( u8 *ucBuffer )
{
    // AT GET is the main() of the cell functions, it processes the HTTP transactions
    
    return( FALSE );
}

u8 handlerATCSQ( u8 *ucBuffer )
{
    // +CSQ is the AT response for Signal Quality, 0-31, 99 = no service or unknown
    u8 *ucParams[ 2 ], ucSignal;
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT );
    isStringSame( ucParams[ 0 ], (u8*)"+CSQ" ) );
    ucSignal = convertStringToInt( ucParams[ 1 ] );
    // ToDo: Do something with signal strength        

    return( TRUE );
}

u8 handlerATCCLK( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ];

    getNextSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT );
   
    if( isStringSame( ucParams[ 0 ], "+CCLK" ) && set_time( ucParams[ 1 ] ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handlerATQSS( u8 *ucBuffer )
{
    u8 *ucParams[ 4 ];
 
    getAllSubStrings( ucBuffer, ucParams, 4, WHITESPACE|PUNCT );
    if( !isStringSame( ucParams[ 0 ], "+QSS" ) ){
        return( FALSE );
    } // if

    if( convertStringToInt( ucParams[ 2 ] ) ){
       // ToDo: Add code to do something with QSS value
    } // if

    return( TRUE );
}

u8 handlerATSERVINFO( u8 *ucBuffer )
{
  
    return( FALSE );
}

u8 handlerATSGACT( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
   
    if( isStringSame( ucSubString, "+SGACT" ) || isStringSame( ucSubString, "OK" ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

u8 handlerATCGDCONT( u8 *ucBuffer )
{
    if( isStringSame( getNextSubString( ucBuffer, WHITESPACE|PUNCT ), "OK" ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handlerATSD( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    
    if( isStringSame( ucSubString, "CONNECT" ) ){
        return( TRUE );
    } else if( isStringSame( ucSubString, "ERROR" ) ){
        //cell_close( (u8*)PYGMY_CMD_now );
        //cell_at( "" );
    } // else if

    return( FALSE );
}

u8 handlerATSH( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
    if( isStringSame( ucSubString, "OK" ) || isStringSame( ucSubString, "ERROR" ) ){
        return( TRUE ); // if already disconnected ERROR is valid
    } // if

    return( FALSE );
}

u8 handlerATCMGS( u8 *ucBuffer )
{
    u8 *ucSubString;

    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
    
    if( ucSubString ){
        return( FALSE );
    } // if
    if( isStringSame( ucSubString, "+CMGS" ) ) || 
        isStringSame( ucSubString, "+CMS" ) ){ // Message fail
        if( cmdIsQueued( &pygmyGlobalCmdQueue, "+CMGS" ) == 1 ){
            // ToDo: Add code to do something with SMS
            return( TRUE );
        } // if
    } // if

    return( FALSE );
}

u8 handlerATSMSPROMPT( u8 *ucBuffer )
{
    if( replaceChars( ucBuffer, ">", ' ' ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handlerATCMGF( u8 *ucBuffer )
{
    // ToDo: Look at combining with ATOK
    if( isStringSame( getNextSubString( ucBuffer, WHITESPACE|PUNCT|SEPARATORS ), "OK" ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

void printAT( void )
{
    // If modem is working this will return promptly with OK
    // This command is only to test communications
    // If modem doesn't respond within 1 second power cycle
    print( GSM_COM, "ATE0&W\r" );
}

u8 handlerATAT( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( ucSubString ){
        if( isStringSame( ucSubString, "OK" ) ||
            isStringSame( ucSubString, "ERROR" ) || 
            isStringSame( ucSubString, "#MWI" ) )
            print( GSM_COM, "\rRadio On\r" );
            deleteTask( "AT", DRIVER_AT );
        
            return( TRUE );
        } // if
    } // if
    
    return( FALSE );
}

u8 handlerATATOFF( u8 *ucBuffer )
{
    u8 *ucSubString;

    ucSubString = getNextSubString( ucBuffer, WHITESPACE|NEWLINE );
    if( ucSubString ){
        // cell_onoff( "" );
        return( TRUE ); // if cell responded then it's not off yet
    }

    return( FALSE ); 
}

void printCMGS( void )
{
    // ToDo: Add print code
}

void print_CMGS_Message( void )
{
    // ToDo: Add print code
}
