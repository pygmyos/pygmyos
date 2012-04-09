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

// This file supports the Nebula Gas Sensor Shield, with any of the optional
// Gas Sensors available for the shield.

#pragma once

#include "pygmy_profile.h"

#define GASSENSOR_RESET         MCO
#define GASSENSOR_SCL           TX1
#define GASSENSOR_SDA           RX1
#define GASSENSOR_OUTPUT        T0
#define GASSENSOR_BOOST_PWM     T1
#define GASSENSOR_BOOST_EN      T2
#define GASSENSOR_SENSOR        A5

/*
#define GASSENSOR_RESET         A4
#define GASSENSOR_SCL           TX1
#define GASSENSOR_SDA           RX1
#define GASSENSOR_OUTPUT        T0
#define GASSENSOR_BOOST_PWM     T1
#define GASSENSOR_BOOST_EN      T2
#define GASSENSOR_SENSOR        A5
*/
void gasSensorDisableBoost( void );
void gasSensorEnableBoost( boid );
void gasSensorSetOutput( u8 ucState );
u8 gasSensorGetOutput( void );
u8 gasSensorInit( void );
u8 gasSensorGetGain( u8 ucWiper );
u8 gasSensorSetGain( u8 ucGain );
u8 gasSensorSetLoad( u8 ucLoad );
u8 gasSensorSetHeaterPWM( u8 ucDutyCycle );
float gasSensorRead( void );
