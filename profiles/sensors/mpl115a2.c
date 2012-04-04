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
#include "profiles/sensors/mpl115a2.h"

PYGMYI2CPORT globalI2CPressure;
volatile u8 globalPressureSHUTDOWN, globalPressureRES;

u8 mpl115a2Init( u8 ucSCL, u8 ucSDA, u8 ucSHUTDOWN, u8 ucRES )
{
    if( ucSCL == NONE || ucSDA == NONE ){
        return( FALSE );
    } // if
    globalPressureSHUTDOWN = ucSHUTDOWN;
    globalPressureRES = ucRES;
    i2cConfig( &globalI2CPressure, ucSCL, ucSDA, MPL115A2_ADDRESS, 0 );
    mpl115a2Enable();
    mpl115a2Reset();

    return( TRUE );
}

u8 mpl115a2IsEnabled( void )
{
    if( globalPressureSHUTDOWN != NONE ){
        return( pinGet( globalPressureSHUTDOWN ) );
    } // if

    return( TRUE ); // If shutdown pin isn't used, the sensor is always active
}

void mpl115a2Enable( void )
{
    if( globalPressureSHUTDOWN != NONE ){
        pinSet( globalPressureSHUTDOWN, HIGH );
        delay( 60 );
    } // if
}

void mpl115a2Disable( void )
{
    if( globalPressureSHUTDOWN != NONE ){
        pinSet( globalPressureSHUTDOWN, LOW );
    } // if
}

void mpl115a2Reset( void )
{
    if( globalPressureRES != NONE ){
        pinSet( globalPressureRES, LOW );
        delay( 60 );
        pinSet( globalPressureRES, HIGH );
    } // if
}   

u8 mpl115a2GetChar( u8 ucAddress )
{
    u8 i, ucByte;

    PYGMY_WATCHDOG_REFRESH;
    i2cStart( &globalI2CPressure );
    i2cWriteByte( &globalI2CPressure, globalI2CPressure.Address );
    i2cWriteByte( &globalI2CPressure, ucAddress );
    i2cStart( &globalI2CPressure );
    i2cWriteByte( &globalI2CPressure, globalI2CPressure.Address | 1 ); // Read     
    for( i = 0, ucByte = 0; i < 8; i++ ){
        ucByte = ( ucByte << 1 ) | i2cReadBit( &globalI2CPressure );
    } // for
    i2cWriteBit( &globalI2CPressure, 1 ); // High NACK to end sequence
    i2cStop( &globalI2CPressure );

    return( ucByte );
}

u8 mpl115a2PutChar( u8 ucAddress, u8 ucChar )
{
    u8 i, ucRetry, ucAck;

    PYGMY_WATCHDOG_REFRESH;
    i2cStart( &globalI2CPressure );
    i2cWriteByte( &globalI2CPressure, globalI2CPressure.Address );
    i2cWriteByte( &globalI2CPressure, ucAddress );
    i2cWriteByte( &globalI2CPressure, ucChar );
    i2cStop( &globalI2CPressure );
    
    return( TRUE );
}

float mpl115a2ReadTemp( void )
{
    float fADCTemp;
    u16 uiADCTemp;

    if( !mpl115a2IsEnabled(  ) ){
        mpl115a2Enable();
    } // if
    mpl115a2PutChar( MPL115A2_START, 0 ); // Start Conversions
    delayms( 1 );
    uiADCTemp = ( ( mpl115a2GetChar( 2 ) << 8 ) | mpl115a2GetChar( 3 ) ) >> 6;
    //print( COM3, "\rADCTemp: %d", uiADCTemp );
    fADCTemp = uiADCTemp;
    //print( COM3, "\rADCTemp: %f", fADCTemp );
    fADCTemp = 25 + ( ( fADCTemp - 498.0) / -5.35 );
    //print( COM3, "\rTemp: %f", fADCTemp );

    return( fADCTemp );
}

float mpl115a2ReadkPa( void )
{
    // conversion formula and sample code provided by Freescale
    static s16 mplCR = 0, sia0, sib1, sib2, sic12;
    u16 uiPadc, uiTadc;
    s16 siPcomp;
	s32 lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
	s32 si_a1, si_a2, si_a1x1, si_y1, si_a2x2;

    mpl115a2PutChar( MPL115A2_START, 0 ); // Start Conversions
    delayms( 1 );
    if( !mplCR ){ 
        mplCR = 1;
        sia0 = ( mpl115a2GetChar( MPL115A2_A0_MSB ) << 8 ) | mpl115a2GetChar( MPL115A2_A0_LSB );
        sib1 = ( mpl115a2GetChar( MPL115A2_B1_MSB ) << 8 ) | mpl115a2GetChar( MPL115A2_B1_LSB );
        sib2 = ( mpl115a2GetChar( MPL115A2_B2_MSB ) << 8 ) | mpl115a2GetChar( MPL115A2_B2_LSB );
        sic12 = ( mpl115a2GetChar( MPL115A2_C12_MSB ) << 8 ) | mpl115a2GetChar( MPL115A2_C12_LSB ); 
    } // if
    uiPadc = ( ( mpl115a2GetChar( 0 ) << 8 ) | mpl115a2GetChar( 1 ) ) >> 6;
    uiTadc = ( ( mpl115a2GetChar( 2 ) << 8 ) | mpl115a2GetChar( 3 ) ) >> 6;

	si_c11x1 = 0;
	si_a11 = (s32)(((s32)sib1<<14)>>14);
	si_c12x2 = (s32)sic12*uiTadc;
	si_a1 = (s32)(((s32)si_a11<<11) + si_c12x2 ) >> 11;
	si_a2 = ((s32)((s32)sib2<<15) >> 16);
	si_a1x1 = (s32)( si_a1 * uiPadc );
	si_y1 = ((s32)(((s32)sia0<<10) + (s32)si_a1x1 )>>10);
	si_a2x2 = (s32)( si_a2 * uiTadc );//(lt3);

	siPcomp = ((s32)(((s32)si_y1<<10) + si_a2x2)>>13);

	// Raw range 1024, pressure range from 50kPa to 115kPa
	return (((65.0/1023.0)*(float)siPcomp)+50);
}

