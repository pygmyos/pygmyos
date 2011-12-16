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

#include "pygmy_profile.h"

const u8 PYGMY_ADC_CHANNELS[] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PB0, PB1, PC0, PC1, PC2, PC3, PC4, PC5, ADCTEMP, ADCREF };
u16 uiGlobalADC1Channel, uiGlobalADC1[ 16 ];

void ADC1_2_IRQHandler( void )
{
    u16 i;

    if( ADC1->SR & ADC_EOC ){
        print( COM3, "( %d )", ADC1->DR );
        //uiGlobalADC1[ uiGlobalADC1Channel++ ] = ADC1->DR;
        
    } // if
    
    ADC1->SR = 0x00;
        
}

void DMAChannel1_IRQHandler( void )
{
    u16 i;

    if( DMA1->ISR & BIT3 ){
        print( COM3, "\r" );
        for( i = 0; i < 16; i++ ){
            print( COM3, " %d", uiGlobalADC1[ i ]  );
        } //for
    }
    //DMA1->IFCR = 0x0000000F;
}

void adcInit( void ) //u32 ulScan )
{
    // ADC1 uses DMA1 Channel1 to manage Data Register after conversion
    u16 i;

    for( i = 0; i< 16; i++ ){
        uiGlobalADC1[ i ] = 0;
        ADC1->SQR1 = 0;
        ADC1->SQR2 = 0;
        ADC1->SQR3 = 0;
        ADC1->SMPR1 = 0;
        ADC1->SMPR2 = 0;
    } // for
    PYGMY_RCC_ADC1_ENABLE;
    PYGMY_RCC_DMA1_ENABLE;
    PYGMY_DMA1_CH1_DISABLE;
    ADC1->CR2 = 0x00;       // Clear ADC if already in use
    //enableInterrupt( ADC_IRQ );
    //enableInterrupt( DMAChannel1_IRQ );
    //setInterruptPriority( DMAChannel1_IRQ, 1 );
    //setInterruptPriority( ADC_IRQ, 1 );

    ADC1->CR1 = ADC_SCAN;//BIT5; // Enable EOC Interrupt 
    
    //ADC1->CR2 |= (BIT20|BIT19|BIT18|BIT17); // Software trigger
    ADC1->CR2 = (ADC_DMA|ADC_EXTTRIG|ADC_EXTSEL_SWSTART|ADC_ADON); // Start ADC with Calibration
    //ADC1->CR2 = ADC_ADON;
    //while( !(ADC1->CR2 & ADC_CAL) ); // Wait for Calibration to complete
    //print( COM3, "\rADC Ready..." );
    
    DMA1_CH1->CPAR = (volatile u32)(volatile u32*)ADC1->DR; // Peripheral Register Pointer
    DMA1_CH1->CMAR = (volatile u32)(volatile u32*)uiGlobalADC1; // Destination Memory Address
    DMA1_CH1->CNDTR = 0; // Number of values to transfer
    DMA1_CH1->CCR = ( DMA_MSIZE16 | DMA_PSIZE16 | DMA_MINC | DMA_CIRC );   
}

void adcStart( void )
{
    //ADC1->CR2 |= ADC_ADON;   // Start Single Conversion
    ADC1->CR2 |= ADC_SWSTART;
}

u8 adcSetSampleTime( u8 ucChannel, u8 ucSampleTime )
{
    u8 ucInput;

    //ucInput = 
    
    //ADC1->SMPR1 = 
    //ADC1->SMPR2 = 
}

u8 adcEnableChannel( u8 ucPin ) 
{
    u16 i;
    u8 ucChannel;

    ucChannel = adcGetChannelFromPin( ucPin ) + 1;
    for( i = 1; i < 17; i++ ){
        if( !adcGetSQR( i ) ){
            break; // found empty channel
        } // if
    } // for
    if( i == 17 ) {
        return( 0 ); // No empty channels
    } // if

    if( i < 7 ){
        ADC1->SQR1 |= ( ucChannel << ( ( i - 1 ) * 5 ) );
    } else if( i < 13 ){
        ADC1->SQR2 |= ( ucChannel << ( ( i - 7 ) * 5 ) );
    } else{
        ADC1->SQR3 |= ( ucChannel << ( ( i - 13 ) * 5 ) );
    } // else
    //PYGMY_DMA1_CH1_DISABLE;
    //DMA1_CH1->CNDTR += 1;
    //DMA1_CH1->CCR |= DMA_EN;  

    return( 1 );
}

