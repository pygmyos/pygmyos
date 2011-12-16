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

extern const u8 LCD_BITMASKS[];
PYGMYGUI pygmyGlobalColors;
// !!! Note, LCD_CS, LCD_RESET, LCD_SCK_, LCD_MISO, LCD_MOSI and LCD_SLEEP
// !!!   must be defined in the local copy of pygmy_profile.h

/*#define EI0     LCD_CS
#define DI0     LCD_MOSI
#define XCK     LCD_SCK
#define SLPB    LCD_SLEEP
#define LATCH   LCD_LATCH*/

#define DI0     PA0
#define SLPB    PA1
#define LATCH   PA2
#define XCK     PA3
#define EIO     PA4

char aChar[16] = {1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
char bChar[16] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1};
char cChar[16] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
char dChar[16] = {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1};
char eChar[16] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};
char fChar[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
char gChar[16] = {1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
char hChar[16] = {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0};
char iChar[16] = {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1};
char jChar[16] = {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
char kChar[16] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0};
char lChar[16] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1};
char mChar[16] = {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0};
char nChar[16] = {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0};
char oChar[16] = {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
char pChar[16] = {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
char qChar[16] = {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
char rChar[16] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0};
char sChar[16] = {1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1};
char tChar[16] = {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0};
char uChar[16] = {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1};
char vChar[16] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0};
char wChar[16] = {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0};
char xChar[16] = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
char yChar[16] = {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
char zChar[16] = {1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1};
char zeroChar[16] = {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
char oneChar[16] = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char twoChar[16] = {1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1};
char threeChar[16] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1};
char fourChar[16] = {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
char fiveChar[16] = {1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1};
char sixChar[16] = {1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};
char sevenChar[16] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
char eightChar[16] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};
char nineChar[16] = {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1};

char topData[160] = {
//a,b,c, d, e, f, g, h, i, j, k, l, m, n, o, p
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D1
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D2
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D3
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D4
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D5
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D6
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D7
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D8
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D9
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // D10

char bottomData[160] = {
//a,b,c, d, e, f, g, h, i, j, k, l, m, n, o, p
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D1
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D2
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D3
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D4
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D5
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D6
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D7
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D8
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D9
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // D10

void flipData(char * characterData)
{
    u16 i;
  char tempData[160];
  
  for ( i=0; i<160; i++)
    tempData[i] = characterData[i];
    
  for ( i=0; i<160; i++)
    characterData[i] = tempData[159-i];
}

/* void createData(char * characterData, char * toDisplay)
  This function turns ASCII characters into data that is readable by the display.
  
  There are no return values. The data on char * toDisplay is transformed into
  the corresponding 1's and 0's for the display to understand, and char * characterData
  is updated to reflect that data. toDisplay will not be affected by this functione.
  
  If you want to add more displayable characters to this code, you'll need to add another
  case statment to the large switch. You should be able to pattern the statment after the
  other cases.
*/
void createData(char * characterData, char * toDisplay)
{
    u16 i, j;

  for (i=0; i<10; i++)
  {
    switch (toDisplay[i])
    {
      case ' ':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = 0;
          break;
      case 'A':
      case 'a':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = aChar[j];
        break;
      case 'B':
      case 'b':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = bChar[j];
        break;
      case 'C':
      case 'c':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = cChar[j];
        break;
      case 'D':
      case 'd':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = dChar[j];
        break;
      case 'E':
      case 'e':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = eChar[j];
        break;
      case 'F':
      case 'f':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = fChar[j];
        break;
      case 'G':
      case 'g':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = gChar[j];
        break;
      case 'H':
      case 'h':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = hChar[j];
        break;
      case 'I':
      case 'i':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = iChar[j];
        break;
      case 'J':
      case 'j':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = jChar[j];
        break;
      case 'K':
      case 'k':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = kChar[j];
        break;
      case 'L':
      case 'l':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = lChar[j];
        break;
      case 'M':
      case 'm':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = mChar[j];
        break;
      case 'N':
      case 'n':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = nChar[j];
        break;
      case 'O':
      case 'o':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = oChar[j];
        break;
      case 'P':
      case 'p':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = pChar[j];
        break;
      case 'Q':
      case 'q':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = qChar[j];
        break;
      case 'R':
      case 'r':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = rChar[j];
        break;
      case 'S':
      case 's':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = sChar[j];
        break;
      case 'T':
      case 't':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = tChar[j];
        break;
      case 'U':
      case 'u':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = uChar[j];
        break;
      case 'V':
      case 'v':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = vChar[j];
        break;
      case 'W':
      case 'w':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = wChar[j];
        break;
      case 'X':
      case 'x':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = xChar[j];
        break;
      case 'Y':
      case 'y':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = yChar[j];
        break;
      case 'Z':
      case 'z':
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = zChar[j];
        break;
      case '0':
      case 0:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = zeroChar[j];
        break;
      case '1':
      case 1:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = oneChar[j];
        break;
      case '2':
      case 2:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = twoChar[j];
        break;
      case '3':
      case 3:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = threeChar[j];
        break;
      case '4':
      case 4:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = fourChar[j];
        break;
      case '5':
      case 5:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = fiveChar[j];
        break;
      case '6':
      case 6:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = sixChar[j];
        break;
      case '7':
      case 7:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = sevenChar[j];
        break;
      case '8':
      case 8:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = eightChar[j];
        break;
      case '9':
      case 9:
        for ( j=0; j<16; j++)
          characterData[(9-i)*16 + j] = nineChar[j];
        break;
        
    }
  }
}

void lcdInit( void )
{
    pinConfig( DI0, OUT );
    pinConfig( SLPB, OUT ); 
    pinConfig( LATCH, OUT );
    pinConfig( XCK, OUT );
    pinConfig( EIO, OUT );

    setPin( SLPB, HIGH );      // Sleep high turns the display on
    setPin( DI0, HIGH );         // Initialize data high
    setPin( XCK, LOW );
    setPin( EIO, HIGH );
    setPin( LATCH, LOW );
    createData (topData, "Hello" );
    createData( bottomData, "    World" );
    flipData( topData );
    lcdWriteData( 0, 0 );
}

void lcdClear( void )
{

}

void lcdWriteCommand( u16 uiCommand )
{

}

void lcdWriteData( u16 uiData )
{
    u16 i;

    setPin( EIO, LOW );
    if ( pygmyGlobalColors.BackColor.R ){
        setPin( DI0, HIGH );
    } else{
        setPin( DI0, LOW );
    } // else
    delay( 1 );
    setPin( XCK, HIGH );
    delay( 1 );
    setPin( XCK, LOW );
    delay( 1 );
    setPin( EIO, HIGH );

    for( i = 0; i < 160; i++ ){
        if ( topData[i] ){ // was bottom
            setPin( DI0, LOW );
        } else{
            setPin(DI0, HIGH);
        } // else
        delay( 1 );
        setPin( XCK, HIGH );
        delay( 1 );
        setPin( XCK, LOW );
        delay( 1 );
    } // for
    setPin( DI0, HIGH );
  
    setPin( XCK, HIGH );
    delay( 1 );
    setPin( XCK, LOW );
    delay( 1 ); 
  
  //--- 2nd display ---
  /*
  if (bw)
    digitalWrite(DI0pin, HIGH);
  else
    digitalWrite(DI0pin, LOW);
  
  clock();     // Y0

  for(int i = 0; i<160; i++)
  {
    if (displayTop[i])
      digitalWrite(DI0pin, LOW);
    else
      digitalWrite(DI0pin, HIGH);
    delayMicroseconds(1);
    clock();
  }
  if (com)
    digitalWrite(DI0pin, HIGH);
  else
    digitalWrite(DI0pin, LOW);
  delayMicroseconds(1);
  clock();     // Y161  
  */
    setPin( LATCH, HIGH );
    delay( 5 );
    setPin( LATCH, LOW );
    delay( 5 );
}
/*
// clock(): Simple function to advance the XCK line one period
void clock()
{
  digitalWrite(XCKpin, HIGH);
  delayMicroseconds(1);
  digitalWrite(XCKpin, LOW);
  delayMicroseconds(1);
}

// latch(): Simple function to activate and deactivate the latch
void latch()
{
  digitalWrite(LATCHpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(LATCHpin, LOW);
  delayMicroseconds(5);
}*/


void lcdDrawPixel( u16 uiX, u16 uiY )
{
    
}

u16 lcdGetCursorX( void )
{

}

u16 lcdGetCursorY( void )
{

}

void lcdSetCursor( u16 uiX, u16 uiY )
{

}

void lcdSetBPP( u8 ucBPP )
{

}

void lcdSetColor( u8 ucR, u8 ucG, u8 ucB )
{

}

void lcdBackColor( u8 ucR, u8 ucG, u8 ucB )
{

}

void lcdClearArea( u16 uiX1, u16 uiY1, u16 uiX2, u16 uiY2 )
{

}

