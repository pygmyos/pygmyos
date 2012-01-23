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

const u8 AT_OK[]                    = "OK";
const u8 AT_ERROR[]                 = "ERROR";
const u8 AT_CONNECT[]               = "CONNECT";
const u8 AT_NOCARRIER[]             = "NO CARRIER";
const u8 AT_ESCAPE[]                = "+++";
const u8 AT_SGACT[]                 = "+SGACT";
const u8 AT_CMGF[]                  = "+CMGF";
const u8 AT_CMGS[]                  = "+CMGS";
const u8 AT_CMS[]                   = "+CMS";
const u8 AT_CSQ[]                   = "+CSQ";
const u8 AT_QSS[]                   = "+QSS";
const u8 AT_CCLK[]                  = "+CCLK";
const u8 AT_MWI[]                   = "#MWI";
const u8 AT_CGDCONT[]               = "+CGDCONT";
const u8 AT_CGDATA[]                = "+CGDATA";
const u8 AT_SD[]                    = "#SD";
const u8 AT_SH[]                    = "#SH";
const u8 AT_CMGF[]                  = "+CMGF";
const u8 AT_ATE[]                   = "ATE";
const u8 AT_ATIP[]                  = "+IP";
const u8 AT_W[]                     = "&W";

const u8 AT_VTS[]                   = "+VTS"; // DTMF Tones Transmission
const u8 AT_VTD[]                   = "+VTD"; // Tone Duration
const u8 AT_IPR[]                   = "+IPR"; // Fixed Interface Data Rate
const u8 AT_IFC[]                   = "+IFC"; // Local Flow Control
const u8 AT_ICF[]                   = "+ICF"; // Local Character Framing

const u8 AT_CRLF[]                  = {13,10,'\0'};
const u8 AT_CTRLZ[]                 = {0x1A,'\0'};
const u8 AT_ESC[]                   = {0x1B,'\0'};

const u8 EMAIL_AUTHLOGIN[]          = "AUTH LOGIN";
const u8 EMAIL_HELO[]               = "HELO "; // SMTP "Hello"
const u8 EMAIL_RCPT[]               = "RCPT TO: ";
const u8 EMAIL_MAILFROM[]           = "MAIL FROM: ";
const u8 EMAIL_DATA[]               = "DATA";
const u8 EMAIL_QUIT[]               = "QUIT";
const u8 EMAIL_From[]               = "From: ";
const u8 EMAIL_To[]                 = "To: ";
const u8 EMAIL_Cc[]                 = "Cc: ";
const u8 EMAIL_Subject[]            = "Subject: ";
const u8 EMAIL_ContentType[]        = "Content-Type: text/plain; charset=\"iso-8859-1\"";
const u8 EMAIL_ContentEncoding[]    = "Content-Transfer-Encoding: 7bit";
const u8 EMAIL_MIMVersion[]         = "MIME-Version: 1.0";
const u8 EMAIL_Priority[]           = "Priority: ";
const u8 EMAIL_Importance[]         = "Importance: ";
const u8 EMAIL_220[]                = "220"; // Greeting
const u8 EMAIL_221[]                = "221"; // Goodbye
const u8 EMAIL_235[]                = "235"; // 
const u8 EMAIL_250[]                = "250"; // General Ack
const u8 EMAIL_334[]                = "334"; // Authentication accepted
const u8 EMAIL_354[]                = "354"; // Ready for Data
const u8 EMAIL_451[]                = "451"; // Internal Error, requires complete restart
const u8 EMAIL_530[]                = "530";

const u8 HTTP_Connection[]          = "Connection: ";
const u8 HTTP_keepalive[]           = "keep-alive";
const u8 HTTP_V1P0[]                = "HTTP/1.0";
const u8 HTTP_V1P1[]                = "HTTP/1.1";
const u8 HTTP_Host[]                = "Host";
const u8 HTTP_GET[]                 = "GET";

