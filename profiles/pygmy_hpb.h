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

// This file provides hardware/pin mapping for the Pygmy HighPerformance Boards:
//  Version 1.X
// Pygmy HighPerformance Boards in this series use the STM32F205V series Cortex M3 ARM MCUs

#pragma once

#include "profiles/wireless/RF24.h"

// DAC switches pin and requires mode to be set to IN_FLOAT before enabling DAC 

#ifndef __PYGMY_BOOT

#endif

// Internal Pin Defines
#define USB_DM              PA11  
#define USB_DP              PA12
#define USB_DISC            PC13
#define RF24_IRQ            PD3
#define RF24_CS             PD1
#define RF24_CE             PD0
#define FLASH_CS            PD11
#define SPI_MISO            PD7
#define SPI_MOSI            PD10
#define SPI_SCK             PD4
#define SDIO_D0             PC9
#define SDIO_D1             PC8
#define SDIO_D2             PC11
#define SDIO_D3             PE0
#define SDIO_CMD            PD2
#define SDIO_CK             PC12
#define SDIO_DETECT         PB13
// End Internal Pin Defines

// External Pin Defines
#define MCO                 PA8
// !!! Note: HPB V1.4 ( proto release ) has COM3_TX swapped with COM1_TX in copper
#define COM1_TX             PA9
#define COM1_RX             PA10
#define COM2_TX             PA2
#define COM2_RX             PA3
#define COM3_TX             PB10
#define COM3_RX             PB11
#define LED0                PC5
#define LED1                PC4
#define DAC1                PA4
#define DAC2                PA5
#define TA11                PC0
#define TA10                PC1
#define TA9                 PC2
#define TA8                 PC3
#define TA7                 PA0
    #define WAKEUP          PA0
#define TA6                 PA1
#define TA5                 PA2
#define TA4                 PA3
#define TA3                 PA6
#define TA2                 PA7
#define TA1                 PB0
#define TA0                 PB1
#define T14                 PB11
#define T13                 PB10
#define T12                 PB14
#define T11                 PB15
#define T10                 PD12
#define T9                  PD13
#define T8                  PD14
#define T7                  PD15
#define T6                  PC6
#define T5                  PC7
#define T4                  PB5
#define T3                  PB6
#define T2                  PB7
#define T1                  PB8
#define T0                  PB9
#define D16                 PE7
#define D15                 PE8
#define D14                 PE9
#define D13                 PE10
#define D12                 PE11
#define D11                 PE12
#define D10                 PE13
#define D9                  PE14
#define D8                  PE15
#define D7                  PB12
#define D6                  PE6
#define D5                  PE5
#define D4                  PE4
#define D3                  PE3
#define D2                  PE2
#define D1                  PE1
#define D0                  PC10

// External Pin Defines
