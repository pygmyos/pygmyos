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

#define SHIELD_HOME             PA9 // COM1_TX
#define SHIELD_HOME_INIT        GPIOA->CRH &= ~PIN9_CLEAR; GPIOA->CRH |= PIN9_IN_PULL; GPIOA->BRR = BIT9
#define SHIELD_HOME_GET         ( GPIOA->IDR & BIT9 )
#define SHIELD_UP               PA0 // A0
#define SHIELD_UP_INIT          GPIOA->CRL &= ~PIN0_CLEAR; GPIOA->CRL |= PIN0_IN_PULL; GPIOA->BRR = BIT0
#define SHIELD_UP_GET           ( GPIOA->IDR & BIT0 )
#define SHIELD_DOWN             PA1 // A1
#define SHIELD_DOWN_INIT        GPIOA->CRL &= ~PIN1_CLEAR; GPIOA->CRL |= PIN1_IN_PULL; GPIOA->BRR = BIT1
#define SHIELD_DOWN_GET         ( GPIOA->IDR & BIT1 )
#define SHIELD_CENTER           PC8 // D3
#define SHIELD_CENTER_INIT      GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_IN_PULL; GPIOC->BRR = BIT8
#define SHIELD_CENTER_GET       ( GPIOC->IDR & BIT8 )
#define SHIELD_LEFT             PA7 // TA1
#define SHIELD_LEFT_INIT        GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_PULL; GPIOA->BRR = BIT7
#define SHIELD_LEFT_GET         ( GPIOA->IDR & BIT7 )
#define SHIELD_RIGHT            PA6 // TA0
#define SHIELD_RIGHT_INIT       GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_PULL; GPIOA->BRR = BIT6
#define SHIELD_RIGHT_GET        ( GPIOA->IDR & BIT6 )
#define SHIELD_SHUTDOWN         PA5 // DAC2
#define SHIELD_SHUTDOWN_INIT    GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_OUT50_GPPUSHPULL
#define SHIELD_SHUTDOWN_HIGH    GPIOA->BSRR = BIT5
#define SHIELD_SHUTDOWN_LOW     GPIOA->BRR = BIT5
#define SHIELD_MICPWR           PA3 // COM2_RX
#define SHIELD_MICPWR_INIT      GPIOA->CRL &= ~PIN3_CLEAR; GPIOA->CRL |= PIN3_OUT50_GPPUSHPULL
#define SHIELD_MICPWR_HIGH      GPIOA->BSRR = BIT3
#define SHIELD_MICPWR_LOW       GPIOA->BRR = BIT3
#define SHIELD_MIC              PA2 // COM2_TX
#define SHIELD_MIC_INIT         GPIOA->CRL &= ~PIN2_CLEAR; GPIOA->CRL |= PIN2_IN_ANALOG

#define LCD_RD_INIT             GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_GPPUSHPULL; GPIOB->BSRR = BIT8
#define LCD_RD_HIGH             GPIOB->BSRR = BIT8
#define LCD_RD_LOW              GPIOB->BRR = BIT8
#define LCD_WR_INIT             GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_GPPUSHPULL; GPIOB->BSRR = BIT7
#define LCD_WR_HIGH             GPIOB->BSRR = BIT7
#define LCD_WR_LOW              GPIOB->BRR = BIT7
#define LCD_DATA_RD_INIT        GPIOC->CRL = ALLPINS_IN_FLOAT
#define LCD_DATA_WR_INIT        GPIOC->CRL = ALLPINS_OUT50_GPPUSHPULL
#define LCD_DATA_RD             ( GPIOC->IDR & 0x00FF )
#define LCD_DATA_WR( LCDDAT )   GPIOC->ODR &= 0xFF00; GPIOC->ODR |= LCDDAT
#define LCD_A0_INIT             GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_GPPUSHPULL; GPIOB->BSRR = BIT6
#define LCD_A0_HIGH             GPIOB->BSRR = BIT6
#define LCD_A0_LOW              GPIOB->BRR = BIT6
#define LCD_RESET_INIT          GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= PIN5_OUT50_GPPUSHPULL; GPIOB->BSRR = BIT5
#define LCD_RESET_HIGH          GPIOB->BSRR = BIT5
#define LCD_RESET_LOW           GPIOB->BRR = BIT5
#define LCD_CS_INIT             GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_GPPUSHPULL; GPIOB->BSRR = BIT9
#define LCD_CS_HIGH             GPIOB->BSRR = BIT9
#define LCD_CS_LOW              GPIOB->BRR = BIT9
#define LCD_BL_INIT             GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_OUT50_GPPUSHPULL; GPIOA->BSRR = BIT8
#define LCD_BL_HIGH             GPIOA->BSRR = BIT8
#define LCD_BL_LOW              GPIOA->BRR = BIT8

#define LCD_RD                  PB8 // T2
#define LCD_WR                  PB7 // T1
#define LCD_D0                  PC0 // A2
#define LCD_A0                  PB6 // T0
#define LCD_RESET               PB5 // D0
#define LCD_CS                  PB9 // T3
#define LCD_BL                  PA8 // MCO

#define MIC                     PA2
#define MIC_PWR                 PA3
#define MIC_INIT                GPIOA->CRL &= ~(PIN2_CLEAR|PIN3_CLEAR); GPIOA->CRL |= (PIN2_IN_ANALOG|PIN3_OUT50_GPPUSHPULL )
#define MIC_ENABLE              GPIOA->BSRR = BIT3
#define MIC_DISABLE             GPIOA->BRR = BIT3


