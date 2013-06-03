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
#ifndef __PYGMY_HEADER_AUDIO
	#define __PYGMY_HEADER_AUDIO

#include "pygmy_profile.h"

#define PYGMY_DAC_CONTINUOUS        BIT0

//DAC->CR &= ~DAC_EN1;


void generateSineWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude );
void generateDTMF( u8 *ucBuffer, u16 uiFrequency1, u16 uiFrequency2, u16 uiSamples, u16 uiSampleRate, u16 uiAmplitude );
void generateSquareWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude );
void generateTriangleWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude );
void generateSawWave( u8 *ucBuffer, u16 uiFrequency, u16 uiSampleRate, u16 uiAmplitude );
void blendWaves( u8 *ucBufferDest, u16 uiBufferDestLen, u8 *ucBuffer, u16 uiBufferLen );
void dac1Output( u8* ucBuffer, u32 uiBufferLen, u16 uiSampleRate, u8 uiMode );
void dac2Output( u8* ucBuffer, u32 uiBufferLen, u16 uiSampleRate, u8 uiMode );

#endif // __PYGMY_HEADER_AUDIO
