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

#pragma once

extern const PYGMYCMD STDCOMMANDS[];

void cmdGetsCOM1( void );
void cmdGetsCOM2( void );
void cmdGetsCOM3( void );
void cmdGetsCOM4( void );
void cmdGetsCOM5( void );
void cmdSetActionChars( u8 ucStream, u8 *ucString );
void cmdInit( void );
u8 cmdExecute( u8 *ucBuffer, PYGMYCMD *pygmyCmds );

u8 cmdNull( u8 *ucBuffer );
u8 cmd_reset( u8 *ucBuffer );
u8 cmd_peek( u8 *ucBuffer );
u8 cmd_poke( u8 *ucBuffer );
u8 cmd_ps( u8 *ucBuffer );
u8 cmd_kill( u8 *ucBuffer );
u8 cmd_time( u8 *ucBuffer );

u8 cmd_pinset( u8 *ucBuffer );
u8 cmd_pinget( u8 *ucBuffer );
u8 cmd_pinanalog( u8 *ucBuffer );
u8 cmd_pinpwm( u8 *ucBuffer );
u8 cmd_pinconfig( u8 *ucBuffer );

u8 cmd_mount( u8 *ucBuffer );
u8 cmd_format( u8 *ucBuffer );
u8 cmd_read( u8 *ucBuffer );
u8 cmd_write( u8 *ucBuffer );
u8 cmd_ls( u8 *ucBuffer );
u8 cmd_rm( u8 *ucBuffer );
u8 cmd_open( u8 *ucBuffer );
u8 cmd_mv( u8 *ucBuffer );

u8 cmd_rfscan( u8 *ucBuffer );
u8 cmd_rfopen( u8 *ucBuffer );
u8 cmd_rfsend( u8 *ucBuffer );

