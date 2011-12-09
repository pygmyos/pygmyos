#include "pygmy_profile.h"

// RF communicates through sockets, each socket is negotiated for one
// transaction between two devices. Once the transaction is completed,
// the socket should be closed.
// [ DESTID 4B ][ PORT 1B ][ PCOUNT 1B ][ DATA 25B ][ CRC 1B ]
// 

PYGMYRFSOCKET pygmyGlobalRFSockets[ RF_MAXSOCKETS ];
u32 globalRFConfig;
PYGMYSPIPORT pygmyRFSPI;
const u8 ucAddrP0[] = {0x78,0x78,0x78,0x78,0x78};
u32 pygmyGlobalRFID;

u32 rfGetID( void )
{
    return( pygmyGlobalRFID );
}

void rfGetRXPayload( u8 *ucBuffer )
{
    u8 i, ii, ucPipe, ucLen;
    
    ucPipe = rfGetStatus() & (BIT3|BIT2|BIT1) >> 1;
    if( ucPipe == 7 ){
        return;
    } // if
    
    while( !rfIsRXFIFOEmpty() ){
        ucLen = rfGetRXPayloadLen( );
        RF_CS_LOW;
        spiWriteByte( &pygmyRFSPI, RF_RX_PAYLOAD );
        for( i = 0; i < ucLen; i++ ){
            ucBuffer[ i ] = spiReadByte( &pygmyRFSPI );
        } // if
        ucBuffer[ i ] = '\0';
        RF_CS_HIGH;
        print( COM3, ucBuffer );
       // rfProcessPacket( ucBuffer, ucLen );
    } // while
    rfClearStatus();
}

void rfSendOpenSocket( u32 ulDestID, u8 ucType  )
{
    u16 uiCRC;
    u8 ucBuffer[ 32 ];

    ucBuffer[ 0 ] = ulDestID >> 24;
    ucBuffer[ 1 ] = ulDestID >> 16;
    ucBuffer[ 2 ] = ulDestID >> 8;
    ucBuffer[ 3 ] = 0xFF; // No port defined yet
    if( ucType == RF_FILE ){
    
    } else if( ucType == RF_AVSTREAM ){
    
    } else if( ucType == RF_COMLINK ){
        ucBuffer[ 4 ] = RF_OPEN | RF_COMLINK;
        ucBuffer[ 5 ] = 0;
        uiCRC = sysCRC16( ucBuffer, 6 );
        ucBuffer[ 6 ] = (u8)uiCRC >> 8;
        ucBuffer[ 7 ] = (u8)uiCRC;
    } // else if
}

void rfProcessPacket( u8 *ucBuffer, u8 ucLen )
{    
    // Unified Socket
    // IDs = ( PDIASUM & 0xFFFFFF00 ) 
    // [ DESTID 3B PORT 1B ][ CMD 1B ][ CHUNK 1B ][ PAYLOAD 1-24B ][ CRC 2B ]

    // New Socket
        // [ DESTID 0xFF ][ RF_OPEN | RF_FILE ][ 0x00 ][ SRCID 0xFF ][ FILENAME 1-12B 0x00 ][ LEN ][ CRC ]
        // [ DESTID 0xFF ][ RF_OPEN | RF_AV ][ 0x00 ][ SRCID 0xFF ][ CRC ]
        // [ DESTID 0xFF ][ RF_OPEN | RF_COMLINK ][ 0x00 ][ CRC ]
    // Close Socket
        // [ DESTID PORT ][ RF_CLOSE ][ 0x00 ][ CRC ]
    // New Socket Response
        // [ DESTID PORT ][ RF_OPEN | RF_ACK ][ 0x00 ][ CRC ]
    // Ack/Nack Packet
        // [ DESTID PORT ][ RF_ACK/RF_NACK ][ 0x00 ][ CRC ]
    
    u32 ulID, ulDestID;
    u8 i, ucCMD, ucType, ucPort, ucChunk;
 
    if( sysCRC8( 31, ucBuffer ) != ucBuffer[ 31 ] ){
        return;
    } // if
    ulID = bufferToU32( ucBuffer );
    ucPort = (u8)ulID;
    ucBuffer += 4;
    if ( ulID & 0xFFFFFF00 != pygmyGlobalRFID ){
        return;
    } // if
    ucCMD = *(ucBuffer++);
    ucType = ucCMD & 0x0F;
    ucChunk = *(ucBuffer++);
        
    if( ucCMD & RF_OPEN ){
        
        rfOpenSocket( ulDestID & 0xFFFFFF00, ucType );
        print( COM3, "\rSocket Opened, ID: %8X", ulDestID & 0xFFFFFF00 );
    } else if( ucCMD & RF_CLOSE ){
        
    } else if( ucCMD & RF_RXTX ){
        
    } else if( ucCMD & RF_ACK ){
        
    } // else if
}

