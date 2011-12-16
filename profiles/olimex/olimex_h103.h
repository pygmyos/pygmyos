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

// This file provides hardware/pin mapping for the Olimex STM32-H103 for use with Pygmy OS:
//  Version 1.X
// The STM32-H103 uses the STM32F103 series Cortex M3 ARM MCUs

#pragma once

// DAC switches pin and requires mode to be set to IN_FLOAT before enabling DAC 

#ifndef __PYGMY_BOOT

#endif

// Internal Pin Defines
#define USB_DM              PA11 
#define USB_DP              PA12
#define USB_DISC            PD2
#define USB_P               PC4
// End Internal Pin Defines

// External Pin Defines
#define EXT1_1              PA11    // USB_DM
    #define COM1_CTS        PA11
    #define USART1_CTS      PA11
    #define TIM1_CH4        PA11
    #define CAN_RX          PA11
#define EXT1_2              PA8
    #define MCO             PA8
    #define TIM1_CH1        PA8
#define EXT1_3              PA12    // USB_DP
    #define COM1_RTS        PA12
    #define USART1_RTS      PA12
    #define CAN_TX          PA12
#define EXT1_4              PA9
    #define COM1_TX         PA9
    #define USART1_TX       PA9
    #define TIM1_CH2        PA9
#define EXT1_7              PA10
    #define COM1_RX         PA10
    #define USART1_RX       PA10
    #define TIM1_CH3        PA10
#define EXT1_8              PC10
#define EXT1_9              PC11
#define EXT1_10             PC12
    #define LED             PC12
#define EXT1_12             PB5
#define EXT1_13             PB6
    #define TIM4_CH1        PB6
#define EXT1_14             PA6
    #define TIM3_CH1        PA6
    #define ADC6            PA6
#define EXT1_15             PB7
    #define TIM4_CH2        PB7
#define EXT1_16             PB8
    #define TIM4_CH3        PB8
#define EXT1_18             PA5
    #define ADC5            PA5
#define EXT1_19             PC0
    #define ADC10           PC0
#define EXT1_20             PC1
    #define ADC11           PC1
#define EXT1_21             PB0
    #define TIM3_CH3        PB0
    #define ADC8            PB0
#define EXT1_22             PA7
    #define TIM3_CH2        PA7
    #define ADC7            PA7
#define EXT1_24             PC13
    #define TAMPER          PC13
#define EXT1_26             PB1
    #define TIM3_CH4        PB1
    #define ADC9            PB1
#define EXT2_2              PC2
    #define ADC12           PC2
#define EXT2_4              PA0
    #define WAKEUP          PA0
    #define BUTTON          PA0
    #define BUT             PA0
    #define ADC0            PA0
    #define COM2_CTS        PA0
    #define USART2_CTS      PA0
#define EXT2_7              PA2
    #define COM2_TX         PA2
    #define USART2_TX       PA2
    #define TIM2_CH3        PA2
    #define ADC2            PA2
#define EXT2_8              PA1
    #define COM2_RTS        PA1
    #define USART2_RTS      PA1
    #define TIM2_CH2        PA1
    #define ADC1            PA1
#define EXT2_9              PC3
    #define ADC13           PC3
#define EXT2_10             PA3
    #define COM2_RX         PA3
    #define USART2_RX       PA3
    #define TIM2_CH4        PA3
    #define ADC3            PA3
#define EXT2_11             PA4
    #define ADC4            PA4
#define EXT2_12             PC4
    #define ADC14           PC4
#define EXT2_13             PC5
    #define ADC15           PC5
#define EXT2_14             PB10
#define EXT2_15             PB11
#define EXT2_16             PB13
#define EXT2_17             PB12
#define EXT2_18             PB14
#define EXT2_19             PB15
#define EXT2_20             PC6
#define EXT2_21             PC7
#define EXT2_22             PC8
#define EXT2_24             PC9
// End External Pin Defines