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
#include "profiles/nebula/shields/gasSensor.h"
#include "profiles/digipots/mcp443x.h"
#include "profiles/eeprom/at24hc02b.h"

PYGMYI2CPORT globalGasSensorEEPROM, globalGasSensorDigipot;
u16 globalGasSensorCR = 0;

u8 gasSensorInit( void )
{
    u8 ucBuffer[ 5 ];

    globalGasSensorCR = BIT0;
    
    pinConfig( GASSENSOR_SENSOR, ANALOG ); // Output of OpAmp
    pinConfig( GASSENSOR_BOOST_EN, OUT ); // Enables 5V boost converter when set HIGH
    pinSet( GASSENSOR_BOOST_EN, HIGH ); 
    pinConfig( GASSENSOR_OUTPUT, OUT); // General purpose high current output
    pinSet( GASSENSOR_OUTPUT, LOW ); // Default output to LOW ( off ) 
    pinConfig( GASSENSOR_RESET, OUT );
    pinSet( GASSENSOR_RESET, LOW );
    delay( 100 );
    pinSet( GASSENSOR_RESET, HIGH );
    // Generate PWM drive voltage for Gas Sensor ( 100% to read )
    // Sensor takes up to half an hour to read if not kept warm
    // If battery power allows, 50% drive voltage will keep sensor warm for quick sampling
    //pinPWM( GASSENSOR_BOOST_PWM, 300, 50 );
    pinConfig( GASSENSOR_BOOST_PWM, OUT );
    pinSet( GASSENSOR_BOOST_PWM, HIGH );
    eepromOpen( 0x50, TX1, RX1, NONE );
    eepromPutString( 0, "Test" );
    eepromGetBuffer( 0, ucBuffer, 4 );
    ucBuffer[ 4 ] = '\0';
    print( COM3, "\rEERPOM Test: %s", ucBuffer );
    i2cConfig( &globalGasSensorDigipot, TX1, RX1,0x2F, I2CSPEEDFAST );
    //digipotSetWiper( &globalGasSensorDigipot, 2, 200 );
}

void gasSensorSetOutput( u8 ucState )
{
    pinSet( GASSENSOR_OUTPUT, ucState );
}

u8 gasSensorGetOutput( void )
{
    return( pinGet( GASSENSOR_OUTPUT ) );
}

u8 gasSensorSetHeaterPWM( u8 ucDutyCycle )
{
    if( ucDutyCycle > 100 ){
        return( FALSE );
    } // if
    if( ucDutyCycle == 0 ){
        pinConfig( GASSENSOR_BOOST_PWM, OUT );
        pinSet( GASSENSOR_BOOST_PWM, LOW );
    } else if( ucDutyCycle == 100 ){
        pinConfig( GASSENSOR_BOOST_PWM, OUT );
        pinSet( GASSENSOR_BOOST_PWM, HIGH );
    } else{
        pinPWM( GASSENSOR_BOOST_PWM, 100, ucDutyCycle );
    } // else

    return( TRUE );
}

void gasSensorBoostDisable( void )
{
    pinSet( GASSENSOR_BOOST_EN, LOW );
}

void gasSensorBoostEnable( boid )
{
    pinSet( GASSENSOR_BOOST_EN, HIGH );
}

u8 gasSensorGetGain( u8 ucWiper )
{
    return( digipotGetWiper( &globalGasSensorDigipot, ucWiper ) );
}

u8 gasSensorSetGain( u8 ucGain )
{
    digipotSetWiper( &globalGasSensorDigipot, 3, ucGain );
}

u8 gasSensorSetLoad( u8 ucLoad )
{
    // Use to set load resistance
    digipotSetWiper( &globalGasSensorDigipot, 2, ucLoad );
}

float gasSensorRead( void )
{
    float fSample, fFinal;
    u16 uiSample;

    uiSample = pinAnalog( GASSENSOR_SENSOR ); 
    fSample = uiSample;
    print( COM3, "\r\tGas Sensor ADC: %d", uiSample );
    fFinal = sysGetADCVoltage();
    fFinal /= 4096.0;
    fFinal *= fSample;

    return( fFinal );
}


