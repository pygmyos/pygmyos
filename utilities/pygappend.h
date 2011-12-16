/**************************************************************************
    pygappend ( Pygmy Operating System Utilities: Append Image to Imagestrip )
    Copyright (C) 2011  Warren D Greenway

    This file is part of pygappend.

    pygappend is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    pygappend is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with pygappend.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#ifndef __PYGRES_H
	#define __PYGRES_H
	
#define TGAMAXBYTES	65535
#define PIXELON		0x00

#define PIXELOFF	0xFF
#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

#define PYGRES_TYPEMASK				0xF000
#define PYGRES_IMAGE				0x0000
#define PYGRES_IMAGESTRIP			0x1000
#define PYGRES_ANIMATION			0x2000
#define PYGRES_VIDEO				0x3000
#define PYGRES_AUDIO				0x4000
#define PYGRES_BINARY				0x5000
#define PYGRES_TEXT					0x6000
#define PYGRES_EXECUTABLE			0x7000
#define PYGRES_BYTECODE				0x8000
#define PYGRES_LIBRARY				0x9000
#define PYGRES_GUI					0xA000
#define PYGRES_SCRIPT				0xB000
#define PYGRES_DRIVER				0xC000
#define PYGRES_ARCHIVE				0xD000
#define PYGRES_STREAM				0xE000
#define PYGRES_FONT  				0xF000	

// For Image formats
#define PYGRES_TABLEMASK			0x0300
#define PYGRES_TABLE16				0x0000
#define PYGRES_TABLE32				0x0400
#define PYGRES_DIMENSIONSIZEMASK	0x0100
#define PYGRES_8BITD				0x0000
#define PYGRES_16BITD				0x0100
#define PYGRES_DRAWSTYLEMASK		0x00C0
#define PYGRES_RAW					0x0000
#define PYGRES_ALPHA				0x0040
#define PYGRES_SCATTER				0x0080
#define PYGRES_VECTOR				0x00C0
#define PYGRES_CRCMASK				0x0020
#define PYGRES_CRC					0x0020
#define PYGRES_VERSIONMASK			0x0010
#define PYGRES_VERSION				0x0010
#define PYGRES_BPPMASK				0x000F
// End For Image formats
// For Audio Formats
#define PYGRES_SRMASK
#define PYGRES_SR8000               0x0000
#define PYGRES_SR11025              0x0100
#define PYGRES_SR22050              0x0200
#define PYGRES_SR44100              0x0400

// End For Audio Formats
// For All Formats
// Audio Supports 1bpp for Audio Data, 8bpp, 16bpp, 24bpp, 32bpp
#define PYGRES_1BPP					0x0000
#define PYGRES_4BPP					0x0001
#define PYGRES_8BPP					0x0002
#define PYGRES_16BPP				0x0003
#define PYGRES_18BPP				0x0004
#define PYGRES_24BPP				0x0005
#define PYGRES_32BPP				0x0006
// End For All Formats

#endif
