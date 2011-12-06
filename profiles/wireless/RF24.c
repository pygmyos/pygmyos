#include "pygmy_sys.h"
#include "pygmy_profile.h"
#include "RF24.h"

u32 globalRF24Config;

u8 Pygmy_RF24_GetSignalQuality( void )
{
    u8 ucSignalQuality;

    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_READ|RF24_REG_OBSERVETX );
    ucSignalQuality = 15 - ( RF24_REG_OBSERVETX_ARCCNT & Pygmy_RF24_SPI_ReadByte( ) ); 
    RF24_CS_HIGH;
    
    Pygmy_RF24_ClearStatus();
    
    return( ucSignalQuality );
}

u8 Pygmy_RF24_GetTX( void )
{
    return( globalRF24Config & BIT0 );
}

u8 Pygmy_RF24_SetTX( void )
{
    globalRF24Config |= BIT0;

    RF24_EN_LOW;
    Pygmy_RF24_ConfigAsTX( );
    Pygmy_RF24_ClearStatus( );
    RF24_EN_HIGH;
}

u8 Pygmy_RF24_SetRX( void )
{
    globalRF24Config &= ~BIT0;
    RF24_EN_LOW;
    Pygmy_RF24_ConfigAsRX( );
    Pygmy_RF24_ClearStatus( );
    RF24_EN_HIGH;
}

void Pygmy_RF24_PutTXBuffer( u8 ucLen, u8 *ucBuffer )
{
    u8 i;
    
    Pygmy_RF24_SetTX();
    Pygmy_RF24_ClearStatus(  );
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_TX_PAYLOAD );
    for( i = 0; i < ucLen; i++ ){
        Pygmy_RF24_SPI_WriteByte( ucBuffer[ i ] ); 
    }
    RF24_CS_HIGH;
}

u8 Pygmy_RF24_GetRXPayloadLen( u8 ucPipe )
{
    u8 ucByte;

    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_RX_LEN );
    ucByte = Pygmy_RF24_SPI_ReadByte(  );

    RF24_CS_HIGH;
    
    return( ucByte ); 
}

u8 Pygmy_RF24_GetRXPayload( u8 *ucBuffer )
{
    u8 i, ucPipe, ucLen, ucChar[ 2 ];
    
    RF24_CS_LOW;
    ucChar[ 0 ] = Pygmy_RF24_SPI_ReadByte(  );
    RF24_CS_HIGH;
    if( !ucChar[ 0 ] )
        return( 0 );
    ucLen = Pygmy_RF24_GetRXPayloadLen( ucPipe );
    //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rPacketLen: " );
    //Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%d", ucLen );
    
    RF24_CS_LOW;
    ucChar[ 1 ] = 0;
    Pygmy_RF24_SPI_WriteByte( RF24_RX_PAYLOAD );
    for( i = 0; i < ucLen; i++ ){
        ucChar[ 0 ] = Pygmy_RF24_SPI_ReadByte(  );
        ucBuffer[ i ] = ucChar[ 0 ]; 
    }
    ucBuffer[ i ] = '\0';
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_STATUS );
    Pygmy_RF24_SPI_WriteByte( BIT6 );
    RF24_CS_HIGH;
    
    return( ucLen );
}

void Pygmy_RF24_ConfigAsTX( void )
{
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_CONFIG );
    Pygmy_RF24_SPI_WriteByte( RF24_REG_CONFIG_PWR_UP|RF24_REG_CONFIG_EN_CRC ); // Unique part of address
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_ENRXADDR );
    Pygmy_RF24_SPI_WriteByte( BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_DYNPD );
    Pygmy_RF24_SPI_WriteByte( BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_FEATURE );
    Pygmy_RF24_SPI_WriteByte( RF24_REG_FEATURE_EN_DPL|RF24_REG_FEATURE_EN_ACK_PAY );
    RF24_CS_HIGH;
}


void Pygmy_RF24_ConfigAsRX( void )
{
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_CONFIG );
    Pygmy_RF24_SPI_WriteByte( RF24_REG_CONFIG_PWR_UP|RF24_REG_CONFIG_PRIM_RX|RF24_REG_CONFIG_EN_CRC ); // Unique part of address
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_ENRXADDR );
    Pygmy_RF24_SPI_WriteByte( BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_DYNPD );
    Pygmy_RF24_SPI_WriteByte( BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 );
    RF24_CS_HIGH;
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_FEATURE );
    Pygmy_RF24_SPI_WriteByte( RF24_REG_FEATURE_EN_DPL|RF24_REG_FEATURE_EN_ACK_PAY );
    RF24_CS_HIGH;
}

void Pygmy_RF24_ClearStatus( void )
{
    RF24_CS_LOW;
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|RF24_REG_STATUS );
    Pygmy_RF24_SPI_WriteByte( BIT6|BIT5|BIT4 ); 
    RF24_CS_HIGH;
}

void Pygmy_RF24_WriteAddress( u8 ucReg, u8 *ucAddress )
{
    u8 i;

    // ucAddress is presumed to be default 5 byte mode
    RF24_CS_LOW;
    
    Pygmy_RF24_SPI_WriteByte( RF24_WRITE|ucReg );
    for( i = 0; i < 5; i++ ){
        Pygmy_RF24_SPI_WriteByte( ucAddress[ 4 - i ] ); // byte order must be inverted
    }
    RF24_CS_HIGH;
}

void Pygmy_RF24_SPI_WriteByte( u8 ucByte )
{
	u16 i;
	
    RF24_SCK_LOW;		            // Clock starts low, low-high-low clocks data in or out
    for( i = 0; i < 8; i++ ){ 		// Clock out Write CMD
        if( ucByte & ( BIT7 >> i ) ){		
            RF24_MOSI_HIGH;          // SlaveInMasterOut high if bit set
        } else{
            RF24_MOSI_LOW;           // SlaveInMasterOut low if bit clear
        }
        RF24_SCK_HIGH;				// clock must start low, transition high
        RF24_SCK_LOW;			    // Low transition finishes clock sequence
    }
    
} 

void Pygmy_RF24_SPI_WriteWord( u16 uiWord )
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

void Pygmy_RF24_SPI_WriteLong( u32 ulLong )
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

u8 Pygmy_RF24_SPI_ReadByte( void )
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

void RF24_TX_Packet( void )
{

}

void RF24_RX_Packet( void )
{
    
}
