/***************************************************************************/
/*                                                                         */
/*  File: loadjpg.cpp                                                      */
/*  Author: bkenwright@xbdev.net                                           */
/*  Date: 19-01-06                                                         */
/*                                                                         */
/*  Revised: 26-07-07                                                      */
/*                                                                         */
/***************************************************************************/
#include <math.h>
#include "pygmy_profile.h"
#include "pygmy_jpeg.h"

const s32 ZigZagArray[64] = 
{
    0,   1,   5,  6,   14,  15,  27,  28,
    2,   4,   7,  13,  16,  26,  29,  42,
    3,   8,  12,  17,  25,  30,  41,  43,
    9,   11, 18,  24,  31,  40,  44,  53,
    10,  19, 23,  32,  39,  45,  52,  54,
    20,  22, 33,  38,  46,  51,  55,  60,
    21,  34, 37,  47,  50,  56,  59,  61,
    35,  36, 48,  49,  57,  58,  62,  63,
};


/***************************************************************************/

// Clamp our integer between 0 and 255
u8 Clamp( s32 iData )
{
    if( iData < 0 ){
        return( 0 );
    } else if( iData > 255 ){
        return( 255 );
    } else{
        return( iData );
    } // else
}   

/***************************************************************************/

void GenHuffCodes( s32 num_codes, stBlock* arr, u8* huffVal )
{
    s32 hufcounter = 0;
    s32 codelengthcounter = 1;
    s32 cc;

     for( cc = 0; cc < num_codes; cc++ ){
         while ( arr[cc].length > codelengthcounter ){
            hufcounter = hufcounter << 1;
            codelengthcounter++;
         } // while

         arr[cc].lCode = hufcounter;
         arr[cc].value = huffVal[cc];
         hufcounter = hufcounter + 1;
    } // for
}

/***************************************************************************/

float C( s32 u )
{
    if (u == 0){
         return (1.0f/sqrtf(2));
    } else{
         return( 1.0f );
    }
}


s32 func( s32 x, s32 y, s32 block[8][8] )
{
    const float PI = 3.14f;
    float sum=0;
    s32 u, v;

    for( u = 0; u < 8; u++ ){
         for( v = 0; v < 8; v++ ){
             sum += ( C(u) * C(v) ) * block[u][v] * cosf( ((2*x+1) * u * PI) / 16)  * cosf( ((2*y+1) * v * PI) / 16);
         } // for
    }         
    return( (s32) ((1.0/4.0) * sum) );
}

void PerformIDCT( s32 outBlock[8][8], s32 inBlock[8][8] )
{
    s32 y, x;

    for( y = 0; y < 8; y++ ){
        for( x = 0; x < 8; x++ ){
            outBlock[x][y]  =  func( x, y, inBlock);
        } // for
    } // for
}

/***************************************************************************/

void DequantizeBlock( s32 block[64], float quantBlock[64] )
{
    s32 c;

    for( c = 0; c < 64; c++ ){
        block[c] = (s32)(block[c] * quantBlock[c]);
    } // for
}

/***************************************************************************/

void DeZigZag( s32 outBlock[64], s32 inBlock[64])
{
    s32 i;

    for( i = 0; i < 64; i++ ){
        outBlock[ i ] = inBlock[ZigZagArray[i]];
    } // for
}

/***************************************************************************/

void TransformArray( s32 outArray[8][8], s32 inArray[64] )
{
    s32 x, y, cc = 0;

    for( y = 0; y < 8 ; y++ ){
        for( x = 0; x < 8; x++ ){
            outArray[x][y]  =  inArray[cc];
            cc++;
        } // for
    } // for
}

/***************************************************************************/

void DumpDecodedBlock( s32 val[8][8] )
{
    s32 x, y;

    //dprintf("# Decoded 8x8 Block#\n");
    for( y = 0; y < 8; y++ ){
        for( x = 0; x < 8; x++ ){
            //dprintf("%2x ", val[x][y]);
        }
        //dprintf("\n");
    }
}

/***************************************************************************/

