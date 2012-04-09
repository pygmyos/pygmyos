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

#include "pygmy_profile.h"
#include "pygmy_xmodem.h"
#include "pygmy_file.h"

u8 xmodemRecv( PYGMYXMODEM *pygmyXModem, u8 *ucFileName )
{
    if( fileOpen( &pygmyXModem->File, ucFileName, WRITE ) ){
        pygmyXModem->Status |= XMODEM_ACTIVE;       // BIT1 used to mark In XModem Status
        pygmyXModem->Timeout = 1000; // 10 seconds
        pygmyXModem->Transaction = 60;
        pygmyXModem->Count = 1;
    
        return( TRUE );
    } // if
        
    return( FALSE );
}

u8 xmodemSend( PYGMYXMODEM *pygmyXModem, u8 *ucFileName )
{
    if( fileOpen( &pygmyXModem->File, ucFileName, READ ) ){
        pygmyXModem->Count = 1;
        pygmyXModem->Timeout = 1000;
        pygmyXModem->Transaction = 10;
        pygmyXModem->Status = ( XMODEM_SEND | XMODEM_SEND_WAIT ); 
    
        return( TRUE );
    } // if
 
    return( FALSE );
}

u8 xmodemProcess( PYGMYXMODEM *pygmyXModem, u8 ucByte )
{
    // returns TRUE if xmodem active and processing char, else FALSE
    if( pygmyXModem->Status & XMODEM_RECV ){ // RECV      
        if( pygmyXModem->Status & XMODEM_MODE_SOH ){ // Recieving Payload, after <SOH>
            pygmyXModem->Buffer[ pygmyXModem->Index++ ] = ucByte;
            if( pygmyXModem->Index == 131 ){
                pygmyXModem->Status &= ~XMODEM_MODE_SOH; // Reset Packet Marker
                if( pygmyXModem->Count == pygmyXModem->Buffer[ 0 ] && 
                    //xmodemWritePacket( pygmyXModem, (u8*)(pygmyXModem->Buffer+2) ) ) { // Returns 1 if check sum correct, 0 if not
                    xmodemWritePacket( pygmyXModem ) ) { // Returns 1 if check sum correct, 0 if not
                    if( pygmyXModem->Count + 1 == 256 ){
                        pygmyXModem->Count = 0;
                    } else{
                        ++pygmyXModem->Count;
                    } // else
                    putcUSART3( 0x06 );// Return ACK
                } else{
                    putcUSART3( 0x15 );// Return NACK
                } // else  
            } 
        } else{
            if( ucByte == 0x01 ){ // This is XModem <SOH>
                pygmyXModem->Status |= XMODEM_MODE_SOH; // Set Packet Marker
                pygmyXModem->Timeout = 1000;
                pygmyXModem->Transaction = 10;
            } else if( ucByte == 0x04 || ucByte == XMODEM_CAN ){ // 0x04 Marks End Of Transmission              
                pygmyXModem->Status = 0;//&= ~( XMODEM_RECV );
                filePutBuffer( &pygmyXModem->File, 128, pygmyXModem->Buffer );
                fileClose( &pygmyXModem->File );
                putcUSART3( 0x06 ); // Send Ack to close connection
                //putstr( "Done\r> " );
            } // else if
            pygmyXModem->Index = 0;
        } // else
        return( TRUE );
    } else if( pygmyXModem->Status & XMODEM_SEND ){ // SEND
        if( ucByte == XMODEM_ACK ){
            if( pygmyXModem->Status & XMODEM_SEND_EOT ){
                pygmyXModem->Status = 0;
                //putstr( (u8*)BOOT_PROMPT );
                return;
            } // if
            pygmyXModem->Timeout = 1000;
            pygmyXModem->Transaction = 10;
            if( pygmyXModem->File.Attributes & EOF ){
                pygmyXModem->Status |= XMODEM_SEND_EOT;
                putcUSART3( XMODEM_EOT );
            } else{
                ++pygmyXModem->Count;
                xmodemSendPacket( pygmyXModem, XMODEM_NEXT ); // 0 = next pack
            } // else
        } else if( ucByte == XMODEM_NACK ) {//|| ucByte == XMODEM_CAN ){
            pygmyXModem->Timeout = 1000;
            pygmyXModem->Transaction = 10;
            if( pygmyXModem->Status & XMODEM_SEND_WAIT ){
                xmodemSendPacket( pygmyXModem, XMODEM_NEXT ); // 1 = next, in this case this is first
                pygmyXModem->Status &= ~XMODEM_SEND_WAIT;
                //    xmodemSendPacket( XMODEM_LAST ); // 0 = next pack
            } else{
                xmodemSendPacket( pygmyXModem, XMODEM_LAST ); // 0 = next pack
            } // else
        } else if( ucByte == XMODEM_CAN ){
            pygmyXModem->Status = 0;
            putcUSART3( 0x06 ); // Send Ack to close connection
            //putstr( (u8*)BOOT_PROMPT );
        } // else if
        return( TRUE );
    } // else if
    
    return( FALSE );
}

