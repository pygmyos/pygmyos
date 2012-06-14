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


//#pragma once
#ifndef __PYGMY_HEADER_NVIC
	#define __PYGMY_HEADER_NVIC

#include "pygmy_sys.h"
// Pygmy NVIC Header
// Includes NVIC, System Control and EXTI related defines
// This file is for STM32 Family MCUs

typedef struct
{
  volatile u32 ISER[2];
  u32 RESERVED0[30];
  volatile u32 ICER[2];
  u32 RESERVED1[30];
  volatile u32 ISPR[2];
  u32 RESERVED2[30];
  volatile u32 ICPR[2];
  u32 RESERVED3[30];
  volatile u32 IABR[2];
  u32 RESERVED4[62];
  volatile u32 IPR[11];
} NVIC_TYPEDEF;

typedef struct
{
  volatile u32 EVCR;
  volatile u32 MAPR;
  volatile u32 EXTICR[4];
} AFIO_TYPEDEF;

typedef struct
{
  volatile u32 IMR;
  volatile u32 EMR;
  volatile u32 RTSR;
  volatile u32 FTSR;
  volatile u32 SWIER;
  volatile u32 PR;
} EXTI_TYPEDEF;

typedef struct
{
  volatile u32 CTRL;
  volatile u32 LOAD;
  volatile u32 VAL;
  volatile const u32 CALIB;
} SYSTICK_TYPEDEF;

typedef struct
{
  volatile const u32 CPUID;
  volatile u32 ICSR;
  volatile u32 VTOR;
  volatile u32 AIRCR;
  volatile u32 SCR;
  volatile u32 CCR;
  volatile u32 SHPR[3];
  volatile u32 SHCSR;
  volatile u32 CFSR;
  volatile u32 HFSR;
  volatile u32 DFSR;
  volatile u32 MMFAR;
  volatile u32 BFAR;
  volatile u32 AFSR;
} SCB_TYPEDEF;

//#define SRAM_BASE             ((u32)0x20000000)
//#define PERIPH_BASE           ((u32)0x40000000)
//#define PERIPH_BB_BASE        ((u32)0x42000000) // Alias
//#define SRAM_BB_BASE          ((u32)0x22000000) // Alias
//#define FLASH_BASE            ((u32)0x40022000) // Flash Registers Starting Address
//#define OB_BASE               ((u32)0x1FFFF800) // Flash Option Registers Starting Address
//#define APB1PERIPH_BASE       PERIPH_BASE
//#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
//#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
//#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
//#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)

#ifndef AFIO_BASE
    #define AFIO_BASE             ( ( (u32)0x40000000 ) + 0x10000 )
#endif
#ifndef EXTI_BASE
    #define EXTI_BASE             ( ( (u32)0x40000000 ) + 0x10400 )
#endif

#define AIRCR_VECTKEY_MASK          ((u32)0x05FA0000)

// Naming retains compatibility with ST Reference Material
// A notable exception being the L15X Low Power series which have a selection
// of specially allocated vectors marked L15X_IRQ instead of IRQ
// All other Vectors are compatible through F10X, F2 and F4 series
// Note that not all vectors are used in all MCUs, refer to Reference manual
// for specific part number for more information.
#define WWDG_IRQ                    ((u8)0x00)  // ( 0 ) Window WatchDog 
#define PVD_IRQ                     ((u8)0x01)  // ( 1 )  PVD through EXTI Line detection
#define TAMPER_IRQ                  ((u8)0x02)  // ( 2 )  Tamper Pin
#define RTC_IRQ                     ((u8)0x03)  // ( 3 )  RTC wakeup through EXTI
#define FLASH_IRQ                   ((u8)0x04)  // ( 4 )  FLASH General
#define RCC_IRQ                     ((u8)0x05)  // ( 5 )  RCC General
#define EXTI0_IRQ                   ((u8)0x06)  // ( 6 )  EXTI Line0  
#define EXTI1_IRQ                   ((u8)0x07)  // ( 7 )  EXTI Line1 
#define EXTI2_IRQ                   ((u8)0x08)  // ( 8 )  EXTI Line2  
#define EXTI3_IRQ                   ((u8)0x09)  // ( 9 )  EXTI Line3
#define EXTI4_IRQ                   ((u8)0x0A)  // ( 10 ) EXTI Line4 
#define DMA1_CH1_IRQ                ((u8)0x0B)  // ( 11 ) DMA1 Channel1 
#define DMA1_CH2_IRQ                ((u8)0x0C)  // ( 12 ) DMA1 Channel2 
#define DMA1_CH3_IRQ                ((u8)0x0D)  // ( 13 ) DMA1 Channel3 
#define DMA1_CH4_IRQ                ((u8)0x0E)  // ( 14 ) DMA1 Channel4 
#define DMA1_CH5_IRQ                ((u8)0x0F)  // ( 15 ) DMA1 Channel5 
#define DMA1_CH6_IRQ                ((u8)0x10)  // ( 16 ) DMA1 Channel6 
#define DMA1_CH7_IRQ                ((u8)0x11)  // ( 17 ) DMA1 Channel7 
// Some of the Vectors following have aliases to account for varying allocations across MCUs 
#define ADC_IRQ                     ((u8)0x12)  // ( 18 ) ADC1 && ( ADC2, ADC3 )
#define USB_HP_CAN_TX_IRQ           ((u8)0x13)  // ( 19 ) USB High Priority or CAN TX 
    #define USB_HP_IRQ              ((u8)0x13)  // ( 19 ) USB High Priority
    #define CAN_TX_IRQ              ((u8)0x13)  // ( 19 ) CAN TX