void DecodeSingleBlock( stComponent *comp, u8 *outputBuf, s32 stride )
{
    s32 x, y, i, block[ 64 ], arrayBlock[ 8 ][ 8 ], val[ 8 ][ 8 ], data[ 64 ];
    u8 *outptr;
    u16* inptr    = comp->m_DCT;
    float* quantptr = comp->m_qTable;


    // Create a temp 8x8, i.e. 64 array for the data
    for ( i = 0; i < 64; i++ ){
        data[ i ] = 0;
        block[ i ] = 0;
        arrayBlock[ i / 8 ][ i % 8 ] = 0;
        val[ i / 8 ][ i % 8 ] = 0;
    } // for
    // Copy our data into the temp array
    for ( i = 0; i < 64; i++ ){
        data[i] = inptr[i];
        
    } // for

    // De-Quantize
    DequantizeBlock(data, quantptr);

    // De-Zig-Zag
    DeZigZag(block, data);

    // Create an 8x8 array
    TransformArray(arrayBlock, block);

    // Inverse DCT
    PerformIDCT(val, arrayBlock);

    // Level Shift each element (i.e. add 128), and copy to our
    // output
    outptr = outputBuf;
    for ( y = 0; y < 8; y++){
        for ( x = 0; x < 8; x++ ){
            val[x][y] += 128;

            outptr[x] = Clamp(val[x][y]);
        } // for
        outptr += stride;
    } // for

    DumpDecodedBlock(val);
}

/***************************************************************************/

/***************************************************************************/
//
// Save a buffer in 24bits Bitmap (.bmp) format 
//
/***************************************************************************/
/*
void WriteBMP24( const s8* szBmpFileName, s32 Width, s32 Height, u8* RGB )
{
    
    //#pragma pack(1)
    struct stBMFH // BitmapFileHeader & BitmapInfoHeader
    {
        // BitmapFileHeader
        s8         bmtype[2];     // 2 bytes - 'B' 'M'
        u32 iFileSize;     // 4 bytes
        s16    reserved1;     // 2 bytes
        s16    reserved2;     // 2 bytes
        u32 iOffsetBits;   // 4 bytes
        // End of stBMFH structure - size of 14 bytes
        // BitmapInfoHeader
        u32 iSizeHeader;    // 4 bytes - 40
        u32 iWidth;         // 4 bytes
        u32 iHeight;        // 4 bytes
        s16    iPlanes;        // 2 bytes
        s16    iBitCount;      // 2 bytes
        u32 Compression;    // 4 bytes
        u32 iSizeImage;     // 4 bytes
        u32 iXPelsPerMeter; // 4 bytes
        u32 iYPelsPerMeter; // 4 bytes
        u32 iClrUsed;       // 4 bytes
        u32 iClrImportant;  // 4 bytes
        // End of stBMIF structure - size 40 bytes
        // Total size - 54 bytes
    };
    //#pragma pack()

    // Round up the width to the nearest DWORD boundary
    s8 temp[1024];
    s16 iNumPaddedBytes = 4 - (Width * 3) % 4;
    iNumPaddedBytes = iNumPaddedBytes % 4;

    stBMFH bh;
    memset(&bh, 0, sizeof(bh));
    bh.bmtype[0]='B';
    bh.bmtype[1]='M';
    bh.iFileSize = (Width*Height*3) + (Height*iNumPaddedBytes) + sizeof(bh);
    bh.iOffsetBits = sizeof(stBMFH);
    bh.iSizeHeader = 40;
    bh.iPlanes = 1;
    bh.iWidth = Width;
    bh.iHeight = Height;
    bh.iBitCount = 24;

    for( i = 0; i < 1024; i++ ){
        temp[ i ] = 0;
    } // for
    //sprintf(temp, "%s", szBmpFileName);
    FILE* fp = fopen(temp, "wb");
    fwrite(&bh, sizeof(bh), 1, fp);
    for (int y=Height-1; y>=0; y--)
    {
        for (int x=0; x<Width; x++)
        {
            int i = (x + (Width)*y) * 3;
            unsigned int rgbpix = (RGB[i]<<16)|(RGB[i+1]<<8)|(RGB[i+2]<<0);
            fwrite(&rgbpix, 3, 1, fp);
        }
        if (iNumPaddedBytes>0)
        {
            unsigned char pad = 0;
            fwrite(&pad, iNumPaddedBytes, 1, fp);
        }
    }
    fclose(fp);
}*/

/***************************************************************************/

// Takes two array of bits, and build the huffman table for size, and lCode

