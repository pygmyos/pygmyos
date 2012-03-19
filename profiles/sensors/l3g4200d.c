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

PYGMYI2CPORT globalGyroPort;
PYGMYVOIDFUNC GyroEventHandler, GyroDataHandler;

u8 gyroInit( u8 ucSCL, u8 ucSDA, u8 ucINT1, u8 ucINT2, u8 ucAddress, void *EventHandler, void* DataHandler )
{
    u8 ucTmpAddress;

    if( ucSCL == NONE || ucSDA == NONE || ucINT2 == NONE || !GyroDataHandler ){
        // Only INT1 ( programmable interrupt ) is optional 
        return( FALSE );
    } // if
    GyroEventHandler = EventHandler;
    GyroDataHandler = DataHandler;
    if( ucAddress == 0 ){
        // User is requesting device address 0 ( SD0 == LOW )
        ucTmpAddress = L3G4200D_ADDRESS_0;
    } else if( ucAddress == 1 ){
        // User is requesting device address 1 ( SD0 == HIGH )
        ucTmpAddress = L3G4200D_ADDRESS_1;
    } else{
        // User is applying address directly ( not recommended )
        ucTmpAddress = ucAddress;
    } // else
    i2cConfig( &globalGyroPort, ucSCL, ucSDA, ucTmpAddress, I2CSPEEDFAST );
    if( ucINT1 != NONE && GyroEventHandler ){
        pinInterrupt( GyroEventHandler, ucINT1, TRIGGER_FALLING, 5 );
    } // if
    pinInterrupt( GyroDataHandler, ucINT2, TRIGGER_FALLING, 5 );
    
    return( TRUE );
}

void gyroSetThreshholds( u16 uiX, u16 uiY, u16 uiZ )
{

}

u16 gyroGetXThreshhold( void )
{

}

u16 gyroGetYThreshhold( void )
{

}

u16 gyroGetZThreshhold( void )
{

}

u16 gyroGetX( void )
{

}

u16 gyroGetY( void )
{

}

u16 gyroGetZ( void )
{

}

gyroGetStatus( void )
{

}
