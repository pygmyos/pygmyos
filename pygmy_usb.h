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

#include "pygmy_sys.h" 

// Typedefs 
typedef struct {
                vu32 EPR[ 8 ];
                u32  RESERVED[ 32 ];
                vu32 CNTR;
                vu32 ISTR;
                vu32 FNR;
                vu32 DADDR;
                vu32 BTABLE;
                } USB_TYPEDEF;

typedef struct {
                u32 TXAddress;
                u32 TXCount;
                u32 RXAddress;
                u32 RXCount;
                } ENDPOINT;
            
typedef struct {
                u8 Signature[ 4 ];
                u8 Tag[ 4 ];
                u8 DataLength[ 4 ];
                u8 Flags;
                u8 LogicalUnitNumber;
                u8 CommandLength;
                u8 Command[ 16 ];
                } SCSICBW;
            
typedef struct {
                u8 Signature[ 4 ];
                u8 Tag[ 4 ];
                u8 DataResidue[ 4 ];
                u8 Status;
                } SCSICSW;
// End Typedefs

#ifndef APB1PERIPH_BASE
    #ifndef PERIPH_BASE
        #define PERIPH_BASE           ((u32)0x40000000)
    #endif
    #define APB1PERIPH_BASE       PERIPH_BASE
#endif    
#ifndef USB_BASE            
    #define USB_BASE  (APB1PERIPH_BASE+0x00005C00)  
#endif
#ifndef PMA_BASE
    #define PMA_BASE  (APB1PERIPH_BASE+0x00006000)
#endif
#ifndef USB
    #define USB ((USB_TYPEDEF *)USB_BASE)
#endif
// CBW Flags
// DIR = 0, Data Out, Host - > STM32
// DIR = 1, Data In, STM32 -> Host
#define CBW_DIR                     BIT7
// End CBW Flags            
// CSW Flags
#define CSW_COMMAND_PASS            0x00
#define CSW_COMMAND_FAIL            0x01
#define CSW_ERROR_PHASE             0x02
// End CSW Flags   
// SCSI Op Codes
#define SCSI_INQUIRY                0x12
#define SCSI_READ_FORMATCAPACITIES  0x23
#define SCSI_SENSE_MODE             0x1A
#define SCSI_REMOVAL                0x1E
#define SCSI_READ                   0x28
#define SCSI_READ_CAPACITY          0x25
#define SCSI_SENSE_REQUEST          0x03
#define SCSI_UNIT_STARTSTOP         0x1B
#define SCSI_UNIT_READY             0x00
#define SCSI_VERIFY                 0x2F
#define SCSI_WRITE                  0x2A
// End SCSI Op Codes            
// USB Control Register
#define USB_CNTR_CTRM               BIT15
#define USB_CNTR_PMAORVM            BIT14
#define USB_CNTR_ERRM               BIT13
#define USB_CNTR_WKUPM              BIT12
#define USB_CNTR_SUSPM              BIT11
#define USB_CNTR_RESETM             BIT10
#define USB_CNTR_ESOFM              BIT9
#define USB_CNTR_RESUME             BIT4
#define USB_CNTR_FSUSP              BIT3
#define USB_CNTR_LPMODE             BIT2
#define USB_CNTR_PDWN               BIT1
#define USB_CNTR_FRES               BIT0
// End USB Control Register
// USB Interrupt Status Register
#define USB_ISTR_CTR                BIT15
#define USB_ISTR_PMAOVR             BIT14
#define USB_ISTR_ERR                BIT13
#define USB_ISTR_WKUP               BIT12
#define USB_ISTR_SUSP               BIT11
#define USB_ISTR_RESET              BIT10
#define USB_ISTR_SOF                BIT9
#define USB_ISTR_ESOF               BIT8
#define USB_ISTR_DIR                BIT4
#define USB_ISTR_EPID0              0
#define USB_ISTR_EPID1              1
#define USB_ISTR_EPID2              2
#define USB_ISTR_EPID3              3
#define USB_ISTR_EPID4              4
#define USB_ISTR_EPID5              5
#define USB_ISTR_EPID6              6
#define USB_ISTR_EPID7              7
#define USB_ISTR_EPID8              8
#define USB_ISTR_EPID9              9
#define USB_ISTR_EPID10             10
#define USB_ISTR_EPID11             11
#define USB_ISTR_EPID12             12
#define USB_ISTR_EPID13             13
#define USB_ISTR_EPID14             14
#define USB_ISTR_EPID15             15
//#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
//                | CNTR_ESOFM | CNTR_RESETM )

