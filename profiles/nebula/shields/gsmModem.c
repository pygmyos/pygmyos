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

const u8 AT_ACTIONCHARS[]           = { 0x0A, '>', '\0' };
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
const u8 AT_ATE[]                   = "ATE";
const u8 AT_ATIP[]                  = "+IP";
const u8 AT_W[]                     = "&W";
const u8 AT_SERVINFO[]              = "#SERVINFO";
const u8 AT_VTS[]                   = "+VTS"; // DTMF Tones Transmission
const u8 AT_VTD[]                   = "+VTD"; // Tone Duration
const u8 AT_IPR[]                   = "+IPR"; // Fixed Interface Data Rate
const u8 AT_IFC[]                   = "+IFC"; // Local Flow Control
const u8 AT_ICF[]                   = "+ICF"; // Local Character Framing

const u8 AT_CRLF[]                  = {13,10,'\0'};
const u8 AT_CTRLZ[]                 = {0x1A,'\0'};
const u8 AT_ESC[]                   = {0x1B,'\0'};

const u8 EMAIL_AUTHLOGIN[]          = "AUTH LOGIN";
const u8 EMAIL_HELO[]               = "HELO"; // SMTP "Hello"
const u8 EMAIL_RCPT[]               = "RCPT TO:";
const u8 EMAIL_MAILFROM[]           = "MAIL FROM:";
const u8 EMAIL_DATA[]               = "DATA";
const u8 EMAIL_QUIT[]               = "QUIT";
const u8 EMAIL_From[]               = "From:";
const u8 EMAIL_To[]                 = "To:";
const u8 EMAIL_Cc[]                 = "Cc:";
const u8 EMAIL_Subject[]            = "Subject:";
const u8 EMAIL_ContentType[]        = "Content-Type: text/plain; charset=\"iso-8859-1\"";
const u8 EMAIL_ContentEncoding[]    = "Content-Transfer-Encoding: 7bit";
const u8 EMAIL_MIMVersion[]         = "MIME-Version: 1.0";
const u8 EMAIL_Priority[]           = "Priority:";
const u8 EMAIL_Importance[]         = "Importance:";
const u8 EMAIL_220[]                = "220"; // Greeting
const u8 EMAIL_221[]                = "221"; // Goodbye
const u8 EMAIL_235[]                = "235"; // 
const u8 EMAIL_250[]                = "250"; // General Ack
const u8 EMAIL_334[]                = "334"; // Authentication accepted
const u8 EMAIL_354[]                = "354"; // Ready for Data
const u8 EMAIL_451[]                = "451"; // Internal Error, requires complete restart
const u8 EMAIL_530[]                = "530";

const u8 HTTP_Connection[]          = "Connection:";
const u8 HTTP_keepalive[]           = "keep-alive";
const u8 HTTP_V1P0[]                = "HTTP/1.0";
const u8 HTTP_V1P1[]                = "HTTP/1.1";
const u8 HTTP_Host[]                = "Host";
const u8 HTTP_GET[]                 = "GET";

const PYGMYCMD PYGMYMODEMCMDS[] = {
                                    {(u8*)"csq",        modem_csq},
                                    {(u8*)"signal",     modem_csq},
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
                                    {(u8*)"suspend",    modem_suspend},
                                    {(u8*)"close",      modem_close},
                                    {(u8*)"flow",       modem_flow},
                                    {(u8*)"smsread",    modem_smsread},
                                    {(u8*)"smsdel",     modem_smsdel},
                                    {(u8*)"smslist",    modem_smslist},
                                    //{(u8*)"csca",       cell_csca},
                                    {(u8*)"", cmdNull}
                                    }; 

//const PYGMYCOMMAND PYGMYATCOMMANDS[] = { 
//                                    {(u8*)AT_SGACT, handler

u32 globalModemStatus = 0;
u16 globalModemTCPPort = 80;
u8 globalModemPortal[ 40 ];
u8 globalModemURL[ 128 ];
u8 globalModemHostName[ 40 ];
u8 globalModemSMTPServer[ 40 ];
PYGMYSMS globalModemSMS;
PYGMYPFTP globalModemPFTP;
PYGMYEMAIL globalModemEmail;
PYGMYCOMMANDQUEUE globalModemQueue;

void cmdGetsModem( void )
{
    u8 ucBuffer[ __PYGMYCOM2BUFFERLEN ];
    streamFIFOToString( MODEM_COM, ucBuffer );
    //globalStreams[ COM2 ].RXBuffer[ globalStreams[ COM2 ].RXIndex++ ] = ;
    
    if( globalModemQueue.Count ){
        if( globalModemQueue.Commands[ globalModemQueue.Index ].Status ){
            //print( MODEM_DEBUG, "\rFIFO(%s)", ucBuffer );   
            if( globalModemQueue.Commands[ globalModemQueue.Index ].EventHandler( ucBuffer ) )
                cmdDelete( &globalModemQueue, globalModemQueue.Index );
            else{
                
                globalModemQueue.Commands[ globalModemQueue.Index ].Status &= ~PYGMY_AT_TX;
                cmdProcess( &globalModemQueue );
            } // else
        } // if
    } else{
        
    } // else
    //print( MODEM_DEBUG, "\rProc Done." );
}
                                
void blink( void )
{
    if( pinGet( T1 ) ){
        pinSet( T1, LOW );
    } else{
        pinSet( T1, HIGH );
    } // else
}

void DriverThread_ProcessCommands( void )
{
    cmdProcess( &globalModemQueue );
}



