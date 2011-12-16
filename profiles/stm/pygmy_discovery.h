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

#pragma once

#define PYGMY_XTAL          8000000
// end Board Defines for Pygmy OS 

// Internal Pin Defines
//#define BAT_LVL1            PA6
//#define BAT_LVL2            PA7
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