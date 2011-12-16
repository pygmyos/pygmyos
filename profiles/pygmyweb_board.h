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

#include "profiles/wireless/RF24.h"

#define PA0_INIT            GPIOA->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_OUT50_GPPUSHPULL
#define PA0_HIGH            GPIOA->ODR |= BIT0
#define PA0_LOW             GPIOA->ODR &= ~BIT0
#define PA1_INIT            GPIOA->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_OUT50_GPPUSHPULL
#define PA1_HIGH            GPIOA->ODR |= BIT1
#define PA1_LOW             GPIOA->ODR &= ~BIT1
#define PA2_INIT            GPIOA->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_OUT50_GPPUSHPULL
#define PA2_HIGH            GPIOA->ODR |= BIT2
#define PA2_LOW             GPIOA->ODR &= ~BIT2
#define PA3_INIT            GPIOA->CRL &= ~PIN3_CLEAR; GPIOB->CRL |= PIN3_OUT50_GPPUSHPULL
#define PA3_HIGH            GPIOA->ODR |= BIT3
#define PA3_LOW             GPIOA->ODR &= ~BIT3

#define TIM1_CH1_INIT       GPIOA->CRH &= ~PIN8_CLEAR; GPIOA->CRH |= PIN8_OUT50_ALTPUSHPULL
#define TIM3_CH1_INIT       GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_OUT50_ALTPUSHPULL
#define TIM3_CH2_INIT       GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_OUT50_ALTPUSHPULL
#define TIM3_CH3_INIT       GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_OUT50_ALTPUSHPULL
#define TIM3_CH4_INIT       GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_OUT50_ALTPUSHPULL
#define TIM4_CH1_INIT       GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_ALTPUSHPULL
#define TIM4_CH2_INIT       GPIOB->CRH &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_ALTPUSHPULL
#define TIM4_CH3_INIT       GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_ALTPUSHPULL
#define TIM4_CH4_INIT       GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_ALTPUSHPULL

#define ADC1_IN15_INIT      GPIOC->CRL &= ~PIN5_CLEAR; GPIOC->CRL |= PIN5_IN_ANALOG
#define ADC1_IN14_INIT      GPIOC->CRL &= ~PIN4_CLEAR; GPIOC->CRL |= PIN4_IN_ANALOG
#define ADC1_IN13_INIT      GPIOC->CRL &= ~PIN3_CLEAR; GPIOC->CRL |= PIN3_IN_ANALOG
#define ADC1_IN12_INIT      GPIOC->CRL &= ~PIN2_CLEAR; GPIOC->CRL |= PIN2_IN_ANALOG
#define ADC1_IN11_INIT      GPIOC->CRL &= ~PIN1_CLEAR; GPIOC->CRL |= PIN1_IN_ANALOG
#define ADC1_IN10_INIT      GPIOC->CRL &= ~PIN0_CLEAR; GPIOC->CRL |= PIN0_IN_ANALOG

#define RF24_IRQ_INIT       GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= GPIOB->CRL |= PIN5_IN_PULL; GPIOB->ODR |= BIT5
#define RF24_IRQ_STATE      ( GPIOB->IDR & BIT5 )
#define RF24_EN_INIT        GPIOC->CRL &= ~PIN7_CLEAR; GPIOC->CRL |= PIN7_OUT50_GPPUSHPULL
#define RF24_EN_HIGH        GPIOC->ODR |= BIT7
#define RF24_EN_LOW         GPIOC->ODR &= ~BIT7
#define RF24_CS_INIT        GPIOC->CRL &= ~PIN6_CLEAR; GPIOC->CRL |= PIN6_OUT50_GPPUSHPULL
#define RF24_CS_HIGH        GPIOC->ODR |= BIT6
#define RF24_CS_LOW         GPIOC->ODR &= ~BIT6
#define RF24_SCK_INIT       GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_OUT50_GPPUSHPULL
#define RF24_SCK_HIGH       GPIOB->ODR |= BIT13
#define RF24_SCK_LOW        GPIOB->ODR &= ~BIT13
#define RF24_MISO_INIT      GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_PULL
#define RF24_MISO_STATE     ( GPIOB->IDR & BIT14 )
#define RF24_MOSI_INIT      GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_OUT50_GPPUSHPULL
#define RF24_MOSI_HIGH      GPIOB->ODR |= BIT15
#define RF24_MOSI_LOW       GPIOB->ODR &= ~BIT15
#define RF24_INIT           RF24_IRQ_INIT;RF24_EN_INIT;RF24_CS_INIT;RF24_SCK_INIT;RF24_MISO_INIT;RF24_MOSI_INIT

