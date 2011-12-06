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
#include "pygmy_usb.h"

// Steps to reset USB
// 1. Configure endpoint 0 as the default control endpoint
// 2. Configure the endpoint 0 Rx and Tx count and buffer addresses in the BTABLE (usb_conf.h file)
// 3. Configure the endpoint Rx status as VALID and the Tx status as NAK.
// 4. Configure endpoint 1 as bulk IN
// 5. Configure the endpoint 1 Tx count and data buffer address in the BTABLE (usb_conf.h file)
// 6. Disable the endpoint 1 Rx
// 7. Configure the endpoint 1 Tx status as NAK
// 8. Configure the endpoint 2 as bulk OUT
// 9. Configure the endpoint 2 Rx count and data buffer address in the BTABLE (usb_conf.h file)
// 10. Disable the endpoint 2 Tx
// 11. Configure the endpoint 2 Rx status as VALID.

const u8 CSW_SIGNATURE[] = "USBS";
const u8 CBW_SIGNATURE[] = "USBC";
const u8 EP_DESCRIPTOR_IN[] = { 0x07, 0x05, 0x81, 0x02, 0x04, 0x00 };
const u8 EP_DESCRIPTOR_OUT[] = { 0x07, 0x05, 0x02, 0x02, 0x40, 0x00 }; // 0x40 for 64 byte packets
const u8 INTERFACE_DECRIPTOR[] = { 0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x01, 0x06 }; // last value int index of string descriptor
const u8 CONFIG_DESCRIPTOR[] = { 0x09, 0x02, 0x00, 0x00, 0x00, 0x32, 0x01, 0x01, 0x00, 0x00, 0x80, 0x32 };
const u8 DEVICE_DESCRIPTOR[] = { 0x12, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40, 
                                 0x04, 0x83, 0x57, 0x20, 0x01, 0x00, 0x00, 0x04,
                                 0x00, 0x42, 0x00, 0x96, 0x01 };
