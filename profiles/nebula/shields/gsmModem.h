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

#pragma once

#define MODEM_LED1                  T2
#define MODEM_LED2                  T1
#define MODEM_SERVICE               T3
#define MODEM_CTS                   A0
#define MODEM_RTS                   A1
#define MODEM_STAT                  D3
#define MODEM_PWRMON                D2
#define MODEM_RESET                 D1
#define MODEM_ONOFF                 D0
#define MODEM_TX                    TX2
#define MODEM_RX                    RX2

#define MODEM_MAXQUEUE              16

#define MODEM_COM                   COM2
#define MODEM_DEBUG                 COM3

#define MODEM_BUSY                  BIT0
#define MODEM_SMSINPROGRESS         BIT1
#define MODEM_HTTPINPROGRESS        BIT2
#define MODEM_PFTPINPROGRESS        BIT3
#define MODEM_EMAILINPROGRESS       BIT4
#define MODEM_PFTPPUT               BIT5
#define MODEM_PFTPGET               BIT6

typedef struct {
                u8 PhoneNumber[16];
                u8 Message[ 128 ];
                //u8 *Message;
                u8 Format;
                PYGMYFILE File;
                } PYGMYSMS;

typedef struct {
                u8 Domain[ 40 ];
                u8 Password[ 20 ];
                u8 Priority[ 12 ];
                u8 Importance[ 12 ];
                u8 To[ 40 ];
                u8 From[ 40 ];
                u8 Subject[ 40 ];
                u8 Format;
                u8 *Body;
                PYGMYFILE File;
                } PYGMYEMAIL;

typedef struct {
                u8 Portal[ 40 ];
                u8 URL[ 128 ];
                u8 Port;
                } PYGMYHTTP;

typedef struct{
                u16 ChunkSize;
                u16 CurrentChunk;
                u8 *HostName;
                PYGMYFILE File;
                } PYGMYPFTP;
            
extern const PYGMYCMD PYGMYMODEMCMDS[];
            
extern const u8 AT_OK[];
extern const u8 AT_ERROR[];
extern const u8 AT_CONNECT[];
extern const u8 AT_NOCARRIER[];
extern const u8 AT_ESCAPE[];
extern const u8 AT_SGACT[];
extern const u8 AT_CMGF[];
extern const u8 AT_CMGS[];
extern const u8 AT_CMS[];
extern const u8 AT_CSQ[];
extern const u8 AT_QSS[];
extern const u8 AT_CCLK[];
extern const u8 AT_MWI[];
extern const u8 AT_CGDCONT[];
extern const u8 AT_CGDATA[];
extern const u8 AT_SD[];
extern const u8 AT_SH[];
extern const u8 AT_CMGF[];
extern const u8 AT_ATE[];
extern const u8 AT_ATIP[];
extern const u8 AT_W[];
extern const u8 AT_SERVINFO[];
extern const u8 AT_VTS[]; 
extern const u8 AT_VTD[]; 
extern const u8 AT_IPR[]; 
extern const u8 AT_IFC[]; 
extern const u8 AT_ICF[]; 

extern const u8 AT_CRLF[];
extern const u8 AT_CTRLZ[];
extern const u8 AT_ESC[];

extern const u8 EMAIL_AUTHLOGIN[];
extern const u8 EMAIL_HELO[];
extern const u8 EMAIL_RCPT[];
extern const u8 EMAIL_MAILFROM[];
extern const u8 EMAIL_DATA[];
extern const u8 EMAIL_QUIT[];
extern const u8 EMAIL_From[];
extern const u8 EMAIL_To[];
extern const u8 EMAIL_Cc[];
extern const u8 EMAIL_Subject[];
extern const u8 EMAIL_ContentType[];
extern const u8 EMAIL_ContentEncoding[];
extern const u8 EMAIL_MIMVersion[];
extern const u8 EMAIL_Priority[];
extern const u8 EMAIL_Importance[];
extern const u8 EMAIL_220[];
extern const u8 EMAIL_221[];
extern const u8 EMAIL_235[];
extern const u8 EMAIL_250[];
extern const u8 EMAIL_334[];
extern const u8 EMAIL_354[];
extern const u8 EMAIL_451[]; 
extern const u8 EMAIL_530[];

