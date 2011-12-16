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

// This file is for the Nebula Shield based on the Toshiba TCM8240MD 1.3MP CMOS 

#pragma once

#define MMC_CLK          TX1 // PA9
#define MMC_CLK_INIT     GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_OUT50_GPPUSHPULL
#define MMC_CLK_HIGH     GPIOA->BSRR = BIT9
#define MMC_CLD_LOW      GPIOA->BRR = BIT9
#define MMC_INSERT       RX1 // PA10
#define MMC_INSERT_INIT  GPIOA->CRH &= ~PIN10_CLEAR; GPIOA->CRH |= PIN10_IN_PULL; GPIOA->BSRR = BIT10
#define MMC_DAT0         T0  // PB6
#define MMC_DAT0_INIT    
#define MMC_DAT1         T1  // PB7
#define MMC_DAT2         T2  // PB8
#define MMC_DAT3         T3  // PB9
#define MMC_CMD          TX2 // PA2
#define MMC_CMD_INIT     GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_OUT50_GPPUSHPULL
#define MMC_CMD_HIGH     GPIOA->BSRR = BIT2
#define MMC_CMD_LOW      GPIOA->BRR = BIT2

#define CAMERA_EXTCLK       MCO // PA8
#define CAMERA_EXTCLK_INIT  GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_OUT50_ALTPUSHPULL
#define CAMERA_EXTCLK_HIGH  GPIOA->BSRR = BIT8
#define CAMERA_EXTCLK_LOW   GPIOA->BRR = BIT8
#define CAMERA_RST          A0  // PA0
#define CAMERA_RST_INIT     GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_OUT50_GPPUSHPULL
#define CAMERA_RST_HIGH     GPIOA->BSRR = BIT0
#define CAMERA_RST_LOW      GPIOA->BRR = BIT0
#define CAMERA_ENABLE       A1  // PA1
#define CAMERA_ENABLE_INIT  GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_OUT50_GPPUSHPULL
#define CAMERA_ENABLE_HIGH  GPIOA->BSRR = BIT1
#define CAMERA_ENABLE_LOW   GPIOA->BRR = BIT1
#define CAMERA_D0           A2  // PC0
#define CAMERA_D1           A3  // PC1
#define CAMERA_D2           A4  // PC2
#define CAMERA_D3           A5  // PC3
#define CAMERA_D4           A6  // PC4
#define CAMERA_D5           A7  // PC5
#define CAMERA_D6           D1  // PC6
#define CAMERA_D7           D2  // PC7
#define CAMERA_D_INIT       GPIOC->CRL = ALLPINS_IN_FLOAT
#define CAMERA_D_IN         ( GPIOC->IDR & 0x00FF )
#define CAMERA_HBLK         PC8 // D3
#define CAMERA_HBLK_INIT    GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_PULL; GPIOC->BSRR = BIT8
#define CAMERA_HBLK_IN      ( GPIOC->IDR & BIT8 )
#define CAMERA_VBLK         PB5 // D0
#define CAMERA_VBLK_INIT    GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_IN_FLOAT
#define CAMERA_VBLK_IN      ( GPIOB->IDR & BIT5 )
#define CAMERA_SCL          DAC1// PA4   
#define CAMERA_SCL_INIT     GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_OUT50_GPPUSHPULL
#define CAMERA_SDA          DAC2// PA5
#define CAMERA_SDA_INIT_IN  GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_FLOAT
#define CAMERA_SDA_INIT_OUT GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_IN_FLOAT
#define CAMERA_DCLK         TA0 // PA6
#define CAMERA_DCLK_INIT    GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_FLOAT // GPIOA->BSRR = BIT6
#define CAMERA_DCLK_IN      ( GPIOA->IDR & BIT6 )
#define CAMERA_STROBE       TA1 // PA7
#define CAMERA_STROBE_INIT  GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_FLOAT
#define CAMERA_STROBE_IN    ( GPIOA->CRL & BIT7 )

#define CAMERA_ADDRESS      0x3D
// Register 0x02
#define STANDBYSW           BIT7
#define SRST                BIT6
// Register 0x03
#define DCLKPOL             BIT4
#define PLLON               BIT0
// Register 0x04
#define DOUTOFF             BIT7
#define JPEGON              BIT6
#define PICMODE_4VGA        0
#define PICMODE_SXGA        BIT4
#define SELRGB              BIT3
#define PICSIZ_FULL         0
#define PICSIZ_VGA          1
#define PICSIZ_QVGA         2
#define PICSIZ_QQVGA        3
#define PICSIZ_CIF          4
#define PICSIZ_QCIF         5
#define PICSIZ_SUBQCIF      6
// Register 0x05
#define FRM_SPD0            0
#define FRM_SPD1            BIT6
#define FRM_SPD2            BIT7
// Register 0x06
#define UDINV               BIT7
// Register 0x0D
#define VSUPCNT0            0
#define VSUPCNT1            BIT2
#define VSUPCNT2            BIT3
#define VSUPCNT3            ( BIT3|BIT2 )
// Register 0x22
#define STSET_SW            BIT7
#define STSET_POL           BIT6
#define STSET_REG           BIT3
#define ST_MODE0            0
#define ST_MODE1            BIT0
#define ST_MODE2            BIT1
#define ST_MODE3            ( BIT1|BIT0 )
#define ST_MODE4            BIT2
#define ST_MODE5            ( BIT2|BIT0 )
#define ST_MODE6            ( BIT2|BIT1 )
#define ST_MODE7            ( BIT2|BIT1|BIT0 )
// Register 0x23
#define STOUT_POL           BIT3
#define STOUT_W0            0
#define STOUT_W1            BIT1
#define STOUT_W2            BIT2
#define STOUT_W3            ( BIT2|BIT1 )
#define ST_OUTSIG           BIT0
// Register 0x2E
#define DINSW               BIT7
// Register 0x31
#define APCSW               BIT7
// Register 0x43
#define LSCSW               BIT7
// Register 0x93
#define CCB_SW              BIT7
// Register 0x9D
#define LPFMODE             BIT7
// Register 0x9F
#define CLR_ANRSW           BIT7
// Register 0xA9
#define ABB_SW              BIT7
// Register 0xC0
#define HDCVHSW             BIT7
// Register 0xC3
#define HDCHSW              BIT7
// Register 0xD3
#define NEPO                BIT7
#define Y_MATSW             BIT5
#define DA_MODE             BIT4
// Register 0xD8
#define SEPHIA              BIT7
// Register 0xFA
#define FULL_ERRN           BIT7
#define ENC_ERRN            BIT6


enum { 
        FULL, VGA, QVGA, QQVGA, CIF, QCIF, SUBQCIF, JPEG 
};

   

void cameraInit( void );
void cameraRegisterWrite( u8 ucReg, u8 ucData );
void cameraHBLK( void );
void cameraVBLK( void );
void cameraSetZoom( u8 ucZoom );
//u16 cameraGetZoom( void );
void cameraSetFormat( u8 ucFormat );
u8 cameraGetFormat( u8 ucFormat );
void cameraGetImage( void );
void cameraGetVideo( void );