#define USB_LP_CAN_RX0_IRQ          ((u8)0x14)  // ( 20 ) USB Low Priority or CAN RX0 
    #define USB_LP_IRQ              ((u8)0x14)  // ( 20 ) USB Low Priority
    #define CAN_RX0_IRQ             ((u8)0x14)  // ( 20 ) CAN RX0
#define CAN_RX1_IRQ                 ((u8)0x15)  // ( 21 ) CAN RX1
    #define DAC_L15X_IRQ            ((u8)0x15)  // ( 21 ) DAC Underrun ( STM32L15X )
#define CAN_SCE_IRQ                 ((u8)0x16)  // ( 22 ) CAN SCE 
    #define COMP_L15X_IRQ           ((u8)0x16)  // ( 23 ) Comparator EXTI ( STM32L15X )
#define EXTI9_5_IRQ                 ((u8)0x17)  // ( 23 ) External Line[9:5]
#define TIM1_BRK_IRQ                ((u8)0x18)  // ( 24 ) Timer1 Break 
    #define TIM9_IRQ                ((u8)0x18)  // ( 24 ) Timer9 General  ( F103XL )
    #define TIM15_IRQ               ((u8)0x18)  // ( 24 ) Timer15 General ( F100 )
    #define LCD_L15X_IRQ            ((u8)0x18)  // ( 24 ) LCD ( STM32L15X )
#define TIM1_UP_IRQ                 ((u8)0x19)  // ( 25 ) Timer1 Update
    #define TIM10_IRQ               ((u8)0x19)  // ( 25 ) Timer10 General ( F103XL )
    #define TIM16_IRQ               ((u8)0x19)  // ( 25 ) Timer16 General ( F100 )
    #define TIM9_L15X_IRQ           ((u8)0x19)  // ( 25 ) Timer9 General ( STM32L15X )
#define TIM1_TRG_COM_IRQ            ((u8)0x1A)  // ( 26 ) Timer1 Trigger and Commutation
    #define TIM11_IRQ               ((u8)0x1A)  // ( 26 ) Timer11 Global ( F103XL )
    #define TIM10_L15X_IRQ          ((u8)0x1A)  // ( 26 ) Timer10 General ( STM32L15X )
#define TIM1_CC_IRQ                 ((u8)0x1B)  // ( 27 ) Timer1 Capture Compare
    #define TIM17_IRQ               ((u8)0x1B)  // ( 27 ) Timer17 General ( F100 )
    #define TIM11_L15X_IRQ          ((u8)0x1B)  // ( 27 ) Timer11 General ( STM32L15X )
