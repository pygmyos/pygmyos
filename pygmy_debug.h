#include "pygmy_profile.h"

#pragma once

typedef struct{
    volatile u32 CR;
} DEBUGMCU_TYPEDEF;

#define DEBUGMCU_BASE               ((u32)0xE00420040) 
#define DEBUGMCU                     ((DEBUGMCU_TYPEDEF *) DEBUGMCU_BASE )

#define DEBUG_TIM14_STOP            BIT27
#define DEBUG_TIM13_STOP            BIT26
#define DEBUG_TIM12_STOP            BIT25
#define DEBUG_TIM17_STOP            BIT24
#define DEBUG_TIM16_STOP            BIT23
#define DEBUG_TIM15_STOP            BIT22
#define DEBUG_TIM7_STOP             BIT20
#define DEBUG_TIM6_STOP             BIT19
#define DEBUG_TIM5_STOP             BIT18
#define DEBUG_I2C2_SMBUS_TIMEOUT    BIT16
#define DEBUG_I2C1_SMBUS_TIMEOUT    BIT15
#define DEBUG_TIM4_STOP             BIT13
#define DEBUG_TIM3_STOP             BIT12
#define DEBUG_TIM2_STOP             BIT11
#define DEBUG_TIM1_STOP             BIT10
#define DEBUG_WWDG_STOP             BIT9
#define DEBUG_IWDG_STOP             BIT8
#define DEBUG_TRACEMODE_CLEAR       ( BIT7|BIT6 )
#define DEBUG_TRACEMODE_0           0
#define DEBUG_TRACEMODE_1           BIT6
#define DEBUG_TRACEMODE_2           BIT7
#define DEBUG_TRACEMODE_3           ( BIT7|BIT6 )
#define DEBUG_TRACE_IOEN            BIT5
#define DEBUG_STANDBY               BIT2
#define DEBUG_STOP                  BIT1
#define DEBUG_SLEEP                 BIT0

#define DEBUG_DISABLE               DEBUGMCU->CR &= ~DEBUG_STOP
#define DEBUG_ENABLE                DEBUGMCU->CR |= DEBUG_STOP