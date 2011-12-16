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

// This file provides hardware/pin mapping for the Pygmy LowPower Boards:
//  Version 1.X
// Pygmy LowPower Boards in this series use the STM32L152 series Cortex M3 ARM MCUs

#pragma once

#include "profiles/wireless/RF24.h"

// DAC switches pin and requires mode to be set to IN_FLOAT before enabling DAC 

#ifndef __PYGMY_BOOT

#endif

// Board defines for Pygmy OS
#define PYGMY_XTAL          12000000
// end Board Defines for Pygmy OS 

// Internal Pin Defines
#define BAT_LVL1            PA6
#define BAT_LVL2            PA7
#define BAT_CHG             PC8 
#define BAT_PG              PC9
#define USB_DM              PA11 
#define USB_DP              PA12
#define USB_DISC            PD2
#define RF24_IRQ            PB5
#define RF24_CS             PB6
#define RF24_CE             PB7
#define FLASH_CS            PB8         
#define SRAM_CS             PB9
#define SPI_MISO            PC10
    #define FLASH_MISO      SPI_MISO
#define SPI_MOSI            PC11
    #define FLASH_MOSI      SPI_MOSI
#define SPI_SCK             PC12
    #define FLASH_SCK       SPI_SCK
// End Internal Pin Defines

// External Pin Defines
#define JTRST               PB4
#define JTDI                PA15            
#define JTMS                PA13
#define JTCK                PA14
#define JTDO                PB3
#define MCO                 PA8
#define COM1_TX             PA9
#define COM1_RX             PA10
#define COM2_TX             PA2
#define COM2_RX             PA3
#define COM3_TX             PB10
#define COM3_RX             PB11
#define LED0                PC13
#define DAC1                PA4
#define DAC2                PA5
#define A6                  PC0
#define A5                  PC1
#define A4                  PC2
#define A3                  PC3
#define A2                  PA0
    #define WAKEUP          PA0
#define A1                  PC4
#define A0                  PC5
#define TA6                 PA1
#define TA5                 PB0
#define TA4                 PB1
#define TA3                 PB12
#define TA2                 PB13
#define TA1                 PB14
#define TA0                 PB15
#define T1                  PC6  
#define T0                  PC7
// End External Pin Defines