#define TIM2_IRQ                    ((u8)0x1C)  // ( 28 ) Timer2 General
#define TIM3_IRQ                    ((u8)0x1D)  // ( 29 ) Timer3 General
#define TIM4_IRQ                    ((u8)0x1E)  // ( 30 ) Timer4 General
#define I2C1_EV_IRQ                 ((u8)0x1F)  // ( 31 ) I2C1 Event
#define I2C1_ER_IRQ                 ((u8)0x20)  // ( 32 ) I2C1 Error
#define I2C2_EV_IRQ                 ((u8)0x21)  // ( 33 ) I2C2 Event
#define I2C2_ER_IRQ                 ((u8)0x22)  // ( 34 ) I2C2 Error
#define SPI1_IRQ                    ((u8)0x23)  // ( 35 ) SPI1 General
#define SPI2_IRQ                    ((u8)0x24)  // ( 36 ) SPI2 General
#define USART1_IRQ                  ((u8)0x25)  // ( 37 ) USART1 General
#define USART2_IRQ                  ((u8)0x26)  // ( 38 ) USART2 General
#define USART3_IRQ                  ((u8)0x27)  // ( 39 ) USART3 General
#define EXTI15_10_IRQ               ((u8)0x28)  // ( 40 ) External Line[15:10]
#define RTC_ALARM_IRQ               ((u8)0x29)  // ( 41 ) RTC Alarm through EXTI Line
#define USB_WAKEUP_IRQ              ((u8)0x2A)  // ( 42 ) USB WakeUp from suspend through EXTI Line
    #define CEC_IRQ                 ((u8)0x2A)  // ( 42 ) HDMI CEC
    #define OTG_FS_WKUP_IRQ         ((u8)0x2A)  // ( 42 ) USB OTG FS EXTI Wakeup
    #define USB_FS_WKUP_L15X_IRQ    ((u8)0x2A)  // ( 42 ) USB FS WakeUp ( STM32L15X )
#define TIM8_BRK_IRQ                ((u8)0x2B)  // ( 43 ) Timer8 Break
    #define TIM12_IRQ               ((u8)0x2B)  // ( 43 ) Timer12 General
    #define TIM6_L15X_IRQ           ((u8)0x2B)  // ( 43 ) Timer6 General ( STM32L15X )
#define TIM8_UP_IRQ                 ((u8)0x2C)  // ( 44 ) Timer8 Up
    #define TIM13_IRQ               ((u8)0x2C)  // ( 44 ) Timer13 General
    #define TIM7_L15X_IRQ           ((u8)0x2C)  // ( 44 ) Timer7 General ( STM32L15X )
#define TIM8_TRG_COM_IRQ            ((u8)0x2D)  // ( 45 ) Timer8 Trigger
    #define TIM14_IRQ               ((u8)0x2D)  // ( 45 ) Timer14 General
#define TIM8_CC_IRQ                 ((u8)0x2E)  // ( 46 ) Timer8 Capture Compare
#define ADC3_IRQ                    ((u8)0x2F)  // ( 47 ) ADC3 General
    #define DMA1_CH8_IRQ            ((u8)0x2F)  // ( 47 ) DMA1 Channel8 
#define FSMC_IRQ                    ((u8)0x30)  // ( 48 ) FSMC General
#define SDIO_IRQ                    ((u8)0x31)  // ( 49 ) SDIO General
#define TIM5_IRQ                    ((u8)0x32)  // ( 50 ) Timer5 General
#define SPI3_IRQ                    ((u8)0x33)  // ( 51 ) SPI3 General
#define UART4_IRQ                   ((u8)0x34)  // ( 52 ) UART4 General
#define UART5_IRQ                   ((u8)0x35)  // ( 53 ) UART5 General
#define TIM6_IRQ                    ((u8)0x36)  // ( 54 ) Timer6 General
    #define DAC_IRQ                 ((u8)0x36)  // ( 54 ) DAC Underrun