u8 adcGetSQR( u8 ucIndex )
{
    // Note!!! This function uses a 1 base index, i.e. SQ1, etc., to match the ST datasheets
    u8 ucSQR;
    
    if( ucIndex < 7 ){
        ucSQR = ADC1->SQR1 >> ( ( ucIndex - 1 ) * 5 );
    } else if( ucIndex < 13 ){
        ucSQR = ADC1->SQR2 >> ( ( ucIndex - 7 ) * 5 );
    } else{
        ucSQR = ADC1->SQR3 >> ( ( ucIndex - 13 ) * 5 );
    } // else
    
    return( ucSQR & 0x1F ); // Mask the 5 bit register value
}

u8 adcDisableChannel( u8 ucChannel )
{

}

void adcDisableAll( u8 ucChannel )
{

}

void adcSingleSampleInit( void )
{
    u16 i;

    PYGMY_RCC_ADC1_ENABLE;
    ADC1->CR2 = 0;
    ADC1->SR = 0;
    ADC1->CR1 = 0;
    ADC1->SMPR1 = 0;
    ADC1->SMPR2 = 0;
    ADC1->SQR1 = 0;
    ADC1->SQR2 = 0;
    ADC1->SQR3 = 0;
    ADC1->CR2 = (ADC_CAL|ADC_EXTTRIG|ADC_EXTSEL_SWSTART|ADC_ADON);
    while( ADC1->CR2 & ADC_CAL );
    //for( i = 0; i < 10; i++ ){
    //    adcSingleSample( ADCREF );
    //} // for
    
}

u16 adcSingleSample( u8 ucPin )
{
    u16 uiADC;

    ADC1->SQR3 = adcGetChannelFromPin( ucPin );
    ADC1->SR = 0;
    ADC1->CR2 |= (ADC_ADON);
    while( !( ADC1->SR & ADC_EOC ) );    
    uiADC = ADC1->DR;
    return( uiADC ); 
}

u8 adcGetChannelFromPin( u8 ucPin )
{
    u8 i;

    for( i = 0; i < 18; i++ ){
        if( PYGMY_ADC_CHANNELS[ i ] == ucPin ){
            return( i );
        } // if
    } // for

    return( 0 );
}

u16 adcGet( u8 ucChannel )
{
    
}



