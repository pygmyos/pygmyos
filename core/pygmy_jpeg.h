
//#pragma once
#ifndef __PYGMY_HEADER_JPEG
	#define __PYGMY_HEADER_JPEG

#include "pygmy_profile.h"


#define DQT      0xDB    // Define Quantization Table
#define SOF      0xC0    // Start of Frame (size information)
#define DHT      0xC4    // Huffman Table
#define SOI      0xD8    // Start of Image
#define SOS      0xDA    // Start of Scan
#define EOI      0xD9    // End of Image, or End of File
#define APP0     0xE0

#define BYTE_TO_WORD(x) (((x)[0]<<8)|(x)[1])


#define HUFFMAN_TABLES        4
#define COMPONENTS            4

#define cY    1
#define cCb    2
#define cCr    3

typedef struct {
                s32 value;                    // Decodes to.
                s32 length;                // Length in bits.
                u16 lCode;    // 2 byte lCode (variable length)
}stBlock;

typedef struct {
                u8    m_length[17];        // 17 values from jpg file, 
                                        // k =1-16 ; L[k] indicates the number of Huffman codes of length k
                u8    m_hufVal[257];        // 256 codes read in from the jpeg file

                s32                m_numBlocks;
                stBlock            m_blocks[1024];
}stHuffmanTable;

typedef struct {
                u32            m_hFactor;
                u32            m_vFactor;
                float *                m_qTable;            // Pointer to the quantisation table to use
                stHuffmanTable*        m_acTable;
                stHuffmanTable*        m_dcTable;
                s16                m_DCT[65];            // DCT coef
                s32                    m_previousDC;
}stComponent;

typedef struct {
                u8*        m_rgb;                // Final Red Green Blue pixel data
                u32        m_width;            // Width of image
                u32        m_height;            // Height of image
                const u8*m_stream;            // Pointer to the current stream
                stComponent            m_component_info[COMPONENTS];
                float                m_Q_tables[COMPONENTS][64];    // quantization tables
                stHuffmanTable        m_HTDC[HUFFMAN_TABLES];        // DC huffman tables  
                stHuffmanTable        m_HTAC[HUFFMAN_TABLES];        // AC huffman tables
                // Temp space used after the IDCT to store each components
                u8        m_Y[64*4];
                u8        m_Cr[64];
                u8        m_Cb[64];
                // Internal Pointer use for colorspace conversion, do not modify it !!!
                u8 *        m_colourspace;
}stJpegData;

s32 DecodeJpgFileData( u8* buf, s32 sizeBuf, u8** rgbpix, u32* width, u32* height);
u8 ConvertJpgFile( u8* szJpgFileInName, u8 *szBmpFileOutName );

#endif // __PYGMY_HEADER_JPEG
