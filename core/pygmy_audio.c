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

#include <math.h>
#include "pygmy_profile.h"

PYGMYFILE pygmyAudioFile;
u8 globalAudioBuffer[ 1024 ], globalAudioPage;

void audioPlayWave( u8 *ucName )
{ 
    // Word and Long Fields stored little-endian
    u32 i, ulChunkSize, ulSubChunk1Size;
    u32 ulSampleRate, ulByteRate, ulSubChunk2Size;
    u16 uiAudioFormat, uiNumChannels, uiBlockAlign, uiBitsPerSample;
    u8 ucBuffer[ 5 ];

    if( !fileOpen( &pygmyAudioFile, ucName, READ ) ){
        return;
    } // if
    fileGetBuffer( &pygmyAudioFile, 4, ucBuffer );
    ucBuffer[ 4 ] = '\0';
    if( !isStringSame( ucBuffer, "RIFF" ) ){
        return;
    } // if
    ulChunkSize = fileGetLong( &pygmyAudioFile, LITTLEENDIAN );
    fileGetBuffer( &pygmyAudioFile, 4, ucBuffer );
    ucBuffer[ 4 ] = '\0';
    if( !isStringSame( ucBuffer, "WAVE" ) ){
        return;
    } // if
    fileGetBuffer( &pygmyAudioFile, 4, ucBuffer );
    ucBuffer[ 4 ] = '\0';
    if( !isStringSame( ucBuffer, "fmt " ) ){
        return;
    } // if
    ulSubChunk1Size = fileGetLong( &pygmyAudioFile, LITTLEENDIAN );
    uiAudioFormat = fileGetWord( &pygmyAudioFile, LITTLEENDIAN );
    uiNumChannels = fileGetWord( &pygmyAudioFile, LITTLEENDIAN );
    ulSampleRate = fileGetLong( &pygmyAudioFile, LITTLEENDIAN );
    ulByteRate = fileGetLong( &pygmyAudioFile, LITTLEENDIAN );
    uiBlockAlign = fileGetWord( &pygmyAudioFile, LITTLEENDIAN );
    uiBitsPerSample = fileGetWord( &pygmyAudioFile, LITTLEENDIAN );
    fileGetBuffer( &pygmyAudioFile, 4, ucBuffer );
    ucBuffer[ 4 ] = '\0';
    if( !isStringSame( ucBuffer, "data" ) ){
        return;
    } // if
    ulSubChunk2Size = fileGetLong( &pygmyAudioFile, LITTLEENDIAN );
    
    print( COM3, "\rValid WAVE File" );
    print( COM3, "\rAudioFormat: %d", uiAudioFormat );
    print( COM3, "\rChannels: %d", uiNumChannels );
    print( COM3, "\rSampleRate: %d", ulSampleRate );
    print( COM3, "\rByteRate: %d", ulByteRate );
    print( COM3, "\rBlockAlign: %d", uiBlockAlign );
    print( COM3, "\rBitsPerSample: %d", uiBitsPerSample );
    print( COM3, "\rDataLen: %d", ulSubChunk2Size );
    
    fileGetBuffer( &pygmyAudioFile, 1024, globalAudioBuffer );
    globalAudioPage = 0;
    DAC1_INIT;
    //SHIELD_SHUTDOWN_INIT;
    //SHIELD_SHUTDOWN_LOW;
    interruptEnable( DMA1_CH3_IRQ );
    dac1Output( globalAudioBuffer, 1024, ulSampleRate, PYGMY_DAC_CONTINUOUS );
}
/*
void DMA1_Channel3_IRQHandler( void )
{
    print( COM3, "\rInvalid IRQ" );
    if( !( DMA2->ISR & BIT10 ) ){
        // Test For Channel3 Half Transfer Flag, BIT10
        DMA2->IFCR = ( BIT8|BIT9|BIT10|BIT11);
        return;
    } // if
    fileGetBuffer( &pygmyAudioFile, 512, globalAudioBuffer + ( globalAudioPage * 512 ) );
    if( globalAudioPage ){
        globalAudioPage = 0;
        DMA2_CH3->CMAR = (volatile u32)(volatile u32*)globalAudioBuffer; 
    } else{
        globalAudioPage = 1;
    } // else
    DMA2->IFCR = ( BIT8|BIT9|BIT10|BIT11);
    //PYGMY_DMA1_CH3_DISABLE;
    //DMA1_CH3->CNDTR = 1024 ; // length of test  
    
    if( (fileIsEOF( &pygmyAudioFile ) ) ){
        PYGMY_DMA2_CH3_DISABLE;
    } // if
    
}*/

void generateSineWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude )
{
    unsigned int i, uiSamples;
    float factor; 
    
    uiSamples = uiSampleRate / uiFrequency;  
    // uiSamplesRate = uiSamples * uiFrequency; 
    factor = ( 2 * 3.14159265 ) / uiSamples;

    for( i = 0; i < uiSamples; i++ ){
        ucBuffer[ i ] =  ( uiAmplitude / 2) + sin( i * factor) * (float)( uiAmplitude / 2 ) ;
    } // for
    
}

void generateDTMF( u8 *ucBuffer, u16 uiFrequency1, u16 uiFrequency2, u16 uiSamples, u16 uiSampleRate, u16 uiAmplitude )
{
    unsigned int i, uiValue1, uiValue2;
    float fFactor1, fFactor2; 
    
    fFactor1 = ( 2 * 3.14159265 ) / ( uiSampleRate / uiFrequency1 );
    fFactor2 = ( 2 * 3.14159265 ) / ( uiSampleRate / uiFrequency2 );
    uiAmplitude /= 2;
    
    for( i = 0; i < uiSamples; i++ ){
        uiValue1 = uiAmplitude + sin( i * fFactor1 ) * (float)uiAmplitude;
        uiValue2 = uiAmplitude + sin( i * fFactor2 ) * (float)uiAmplitude;
        ucBuffer[ i ] = ( uiValue1 + uiValue2 ) / 2;  
    } // for
    
}

void generateSquareWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude )
{
    unsigned int i, uiSamples;
    
    uiSamples = uiSampleRate / uiFrequency;  
    // uiSamplesRate = uiSamples * uiFrequency; 

    for( i = 0; i < uiSamples/2; i++ ){
        ucBuffer[ i ] = uiAmplitude;
    } // for
    for( ; i < uiSamples; i++ ){
        ucBuffer[ i ] = 0;
    } // for
    
}

void generateTriangleWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude )
{
    unsigned int i, uiSamples, uiRampUp, uiValue;
    
    if( uiAmplitude < 1 ){
        uiAmplitude = 1;
    } // if
    uiSamples = uiSampleRate / uiFrequency;  
    // uiSamplesRate = uiSamples * uiFrequency; 
    uiRampUp = ( uiSamples / 2 ) + ( uiSamples % 2 );
    i = 0;
    if( uiSamples % 2 ){
        ucBuffer[ i++ ] = 0;
    } // for
    for( ; i <= uiRampUp; i++ ){
        uiValue = i * ( uiAmplitude / uiRampUp );
        if( uiValue > 255 )
            uiValue = 255;
        ucBuffer[ i ] = uiValue;
        ucBuffer[ uiSamples - i ] = uiValue;
    } // for

}

void generateSawWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude )
{
    unsigned int i, uiSamples, uiValue;
    uiSamples = uiSampleRate / uiFrequency;  
    // uiSamplesRate = uiSamples * uiFrequency; 

    for( i = 0; i < uiSamples; i++ ){
        uiValue = i * ( uiAmplitude / uiSamples );
        ucBuffer[ i ] = uiValue;
    } // for
}

void blendWaves( u8 *ucBufferDest, u16 uiBufferDestLen, u8 *ucBuffer, u16 uiBufferLen )
{
    unsigned int i, ii;

    for( i = 0, ii = 0; i < uiBufferDestLen ; i++, ii++ ){
        if( ii == uiBufferLen ){
            ii = 0;
        } // if
        //if( ucBufferDest[ i ] < ucBuffer[ ii ] )
        ucBufferDest[ i ] = ( ucBufferDest[ i ] + ucBuffer[ ii ] ) / 2;
    } // if
}