/*
u32 adcInit( PYGMYADC *pygmyADC )
{
    u32 ulCR2;
    //EOCIE = BIT5 = EOC Int enable ( ADC->CR1 )
    //ADC1->CR1 = 0;
    //TSVREFE = BIT23 = temp + vref enable ( ADC>CR2 )
    //SWSTART = BIT22 = SW conversion start ( ADC>CR2 )
    //Bits 19:17 EXTSEL[2:0], 111 = SW start ( ADC>CR2 )
    //Bit 8 = DMA enable ( ADC>CR2 )
    //Bit 3 RSTCAL: Reset calibration ( ADC>CR2 )
    //Bit 2 CAL: A/D Calibration ( ADC>CR2 )
    //Bit 0 ADON: A/D converter ON / OFF ( ADC>CR2 )

    ulCR2 = pygmyADC->CR2;
    //pygmyADC->CR2 |= ADC_SWSTART;
    pygmyADC->CR2   = 0x00000000;
    pygmyADC->CR1   = ADC_EOCIE;//0x00000000;
    pygmyADC->SMPR1 = 0x00000000;
    pygmyADC->SMPR2 = 0x00000000;
    pygmyADC->JOFR1 = 0x00000000;
    pygmyADC->JOFR2 = 0x00000000;
    pygmyADC->JOFR3 = 0x00000000;
    pygmyADC->JOFR4 = 0x00000000;
    pygmyADC->HTR   = 0x00000FFF;
    pygmyADC->LTR   = 0x00000000;
    pygmyADC->SQR1  = 0x00000000;
    pygmyADC->SQR2  = 0x00000000;
    pygmyADC->SQR3  = 0x00000000;
    pygmyADC->JSQR  = 0x00000000;
    
    //ADC1->SMPR1 |= ( BIT23|BIT22|BIT21|BIT20|BIT19|BIT18); // maximum time on vref and internal temp sensor
    //ADC1->SMPR1 |= ( BIT4|BIT3); // 28.5 clock cycles for voltage in on channel ADC_IN1
    //ADC1->SQR3 |= ( BIT0 ); // Set IN1 in sequence 1
    //ADC1->CR2 |= ( BIT19|BIT18|BIT17|BIT0 );
    return( ulCR2 );
}

void adcInterruptEnable( PYGMYADC *pygmyADC, u32 ulInterruptMask )
{
    pygmyADC->CR1 |= ulInterruptMask;
}

void adcInterruptDisable( PYGMYADC *pygmyADC, u32 ulInterruptMask )
{
    pygmyADC->CR1 &= ~ulInterruptMask;
}

void adcStartRegularConversions( PYGMYADC *pygmyADC )
{
    // Works for Software Start Only
   // pygmyADC->CR2 &= ~ADC_ADON;
    //pygmyADC->CR2 |= ADC_SWSTART;
    pygmyADC->CR2 |= ADC_ADON;
}

void adcEnable( PYGMYADC *pygmyADC )
{
    pygmyADC->CR2 |= ADC_ADON;
}

void adcDisable( PYGMYADC *pygmyADC )
{
    pygmyADC->CR2 &= ~ADC_ADON;
}

void adcTempSensorEnable( void )
{
    // Temp sensor is only integrated into ADC1

    Pygmy_ADC_SetSampleTime( ADC1, 16, ADC_SMP_28P5 );
    Pygmy_ADC_SetSampleTime( ADC1, 17, ADC_SMP_28P5 );
    ADC1->CR2 |= ADC_TSVREFE;
}

void adcTempSensorDisable( void )
{
    ADC1->CR2 &= ~ADC_TSVREFE;
}

u16 getGetMillivolts( PYGMYADC *pygmyADC, u32 ulRef )
{   
    u32 ulData;

    ulData = pygmyADC->DR;
    ulData = ( ulData * ulRef ) / 4096;

    return( (u16)ulData );
}

u8 adcTriggerSelect( PYGMYADC *pygmyADC, u32 ulTriggerMask )
{
    u32 ulCR2;
    
    ulCR2 = pygmyADC->CR2;
    pygmyADC->CR2 &= ~ADC_ADON; // Disable ADC before changing settings
    pygmyADC->CR2 &= ~ADC_EXTSEL_CLEAR;
    pygmyADC->CR2 |= ulTriggerMask;
    if( ulCR2 & ADC_ADON )
        pygmyADC->CR2 |= ADC_ADON;
    
    return( 1 ); 
}

u8 adcSetSampleTime( PYGMYADC *pygmyADC, u8 ucChannel, u32 ucTimeMask )
{
    // Time Mask must be a predefined SMP mask, not tp be used as a variable

    u32 i;

    if( ucTimeMask > 7 )
        return( 0 );
    if( ucChannel < 18 && ucChannel > 9 ){ // SMPR1 register contains channels 10 through 17
        pygmyADC->SMPR1 &= ~( ADC_SMP_CLEAR << ( ( ucChannel - 10 ) * 3 ) ); // 3 bits per channel 
        pygmyADC->SMPR1 |= ucTimeMask << ( ( ucChannel - 10 ) * 3 );
        return( 1 );
    } else if( ucChannel < 10 ){
        pygmyADC->SMPR2 &= ~( ADC_SMP_CLEAR << ( ucChannel * 3 ) ); // 3 bits per channel 
        pygmyADC->SMPR2 |= ucTimeMask << ( ucChannel * 3 );
        return( 1 );
    }
    
    return( 0 );
}

u8 adcAddChannelToRegularSequence( PYGMYADC *pygmyADC, u8 ucChannel )
{
    // Returns 0 for error or number in sequence + 1

    u32 ulConversions;
    
    ulConversions = ( 0x00F00000 & pygmyADC->SQR1 ) >> 20;
    if( ulConversions == 0x0000000F )                   // Make sure that there is a sequence left, max 16
        return( 0 );
    ++ulConversions;                                    // Increment sequence count
    pygmyADC->SQR1 |= ulConversions << 20;              // Write updated sequence count to register
    if( ulConversions > 12 ){                           // Sequence 16-13 in SQR1
        pygmyADC->SQR1 &= ~( 0x0000001F << ( ulConversions - 13 ) ); // 5 bits per sequence for channel number
        pygmyADC->SQR1 |= ucChannel << ( ulConversions - 13 );
    } else if( ulConversions > 6 ){                      // Sequence 12-7 in SQR2
        pygmyADC->SQR2 &= ~( 0x0000001F << ( ulConversions - 7 ) );
        pygmyADC->SQR2 |= ucChannel << ( ulConversions - 7 );
    } else {                                            // Sequence 6-0 in SQR3    
        pygmyADC->SQR3 &= ~( 0x0000001F << ulConversions );
        pygmyADC->SQR3 |= ucChannel << ulConversions;
    }
    
    return( 1 + ulConversions );
}
*/