#define TIM7_IRQ                    ((u8)0x37)  // ( 55 ) Timer7 General
#define DMA2_CH1_IRQ                ((u8)0x38)  // ( 56 ) DMA2 Channel1
#define DMA2_CH2_IRQ                ((u8)0x39)  // ( 57 ) DMA2 Channel2
#define DMA2_CH3_IRQ                ((u8)0x3A)  // ( 58 ) DMA2 Channel3 
#define DMA2_CH4_5_IRQ              ((u8)0x3B)  // ( 59 ) DMA2 Channel4 and Channel5
#define DMA2_CH5_IRQ                ((u8)0x3C)  // ( 60 ) DMA2 Channel5 ( if MISC_REMAP set in AFIO_MAPR2 )
#define ETH_IRQ                     ((u8)0x3D)  // ( 61 ) Ethernet General
#define ETH_WKUP_IRQ                ((u8)0x3E)  // ( 62 ) Ethernet Wakeup
#define CAN2_TX                     ((u8)0x3F)  // ( 63 ) CAN2 TX
#define CAN2_RX0_IRQ                ((u8)0x40)  // ( 64 ) CAN2 RX0
#define CAN2_RX1_IRQ                ((u8)0x41)  // ( 65 ) CAN2 RX1
#define CAN2_SCE_IRQ                ((u8)0x42)  // ( 66 ) CAN2 SCE
#define OTG_FS_IRQ                  ((u8)0x43)  // ( 67 ) USB OTG FS 
#define DMA2_CH6_IRQ                ((u8)0x44)  // ( 68 ) DMA2 Channel6 
#define DMA2_CH7_IRQ                ((u8)0x45)  // ( 69 ) DMA2 Channel7
#define DMA2_CH8_IRQ                ((u8)0x46)  // ( 70 ) DMA2 Channel8
#define USART6_IRQ                  ((u8)0x47)  // ( 71 ) USART6 General
#define I2C3_EV_IRQ                 ((u8)0x48)  // ( 72 ) I2C3 Event Interrupt
#define I2C3_ER_IRQ                 ((u8)0x49)  // ( 73 ) I2C3 Error Interrupt
#define OTG_HS_EP1_OUT_IRQ          ((u8)0x4A)  // ( 74 ) USB OTG HS EndPoint 1 Out
#define OTG_HS_EP1_IN_IRQ           ((u8)0x4B)  // ( 75 ) USB OTG HS EndPoint 1 In
#define OTG_HS_WKUP_IRQ             ((u8)0x4C)  // ( 76 ) USB OTG HS WakeUp
#define OTG_HS_IRQ                  ((u8)0x4D)  // ( 77 ) USB OTG HS 
#define DCMI_IRQ                    ((u8)0x4E)  // ( 78 ) DCMI ( Camera Bus )
#define CRYP_IRQ                    ((u8)0x4F)  // ( 79 ) Cryptographic Cell
#define HASH_IRQ                    ((u8)0x50)  // ( 80 ) Hash and Ring
// The following are intentionally defined out-of-range for special handling
#define MEMFAULT_IRQ                ((u8)0xF0)  // System Register Memory Fault
#define BUSFAULT_IRQ                ((u8)0xF1)  // System Register Bus Fault
#define USAGEFAULT_IRQ              ((u8)0xF2)  // System Register Usage Fault
#define SVCALL_IRQ                  ((u8)0xF3)  // System Register SV Call Interrupt
#define PENDSVCALL_IRQ              ((u8)0xF4)  // System Register Pending SV Interrupt
#define SYSTICK_IRQ                 ((u8)0xF5)  // System Register SysTick Interrupt

#define NVIC_VECTORTABLE_RAM        ((u32)0x20000000)
#define NVIC_VECTORTABLE_FLASH      ((u32)0x08000000)
#define NVIC_LP_SEVONPEND           ((u8)0x10)
#define NVIC_LP_SLEEPDEEP           ((u8)0x04)
#define NVIC_LP_SLEEPONEXIT         ((u8)0x02)
/* Deprecated
#define NVIC_PRIORITY0             ((u32)0x700) // 0 bits for pre-emption priority 4 bits for subpriority 
#define NVIC_PRIORITY1              ((u32)0x600) // 1 bits for pre-emption priority 3 bits for subpriority 
#define NVIC_PRIORITY2              ((u32)0x500) // 2 bits for pre-emption priority 2 bits for subpriority 
#define NVIC_PRIORITY3              ((u32)0x400) // 3 bits for pre-emption priority 1 bits for subpriority 
#define NVIC_PRIORITY4              ((u32)0x300) // 4 bits for pre-emption priority 0 bits for subpriority 
*/

