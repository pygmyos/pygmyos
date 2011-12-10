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

#include "pygmy_profile.h"

// ToDo: Macros must be named following convention and moved to pygmy_lcd.h and profile.h as appropriate

const unsigned char LCD_BITMASKS[] = { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7};

void Pygmy_LCD_Init( void )
{
  unsigned int i;
	
    // Clear register
    // NOTE: CRL is for port pins 0-7, CRH contains 8-15
    // Set Data Pins to 50MHz PushPull Output
    if( pygmyGlobalData.Status & PYGMY_SYS_FILEWRITE )
        return;
    pygmyGlobalData.Status |= PYGMY_SYS_LCDWRITE;
    GPIOC->CRL &= ~( PIN0_CLEAR|PIN1_CLEAR|PIN2_CLEAR|PIN3_CLEAR|PIN4_CLEAR|PIN5_CLEAR|PIN6_CLEAR|PIN7_CLEAR );
    GPIOC->CRL |= ( PIN0_OUT50_GPPUSHPULL | PIN1_OUT50_GPPUSHPULL | PIN2_OUT50_GPPUSHPULL
        | PIN3_OUT50_GPPUSHPULL | PIN4_OUT50_GPPUSHPULL | PIN5_OUT50_GPPUSHPULL 
        | PIN6_OUT50_GPPUSHPULL | PIN7_OUT50_GPPUSHPULL );
    // Set Control Pins to 50MHz PushPull Output
    GPIOC->CRH &= ~(  PIN9_CLEAR | PIN10_CLEAR | PIN11_CLEAR | PIN12_CLEAR );

    GPIOC->CRH |= (  PIN9_OUT50_GPPUSHPULL | PIN10_OUT50_GPPUSHPULL
        | PIN11_OUT50_GPPUSHPULL | PIN12_OUT50_GPPUSHPULL );
    
    
    GPIOC->BSRR = ( LCD_CMD_CS | LCD_CMD_WR | LCD_CMD_A0 | LCD_CMD_RES  );//| LCD_CMD_RD
    GPIOC->BRR = LCD_CMD_RES;
    for( i = 0; i < 100; i++ )
		;
    GPIOC->BSRR = LCD_CMD_RES;
    for( i = 0; i < 1000; i++ )
		;
	Pygmy_LCD_Command_Write(LCD_CMD_ADCNORMAL);
	Pygmy_LCD_Command_Write(LCD_CMD_OFF);
	Pygmy_LCD_Command_Write(LCD_CMD_SCANREVERSE);//(0xC8);
	Pygmy_LCD_Command_Write(LCD_CMD_BIAS17);
	Pygmy_LCD_Command_Write(0x2F);
	Pygmy_LCD_Command_Write(0x26);
	Pygmy_LCD_Command_Write(0x81);
	Pygmy_LCD_Command_Write(0x0C);
	Pygmy_LCD_Command_Write(LCD_CMD_ON);
	Pygmy_LCD_Command_Write( LCD_CMD_ADDRROW );
    pygmyGlobalData.Status &= ~PYGMY_SYS_LCDWRITE;
}


void Pygmy_LCD_Command_Write( unsigned int uiCommand )
{
    GPIOC->ODR &= ~( LCD_CMD_CS | LCD_CMD_A0 | LCD_CMD_WR );
    GPIOC->ODR &= 0xFF00;
    GPIOC->ODR |= ( 0x00FF & uiCommand );
    GPIOC->ODR |= ( LCD_CMD_CS | LCD_CMD_WR );
}

void Pygmy_LCD_Data_Write( unsigned int uiData )
{
    GPIOC->ODR |= LCD_CMD_A0;
    GPIOC->ODR &= ~( LCD_CMD_CS | LCD_CMD_WR );
    GPIOC->ODR &= 0xFF00 ;
    GPIOC->ODR |= ( 0x00FF & uiData );
    GPIOC->ODR |= ( LCD_CMD_CS | LCD_CMD_WR );
}
