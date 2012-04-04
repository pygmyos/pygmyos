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

#define MPL115A2_ADDRESS            0x60 // I2C address, must be left shifted for bit0 read/write
#define MPL115A2_PADC_MSB           0x00
#define MPL115A2_PADC_LSB           0x01
#define MPL115A2_TADC_MSB           0x02
#define MPL115A2_TADC_LSB           0x03
#define MPL115A2_A0_MSB             0x04
#define MPL115A2_A0_LSB             0x05
#define MPL115A2_B1_MSB             0x06
#define MPL115A2_B1_LSB             0x07
#define MPL115A2_B2_MSB             0x08
#define MPL115A2_B2_LSB             0x09
#define MPL115A2_C12_MSB            0x0A
#define MPL115A2_C12_LSB            0x0B
#define MPL115A2_C11_MSB            0x0C
#define MPL115A2_C11_LSB            0x0D
#define MPL115A2_C22_MSB            0x0E
#define MPL115A2_C22_LSB            0x0F

#define MPL115A2_START              0x12
#define MPL115A2_START_TEMP         0x11
#define MPL115A2_START_PRESSURE     0x10

u8 mpl115a2Init( u8 ucSCL, u8 ucSDA, u8 ucSHUTDOWN, u8 ucRES );
u8 mpl115a2IsEnabled( void );
void mpl115a2Enable( void );
void mpl115a2Disable( void );
void mpl115a2Reset( void );
u8 mpl115a2GetChar( u8 ucAddress );
u8 mpl115a2PutChar( u8 ucAddress, u8 ucChar );
float mpl115a2ReadkPa( void );
float mpl115a2ReadTemp( void );
