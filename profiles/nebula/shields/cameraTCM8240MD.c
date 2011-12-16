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

PYGMYI2CPORT pygmyCameraPort;
PYGMYFILE pygmyCameraFile;
volatile u16 globalCameraHCOUNT, globalCameraVCOUNT;
volatile u16 globalCameraX, globalCameraY;

void cameraInit( void )
{
    PYGMY_RCC_GPIOA_ENABLE;
    PYGMY_RCC_GPIOB_ENABLE;
    PYGMY_RCC_GPIOC_ENABLE;
    CAMERA_EXTCLK_INIT;
    CAMERA_RST_INIT;
    CAMERA_ENABLE_INIT;
    CAMERA_D_INIT;
    CAMERA_HBLK_INIT;
    CAMERA_VBLK_INIT;
    CAMERA_DCLK_INIT;
    //CAMERA_STROBE_INIT;
    /*pinConfig( PA8, ALT );
    pinConfig( CAMERA_RST, OUT );
    pinConfig( CAMERA_ENABLE, OUT );
    pinConfig( CAMERA_HBLK, IN );
    pinConfig( CAMERA_VBLK, IN );
    pinConfig( CAMERA_DCLK, IN );
    pinConfig( CAMERA_STROBE, IN );
    CAMERA_D_INIT;*/
    CAMERA_RST_HIGH;
    CAMERA_ENABLE_HIGH;
    //delay( 100 );
    mcoEnable( HSI );
    //delay( 100 );
    //CAMERA_RST_LOW;
    //delay( 100 );
    //CAMERA_RST_HIGH;
    //delay( 100 );
    i2cConfig( &pygmyCameraPort, CAMERA_SCL, CAMERA_SDA, 0 );
    //pinInterrupt( cameraHBLK, CAMERA_HBLK, u16 uiMode );
    cameraRegisterWrite( 0x02, SRST );
    //cameraRegisterWrite( 0x03, PLLON );
    cameraRegisterWrite( 0x05, FRM_SPD0 );
    //cameraSetFormat( SUBQCIF );
}

void cameraRegisterWrite( u8 ucReg, u8 ucData )
{
    i2cStart( &pygmyCameraPort );
    i2cWriteByte( &pygmyCameraPort, CAMERA_ADDRESS<<1 );     // write 0x3D
    i2cWriteByte( &pygmyCameraPort, ucReg );	// write register address
    i2cWriteByte( &pygmyCameraPort, ucData );
    i2cStop( &pygmyCameraPort );
}

u8 cameraRegisterRead( u8 ucReg )
{
    u8 ucData;
 
    i2cStart( &pygmyCameraPort );
    i2cWriteByte( &pygmyCameraPort, CAMERA_ADDRESS<<1 );	
    i2cWriteByte( &pygmyCameraPort, ucReg );	// write register address
    i2cStart( &pygmyCameraPort );
    i2cWriteByte( &pygmyCameraPort, (CAMERA_ADDRESS<<1)|1 );
    ucData = i2cReadByte( &pygmyCameraPort ); 
    i2cStop( &pygmyCameraPort ); 
    
    return( ucData ); 
}

void cameraHBLK( void )
{
    
}

void cameraVBLK( void )
{

}

void cameraSetZoom( u8 ucZoom )
{
    cameraRegisterWrite( 0xE0, ucZoom & 0x3F );
}

u16 cameraGetZoom( void )
{

}

void cameraSetFormat( u8 ucFormat )
{
    // FULL     ( PICMODE = 0 ) 1280x960, ( PICMODE = 1 ) 1280x1024 
    // VGA      640x480 
    // QVGA     320x240 
    // QQVGA    160x120 
    // CIF      352x288  
    // QCIF     176x144
    // SUBQCIF  128x96 
   
    u32 ulSPCount, ulHCount, ulVCount;
    
    cameraRegisterWrite( 0x04, DOUTOFF|PICMODE_SXGA|( ucFormat & 0x07 ) );
    if( ucFormat == FULL ){
        cameraRegisterWrite( 0x0E, 0xB0 );
        cameraRegisterWrite( 0x11, 0x6A );
        cameraRegisterWrite( 0x14, 0x33 );
    } else if( ucFormat == QVGA ){
        cameraRegisterWrite( 0x0E, 0xAB );
        cameraRegisterWrite( 0x11, 0x4A );
        cameraRegisterWrite( 0x14, 0x34 );
    } else{
        cameraRegisterWrite( 0x0E, 0xAC );
        cameraRegisterWrite( 0x11, 0x4A );
        cameraRegisterWrite( 0x14, 0x33 );
    } // else 
    ulSPCount = 2 * ( ulHCount + 191 );
    //cameraRegisterWrite( );
    //cameraRegisterWrite( );
}