/***************************************************************************/
void BuildHuffmanTable( u8 *bits, u8 *stream, stHuffmanTable *HT )
{
    s32 i, j, c = 0, numBlocks = 0;

    for ( j = 1; j <= 16; j++ ){
        HT->m_length[j] = bits[j];
    } // for

    // Work out the total number of codes
    for ( i = 1; i <= 16; i++ ){
        numBlocks += HT->m_length[i];
    }
    HT->m_numBlocks = numBlocks;

    // Fill in the data our our blocks, so we know how many bits each
    // one is
    for ( i = 1; i <= 16; i++ ){
        for ( j = 0; j < HT->m_length[i]; j++ ){
            HT->m_blocks[c].length = i;
            c++;
        } // for
    } // for

    GenHuffCodes(HT->m_numBlocks, HT->m_blocks, HT->m_hufVal);
}

/***************************************************************************/
/*
void PrintSOF( const u8 *stream )
{
    s32 width, height, nr_components, precision;

    const s8 *nr_components_to_string[] =    {    "????",
                                                "Grayscale",
                                                "????",
                                                "YCbCr",
                                                "CYMK" };

    precision = stream[2];
    height = BYTE_TO_WORD(stream+3);
    width  = BYTE_TO_WORD(stream+5);
    nr_components = stream[7];

    dprintf("> SOF marker\n");
    dprintf("Size:%dx%d nr_components:%d (%s)  precision:%d\n", 
                            width, height,
                            nr_components, 
                            nr_components_to_string[nr_components],
                            precision);
    
}*/

/***************************************************************************/

s32 ParseSOF( stJpegData *jdata, u8 *stream)
{
    /*
    SOF        16        0xffc0        Start Of Frame
    Lf        16        3Nf+8        Frame header length
    P        8        8            Sample precision
    Y        16        0-65535        Number of lines
    X        16        1-65535        Samples per line
    Nf        8        1-255        Number of image components (e.g. Y, U and V).

    ---------Repeats for the number of components (e.g. Nf)-----------------
    Ci        8        0-255        Component identifier
    Hi        4        1-4            Horizontal Sampling Factor
    Vi        4        1-4            Vertical Sampling Factor
    Tqi        8        0-3            Quantization Table Selector.
    */

    //PrintSOF(stream);

    s32 height = BYTE_TO_WORD(stream+3);
    s32 width  = BYTE_TO_WORD(stream+5);
    s32 nr_components = stream[7];
    s32 i;
    s32 cid                = *stream++;
    s32 sampling_factor = *stream++;
    s32 Q_table            = *stream++;
    
    stream += 8;
    for( i = 0; i < nr_components; i++ ) { 
        stComponent *c = &jdata->m_component_info[cid];
        c->m_vFactor = sampling_factor&0xf;
        c->m_hFactor = sampling_factor>>4;
        c->m_qTable = jdata->m_Q_tables[Q_table];

        /*dprintf("Component:%d  factor:%dx%d  Quantization table:%d\n",
                cid, 
                c->m_vFactor, 
                c->m_hFactor, 
                Q_table );
        */
    }
    jdata->m_width = width;
    jdata->m_height = height;

    return( 0 );
}

/***************************************************************************/

void BuildQuantizationTable( float *qtable, u8 *ref_table )
{
    s32 i, j, c = 0;
    u8 val;

    for ( i = 0; i < 8; i++ ){
        for ( j = 0; j < 8; j++ ) {
            val = ref_table[c];

            qtable[c] = val;
            c++;
        } // for
    } // for
}


/***************************************************************************/

s32 ParseDQT( stJpegData *jdata, u8 *stream )
{
    s32 length, qi, qprecision, qindex;
    float *table;

    dprintf("> DQT marker\n");
    length = BYTE_TO_WORD(stream) - 2;
    stream += 2;    // Skip length

    while (length>0)
    {
        qi = *stream++;

        qprecision = qi>>4;     // upper 4 bits specify the precision
        qindex     = qi&0xf; // index is lower 4 bits

        if ( qprecision ){
            // precision in this case is either 0 or 1 and indicates the precision 
            // of the quantized values;
            // 8-bit (baseline) for 0 and  up to 16-bit for 1 

            //dprintf("Error - 16 bits quantization table is not supported\n");
        } // if

        if ( qindex > 4 ){
            //dprintf("Error - No more 4 quantization table is supported (got %d)\n", qi);
        } // if

        // The quantization table is the next 64 bytes
        table = jdata->m_Q_tables[qindex];

        // the quantization tables are stored in zigzag format, so we
        // use this functino to read them all in and de-zig zag them
        BuildQuantizationTable(table, stream);
        stream += 64;
        length -= 65;
    } // while

    return( 0 );
}