void dac1Output( u8* ucBuffer, u32 uiBufferLen, u16 uiSampleRate, u8 uiMode )
{
    PYGMY_RCC_AFIO_ENABLE;
    PYGMY_RCC_GPIOA_ENABLE;
    PYGMY_RCC_TIMER6_ENABLE;
    PYGMY_RCC_TIMER7_ENABLE
    
    PYGMY_RCC_DMA2_ENABLE;
    PYGMY_RCC_DAC_ENABLE;
    DAC1_INIT; // PA5
    PYGMY_DMA2_CH3_DISABLE;

    TIM6->CNT = 0; // Counter
    TIM6->PSC = 0; // Prescaler register
    TIM6->ARR = 72000000 / uiSampleRate; // Auto Reload Register
    TIM6->DIER = BIT8;//BIT0 is Update Interrupt Enabled, BIT8 is Update DMA request Enabled
    TIM6->CR2 = BIT5; // 
    TIM6->CR1 = BIT0; // BIT0 is Counter Enable
    DAC->CR |=  DAC_DMAEN1 | DAC_TEN1 | DAC_BOFF1|DAC_EN1; // 

    DMA2_CH3->CNDTR = uiBufferLen; // length of test
    DMA2_CH3->CPAR = (volatile u32)(volatile u32*)&DAC->DHR8R1;
    DMA2_CH3->CMAR = (volatile u32)(volatile u32*)ucBuffer; 
    // Must be enabled last
    if( uiMode & PYGMY_DAC_CONTINUOUS ){
        DMA2_CH3->CCR =  DMA_HTIE | DMA_MINC | DMA_CIRC | DMA_DIR | DMA_EN; // MINC set = Memory address inc mode, DIR set = mem to peripheral
    } else{
        DMA2_CH3->CCR =  DMA_HTIE | DMA_MINC | DMA_DIR | DMA_EN;
    } // else
}

void dac2Output( u8* ucBuffer, u32 uiBufferLen, u16 uiSampleRate, u8 uiMode )
{
    DMA2_CH4->CCR &= ~DMA_EN;
    PYGMY_RCC_AFIO_ENABLE;
    PYGMY_RCC_GPIOA_ENABLE
    PYGMY_RCC_TIMER7_ENABLE;
    
    PYGMY_RCC_DMA2_ENABLE;
    PYGMY_RCC_DAC_ENABLE;
    DAC2_INIT;
    TIM7->CR1 |= BIT0;
    TIM7->CNT = 0; // Counter
    TIM7->PSC = 0; // Prescaler register
    TIM7->ARR = 72000000 / (u32)uiSampleRate; // Auto Reload Register
    TIM7->DIER = BIT8|BIT0;//BIT0 is Update Interrupt Enabled, BIT8 is Update DMA request Enabled
    TIM7->CR2 = BIT5; // 
    TIM7->CR1 = BIT0; // BIT0 is Counter Enable
    
    DAC->CR |= BIT20 | DAC_DMAEN2 | DAC_TEN2 | DAC_EN2; // BIT20 is Channel2, Timer 7 select
    
    DMA2_CH4->CNDTR = uiBufferLen; // length of test
    DMA2_CH4->CPAR = (volatile u32)(volatile u32*)&DAC->DHR8R2;
    DMA2_CH4->CMAR = (volatile u32)(volatile u32*)ucBuffer; 
    // Must be enabled last
    if( uiMode & PYGMY_DAC_CONTINUOUS ){
        DMA2_CH4->CCR = DMA_MINC | DMA_CIRC | DMA_DIR | DMA_EN; // MINC set = Memory address inc mode, DIR set = mem to peripheral
    } else{
        DMA2_CH4->CCR = DMA_MINC | DMA_DIR | DMA_EN;
    } // else
}



