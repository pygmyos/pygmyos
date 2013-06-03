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

const u8 PYGMY_TIMERVECTORS[] = {      
    0,          TIM1_UP_IRQ,    TIM2_IRQ,       TIM3_IRQ,       TIM4_IRQ,       TIM5_IRQ, 
    TIM6_IRQ,   TIM7_IRQ,       TIM8_UP_IRQ,    TIM9_IRQ,       TIM10_IRQ,      TIM11_IRQ, 
    TIM12_IRQ,  TIM13_IRQ,      TIM14_IRQ,      TIM15_IRQ,      TIM16_IRQ,      TIM17_IRQ 
};

const u8 PYGMY_TIMERVECTORS_L15X[] = { 
    0,          0,              TIM2_IRQ,       TIM3_IRQ,       TIM4_IRQ,       0, 
    TIM6_IRQ,   TIM7_IRQ,       0,              TIM9_L15X_IRQ,  TIM10_L15X_IRQ, TIM11_L15X_IRQ,
    0,          0,              0,              0,              0,              0
};

u8 interruptGetTimerVector( u8 ucTimer )
{
    // return value of 0 means no timer present
    if( descriptorGetID() == DESC_STM32L151 ){
        return( PYGMY_TIMERVECTORS_L15X[ ucTimer ] );
    } else{
        return( PYGMY_TIMERVECTORS[ ucTimer ] );
    } // else
    
}

void interruptEnable( u32 ulVector )
{
    // The vectors are arranged 1 bit per vector in 32 groups
    // The vector number must be decoded, first for which group, second the bit to set
    if( ulVector > 63 ){
        NVIC->ISER[ 2 ] = 0x00000001 << ( ulVector - 64 );
    } else if( ulVector > 31 ){
        NVIC->ISER[ 1 ] = 0x00000001 << ( ulVector - 32 );
    } else{
        NVIC->ISER[ 0 ] = 0x00000001 << ulVector;
    }
}

void interruptDisable( u32 ulVector )
{
    // The vectors are arranged 1 bit per vector in 32 groups
    // The vector number must be decoded, first for which group, second the bit to set
    // Notice! Bits set in the ICER registers actually clear the interrupt vectors
    if( ulVector > 63 ){
        NVIC->ICER[ 2 ] = 0x00000001 << ( ulVector - 64 );
    } else if( ulVector > 31 ){
        NVIC->ICER[ 1 ] = 0x00000001 << ( ulVector - 32 );
    } else{
        NVIC->ICER[ 0 ] = 0x00000001 << ulVector;
    }
}

void interruptSetPriority( u32 ulVector, u8 ucPriority )
{
    // IPR is a register array of up to 20 32 bit d-words
    // Each register contains 4 8 bit priorities which are linearly mapped
    // To a corresponding Vector. Only the High nibble of each 8 bit group
    // Is used to represent the priority from 0 - 15
    // Note that some of the Vectors are dealt with in System Registers
    // Those Vectors are all start at 0xF0 according to the system to allow
    // Decoding, they are separated and handled individually below
    if( ulVector > 0xEF ){
        switch( ulVector ){
            case MEMFAULT_IRQ:
                SCB->SHPR[0] &= 0xFFFFFF0F;               // Clear nibble
                SCB->SHPR[0] |=  ( u32 ) ucPriority ;     // Write Priority
            break;
            case BUSFAULT_IRQ:      
                SCB->SHPR[0] &= 0xFFFF0FFF;               // Clear nibble
                SCB->SHPR[0] |= ( u32 ) ucPriority << 8;  // Write Priority
            break;
            case USAGEFAULT_IRQ:
                SCB->SHPR[0] &= 0xFF0FFFFF;               // Clear nibble
                SCB->SHPR[0] |= ( u32 ) ucPriority << 16; // Write Priority
            break;
            case SVCALL_IRQ:
                SCB->SHPR[1] &= 0x0FFFFFFF;               // Clear nibble
                SCB->SHPR[1] |= ( u32 ) ucPriority << 24; // Write Priority
            break;
            case PENDSVCALL_IRQ:
                SCB->SHPR[1] &= 0xFF0FFFFF;               // Clear nibble
                SCB->SHPR[1] |= ( u32 ) ucPriority << 16; // Write Priority
            break;
            case SYSTICK_IRQ:
                SCB->SHPR[2] &= 0x0FFFFFFF;               // Clear nibble
                SCB->SHPR[2] |= ( u32 ) ucPriority << 24; // Write Priority
            break;
        }
    } else{
        NVIC->IPR[ ulVector / 4 ] &= ~( ( u32 )0xF0 ) << ( ( ulVector % 4 ) * 8 );      // Clear nibble
        NVIC->IPR[ ulVector / 4 ] |= ( ( u32 )ucPriority ) << ( ( ulVector % 4 ) * 8 ); // Write Priority
    } 
}


