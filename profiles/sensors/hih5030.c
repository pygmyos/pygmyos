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

u8 globalHumidityCR = 0;
u8 globalHumidityPin, globalHumidityPower;

float hih5030Read( void )
{
    float fReading;

    if( globalHumidityCR == 0 ){
        return( 0 );
    } // if

    if( globalHumidityPower != 0 && pinGet( globalHumidityPower ) == LOW ){
        pinSet( globalHumidityPower, HIGH );
    } // if
    delayms( 1 );
    fReading = (((double)pinAnalog( globalHumidityPin ) / (float)3300) * 3.3);
    // Define the voltage the sensor returns at 0% humidity.
    float maxVoltage = (2.1582 - (0.004426 * 75.0));
    fReading = ((fReading - 0.528) / maxVoltage) * 100;

    return( fReading );
}

void hih5030Init( u8 ucAnalogPin, u8 ucPowerPin )
{
    globalHumidityCR = 1;
    globalHumidityPin = ucAnalogPin;
    globalHumidityPower = ucPowerPin;
    pinConfig( ucAnalogPin, ANALOG );
    if( ucPowerPin != NONE ){
        pinConfig( ucPowerPin, OUT );
        pinSet( ucPowerPin, HIGH );
    } // if
}

void hih5030Disable( void )
{
    if( globalHumidityPower != 0 ){
        pinSet( globalHumidityPower, LOW );
    }
}
