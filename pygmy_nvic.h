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


#pragma once

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

// Naming retains compatibility with ST libs
#define WWDG_IRQ                    ((u8)0x00)  // Window WatchDog Interrupt 
#define PVD_IRQ                     ((u8)0x01)  // PVD through EXTI Line detection Interrupt 
#define TAMPER_IRQ                  ((u8)0x02)  // Tamper Interrupt 
#define RTC_IRQ                     ((u8)0x03)  // RTC wakeup through EXTI
#define FLASH_IRQ                   ((u8)0x04)  // FLASH global Interrupt 
#define RCC_IRQ                     ((u8)0x05)  // RCC global Interrupt 
#define EXTI0_IRQ                   ((u8)0x06)  // EXTI Line0 Interrupt 
#define EXTI1_IRQ                   ((u8)0x07)  // EXTI Line1 Interrupt 
#define EXTI2_IRQ                   ((u8)0x08)  // EXTI Line2 Interrupt 
#define EXTI3_IRQ                   ((u8)0x09)  // EXTI Line3 Interrupt 
#define EXTI4_IRQ                   ((u8)0x0A)  // EXTI Line4 Interrupt 
#define DMACHANNEL1_IRQ             ((u8)0x0B)  // DMA Channel 1 global Interrupt 
#define DMACHANNEL2_IRQ             ((u8)0x0C)  // DMA Channel 2 global Interrupt 
#define DMACHANNEL3_IRQ             ((u8)0x0D)  // DMA Channel 3 global Interrupt 
#define DMACHANNEL4_IRQ             ((u8)0x0E)  // DMA Channel 4 global Interrupt 
#define DMACHANNEL5_IRQ             ((u8)0x0F)  // DMA Channel 5 global Interrupt 
#define DMACHANNEL6_IRQ             ((u8)0x10)  // DMA Channel 6 global Interrupt 
#define DMACHANNEL7_IRQ             ((u8)0x11)  // DMA Channel 7 global Interrupt 
#define ADC_IRQ                     ((u8)0x12)  // ADC global Interrupt 
#define USB_HP_CAN_TX_IRQ           ((u8)0x13)  // USB High Priority or CAN TX Interrupts 
#define USB_LP_CAN_RX0_IRQ          ((u8)0x14)  // USB Low Priority or CAN RX0 Interrupts 
#define CAN_RX1_IRQ                 ((u8)0x15)  // CAN RX1 Interrupt 
#define CAN_SCE_IRQ                 ((u8)0x16)  // CAN SCE Interrupt 
#define EXTI9_5_IRQ                 ((u8)0x17)  // External Line[9:5] Interrupts 
#define TIM1_BRK_IRQ                ((u8)0x18)  // TIM1 Break Interrupt 
#define TIM1_UP_IRQ                 ((u8)0x19)  // TIM1 Update Interrupt 
#define TIM1_TRG_COM_IRQ            ((u8)0x1A)  // TIM1 Trigger and Commutation Interrupt 
#define TIM1_CC_IRQ                 ((u8)0x1B)  // TIM1 Capture Compare Interrupt 
#define TIM2_IRQ                    ((u8)0x1C)  // TIM2 global Interrupt 
#define TIM3_IRQ                    ((u8)0x1D)  // TIM3 global Interrupt 
#define TIM4_IRQ                    ((u8)0x1E)  // TIM4 global Interrupt 
#define I2C1_EV_IRQ                 ((u8)0x1F)  // I2C1 Event Interrupt 
#define I2C1_ER_IRQ                 ((u8)0x20)  // I2C1 Error Interrupt 
#define I2C2_EV_IRQ                 ((u8)0x21)  // I2C2 Event Interrupt 
#define I2C2_ER_IRQ                 ((u8)0x22)  // I2C2 Error Interrupt 
#define SPI1_IRQ                    ((u8)0x23)  // SPI1 global Interrupt 
#define SPI2_IRQ                    ((u8)0x24)  // SPI2 global Interrupt 
#define USART1_IRQ                  ((u8)0x25)  // USART1 global Interrupt 
#define USART2_IRQ                  ((u8)0x26)  // USART2 global Interrupt 
#define USART3_IRQ                  ((u8)0x27)  // USART3 global Interrupt 
#define EXTI15_10_IRQ               ((u8)0x28)  // External Line[15:10] Interrupts 
#define RTCAlarm_IRQ                ((u8)0x29)  // RTC Alarm through EXTI Line Interrupt 
#define USBWakeUp_IRQ               ((u8)0x2A)  // USB WakeUp from suspend through EXTI Line Interrupt 
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

//void configExternalInterrupt( u16 uiPort, u16 uiPin, u16 uiMode );
void extiConfig( u8 ucPin, u16 uiMode );
void interruptEnable( u32 ulVector );
void interruptDisable( u32 ulVector );
void interruptSetPriority( u32 ulVector, u8 ucPriority );
