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

#define GSM_LED1                T2
#define GSM_LED2                T1
#define GSM_SERVICE             T3
#define GSM_CTS                 A0
#define GSM_RTS                 A1
#define GSM_STAT                D3
#define GSM_PWRMON              D2
#define GSM_RST                 D1
#define GSM_ONOFF               D0
#define GSM_TX                  TX2
#define GSM_RX                  RX2

#define GSM_COM                 COM2

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
u8 modem_detach( u8 *ucBuffer );
u8 modem_portal( u8 *ucBuffer );
u8 modem_suspend( u8 *ucBuffer );
u8 modem_close( u8 *ucBuffer );
                                    
