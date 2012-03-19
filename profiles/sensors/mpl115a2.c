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

#include "pygmy_profile.h";
#include "profiles/sensors/mpl115a2.h";

PYGMYI2CPORT globalI2CPressure;
volatile u8 globalPressureSHUTDOWN, globalPressureRES;

u8 pressureInit( u8 ucSCL, u8 ucSDA, u8 ucSHUTDOWN, u8 ucRES )
{
    if( ucSCL == NONE || ucSDA == NONE ){
        return( FALSE );
    } // if
    globalPressureSHUTDOWN = ucSHUTDOWN;
    globalPressureRES = ucRES;
    i2cConfig( &globalI2CPressure, ucSCL, ucSDA, MPL115A2_ADDRESS, 0 );
    pressureEnable();
    pressureReset();

    return( TRUE );
}

void pressureEnable( void )
{
    if( globalPressureSHUTDOWN != NONE ){
        pinSet( globalPressureSHUTDOWN, HIGH );
        delay( 60 );
    } // if
}

void pressureDisable( void )
{
    if( globalPressureSHUTDOWN != NONE ){
        pinSet( globalPressureSHUTDOWN, LOW );
    } // if
}

void pressureReset( void )
{
    if( globalPressureRES != NONE ){
        pinSet( globalPressureRES, LOW );
        delay( 60 );
        pinSet( globalPressureRES, HIGH );
    } // if
}   

float pressureReadkPa( void )
{
    static float a0, b1, b2, c12;
    static u8 cr = 0;
    s16 sA0, sB1, sB2, sC12, iPCOMP;
    s16 iPADC, iTADC;
    u8 i;
    u8 ucBuffer[ 8 ];

    //a0 Signed, Integer Bits = 12, Fractional Bits = 3 : Coeff a0 = S I11 I10 I9 I8 I7 I6 I5 I4 I3 I2 I1 I0 . F2 F1 F0
    //b1 Signed, Integer Bits = 2, Fractional Bits = 7 : Coeff b1 = S I1 I0 . F12 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0
    //b2 Signed, Integer Bits = 1, Fractional Bits = 14 : Coeff b2 = S I0 . F13 F12 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0
    //c12 Signed, Integer Bits = 0, Fractional Bits = 13, dec pt zero pad = 9 : Coeff c12 = S 0 . 000 000 000 F12 F10 F9 F8 F7 F6 F5 F4 F3 F2 F1 F0
    pressureEnable();
    i2cWriteBuffer( &globalI2CPressure, MPL115A2_START, ucBuffer, 0 ); // Start Conversions
    delay( 100 );
    if( cr == 0 ){
        cr = 1;
        i2cReadBuffer( &globalI2CPressure, MPL115A2_A0_MSB, ucBuffer, 8 );
        print( COM3, "\rPressure Coefficient Registers:\r" );
        for( i = 0; i < 12; i++ ){
            print( COM3, " 0x%02X", ucBuffer[ i ] );
        } // for
        sA0 = ( ucBuffer[ 0 ] << 8 ) | ucBuffer[ 1 ];
        sB1 = ( ucBuffer[ 2 ] << 8 ) | ucBuffer[ 3 ];
        sB2 = ( ucBuffer[ 4 ] << 8 ) | ucBuffer[ 5 ];
        sC12 = ( ( ucBuffer[ 6 ] << 8 ) | ucBuffer[ 7 ] ) >> 2; 
        a0 = (float)sA0 / 8;
        b1 = (float)sB1 / 8192;
        b2 = (float)sB2 / 16384;
        c12 = (float)sC12 / 8192;
    } // 
    i2cReadBuffer( &globalI2CPressure, 0, ucBuffer, 4 );
    print( COM3, "\rPressure ADC Registers:\r" );
    for( i = 0; i < 4; i++ ){
        print( COM3, " 0x%02X", ucBuffer[ i ] );
    } // for
    iPADC = ( ( ucBuffer[ 0 ] << 8 ) | ucBuffer[ 1 ] ) >> 6;
    iTADC = ( ( ucBuffer[ 3 ] << 8 ) | ucBuffer[ 4 ] ) >> 6;
    // Pcomp = a0 + (b1 + c12 · Tadc) · Padc + b2 · Tadc
    //uiPCOMP = mpl115a2CalculateCompensation( uiPADC, uiTADC, 
    iPCOMP = a0 + (b1 + c12 * iTADC ) * iPADC + b2 * iTADC;
    
    return( ((65.0/1023.0)*(float)iPCOMP)+50 );
}

s16 mpl115a2CalculateCompensation( u16 uiPadc, u16 uiTadc, s32 sic11 )
{
   
    
    // kPa // decPcomp = ((65.0/1023.0)*siPcomp)+50;

}
