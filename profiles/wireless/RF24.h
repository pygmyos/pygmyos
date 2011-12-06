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

// Header for Nordic Semiconductor RF24XXX RF Tranceivers

#pragma once
    
#include "pygmy_sys.h"

#define RF24_NOP                        0xFF 
#define RF24_READ                       0x00 // 5 LSb are Address Bits
#define RF24_WRITE                      0x20 // 5 LSb are Address Bits
#define RF24_RX_PAYLOAD                 0x61 // Followed by 1-32 Data Bytes, LSB First
#define RF24_TX_PAYLOAD                 0xA0 // Followed by 1-32 Data Bytes, LSB First
#define RF24_TX_FLUSH                   0xE1 
#define RF24_RX_FLUSH                   0xE2 // Use Post ACK Only
#define RF24_TX_REUSE                   0xE3 // Reuse Last Transmitted Payload
#define RF24_RX_LEN                     0x60 // Length of Top Packet in FIFO
#define RF24_ACK_PAYLOAD                0xA8 // Payload to Include with ACK, 3 LSb are Pipe Address
#define RF24_TX_PAYLOAD_NACK            0xB0 // Payload With Disabled AutoACK

#define RF24_REG_CONFIG                 0x00 
#define RF24_REG_CONFIG_RX_DR           BIT6
#define RF24_REG_CONFIG_TX_DS           BIT5
#define RF24_REG_CONFIG_MAX_RT          BIT4
#define RF24_REG_CONFIG_EN_CRC          BIT3
#define RF24_REG_CONFIG_CRCO            BIT2
#define RF24_REG_CONFIG_PWR_UP          BIT1
#define RF24_REG_CONFIG_PRIM_RX         BIT0

#define RF24_REG_ENAA                   0x01
#define RF24_REG_ENAA_P5                BIT5
#define RF24_REG_ENAA_P4                BIT4
#define RF24_REG_ENAA_P3                BIT3
#define RF24_REG_ENAA_P2                BIT2
#define RF24_REG_ENAA_P1                BIT1
#define RF24_REG_ENAA_P0                BIT0

#define RF24_REG_ENRXADDR               0x02
#define RF24_REG_ENRXADDR_P5            BIT5
#define RF24_REG_ENRXADDR_P4            BIT4
#define RF24_REG_ENRXADDR_P3            BIT3
#define RF24_REG_ENRXADDR_P2            BIT2
#define RF24_REG_ENRXADDR_P1            BIT1
#define RF24_REG_ENRXADDR_P0            BIT0

#define RF24_REG_SETUP_AW               0x03
#define RF24_REG_SETUP_AW_3BYTE         BIT0
#define RF24_REG_SETUP_AW_4BYTE         BIT1
#define RF24_REG_SETUP_AW_5BYTE         (BIT1|BIT0)

#define RF24_REG_SETUP_RETR             0x04
#define RF24_REG_SETUP_RETR_ARD_250
#define RF24_REG_SETUP_RETR_ARD_500     BIT4
#define RF24_REG_SETUP_RETR_ARD_750     BIT5
#define RF24_REG_SETUP_RETR_ARD_1000    (BIT5|BIT4)
#define RF24_REG_SETUP_RETR_ARD_1250    BIT6
#define RF24_REG_SETUP_RETR_ARD_1500    (BIT6|BIT4)
#define RF24_REG_SETUP_RETR_ARD_1750    (BIT6|BIT5)
#define RF24_REG_SETUP_RETR_ARD_2000    (BIT6|BIT5|BIT4)
#define RF24_REG_SETUP_RETR_ARD_2250    BIT7
#define RF24_REG_SETUP_RETR_ARD_2500    (BIT7|BIT4)
#define RF24_REG_SETUP_RETR_ARD_2750    (BIT7|BIT5)
#define RF24_REG_SETUP_RETR_ARD_3000    (BIT7|BIT5|BIT4)
#define RF24_REG_SETUP_RETR_ARD_3250    (BIT7|BIT6)
#define RF24_REG_SETUP_RETR_ARD_3500    (BIT7|BIT6|BIT4)
#define RF24_REG_SETUP_RETR_ARD_3750    (BIT7|BIT6|BIT5)
#define RF24_REG_SETUP_RETR_ARD_4000    (BIT7|BIT6|BIT5|BIT4)
    
#define RF24_REG_SETUP_RETR_ARC_0       0
#define RF24_REG_SETUP_RETR_ARC_1       1
#define RF24_REG_SETUP_RETR_ARC_2       2
#define RF24_REG_SETUP_RETR_ARC_3       3
#define RF24_REG_SETUP_RETR_ARC_4       4
#define RF24_REG_SETUP_RETR_ARC_5       5
#define RF24_REG_SETUP_RETR_ARC_6       6
#define RF24_REG_SETUP_RETR_ARC_7       7
#define RF24_REG_SETUP_RETR_ARC_8       8
#define RF24_REG_SETUP_RETR_ARC_9       9
#define RF24_REG_SETUP_RETR_ARC_10      10
#define RF24_REG_SETUP_RETR_ARC_11      11
#define RF24_REG_SETUP_RETR_ARC_12      12
#define RF24_REG_SETUP_RETR_ARC_13      13
#define RF24_REG_SETUP_RETR_ARC_14      14
#define RF24_REG_SETUP_RETR_ARC_15      15

