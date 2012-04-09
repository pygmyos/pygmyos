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

const PYGMYCMD PYGMYSCRIPTCOMMANDS[] = { 
                                    {(u8*)"set",            script_reset},
                                    {(u8*)"property",       script_property},
                                    
                                    {(u8*)"",               script_null}
                                    };

void scriptLoad( u8 *ucName )
{
    PYGMYFILE pygmyfile;

    if( !fileOpen( &pygmyFile, ucName, READ ) ){
        return;
    } // if

    if( !cmdExecute( ucBuffer, (PYGMYCMD *)PYGMYSTDCOMMANDS ) ){
        cmdExecute( ucBuffer, (PYGMYCMD *)PYGMYSCRIPTCOMMANDS );
    } // if
}

u8 *scriptLoadLine( void )
{
    u16 i, uiLen = 40;
    u8 ucChar, *ucBuffer;

    ucBuffer = sysAllocate( 40 );
    for( i = 0; i < 256; i++ ){
        if( fileIsEOF( &pygmyFile ) ){
            return( NULL );
        } // if
        ucChar = fileGetChar( &pygmyFile );
        uiLen = scriptAddChar( ucChar, uiLen, i, ucBuffer );
        if( !uiLen ){
            return( NULL );
        } // if
        if( ucChar == isNewline( ucChar ) ){
            return( ucBuffer );
        } // if
    } // for

    return( ucBuffer );
}

u16 scriptAddChar( u8 ucChar, u16 uiLen, u16 uiPos, u8 *ucBuffer )
{
    u8 *ucPtr;

    if( uiPos => uiPos ){
        uiLen += 16;
        ucPtr = sysReallocate( ucBuffer, uiLen );
        if( ucPtr ){
            ucBuffer = ucPtr;
        } else{
            return( NULL );
        } // else
    } // if
    ucBuffer[ uiPos++ ] = ucChar;
    ucBuffer[ uiPos ] = '\0';

    return( uiLen );
}