/***************************************************************************/

s32 ParseSOS(stJpegData *jdata, u8 *stream )
{
    /*
    SOS        16        0xffd8            Start Of Scan
    Ls        16        2Ns + 6            Scan header length
    Ns        8        1-4                Number of image components
    Csj        8        0-255            Scan Component Selector
    Tdj        4        0-1                DC Coding Table Selector
    Taj        4        0-1                AC Coding Table Selector
    Ss        8        0                Start of spectral selection
    Se        8        63                End of spectral selection
    Ah        4        0                Successive Approximation Bit High
    Ai        4        0                Successive Approximation Bit Low
    */

    u32 i, cid, table, nr_components = stream[2];

    //dprintf("> SOS marker\n");

    if ( nr_components != 3 ){
        //dprintf("Error - We only support YCbCr image\n");
    }

    stream += 3;
    for ( i = 0; i < nr_components; i++ ) {
        cid   = *stream++;
        table = *stream++;

        if ((table&0xf)>=4){
            //dprintf("Error - We do not support more than 2 AC Huffman table\n");
        } // if
        if ((table>>4)>=4){
            //dprintf("Error - We do not support more than 2 DC Huffman table\n");
        } // if
        //dprintf("ComponentId:%d  tableAC:%d tableDC:%d\n", cid, table&0xf, table>>4);

        jdata->m_component_info[cid].m_acTable = &jdata->m_HTAC[table&0xf];
        jdata->m_component_info[cid].m_dcTable = &jdata->m_HTDC[table>>4];
    }
    jdata->m_stream = stream+3;

    return( 0 );
}

/***************************************************************************/

s32 ParseDHT( stJpegData *jdata, u8 *stream)
{
    /*
    u8 0xff 
    u8 0xc4 (type of segment) 
    u16 be length of segment 
    4-bits class (0 is DC, 1 is AC, more on this later) 
    4-bits table id 
    array of 16 u8 number of elements for each of 16 depths 
    array of u8 elements, in order of depth 
    */

    u32 count, i;
    u8 huff_bits[17];
    s32 length, index;
    u8* huffval;

    length = BYTE_TO_WORD(stream) - 2;
    stream += 2;    // Skip length

    //dprintf("> DHT marker (length=%d)\n", length);

    while (length>0){
        index = *stream++;

        // We need to calculate the number of bytes 'vals' will takes
        huff_bits[0] = 0;
        count = 0;
        for ( i = 1; i < 17; i++ ) {
            huff_bits[i] = *stream++;
            count += huff_bits[i];
        } // for

        if ( count > 256 ){
            //dprintf("Error - No more than 1024 bytes is allowed to describe a huffman table");
        } // if
        if ( (index &0xf) >= HUFFMAN_TABLES ){
            //dprintf("Error - No mode than %d Huffman tables is supported\n", HUFFMAN_TABLES);
        } // if
        //dprintf("Huffman table %s n%d\n", (index&0xf0)?"AC":"DC", index&0xf);
        //dprintf("Length of the table: %d\n", count);

        if (index & 0xf0 ){
            huffval = jdata->m_HTAC[index&0xf].m_hufVal;
            for (i = 0; i < count; i++){
                huffval[i] = *stream++;
            } // for
            BuildHuffmanTable(huff_bits, stream, &jdata->m_HTAC[index&0xf]); // AC
        } else{
            huffval = jdata->m_HTDC[index&0xf].m_hufVal;
            for (i = 0; i < count; i++){
                huffval[i] = *stream++;
            }
            BuildHuffmanTable(huff_bits, stream, &jdata->m_HTDC[index&0xf]); // DC
        } // else

        length -= 1;
        length -= 16;
        length -= count;
    }
    //dprintf("< DHT marker\n");
    return( 0 );
}

/***************************************************************************/

