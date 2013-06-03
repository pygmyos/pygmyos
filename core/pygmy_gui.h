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

#pragma once
//#ifndef __PYGMY_HEADER_GUI
//	#define __PYGMY_HEADER_GUI

#include "pygmy_profile.h"

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

#define PYGMY_VECTOR_MASK               (BIT7|BIT6|BIT5)
#define PYGMY_VECTOR_END                0
#define PYGMY_VECTOR_BRUSH              BIT5
#define PYGMY_VECTOR_POLY               BIT6
#define PYGMY_VECTOR_ARC                (BIT6|BIT5)
#define PYGMY_VECTOR_SPLINE             BIT7
#define PYGMY_VECTOR_TEXT               (BIT7|BIT5)
#define PYGMY_VECTOR_RASTER             (BIT7|BIT6)

// Draw Style Defines
#define VISIBLE           BIT0
#define ACTIVE            BIT1
#define DOT               BIT2
#define DASH              BIT3
#define BORDER            BIT4
#define BORDER3D          BIT5
#define ROUNDED           BIT6
#define CHAMFER           BIT7
#define FILLED            BIT8
#define BLEND             BIT9
#define INVERT            BIT10
#define GRAYED            BIT11
#define BLINK             BIT12
#define WORDWRAP          BIT13
#define LETTERWRAP        BIT14
#define LEFTALIGN         BIT15
#define CENTERED          BIT16
#define RIGHTALIGN        BIT17
#define SCROLL            BIT18
#define CLICKABLE         BIT19
#define CAPTION           BIT20
// Object Style Defines
enum {  TITLE = 1,
        WINDOW,
        CANVAS,
        TABSTRIP,
        BUTTON,
        LABEL,
        TEXTBOX,
        CHECKBOX,
        RADIOBUTTON,
        WHEEL,
        SLIDER,
        VSCROLLBAR,
        HSCROLLBAR,
        PROGRESS,
        ALERT,
        BUSY };
    
enum { BUTTON_LEFT = 1,
       BUTTON_CENTER,
       BUTTON_RIGHT };
    
enum {  MOUSE_MOVE = 1, // Primary
        MOUSE_DOWN,
        MOUSE_UP,
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_CLICK,    // Primary
        MOUSE_OVER,
        GOTFOCUS,
        LOSTFOCUS,
        SELECTION,      // Primary
        SELECTED,
        MESSAGE, 
        DRAW,           // Primary
        CREATED,        // Primary
        DESTROYED };    // Primary

/*      
#define TITLE             BIT19
#define WINDOW            BIT20
#define BUTTON            BIT21
#define TEXT              BIT22
#define SLIDER            BIT23
#define PROGRESS          BIT24
#define CHECK             BIT25
#define RADIO             BIT26
#define TABS              BIT27
#define WHEEL             BIT28
#define ALERT             BIT29
#define BUSY              BIT30
#define TOOLBAR           BIT31
*/
//#define PYGMY_STYLE_BARGRAPH        BIT14
//#define PYGMY_STYLE_LINEGRAPH       BIT15
//#define PYGMY_STYLE_PIEGRAPH        BIT16
//#define PYGMY_STYLE_POINTGRAPH      BIT17
//#define PYGMY_STYLE_TOOLBAR         BIT18
//

typedef struct{
                u16 X;
                u16 Y;
                u16 Status;
                u8 Button;
                } PYGMYCURSOR;

typedef struct{
                u8 R;
                u8 G;
                u8 B;
                u8 A;
                } PYGMYCOLOR;

typedef struct {
                PYGMYCOLOR Color;
                PYGMYCOLOR BackColor;
                PYGMYFILE *File;
                u16 Style;
                u16 Height;
				} PYGMYFONT;
				
typedef struct {
                u16 X;
                u16 Y;
                u16 Width;
                u16 Height;
                } PYGMYCANVAS;

