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

//#pragma once
#ifndef __PYGMY_HEADER_ADC
	#define __PYGMY_HEADER_ADC

#include "pygmy_sys.h"                   

//CR1
#define ADC_AWDEN                   BIT23
#define ADC_JAWDEN                  BIT22
#define ADC_JDISCEN                 BIT12
#define ADC_DISCEN                  BIT11
#define ADC_JAUTO                   BIT10
#define ADC_AWDSGL                  BIT9
#define ADC_SCAN                    BIT8
#define ADC_JEOCIE                  BIT7
#define ADC_AWDIE                   BIT6
#define ADC_EOCIE                   BIT5
//SR

#define ADC_STRT                    BIT4    // Conversion started flag
#define ADC_JSTRT                   BIT3    // Injected conversion started flag
#define ADC_JEOC                    BIT2    // Injected End Of Conversion flag
#define ADC_EOC                     BIT1    // End Of Conversion flag
#define ADC_AWD                     BIT0    // Analog Watch Dog flag

//CR2
#define ADC_TSVREFE                 BIT23   // Temp Sense and VREF Enable
#define ADC_SWSTART                 BIT22   // Software Start Conversion
#define ADC_JSWSTART                BIT21   // Injected Conversion Software Start
#define ADC_EXTTRIG                 BIT20
#define ADC_EXTSEL_CLEAR            (BIT19|BIT18|BIT17)
#define ADC_EXTSEL_TIMER1CC1        0        // All bits clear, default
#define ADC_EXTSEL_TIMER1CC2        BIT17
#define ADC_EXTSEL_TIMER1CC3        BIT18
#define ADC_EXTSEL_TIMER2CC2        (BIT18|BIT17)
#define ADC_EXTSEL_TIMER3TRGO       BIT19
#define ADC_EXTSEL_TIMER4CC4        (BIT19|BIT17)
#define ADC_EXTSEL_EXTI11           (BIT19|BIT18)
#define ADC_EXTSEL_SWSTART          (BIT19|BIT18|BIT17)
#define ADC_JEXTTRIG                BIT15
#define ADC_JEXTSEL_TIMER1TRGO      0         // All bits clear, default
#define ADC_JEXTSEL_ALIGN           BIT11
#define ADC_DMA                     BIT8
#define ADC_RSTCAL                  BIT3
#define ADC_CAL                     BIT2
#define ADC_CONT                    BIT1
#define ADC_ADON                    BIT0
#define ADC_SMP_CLEAR               (BIT0|BIT1|BIT3) // All SMP bits must be shifted into place
#define ADC_SMP_1P5                 0        // All bits clear
#define ADC_SMP_7P5                 BIT0
#define ADC_SMP_13P5                BIT1
#define ADC_SMP_28P5                (BIT1|BIT0)
#define ADC_SMP_41P5                BIT2
#define ADC_SMP_55P5                (BIT2|BIT0)
#define ADC_SMP_71P5                (BIT3|BIT1)
#define ADC_SMP_239P5               (BIT2|BIT1|BIT0)

//STM32 ADC

typedef struct{
    volatile u32 SR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 SMPR1;
    volatile u32 SMPR2;
    volatile u32 JOFR1;
    volatile u32 JOFR2;
    volatile u32 JOFR3;
    volatile u32 JOFR4;
    volatile u32 HTR;
    volatile u32 LTR;
    volatile u32 SQR1;
    volatile u32 SQR2;
    volatile u32 SQR3;
    volatile u32 JSQR;
    volatile u32 JDR1;
    volatile u32 JDR2;
    volatile u32 JDR3;
    volatile u32 JDR4;
    volatile u32 DR;
    } PYGMYADC;

#define ADC1_BASE               (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE               (APB2PERIPH_BASE + 0x2800)
#define ADC1                    ((PYGMYADC *) ADC1_BASE)
#define ADC2                    ((PYGMYADC *) ADC2_BASE)

extern const u8 PYGMY_ADC_CHANNELS[];

u16 adcGetStatus( void );
void adcInit( void ); //PYGMYADC *pygmyADC );
u16 adcSingleSample( u8 ucPin );
void adcSingleSampleInit( void );
u8 adcGetChannelFromPin( u8 ucPin );
u8 adcEnableChannel( u8 ucPin ); //PYGMYADC *pygmyADC );
u8 adcDisableChannel( u8 ucPin ); //PYGMYADC *pygmyADC );
u8 adcGetSQR( u8 ucIndex );
u16 adcGet( u8 ucPin ); //PYGMYADC *pygmyADC );
u8 adcSetSampleTime( u8 ucChannel, u8 ucSampleTime ); //PYGMYADC *pygmyADC, u8 ucChannel, u32 ucTimeMask );
//u8 Pygmy_ADC_AddChannelToRegularSequence( PYGMYADC *pygmyADC, u8 ucChannel );

#endif // __PYGMY_HEADER_ADC
