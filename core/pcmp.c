#include "pygmy_file.h"
#include "pygmy_cmd.h"
#include "pygmy_pcmp.h"
#include "pygmy_sys.h"

u8 Pygmy_PCMP_DecompressStreamToFile( u8 *ucBuffer, u16 uiLen, u8 ucCmd, u8 ucCompressed, u8 *ucName )
{
    static PYGMYFILE pygmyPCMPFile;
    static u8 pcmpS1[ 128 ][ PYGMY_PCMP_PLACES + 1 ]; // Limited to 4 places max
    static u8 pcmpS2[ 32 ][ 2 ], ucAttrib, ucTableLen, ucTableIndex, ucLen, ucMode;
    static u16 uiIndex;
    static u8 ucHeader[ 13 ];
    u8 i, ii, iii, ucChar;
    u16 uiByteCount;
 
    if( ucCmd == PYGMY_PCMP_STOP ){
        Pygmy_FILE_Close( &pygmyPCMPFile );
        Pygmy_FILE_Rename( pygmyPCMPFile.Name, ucHeader );
        
        //Debug output
        //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rNaming download: " );
        //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, ucHeader ); // ucHeader will contain the filename
        return( 1 ); 
    } // if
    if( ucCmd == PYGMY_PCMP_START ){
        if( !ucCompressed && ucName[ 0 ]){
            // Filename passed to function only used for uncompressed
            // Temp name used until download completes and then name changed to 
            // contents of ucHeader, so copy ucName to ucHeader
            Pygmy_STRING_StringCopyToString( ucName, ucHeader );
            
        } //if
        if( !Pygmy_FILE_Open( &pygmyPCMPFile, "pftp.tmp", PYGMY_FILE_ATTRIB_WRITE ) ){
            Pygmy_SYS_LogEvent( "File Fail -> PCMP Open Temp" );
            
            return( 0 );
        } // if
        ucMode = PYGMY_PCMP_MODE_PCMP;
        uiIndex = 0; // Header and Table Index
    } // if
    
    //for( uiByteCount = 0; *ucBuffer; ucBuffer++, uiByteCount++ ){
    for( uiByteCount = 0; uiByteCount < uiLen; ucBuffer++, uiByteCount++ ){
        // If Compression not selected, just save the file raw.
        if( !ucCompressed ){
            Pygmy_FILE_PutChar( &pygmyPCMPFile, *ucBuffer );
            continue;
        } // if
        
        if( ucMode == PYGMY_PCMP_MODE_PCMP ){ // We are still processing header
            if( uiIndex == 4 ){ // We have reached Attribute Byte
                ucHeader[ uiIndex ] = '\0';
                ucAttrib = *ucBuffer;
                ucLen = 0;
                uiIndex = 0;
                if( !Pygmy_STRING_TestString( "PCMP", ucHeader ) ){
                    
                    return( 0 ); // Corrupt transfer or incorrect file type
                } else{
                    ucMode = PYGMY_PCMP_MODE_FILENAME; // Time to get the namelen and filename
                    
                }
            } else{ // Add first 4 bytes to ucHeader and test
                ucHeader[ uiIndex++ ] = (u8)*ucBuffer;
            }
            continue;
        } else if( ucMode == PYGMY_PCMP_MODE_FILENAME ){
            if( ucLen == 0 ){
                ucLen = *ucBuffer;
                if( ucLen > 12 ){
                    return( 0 ); // filename too long
                }
                
            } else if( uiIndex == ucLen ){ // We have the entire name, now open
                ucHeader[ uiIndex ] = '\0';
                uiIndex = 0; 
                ucTableIndex = 1; // Table must skip NULL at 0
                ucTableLen = *ucBuffer; // First byte of table is number of entries
                
                if( ucAttrib & PYGMY_PCMP_ENCODING_SECOND ){ // There are two tables if this bit is set
                    ucMode = PYGMY_PCMP_MODE_STAGE2TABLE; // So we start by loading table for Stage2
                } else{
                    ucMode = PYGMY_PCMP_MODE_STAGE1TABLE; // Else we only have table for Stage1 to load
                }
                if( !Pygmy_FILE_Open( &pygmyPCMPFile, ucHeader, PYGMY_FILE_ATTRIB_WRITE ) ){
                    return( 0 ); // Time to panic, we must be out of memory
                }
                //Debug output
                //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rContains: " );
                //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, ucHeader );
            } else if( uiIndex < ucLen ){
                ucHeader[ uiIndex++ ] = *ucBuffer; // Capture filename from stream
            } 
            continue;
        } else if( ucMode == PYGMY_PCMP_MODE_STAGE2TABLE ){
            if( ( ucTableIndex ) == ( ucTableLen  ) ){ // double incrementing will put us 2 past
                ucMode = PYGMY_PCMP_MODE_STAGE1TABLE;
                /* Debug output
                for( i = 1; i < ucTableLen; i++ ){
                    Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\r" );
                    Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%2d", i );
                    Pygmy_SYS_PrintString( PYGMY_SYS_COM3, " S2: " );
                    Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%2X", pcmpS2[ i ][ 0 ] );
                    Pygmy_SYS_PrintString( PYGMY_SYS_COM3, " " );
                    Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%2X", pcmpS2[ i ][ 1 ] );
                }*/
                
                ucTableLen = *ucBuffer; // BIT7 shouldn't be cleared on the first len, or len of lens
                //Debug output
                //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rStage1 Table Found: " );
                //Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%d", ucTableLen );
                uiIndex = 0;
                ucLen = 0;
                ucTableIndex = 0;
            } else {
                if( ucTableIndex == 9 ){
                    for( i = 0; i < 2; i++ ){
                        pcmpS2[ ucTableIndex ][ 0 ] = 0;
                        pcmpS2[ ucTableIndex++ ][ 1 ] = 0;
                    }
                } else if( ucTableIndex == 13 ){
                    //uiIndex = 0;// += 2;
                    ++ucTableIndex;
                }
                pcmpS2[ ucTableIndex ][ uiIndex++ ] = *ucBuffer;
                if( uiIndex == 2 ){
                    uiIndex = 0;
                    ++ucTableIndex;
                }
            }
            continue;
        } else if( ucMode == PYGMY_PCMP_MODE_STAGE1TABLE ){
            
                // Now drop though Data Section and begin decoding
            if( !ucLen ){
                ucTableIndex = 0;
                ucLen = *ucBuffer & ~BIT7; // Clear BIT7

                continue; 
            } else if( uiIndex == ucLen ) {
                pcmpS1[ ucTableIndex ][ uiIndex ] = '\0';
                //Debug output
                //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rEntry: " );
                //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, pcmpS1[ ucTableIndex ] );
                ++ucTableIndex;
                if( ucTableIndex == ucTableLen ){
                    ucMode = PYGMY_PCMP_MODE_DATA;
                } else{
                    ucLen = *ucBuffer & ~BIT7; // Clear BIT7
                    uiIndex = 0;
                    continue;
                }
            } else{
                pcmpS1[ ucTableIndex ][ uiIndex ] = *ucBuffer;
                ++uiIndex;
                continue;
            }
            
            
        }  
        ucChar = *ucBuffer;
        if( ucChar < 32 ){ // Stage2
            if( ucChar == 9 || ucChar == 10 || ucChar == 13 ){ // Exception chars, no substitution
                Pygmy_FILE_PutChar( &pygmyPCMPFile, ucChar );
               
            } else{ // Look up substitute grouping
                // Now check each for Stage1
                for( i = 0; i < 2; i++ ){
                    iii = pcmpS2[ ucChar ][ i ];
                    if( iii < 32 ){
                        if( iii == 9 || iii == 10 || iii == 13 ){
                            Pygmy_FILE_PutChar( &pygmyPCMPFile, iii );
                        
                        } else if( pcmpS2[ iii ][ i ] & 0x80 ){
                            
                            for( ii = 0; pcmpS1[ pcmpS2[ iii ][ i ] & ~0x80 ][ ii ]; ii++ ){ // Output referenced Stage1 grouping 
                                Pygmy_FILE_PutChar( &pygmyPCMPFile, pcmpS1[ pcmpS2[ iii ][ i ] & ~0x80 ][ ii ] );
                            }
                        } else{
                            Pygmy_FILE_PutChar( &pygmyPCMPFile, pcmpS2[ iii ][ i ] ); // Output as-is
                        } // else
                    } else if(  iii & 0x80 ){ // BIT7 marks Stage1 Compressed
                        iii &= ~0x80;
                        
                        for( ii = 0; pcmpS1[ iii ][ ii ]; ii++ ){ // Output referenced Stage1 grouping 
                            Pygmy_FILE_PutChar( &pygmyPCMPFile, pcmpS1[ iii ][ ii ] );
                        }
                    } else{
                        Pygmy_FILE_PutChar( &pygmyPCMPFile, iii );
                    } // else
                } // for
            } // else
        } else if( ucChar & 0x80 ){ // Stage1
            iii = ucChar & ~0x80;
            
            for( ii = 0; pcmpS1[ iii ][ ii ]; ii++ ){ // Output referenced Stage1 grouping 
                Pygmy_FILE_PutChar( &pygmyPCMPFile, pcmpS1[ iii ][ ii ] );
            }
        }else{
            Pygmy_FILE_PutChar( &pygmyPCMPFile, ucChar );
        }
    }  
    
    //Pygmy_SYS_PrintString( PYGMY_SYS_COM3, "\rBytes Written: " );
    //Pygmy_SYS_PrintInteger( PYGMY_SYS_COM3, "%d", uiByteCount  );
    
    return( 1 );
}