extern const u8 HTTP_Connection[];
extern const u8 HTTP_keepalive[];
extern const u8 HTTP_V1P0[];
extern const u8 HTTP_V1P1[];
extern const u8 HTTP_Host[];
extern const u8 HTTP_GET[];

void modemInit( void );
u8 modemtask_reset( void );
u8 modemtask_onoff( void );

u8 modem_flow( u8 *ucBuffer );
u8 modem_csq( u8 *ucBuffer );
u8 modem_echo( u8 *ucBuffer );
u8 modem_imei( u8 *ucBuffer );
u8 modem_simin( u8 *ucBuffer );
u8 modem_reset( u8 *ucBuffer );
u8 modem_onoff( u8 *ucBuffer );
u8 modem_email( u8 *ucBuffer );
u8 modem_sms( u8 *ucBuffer );
u8 modem_http( u8 *ucBuffer );
u8 modem_pftp( u8 *ucBuffer );
u8 modem_carrier( u8 *ucBuffer );
u8 modem_time( u8 *ucBuffer );
u8 modem_attach( u8 *ucBuffer );
u8 modem_attachtest( u8 *ucBuffer );
u8 modem_detach( u8 *ucBuffer );
u8 modem_portal( u8 *ucBuffer );
u8 modem_suspend( u8 *ucBuffer );
u8 modem_close( u8 *ucBuffer );
u8 modem_sd( u8 *ucBuffer );
u8 modem_smsread( u8 *ucBuffer );
u8 modem_smsdel( u8 *ucBuffer );
u8 modem_smslist( u8 *ucBuffer );
                                    
void email_wait( void );
void email_HELO( void );
void email_AUTHLOGIN( void );
void email_user( void );
void email_pw( void );
void email_MAILFROM( void );
void email_RCPT( void );
void email_DATA( void );
void email_body( void );
void email_quit( void );
void email_wait( void );
u8 email_get( u8 *ucBuffer );

u8 pftp_close( u8 *ucBuffer );
u8 pftp_close_handler( u8 *ucBuffer );
u8 pftp_get( u8 *ucBuffer );

void print_CSQ( void );
void print_QSS( void );
void print_HELO( void );
void print_EMAILPW( void );
void print_EMAILUSER( void );
void print_EMAILUSER( void );
void print_AUTHLOGIN( void );
void print_MAILFROM( void );
void print_RCPT( void );
void print_DATA( void );
void print_EMAILBODY( void );
void print_QUIT( void );
void print_wait( void );
void print_CMGF( void );
void print_CMGS( void );
void print_CMGSMessage( void );
void print_GET( void );
void print_pftp_put( void );
void print_pftp_get( void );
void print_pftp_close( void );
void print_SERVINFO( void );
void print_CCLK( void );
void print_SGACT( void );
void print_SGACT1( void );
void print_SGACT0( void );
void print_CGDCONT( void );
void print_ATESCAPE( void );
void print_SH( void );
void print_SD( void );
void print_http( void );
void print_ATCMGR( void );
void print_ATCMGD( void );
void print_ATCMGL( void );
void print_ATCSDH( void );

u8 handler_ATCSQ( u8 *ucBuffer );
u8 handler_ATQSS( u8 *ucBuffer );
u8 handler_ATCMGF( u8 *ucBuffer );
u8 handler_ATSMSPROMPT( u8* ucBuffer );
u8 handler_ATCMGS( u8 *ucBuffer );
u8 handler_ATGET( u8 *ucBuffer );
u8 pftp_put_handler( u8 *ucBuffer );
u8 pftp_handler( u8 *ucBuffer );
u8 pftp_close_handler( u8 *ucBuffer );
u8 handler_ATSERVINFO( u8 *ucBuffer );
u8 handler_ATCCLK( u8 *ucBuffer );
u8 handler_ATSGACT( u8 *ucBuffer );
u8 handler_ATSGACT( u8 *ucBuffer );
u8 handler_ATCGDCONT( u8 *ucBuffer );
u8 handler_ATESCAPE( u8 *ucBuffer );
u8 handler_ATSH( u8 *ucBuffer );
u8 handler_ATSD( u8 *ucBuffer );
u8 handler_ATCMGR( u8 *ucBuffer );
u8 handler_ATCMGD( u8 *ucBuffer );
u8 handler_ATCMGL( u8 *ucBuffer );
u8 handler_ATCSDH( u8 *ucBuffer );