s32 ParseJFIF(stJpegData *jdata, u8 *stream)
{
    s32 chuck_len;
    s32 marker;
    s32 sos_marker_found = 0;
    s32 dht_marker_found = 0;

    // Parse marker
    while (!sos_marker_found){
        if (*stream++ != 0xff){
            goto bogus_jpeg_format;
        } // if

        // Skip any padding ff byte (this is normal)
        while (*stream == 0xff){
            stream++;
        } // while

        marker = *stream++;
        chuck_len = BYTE_TO_WORD(stream);

        switch (marker){
            case SOF:
                if (ParseSOF(jdata, stream) < 0){
                    return( -1 );
                } // if
                break;
            case DQT:
                if (ParseDQT(jdata, stream) < 0){
                    return( -1 );
                } // if
                break;
            case SOS:
                if (ParseSOS(jdata, stream) < 0){
                    return( -1 );
                } // if
                sos_marker_found = 1;
                break;
            case DHT:
                if (ParseDHT(jdata, stream) < 0){
                    return( -1 );
                } // if
                dht_marker_found = 1;
                break;

            // The reason I added these additional skips here, is because for
            // certain jpg compressions, like swf, it splits the encoding 
            // and image data with SOI & EOI extra tags, so we need to skip
            // over them here and decode the whole image
            case SOI:
            case EOI:
                chuck_len = 0;
                break;
            case 0xDD: //DRI: Restart_markers=1;
                //dprintf("DRI - Restart_marker\n");
                break;
            case APP0:
                //dprintf("APP0 Chunk ('txt' information) skipping\n");
                break;
            default:
                //dprintf("ERROR> Unknown marker %2.2x\n", marker);
                break;
        } // switch
        stream += chuck_len;
    }

    if (!dht_marker_found) {
        //dprintf("ERROR> No Huffman table loaded\n");
    }

    return( 0 );

    bogus_jpeg_format:
    //dprintf("ERROR> Bogus jpeg format\n");
    return( -1 );
}

/***************************************************************************/

s32 JpegParseHeader(stJpegData *jdata, u8 *buf, u32 size)
{
    u8* startStream;
    s32 fileSize;

    // Identify the file
    if ((buf[0] != 0xFF) || (buf[1] != SOI)){
        //dprintf("Not a JPG file ?\n");
        return( -1 );
    } // if

    startStream = buf+2;
    fileSize = size-2;

    //dprintf("-|- File thinks its size is: %d bytes\n", fileSize);

    return( ParseJFIF(jdata, startStream) );
}

/***************************************************************************/

void JpegGetImageSize(stJpegData *jdata, u32 *width, u32 *height)
{
    *width  = jdata->m_width;
    *height = jdata->m_height;
}

/***************************************************************************/

u32 g_reservoir = 0;
u32 g_nbits_in_reservoir = 0;

void FillNBits( u8** stream, s32 nbits_wanted)
{
    u8 c;

    while( (s32)g_nbits_in_reservoir < nbits_wanted ){
        c = *(*stream)++;
        g_reservoir <<= 8;
        if (c == 0xff && (**stream) == 0x00){
            (*stream)++;
        } // if
        g_reservoir |= c;
        g_nbits_in_reservoir+=8;
    } // while
}

s16 GetNBits( u8** stream, s32 nbits_wanted )
{
    s16 result;

    FillNBits(stream, nbits_wanted);
    
    result = ((g_reservoir)>>(g_nbits_in_reservoir-(nbits_wanted))); 

    g_nbits_in_reservoir -= (nbits_wanted); 
    g_reservoir &= ((1U<<g_nbits_in_reservoir)-1);

    return( result );
}

s32 LookNBits( u8** stream, s32 nbits_wanted )
{
    FillNBits(stream, nbits_wanted);

    return( ((g_reservoir)>>(g_nbits_in_reservoir-(nbits_wanted))) );
}

void SkipNBits( u8** stream, s32 nbits_wanted )
{
    FillNBits(stream, nbits_wanted);

    g_nbits_in_reservoir -= (nbits_wanted); 
    g_reservoir &= ((1U<<g_nbits_in_reservoir)-1);
}


/***************************************************************************/


u8 IsInHuffmanCodes( s32 lCode, s32 numCodeBits, s32 numBlocks, stBlock* blocks, s32* outValue)
{
    s32 j, hufhCode, hufCodeLenBits, hufValue;

    for( j = 0; j < numBlocks; j++ ){
        hufhCode        = blocks[j].lCode;
        hufCodeLenBits    = blocks[j].length;
        hufValue        = blocks[j].value;

        // We've got a match!
        if ((lCode==hufhCode) && (numCodeBits==hufCodeLenBits)){
            *outValue = hufValue;
            return( TRUE );
        } // if
    } // for

    return( FALSE );
}

/***************************************************************************/

s32 DetermineSign( s32 val, s32 nBits)
{
    u8 negative = val < (1<<(nBits-1));

    if( negative ){
        // (-1 << (s)), makes the last bit a 1, so we have 1000,0000 for example for 8 bits

        val = val + (-1 << (nBits)) + 1; 
    } // if

    // Else its unsigned, just return
    return( val );
}