const PYGMYCMD PYGMYMODEMCOMMANDS[] = {
                                    {(u8*)"csq",        modem_csq},
                                    {(u8*)"signal"      modem_csq},
                                    {(u8*)"echo",       modem_echo},
                                    //{(u8*)"sim",        modem_sim},
                                    {(u8*)"imei",       modem_imei},
                                    {(u8*)"simin",      modem_simin},
                                    {(u8*)"reset",      modem_reset},
                                    {(u8*)"onoff",      modem_onoff},
                                    {(u8*)"email",      modem_email},
                                    {(u8*)"sms",        modem_sms},
                                    {(u8*)"http",       modem_http},
                                    {(u8*)"pftp",       modem_pftp},
                                    {(u8*)"carrier",    modem_carrier},
                                    {(u8*)"time",       modem_time},
                                    {(u8*)"attach",     modem_attach},
                                    {(u8*)"detach",     modem_detach},
                                    {(u8*)"portal",     modem_portal},
                                    //{(u8*)"get",cell_httpget,
                                    {(u8*)"suspend",    modem_suspend},
                                    {(u8*)"close",      modem_close},
                                    //{(u8*)"csca",       cell_csca},
                                    {(u8*)"", cmdNull}
                                    }; 

const PYGMYCMD PYGMYATCOMMANDS[] = { 
                                    {(u8*)AT_SGACT, handler


void modemOn( void )
{

}

void modemOff( void )
{

}


//---------------------------------------Modem Commands---------------------------------------
//--------------------------------------------------------------------------------------------

u8 modem_csq( u8 *ucBuffer )
{
    
    return( FALSE );
}
      
u8 modem_echo( u8 *ucBuffer )
{

    return( FALSE );
}

u8 modem_imei( u8 *ucBuffer )
{

    return( FALSE );
}

u8 modem_simin( u8 *ucBuffer )
{

    return( FALSE );
}

u8 modem_reset( u8 *ucBuffer )
{

    return( FASLE );
}

u8 modem_onoff( u8 *ucBuffer )
{
    
    return( FALSE );
}

u8 modem_email( u8 *ucBuffer )
{

    return( FALSE );
}

u8 modem_sms( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd1, pygmyCmd2;
    u8 *ucParams[ 4 ];

    if( isStringSame( ucParams[ 0 ], "--format" ) ){ 
        // set SMS format
        pygmyCmd1.Name = (u8*)AT_CMGF;
        pygmyCmd1.PrintHandler = printCMGF;
        pygmyCmd1.EventHandler = handlerATCMGF;
        pygmyCmd1.Expire = 2;
        pygmyCmd1.Retry = 1;

        queueCommand( &pygmyGlobalCmdQueue, &pygmyCmd );
    } // if
    // ToDo: Add message body code
    pygmyCmd1.Name = ">";   
    pygmyCmd1.Expire = 2;
    pygmyCmd1.Retry = 0;
    pygmyCmd1.EventHandler = handlerATSMSPROMPT;
    pygmyCmd1.PrintHandler = printCMGS;

    pygmyCmd2.Name = (u8*)AT_CMGS;
    pygmyCmd2.Expire = 2;
    pygmyCmd2.Retry = 1;
    pygmyCmd2.EventHandler = handlerATCMGS;
    pygmyCmd2.PrintHandler = printCMGSMessage;
       
    cmdQueue( &pygmyGlobalCmdQueue, &pygmyCmd1 );
    cmdQueue( &pygmyGlobalCmdQueue, &pygmyCmd2 );

    return( FALSE );
}
                             
u8 modem_http( u8 *ucBuffer )
{

    return( FALSE );
}
                                    
u8 modem_pftp( u8 *ucBuffer )
{

    return( FALSE );
}
             
u8 modem_carrier( u8 *ucBuffer )
{

    return( FALSE );
}
                                 
u8 modem_time( u8 *ucBuffer )
{

    return( FALSE );
}
                                   
u8 modem_attach( u8 *ucBuffer )
{

    return( FALSE );
}
                                   
u8 modem_detach( u8 *ucBuffer )
{

    return( FALSE );
}
                  
u8 modem_portal( u8 *ucBuffer )
{

    return( FALSE );
}
                                    
u8 modem_suspend( u8 *ucBuffer )
{
    PYGMYCMD pygmyCMD;

    pygmyCMD.Name = (u8*)AT_ESCAPE;
    pygmyCMD.PrintHandler = printATESCAPE;
    pygmyCMD.EventHandler = handlerATESCAPE;
    pygmyCMD.Expire = 1;
    pygmyCMD.Retry = 0;

    cmdQueue( &pygmyCMD );

    return( FALSE );
}
           
u8 modem_close( u8 *ucBuffer )
{
    PYGMYCMD pygmyCMD;
    u8 *ucParams[ 2 ], ucLen;
    
    ucLen = getAllSubStrings( ucBuffer, WHITESPACE|PUNCT );
    if( isStringSameIgnoreCase( ucParams[ 0 ], "now" ) ){
        cmdInitQueue( );
    } else{
        pygmyCMD.Name = (u8*)AT_SH;
        pygmyCMD.PrintHandler = printSH;
        pygmyCMD.EventHandler = handlerATSH;
        pygmyCMD.Expire = 2;
        pygmyCMD.Retry = 1;

        cmdQueue( &pygmyCMD );
    } // else

    return( TRUE );
}

//-------------------------------------End Modem Commands-------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------Response Handlers-------------------------------------
//--------------------------------------------------------------------------------------------


u8 handlerATOK( u8 *ucBuffer )
{
    // Waits for OK response
    // Returns TRUE for "OK" FALSE for anything else
    u8 *ucSubString;

    ucSubString = getNextSubString( ucParams, WHITESPACE|PUNCT );
    if( ucSubString && isStringSame( ucSubString, (u8*)AT_OK ) ){
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
    isStringSame( ucParams[ 0 ], (u8*)AT_CSQ ) );
    ucSignal = convertStringToInt( ucParams[ 1 ] );
    // ToDo: Do something with signal strength        

    return( TRUE );
}

u8 handlerATCCLK( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ];

    getNextSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT );
   
    if( isStringSame( ucParams[ 0 ],  ) && set_time( ucParams[ 1 ] ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handlerATQSS( u8 *ucBuffer )
{
    u8 *ucParams[ 4 ];
 
    getAllSubStrings( ucBuffer, ucParams, 4, WHITESPACE|PUNCT );
    if( !isStringSame( ucParams[ 0 ], (u8*)AT_QSS ) ){
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
   
    if( isStringSame( ucSubString, (u8*)AT_SGACT ) || isStringSame( ucSubString, (u8*)AT_OK ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

u8 handlerATCGDCONT( u8 *ucBuffer )
{
    if( isStringSame( getNextSubString( ucBuffer, WHITESPACE|PUNCT ), (u8*)AT_OK ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handlerATSD( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    
    if( isStringSame( ucSubString, (u8*)AT_CONNECT ) ){
        return( TRUE );
    } else if( isStringSame( ucSubString, (u8*)AT_ERROR ) ){
        //cell_close( (u8*)PYGMY_CMD_now );
        //cell_at( "" );
    } // else if

    return( FALSE );
}

u8 handlerATSH( u8 *ucParams )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
    if( isStringSame( ucSubString, (u8*)AT_OK ) || isStringSame( ucSubString, (u8*)AT_ERROR ) ){
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
    if( isStringSame( ucSubString, (u8*)AT_CMGS ) ) || 
        isStringSame( ucSubString, (u8*)AT_CMS ) ){ // Message fail
        if( cmdIsQueued( &pygmyGlobalCmdQueue, (u8*)AT_CMGS ) == 1 ){
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
    if( isStringSame( getNextSubString( ucBuffer, WHITESPACE|PUNCT|SEPARATORS ), (u8*)AT_OK ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

u8 handlerATAT( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( ucSubString ){
        if( isStringSame( ucSubString, (u8*)AT_OK ) ||
            isStringSame( ucSubString, (u8*)AT_ERROR ) || 
            isStringSame( ucSubString, (u8*)AT_MWI ) )
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

//-----------------------------------End Response Handlers------------------------------------
//--------------------------------------------------------------------------------------------

//---------------------------------------Print Handlers---------------------------------------
//--------------------------------------------------------------------------------------------

void printAT( void )
{
    // If modem is working this will return promptly with OK
    // This command is only to test communications
    // If modem doesn't respond within 1 second power cycle
    print( GSM_COM, "ATE0&W\r" );
}

void printCMGS( void )
{
    // ToDo: Add print code
}

void printCMGSMessage( void )
{
    // ToDo: Add print code
}

void print_CMGF( void )
{
    print( MODEM_COM, "%sAT+CMGF=1\r", (u8*)AT_CTRLZ );
}

void print_SH( void )
{
    print( MODEM_COM, "%sAT#SH=1\r", (u8*)AT_CTRLZ );
}


//-------------------------------------End Print Handlers-------------------------------------
//--------------------------------------------------------------------------------------------