void modemInit( void )
{
    
    //pinConfig( PA2, ALT );
    //pinConfig( PA3, IN );
   
    //USART2->BRR = 24000000/ 115200; //( ( (ulClock >> 3 ) / BOOT_BAUDRATE ) << 4 ) + ( ( ( ulClock / BOOT_BAUDRATE ) ) & 0x0007 );
    //USART2->CR3  = 0;//|= USART_ONEBITE;
    //USART2->CR1 = ( USART_UE | USART_RXNEIE | USART_TE | USART_RE  );

    comConfig( COM2, 0, RTS|CTS, 19200 );
    
    streamEnableActionChars( COM2 );
    streamSetActionChars( COM2, (u8*)AT_ACTIONCHARS );
    streamSetGet( MODEM_COM, cmdGetsModem );
    streamSetRXBuffer( MODEM_COM, globalCOM2RXBuffer, __PYGMYCOM2BUFFERLEN );
    streamSetPut( MODEM_COM, putsUSART2 );
    pinConfig( D1, OUT );
    pinConfig( D0, OUT );
    pinSet( D1, LOW );
    pinSet( D0, LOW );
    pinConfig( D3, IN );
    pinConfig( T1, OUT );
    
    //pinSet( D0, HIGH );
    //delay( 15000 );
    //pinSet( D0, LOW );
 
    pinInterrupt( blink, D3, TRIGGER_RISING|TRIGGER_FALLING, 7  );  
    cmdInitQueue( &globalModemQueue );
    taskNew( "modemcmd", 500, 500, 0, (void*)DriverThread_ProcessCommands );
}

u8 modemtask_reset( void )
{
    pinSet( MODEM_RESET, LOW );
    taskDelete( "modemreset"  );
    
    return( TRUE );
}

u8 modemtask_onoff( void )
{
    pinSet( MODEM_ONOFF, LOW );
    taskDelete( "modemonoff" );

    return( TRUE );
}


//---------------------------------------Modem Commands---------------------------------------
//--------------------------------------------------------------------------------------------

u8 modem_flow( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], ucLen;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT|NEWLINE );
    if( ucLen > 1 ){
        print( MODEM_COM, "AT+IFC=%d,%d\r", convertStringToInt( ucParams[ 0 ] ), 
            convertStringToInt( ucParams[ 1 ] ) );
        return( TRUE );
    } // if

    return( FALSE );
}

u8 modem_csq( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_CSQ;
    pygmyCmd.PrintHandler = print_CSQ;
    pygmyCmd.EventHandler = handler_ATCSQ;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 2;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( TRUE );
}
      
u8 modem_echo( u8 *ucBuffer )
{
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE | PUNCT );
    modem_close( "now" );
    if( isStringSame( ucParam, "on" ) ){
        println( MODEM_COM, "ATE1&W" );
        return( TRUE );
    } else if( isStringSame( ucParam, "off" ) ){
        println( MODEM_COM, "ATE0&W" );
        return( TRUE );
    } // else if

    return( FALSE );
}

u8 modem_imei( u8 *ucBuffer )
{

    return( FALSE );
}

u8 modem_simin( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_QSS;
    pygmyCmd.PrintHandler = print_QSS;
    pygmyCmd.EventHandler = handler_ATQSS;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 2;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( TRUE );
}

u8 modem_reset( u8 *ucBuffer )
{
    pinSet( MODEM_RESET, HIGH );
    taskNewSimple( "modemreset", 3000, modemtask_reset );

    return( TRUE );
}

u8 modem_onoff( u8 *ucBuffer )
{
    pinSet( MODEM_ONOFF, HIGH );  // Switches ONOFF to GND, must be kept low for min 1 sec 
    taskNewSimple( "modemonoff", 1200, modemtask_onoff );

    return( TRUE );
}

u8 modem_email( u8 *ucBuffer )
{
    modem_close( "now" ); // this clears the status bits
    modem_close( "" );
    modem_portal( "" );//Configure internet portal ( APN )
    modem_attach( "" );//Attach, always to default context 1
    globalModemTCPPort = 25;
    modem_sd( "" ); // Socket dial
    email_wait();
    email_HELO();
    email_AUTHLOGIN();
    email_user();
    email_pw();
    email_MAILFROM();
    email_RCPT();
    email_DATA();
    email_body();
    email_quit();
    email_wait();
    modem_suspend( "" ); // Connection must be suspended to close

    return( TRUE );
}

