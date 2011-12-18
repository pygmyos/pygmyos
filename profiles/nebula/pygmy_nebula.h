/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011  Warren D Greenway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

// Pygmy Nebula Revision 2 and above
#ifndef __PYGMY_NEBULA_R1
#define LED0                PA11
    #define LED0_INIT       GPIOA->CRH &= ~PIN11_CLEAR; GPIOA->CRH |= PIN11_OUT50_GPPUSHPULL               
    #define LED0_HIGH       GPIOA->BSRR = BIT11
    #define LED0_LOW        GPIOA->BRR = BIT11
#define LED1                PA12
    #define LED1_INIT       GPIOA->CRH &= ~PIN12_CLEAR; GPIOA->CRH |= PIN12_OUT50_GPPUSHPULL
    #define LED1_HIGH       GPIOA->BSRR = BIT12
    #define LED1_LOW        GPIOA->BRR = BIT12
#define BAT1                PD2
    #define BAT1_INIT       GPIOD->CRH &= ~PIN2_CLEAR; GPIOD->CRL |= PIN2_IN_PULL; GPIOD->BSRR = BIT2
    #define BAT1_IN         ( GPIOD->IDR & BIT2 )
#define BAT2                PC13
    #define BAT2_INIT       GPIOC->CRH &= ~PIN13_CLEAR; GPIOC->CRH |= PIN13_IN_PULL; GPIOC->BRR = BIT13
    #define BAT2_IN         ( GPIOC->IDR & BIT13 )
#define TEMP                PB0
    #define TEMP_INIT       GPIOB->CRL &= ~PIN0_CLEAR; GPIOB->CRL |= PIN0_IN_ANALOG
#define FLASH_CS            PB12
    #define FLASH_CS_INIT   GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_OUT50_GPPUSHPULL
    #define FLASH_CS_HIGH   GPIOB->BSRR = BIT12
    #define FLASH_CS_LOW    GPIOB->BRR = BIT12
#define FLASH_SCK           PB13
    #define FLASH_SCK_INIT  GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_OUT50_GPPUSHPULL
    #define FLASH_SCK_HIGH  GPIOB->BSRR = BIT13
    #define FLASH_SCK_LOW   GPIOB->BRR = BIT13
#define FLASH_MISO          PB14
    #define FLASH_MISO_INIT GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_FLOAT
    #define FLASH_MISO_IN   ( GPIOB->IDR & BIT14 )
    #define FLASH_MISO_STATE FLASH_MISO_IN
#define FLASH_MOSI          PB15
    #define FLASH_MOSI_INIT GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_OUT50_GPPUSHPULL
    #define FLASH_MOSI_HIGH GPIOB->BSRR = BIT15
    #define FLASH_MOSI_LOW  GPIOB->BRR = BIT15
#define RF_IRQ              PC9
    #define RF_IRQ_INIT     GPIOC->CRH &= ~PIN9_CLEAR; GPIOC->CRH |= PIN9_IN_PULL; GPIOC->BSRR = BIT9
    #define RF_IRQ_IN       ( GPIOC->IDR & BIT9 )
    #define RF_IRQ_STATE    RF_IRQ_IN
#define RF_CS               PC11
#define RF_CS_INIT          GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_GPPUSHPULL
#define RF_CS_HIGH          GPIOC->BSRR = BIT11
#define RF_CS_LOW           GPIOC->BRR = BIT11
#define RF_EN               PC10
#define RF_EN_INIT          GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_OUT50_GPPUSHPULL
#define RF_EN_HIGH          GPIOC->BSRR = BIT10
#define RF_EN_LOW           GPIOC->BRR = BIT10
#define RF_SCK              PC12
    #define RF_SCK_INIT     GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_OUT50_GPPUSHPULL
    #define RF_SCK_HIGH     GPIOC->BSRR = BIT12
    #define RF_SCK_LOW      GPIOC->BRR = BIT12
#define RF_MOSI             PB2
    #define RF_MOSI_INIT    GPIOB->CRL &= ~PIN2_CLEAR; GPIOB->CRL |= PIN2_OUT50_GPPUSHPULL
    #define RF_MOSI_HIGH    GPIOB->BSRR = BIT2
    #define RF_MOSI_LOW     GPIOB->BRR = BIT2
#define RF_MISO             PB1
    #define RF_MISO_INIT    GPIOB->CRL &= ~PIN1_CLEAR; GPIOB->CRL |= PIN1_IN_FLOAT
    #define RF_MISO_IN      ( GPIOB->IDR & BIT1 )
    #define RF_MISO_STATE   RF_MISO_IN
#define T0                  PB6
    #define T0_PORT         GPIOB
    #define T0_PIN          BIT6
    #define T0_ALT          GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_ALTPUSHPULL;
    #define T0_OUT          GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_OUT50_GPPUSHPULL;
    #define T0_FLOAT        GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_FLOAT;
    #define T0_PULLUP       GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_PULL; GPIOB->BSRR = BIT6;
    #define T0_PULLDOWN     GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_PULL; GPIOB->BRR = BIT6;
    #define T0_ANALOG       GPIOB->CRL &= ~PIN6_CLEAR; GPIOB->CRL |= PIN6_IN_ANALOG;
    #define T0_GET          ( GPIOB->IDR & BIT6 )
    #define T0_HIGH         GPIOB->BSRR = PIN6;
    #define T0_LOW          GPIOB->BRR = PIN6;
    //#define pinPullupT0( )