void xmodemProcessTimer( PYGMYXMODEM *pygmyXModem )
{
    if( pygmyXModem->Status & XMODEM_RECV ){
        if( pygmyXModem->Timeout ){
            --pygmyXModem->Timeout;
        } else{
            putcUSART3( XMODEM_NACK );
            pygmyXModem->Timeout = 1000;
            if( pygmyXModem->Transaction ){
                --pygmyXModem->Transaction;
            } else{
                pygmyXModem->Status &= ~(XMODEM_RECV|XMODEM_MODE_SOH);
                //putstr( "Timeout\r> " );
            } // lse
        } // else
    } else if( pygmyXModem->Status & XMODEM_SEND ){
        if( pygmyXModem->Timeout )
            --pygmyXModem->Timeout;
        else{
            xmodemSendPacket( pygmyXModem, XMODEM_LAST );
            pygmyXModem->Timeout = 1000;
            if( pygmyXModem->Transaction )
                --pygmyXModem->Transaction;
            else{
                pygmyXModem->Status &= ~(XMODEM_SEND|XMODEM_SEND_EOT);
                //putstr( (u8*)BOOT_ERROR );
            } // else
        } // else
    } // else if
}

u8 xmodemWritePacket( PYGMYXMODEM *pygmyXModem )//u8 *ucBuffer )
{
    u8 i, ucSum, *ucBuffer;

    // count ( 0-1 before calling this function ) must be validated by calling function 
    // 0-127 are data bytes, 128 is checksum 
    ucBuffer = pygmyXModem->Buffer+2;
    ucSum = ucBuffer[ 0 ];
    for( i = 1; i < 128; i++ ){
        ucSum += ucBuffer[ i ]; 
    } // for  
    if( ucSum == ucBuffer[ 128 ] ){
        filePutBuffer( &pygmyXModem->File, 128, ucBuffer );
        return( TRUE );
    } // if
	
    return( FALSE );
}

void xmodemSendPacket( PYGMYXMODEM *pygmyXModem, u8 ucLast )
{
    //u8 ucBuffer[ 132 ]; // was static
    u8 i, ucSum, *ucBuffer;
    
    ucBuffer = pygmyXModem->Buffer;
    if( !ucLast ){ // If not resend then make a new packet
        ucBuffer[ 0 ] = 0x01; // <SOH>
        ucBuffer[ 1 ] = pygmyXModem->Count; // Packet Count
        ucBuffer[ 2 ] = 0xFF - pygmyXModem->Count; // Ones Complement of Count
        for( ucSum = 0, i = 3; i < 131; i++ ){ // 128 bytes payload
            ucBuffer[ i ] = fileGetChar( &pygmyXModem->File );
            ucSum += ucBuffer[ i ];
        } // for
        ucBuffer[ i ] = ucSum; // add the check sum as the final bytes    
    } // if
    
    putBufferUSART3( 132, ucBuffer );
}