/*            
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
*/
typedef struct{
                PYGMYFONT *SmallFont;
                PYGMYFONT *MediumFont;
                PYGMYFONT *LargeFont;
                PYGMYCOLOR Color;
                PYGMYCOLOR BackColor;
                PYGMYCOLOR AlphaColor;
                u32 Style;
                u8 Radius;
                } PYGMYTHEME;
            
typedef struct{
                PYGMYCOLOR Color;
                PYGMYCOLOR BackColor;
                PYGMYCOLOR FocusColor;
                PYGMYCOLOR FocusBackColor;
                //PYGMYCURSOR Cursor;
                //u32 Style;
                u32 Value;
                u16 X;
                u16 Y;
                u16 Width;
                u16 Height;
                u32 Style;
                u8 Type;
                u8 *String;
                //void *Parent
                PYGMYEVENTFUNC Draw;
                PYGMYEVENTFUNC MouseMove;
                PYGMYEVENTFUNC MouseClick;
                PYGMYEVENTFUNC GotFocus;
                PYGMYEVENTFUNC LostFocus;
                PYGMYEVENTFUNC Selected;
                PYGMYEVENTFUNC Created;
                PYGMYEVENTFUNC Destroyed;
                } PYGMYWIDGET;
//typedef PYGMYWIDGET *PYGMYWIDGETPTR[];
typedef struct{
                PYGMYCOLOR ClearColor;
                PYGMYCOLOR AlphaColor;
                PYGMYCOLOR BackColor;
                PYGMYCOLOR Color;
                PYGMYCOLOR FocusColor;
                PYGMYCOLOR FocusBackColor;
                u16 X;
                u16 Y;
                u16 Width;
                u16 Height;
                u16 Len;
                u16 Selected;
                //u8 Event;
                //u8 EventValue;
                PYGMYCURSOR Mouse;
                PYGMYWIDGET *Widgets;
                PYGMYEVENTFUNC Created;
                PYGMYEVENTFUNC Destroyed;
                PYGMYEVENTFUNC MouseMove;
                PYGMYEVENTFUNC MouseClick;
                PYGMYEVENTFUNC Selection;
                PYGMYEVENTFUNC Draw;
                } PYGMYFORM;
            
typedef struct{
                PYGMYFONT *Font; // Active Font
                PYGMYFONT *SmallFont;
                PYGMYFONT *MediumFont;
                PYGMYFONT *LargeFont;
                PYGMYCOLOR ActiveColor;
                PYGMYCOLOR ClearColor;
                PYGMYCOLOR AlphaColor;
                PYGMYCOLOR BackColor;
                PYGMYCOLOR Color;
                PYGMYCOLOR FocusColor;
                PYGMYCOLOR FocusBackColor;
                PYGMYCURSOR Cursor;
                PYGMYCURSOR Mouse;
                u32 Style;
                u8 Radius;
                u8 Contrast;
                u8 BPP;
                u8 Event;
                u8 EventValue;
                PYGMYEVENTFUNC MouseClickLeft;
                PYGMYEVENTFUNC MouseClickCenter;
                PYGMYEVENTFUNC MouseClickRight;
                PYGMYEVENTFUNC MouseUp;
                PYGMYEVENTFUNC MouseDown;
                PYGMYEVENTFUNC MouseLeft;
                PYGMYEVENTFUNC MouseRight;
                //PYGMYEVENTFUNC KeyStroke;
                } PYGMYGUI;
    
typedef struct {
                u32 Timer;
                u32 Reload;
                u32 TimeStamp;
                u32 Expire;
                u16 *Coords;
                u16 Len;
                u16 ID;
                u16 Index;
                PYGMYFILE File;
                } PYGMYSPRITE;

void drawForms( void ); 
void drawWidget( PYGMYWIDGET *pygmyWidget );
void drawJPEG( PYGMYFILE *pygmyFile, u16 uiX, u16 uiY );            
void drawPNG( PYGMYFILE *pygmyFile, u16 uiX, u16 uiY ); 
     