u8 cameraGetFormat( u8 ucFormat )
{

}

void cameraGetImage( void )
{
    u32 i, ii, ulLen, ulAddress;
    u16 uiData;

    print( COM3, "\rInit camera..." );
    cameraInit();
    print( COM3, "\rBeginning Image Capture..." );

   /* if( ucFormat == FULL ){
        cameraRegisterWrite( 0x0E, 0xB0 );
        cameraRegisterWrite( 0x11, 0x6A );
        cameraRegisterWrite( 0x14, 0x33 );
    } else if( ucFormat == QVGA ){
        cameraRegisterWrite( 0x0E, 0xAB );
        cameraRegisterWrite( 0x11, 0x4A );
        cameraRegisterWrite( 0x14, 0x34 );
    } else{*/
        cameraRegisterWrite( 0x0E, 0xAC );
        //print( COM3, "\r0x0E: %2X", cameraRegisterRead( 0x0E ) );
        cameraRegisterWrite( 0x11, 0x4A );
        cameraRegisterWrite( 0x14, 0x33 );
    //} // else 
    cameraRegisterWrite( 0x1B, (u8)(((u16)713 >> 8)|(539<<4)) );
    cameraRegisterWrite( 0x1A, (u8)713 );
    cameraRegisterWrite( 0x1C, (u8)((u16)539>>2) );
    cameraRegisterWrite( 0x1E, (u8)1808 );
    cameraRegisterWrite( 0x1F, (u8)((u16)1808 >> 8 ) );
    
    ulLen = (128*96)*3;
    if( !fileOpen( &pygmyCameraFile, "camdump", WRITE ) ){
        print( COM3, "\rFile failed to open" );
        return;
    } // if
    
    ulAddress = filePreAllocate( &pygmyCameraFile, ulLen ) - 4096;
    guiOutputBMHeader( &pygmyCameraFile, 128, 96, PYGMY_PBM_24BPP );
    if( ulAddress == 0 ){
        print( COM3, "\rAllocation Error" );
        return;
    } // if
    ulAddress += 54;
    print( COM3, "\rOutput Header" );
    
    cameraRegisterWrite( 0x04, DOUTOFF|SUBQCIF  );
    ulAddress |= 0x02000000;
    FLASH_CS_LOW;
    spiWriteByte( &pygmyFlashSPI, PYGMY_FLASH_WREN ); // WREN unlocks write capability for one operation
    FLASH_CS_HIGH;
    FLASH_CS_LOW;
    //spiWriteByte( &pygmyFlashSPI, 0xAD ); // Austo Increment
    spiWriteLong( &pygmyFlashSPI, 0xAD000000 | ulAddress ); // Write instruction and 3 address bytes
    //while( !CAMERA_HBLK_IN );
    //print( COM3, "\rPixels" );
    do{
        while( !CAMERA_HBLK_IN );
        while( CAMERA_DCLK_IN ); // Wait for clock low to sample
        uiData = (u16)CAMERA_DCLK_IN << 8;
        while( !CAMERA_DCLK_IN );
        uiData |= (u16)CAMERA_DCLK_IN;
        //spiWriteWord( &pygmyFlashSPI, uiData ); // Write data byte
        //spiWriteByte( &pygmyFlashSPI, 0 );
    } while( CAMERA_VBLK_IN );// for
    pygmyCameraFile.Length = ulLen;
    FLASH_CS_HIGH;
    //flashWaitForBusy(); // Busy must be clear before next write cycle may start
    cameraRegisterWrite( 0x04, 0x00 ); // Turn Off Data
    fileClose( &pygmyCameraFile );
    print( COM3, "\rImage Captured: %d Bytes", ii );
}

void cameraGetVideo( void )
{
    //pinInterrupt( cameraHBLK, CAMERA_HBLK, u16 uiMode );
}