#define T1                  PB7
    #define T1_PORT         GPIOB
    #define T1_PIN          BIT7
    #define T1_ALT          GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_ALTPUSHPULL;
    #define T1_OUT          GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_OUT50_GPPUSHPULL;
    #define T1_FLOAT        GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_FLOAT;          
    #define T1_PULLUP       GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_PULL; GPIOB->BSRR = BIT7;
    #define T1_PULLDOWN     GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_PULL; GPIOB->BRR = BIT7;
    #define T1_ANALOG       GPIOB->CRL &= ~PIN7_CLEAR; GPIOB->CRL |= PIN7_IN_ANALOG;
    #define T1_GET          ( GPIOB->IDR & BIT7 )
    #define T1_HIGH         GPIOB->BSRR = BIT7;
    #define T1_LOW          GPIOB->BRR = BIT7;
#define T2                  PB8
    #define T2_PORT         GPIOB
    #define T2_PIN          BIT8
    #define T2_ALT          GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_ALTPUSHPULL;
    #define T2_OUT          GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_OUT50_GPPUSHPULL;
    #define T2_FLOAT        GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_FLOAT;
    #define T2_PULLUP       GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_PULL; GPIOB->BSRR = BIT8;
    #define T2_PULLDOWN     GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_PULL; GPIOB->BRR = BIT8;
    #define T2_ANALOG       GPIOB->CRH &= ~PIN8_CLEAR; GPIOB->CRH |= PIN8_IN_ANALOG;
    #define T2_GET          ( GPIOB->IDR & BIT8 )
    #define T2_HIGH         GPIOB->BSRR = BIT8;
    #define T2_LOW          GPIOB->BRR = BIT8;
#define T3                  PB9
    #define T3_PORT         GPIOB
    #define T3_PIN          BIT9
    #define T3_ALT          GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_ALTPUSHPULL;
    #define T3_OUT          GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_OUT50_GPPUSHPULL;
    #define T3_FLOAT        GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_FLOAT;
    #define T3_PULLUP       GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_PULL; GPIOB->BSRR = BIT9;
    #define T3_PULLDOWN     GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_PULL; GPIOB->BRR = BIT9;
    #define T3_ANALOG       GPIOB->CRH &= ~PIN9_CLEAR; GPIOB->CRH |= PIN9_IN_ANALOG;
    #define T3_GET          ( GPIOB->IDR & BIT9 )
    #define T3_HIGH         GPIOB->BSRR = BIT9;
    #define T3_LOW          GPIOB->BRR = BIT9;
#define TA0                 PA6
    #define TA0_PORT        GPIOA
    #define TA0_PIN         BIT6
    #define TA0_ALT         GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_OUT50_ALTPUSHPULL;
    #define TA0_OUT         GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_OUT50_GPPUSHPULL;
    #define TA0_FLOAT       GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_FLOAT;
    #define TA0_PULLUP      GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_PULL; GPIOA->BSRR = BIT6;
    #define TA0_PULLDOWN    GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_PULL; GPIOA->BRR = BIT6;
    #define TA0_ANALOG      GPIOA->CRL &= ~PIN6_CLEAR; GPIOA->CRL |= PIN6_IN_ANALOG;
    #define TA0_GET         ( GPIOA->IDR & BIT6 )
    #define TA0_HIGH        GPIOA->BSRR = BIT6;
    #define TA0_LOW         GPIOA->BRR = BIT6;
#define TA1                 PA7
    #define TA1_PORT        GPIOA
    #define TA1_PIN         BIT7
    #define TA1_ALT         GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_OUT50_ALTPUSHPULL;
    #define TA1_OUT         GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_OUT50_GPPUSHPULL;
    #define TA1_FLOAT       GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_FLOAT;
    #define TA1_PULLUP      GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_PULL; GPIOA->BSRR = BIT7;
    #define TA1_PULLDOWN    GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_PULL; GPIOA->BRR = BIT7;
    #define TA1_ANALOG      GPIOA->CRL &= ~PIN7_CLEAR; GPIOA->CRL |= PIN7_IN_ANALOG;
    #define TA1_GET         ( GPIOA->IDR & BIT7 )
    #define TA1_HIGH        GPIOA->BSRR = BIT7;
    #define TA1_LOW         GPIOA->BRR = BIT7;
#define D0                  PB5
    #define D0_PORT         GPIOB
    #define D0_PIN          BIT5
    #define D0_ALT          GPIOB->CRL &= ~PIN5_CLEAR; GPIOB->CRL |= 