// DAC switches pin and requires mode to be set to IN_FLOAT before enabling DAC 
#define DAC1_INIT           GPIOA->CRL &= ~PIN4_CLEAR//; GPIOA->CRL |= PIN5_IN_ANALOG
#define DAC2_INIT           GPIOA->CRL &= ~PIN5_CLEAR//; GPIOA->CRL |= PIN5_IN_ANALOG

#define BAT_PG_INIT         GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_IN_PULL; GPIOB->ODR |= BIT12
#define BAT_PG_STATE        ( GPIOB->IDR & BIT12 )

#define BAT_CHG_INIT        GPIOD->CRL &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_PULL; GPIOD->ODR |= BIT2
#define BAT_CHG_STATE       ( GPIOD->IDR & BIT2 )

#ifndef __PYGMY_BOOT
#define FLASH_CS_INIT       GPIOC->CRH &= ~PIN8_CLEAR; GPIOC->CRH |= PIN8_OUT50_GPPUSHPULL
#define FLASH_CS_HIGH       GPIOC->ODR |= BIT8
#define FLASH_CS_LOW        GPIOC->ODR &= ~BIT8
#endif

#define SRAM_CS_INIT        GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_OUT50_GPPUSHPULL
#define SRAM_CS_HIGH        GPIOC->CRH |= BIT12
#define SRAM_CS_LOW         GPIOC->CRH &= ~BIT12

#define SPI_CS1_INIT        GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_OUT50_GPPUSHPULL
#define SPI_CS1_HIGH        GPIOC->CRH |= BIT13
#define SPI_CS1_LOW         GPIOC->CRH &= ~BIT13

#define SPI_SCK_INIT        GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_GPPUSHPULL
#define SPI_SCK_HIGH        GPIOC->ODR |= BIT11
#define SPI_SCK_LOW         GPIOC->ODR &= ~BIT11
#define SPI_MOSI_INIT       GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_OUT50_GPPUSHPULL
#define SPI_MOSI_HIGH       GPIOC->ODR |= BIT9
#define SPI_MOSI_LOW        GPIOC->ODR &= ~BIT9
#define SPI_MISO_INIT       GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_IN_PULL; GPIOB->ODR |= BIT10
#define SPI_MISO_STATE      ( GPIOC->IDR & BIT10 ) 

#define BAT_CHARGE          PD2
#define BAT_POWERGOOD       PB12
#define FLASH_CS            PC8
#define SRAM_CS             PC12
#define SPI_CS1             PC13
#define SPI_SCK             PC11
#define SPI_MOSI            PC9
#define SPI_MISO            PC10
#define RF24_IRQ            PB5
#define RF24_CS             PC6
#define RF24_SCK            PB13
#define RF24_MOSI           PB15
#define RF24_MISO           PB14
#define TIM1_CH1            PA8
#define TIM3_CH1            PA6
#define TIM3_CH2            PA7
#define TIM3_CH3            PB0
#define TIM3_CH4            PB1
#define TIM4_CH1            PB6
#define TIM4_CH2            PB7
#define TIM4_CH3            PB8
#define TIM4_CH4            PB9
#define ADC1_IN10           PC0
#define ADC1_IN11           PC1
#define ADC1_IN12           PC2
#define ADC1_IN13           PC3
#define ADC1_IN14           PC4
#define ADC1_IN15           PC5
#define DAC1                PA4
#define DAC2                PA5
#define USART1_TX           PA9
#define USART1_RX           PA10
#define USART1_CTS          PA11
#define USART1_RTS          PA12

#define FLASH_CS            PC8
#define FLASH_SCK           SPI_SCK
#define FLASH_MISO          SPI_MISO
#define FLASH_MOSI          SPI_MOSI

#define HSE_Value                 ((u32)12000000) // XTAL frequency in Hz
#define HSI_Value                 ((u32)8000000) // This is a generic value for default internal frequency