void colorSet( PYGMYCOLOR *pygmyColor, u8 ucR, u8 ucG, u8 ucB );
void colorCopy( PYGMYCOLOR *colorFrom, PYGMYCOLOR *colorTo );
void colorApply( PYGMYCOLOR *pygmyColor );    

void fontSetColor( PYGMYFONT *pygmyFont, PYGMYCOLOR *pygmyColor );
void fontSetBackColor( PYGMYFONT *pygmyFont, PYGMYCOLOR *pygmyColor );
PYGMYFONT *fontGetActive( void );
PYGMYFONT *fontGetSmall( void );
PYGMYFONT *fontGetMedium( void );
PYGMYFONT *fontGetLarge( void );
void fontSetActive( PYGMYFONT *pygmyFont );
void fontSetSmall( PYGMYFONT *pygmyFont );
void fontSetMedium( PYGMYFONT *pygmyFont );
void fontSetLarge( PYGMYFONT *pygmyFont );
void fontSetAll( PYGMYFONT *pygmyFont );
u8 fontLoad( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont );

void formFreeAll( void );
void formCurrentSetColor( u8 ucR, u8 ucG, u8 ucB );
void formCurrentSetBackColor( u8 ucR, u8 ucG, u8 ucB );
void formCurrentSetAlphaColor( u8 ucR, u8 ucG, u8 ucB );
void formCurrentSetClearColor( u8 ucR, u8 ucG, u8 ucB );            
u8 formNew( u16 uiX, u16 uiY, u16 uiWidth, u16 uiHeight );
void formRemove( void );
void formDrawAll( void );
u8 formAddWidget( PYGMYWIDGET *pygmyWidget );
u16 formGetWidgetCount( void );
void formCallFocused( void );
void formFocusNext( void );
void formFocusPrevious( void );
u8 formSetFocus( u16 uiSelected );
PYGMYWIDGET *widgetGetCurrent( void );
PYGMYWIDGET *widgetGetFocused( void );
PYGMYWIDGET *widgetGet( u8 *ucName );
void widgetRun( PYGMYWIDGET *pygmyWidget );
u8 widgetAddEventHandler( PYGMYWIDGET *pygmyWidget, void *pygmyFunc, u8 ucEvent );

void eventGotFocus( void );
void eventLostFocus( void );
void eventMouseMove( void );
void eventMouseMoveUp( void );
void eventMouseMoveDown( void );
void eventMouseMoveLeft( void );
void eventMouseMoveRight( void );
void eventMouseClickLeft( void );
void eventMouseClickCenter( void );
void eventMouseClickRight( void );
void eventGotFocus( void );
void eventLostFocus( void );
void eventSelected( void );


