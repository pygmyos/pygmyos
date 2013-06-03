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
#include "pygmy_pfat.h"
//#include "pygmy_file.h"

void xmodemInit( PYGMYXMODEM *XModem )
{
    XModem->Status = 0;
    XModem->Timeout = 0;
    XModem->Transaction = 0;
    XModem->Failures = 0;
    XModem->Count = 0;
    XModem->Index = 0;
    XModem->Port = 0;
    XModem->Enabled = FALSE;
}

void xmodemEnable( PYGMYXMODEM *XModem )
{
    XModem->Enabled = TRUE;
}

void xmodemDisable( PYGMYXMODEM *XModem )
{
    XModem->Enabled = FALSE;
}

u8 xmodemRecv( PYGMYXMODEM *XModem, u8 *FileName )
{
    XModem->File = fileOpen( FileName, WRITE, 0 );
    //if( fileOpen( &pygmyXModem->File, ucFileName, WRITE ) ){
    if( XModem->File ){
        XModem->Status |= XMODEM_ACTIVE;       // BIT1 used to mark In XModem Status
        XModem->Timeout = 1000; // 10 seconds
        XModem->Transaction = 60;
        XModem->Count = 1;
        //print( COM3, "%c", XMODEM_NACK );

        return( TRUE );
    } // if
        
    return( FALSE );
}

u8 xmodemSend( PYGMYXMODEM *XModem, u8 *FileName )
{
    XModem->File = fileOpen( FileName, READ, 0 );
    //if( fileOpen( &pygmyXModem->File, ucFileName, READ ) ){
    if( XModem->File ){
        XModem->Count = 1;
        XModem->Timeout = 1000;
        XModem->Transaction = 10;
        XModem->Status = ( XMODEM_SEND | XMODEM_SEND_WAIT ); 
    
        return( TRUE );
    } // if
 
    return( FALSE );
}

u8 xmodemProcess( PYGMYXMODEM *XModem, u8 Byte )
{
    // returns TRUE if xmodem active and processing char, else FALSE
    if( XModem->Status & XMODEM_RECV ){ // RECV      
        if( XModem->Status & XMODEM_MODE_SOH ){ // Recieving Payload, after <SOH>
            XModem->Buffer[ XModem->Index++ ] = Byte;
            if( XModem->Index == 131 ){
                XModem->Status &= ~XMODEM_MODE_SOH; // Reset Packet Marker
                if( XModem->Count == XModem->Buffer[ 0 ] && xmodemWritePacket( XModem ) ) { // Returns 1 if check sum correct, 0 if not
                    if( XModem->Count + 1 == 256 ){
                        XModem->Count = 0;
                    } else{
                        ++XModem->Count;
                    } // else
                    XModem->Failures = 0; // Reset failure counter, counter starts with each ACK
                    print( COM3, "%c", XMODEM_ACK );
                } else{
                    print( COM3, "%c", XMODEM_NACK );
                } // else  
            } 
        } else{
            if( Byte == XMODEM_SOH ){ // This is XModem <SOH> // was 0x01
                XModem->Status |= XMODEM_MODE_SOH; // Set Packet Marker
                XModem->Timeout = 1000;
                XModem->Transaction = 10;
            } else if( Byte == XMODEM_EOT || Byte == XMODEM_CAN ){ // 0x04 Marks End Of Transmission   
                xmodemInit( XModem );
                //XModem->Status = 0;//&= ~( XMODEM_RECV );
                filePutBuffer( XModem->File, XModem->Buffer, 128 );
                print( COM3, "%c", XMODEM_ACK );
                fileClose( XModem->File );
                
                //delay(500);
                print( COM3, "\rClosing File %s", XModem->File->Properties.Name );
                //putstr( "Done\r> " );
            } // else if
            XModem->Index = 0;
        } // else

        return( TRUE );
    } else if( XModem->Status & XMODEM_SEND ){ // SEND
        if( Byte == XMODEM_ACK ){
            XModem->Failures = 0; // Reset Failure counter, counter starts with each ACK
            if( XModem->Status & XMODEM_SEND_EOT ){
                xmodemInit( XModem );
                //XModem->Status = 0;
                //putstr( (u8*)BOOT_PROMPT );
                return( TRUE );
            } // if
            XModem->Timeout = 1000;
            XModem->Transaction = 10;
            //if( pygmyXModem->File.Attributes & EOF ){
            if( XModem->File->Properties.Attributes & EOF ){
                XModem->Status |= XMODEM_SEND_EOT;
                print( COM3, "%c", XMODEM_EOT );
            } else{
                ++XModem->Count;
                xmodemSendPacket( XModem, XMODEM_NEXT ); // 0 = next pack
            } // else
        } else if( Byte == XMODEM_NACK ) {//|| ucByte == XMODEM_CAN ){
            XModem->Timeout = 1000;
            XModem->Transaction = 10;
            if( XModem->Status & XMODEM_SEND_WAIT ){
                xmodemSendPacket( XModem, XMODEM_NEXT ); // 1 = next, in this case this is first
                XModem->Status &= ~XMODEM_SEND_WAIT;
                //    xmodemSendPacket( XMODEM_LAST ); // 0 = next pack
            } else{
                xmodemSendPacket( XModem, XMODEM_LAST ); // 0 = next pack
            } // else
        } else if( Byte == XMODEM_CAN ){
            xmodemInit( XModem );
            //XModem->Status = 0;
            print( COM3, "%c", 0x06 ); // Send Ack to close connection
            //putstr( (u8*)BOOT_PROMPT );
        } // else if
        return( TRUE );
    } // else if
    
    return( FALSE );
}