void email_HELO( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_HELO;
    pygmyCmd.PrintHandler = print_HELO;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 10;
    pygmyCmd.Retry = 4;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_pw( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "emailpw";
    pygmyCmd.PrintHandler = print_EMAILPW;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 0;
    cmdQueue( &globalModemQueue, &pygmyCmd ); 
}

void email_user( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "emailuser";
    pygmyCmd.PrintHandler = print_EMAILUSER;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 0;
    cmdQueue( &globalModemQueue, &pygmyCmd ); 
}

void email_AUTHLOGIN( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_AUTHLOGIN;
    pygmyCmd.PrintHandler = print_AUTHLOGIN;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 10;
    pygmyCmd.Retry = 0;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_MAILFROM( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_MAILFROM;
    pygmyCmd.PrintHandler = print_MAILFROM;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 10;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_RCPT( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_RCPT;
    pygmyCmd.PrintHandler = print_RCPT;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 10;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_DATA( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_DATA;
    pygmyCmd.PrintHandler = print_DATA;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 0;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_body( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "email";
    pygmyCmd.PrintHandler = print_EMAILBODY;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 0;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_quit( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)EMAIL_QUIT;
    pygmyCmd.PrintHandler = print_QUIT;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 3;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void email_wait( void )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)"wait";
    pygmyCmd.PrintHandler = print_wait;
    pygmyCmd.EventHandler = email_get;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 0; // only a pause to allow prompt from smtp server
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

u8 email_get( u8 *ucBuffer )
{
    // This function doesn't "get" an e-mail, rather it acts as the equivalent to HTTP GET
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    
    if( ucParam ){
        if( isStringSame( ucParam, (u8*)EMAIL_220 ) || // Greeting
            isStringSame( ucParam, (u8*)EMAIL_235 ) || // Authentication accepted
            isStringSame( ucParam, (u8*)EMAIL_250 ) || // Ack 
            isStringSame( ucParam, (u8*)EMAIL_334 ) || // Ready for Login
            isStringSame( ucParam, (u8*)EMAIL_354 ) ){ // Ready for Data
            return( TRUE );
        } else if( isStringSame( ucParam, "500" ) || isStringSame( ucParam, "503" ) ){
            modem_close( "now" );
        } else if( isStringSame( ucParam, (u8*)EMAIL_221 ) ){
            modem_close( "now" );
            return( TRUE );
        } //else if
    }

    return( FALSE );
}

u8 modem_sms( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd1, pygmyCmd2, pygmyCmdCMGF;
    
    
    //streamSetActionChars( MODEM_COM, "\r>" );
    //cmdInitQueue( &globalModemQueue );
    copyString( getNextSubString( ucBuffer, WHITESPACE ), globalModemSMS.PhoneNumber ); 
    copyString( getNextSubString( NULL, NEWLINE ), globalModemSMS.Message );
    //print( MODEM_DEBUG, "\rSending \"%s\"", globalModemSMS.Message ); 
    // set SMS format
    //pygmyCmdCMGF.Name = (u8*)AT_CMGF;
    //pygmyCmdCMGF.PrintHandler = print_CMGF;
    //pygmyCmdCMGF.EventHandler = handler_ATCMGF;
    //pygmyCmdCMGF.Expire = 2;
    //pygmyCmdCMGF.Retry = 1;
    
    pygmyCmd1.Name = ">";   
    pygmyCmd1.Expire = 2;
    pygmyCmd1.Retry = 0;
    pygmyCmd1.EventHandler = handler_ATSMSPROMPT;
    pygmyCmd1.PrintHandler = print_CMGS;

    pygmyCmd2.Name = (u8*)AT_CMGS;
    pygmyCmd2.Expire = 2;
    pygmyCmd2.Retry = 1;
    pygmyCmd2.EventHandler = handler_ATCMGS;
    pygmyCmd2.PrintHandler = print_CMGSMessage;
    
    //cmdQueue( &globalModemQueue, &pygmyCmdCMGF );
    cmdQueue( &globalModemQueue, &pygmyCmd1 );
    cmdQueue( &globalModemQueue, &pygmyCmd2 );

    return( TRUE );
}
                             
u8 modem_http( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "GET";
    pygmyCmd.PrintHandler = print_http;
    pygmyCmd.EventHandler = handler_ATGET;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 2;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( FALSE );
}
                                    
u8 pftp_put( u8 *ucParams )
{
    PYGMYCOMMAND pygmyCmd;
    
    pygmyCmd.Name = "pftp";
    pygmyCmd.PrintHandler = (void *)print_pftp_put;
    pygmyCmd.EventHandler = (void *)pftp_put_handler;
    pygmyCmd.Expire = 7;
    pygmyCmd.Retry = 3;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

void print_pftp_put( void )
{
    u32 ulSum;
    u16 i;
    u8 ucByte;

    // This function establishes connection
    globalModemStatus |= (MODEM_PFTPINPROGRESS|MODEM_BUSY); // Setting this earlier causes a UART conflict
    print( MODEM_COM, "GET /cgi/pftp.py?put=" );
    print( MODEM_COM, globalModemPFTP.File.Name );
    if( globalModemPFTP.ChunkSize != 256 ) { // 256 is PFTP server default
        print( MODEM_COM, "&sz=%d", globalModemPFTP.ChunkSize );
    } // if
    // Character '+' is substitute for SPACE in URI
    print( MODEM_COM, "&dat=PFTP+%d+", globalModemPFTP.CurrentChunk );
    
    fileSetPosition( &globalModemPFTP.File, START, 
        ( globalModemPFTP.CurrentChunk - 1 ) * globalModemPFTP.ChunkSize );
    
    ulSum = 0;
    pdiaEncode( 0, PDIA_NEW, &ulSum );
    for( i = 0; i < globalModemPFTP.ChunkSize; i++ ){
        if( globalModemPFTP.File.Attributes & EOF ){
            break;
        } // if
        ucByte = fileGetChar( &globalModemPFTP.File ); // feed each byte sent to PDIA Encoder 
        pdiaEncode( ucByte, PDIA_ADD, &ulSum );
        print( MODEM_COM, "%2X", ucByte ); // send in bar uppercase hex with no prefix
    } // for
    // PDIA will not increment the frame if only 256 bytes are sent
    pdiaEncode( 0, PDIA_END, &ulSum ); // Close out PDIA
    print( MODEM_COM, "+%d+%08X", i, ulSum );
    print( MODEM_COM, " HTTP/1.1%s", (u8*)AT_CRLF );
    print( MODEM_COM, "Host: %s%s%s", globalModemPFTP.HostName, (u8*)AT_CRLF, (u8*)AT_CRLF );
}

u8 modem_pftp( u8 *ucBuffer )
{
    u8 *ucParams[ 3 ], ucLen, *ucCommand, *ucFileName, *ucSubString, ucCMD;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 3, WHITESPACE|NEWLINE );
    ucCommand = ucParams[ 0 ];
    ucFileName = ucParams[ 1 ];
    ucSubString = ucParams[ 2 ];
    
    if( ucCommand ){
        if( isStringSame( ucCommand, "get" ) ){
            ucCMD = 0; 
        } else if( isStringSame( ucCommand, "put" ) ){
            ucCMD = 1;
        } else{
            return( FALSE );
        } // else
    }// if
    
    if( ucFileName ){
        copyString( ucFileName, globalModemPFTP.File.Name );
    } else{
        return( FALSE );
    } // else
    
    // Next test for pcmp tag, if used the file will be decompressed with PCMP as it's saved
    globalModemPFTP.CurrentChunk = 1; // 0 is reserved for error
    if ( ucSubString ){ // Second optional parameter is start chunk for resume
        globalModemPFTP.CurrentChunk = convertStringToInt( ucSubString );
        if( globalModemPFTP.CurrentChunk < 1 ){
            globalModemPFTP.CurrentChunk = 1;
        } // if
    } // if
    
    globalModemTCPPort = 80; // HTTP Port
    globalModemStatus &= ~( MODEM_HTTPINPROGRESS|MODEM_PFTPINPROGRESS|MODEM_SMSINPROGRESS|MODEM_EMAILINPROGRESS);
    globalModemStatus |= MODEM_BUSY;
    globalModemPFTP.ChunkSize = 256; // 256 is default, no sz parameter will be sent, saves bandwidth
    modem_close( "" ); // Clear command queue
    modem_close( "" ); // close any open connections
    modem_portal( "" );//Configure internet portal ( APN )
    modem_attach( "" );//Attach, always to default context 1
    modem_sd( "" ); // Socket dial

    if( ucCMD ){
        if( !fileOpen( &globalModemPFTP.File, globalModemPFTP.File.Name, READ ) ){
            return( FALSE );
        } // if
        globalModemStatus |= MODEM_PFTPPUT;
        pftp_put( "" ); // connection will be closed by server
    } else{
        globalModemStatus &= ~MODEM_PFTPPUT;
        pftp_get( "" ); // connection will be closed by server
    } // else
    
    return( TRUE );
} 

u8 pftp_get( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;
    
    pygmyCmd.Name = "pftp";
    pygmyCmd.PrintHandler = (void *)print_pftp_get;
    pygmyCmd.EventHandler = (void *)pftp_handler;
    pygmyCmd.Expire = 7;
    pygmyCmd.Retry = 3;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

u8 pftp_close( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;
    
    pygmyCmd.Name = "close";
    pygmyCmd.PrintHandler = (void *)print_pftp_close;
    pygmyCmd.EventHandler = (void *)pftp_close_handler;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );
}

u8 pftp_close_handler( u8 *ucBuffer )
{
    globalModemStatus &= ~MODEM_HTTPINPROGRESS;

    return( TRUE );
}
             
u8 modem_carrier( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_SERVINFO;
    pygmyCmd.PrintHandler = print_SERVINFO;
    pygmyCmd.EventHandler = handler_ATSERVINFO;
    pygmyCmd.Expire = 4;
    pygmyCmd.Retry = 2;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( TRUE );
}
                                 
u8 modem_time( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_CCLK;
    pygmyCmd.PrintHandler = print_CCLK;
    pygmyCmd.EventHandler = handler_ATCCLK;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 2;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( TRUE );
}
                                   
u8 modem_attach( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_SGACT;
    pygmyCmd.PrintHandler = print_SGACT;
    pygmyCmd.EventHandler = modem_attachtest;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( FALSE );
}

u8 modem_attachtest( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;
    u8 *ucParams[ 3 ], ucLen; // *ucSubString1, *ucSubString2, *ucSubString;
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 3, WHITESPACE|PUNCT|NEWLINE );
    
    if( ucLen >= 3 ){
        if( isStringSame( ucParams[ 0 ], (u8*)AT_SGACT ) && 
            convertStringToInt( ucParams[ 1 ] ) == 1 ){
            if( convertStringToInt( ucParams[ 2 ] ) == 1 ){
                print( MODEM_DEBUG, "\rAttached" );
                return( TRUE ); 
            }else{
                pygmyCmd.Name = (u8*)AT_SGACT;
                pygmyCmd.PrintHandler = print_SGACT1;
                pygmyCmd.EventHandler = handler_ATSGACT;
                pygmyCmd.Expire = 5;
                pygmyCmd.Retry = 1;
                cmdReplace( &globalModemQueue, &pygmyCmd );
                return( 1 ); 
            } // if
        } // else
    } // if

    return( FALSE );
}
                                   
u8 modem_detach( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_SGACT;
    pygmyCmd.PrintHandler = print_SGACT0;
    pygmyCmd.EventHandler = handler_ATSGACT;
    pygmyCmd.Expire = 2;
    pygmyCmd.Retry = 4;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( FALSE );
}
                  
u8 modem_portal( u8 *ucBuffer )
{
    // Applies portal settings currently configured ( APN )
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_CGDCONT;
    pygmyCmd.PrintHandler = print_CGDCONT;
    pygmyCmd.EventHandler = handler_ATCGDCONT;
    pygmyCmd.Expire = 4;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( TRUE );
}
                                    
u8 modem_suspend( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_ESCAPE;
    pygmyCmd.PrintHandler = print_ATESCAPE;
    pygmyCmd.EventHandler = handler_ATESCAPE;
    pygmyCmd.Expire = 1;
    pygmyCmd.Retry = 0;

    cmdQueue( &globalModemQueue, &pygmyCmd );

    return( FALSE );
}
           
u8 modem_close( u8 *ucBuffer )
{
    PYGMYCOMMAND pygmyCmd;
    u8 *ucParams[ 2 ], ucLen;
    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT );
    if( isStringSameIgnoreCase( ucParams[ 0 ], "now" ) ){
        cmdInitQueue( &globalModemQueue );
    } else{
        pygmyCmd.Name = (u8*)AT_SH;
        pygmyCmd.PrintHandler = print_SH;
        pygmyCmd.EventHandler = handler_ATSH;
        pygmyCmd.Expire = 2;
        pygmyCmd.Retry = 1;

        cmdQueue( &globalModemQueue, &pygmyCmd );
    } // else

    return( TRUE );
}

u8 modem_sd( u8 *ucBuffer )
{
    // Socket Dial, used to open internet connection
    PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = (u8*)AT_SD;
    pygmyCmd.PrintHandler = print_SD;
    pygmyCmd.EventHandler = handler_ATSD;
    pygmyCmd.Expire = 60;
    pygmyCmd.Retry = 3;
    cmdQueue( &globalModemQueue, &pygmyCmd );
    
    return( TRUE );
}

u8 modem_smsread( u8 *ucBuffer )
{
    /*PYGMYCOMMAND pygmyCmd, pygmyCmd2;

    pygmyCmd2.Name = "smscsdh";
    pygmyCmd2.PrintHandler = print_ATCSDH;
    pygmyCmd2.EventHandler = handler_ATCSDH;
    pygmyCmd2.Expire = 15;
    pygmyCmd2.Retry = 1;

    pygmyCmd.Name = "smsread";
    pygmyCmd.PrintHandler = print_ATCMGR;
    pygmyCmd.EventHandler = handler_ATCMGR;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 1;

    cmdQueue( &globalModemQueue, &pygmyCmd2 );
    cmdQueue( &globalModemQueue, &pygmyCmd );
    */
    print_ATCMGR();
    
    return( TRUE );
}

u8 modem_smsdel( u8 *ucBuffer )
{
    u8 *ucParams[ 2 ], ucLen;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT|NEWLINE );
    if( ucLen > 1 ){
        print( MODEM_COM, "AT+CMGD=%d,%d\r", convertStringToInt( ucParams[ 0 ] ), convertStringToInt( ucParams[ 1 ] ) );
        
        return( TRUE );
    } // if
    /*PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "smsdel";
    pygmyCmd.PrintHandler = print_ATCMGD;
    pygmyCmd.EventHandler = handler_ATCMGD;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );
    */

    return( FALSE );
}

u8 modem_smslist( u8 *ucBuffer )
{
    /*PYGMYCOMMAND pygmyCmd;

    pygmyCmd.Name = "smslist";
    pygmyCmd.PrintHandler = print_ATCMGL;
    pygmyCmd.EventHandler = handler_ATCMGL;
    pygmyCmd.Expire = 15;
    pygmyCmd.Retry = 1;
    cmdQueue( &globalModemQueue, &pygmyCmd );
    */
    print_ATCMGL();

    return( TRUE );
}

u8 modem_smssave( u8 *ucBuffer )
{
    print_CSAS( );
    
    return( TRUE );
}

//-------------------------------------End Modem Commands-------------------------------------
//--------------------------------------------------------------------------------------------

//--------------------------------------Response Handlers-------------------------------------
//--------------------------------------------------------------------------------------------

u8 handler_ATESCAPE( u8 *ucBuffer )
{
    u8 *ucParam, ucLen;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( isStringSame( ucParam, (u8*)AT_OK ) ){
        return( TRUE );
    }

    return( FALSE );
}

u8 handler_ATGET( u8 *ucBuffer )
{
    u8 i, *ucParams[ 6 ], ucLen;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 6, WHITESPACE|SEPARATORS|NEWLINE ); 
    
    modem_close( "now" );
    globalModemStatus &= ~(MODEM_HTTPINPROGRESS);
    
    // ToDo: Add code to process HTTP here
    // Check for pending failure and restart code as needed
    //if( !cmdRetries( &globalModemQueue ) ){
        // This will restart connection process
    //    modem_http( "" ); 
    //} // if

    return( 0 );
}

u8 handler_ATOK( u8 *ucBuffer )
{
    // Waits for OK response
    // Returns TRUE for "OK" FALSE for anything else
    u8 *ucSubString;

    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( ucSubString && isStringSame( ucSubString, (u8*)AT_OK ) ){
        return( TRUE );
    }

    return( FALSE );
}

u8 handler_HTTP( u8 *ucBuffer )
{
    // AT GET is the main() of the cell functions, it processes the HTTP transactions
    
    return( FALSE );
}

u8 handler_ATCSQ( u8 *ucBuffer )
{
    // +CSQ is the AT response for Signal Quality, 0-31, 99 = no service or unknown
    u8 *ucParams[ 2 ], ucSignal;
    
    getAllSubStrings( ucBuffer, ucParams, 2, WHITESPACE|PUNCT );
    if( isStringSame( ucParams[ 0 ], (u8*)AT_CSQ ) ){
        ucSignal = convertStringToInt( ucParams[ 1 ] );
        // ToDo: Do something with signal strength        
    } // if

    return( TRUE );
}

u8 handler_ATCCLK( u8 *ucBuffer )
{
    u8 *ucParam, *ucTime;//[ 2 ];

    ucParam = getNextSubString( ucBuffer, WHITESPACE );
    ucTime = getNextSubString( NULL, NEWLINE );
    print( MODEM_DEBUG, "\rATCCLK( %s, %s )", ucParam, ucTime );
    if( isStringSame( ucParam,  (u8*)AT_CCLK ) ){//&& set_time( ucParams[ 1 ] ) ){
        print( MODEM_DEBUG, "TIME( %s )" );
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handler_ATQSS( u8 *ucBuffer )
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

u8 handler_ATSERVINFO( u8 *ucBuffer )
{
  
    return( FALSE );
}

u8 handler_ATSGACT( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
   
    if( isStringSame( ucSubString, (u8*)AT_SGACT ) || isStringSame( ucSubString, (u8*)AT_OK ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

u8 handler_ATCGDCONT( u8 *ucBuffer )
{
    if( isStringSame( getNextSubString( ucBuffer, WHITESPACE|PUNCT ), (u8*)AT_OK ) ){
        return( TRUE );
    } // if
    
    return( FALSE );
}

u8 handler_ATSD( u8 *ucBuffer )
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

u8 handler_ATSH( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
    if( isStringSame( ucSubString, (u8*)AT_OK ) || isStringSame( ucSubString, (u8*)AT_ERROR ) ){
        return( TRUE ); // if already disconnected ERROR is valid
    } // if

    return( FALSE );
}

u8 handler_ATCMGS( u8 *ucBuffer )
{
    u8 *ucSubString;

    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT|NEWLINE );
   // print( MODEM_COM, "\rATCMGS( %s )", ucSubString );
    if( isStringSame( ucSubString, (u8*)AT_CMGS ) || isStringSame( ucSubString, (u8*)AT_CMS ) ){ 
        // Message fail
        if( cmdIsQueued( &globalModemQueue, (u8*)AT_CMGS ) == 1 ){
            
            return( TRUE );
        } // if
    } // if

    return( FALSE );
}

u8 handler_ATSMSPROMPT( u8 *ucBuffer )
{
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, NEWLINE );
    
    if( replaceChars( ucParam, ">", ' ' ) ){
        print( MODEM_COM, "\rATSMSPROMPT", ucParam );
        return( TRUE );
    } // if
    
    return( TRUE );
}

u8 handler_ATCMGF( u8 *ucBuffer )
{
    u8 *ucParam;

    ucParam = getNextSubString( ucBuffer, WHITESPACE|PUNCT|SEPARATORS );
    //print( MODEM_COM, "\rATCMGF( %s )", ucParam );
    if( isStringSame( ucParam, (u8*)AT_OK ) ){
        return( TRUE );
    } // if

    return( FALSE );
}

u8 handler_ATAT( u8 *ucBuffer )
{
    u8 *ucSubString;
    
    ucSubString = getNextSubString( ucBuffer, WHITESPACE|PUNCT );
    if( ucSubString ){
        if( isStringSame( ucSubString, (u8*)AT_OK ) ||
            isStringSame( ucSubString, (u8*)AT_ERROR ) || 
            isStringSame( ucSubString, (u8*)AT_MWI ) ){
            print( MODEM_COM, "\rRadio On\r" );
            deleteTask( "AT", 0 );
        
            return( TRUE );
        } // if
    } // if
    
    return( FALSE );
}

u8 handler_ATATOFF( u8 *ucBuffer )
{
    u8 *ucSubString;

    ucSubString = getNextSubString( ucBuffer, WHITESPACE|NEWLINE );
    if( ucSubString ){
        // cell_onoff( "" );
        return( TRUE ); // if cell responded then it's not off yet
    }

    return( FALSE ); 
}

u8 pftp_handler( u8 *ucBuffer )
{
    u8 *ucParams[ 5 ], ucLen, *ucSubString, *ucPayload;
    u16 uiCRC, uiLen, uiChunk;
    u32 i, ulPDIA;

    ucLen = getAllSubStrings( ucBuffer, ucParams, 5, WHITESPACE|PUNCT|SEPARATORS );
    
    if( isStringSame( ucParams[ 0 ], "PFTP" ) ){
        if( isStringSame( ucParams[ 1 ], (u8*)AT_ERROR ) ){
            // ERROR is returned when resource is unavailable, such as incorrect filename
            globalModemStatus &= ~(MODEM_PFTPINPROGRESS|MODEM_BUSY);
            modem_close( "now" ); // Clear command queue
            return( TRUE ); // was 0, must test, but 1 cancels
        }else if( !isStringSame( ucSubString, "EOF" ) ){
            uiChunk = convertStringToInt( ucParams[ 1 ] );
            print( MODEM_DEBUG, "\rChunkReq: %d", globalModemPFTP.CurrentChunk );
            print( MODEM_DEBUG, "\rChunkRcv: %d", uiChunk );
            uiLen = convertStringToInt( ucParams[ 2 ] );
                
            ucPayload = ucParams[ 3 ]; //getNextSubString( "", PYGMY_STRING_WHITE|PYGMY_STRING_SEPARATORS );
            print( MODEM_DEBUG, "\rPayLen: %d", ucParams[ 3 ] );
            if( convertHexEncodedStringToBuffer( ucPayload, ucPayload ) != uiLen ){
                print( MODEM_DEBUG, "\rLEN Fail!" );
                return( FALSE );
            } // if
                // Updated to 32bit PDIA, single frame in HEX n 03/17/2011
            pdiaEncode( 0, PDIA_NEW, 0 );
            for( i = 0, ulPDIA = 0, uiCRC = 0; i < uiLen; i++ ){
                pdiaEncode( ucPayload[ i ], PDIA_ADD, (u32*)&ulPDIA );
            } // for
            pdiaEncode( 0, PDIA_END, (u32*)&ulPDIA );
            //Next append h to mark string as hexadecimal
            i = len( ucParams[ 4 ] );
            ucParams[ 4 ][ i++ ] = 'h';
            ucParams[ 4 ][ i ] = '\0';
            // Then convert hexadecimal string to integer
            // Now compare integer against that produced by PDIA encoder
            if( ulPDIA != convertStringToInt( ucParams[ 4 ] ) ){ // Contains CRC
                print( MODEM_DEBUG, "\rData Corrupt!" );
                return( FALSE );
            } // if
            if( uiChunk == globalModemPFTP.CurrentChunk ){
                if( globalModemPFTP.CurrentChunk == 1 ){
                    //if( !Pygmy_PCMP_DecompressStreamToFile( ucPayload, uiLen, PYGMY_PCMP_START, 
                    //    backlinkData.PFTP_Compressed, backlinkData.PFTP_File.Name ) ){
                    //    return( 0 );
                    //}
                    ++globalModemPFTP.CurrentChunk;
                    pftp_get( "" );
                    return( TRUE );
                } else{
                    //if( !Pygmy_PCMP_DecompressStreamToFile( ucPayload, uiLen, PYGMY_PCMP_DATA, 
                    //    backlinkData.PFTP_Compressed, "" ) ){
                    //    return( FALSE );
                    //}
                    ++globalModemPFTP.CurrentChunk;
                    pftp_get( "" );
                    return( TRUE );
                } // else
                print( MODEM_DEBUG, "." );
            }
            return( FALSE );
        } else{
            //Pygmy_PCMP_DecompressStreamToFile( "", uiLen, PYGMY_PCMP_STOP, backlinkData.PFTP_Compressed, "" );
            globalModemStatus &= ~(MODEM_PFTPINPROGRESS|MODEM_BUSY);
            modem_close( "now" ); // Clear command queue
        } // else
            
        return( TRUE ); 
    } // if

    return( FALSE );
}

u8 pftp_put_handler( u8 *ucBuffer )
{
    u8 *ucParams[ 3 ], ucLen;//ucSubString1, *ucSubString2;

    
    ucLen = getAllSubStrings( ucBuffer, ucParams, 3, WHITESPACE|SEPARATORS );
    if( ucLen < 3 || !isStringSame( ucParams[ 0 ], "PFTP" ) ){
        return( FALSE );
    } // if

    if( isStringSame( ucParams[ 1 ], (u8*)AT_ERROR ) ){
        if( isStringSame( ucParams[ 2 ], "403" ) || isStringSame( ucParams[ 2 ], "400" ) ){
            // 403 is Forbidden, transaction must be canceled
            // 400 is Unsupported
            globalModemStatus &= ~(MODEM_PFTPINPROGRESS|MODEM_BUSY);
            modem_close( "now" ); // Clear command queue
            return( TRUE );
        } else {
            // Any other error requires a resend of last packet
            fileSetPosition( &globalModemPFTP.File, START, globalModemPFTP.CurrentChunk * globalModemPFTP.ChunkSize );
            return( FALSE ); // utilize retries
        } // else
    } else if( isStringSame( ucParams[ 2 ], (u8*)AT_OK ) ){
        print( MODEM_DEBUG, "." );
        if( globalModemPFTP.File.Attributes & EOF ){
            print( MODEM_DEBUG, "Upload Complete." );
            globalModemStatus &= ~(MODEM_PFTPINPROGRESS|MODEM_BUSY|MODEM_PFTPPUT);
            modem_close( "now" ); // Clear command queue
            return( TRUE );
        } // if
        ++globalModemPFTP.CurrentChunk;
        pftp_put( "" );
        return( TRUE );
    } // else if

    return( FALSE );
}

u8 handler_ATCSDH( u8 *ucBuffer )
{

    return( TRUE );
}

u8 handler_ATCMGR( u8 *ucBuffer )
{
    u8 *ucParam;

    //ucParam = getNextSubString( ucBuffer, NEWLINE );
    //print( MODEM_DEBUG, "SMS( %s )", ucParam );

    return( TRUE );
}

u8 handler_ATCMGD( u8 *ucBuffer )
{

    return( TRUE );
}

u8 handler_ATCMGL( u8 *ucBuffer )
{
    print( MODEM_DEBUG, "SMS( %s )", getNextSubString( ucBuffer, NEWLINE ) );

    return( TRUE );
}

//-----------------------------------End Response Handlers------------------------------------
//--------------------------------------------------------------------------------------------

//---------------------------------------Print Handlers---------------------------------------
//--------------------------------------------------------------------------------------------

void print_AT( void )
{
    // If modem is working this will return promptly with OK
    // This command is only to test communications
    // If modem doesn't respond within 1 second power cycle
    print( MODEM_COM, "ATE0&W\r" );
}

void print_CMGS( void )
{
    print( MODEM_COM, "AT+CMGS=%s\r", globalModemSMS.PhoneNumber );
}

void print_CMGSMessage( void )
{
    //print( MODEM_DEBUG, "\rSent \"%s\"", globalModemSMS.Message );
    //streamSetActionChars( MODEM_COM, "\r>" );
    print( MODEM_COM, "%s%s", globalModemSMS.Message, (u8*)AT_CTRLZ );
}

void print_CMGF( void )
{
    print( MODEM_COM, "%sAT+CMGF=1\r", (u8*)AT_CTRLZ );
}

void print_SH( void )
{
    print( MODEM_COM, "%sAT#SH=1\r", (u8*)AT_CTRLZ );
}

void print_SD( void )
{
    // AT#SD=<context><protocol><port><ip><closure><localport><mode>
    // <port> 0 = TCP, 1 = UDP
    // <closure> 0 = auto ( default )
    // <localport> for UDP connections
    // <mode> 0 = online, 1 = command
    
    print( MODEM_COM, "AT#SD=1,0,%d,\"", globalModemTCPPort);
    if( globalModemTCPPort == 25 ){
        print( MODEM_COM, "%s\"\r", globalModemSMTPServer );
    } else{ 
        print( MODEM_COM, "%s\"\r", globalModemHostName ); // URL 1st half
    }  // else
}

void print_CGDCONT( void )
{
    print( MODEM_COM, "AT+CGDCONT=1,\"IP\",\"" );
    //print( MODEM_COM,  ); //ToDo: Add global portal ( APN ) string
    print( MODEM_COM, "\",\"0.0.0.0\",0,0\r" );
}

void print_SGACT( void )
{
    // This function only prints out the basic command and context 
    println( MODEM_COM, "AT#SGACT?" );
}

void print_SGACT0( void )
{
    println( MODEM_COM, "AT#SGACT=1,0" );
}

void print_SGACT1( void )
{ 
    println( MODEM_COM, "AT#SGACT=1,1" );
}

void print_CCLK( void )
{
    println( MODEM_COM, "AT+CCLK?" );
}

void print_SERVINFO( void )
{
    println( MODEM_COM, "AT#SERVINFO" );
}

void print_QSS( void )
{
    println( MODEM_COM, "AT#QSS?" );
}

void print_CSQ( void )
{
    println( MODEM_COM, "AT+CSQ" ); 
}

void print_AUTHLOGIN( void )
{
    print( MODEM_COM, "%s%s", (u8*)EMAIL_AUTHLOGIN, (u8*)AT_CRLF );
}

void print_EMAILPW( void )
{
    u8 ucBuffer[40];
    
    // ToDo: Add variable pw string
    convertStringToBase64( globalModemEmail.Password, ucBuffer ); // SMTP Servers use Base64 encoded login data
    print( MODEM_COM, "%s%s", ucBuffer, (u8*)AT_CRLF );
}

void print_EMAILUSER( void )
{
    u8 ucBuffer[64];
    
    // ToDo: Add variable email address string
    convertStringToBase64( globalModemEmail.From, ucBuffer ); // SMTP Servers use Base64 encoded login data
    print( MODEM_COM, "%s%s", ucBuffer, (u8*)AT_CRLF );
}

void print_HELO( void )
{
    // ToDo: Add variable email server string
    print( MODEM_COM, "%s %s%s", (u8*)EMAIL_HELO, globalModemEmail.Domain, (u8*)AT_CRLF  ); 
}

void print_MAILFROM( void )
{
    // ToDo: Add variable email from address
    print( MODEM_COM, "%s %s%s", (u8*)EMAIL_MAILFROM, globalModemEmail.From, (u8*)AT_CRLF );
}

void print_RCPT( void )
{
    print( MODEM_COM, "%s <%s>%s", (u8*)EMAIL_RCPT, globalModemEmail.To, (u8*)AT_CRLF );
}

void print_DATA( void )
{
    print( MODEM_COM, "%s%s", (u8*)EMAIL_DATA, (u8*)AT_CRLF );
    // Now we wait for a 354 response and then send Email body
}

void print_EMAILBODY( void )
{
    print( MODEM_COM, "%s <%s>%s", (u8*)EMAIL_From, globalModemEmail.From, (u8*)AT_CRLF );
    print( MODEM_COM, "%s <%s>%s", (u8*)EMAIL_To, globalModemEmail.To, (u8*)AT_CRLF );
   
    print( MODEM_COM, "%s %s%s", (u8*)EMAIL_Subject, globalModemEmail.Subject, (u8*)AT_CRLF );
    print( MODEM_COM, "%s%s", (u8*)EMAIL_MIMVersion, (u8*)AT_CRLF );
    print( MODEM_COM, "%s %s%s", (u8*)EMAIL_Priority, globalModemEmail.Priority, (u8*)AT_CRLF );
    print( MODEM_COM, "%s %s%s", (u8*)EMAIL_Importance, globalModemEmail.Importance, (u8*)AT_CRLF );
    print( MODEM_COM, "%s%s", (u8*)EMAIL_ContentType, (u8*)AT_CRLF );
    print( MODEM_COM, "%s%s%s", (u8*)EMAIL_ContentEncoding, (u8*)AT_CRLF, (u8*)AT_CRLF );
    
    // Content Of Email
    // ToDo: Add email content print code
}

void print_QUIT( void )
{
    // End sequence for SMTP is <CR><LF>.<CR><LF>
    // Ends body of e-mail, does not close connection
    print( MODEM_COM, "%s%s.%s%s", (u8*)AT_CRLF, (u8*)AT_CRLF, (u8*)AT_CRLF, (u8*)AT_CRLF ); 
}

void print_wait( void )
{
    // This page left blank intentionally :)
}

void print_pftp_getlen( void )
{
    print_pftp_GET();
    // Any payload for CGI goes here
    print( MODEM_COM, "getlen=%s", globalModemPFTP.File.Name);
    // End Payload for CGI   
    print_HTTP_GET_secondhalf();
}

void print_pftp_get( void )
{
    // This function establishes connection
    globalModemStatus |= MODEM_PFTPINPROGRESS;
    print( MODEM_COM, "GET /cgi/pftp.py?get=", globalModemPFTP.File.Name );
    if( globalModemPFTP.ChunkSize != 256 ) { // 256 is PFTP server default
        print( MODEM_COM, "&sz=%d", globalModemPFTP.ChunkSize);
    } // if
    print( MODEM_COM, "&chk=%d", globalModemPFTP.CurrentChunk );
    print_http();
    
}

void print_http( void )
{
    print( MODEM_COM, " HTTP/1.1%s", (u8*)AT_CRLF );
    print( MODEM_COM, "Host: %s%s", globalModemHostName, (u8*)AT_CRLF );
    print( MODEM_COM, "Connection: keep-alive%s%s", (u8*)AT_CRLF, (u8*)AT_CRLF );
}

void print_ATESCAPE( void )
{
    print( MODEM_COM, (u8*)AT_ESCAPE );
}

void print_ATCMGR( void )
{
    print( MODEM_COM, "AT+CMGR=1\r" );
}

void print_ATCMGD( void )
{
    // index, flag, flag values follow:
    // 0, delete message at index
    // 1,2 delete all read messages
    // 3, delete all messages
    print( MODEM_COM, "AT+CMGD=1,4\r" );
}

void print_ATCMGL( void )
{
    print( MODEM_COM, "AT+CMGL\r" );
}

void print_ATCSDH( void )
{
    print( MODEM_COM, "AT+CSDH=1\r" );
}

void print_CSAS( void )
{
    print( MODEM_COM, "AT+CSAS\r" );
}


//-------------------------------------End Print Handlers-------------------------------------
//--------------------------------------------------------------------------------------------
