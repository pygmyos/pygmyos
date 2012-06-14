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


//#pragma once
#ifndef __PYGMY_HEADER_RF
	#define __PYGMY_HEADER_RF
#include "pygmy_profile.h"
//#include "pygmy_file.h"

#define RF_NOP                        0xFF 
#define RF_READ                       0x00 // 5 LSb are Address Bits
#define RF_WRITE                      0x20 // 5 LSb are Address Bits
#define RF_RX_PAYLOAD                 0x61 // Followed by 1-32 Data Bytes, LSB First
#define RF_TX_PAYLOAD                 0xA0 // Followed by 1-32 Data Bytes, LSB First
#define RF_TX_FLUSH                   0xE1 
#define RF_RX_FLUSH                   0xE2 // Use Post ACK Only
#define RF_TX_REUSE                   0xE3 // Reuse Last Transmitted Payload
#define RF_RX_LEN                     0x60 // Length of Top Packet in FIFO
#define RF_ACK_PAYLOAD                0xA8 // Payload to Include with ACK, 3 LSb are Pipe Address
#define RF_TX_PAYLOAD_NACK            0xB0 // Payload With Disabled AutoACK

#define RF_REG_CONFIG                 0x00 
#define RF_REG_CONFIG_RX_DR           BIT6
#define RF_REG_CONFIG_TX_DS           BIT5
#define RF_REG_CONFIG_MAX_RT          BIT4
#define RF_REG_CONFIG_EN_CRC          BIT3
#define RF_REG_CONFIG_CRCO            BIT2
#define RF_REG_CONFIG_PWR_UP          BIT1
#define RF_REG_CONFIG_PRIM_RX         BIT0

#define RF_REG_ENAA                   0x01
#define RF_REG_ENAA_P5                BIT5
#define RF_REG_ENAA_P4                BIT4
#define RF_REG_ENAA_P3                BIT3
#define RF_REG_ENAA_P2                BIT2
#define RF_REG_ENAA_P1                BIT1
#define RF_REG_ENAA_P0                BIT0

#define RF_REG_ENRXADDR               0x02
#define RF_REG_ENRXADDR_P5            BIT5
#define RF_REG_ENRXADDR_P4            BIT4
#define RF_REG_ENRXADDR_P3            BIT3
#define RF_REG_ENRXADDR_P2            BIT2
#define RF_REG_ENRXADDR_P1            BIT1
#define RF_REG_ENRXADDR_P0            BIT0

#define RF_REG_SETUP_AW               0x03
#define RF_REG_SETUP_AW_3BYTE         BIT0
#define RF_REG_SETUP_AW_4BYTE         BIT1
#define RF_REG_SETUP_AW_5BYTE         (BIT1|BIT0)

#define RF_REG_SETUP_RETR             0x04
#define RF_REG_SETUP_RETR_ARD_250
#define RF_REG_SETUP_RETR_ARD_500     BIT4
#define RF_REG_SETUP_RETR_ARD_750     BIT5
#define RF_REG_SETUP_RETR_ARD_1000    (BIT5|BIT4)
#define RF_REG_SETUP_RETR_ARD_1250    BIT6
#define RF_REG_SETUP_RETR_ARD_1500    (BIT6|BIT4)
#define RF_REG_SETUP_RETR_ARD_1750    (BIT6|BIT5)
#define RF_REG_SETUP_RETR_ARD_2000    (BIT6|BIT5|BIT4)
#define RF_REG_SETUP_RETR_ARD_2250    BIT7
#define RF_REG_SETUP_RETR_ARD_2500    (BIT7|BIT4)
#define RF_REG_SETUP_RETR_ARD_2750    (BIT7|BIT5)
#define RF_REG_SETUP_RETR_ARD_3000    (BIT7|BIT5|BIT4)
#define RF_REG_SETUP_RETR_ARD_3250    (BIT7|BIT6)
#define RF_REG_SETUP_RETR_ARD_3500    (BIT7|BIT6|BIT4)
#define RF_REG_SETUP_RETR_ARD_3750    (BIT7|BIT6|BIT5)
#define RF_REG_SETUP_RETR_ARD_4000    (BIT7|BIT6|BIT5|BIT4)
    
#define RF_REG_SETUP_RETR_ARC_0       0
#define RF_REG_SETUP_RETR_ARC_1       1
#define RF_REG_SETUP_RETR_ARC_2       2
#define RF_REG_SETUP_RETR_ARC_3       3
#define RF_REG_SETUP_RETR_ARC_4       4
#define RF_REG_SETUP_RETR_ARC_5       5
#define RF_REG_SETUP_RETR_ARC_6       6
#define RF_REG_SETUP_RETR_ARC_7       7
#define RF_REG_SETUP_RETR_ARC_8       8
#define RF_REG_SETUP_RETR_ARC_9       9
#define RF_REG_SETUP_RETR_ARC_10      10
#define RF_REG_SETUP_RETR_ARC_11      11
#define RF_REG_SETUP_RETR_ARC_12      12
#define RF_REG_SETUP_RETR_ARC_13      13
#define RF_REG_SETUP_RETR_ARC_14      14
#define RF_REG_SETUP_RETR_ARC_15      15

