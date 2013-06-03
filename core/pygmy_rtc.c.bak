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

#include "pygmy_profile.h"

//-------------------------------------------------------------------------------------
//------------------------------------Pygmy OS Time------------------------------------

void resetBackupDomain( void )
{
    RCC->BDCR = BIT16; // Sets reset bit, BDCR will be 0 after reset
}

void timeInit( void )
{
    u16 i;
    // Warning! Follow Configuration procedure:
    // 1. Poll RTOFF, wait until its value goes to ‘1’
    // 2. Set the CNF bit to enter configuration mode
    // 3. Write to one or more RTC registers
    // 4. Clear the CNF bit to exit configuration mode
    //5. Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation.
    
    // Use the following line as a Macro if needed
        //RCC->BDCR &= ~BIT16; // Reset Backup Domain
    PYGMY_RCC_PWR_ENABLE;
    PYGMY_RCC_BKP_ENABLE;
    PWR->CR |= BIT8;     // Disables Backup Domain/ RTC register write protection
    PYGMY_RCC_LSE_ENABLE;
    RCC->BDCR |= ( BIT0 | BIT8 | BIT15 );
    for( i = 0; !(RCC->BDCR & BIT1) && i < 20; i++ ){;} // wait for LSE clock to stabilize
    for( i = 0; !( RTC->CRL & RTC_RTOFF) && i < 500; i++ ){;}       // don't access registers until RTC sets RTOFF to 1
    RTC->CRL    |=  RTC_CNF;                // Enter configuration mode 
    RTC->CRH    |=  BIT0;                 // Clear and set BIT0, second interrupt bit
    RTC->CRL    &=  ~( RTC_RSF | RTC_OWF | RTC_ALRF | RTC_SECF ); // Clear any set flags
    RTC->PRLH   =   0x0000;                 // MSBs, not used with LSE
    RTC->PRLL   =   0x7FFF;                 // Prescale value for 32.768KHz LSE, see device reference
    //RTC->CNTH   =   0;//( ulTime >> 16 );  // RTC Time register is divided into 16 bits MSBs
    //RTC->CNTL   =   0;//(u16)( ulTime );        // and 16 bits LSBs
    RTC->CRL    &=  ~RTC_CNF;                // Exit config mode for RTC to update registers
}

void rtcClearFlags( void )
{
    u16 i;

    for( i = 0; !( RTC->CRL & RTC_RTOFF) && i < 500; i++ ){;}       // don't access registers until RTC sets RTOFF to 1
    RTC->CRL    |=  RTC_CNF;                // Enter configuration mode 
    RTC->CRL    &=  ~( RTC_RSF | RTC_OWF | RTC_ALRF | RTC_SECF ); // Clear any set flags
    RTC->CRL    &=  ~RTC_CNF;                // Exit config mode for RTC to update registers
}

void timeSet( u32 ulTime )
{
    // Sets time in seconds
    u16 i;

    for( i = 0; !( RTC->CRL & RTC_RTOFF) && i < 500; i++ ){;}      // don't access registers until RTC sets RTOFF to 1
    RTC->CRL    |=  RTC_CNF;                // Enter configuration mode 
    RTC->CNTH   =   ( ulTime >> 16 );  // RTC Time register is divided into 16 bits MSBs
    RTC->CNTL   =   (u16)( ulTime );        // and 16 bits LSBs
    RTC->CRL    &=  ~RTC_CNF;               // Exit config mode for RTC to update registers
}

u32 timeGet( void )
{
    // Gets time in seconds
    u32 ulTime;

    ulTime  =   RTC->CNTH;
    ulTime  <<= 16;
    ulTime  |=  RTC->CNTL;
    
    return( ulTime );
}

u32 convertSystemTimeToSeconds( PYGMYTIME *pygmyTime )
{
    u32 i, ulSeconds;
    
    for( i = 1970, ulSeconds = 0; i < pygmyTime->Year; i++ ){ 
        if( !( i % 100 ) && ( i % 400 ||  i % 4 ) ){
            ulSeconds += PYGMY_SECONDS_LEAPYEAR;
        } else{
            ulSeconds += PYGMY_SECONDS_COMMONYEAR;
        } // else
    } // for
    if( pygmyTime->Month < 1 ){
        pygmyTime->Month = 1;
    } // if
    if( pygmyTime->Month > 12 ){
        pygmyTime->Month = 12;
    } // if
    for( i = 0; i < pygmyTime->Month - 1; i++ ){
        ulSeconds += PYGMY_DAYSINMONTH[ i ] * PYGMY_SECONDS_DAY;
    } // for
    
    ulSeconds += ( pygmyTime->Day - 1 ) * PYGMY_SECONDS_DAY;
    ulSeconds += ( ( pygmyTime->Hour * 60 ) * 60 ) + ( pygmyTime->Minute * 60 ) + pygmyTime->Second;
    
    return( ulSeconds );
}

void convertSecondsToSystemTime( u32 ulSeconds, PYGMYTIME *pygmyTime )
{
    u32 i;

    i = 1970 + ( ulSeconds / PYGMY_SECONDS_AVERAGEYEAR );
    for( pygmyTime->Year = 1970; pygmyTime->Year < i ; pygmyTime->Year++ ){
        if( !( pygmyTime->Year % 100 ) && ( pygmyTime->Year % 400 ||  pygmyTime->Year % 4 ) ){
            ulSeconds -= PYGMY_SECONDS_LEAPYEAR;
        } else{
            ulSeconds -= PYGMY_SECONDS_COMMONYEAR;
        } // else
    } // for
    i = ulSeconds / PYGMY_SECONDS_AVERAGEMONTH;
    if( i > 11 ){
        i = 11;
    } // if
    for( pygmyTime->Month = 0; pygmyTime->Month < i  ; pygmyTime->Month++ ){ 
        ulSeconds -= PYGMY_DAYSINMONTH[ pygmyTime->Month ] * PYGMY_SECONDS_DAY; 
    } // for
    ++pygmyTime->Month;
    pygmyTime->Day = ulSeconds / PYGMY_SECONDS_DAY ;
    ulSeconds -= pygmyTime->Day * PYGMY_SECONDS_DAY;
    ++pygmyTime->Day;
    pygmyTime->Hour = ulSeconds / PYGMY_SECONDS_HOUR;
    ulSeconds -= pygmyTime->Hour * PYGMY_SECONDS_HOUR;
    pygmyTime->Minute = ulSeconds / PYGMY_SECONDS_MINUTE;
    pygmyTime->Second = ulSeconds % PYGMY_SECONDS_MINUTE;
} 

void setSysTick( u32 ulSystemClock, u32 ulTicksPerSecond )
{
    // Based on  ( 2X the System Clock ) / 1000
    SYSTICK->CTRL = 0;
    SYSTICK->LOAD = ( ulSystemClock / ulTicksPerSecond ) * 2;
    SYSTICK->VAL = ( ulSystemClock / ulTicksPerSecond ) * 2;
    SYSTICK->CTRL = 0x07;   // Enable system timer
}

void enableSysTick( void )
{
    SYSTICK->CTRL = 0x07;   // Enable system timer
}

void disableSysTick( void )
{
    SYSTICK->CTRL = 0;
}