u8 rfOpenSocket( u32 ulDest, u8 ucType )
{
    u8 i;

    for( i = 0; i < RF_MAXSOCKETS; i++ ){
        if( !pygmyGlobalRFSockets[ i ].DestID ){
            pygmyGlobalRFSockets[ i ].DestID = ulDest | i;
            pygmyGlobalRFSockets[ i ].SocketType = ucType;
            pygmyGlobalRFSockets[ i ].PacketCount = 0;
            pygmyGlobalRFSockets[ i ].StartTime = timeGet();
            return( i );
        } // if
    } // for
    return( 0xFF );
}

void rfCloseSocket( u8 ucSocket )
{
    pygmyGlobalRFSockets[ ucSocket ].DestID = 0;
}

void rfRX( void )
{
    u8 ucStatus, ucBuffer[ 40 ];
    
    ucStatus = rfGetStatus();
    
    if( ucStatus & BIT6 ){
        rfGetRXPayload( ucBuffer );
    } //if
    if( ucStatus & BIT5 ){
        rfSetRX();
    } // if 
}

void rfInit( void )
{
    u8 i, *strID;

    strID = getID( );
    pdiaEncode( 0, PDIA_NEW, &pygmyGlobalRFID );
    for( i = 0; i < 12; i++ ){
        pdiaEncode( strID[ i ], PDIA_ADD, &pygmyGlobalRFID );
    }
    pdiaEncode( 0, PDIA_END, &pygmyGlobalRFID );
    spiConfig( &pygmyRFSPI, RF_CS, RF_SCK, RF_MISO, RF_MOSI );
    pinConfig( RF_EN, OUT );
    RF_EN_LOW;
    pinConfig( RF_IRQ, PULLUP );
    pinInterrupt( rfRX, RF_IRQ, TRIGGER_FALLING );

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENAA );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    RF_CS_HIGH;
    rfFlushTX();
    rfFlushRX();
    rfWriteAddress( RF_REG_TXADDR, (u8*)ucAddrP0 );
    rfWriteAddress( RF_REG_RXADDR_P0, (u8*)ucAddrP0 );
    rfSetRX();
}

u8 rfGetSignalQuality( void )
{
    u8 ucSignalQuality;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_READ|RF_REG_OBSERVETX );
    ucSignalQuality = 15 - ( RF_REG_OBSERVETX_ARCCNT & spiReadByte( &pygmyRFSPI ) ); 
    RF_CS_HIGH;
    
    rfClearStatus();
    
    return( ucSignalQuality );
}

u8 rfGetTX( void )
{
    return( globalRFConfig & BIT0 );
}

u8 rfSetTX( void )
{
    globalRFConfig |= BIT0;
    RF_EN_LOW;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_EN_CRC ); 
    RF_CS_HIGH;
    rfClearStatus( );
    RF_EN_HIGH;
}

u8 rfSetRX( void )
{
    globalRFConfig &= ~BIT0;
    RF_EN_LOW;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_PRIM_RX|RF_REG_CONFIG_EN_CRC ); 
    RF_CS_HIGH;
    rfClearStatus( );
    RF_EN_HIGH;
}

void rfFlushTX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_FLUSH );
    RF_CS_HIGH;
}

void rfFlushRX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_RX_FLUSH );
    RF_CS_HIGH;
}

void rfPutString( u8 *ucBuffer )
{
    u8 i;

    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    for( i = 0; *ucBuffer; i++ ){
        if( i == 32 ){
            RF_CS_HIGH;
            while( !( rfGetStatus() & BIT5 ) ){;}
            rfClearStatus();
            i = 0;
            RF_CS_LOW;
            spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
        } // if
        spiWriteByte( &pygmyRFSPI, *(ucBuffer++) ); 
    } // for
    RF_CS_HIGH;
    while( !( rfGetStatus() & BIT5 ) ){;}
    //rfFlushTX();
    rfClearStatus();
    rfSetRX();
}

void rfPutTXBuffer( u16 uiLen, u8 *ucBuffer )
{
    u8 i;
    
    rfSetTX();
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
    for( i = 0; i < uiLen; i++ ){
        if( i == 32 ){
            RF_CS_HIGH;
            while( !( rfGetStatus() & BIT5 ) ){;}
            rfClearStatus();
            uiLen -= 32;
            RF_CS_LOW;
            spiWriteByte( &pygmyRFSPI, RF_TX_PAYLOAD );
        } // if
        spiWriteByte( &pygmyRFSPI, ucBuffer[ i ] ); 
    } // for
    RF_CS_HIGH;
    while( !( rfGetStatus() & BIT5 ) ){;}
    rfClearStatus();
    rfSetRX();
}