#define RF_REG_RFCH                   0x05

#define RF_REG_SETUP_RF               0x06
#define RF_REG_SETUP_RF_CONTWAVE      BIT7
#define RF_REG_SETUP_RF_DRLOW         BIT5
#define RF_REG_SETUP_RF_PLLLOCK       BIT4
#define RF_REG_SETUP_RF_DRHIGH        BIT3
#define RF_REG_SETUP_RF_PWR_N18DBM    0
#define RF_REG_SETUP_RF_PWR_N12DBM    BIT1
#define RF_REG_SETUP_RF_PWR_N6DBM     BIT2
#define RF_REG_SETUP_RF_PWR_0DBM      (BIT2|BIT1)

#define RF_REG_STATUS                 0x07
#define RF_REG_STATUS_RXDR            BIT6
#define RF_REG_STATUS_TXDS            BIT5
#define RF_REG_STATUS_MAXRT           BIT4
#define RF_REG_STATUS_RXPNO_MASK     (BIT3|BIT2|BIT1)
#define RF_REG_STATUS_TXFULL          BIT0

#define RF_REG_OBSERVETX              0x08
#define RF_REG_OBSERVETX_PLOSCNT      0xF0
#define RF_REG_OBSERVETX_ARCCNT       0x0F

#define RF_REG_RPD                    0x09
#define RF_REG_RDP_RPD                BIT0

#define RF_REG_RXADDR_P0              0x0A
#define RF_REG_RXADDR_P1              0x0B
#define RF_REG_RXADDR_P2              0x0C
#define RF_REG_RXADDR_P3              0x0D
#define RF_REG_RXADDR_P4              0x0E
#define RF_REG_RXADDR_P5              0x0F

#define RF_REG_TXADDR                 0x10
#define RF_REG_RXPW_P0                0x11
#define RF_REG_RXPW_P1                0x12
#define RF_REG_RXPW_P2                0x13
#define RF_REG_RXPW_P3                0x14
#define RF_REG_RXPW_P4                0x15
#define RF_REG_RXPW_P5                0x16

#define RF_REG_FIFOSTATUS             0x17
#define RF_REG_FIFOSTATUS_TXREUSE     BIT6
#define RF_REG_FIFOSTATUS_TXFULL      BIT5
#define RF_REG_FIFOSTATUS_TXEMPTY     BIT4
#define RF_REG_FIFOSTATUS_RXFULL      BIT1
#define RF_REG_FIFOSTATUS_RXEMPTY     BIT0

#define RF_REG_DYNPD                  0x1C
#define RF_REG_DYNPD_P5               BIT5
#define RF_REG_DYNPD_P4               BIT4
#define RF_REG_DYNPD_P3               BIT3
#define RF_REG_DYNPD_P2               BIT2
#define RF_REG_DYNPD_P1               BIT1
#define RF_REG_DYNPD_P0               BIT0

#define RF_REG_FEATURE                0x1D
#define RF_REG_FEATURE_EN_DPL         BIT2
#define RF_REG_FEATURE_EN_ACK_PAY     BIT1
#define RF_REG_FEATURE_EN_DYN_ACK     BIT0


/*
#define RF_MARK              0
#define RF_HELLO             1
#define RF_WELCOME           2
#define RF_DECLINE           3
#define RF_ACK               4
#define RF_NACK              5
#define RF_BYE               6
#define RF_QUERY             7
#define RF_DATA              8
#define RF_CLASS0            0
#define RF_CLASS1            BIT6
#define RF_CLASS2            BIT7
#define RF_CLASS3            (BIT7|BIT6)
*/

//#define RF_MAXSOCKETS       32

enum{   RF_250KBPS = 1,
        RF_1MBPS,
        RF_2MBPS
    };


void rfRX( void );
void rfSetTXPower( u8 ucDataRate, u8 ucPower );
void rfFlushTX( void );
void rfFlushRX( void );
u8 rfGetSignalQuality( void );
u8 rfGetStatus( void );
u8 rfGetTX( void );
u8 rfSetTX( void );
u8 rfSetRX( void );
//void rfPutTXBuffer( u16 uiLen, u8 *ucBuffer );
void rfPutTXBuffer( u8 *ucBuffer, u16 uiLen );
void rfPutString( u8 *ucBuffer );
u8 rfGetRXPayloadLen( void );
void rfGetRXPayload( void );
void rfConfigAsTX( void );
void rfConfigAsRX( void );
void rfClearStatus( void );
void rfWriteAddress( u8 ucReg, u8 *ucAddress );
//void rfSPIWriteByte( u8 ucByte );
//void rfSPIWriteWord( u16 uiWord );
//void rfSPIWriteLong( u32 ulLong );
//u8 rfSPIReadByte( void );
void rfTXPacket( void );
void rfRXPacket( void );

u8 rfIsRXFIFOEmpty( void );


//extern PYGMYCOMMANDQUEUE pygmyGlobaRFQueue;
//extern PYGMYRFSTACK pygmyGlobalStack;
//extern PYGMYRFSOCKET pygmyGlobalSockets[];
extern const PYGMYCMD PYGMYCOMMANDS[];

#endif // __PYGMY_HEADER_RF