/***************************************************************************/

s8 g_bigBuf[1024] = {0};
s8* IntToBinary( s32 val, s32 bits)
{
    s32 i, c = 0;
    u8 on;

    for( i=0; i < 32; i++ ){ 
        g_bigBuf[i]='\0';
    } // for
    for( i = bits-1; i >= 0; i-- ){
        on = (val & (1<<i)) ? 1 : 0;
        g_bigBuf[c] = on ? '1' : '0';
        c++;
    } // for

    return( &g_bigBuf[0] );
}

/***************************************************************************/

void DumpHufCodes(stHuffmanTable* table)
{
    s32 i;

    //dprintf("HufCodes\n");
    //dprintf("Num: %d\n", table->m_numBlocks);
    for ( i = 0; i < table->m_numBlocks; i++ ){
        //dprintf("%03d\t [%s]\n", i, IntToBinary(table->m_blocks[i].lCode, table->m_blocks[i].length));
    }
    //dprintf("\n");
}

/***************************************************************************/

void DumpDCTValues( s16 dct[64])
{
    //dprintf("\n#Extracted DCT values from SOS#\n");
    s32 i, c = 0;
    for ( i = 0; i < 64; i++ ){
        //dprintf("% 4d  ", dct[c++]);
        if ( (c>0) && (c%8==0) ){
            //dprintf("\n");
        } // if
    } // for
    //dprintf("\n");
}


/***************************************************************************/

void ProcessHuffmanDataUnit(stJpegData *jdata, s32 indx)
{
    stComponent *c = &jdata->m_component_info[indx];

    // Start Huffman decoding

    // We memset it here, as later on we can just skip along, when we have lots
    // of leading zeros, for our AC run length encoding :)
    s32 nr, j, k, n, lCode, numDataBits, valCode;
    s16 DCT_tcoeff[64];
    s16 data;
    u8 EOB_found, found = FALSE;
    s32 decodedValue = 0;
    u8 size_val, count_0;

    memset(DCT_tcoeff, 0, sizeof(DCT_tcoeff)); //Initialize DCT_tcoeff

    //    DumpHufCodes(c->m_dcTable);
    //    DumpHufCodes(c->m_acTable);

    //dprintf("\nHuff Block:\n\n");


    // First thing is get the 1 DC coefficient at the start of our 64 element
    // block
    for( k = 1; k < 16; k++ ){
        // Keep grabbing one bit at a time till we find one thats a huffman lCode
        lCode = LookNBits(&jdata->m_stream, k);

        // Check if its one of our huffman codes
        if (IsInHuffmanCodes(lCode, k,  c->m_dcTable->m_numBlocks, c->m_dcTable->m_blocks, &decodedValue)){
            // Skip over the rest of the bits now.
            SkipNBits(&jdata->m_stream, k);

            found = TRUE;

            // The decoded value is the number of bits we have to read in next
            numDataBits = decodedValue;

            // We know the next k bits are for the actual data
            if (numDataBits==0){
                DCT_tcoeff[0] = c->m_previousDC;
            } else{
                data = GetNBits(&jdata->m_stream, numDataBits);

                data = DetermineSign(data, numDataBits);

                DCT_tcoeff[0] = data + c->m_previousDC;
                c->m_previousDC = DCT_tcoeff[0];
            } // else

            // Found so we can exit out
            break;
        }
    }

    if (!found){
        //dprintf("-|- ##ERROR## We have a *serious* error, unable to find huffman lCode\n");
    }

    // Second, the 63 AC coefficient
    nr = 1; 
    EOB_found = FALSE;
    while ( (nr<=63)&&(!EOB_found) ){
        //k = 0;
        for ( k=1; k<=16; k++){
            // Keep grabbing one bit at a time till we find one thats a huffman lCode
            lCode = LookNBits(&jdata->m_stream, k);

            // Check if its one of our huffman codes
            if (IsInHuffmanCodes(lCode, k,  c->m_acTable->m_numBlocks, c->m_acTable->m_blocks, &decodedValue)){
                // Skip over k bits, since we found the huffman value
                SkipNBits(&jdata->m_stream, k);

                // Our decoded value is broken down into 2 parts, repeating RLE, and then
                // the number of bits that make up the actual value next
                valCode = decodedValue;

                size_val = valCode&0xF;    // Number of bits for our data
                count_0  = valCode>>4;    // Number RunLengthZeros

                if (size_val==0) {// RLE 
                    if (count_0==0){
                        EOB_found = TRUE;    // EOB found, go out
                    } else if (count_0==0xF){
                        nr+=16;  // skip 16 zeros
                    } // else if
                } else{
                    nr+=count_0; //skip count_0 zeroes
                    
                    if (nr > 63){
                        //dprintf("-|- ##ERROR## Huffman Decoding\n");
                    } // if

                    data = GetNBits(&jdata->m_stream, size_val );
                    data = DetermineSign(data, size_val);

                    DCT_tcoeff[nr++]=data;
                } // else
                break;
            }
        }

        if (k>16){    
            nr++;
        } // if
    }

    DumpDCTValues(DCT_tcoeff);

    // We've decoded a block of data, so copy it across to our buffer
    for ( j = 0; j < 64; j++){
        c->m_DCT[j] = DCT_tcoeff[j];
    } // for
}