u8 rfGetStatus( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    ucStatus = spiReadByte( &pygmyRFSPI );
    RF_CS_HIGH;

    return( ucStatus );
}

u8 rfGetRXPayloadLen( void )
{
    u8 ucByte;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_RX_LEN );
    ucByte = spiReadByte( &pygmyRFSPI  );
    RF_CS_HIGH;
    
    return( ucByte ); 
}

u8 rfIsRXFIFOEmpty( void )
{
    u8 ucStatus;

    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_REG_FIFOSTATUS );
    ucStatus = spiReadByte( &pygmyRFSPI ) & BIT0;
    RF_CS_HIGH;

    return( ucStatus );
}
/*
void rfConfigAsTX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_EN_CRC ); // Unique part of address
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    //spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT0 );
    //spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL|RF_REG_FEATURE_EN_ACK_PAY );
    RF_CS_HIGH;
}*/

/*
void rfConfigAsRX( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_CONFIG );
    spiWriteByte( &pygmyRFSPI, RF_REG_CONFIG_PWR_UP|RF_REG_CONFIG_PRIM_RX|RF_REG_CONFIG_EN_CRC ); // Unique part of address
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_ENRXADDR );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_DYNPD );
    spiWriteByte( &pygmyRFSPI, BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF_CS_HIGH;
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_FEATURE );
    spiWriteByte( &pygmyRFSPI, RF_REG_FEATURE_EN_DPL|RF_REG_FEATURE_EN_ACK_PAY );
    RF_CS_HIGH;
}*/

void rfClearStatus( void )
{
    RF_CS_LOW;
    spiWriteByte( &pygmyRFSPI, RF_WRITE|RF_REG_STATUS );
    spiWriteByte( &pygmyRFSPI, BIT6|BIT5|BIT4 ); 
    RF_CS_HIGH;
}

void rfWriteAddress( u8 ucReg, u8 *ucAddress )
{
    //const u8 ucAddrP0[] = {0xB3,0xB4,0xB5,0xB6,0x05};
    u8 i;

    // ucAddress is presumed to be default 5 byte mode
    RF_CS_LOW;
    
    spiWriteByte( &pygmyRFSPI, RF_WRITE|ucReg );
    for( i = 0; i < 5; i++ ){
        spiWriteByte( &pygmyRFSPI, ucAddress[ 4 - i ] ); // byte order must be inverted
    }
    RF_CS_HIGH;
}

/*
void rfSPIWriteByte( u8 ucByte )
{
	u16 i;
	
    RF_SCK_LOW;		            // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 8; i++ ){ 		// Clock out Write CMD
        if( ucByte & ( BIT7 >> i ) ){		
            RF_MOSI_HIGH;          // SlaveInMasterOut high if bit set
        } else{
            RF_MOSI_LOW;           // SlaveInMasterOut low if bit clear
        }
        RF_SCK_HIGH;				// clock must start low, transition high
        RF_SCK_LOW;			    // Low transition finishes clock sequence
    }
    
} 

void rfSPIWriteWord( u16 uiWord )
{
	u16 i;
	
    RF24_SCK_LOW;					    // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 16; i++ ){ 			// 
        if( uiWord & ( 0x8000 >> i ) ){
            RF24_MOSI_HIGH;              // SlaveInMasterOut high if bit set
        } else{ 		
            RF24_MOSI_LOW;               // SlaveInMasterOut low if bit clear
        }
        RF24_SCK_HIGH;				    // clock must start low, transition high
        RF24_SCK_LOW;				    // Low transition finishes clock sequence
    } 
} 

void rfSPIWriteLong( u32 ulLong )
{
	u16 i;

    RF24_SCK_LOW;					    // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 32; i++ ){ 			// Clock out Write CMD
        if( ulLong & ( 0x80000000 >> i ) ){
            RF24_MOSI_HIGH;              // SlaveInMasterOut high if bit set
        } else{ 		
            RF24_MOSI_LOW;               // SlaveInMasterOut low if bit clear
        }
        RF24_SCK_HIGH;				    // clock must start low, transition high
        RF24_SCK_LOW;				    // Low transition finishes clock sequence
    }
} 

u8 rfSPIReadByte( void )
{
    u16 i;
	u8 ucByte;
	
    RF24_SCK_LOW;
    for( i = 0, ucByte = 0; i < 8; i++ ){    
        RF24_SCK_HIGH;				        // clock must start low, transition high 			
        if( RF24_MISO_STATE ){                // Test port input for high and set bit in ucByte
            ucByte |= ( BIT7 >> i );
        }
        RF24_SCK_LOW;				        // Low transition finishes clock sequence
    }

	return( ucByte );
}

void rfTXPacket( void )
{

}

void rfRXPacket( void )
{
    
}*/
