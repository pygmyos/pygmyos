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

#include "pygmy_profile.h"
//#include "pygmy_file.h"

#define PYGMY_PBM_TYPEMASK				0xF000
#define PYGMY_PBM_IMAGE				    0x0000
#define PYGMY_PBM_IMAGESTRIP			0x1000
#define PYGMY_PBM_ANIMATION			    0x2000
#define PYGMY_PBM_VIDEO				    0x3000
#define PYGMY_PBM_AUDIO				    0x4000
#define PYGMY_PBM_BINARY				0x5000
#define PYGMY_PBM_TEXT					0x6000
#define PYGMY_PBM_EXECUTABLE			0x7000
#define PYGMY_PBM_BYTECODE				0x8000
#define PYGMY_PBM_LIBRARY				0x9000
#define PYGMY_PBM_GUI					0xA000
#define PYGMY_PBM_SCRIPT				0xB000
#define PYGMY_PBM_DRIVER				0xC000
#define PYGMY_PBM_ARCHIVE				0xD000
#define PYGMY_PBM_STREAM				0xE000
#define PYGMY_PBM_FONT  				0xF000	

#define PYGMY_PBM_TABLEMASK			    0x0300
#define PYGMY_PBM_TABLE16				0x0000
#define PYGMY_PBM_TABLE32				0x0400
#define PYGMY_PBM_DIMENSIONSIZEMASK	    0x0100
#define PYGMY_PBM_8BITD				    0x0000
#define PYGMY_PBM_16BITD				0x0100
#define PYGMY_PBM_DRAWSTYLEMASK		    0x00C0
#define PYGMY_PBM_RAW					0x0000
#define PYGMY_PBM_ALPHA				    0x0040
#define PYGMY_PBM_SCATTER				0x0080
#define PYGMY_PBM_VECTOR				0x00C0
#define PYGMY_PBM_CRCMASK				0x0020
#define PYGMY_PBM_CRC					0x0020
#define PYGMY_PBM_VERSIONMASK			0x0010
#define PYGMY_PBM_VERSION				0x0010
#define PYGMY_PBM_BPPMASK				0x000F
#define PYGMY_PBM_1BPP					0x0000
#define PYGMY_PBM_4BPP					0x0001
#define PYGMY_PBM_8BPP					0x0002
#define PYGMY_PBM_12BPP                 0x0003
#define PYGMY_PBM_16BPP				    0x0004
#define PYGMY_PBM_24BPP				    0x0005
#define PYGMY_PBM_32BPP				    0x0006

// Draw Style Defines
#define PYGMY_VISIBLE           BIT0
#define PYGMY_ACTIVE            BIT1
#define PYGMY_DOT               BIT2
#define PYGMY_DASH              BIT3
#define PYGMY_BORDER            BIT4
#define PYGMY_3D                BIT5
#define PYGMY_ROUNDED           BIT6
#define PYGMY_CHAMFER           BIT7
#define PYGMY_FILLED            BIT8
#define PYGMY_BLEND             BIT9
#define PYGMY_INVERT            BIT10
#define PYGMY_GRAYED            BIT11
#define PYGMY_BLINK             BIT12
#define PYGMY_WORDWRAP          BIT13
#define PYGMY_LETTERWRAP        BIT14
#define PYGMY_LEFTALIGN         BIT15
#define PYGMY_CENTERED          BIT16
#define PYGMY_RIGHTALIGN        BIT17
#define PYGMY_SCROLL            BIT18
// Object Style Defines
#define PYGMY_TITLE             BIT19
#define PYGMY_WINDOW            BIT20
#define PYGMY_BUTTON            BIT21
#define PYGMY_TEXT              BIT22
#define PYGMY_SLIDER            BIT23
#define PYGMY_PROGRESS          BIT24
#define PYGMY_CHECK             BIT25
#define PYGMY_RADIO             BIT26
#define PYGMY_TABS              BIT27
#define PYGMY_WHEEL             BIT28
#define PYGMY_ALERT             BIT29
#define PYGMY_BUSY              BIT30
#define PYGMY_TOOLBAR           BIT31
//#define PYGMY_STYLE_BARGRAPH        BIT14
//#define PYGMY_STYLE_LINEGRAPH       BIT15
//#define PYGMY_STYLE_PIEGRAPH        BIT16
//#define PYGMY_STYLE_POINTGRAPH      BIT17
//#define PYGMY_STYLE_TOOLBAR         BIT18
//