/***************************************************************************/

void ConvertYCrCbtoRGB( s32 y, s32 cb, s32 cr, s32* r, s32* g, s32* b)
{
    float red, green, blue;

    red   = y + 1.402f*(cb-128);
    green = y-0.34414f*(cr-128)-0.71414f*(cb-128);
    blue  = y+1.772f*(cr-128);

    *r = (s32) Clamp((s32)red);
    *g = (s32) Clamp((s32)green);
    *b = (s32) Clamp((s32)blue);
}

/***************************************************************************/

void YCrCB_to_RGB24_Block8x8( stJpegData *jdata, s32 w, s32 h, s32 imgx, s32 imgy, s32 imgw, s32 imgh)
{
    u8 *Y, *Cb, *Cr;
    u8 *pix;
    s32 r, g, b;
    s32 olw = 0; // overlap
    s32 olh = 0; // overlap
    s32 y, x, poff, yoff, coff, yc, cb, cr;

    Y  = jdata->m_Y;
    Cb = jdata->m_Cb;
    Cr = jdata->m_Cr;
   
    if ( imgx > (imgw-8*w) ){
        olw = imgw-imgx;
    } // if

    if ( imgy > (imgh-8*h) ){
        olh = imgh-imgy;
    } // if

    for ( y=0; y<(8*h - olh); y++){
        for ( x=0; x<(8*w - olw); x++)
        {
            poff = x*3 + jdata->m_width*3*y;
            pix = &(jdata->m_colourspace[poff]);
            
            yoff = x + y*(w*8);
            coff = (s32)(x*(1.0f/w)) + (s32)(y*(1.0f/h))*8;

            yc =  Y[yoff];
            cb = Cb[coff];
            cr = Cr[coff];

            ConvertYCrCbtoRGB(yc,cr,cb,&r,&g,&b);

            pix[0] = Clamp(r);
            pix[1] = Clamp(g);
            pix[2] = Clamp(b);
        } // for
    } // for
}

/***************************************************************************/
//
//  Decoding
//  .-------.
//  | 1 | 2 |
//  |---+---|
//  | 3 | 4 |
//  `-------'
//
/***************************************************************************/
void DecodeMCU( stJpegData *jdata, s32 w, s32 h)
{
    s32 x, y, stride, offset;

    // Y
    for ( y = 0; y < h; y++ ){
        for ( x = 0; x < w; x++ ){
            stride = w*8;
            offset = x*8 + y*64*w;

            ProcessHuffmanDataUnit(jdata, cY);

            DecodeSingleBlock(&jdata->m_component_info[cY], &jdata->m_Y[offset], stride);
        } // for
    } // for

    // Cb
    ProcessHuffmanDataUnit(jdata, cCb);
    DecodeSingleBlock(&jdata->m_component_info[cCb], jdata->m_Cb, 8);

    // Cr
    ProcessHuffmanDataUnit(jdata, cCr);
    DecodeSingleBlock(&jdata->m_component_info[cCr], jdata->m_Cr, 8);
}

/***************************************************************************/