void guiSetStyle( u32 ulStyle );
u32 guiGetStyle( void );            
void guiSetRadius( u8 ucRadius );
u8 guiGetRadius( u8 ucRadius );            
u8 guiSetFont( PYGMYFILE *pygmyFile, PYGMYFONT *pygmyFont );
void guiSetFonts( PYGMYFONT *fontSmall, PYGMYFONT *fontMedium, PYGMYFONT *fontLarge );
u8 putsLCD( u8 *ucBuffer );
void drawChar( u8 ucChar );
void drawString( u8 *ucBuffer );
void drawCircle( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiRadius, u32 ulStyle );         
void drawSine( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiFrequency, s8 a );
void guiSetCursor( u16 uiX, u16 uiY );
u16 guiGetCursorX( void );
u16 guiGetCursorY( void );
void *guiGetCursor( void );
/*
void guiSetColor( u8 ucR, u8 ucG, u8 ucB );
void guiSetBackColor( u8 ucR, u8 ucG, u8 ucB );
void guiSetAlphaColor( u8 ucR, u8 ucG, u8 ucB );
void guiSetFontColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB );
void guiSetFontBackColor( PYGMYFONT *pygmyFont, u8 ucR, u8 ucG, u8 ucB );
void guiApplyClearColor( void );
*/
/*            
void guiApplyColor( void );
void guiApplyBackColor( void );
void guiApplyFontColor( void );
void guiApplyFontBackColor( void );*/
u16 guiGetCursorX( void );
u16 guiGetCursorY( void );
void *guiGetCursor( void );
void guiClearArea( PYGMYCOLOR *pygmyColor, u16 uiX0, u16 uiY0, u16 uiX1, u16 uiY1 );
void guiClearScreen( PYGMYCOLOR *pygmyColor );
u16 guiGetEntries( PYGMYFILE *pygmyFile );           
u16 guiGetHeader( PYGMYFILE *pygmyFile );
u32 guiGetImage( PYGMYFILE *pygmyFile, u16 uiIndex );
void guiInitSprites( void );
u8 guiDeleteSprite( u16 uiID );
u8 guiRemoveSpriteCoords( u16 uiID, u16 uiX, u16 uiY );
void *guiGetSprite( u16 uiID );
u8 guiCreateSprite( PYGMYFILE *pygmyFile, u16 uiID, u16 *uiCoords, u16 uiLen, u32 ulTimer, u32 ulExpire );
void guiSpriteProcess( void );
u16 drawSprite( PYGMYSPRITE *pygmySprite );
u16 drawImage( u16 uiXPos, u16 uiYPos, PYGMYFILE *pygmyFile, u16 uiIndex );
void drawBitmap( u16 uiX, u16 uiY, PYGMYFILE *fileImage );            
void drawPixel( PYGMYCOLOR *pygmyColor, u16 uiX, u16 uiY ); 
void drawClearPixel( u16 uiX, u16 uiY );
void drawBlendPixel( u16 uiX, u16 uiY );
void drawLine( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle ); 
void drawThickLine( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u8 ucThickness, u32 ulStyle );
void drawPoly( PYGMYCOLOR *pygmyColor, u16 *uiPoints, u16 uiLen, u32 ulStyle );
void drawRect( PYGMYCOLOR *pygmyColor, s16 iX1, s16 iY1, s16 iX2, s16 iY2, u32 ulStyle, u16 uiRadius );
void drawFill( PYGMYCOLOR *pygmyColor, u16 x, u16 y );
void drawAll( u8 *ucBuffer );
u8 guiGetPixelOpening( u16 uiX, u16 uiY, u32 ulColor );
//void drawRadius( u16 uiX0, u16 uiY0, u16 uiSectors, u16 uiRadius );
//void drawRadius( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiRadius );
s16 guiGetStringLen( u8 *ucString );
s16 guiGetArcX( s16 iDegree, s16 iRadius );            
s16 guiGetArcY( s16 iDegree, s16 iRadius );
//void drawRadius( u16 uiX, u16 uiY, u16 uiAngle1, u16 uiAngle2, u16 uiRadius );
void drawRadius( PYGMYCOLOR *pygmyColor, u16 uiCenterX, u16 uiCenterY, u16 uiCorner, u16 uiRadius, u32 ulStyle );
//void drawRadius( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiCenterX, u16 uiCenterY, u16 uiRadius );
//void drawRadius( u16 uiX0, u16 uiY0, u16 uiStart, u16 uiRadius );            
// ToDo: Finish the following functions:
//void drawObject( PYGMYGUIOBJECT *pygmyGUIObject );
void guiOutputBMHeader( PYGMYFILE *pygmyFile, u32 ulWidth, u32 ulHeight, u8 ucBPP );
//void clearVideoArea( u8 *ucBuffer, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2, u16 uiColor );           
//void drawVideoArea( u8 *ucBuffer, u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 );
            
//#endif // __PYGMY_HEADER_GUI
