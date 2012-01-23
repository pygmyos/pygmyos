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

PYGMYI2CPORT globalI2CEEPROM, globalI2CDigipot;
u16 globalVoltShieldStatus = 0; // Used for auto-init

void voltShieldInit( void )
{
    u16 i;

    globalVoltShieldStatus = 1;
    print( COM3, "\rOpening I2C EEPROM" );
    i2cConfig( &globalI2CEEPROM, SHIELD_SCL, SHIELD_SDA, 0x50, 0 );
    print( COM3, "\rOpening I2C Digipot" );
    i2cConfig( &globalI2CDigipot, SHIELD_SCL, SHIELD_SDA, 0x2F, 0 ); // A1-A0 floating or tied to VCC
    print( COM3, "\rConfiguring pins" );
    pinConfig( SHIELD_VENABLE, OUT );
    pinConfig( SHIELD_RESET, OUT );
    pinConfig( SHIELD_INSW1, OUT );
    pinConfig( SHIELD_INSW2, OUT );
    pinConfig( SHIELD_INSW3, OUT );
    pinConfig( SHIELD_INSW4, OUT );
    pinConfig( SHIELD_RES1, OUT );
    pinConfig( SHIELD_RES2, OUT );
    pinConfig( SHIELD_RES3, OUT );
    pinConfig( SHIELD_RES4, OUT );
    pinConfig( SHIELD_VOLT1, ANALOG );
    pinConfig( SHIELD_VOLT2, ANALOG );
    pinConfig( SHIELD_VOLT3, ANALOG );
    pinConfig( SHIELD_VOLT4, ANALOG );

    pinSet( SHIELD_VENABLE, LOW ); // Disable Voltage Regulator
    for( i = 0; i < 4; i++ ){
        voltShieldSetInputMode( i, VOLTAGE );
        voltShieldSetCoupling( i, DC );
    } // for
    pinSet( SHIELD_RESET, LOW );
    delay( 30 );
    pinSet( SHIELD_RESET, HIGH );
}

u16 voltShieldGetGain( u8 ucChannel )
{
    u16 uiGain;

    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if
    uiGain = digipotGetWiper( &globalI2CDigipot, ucChannel - 1 ); // Wipers are 0 base
    if( uiGain < 127 ){
        return( 0 );
    } // if
    uiGain = uiGain / ( 1 + ( 0xFF - uiGain  ) );

    return( uiGain );
}

void voltShieldSetGain( u8 ucChannel, u16 uiGain )
{
    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if
    if( uiGain > 255 ){
        return;
    } // if
    uiGain = 0xFF - ( 0xFF / ( 1 + uiGain ) );
    digipotSetWiper( &globalI2CDigipot, ucChannel - 1, uiGain );
}

void voltShieldSetCoupling( u8 ucChannel, u8 ucCoupling )
{
    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if

    if( ucChannel == 1 ){
        pinSet( SHIELD_INSW1, ucCoupling );
    } else if( ucChannel == 2 ){
        pinSet( SHIELD_INSW2, ucCoupling );
    } else if( ucChannel == 3 ){
        pinSet( SHIELD_INSW3, ucCoupling );
    } else if( ucChannel == 4 ){
        pinSet( SHIELD_INSW4, ucCoupling );
    } // else if
}

void voltShieldSetInputMode( u8 ucChannel, u8 ucMode )
{
    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if

    if( ucChannel == 1 ){
        pinSet( SHIELD_RES1, ucMode );
        if( ucMode == RESISTANCE ){
            pinSet( SHIELD_INSW1, DC );
        } // if
    } else if( ucChannel == 2 ){
        pinSet( SHIELD_RES2, ucMode );
        if( ucMode == RESISTANCE ){
            pinSet( SHIELD_INSW2, DC );
        } // if
    } else if( ucChannel == 3 ){
        pinSet( SHIELD_RES3, ucMode );
        if( ucMode == RESISTANCE ){
            pinSet( SHIELD_INSW2, DC );
        } // if
    } else if( ucChannel == 4 ){
        pinSet( SHIELD_RES4, ucMode );
        if( ucMode == RESISTANCE ){
            pinSet( SHIELD_INSW4, DC );
        } // if
    } // else if
}

u16 voltShieldGetVoltage( u8 ucChannel )
{
    u16 uiADC;

    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if
    voltShieldSetInputMode( ucChannel, VOLTAGE );
    if( ucChannel = 1 ){
        uiADC = pinAnalog( SHIELD_VOLT1 );
    } else if( ucChannel == 2 ){
        uiADC = pinAnalog( SHIELD_VOLT2 );
    } else if( ucChannel == 3 ){
        uiADC = pinAnalog( SHIELD_VOLT3 );
    } else if( ucChannel == 4 ){
        uiADC = pinAnalog( SHIELD_VOLT4 );
    } else{
        return( 0xFFFF );
    } // else

    return( uiADC );
}

u16 voltShieldGetResistance( u8 ucChannel )
{
    u16 uiADC;

    if( !globalVoltShieldStatus ){
        voltShieldInit();
    } // if
    pinSet( SHIELD_VENABLE, HIGH ); // Enable Voltage Regulator
    voltShieldSetInputMode( ucChannel, RESISTANCE );
    if( ucChannel == 1 ){
        uiADC = pinAnalog( SHIELD_VOLT1 );
    } else if( ucChannel == 2 ){
        uiADC = pinAnalog( SHIELD_VOLT2 );
    } else if( ucChannel == 3 ){
        uiADC = pinAnalog( SHIELD_VOLT3 );
    } else if( ucChannel == 4 ){
        uiADC = pinAnalog( SHIELD_VOLT4 );
    } else{
        return( 0xFFFF );
    } // else
    pinSet( SHIELD_VENABLE, LOW ); // Disable Voltage Regulator to save power

    // ToDo: Add code to calculate corrected resistance based on voltage drop measured
    return( uiADC );
}