#define RF24_REG_RFCH                   0x05

#define RF24_REG_SETUP_RF               0x06
#define RF24_REG_SETUP_RF_CONTWAVE      BIT7
#define RF24_REG_SETUP_RF_DRLOW         BIT5
#define RF24_REG_SETUP_RF_PLLLOCK       BIT4
#define RF24_REG_SETUP_RF_DRHIGH        BIT3
#define RF24_REG_SETUP_RF_PWR_N18DBM    0
#define RF24_REG_SETUP_RF_PWR_N12DBM    BIT1
#define RF24_REG_SETUP_RF_PWR_N6DBM     BIT2
#define RF24_REG_SETUP_RF_PWR_0DBM      (BIT2|BIT1)

#define RF24_REG_STATUS                 0x07
#define RF24_REG_STATUS_RXDR            BIT6
#define RF24_REG_STATUS_TXDS            BIT5
#define RF24_REG_STATUS_MAXRT           BIT4
#define RF24_REG_STATUS_RXPNO_MASK     (BIT3|BIT2|BIT1)
#define RF24_REG_STATUS_TXFULL          BIT0

#define RF24_REG_OBSERVETX              0x08
#define RF24_REG_OBSERVETX_PLOSCNT      0xF0
#define RF24_REG_OBSERVETX_ARCCNT       0x0F

#define RF24_REG_RPD                    0x09
#define RF24_REG_RDP_RPD                BIT0

#define RF24_REG_RXADDR_P0              0x0A
#define RF24_REG_RXADDR_P1              0x0B
#define RF24_REG_RXADDR_P2              0x0C
#define RF24_REG_RXADDR_P3              0x0D
#define RF24_REG_RXADDR_P4              0x0E
#define RF24_REG_RXADDR_P5              0x0F

#define RF24_REG_TXADDR                 0x10
#define RF24_REG_RXPW_P0                0x11
#define RF24_REG_RXPW_P1                0x12
#define RF24_REG_RXPW_P2                0x13
#define RF24_REG_RXPW_P3                0x14
#define RF24_REG_RXPW_P4                0x15
#define RF24_REG_RXPW_P5                0x16

#define RF24_REG_FIFOSTATUS             0x17
#define RF24_REG_FIFOSTATUS_TXREUSE     BIT6
#define RF24_REG_FIFOSTATUS_TXFULL      BIT5
#define RF24_REG_FIFOSTATUS_TXEMPTY     BIT4
#define RF24_REG_FIFOSTATUS_RXFULL      BIT1
#define RF24_REG_FIFOSTATUS_RXEMPTY     BIT0

#define RF24_REG_DYNPD                  0x1C
#define RF24_REG_DYNPD_P5               BIT5
#define RF24_REG_DYNPD_P4               BIT4
#define RF24_REG_DYNPD_P3               BIT3
#define RF24_REG_DYNPD_P2               BIT2
#define RF24_REG_DYNPD_P1               BIT1
#define RF24_REG_DYNPD_P0               BIT0

#define RF24_REG_FEATURE                0x1D
#define RF24_REG_FEATURE_EN_DPL         BIT2
#define RF24_REG_FEATURE_EN_ACK_PAY     BIT1
#define RF24_REG_FEATURE_EN_DYN_ACK     BIT0


u8 Pygmy_RF24_GetSignalQuality( void );
u8 Pygmy_RF24_GetTX( void );
u8 Pygmy_RF24_SetTX( void );
u8 Pygmy_RF24_SetRX( void );
void Pygmy_RF24_PutTXBuffer( u8 ucLen, u8 *ucBuffer );
u8 Pygmy_RF24_GetRXPayloadLen( u8 ucPipe );
u8 Pygmy_RF24_GetRXPayload( u8 *ucBuffer );
void Pygmy_RF24_ConfigAsTX( void );
void Pygmy_RF24_ConfigAsRX( void );
void Pygmy_RF24_ClearStatus( void );
void Pygmy_RF24_WriteAddress( u8 ucReg, u8 *ucAddress );
void Pygmy_RF24_SPI_WriteByte( u8 ucByte );
void Pygmy_RF24_SPI_WriteWord( u16 uiWord );
void Pygmy_RF24_SPI_WriteLong( u32 ulLong );
u8 Pygmy_RF24_SPI_ReadByte( void );
void RF24_TX_Packet( void );
void RF24_RX_Packet( void );