// USB Interrupt Status Register
// USB Frame Number Register
#define USB_FNR_RXDP                BIT15
#define USB_FNR_RXDM                BIT14
#define USB_FNR_LCK                 BIT13
#define USB_FNR_LSOF_MASK           (BIT12|BIT11)
#define USB_FNR_LSOF0               0
#define USB_FNR_LSOF1               1
#define USB_FNR_LSOF2               2
#define USB_FNR_LSOF                3
#define USB_FNR_FN_MASK             (BIT10|BIT9|BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
// End USB Frame Number Register
// USB Device Address Register
#define USB_DADDR_EF                BIT7
#define USB_DADDR_ADDR_MASK         (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
// End USB Device Address Register
// USB Endpoint Registers
#define USB_EPR_CTR_RX              BIT15
#define USB_EPR_DTOG_RX             BIT14
#define USB_EPR_STAT_RX_MASK        (BIT13|BIT12)
#define USB_EPR_STAT_RX_DISABLED    0
#define USB_EPR_STAT_RX_STALL       BIT12
#define USB_EPR_STAT_RX_NAK         BIT13
#define USB_EPR_STAT_RX_VALID       (BIT13|BIT12)
#define USB_EPR_SETUP               BIT11
#define USB_EPR_EPTYPE_MASK         (BIT10|BIT9)
#define USB_EPR_EPTYPE_BULK         0
#define USB_EPR_EPTYPE_CONTROL      BIT9
#define USB_EPR_EPTYPE_ISO          BIT10
#define USB_EPR_EPTYPE_INTERRUPT    (BIT10|BIT9)
#define USB_EPR_EPKIND              BIT8
#define USB_EPR_CTR_TX              BIT7
#define USB_EPR_DTOG_TX             BIT6
#define USB_EPR_STAT_TX_MASK        (BIT5|BIT4)
#define USB_EPR_STAT_TX_DISABLED    0
#define USB_EPR_STAT_TX_STALL       BIT4
#define USB_EPR_STAT_TX_NAK         BIT5
#define USB_EPR_STAT_TX_VALID       (BIT5|BIT4)
#define USB_EPR_EA_MASK             (BIT3|BIT2|BIT1|BIT0)

// End USB Endpoint Registers

#define USB_GET_STATUS              0x00
#define USB_CLEAR_FEATURE           0x01
#define USB_SET_FEATURE             0x03
#define USB_SET_ADDRESS             0x05
#define USB_GET_DESCRIPTOR          0x06
#define USB_GET_CONFIGURATION       0x08
#define USB_SET_CONFIGURATION       0x09
#define USB_GET_INTERFACE           0x0A
#define USB_SET_INTERFACE           0x0B


#define  USB_COUNT7_RX_1_BLSIZE_1            ((uint32_t)0x80000000)

#define REQUEST_TYPE      0x60  // Mask to get request type
#define STANDARD_REQUEST  0x00  // Standard request
#define CLASS_REQUEST     0x20  // Class request
#define VENDOR_REQUEST    0x40  // Vendor request

#define RECIPIENT         0x1F  // Mask to get recipient

#define RX_FIFO_SIZE                        128
#define TX0_FIFO_SIZE                       64
#define TX1_FIFO_SIZE                       64
#define TX2_FIFO_SIZE                       16
#define TX3_FIFO_SIZE                       16

#define ENDP0_TXADDR                        0x58
#define ENDP0_RXADDR                        0x18
#define ENDP1_TXADDR                        0x98
#define ENDP2_RXADDR                        0xD8


void usbSetEndpoint( u8 EP, u32 TXAddress, u32 TXCount, u32 RXAddress, u32 RXCount );
void usbReset( void );
void usbInit( void );
u8 usbRead( u8 *ucBuffer );
u8 usbWrite( u8 *ucBuffer );