#define D1                  PC6
#define D2                  PC7
#define D3                  PC8
#define A0                  PA0
#define A1                  PA1
#define A2                  PC0
#define A3                  PC1
#define A4                  PC2
#define A5                  PC3
#define A6                  PC4
#define A7                  PC5
#define DAC1                PA4
#define DAC1_INIT           GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_OUT50_ALTPUSHPULL
#define DAC2                PA5
#define DAC2_INIT           GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_OUT50_ALTPUSHPULL
#define COM1_TX             PA9
#define COM1_RX             PA10
#define COM2_TX             PA2
#define COM2_RX             PA3
#define COM3_TX             PB10
#define COM3_RX             PB11

#define PYGMY_XTAL          ((u32)12000000) // XTAL frequency in Hz
#define PYGMY_HSI           ((u32)8000000) // This is a generic value for default internal frequency
#endif

// Prototype revision of Pygmy Nebula
#ifdef __PYGMY_NEBULA_R1
  
#define LED0                PA11
#define LED1                PA12
#define BAT_CHARGE          PD2
#define BAT_POWERGOOD       PC13
#define BAT_LVL1            PB0
#define BAT_LVL2            PB1
#define FLASH_CS            PB12
#define SRAM_CS             PC12
#define SPI_SCK             PB13
#define SPI_MOSI            PB15
#define SPI_MISO            PB14
#define RF_IRQ              PC9
#define RF_CS               PC11
#define RF_CS_INIT          GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_GPPUSHPULL
#define RF_CS_HIGH          GPIOC->BSRR = BIT11
#define RF_CS_LOW           GPIOC->BRR = BIT11
#define RF_EN               PC10
#define RF_EN_INIT          GPIOC->CRH &= ~PIN10_CLEAR; GPIOC->CRH |= PIN10_OUT50_GPPUSHPULL
#define RF_EN_HIGH          GPIOC->BSRR = BIT10
#define RF_EN_LOW           GPIOC->BRR = BIT10
#define RF_SCK              SPI_SCK
#define RF_MOSI             SPI_MOSI
#define RF_MISO             SPI_MISO
#define T0                  PB6
#define T1                  PB7
#define T2                  PB8
#define T3                  PB9
#define TA0                 PA6
#define TA1                 PA7
#define D0                  PB5
#define D1                  PC6
#define D2                  PC7
#define D3                  PC8
#define A0                  PA0
#define A1                  PA1
#define A2                  PC0
#define A3                  PC1
#define A4                  PC2
#define A5                  PC3
#define A6                  PC4
#define A7                  PC5
#define DAC1                PA4
#define DAC1_INIT           GPIOA->CRL &= ~PIN4_CLEAR; GPIOA->CRL |= PIN4_OUT50_ALTPUSHPULL
#define DAC2                PA5
#define DAC2_INIT           GPIOA->CRL &= ~PIN5_CLEAR; GPIOA->CRL |= PIN5_OUT50_ALTPUSHPULL
#define COM1_TX             PA9
#define COM1_RX             PA10
#define COM2_TX             PA2
#define COM2_RX             PA3
#define COM3_TX             PB10
#define COM3_RX             PB11
//#define FLASH_CS            PB12
#define FLASH_SCK           SPI_SCK
#define FLASH_MISO          SPI_MISO
#define FLASH_MOSI          SPI_MOSI

#ifdef __PYGMY_BOOT
    #define SPI_SCK_INIT    GPIOB->CRH &= ~PIN13_CLEAR; GPIOB->CRH |= PIN13_OUT50_GPPUSHPULL  
    #define SPI_SCK_HIGH    GPIOB->ODR |= BIT13    
    #define SPI_SCK_LOW     GPIOB->ODR &= ~BIT13 
    
    #define SPI_MISO_INIT   GPIOB->CRH &= ~PIN14_CLEAR; GPIOB->CRH |= PIN14_IN_PULL; GPIOB->ODR |= BIT14
    #define SPI_MISO_STATE  ( GPIOB->IDR & BIT14 )
    
    #define SPI_MOSI_INIT   GPIOB->CRH &= ~PIN15_CLEAR; GPIOB->CRH |= PIN15_OUT50_GPPUSHPULL    
    #define SPI_MOSI_HIGH   GPIOB->ODR |= BIT15    
    #define SPI_MOSI_LOW    GPIOB->ODR &= ~BIT15    

    #define FLASH_CS_INIT   GPIOB->CRH &= ~PIN12_CLEAR; GPIOB->CRH |= PIN12_OUT50_GPPUSHPULL
    #define FLASH_CS_HIGH   GPIOB->ODR |= BIT12
    #define FLASH_CS_LOW    GPIOB->ODR &= ~BIT12
    
    #define RF24_CS_INIT    GPIOC->CRH &= ~PIN11_CLEAR; GPIOC->CRH |= PIN11_OUT50_GPPUSHPULL; GPIOC->ODR |= BIT11
    
    #define SRAM_CS_INIT    GPIOC->CRH &= ~PIN12_CLEAR; GPIOC->CRH |= PIN12_OUT50_GPPUSHPULL; GPIOC->ODR |= BIT12
#endif

#define PYGMY_XTAL          ((u32)12000000) // XTAL frequency in Hz
#define PYGMY_HSI           ((u32)8000000) // This is a generic value for default internal frequency
#endif