#define EXTI_0                  ((u32)0x00001)  // External interrupt line 0 
#define EXTI_1                  ((u32)0x00002)  // External interrupt line 1 
#define EXTI_2                  ((u32)0x00004)  // External interrupt line 2 
#define EXTI_3                  ((u32)0x00008)  // External interrupt line 3 
#define EXTI_4                  ((u32)0x00010)  // External interrupt line 4 
#define EXTI_5                  ((u32)0x00020)  // External interrupt line 5 
#define EXTI_6                  ((u32)0x00040)  // External interrupt line 6 
#define EXTI_7                  ((u32)0x00080)  // External interrupt line 7 
#define EXTI_8                  ((u32)0x00100)  // External interrupt line 8 
#define EXTI_9                  ((u32)0x00200)  // External interrupt line 9 
#define EXTI_10                 ((u32)0x00400)  // External interrupt line 10 
#define EXTI_11                 ((u32)0x00800)  // External interrupt line 11 
#define EXTI_12                 ((u32)0x01000)  // External interrupt line 12 
#define EXTI_13                 ((u32)0x02000)  // External interrupt line 13 
#define EXTI_14                 ((u32)0x04000)  // External interrupt line 14 
#define EXTI_15                 ((u32)0x08000)  // External interrupt line 15 
#define EXTI_16                 ((u32)0x10000)  // External interrupt line 16
                                                    // Connected to the PVD Output
#define EXTI_17                 ((u32)0x20000)  // External interrupt line 17 
                                                    // Connected to the RTC Alarm event 
#define EXTI_18                 ((u32)0x40000)  // External interrupt line 18 
/* Deprecated                                                   
#define EXTI_GPIOA                  0x00
#define EXTI_GPIOB                  0x01
#define EXTI_GPIOC                  0x02
#define EXTI_GPIOD                  0x03
#define EXTI_GPIOE                  0x04
#define EXTI_GPIOF                  0x05
#define EXTI_GPIOG                  0x06
#define EXTI_FALLING                0x01
#define EXTI_RISING                 0x02
#define EXTI_WAKE                   0x04
*/      

#define TRIGGER_FALLING             0x01
#define TRIGGER_RISING              0x02
#define TRIGGER_WAKE                0x04

#define SCS_BASE                    ((u32)0xE000E000)         // System Control
#define SYSTICK_BASE                (SCS_BASE + 0x0010)
#define NVIC_BASE                   (SCS_BASE + 0x0100)
#define SCB_BASE                    (SCS_BASE + 0x0D00)       // System Control
#define SYSTICK                     ((SYSTICK_TYPEDEF *) SYSTICK_BASE)
#define NVIC                        ((NVIC_TYPEDEF *) NVIC_BASE)
#define SCB                         ((SCB_TYPEDEF *) SCB_BASE)  
#ifndef AFIO
    #define AFIO                ((AFIO_TYPEDEF *) AFIO_BASE)
#endif
#ifndef EXTI
    #define EXTI                ((EXTI_TYPEDEF *) EXTI_BASE)
#endif
#define PRIORITY0        0x00    // Reserved for Pygmy System
#define PRIORITY1        0x10    // Highest for normal use, 15 is lowest
#define PRIORITY2        0x20    // Always use the lowest feasible to avoid
#define PRIORITY3        0x30    // conflicts with service related interrupts
#define PRIORITY4        0x40
#define PRIORITY5        0x50
#define PRIORITY6        0x60
#define PRIORITY7        0x70
#define PRIORITY8        0x80
#define PRIORITY9        0x90
#define PRIORITY10       0xA0
#define PRIORITY11       0xB0
#define PRIORITY12       0xC0
#define PRIORITY13       0xD0
#define PRIORITY14       0xE0
#define PRIORITY15       0xF0

// The following clears pending bits to prevent recursive access
// must be used at beginning of interrupt handler
#define PYGMY_EXTICLEAR         EXTI->PR = 0x00000000; EXTI->SWIER = 0x00000000
#ifndef PYGMY_RESET
    #define PYGMY_RESET             SCB->AIRCR = (AIRCR_VECTKEY_MASK | BIT2);
#endif

extern const u8 PYGMY_TIMERVECTORS[];
extern const u8 PYGMY_TIMERVECTORS_L15X[];

u8 interruptGetTimerVector( u8 ucTimer );
void extiConfig( u8 ucPin, u16 uiMode );
void interruptEnable( u32 ulVector );
void interruptDisable( u32 ulVector );
void interruptSetPriority( u32 ulVector, u8 ucPriority );

#endif // __PYGMY_HEADER_NVIC