void xmodemProcessTimer( PYGMYXMODEM *XModem )
{
    if( XModem->Status & XMODEM_RECV ){
        if( XModem->Timeout ){
            --XModem->Timeout;
        } else{
            ++XModem->Failures;
            if( XModem->Failures >= XMODEM_MAXFAILURES ){
                xmodemInit( XModem );
                //XModem->Status = 0;
                print( COM3, "%c", XMODEM_CAN );
                return;
            } // if
            print( COM3, "%c", XMODEM_NACK );
            XModem->Timeout = 1000;
            if( XModem->Transaction ){
                --XModem->Transaction;
            } else{
                XModem->Status &= ~(XMODEM_RECV|XMODEM_MODE_SOH);
                //putstr( "Timeout\r> " );
            } // else
        } // else
    } else if( XModem->Status & XMODEM_SEND ){
        if( XModem->Timeout )
            --XModem->Timeout;
        else{
            ++XModem->Failures;
            if( XModem->Failures >= XMODEM_MAXFAILURES ){
                xmodemInit( XModem );
                //XModem->Status = 0;
                print( COM3, "%c", XMODEM_CAN );
                return;
            } // if
            xmodemSendPacket( XModem, XMODEM_LAST );
            XModem->Timeout = 1000;
            if( XModem->Transaction )
                --XModem->Transaction;
            else{
                XModem->Status &= ~(XMODEM_SEND|XMODEM_SEND_EOT);
                //putstr( (u8*)BOOT_ERROR );
            } // else
        } // else
    } // else if
}

u8 xmodemWritePacket( PYGMYXMODEM *XModem )//u8 *ucBuffer )
{
    u8 i, Sum, *Buffer;

    // count ( 0-1 before calling this function ) must be validated by calling function 
    // 0-127 are data bytes, 128 is checksum 
    Buffer = XModem->Buffer+2;
    Sum = Buffer[ 0 ];
    for( i = 1; i < 128; i++ ){
        Sum += Buffer[ i ]; 
    } // for  
    if( Sum == Buffer[ 128 ] ){
        filePutBuffer( XModem->File, Buffer, 128 );
        return( TRUE );
    } // if
	
    return( FALSE );
}

void xmodemSendPacket( PYGMYXMODEM *XModem, u8 ucLast )
{
    u8 i, Sum, *Buffer;
    
    Buffer = XModem->Buffer;
    if( !ucLast ){ // If not resend then make a new packet
        Buffer[ 0 ] = 0x01; // <SOH>
        Buffer[ 1 ] = XModem->Count; // Packet Count
        Buffer[ 2 ] = 0xFF - XModem->Count; // Ones Complement of Count
        for( Sum = 0, i = 3; i < 131; i++ ){ // 128 bytes payload
            #ifdef __PYGMY_PFATLITE
                Buffer[ i ] = fileGetChar( &XModem->File );
            #else
                Buffer[ i ] = fileGetChar( XModem->File );
            #endif
            Sum += Buffer[ i ];
        } // for
        Buffer[ i ] = Sum; // add the check sum as the final bytes    
    } // if

    filePutBuffer( XModem->File, Buffer, 132 );
    // putBufferUSART3( ucBuffer, 132 );
}

