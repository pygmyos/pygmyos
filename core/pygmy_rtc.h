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
#ifndef __PYGMY_HEADER_RTC
	#define __PYGMY_HEADER_RTC

#include "pygmy_profile.h"

#define RTC_OWIE        BIT2
#define RTC_ALRIE       BIT1
#define RTC_SECIE       BIT0
#define RTC_RTOFF       BIT5
#define RTC_CNF         BIT4
#define RTC_RSF         BIT3
#define RTC_OWF         BIT2
#define RTC_ALRF        BIT1
#define RTC_SECF        BIT0
 

//STM32 Watch Dog Timers

typedef struct
{
  volatile u16 CRH;
  u16 RESERVED0;
  volatile u16 CRL;
  u16 RESERVED1;
  volatile u16 PRLH;
  u16 RESERVED2;
  volatile u16 PRLL;
  u16 RESERVED3;
  volatile u16 DIVH;
  u16 RESERVED4;
  volatile u16 DIVL;
  u16 RESERVED5;
  volatile u16 CNTH;
  u16 RESERVED6;
  volatile u16 CNTL;
  u16 RESERVED7;
  volatile u16 ALRH;
  u16 RESERVED8;
  volatile u16 ALRL;
  u16 RESERVED9;
} RTC_TypeDef;
    
#ifndef RTC_BASE
    #define RTC_BASE            (APB1PERIPH_BASE + 0x2800)
#endif


#ifndef RTC
    #define RTC                 ((RTC_TypeDef *) RTC_BASE)
#endif

void resetBackupDomain( void );
void timeInit( void );
void rtcClearFlags( void );
void timeSet( u32 ulTime );
u32 timeGet( void );
u32 convertSystemTimeToSeconds( PYGMYTIME *pygmyTime );
void convertSecondsToSystemTime( u32 ulSeconds, PYGMYTIME *pygmyTime );

void setSysTick( u32 ulSystemClock, u32 ulTicksPerSecond );
void enableSysTick( void );
void disableSysTick( void );

#endif // __PYGMY_HEADER_RTC