typedef struct{
                u16 X;
                u16 Y;
                u16 Properties;
                } PYGMYCURSOR;

typedef struct{
                u8 R;
                u8 G;
                u8 B;
                } PYGMYCOLOR;

typedef struct {
                //u16 uiColor;
                //u16 uiBackColor;
                PYGMYCOLOR Color;
                PYGMYCOLOR BackColor;
                PYGMYFILE File;
                u16 Style;
                u16 Height;
                
				//u8 *ucpFont;
				//u8 ucName[14];
				} PYGMYFONT;
				
typedef struct {
                u16 uiX1;
                u16 uiY1;
                u16 uiX2;
                u16 uiY2;
                } PYGMYGUICANVAS;
            
typedef struct {
                u16             X1;
                u16             X2;
                u16             Y1;
                u16             Y2;
                u16             Style;
                u16             Type;
                PYGMYFONT       *Font;
                u8              *Text;
                u32             Value;
                //PYGMYFUNCPTR    *Parent;
                //void            *Action;
                //void            *Draw;
                
                } PYGMYGUIOBJECT;

typedef struct{
                PYGMYCOLOR Color;
                PYGMYCOLOR BackColor;
                PYGMYCURSOR Cursor;
                u32 Style;
                u32 Value;
                u16 X;
                u16 Y;
                u16 Width;
                u16 Height;
                
                u8 *String;
                void *Parent;
                } PYGMYWIDGET;
            
typedef struct{
                PYGMYCOLOR BackColor;
                PYGMYCOLOR Color;
                PYGMYCURSOR Cursor;
                PYGMYFONT *Font;
                u8 Contrast;
                u8 BPP;
                } PYGMYGUI;
            
u8 setFont( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont );
void drawChar( u8 ucChar );
void drawString( u8 *ucBuffer );
void drawCircle( u16 uiX0, u16 uiY0, u16 uiRadius, u32 ulStyle );         
void drawSine( u16 uiX0, u16 uiY0, u16 uiFrequency, s8 a );
u32 getImage( PYGMYFILE *pygmyFile, u16 uiIndex );
u16 drawImage( u16 uiXPos, u16 uiYPos, PYGMYFILE *pygmyFile, u16 uiIndex );
void drawBitmap( u16 uiX, u16 uiY, PYGMYFILE *fileImage );            
void drawPixel( u16 uiX, u16 uiY ); //, u16 uiColor );
void drawClearPixel( u16 uiX, u16 uiY );
void drawBlendPixel( u16 uiX, u16 uiY );
void drawLine( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle ); 
void drawRect( s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle, u16 uiRadius );
void drawFill( u16 x, u16 y );
u8 guiGetPixelOpening( u16 uiX, u16 uiY, u32 ulColor );
//void drawRadius( u16 uiX0, u16 uiY0, u16 uiSectors, u16 uiRadius );
//void drawRadius( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiRadius );
s16 guiGetStringLen( u8 *ucString );
s16 guiGetArcX( s16 iDegree, s16 iRadius );            
s16 guiGetArcY( s16 iDegree, s16 iRadius );
//void drawRadius( u16 uiX, u16 uiY, u16 uiAngle1, u16 uiAngle2, u16 uiRadius );
void drawRadius( u16 uiCenterX, u16 uiCenterY, u16 uiCorner, u16 uiRadius, u32 ulStyle );
//void drawRadius( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiCenterX, u16 uiCenterY, u16 uiRadius );
//void drawRadius( u16 uiX0, u16 uiY0, u16 uiStart, u16 uiRadius );            
// ToDo: Finish the following functions:
void drawObject( PYGMYGUIOBJECT *pygmyGUIObject );
void guiOutputBMHeader( PYGMYFILE *pygmyFile, u32 ulWidth, u32 ulHeight, u8 ucBPP );
//void clearVideoArea( u8 *ucBuffer, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiColor );           
//void drawVideoArea( u8 *ucBuffer, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
            