s32 JpegDecode( stJpegData *jdata )
{
    s32 x, y, h, w, height, width, xstride_by_mcu, ystride_by_mcu;
    s32 hFactor = jdata->m_component_info[cY].m_hFactor;
    s32 vFactor = jdata->m_component_info[cY].m_vFactor;
    u32 bytes_per_blocklines, bytes_per_mcu;

    // RGB24:
    if ( jdata->m_rgb == NULL){
        h = jdata->m_height*3;
        w = jdata->m_width*3;
        height = h + (8*hFactor) - (h%(8*hFactor));
        width  = w + (8*vFactor) - (w%(8*vFactor));
        jdata->m_rgb = sysAllocate( width * height ); //u8[width * height];

        memset(jdata->m_rgb, 0, width*height);
    } // if

    jdata->m_component_info[0].m_previousDC = 0;
    jdata->m_component_info[1].m_previousDC = 0;
    jdata->m_component_info[2].m_previousDC = 0;
    jdata->m_component_info[3].m_previousDC = 0;

    xstride_by_mcu = 8*hFactor;
    ystride_by_mcu = 8*vFactor;

    // Don't forget that block can be either 8 or 16 lines
    bytes_per_blocklines = jdata->m_width*3 * ystride_by_mcu;

    bytes_per_mcu = 3*xstride_by_mcu;

    // Just the decode the image by 'macroblock' (size is 8x8, 8x16, or 16x16)
    for ( y=0 ; y<(int)jdata->m_height; y+=ystride_by_mcu){
        for ( x=0; x<(int)jdata->m_width; x+=xstride_by_mcu){
            jdata->m_colourspace = jdata->m_rgb + x*3 + (y *jdata->m_width*3);

            // Decode MCU Plane
            DecodeMCU(jdata, hFactor, vFactor );

            YCrCB_to_RGB24_Block8x8(jdata, hFactor, vFactor, x, y, jdata->m_width, jdata->m_height);
        } // for
    } // for


    return( 0 );
}

/***************************************************************************/
//
// Take Jpg data, i.e. jpg file read into memory, and decompress it to an
// array of rgb pixel values.
//
// Note - Memory is allocated for this function, so delete it when finished
//
/***************************************************************************/
s32 DecodeJpgFileData( u8* buf, s32 sizeBuf, u8** rgbpix, u32* width, u32* height) 
{
    
    // Allocate memory for our decoded jpg structure, all our data will be
    // decompressed and stored in here for the various stages of our jpeg decoding
    stJpegData* jdec;
    
    jdec = sysAllocate( sizeof( stJpegData ) );//new stJpegData();
    if (jdec == NULL){
        //dprintf("Not enough memory to alloc the structure need for decompressing\n");
        return( 0 );
    } // if

    // Start Parsing.....reading & storing data
    if (JpegParseHeader(jdec, buf, sizeBuf)<0){
        //dprintf("ERROR > parsing jpg header\n");
    }

    // We've read it all in, now start using it, to decompress and create rgb values
    //dprintf("Decoding JPEG image...\n");
    JpegDecode(jdec);

    // Get the size of the image
    JpegGetImageSize(jdec, width, height);

    *rgbpix = jdec->m_rgb;

    // Release the memory for our jpeg decoder structure jdec
    sysFree( jdec );

    return( 1 );
}

/***************************************************************************/
//
// Load one jpeg image, and decompress it, and save the result.
//
/***************************************************************************/
u8 ConvertJpgFile( u8* szJpgFileInName, u8 *szBmpFileOutName )
{
    PYGMYFILE fp;
    u32 lengthOfFile;
    u8 *buf, *rgbpix;
    u32 width, height;

    // Load the Jpeg into memory
    if ( !fileOpen( &fp, szJpgFileInName, READ ) ){
        print( COM3, "Cannot open jpg file: %s\r", szJpgFileInName );
        return( FALSE );
    } // if

    lengthOfFile = fileLength( &fp );
    buf = sysAllocate( lengthOfFile + 4 ); //new unsigned char[lengthOfFile + 4];
    if ( !buf ){
        print( COM3, "Memory Full\r" );
        return( FALSE );
    } // if
    fileGetBuffer( &fp, lengthOfFile, buf );

    rgbpix = NULL;
    width  = 0;
    height = 0;
    DecodeJpgFileData(buf, lengthOfFile, &rgbpix, &width, &height);

    if ( rgbpix==NULL ){
        print( COM3, "Failed to decode jpg\r" );
        return( FALSE );
    } // if

    // Delete our data we read in from the file
    sysFree( buf );

    // Save it
    WriteBMP24(szBmpFileOutName, width, height, rgbpix);

    // Since we don't need the pixel information anymore, we must
    // release this as well
    sysFree( rgbpix );

    return( TRUE );
}
