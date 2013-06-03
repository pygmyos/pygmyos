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

#pragma once
//#ifndef __PYGMY_HEADER_XMODEM
	////#define __PYGMY_HEADER_XMODEM
#include "pygmy_type.h"
#include "pygmy_profile.h"
#include "pygmy_pfat.h"

#define XMODEM_ACTIVE       BIT1
#define XMODEM_ACK          0x06
#define XMODEM_NACK         0x15
#define XMODEM_EOT          0x04
#define XMODEM_SOH          0x01
#define XMODEM_CAN          0x18
#define XMODEM_NEXT         0x00
#define XMODEM_LAST         0x01
#define XMODEM_RECV         BIT1
#define XMODEM_MODE_SOH     BIT2
#define XMODEM_SEND         BIT3
#define XMODEM_SEND_WAIT    BIT4
#define XMODEM_SEND_EOT     BIT5

#define XMODEM_MAXFAILURES  10
 
typedef struct {
                PYGMYFILE *File;
                u32 Status;
                u32 Timeout;
                u32 Transaction;
                u32 Count;
                u8 Buffer[ 132 ];
                u8 Failures;
                u8 Index;
                u8 Port;
                u8 Enabled;
                } PYGMYXMODEM;

void xmodemInit( PYGMYXMODEM *XModem );
void xmodemEnable( PYGMYXMODEM *XModem );
void xmodemDisable( PYGMYXMODEM *XModem );
u8 xmodemRecv( PYGMYXMODEM *XModem, u8 *FileName );
u8 xmodemSend( PYGMYXMODEM *XModem, u8 *FileName );
u8 xmodemProcess( PYGMYXMODEM *XModem, u8 Byte );
void xmodemProcessTimer( PYGMYXMODEM *XModem );
void xmodemSendPacket( PYGMYXMODEM *XModem, u8 Last );
u8 xmodemWritePacket( PYGMYXMODEM *XModem );//, u8 *ucBuffer );

//#endif // __PYGMY_HEADER_XMODEM