const u8 INQUIRY_PAGE[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 INQUIRY_STANDARD[] = { 0x00, 0x80, 0x02, 0x02, 0x20, 0x00, 0x00, 0x00,
                                'P', 'y', 'g', 'm', 'y', ' ', 'O', 'S',
                                'B', 'o', 'o', 't', 'l', 'o', 'a', 'd', 'e', 'r', ' ', 
                                'I', 'n', 't', 'e', 'r', 'f', 'a', 'c', 'e' };
const u8 SENSE_MODE_6[] = { 0x03, 0x00, 0x00, 0x00 };
const u8 SENSE_MODE_10[] = { 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 SENSE_SCSI[] = { 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 READ_CAPACITY_10[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const u8 READ_FORMATCAPACITY[] = { 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
                                   0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
    
u8 Bulk_Data_Buff[ 0x40 ]; 
u8 Bot_State, ucUSBState;
u8 ucSOFCount;
SCSICBW CBW;
SCSICSW CSW;
u32 Mass_Memory_Size[2] = { 0, 1024 };
u32 Mass_Block_Size[2] = { 0, 1024 };
u32 Mass_Block_Count[2] = { 0, 1 };
  
void USB_LP_IRQHandler( void )
{
    u32 ulMask;

    ulMask = USB->ISTR;

    if ( ulMask & USB_ISTR_CTR ){
        // servicing of the endpoint correct transfer interrupt 
        // clear of the CTR flag into the sub 
        CTR_LP();
    } // if
    if ( ulMask & USB_ISTR_RESET ){
        USB->ISTR &= ~USB_ISTR_RESET;
        usbReset();
    } // if

    if ( ulMask & USB_ISTR_PMAOVR ){
        USB->ISTR &= ~USB_ISTR_PMAOVR;
    } // if
    if ( ulMask & USB_ISTR_ERR ){
        USB->ISTR &= ~USB_ISTR_ERR;
    } // if
    if ( ulMask & USB_ISTR_WKUP ){
        USB->ISTR &= ~USB_ISTR_WKUP;
        Resume(RESUME_EXTERNAL);
    } // if
    if ( ulMask & USB_ISTR_SUSP ){
        // check if SUSPEND is possible
        //if ( fSuspendEnabled){
        //    Suspend();
        //} else{
            // if not possible then resume after xx ms 
            Resume(RESUME_LATER);
        //} // if
        // clear of the ISTR bit must be done after setting of CNTR_FSUSP 
        USB->ISTR &= ~USB_ISTR_SUSP;
    } // if
    if ( ulMask & USB_ISTR_SOF ){
        USB->ISTR &= ~USB_ISTR_SOF;
        ucSOFCount++;
    } //if
    if ( ulMask & ISTR_ESOF ){
        USB->ISTR &= ~CLR_ESOF;
        // resume handling timing is made with ESOFs 
        Resume(RESUME_ESOF); // request without change of the machine state 
    } // if

}

void usbSetEndpoint( u8 EP, u32 TXAddress, u32 TXCount, u32 RXAddress, u32 RXCount )
{
    ENDPOINT *ep;

    ep = ( USB->BTABLE + ( EP * 16 ) );
    ep->TXAddress   = ( TXAddress >> 1 ) << 1;
    ep->TXCount     = TXCount;
    ep->RXAddress   = ( RXAddress >> 1 ) << 1;
    ep->RXCount     = RXCount;
}

void usbReset( void )
{
    USB_Cable_Config(ENABLE);
    USB->CNTR = USB_CNTR_FRES;
    USB->ISTR = 0;
    USB->CNTR = ( USB_CNTR_RESETM | USB_CNTR_SUSPM | USB_CNTR_WKUPM );
    USB->BTABLE = 0x0000;
    // Next configure EP0 as control EP
    USB->EPR[ 0 ] = ( USB_EPR_STAT_TX_NAK | USB_EPR_EPTYPE_CONTROL );
    usbSetEndpoint( 0, ENDP0_TXADDR, 0, ENDP0_RXADDR, 64 );
    usbSetEndpoint( 1, ENDP1_TXADDR, 0, ENDP1_TXADDR, 0 );
    usbSetEndpoint( 2, ENDP2_RXADDR, 0, ENDP2_RXADDR, 64 );
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  SetEPTxStatus(ENDP2, EP_TX_DIS);


  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Set the device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;

  CBW.dSignature = BOT_CBW_SIGNATURE;
  Bot_State = BOT_IDLE;
}

void usbInit( void )
{

}

u16 usbRead( u8 *ucBuffer )
{

}

u16 usbWrite( u8 *ucBuffer )
{

}

uint32_t USB_SIL_Init(void)
{
#ifndef STM32F10X_CL
  
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);
  wInterrupt_Mask = IMR_MSK;
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);
  
#else
  
  /* Perform OTG Device initialization procedure (including EP0 init) */
  OTG_DEV_Init();
  
#endif /* STM32F10X_CL */

  return 0;
}

/*******************************************************************************
* Function Name  : USB_SIL_Write
* Description    : Write a buffer of data to a selected endpoint.
* Input          : - bEpAddr: The address of the non control endpoint.
*                  - pBufferPointer: The pointer to the buffer of data to be written
*                    to the endpoint.
*                  - wBufferSize: Number of data to be written (in bytes).
* Output         : None.
* Return         : Status.
*******************************************************************************/
uint32_t USB_SIL_Write(uint8_t bEpAddr, uint8_t* pBufferPointer, uint32_t wBufferSize)
{
#ifndef STM32F10X_CL

  /* Use the memory interface function to write to the selected endpoint */
  UserToPMABufferCopy(pBufferPointer, GetEPTxAddr(bEpAddr & 0x7F), wBufferSize);

  /* Update the data length in the control register */
  SetEPTxCount((bEpAddr & 0x7F), wBufferSize);
  
#else
  
   /* Use the PCD interface layer function to write to the selected endpoint */
   PCD_EP_Write (bEpAddr, pBufferPointer, wBufferSize); 
   
#endif /* STM32F10X_CL */

  return 0;
}

u32 USB_SIL_Read(uint8 bEpAddr, uint8_t* pBufferPointer)
{
  u32 DataLength = 0;

  // Get the number of received data on the selected Endpoint 
  DataLength = GetEPRxCount(bEpAddr & 0x7F);
  
  // Use the memory interface function to write to the selected endpoint 
  PMAToUserBufferCopy(pBufferPointer, GetEPRxAddr(bEpAddr & 0x7F), DataLength);
 
  return( DataLength );
}

void Mass_Storage_In ( void )
{
    switch (Bot_State){
        case BOT_CSW_Send:
        case BOT_ERROR:
            Bot_State = BOT_IDLE;
            if (GetEPRxStatus(EP2_OUT) == EP_RX_STALL){
                SetEPRxStatus(EP2_OUT, EP_RX_VALID);/* enable the Endpoint to receive the next cmd*/
            } // if
            break;
        case BOT_DATA_IN:
            if( CBW.CB[0] == SCSI_READ10 ){
                SCSI_Read10_Cmd(CBW.bLUN , SCSI_LBA , SCSI_BlkLen);
            } // if
            break;
        case BOT_DATA_IN_LAST:
            Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
            if (GetEPRxStatus(EP2_OUT) == EP_RX_STALL){
                SetEPRxStatus(EP2_OUT, EP_RX_VALID);/* enable the Endpoint to receive the next cmd*/
            }      
            break;
    } // switch
}

void Mass_Storage_Out (void)
{
    u8 CMD;
    CMD = CBW.CB[0];

    Data_Len = USB_SIL_Read(EP2_OUT, Bulk_Data_Buff);

    switch (Bot_State){
        case BOT_IDLE:
            CBW_Decode();
            break;
        case BOT_DATA_OUT:
            if (CMD == SCSI_WRITE10){
                SCSI_Write10_Cmd(CBW.bLUN , SCSI_LBA , SCSI_BlkLen);
                break;
            } // if
            Bot_Abort(DIR_OUT);
            Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
            Set_CSW (CSW_PHASE_ERROR, SEND_CSW_DISABLE);
            break;
        default:
            Bot_Abort(BOTH_DIR);
            Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
            Set_CSW (CSW_PHASE_ERROR, SEND_CSW_DISABLE);
            break;
    }
}

void CBW_Decode(void)
{
    u32 Counter;

    for (Counter = 0; Counter < Data_Len; Counter++){
        *((uint8_t *)&CBW + Counter) = Bulk_Data_Buff[Counter];
    } // for
    CSW.dTag = CBW.dTag;
    CSW.dDataResidue = CBW.dDataLength;
    if (Data_Len != BOT_CBW_PACKET_LENGTH){
        Bot_Abort(BOTH_DIR);
        /* reset the CBW.dSignature to disable the clear feature until receiving a Mass storage reset*/
        CBW.dSignature = 0;
        Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, PARAMETER_LIST_LENGTH_ERROR);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        return;
    } // if

    if ((CBW.CB[0] == SCSI_READ10 ) || (CBW.CB[0] == SCSI_WRITE10 )){
        /* Calculate Logical Block Address */
        SCSI_LBA = (CBW.CB[2] << 24) | (CBW.CB[3] << 16) | (CBW.CB[4] <<  8) | CBW.CB[5];
        /* Calculate the Number of Blocks to transfer */
        SCSI_BlkLen = (CBW.CB[7] <<  8) | CBW.CB[8];
    } // if

    if (CBW.dSignature == BOT_CBW_SIGNATURE){
        /* Valid CBW */
        if ((CBW.bLUN > Max_Lun) || (CBW.bCBLength < 1) || (CBW.bCBLength > 16)){
            Bot_Abort(BOTH_DIR);
            Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
            Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        } else {
            switch (CBW.CB[0]){
                case SCSI_REQUEST_SENSE:
                    SCSI_RequestSense_Cmd (CBW.bLUN);
                    break;
                case SCSI_INQUIRY:
                    SCSI_Inquiry_Cmd(CBW.bLUN);
                    break;
                case SCSI_START_STOP_UNIT:
                    SCSI_Start_Stop_Unit_Cmd(CBW.bLUN);
                    break;
                case SCSI_ALLOW_MEDIUM_REMOVAL:
                    SCSI_Start_Stop_Unit_Cmd(CBW.bLUN);
                    break;
                case SCSI_MODE_SENSE6:
                    SCSI_ModeSense6_Cmd (CBW.bLUN);
                    break;
                case SCSI_MODE_SENSE10:
                    SCSI_ModeSense10_Cmd (CBW.bLUN);
                    break;
                case SCSI_READ_FORMAT_CAPACITIES:
                    SCSI_ReadFormatCapacity_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ_CAPACITY10:
                    SCSI_ReadCapacity10_Cmd(CBW.bLUN);
                    break;
                case SCSI_TEST_UNIT_READY:
                    SCSI_TestUnitReady_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ10:
                    SCSI_Read10_Cmd(CBW.bLUN, SCSI_LBA , SCSI_BlkLen);
                    break;
                case SCSI_WRITE10:
                    SCSI_Write10_Cmd(CBW.bLUN, SCSI_LBA , SCSI_BlkLen);
                    break;
                case SCSI_VERIFY10:
                    SCSI_Verify10_Cmd(CBW.bLUN);
                    break;
                case SCSI_FORMAT_UNIT:
                    SCSI_Format_Cmd(CBW.bLUN);
                    break;
                    /*Unsupported command*/

                case SCSI_MODE_SELECT10:
                    SCSI_Mode_Select10_Cmd(CBW.bLUN);
                    break;
                case SCSI_MODE_SELECT6:
                    SCSI_Mode_Select6_Cmd(CBW.bLUN);
                    break;

                case SCSI_SEND_DIAGNOSTIC:
                    SCSI_Send_Diagnostic_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ6:
                    SCSI_Read6_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ12:
                    SCSI_Read12_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ16:
                    SCSI_Read16_Cmd(CBW.bLUN);
                    break;
                case SCSI_READ_CAPACITY16:
                    SCSI_READ_CAPACITY16_Cmd(CBW.bLUN);
                    break;
                case SCSI_WRITE6:
                    SCSI_Write6_Cmd(CBW.bLUN);
                    break;
                case SCSI_WRITE12:
                    SCSI_Write12_Cmd(CBW.bLUN);
                    break;
                case SCSI_WRITE16:
                    SCSI_Write16_Cmd(CBW.bLUN);
                    break;
                case SCSI_VERIFY12:
                    SCSI_Verify12_Cmd(CBW.bLUN);
                    break;
                case SCSI_VERIFY16:
                    SCSI_Verify16_Cmd(CBW.bLUN);
                    break;
                default:
                    Bot_Abort(BOTH_DIR);
                    Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
                    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
            }
        }
    } else{
        /* Invalid CBW */
        Bot_Abort(BOTH_DIR);
        Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    } // else
}

void Transfer_Data_Request(uint8_t* Data_Pointer, uint16_t Data_Len)
{
    USB_SIL_Write(EP1_IN, Data_Pointer, Data_Len);
    Bot_State = BOT_DATA_IN_LAST;
    CSW.dDataResidue -= Data_Len;
    CSW.bStatus = CSW_CMD_PASSED;
}

void Set_CSW (uint8_t CSW_Status, uint8_t Send_Permission)
{
    CSW.dSignature = BOT_CSW_SIGNATURE;
    CSW.bStatus = CSW_Status;

    USB_SIL_Write(EP1_IN, ((uint8_t *)& CSW), CSW_DATA_LENGTH);

    Bot_State = BOT_ERROR;
    if (Send_Permission){
        Bot_State = BOT_CSW_Send;  
    }
}

void Bot_Abort(uint8_t Direction)
{
    switch (Direction){
        case DIR_IN :
            SetEPTxStatus(ENDP1, EP_TX_STALL);
            break;
        case DIR_OUT :
            SetEPRxStatus(ENDP2, EP_RX_STALL);
            break;
        case BOTH_DIR :
            SetEPTxStatus(ENDP1, EP_TX_STALL);
            SetEPRxStatus(ENDP2, EP_RX_STALL);
            break;
    } // switch
}

void scsiInquiry( u8 lun )
{
    u8 *Inquiry_Data;
    u16 Inquiry_Data_Length;

    if ( CBW.CB[1] & 0x01 ){
        Inquiry_Data = Page00_Inquiry_Data;
        Inquiry_Data_Length = 5;
    } else{
        if ( !lun ){
            Inquiry_Data = Standard_Inquiry_Data;
        } else {
            Inquiry_Data = Standard_Inquiry_Data2;
        } // else

        if (CBW.CB[4] <= STANDARD_INQUIRY_DATA_LEN){
            Inquiry_Data_Length = CBW.CB[4];
        } else{
            Inquiry_Data_Length = STANDARD_INQUIRY_DATA_LEN;
        } // else
    } // else
    Transfer_Data_Request(Inquiry_Data, Inquiry_Data_Length);
}

void scsiReadFormatCapacity( u8 lun )
{
    if (MAL_GetStatus(lun) != 0 ){
        Set_Scsi_Sense_Data(CBW.bLUN, NOT_READY, MEDIUM_NOT_PRESENT);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
        Bot_Abort(DIR_IN);
        return;
    } // if
    ReadFormatCapacity_Data[4] = (uint8_t)(Mass_Block_Count[lun] >> 24);
    ReadFormatCapacity_Data[5] = (uint8_t)(Mass_Block_Count[lun] >> 16);
    ReadFormatCapacity_Data[6] = (uint8_t)(Mass_Block_Count[lun] >>  8);
    ReadFormatCapacity_Data[7] = (uint8_t)(Mass_Block_Count[lun]);

    ReadFormatCapacity_Data[9] = (uint8_t)(Mass_Block_Size[lun] >>  16);
    ReadFormatCapacity_Data[10] = (uint8_t)(Mass_Block_Size[lun] >>  8);
    ReadFormatCapacity_Data[11] = (uint8_t)(Mass_Block_Size[lun]);
    Transfer_Data_Request(ReadFormatCapacity_Data, READ_FORMAT_CAPACITY_DATA_LEN);
}

void scsiReadCapacity10( u8 lun )
{
    if (MAL_GetStatus(lun)){
        Set_Scsi_Sense_Data(CBW.bLUN, NOT_READY, MEDIUM_NOT_PRESENT);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
        Bot_Abort(DIR_IN);
        return;
    } // if

    ReadCapacity10_Data[0] = (uint8_t)((Mass_Block_Count[lun] - 1) >> 24);
    ReadCapacity10_Data[1] = (uint8_t)((Mass_Block_Count[lun] - 1) >> 16);
    ReadCapacity10_Data[2] = (uint8_t)((Mass_Block_Count[lun] - 1) >>  8);
    ReadCapacity10_Data[3] = (uint8_t)(Mass_Block_Count[lun] - 1);

    ReadCapacity10_Data[4] = (uint8_t)(Mass_Block_Size[lun] >>  24);
    ReadCapacity10_Data[5] = (uint8_t)(Mass_Block_Size[lun] >>  16);
    ReadCapacity10_Data[6] = (uint8_t)(Mass_Block_Size[lun] >>  8);
    ReadCapacity10_Data[7] = (uint8_t)(Mass_Block_Size[lun]);
    Transfer_Data_Request(ReadCapacity10_Data, READ_CAPACITY10_DATA_LEN);
}

void scsiModeSense6( u8 lun )
{
    Transfer_Data_Request(Mode_Sense6_data, MODE_SENSE6_DATA_LEN);
}

void scsiModeSense10( u8 lun )
{
    Transfer_Data_Request(Mode_Sense10_data, MODE_SENSE10_DATA_LEN);
}

void scsiRequestSense( u8 lun )
{
    uint8_t Request_Sense_data_Length;

    if ( CBW.CB[4] <= REQUEST_SENSE_DATA_LEN ){
        Request_Sense_data_Length = CBW.CB[4];
    } else{
        Request_Sense_data_Length = REQUEST_SENSE_DATA_LEN;
    } // else
    Transfer_Data_Request(Scsi_Sense_Data, Request_Sense_data_Length);
}

void scsiSetSenseData( u8 lun, u8 Sens_Key, u8 Asc)
{
    Scsi_Sense_Data[2] = Sens_Key;
    Scsi_Sense_Data[12] = Asc;
}

void scsiStartStopUnit( u8 lun )
{
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
}


void scsiRead10( u8 lun , u32 LBA , u32 BlockNbr )
{
    if (Bot_State == BOT_IDLE){
        if (!(SCSI_Address_Management(CBW.bLUN, SCSI_READ10, LBA, BlockNbr))){
            return;
        } // if
        if ((CBW.bmFlags & 0x80) != 0){
            Bot_State = BOT_DATA_IN;
            Read_Memory(lun, LBA , BlockNbr);
        } else{
            Bot_Abort(BOTH_DIR);
            Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
            Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
        } // else
        return;
    } else if (Bot_State == BOT_DATA_IN){
        Read_Memory(lun , LBA , BlockNbr);
    } // else if
}

void scsiWrite10( u8 lun , u32 LBA , u32 BlockNbr )
{
    if (Bot_State == BOT_IDLE){
        // Check for address out of range
        if (!(SCSI_Address_Management(CBW.bLUN, SCSI_WRITE10 , LBA, BlockNbr))){
            return;
        } // if

        if ((CBW.bmFlags & 0x80) == 0){
            Bot_State = BOT_DATA_OUT;
        SetEPRxStatus(ENDP2, EP_RX_VALID);
        } else{
            Bot_Abort(DIR_IN);
            Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
            Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        } // else
        return;
    } else if (Bot_State == BOT_DATA_OUT){
        Write_Memory(lun , LBA , BlockNbr);
    } // else if
}

void scsiVerify10( u8 lun )
{
    if ((CBW.dDataLength == 0) && !(CBW.CB[1] & BLKVFY)){
        Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    } else{
        Bot_Abort(BOTH_DIR);
        Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    } // else
}

void scsiValid( u8 lun )
{
    if (CBW.dDataLength != 0){
        Bot_Abort(BOTH_DIR);
        Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    } else{
        Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    } // else
}

void scsiTestUnitReady( u8 lun )
{
    if (MAL_GetStatus(lun)){
        Set_Scsi_Sense_Data(CBW.bLUN, NOT_READY, MEDIUM_NOT_PRESENT);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
        Bot_Abort(DIR_IN);
        return;
    } else{
        Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    } // else
}

void scsiFormat( u8 lun )
{
    if (MAL_GetStatus(lun)){
        Set_Scsi_Sense_Data(CBW.bLUN, NOT_READY, MEDIUM_NOT_PRESENT);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
        Bot_Abort(DIR_IN);
        return;
    } // if
}

void scsiInvalid( u8 lun )
{
    if (CBW.dDataLength == 0){
        Bot_Abort(DIR_IN);
    } else{
        if ((CBW.bmFlags & 0x80) != 0){
            Bot_Abort(DIR_IN);
        } else{
            Bot_Abort(BOTH_DIR);
        } // else
    } // else
    Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
}

bool scsiAddressManagement( u8 lun, u8 Cmd, u32 LBA, u32 BlockNbr )
{
    if ((LBA + BlockNbr) > Mass_Block_Count[lun] ){
        if (Cmd == SCSI_WRITE10){
            Bot_Abort(BOTH_DIR);
        } // if
        Bot_Abort(DIR_IN);
        Set_Scsi_Sense_Data(lun, ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        return (FALSE);
    } // if

    if (CBW.dDataLength != BlockNbr * Mass_Block_Size[lun]){
        if (Cmd == SCSI_WRITE10){
            Bot_Abort(BOTH_DIR);
        } else{
            Bot_Abort(DIR_IN);
        } // else
        Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
        Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        return (FALSE);
    } // if
    return (TRUE);
}

