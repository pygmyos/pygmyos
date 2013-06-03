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
#include "pygmy_profile.h"
#include "pygmy_type.h"
extern const PYGMYCMD STDCOMMANDS[];
extern const PYGMYCMD PYGMYSTDCOMMANDS[];

void cmdGetsCOM1( void );
void cmdGetsCOM2( void );
void cmdGetsCOM3( void );
void cmdGetsCOM4( void );
void cmdGetsCOM5( void );
void cmdSetActionChars( u8 ucStream, u8 *ucString );
void cmdInit( void );
u8 cmdOpenPort( u8 ucProtocol, u8 ucPin0, u8 ucPin1, u8 ucPin2, u8 ucPin3, u8 ucAddress );
u8 cmdExecute( u8 *ucBuffer, PYGMYCMD *pygmyCmds );

u8 cmd_set( u8 *ucBuffer );

u8 cmdNull( u8 *ucBuffer );
u8 cmd_play( u8 *ucBuffer );
u8 cmd_volume( u8 *ucBuffer );
u8 cmd_reset( u8 *ucBuffer );
u8 cmd_peek( u8 *ucBuffer );
u8 cmd_poke( u8 *ucBuffer );
u8 cmd_ps( u8 *ucBuffer );
u8 cmd_kill( u8 *ucBuffer );
u8 cmd_time( u8 *ucBuffer );

u8 cmd_pinevent( u8 *ucBuffer );
u8 cmd_pinset( u8 *ucBuffer );
u8 cmd_pinget( u8 *ucBuffer );
u8 cmd_pinanalog( u8 *ucBuffer );
u8 cmd_pinpwm( u8 *ucBuffer );
u8 cmd_pinconfig( u8 *ucBuffer );

u8 cmd_mnt( u8 *ucBuffer );
u8 cmd_format( u8 *ucBuffer );
u8 cmd_read( u8 *ucBuffer );
u8 cmd_write( u8 *ucBuffer );
u8 cmd_ls( u8 *ucBuffer );
u8 cmd_rm( u8 *ucBuffer );
u8 cmd_open( u8 *ucBuffer );
u8 cmd_close( u8 *ucBuffer );
u8 cmd_mv( u8 *ucBuffer );
u8 cmd_cp( u8 *ucBuffer );

u8 cmd_voltshield( u8 *ucBuffer );
u8 cmd_modem( u8 *ucBuffer );

u8 cmd_humidity( u8 *ucBuffer );
u8 cmd_port( u8 *ucBuffer );

u8 cmd_rflist( u8 *ucBuffer );
u8 cmd_rfget( u8 *ucBuffer );
u8 cmd_rfput( u8 *ucBuffer );
u8 cmd_rfscan( u8 *ucBuffer );
u8 cmd_rfopen( u8 *ucBuffer );
u8 cmd_rfsend( u8 *ucBuffer );

u8 cmd_eeprom( u8 *ucBuffer );
u8 cmd_gassensor( u8 *ucBuffer );

//extern const u8 PYGMY_confirm[];



