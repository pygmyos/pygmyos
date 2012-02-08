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

#pragma once

#include "pygmy_profile.h"

#define SHIELD_INIT             PYGMY_RCC_GPIOA_ENABLE; PYGMY_RCC_GPIOB_ENABLE; PYGMY_RCC_GPIOC_ENABLE;
#define LCD_RD_INIT             PB8_OUT; PB8_HIGH;
#define LCD_RD_HIGH             PB8_HIGH;
#define LCD_RD_LOW              PB8_LOW;
#define LCD_WR_INIT             PB7_OUT; PB7_HIGH;
#define LCD_WR_HIGH             PB7_HIGH;
#define LCD_WR_LOW              PB7_LOW;
#define LCD_DATA_RD_INIT        GPIOC->CRL = ALLPINS_IN_FLOAT
#define LCD_DATA_WR_INIT        GPIOC->CRL = ALLPINS_OUT50_GPPUSHPULL
#define LCD_DATA_RD             ( GPIOC->IDR & 0x00FF )
#define LCD_DATA_WR( LCDDAT )   GPIOC->ODR &= 0xFF00; GPIOC->ODR |= LCDDAT
#define LCD_A0_INIT             PB6_OUT; PB6_HIGH;
#define LCD_A0_HIGH             PB6_HIGH;
#define LCD_A0_LOW              PB6_LOW;
#define LCD_RESET_INIT          PB5_OUT; PB5_HIGH;
#define LCD_RESET_HIGH          PB5_HIGH;
#define LCD_RESET_LOW           PB5_LOW;
#define LCD_CS_INIT             PB9_OUT; PB9_HIGH;
#define LCD_CS_HIGH             PB9_HIGH;
#define LCD_CS_LOW              PB9_LOW;
#define LCD_BL_INIT             PA7_OUT; PA7_HIGH;
#define LCD_BL_HIGH             PA7_HIGH;
#define LCD_BL_LOW              PA7_LOW;

#define LCD_RD                  PB8 // T2
#define LCD_WR                  PB7 // T1
#define LCD_D0                  PC0 // A2
#define LCD_A0                  PB6 // T0
#define LCD_RESET               PB5 // D0
#define LCD_CS                  PB9 // T3
#define LCD_BL                  PA7 // TA1

#define SHIELD_SCL              PA9 // TX1
#define SHIELD_SDA              PA10// RX1
#define SHIELD_DOWN             D3  // PC8    
#define SHIELD_UP               PA0 // A0
#define SHIELD_RIGHT            TA0 // PA4
#define SHIELD_LEFT             DAC1// PA6
#define SHIELD_CENTER           PA5 // DAC2
#define SHIELD_BUTTON1          PA2 // TX2
#define SHIELD_BUTTON2          PA3 // RX2

