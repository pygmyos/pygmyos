/**************************************************************************
    pygappend ( Pygmy Operating System Utilities: Append Image to Imagestrip )
    Copyright (C) 2011  Warren D Greenway

    This file is part of pygappend.

    pygappend is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    pygappend is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with pygappend.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "pygres.h"

#define MAXFILESIZE	650000

typedef struct  {
			unsigned int uiType; 
			unsigned int uiTableSize; 
			unsigned int uiDimensionType; 
			unsigned int uiDimensionSize; 
			unsigned int uiDrawStyle; 
			unsigned int uiVersion; 
			unsigned int uiBPP; 
		}PygResHeader;
		
typedef struct {
				unsigned int uiWidth;
				unsigned int uiHeight;
				unsigned int uiImageWidth;
				unsigned int uiImageHeight;
				unsigned int uiPygmyInfo;
				unsigned long ulIndex;
				unsigned char *ucpImage;
				}PYGMYImageIndex;

typedef struct {
				unsigned long ulLen;
				unsigned long ulIndex;
				unsigned char *ucpBuffer;
				}PYGMYVirtualFile;
		

const unsigned char ucBitMask[8] = { BIT7, BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };

void PygmyRes_List( unsigned int uiPygmyInfo );
unsigned int PygmyRes_ListTable( unsigned int uiPygmyInfo, unsigned char *ucpIn );
unsigned int PygmyRes_GetBytesPerTableEntry( unsigned int uiPygmyInfo );
void PygmyRes_RecalculateForAppend( unsigned int uiBytesPerEntry, unsigned long ulInLen, unsigned int uiTableEntries, unsigned char *ucpTable );
//void PygmyRes_WriteTable( unsigned int uiBytesPerEntry, unsigned int uiTableEntries, unsigned char *ucpTable, FILE *fileOut );
void PygmyRes_WriteTable( unsigned int uiBytesPerEntry, unsigned int uiTableEntries, unsigned char *ucpTable, PYGMYVirtualFile *pygmyOut );
void Pygmy_fputc( unsigned char ucByte, PYGMYVirtualFile *pygmyOut );
void Pygmy_InternalAppend( PYGMYImageIndex *pygmyImageIndex, PYGMYVirtualFile *pygmyIn );

int main ( int argc, char *argv[] )
{
	unsigned long i, ulIndex, ulInLen, ulAppendLen;
	unsigned int uiPygmyInfo, uiTableEntries, uiTableSize;
	unsigned char ucVersion, ucByte, ucpCommandLine[40];
	PYGMYVirtualFile pygmyOut;
	FILE *fileIn, *fileOut, *fileAppend;
	unsigned char ucpIn[ MAXFILESIZE ];
	unsigned char ucFileBuffer[ MAXFILESIZE ];
	unsigned char ucpAppend[ MAXFILESIZE ];
	
	if( argv[1] ){
		fileIn = fopen( argv[1], "rb" );
		if( !fileIn ){
			printf( "\nFailed to open file %s!", argv[1] );
			exit( 1 );
		}
	}
	else{
		printf( "Please specify Input, usage Input Output Append.\n" );
		exit( 1 );
	}
	// Buffer Input contents
	for( i = 0, ulInLen = 0; !feof( fileIn ) && i <MAXFILESIZE; ){
		ucByte = fgetc( fileIn );
		if( !feof( fileIn ) ){
			ucpIn[ i++ ] = ucByte;
			++ulInLen;
		}
	} 
	printf( "Input %s loaded, %d bytes\n", argv[1], ulInLen );
	fclose( fileIn ); // close for re-open
	
	if( argv[2] ){
		fileAppend = fopen( argv[2], "rb" ); 
		if( !fileAppend ){
			printf( "\nFailed to open file %s!", argv[2] );
			
			exit( 1 );
		}
	}
	else{
		printf( "Please specify Append, usage Input Output Append.\n" );
		exit( 1 );
	}
	
	fileOut = fopen( argv[1], "wb" );
	if( !fileOut ){
		printf( "\nEncountered unexpected file system failure!" );
		exit( 1 );
	}
	/*if( argv[2] ){
		fileOut = fopen( argv[2], "wb" );
		if( !fileOut ){
			printf( "\nFailed to open file %s!", argv[2] );
			exit( 1 );
		}
	}
	else{
		printf( "Please specify Output, usage Input Output Append.\n" );
		exit( 1 );
	}*/
	
	
	//Initialize the Virtual File
	pygmyOut.ucpBuffer = ucFileBuffer;
	pygmyOut.ulLen = 0;
	pygmyOut.ulIndex = 0;
	
	

	for( i = 0, ulAppendLen = 0; !feof( fileAppend ) && i <MAXFILESIZE;  ){
		ucByte = fgetc( fileAppend );
		if( !feof( fileAppend ) ){
			ucpAppend[ i++ ] = ucByte;
			++ulAppendLen;
		}
	} 
	printf( "Input %s loaded, %d bytes\n", argv[3], ulInLen );
	
	ulIndex = 0;
	uiPygmyInfo = ( unsigned int )( ucpIn[ 0 ] << 8 ) | ( unsigned int )ucpIn[ 1 ]; // First 16bits is Pygmy Resource Header
	PygmyRes_List( uiPygmyInfo );
	ulIndex+=2;
	printf( "Raw Table: %X\n", uiPygmyInfo );
	//scanf( "%s", ucpCommandLine );
	//printf( "\n" );
	//uiPygmyInfo |= PYGRES_IMAGESTRIP; // For testing
	Pygmy_fputc( ( unsigned char )( unsigned int ) ( ( uiPygmyInfo | PYGRES_IMAGESTRIP ) >> 8 ),  &pygmyOut );
	Pygmy_fputc( ( unsigned char )( uiPygmyInfo | PYGRES_IMAGESTRIP ), &pygmyOut );
	if( uiPygmyInfo & PYGRES_VERSION ){
		ucVersion = ucpIn[ 2 ];
		++ulIndex;
		printf( "Extended Version: V%d\n", ucVersion );
		Pygmy_fputc( ucVersion, &pygmyOut );
	}
	if( ( uiPygmyInfo & PYGRES_IMAGESTRIP ) || ( uiPygmyInfo & PYGRES_ANIMATION ) || ( uiPygmyInfo & PYGRES_VIDEO )
		|| (uiPygmyInfo & PYGRES_ARCHIVE) || ( uiPygmyInfo & PYGRES_FONT ) ){
		
		uiTableEntries = PygmyRes_ListTable( uiPygmyInfo, ( unsigned char * )( ucpIn + ulIndex ) );
		
		uiTableSize = PygmyRes_GetBytesPerTableEntry( uiPygmyInfo );
		ulIndex += 2;
		
		// Following code needs to be re-written with typecasting enforced and CRC calculation and replace code added
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiTableEntries + 1 ) >> 8, &pygmyOut ); 	// Write Table after Increment
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiTableEntries + 1 ), &pygmyOut ); 		//
		PygmyRes_WriteTable( uiTableSize, uiTableEntries, ( unsigned char * )( ucpIn + ulIndex ), &pygmyOut );
		++uiTableEntries;
		ulIndex += uiTableSize * uiTableEntries ;
		switch ( uiPygmyInfo & PYGRES_TABLEMASK ){
			case PYGRES_TABLE16:
				// Next Output Table Entry for Appended Image
				//  Table Entry Value = LEN + 1 2byte entry
				Pygmy_fputc( ( unsigned char ) ( unsigned int )( ( ulInLen + 2 ) >> 8), &pygmyOut ); 
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 2 ), &pygmyOut );
				break;
			case PYGRES_TABLE32:
				// Next Output Table Entry for Appended Image
				// Table Entry Value = LEN + 1 4byte entry
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 24 ), &pygmyOut ); 
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 16 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 8 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 4 ), &pygmyOut );
				break;
		}
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiPygmyInfo >> 8 ), &pygmyOut ); // Output original header for original image at image start, after table
		Pygmy_fputc( ( unsigned char ) uiPygmyInfo, &pygmyOut );
	}
	else{
		Pygmy_fputc( ( unsigned char ) 0, &pygmyOut ); // New Table is 0x02, original and first appended images
		Pygmy_fputc( ( unsigned char ) 2, &pygmyOut ); //
		switch ( uiPygmyInfo & PYGRES_TABLEMASK ){
			case PYGRES_TABLE16:
				// First Output Table Entry to Current Image, start at end of new table
				// 10 bytes = 16bit table entries field and two 16bit entries 
				Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( ( ulIndex + 6 ) >> 8 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulIndex + 6 ), &pygmyOut );
				// Next Output Table Entry for Initial Appended Image
				// 2 bytes table entries, 2 2bytes entries, 2 bytes duplicate header
				Pygmy_fputc( ( unsigned char ) ( unsigned int )( ( ulInLen + 8 ) >> 8), &pygmyOut ); 
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 8 ), &pygmyOut );
				break;
			case PYGRES_TABLE32:
				// First Output Table Entry to Current Image, start at end of new table 
				// 18 bytes = 16bit table entries field and 2 32bit entries
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulIndex + 12 ) >> 24 ), &pygmyOut );				
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulIndex + 12 ) >> 16 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulIndex + 12 ) >> 8 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulIndex + 12 ), &pygmyOut );
				// Next Output Table Entry for Initial Appended Image
				// 2 bytes table entries, 2 4bytes entries, 2 bytes duplicate header
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 14 ) >> 24 ), &pygmyOut ); 
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 14 ) >> 16 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 14 ) >> 8 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 14 ), &pygmyOut );
				break;
		}
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiPygmyInfo >> 8 ), &pygmyOut ); // Output original header for original image at image start, after table
		Pygmy_fputc( ( unsigned char ) uiPygmyInfo, &pygmyOut );
		printf( "Output new table entry for appended resource %d\n", ( unsigned int ) ulInLen + 1 );
	}

	// Add code for dumping table and appending new entry to table
	
	// Add code for dumping remainder of fileIn to fileOut and dumping fileAppend to end of fileOut
	for( i = ulIndex; i < ulInLen; i++ ){
		Pygmy_fputc( ucpIn[ i ], &pygmyOut );
	}
	// The following code assumes that the file is strictly an image without extended Version, CRC, or table 
	// In this case, CRC from file being appended would be discarded
	
	for( i = 0; i < ulAppendLen; i++ ){
		Pygmy_fputc( ucpAppend[ i ], &pygmyOut );
	}
	for( i = 0; i < pygmyOut.ulLen; i++ )
		fputc( pygmyOut.ucpBuffer[ i ], fileOut );
	printf( "Append complete\n" );
	
	if( fileIn )
		fclose( fileIn );
	if( fileOut )
		fclose( fileOut );
	if( fileAppend )
		fclose( fileAppend );
}


void Pygmy_fputc( unsigned char ucByte, PYGMYVirtualFile *pygmyOut )
{
	++pygmyOut->ulLen;
	pygmyOut->ucpBuffer[ pygmyOut->ulIndex++ ] = ucByte;
	
}
/*
void Pygmy_InternalAppend( PYGMYImageIndex *pygmyImageIndex, PYGMYVirtualFile *pygmyIn )
{
	unsigned long i, ii, ulIndex, ulInLen;
	unsigned int uiPygmyInfo, uiTableEntries, uiTableSize;
	unsigned char ucVersion, ucByte;
	unsigned char ucpIn[ MAXFILESIZE ], ucpOut[ MAXFILESIZE ];
	PYGMYVirtualFile pygmyOut;
	
	pygmyOut.ulLen = 0;
	pygmyOut.ulIndex = 0;
	pygmyOut.ucpBuffer = ucpOut;
	
	ulInLen = pygmyIn->ulLen;
	
	for( i = 0; i < pygmyIn->ulLen; i++ )
		ucpIn[ i ] = pygmyIn->ucpBuffer[ i ];
	
	printf( "\nFont Size at %d bytes", ulInLen );
	ulIndex = 0;
	uiPygmyInfo = ( unsigned int )( ucpIn[ 0 ] << 8 );	// First 16bits is Pygmy Resource Header
	uiPygmyInfo |= ( unsigned int )ucpIn[ 1 ]; 			//

	ulIndex+=2;

	Pygmy_fputc( ( unsigned char )( unsigned int ) ( ( uiPygmyInfo ) >> 8 ), &pygmyOut );
	Pygmy_fputc( ( unsigned char )( uiPygmyInfo ), &pygmyOut );
	// Extended version not supported
	
	if( ( uiPygmyInfo & PYGRES_IMAGESTRIP ) || ( uiPygmyInfo & PYGRES_FONT ) ){
		
		uiTableEntries = PygmyRes_ListTable( uiPygmyInfo, ( unsigned char * )( ucpIn + ulIndex ) );
		
		uiTableSize = PygmyRes_GetBytesPerTableEntry( uiPygmyInfo );
		ulIndex += 2;
		
		// Following code needs to be re-written with typecasting enforced and CRC calculation and replace code added
		// Write Table after Increment
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiTableEntries + 1) >> 8, &pygmyOut );
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( uiTableEntries + 1 ), &pygmyOut );
		PygmyRes_WriteTable( uiTableSize, uiTableEntries, ( unsigned char * )( ucpIn + ulIndex ), &pygmyOut );
		++uiTableEntries;
		ulIndex += uiTableSize * uiTableEntries ;
		switch ( uiPygmyInfo & PYGRES_TABLEMASK ){
			case PYGRES_TABLE16:
				// Next Output Table Entry for Appended Image
				//  Table Entry Value = LEN + 1 2byte entry
				Pygmy_fputc( ( unsigned char ) ( unsigned int )( ( ulInLen + 2 ) >> 8), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 2 ), &pygmyOut );
				break;
			case PYGRES_TABLE32:
				// Next Output Table Entry for Appended Image
				// Table Entry Value = LEN + 1 4byte entry 
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 24 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 16 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( unsigned long ) ( ( ulInLen + 4 ) >> 8 ), &pygmyOut );
				Pygmy_fputc( ( unsigned char ) ( ulInLen + 4 ), &pygmyOut );
				break;
		}
		
		// Output original header for original image at image start, after table
		Pygmy_fputc( ( unsigned char ) ( unsigned int ) ( pygmyImageIndex->uiPygmyInfo >> 8 ), &pygmyOut );
		Pygmy_fputc( ( unsigned char ) pygmyImageIndex->uiPygmyInfo, &pygmyOut );
	}
	
	// copy remainder of original file to prepare for append
	for( i = ulIndex; i < ulInLen; i++ ){
		Pygmy_fputc( ucpIn[ i ], &pygmyOut );
	}
	// The following code assumes that the file is strictly an image without extended Version, CRC, or table 
	// In this case, CRC from file being appended would be discarded
			
	Pygmy_fputc( ( unsigned char ) ( unsigned int ) (  pygmyImageIndex->uiPygmyInfo >> 8 ), &pygmyOut );// Output Header to Binary							
	Pygmy_fputc( ( unsigned char ) pygmyImageIndex->uiPygmyInfo, &pygmyOut ); 							// Output Header to Binary						 
	Pygmy_fputc( ( unsigned char ) pygmyImageIndex->uiWidth, &pygmyOut );								// Output Width to Binary						 	
	Pygmy_fputc( ( unsigned char ) pygmyImageIndex->uiHeight, &pygmyOut );								// Output Height to Binary
	
	Pygmy_ImageEncode( pygmyImageIndex->uiWidth, pygmyImageIndex->uiHeight, pygmyImageIndex->ucpImage, &pygmyOut ); 
	
	// copy finished appended file back to original
	for( i = 0; i < pygmyOut.ulLen; i++ )
		pygmyIn->ucpBuffer[ i ] = pygmyOut.ucpBuffer[ i ];
	pygmyIn->ulLen = pygmyOut.ulLen;
	pygmyIn->ulIndex = pygmyOut.ulIndex;
	
}
*/

void PygmyRes_WriteTable( unsigned int uiBytesPerEntry, unsigned int uiTableEntries, unsigned char *ucpTable, PYGMYVirtualFile *pygmyOut )
{
	unsigned long ulEntryValue;
	unsigned int uiEntry, i;
	
	// First all entries currently in table must be incremented by Bytes Per Entry to correct for additional entry at end of table
	for( uiEntry = 0; uiEntry < uiTableEntries; uiEntry++ ){ 
		switch( uiBytesPerEntry ){
			case 2:
				ulEntryValue = ( unsigned int ) ucpTable[ uiEntry * 2 ] << 8;
				ulEntryValue |= ( unsigned int ) ucpTable[ ( uiEntry * 2 ) + 1 ];
				printf( "\nTableEntry %d From 0x%X to", uiEntry, ulEntryValue );
				ulEntryValue += 2;
				printf( "0x%X", ulEntryValue );
				ucpTable[ ( uiEntry * 2 ) ] = ( unsigned int ) ulEntryValue >> 8;
				ucpTable[ ( uiEntry * 2 ) + 1 ] = ( unsigned int ) ulEntryValue;
				Pygmy_fputc( ucpTable[ ( uiEntry * 2 ) ], pygmyOut );
				Pygmy_fputc( ucpTable[ ( uiEntry * 2 ) + 1 ], pygmyOut );
				break;
			case 4:
				ulEntryValue = ( unsigned long ) ucpTable[ ( uiEntry * 4 ) ] << 24;
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 1 ] << 16;
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 2 ] << 8; 
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 3 ];
				ulEntryValue += 4;
				ucpTable[ ( uiEntry * 4 ) ] = ( unsigned char ) ulEntryValue >> 24;
				ucpTable[ ( uiEntry * 4 ) + 1 ] = ( unsigned char ) ulEntryValue >> 16;
				ucpTable[ ( uiEntry * 4 ) + 2 ] = ( unsigned char ) ulEntryValue >> 8;
				ucpTable[ ( uiEntry * 4 ) + 3 ] = ( unsigned char ) ulEntryValue;
				Pygmy_fputc( ucpTable[ ( uiEntry * 4 ) ], pygmyOut );
				Pygmy_fputc( ucpTable[ ( uiEntry * 4 ) + 1 ], pygmyOut );
				Pygmy_fputc( ucpTable[ ( uiEntry * 4 ) + 2 ], pygmyOut );
				Pygmy_fputc( ucpTable[ ( uiEntry * 4 ) + 3 ], pygmyOut );
				break;
		}
	}
	printf( "\n" );
}


/*void PygmyRes_WriteTable( unsigned int uiBytesPerEntry, unsigned int uiTableEntries, unsigned char *ucpTable, FILE *fileOut )
{
	unsigned long ulEntryValue;
	unsigned int uiEntry, i;
	
	// First all entries currently in table must be incremented by Bytes Per Entry to correct for additional entry at end of table
	for( uiEntry = 0; uiEntry < uiTableEntries; uiEntry++ ){ 
		switch( uiBytesPerEntry ){
			case 2:
				ulEntryValue = ( unsigned int ) ucpTable[ uiEntry * 2 ] << 8;
				ulEntryValue |= ( unsigned int ) ucpTable[ ( uiEntry * 2 ) + 1 ];
				printf( "\nTableEntry %d From 0x%X to", uiEntry, ulEntryValue );
				ulEntryValue += 2;
				printf( "0x%X", ulEntryValue );
				ucpTable[ ( uiEntry * 2 ) ] = ( unsigned int ) ulEntryValue >> 8;
				ucpTable[ ( uiEntry * 2 ) + 1 ] = ( unsigned int ) ulEntryValue;
				fputc( ucpTable[ ( uiEntry * 2 ) ], fileOut );
				fputc( ucpTable[ ( uiEntry * 2 ) + 1 ], fileOut );
				break;
			case 4:
				ulEntryValue = ( unsigned long ) ucpTable[ ( uiEntry * 4 ) ] << 24;
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 1 ] << 16;
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 2 ] << 8; 
				ulEntryValue |= ( unsigned long ) ucpTable[ ( uiEntry * 4 ) + 3 ];
				ulEntryValue += 4;
				ucpTable[ ( uiEntry * 4 ) ] = ( unsigned char ) ulEntryValue >> 24;
				ucpTable[ ( uiEntry * 4 ) + 1 ] = ( unsigned char ) ulEntryValue >> 16;
				ucpTable[ ( uiEntry * 4 ) + 2 ] = ( unsigned char ) ulEntryValue >> 8;
				ucpTable[ ( uiEntry * 4 ) + 3 ] = ( unsigned char ) ulEntryValue;
				fputc( ucpTable[ ( uiEntry * 4 ) ], fileOut );
				fputc( ucpTable[ ( uiEntry * 4 ) + 1 ], fileOut );
				fputc( ucpTable[ ( uiEntry * 4 ) + 2 ], fileOut );
				fputc( ucpTable[ ( uiEntry * 4 ) + 3 ], fileOut );
				break;
		}
	}
	printf( "\n" );
}*/

unsigned int PygmyRes_GetBytesPerTableEntry( unsigned int uiPygmyInfo )
{
	switch( uiPygmyInfo & PYGRES_TABLEMASK ){
		case PYGRES_TABLE16:
			return( 2 );
		case PYGRES_TABLE32:
			return( 4 );
	}
} 

unsigned int PygmyRes_ListTable( unsigned int uiPygmyInfo, unsigned char *ucpIn )
{
	unsigned int i, uiEntries;
	unsigned long ulEntry;
	
	uiEntries = ( unsigned int )*( ucpIn++ ) << 8;
	uiEntries |= *( ucpIn++ );
	printf( "Table Entries: %d", uiEntries );
	switch( uiPygmyInfo & PYGRES_TABLEMASK ){
		case PYGRES_TABLE16:
			for( i = 0; i < uiEntries; i++ ){
				ulEntry = ( unsigned int ) *( ucpIn++ ) << 8;
				ulEntry |= ( unsigned int ) *( ucpIn++ );
				printf("\n %4X",  ulEntry );
			}
			break;
		case PYGRES_TABLE32:
			for( i = 0; i < uiEntries; i++ ){
				ulEntry = ( unsigned long ) *( ucpIn++ ) << 24;
				ulEntry |= ( unsigned long ) *( ucpIn++ ) << 16; 
				ulEntry |= ( unsigned long ) *( ucpIn++ ) << 8;
				ulEntry |= ( unsigned long ) *( ucpIn++ );
				printf("\n %4X", ulEntry );
			}
			break;
	}
	printf( "\n" );
	
	return( uiEntries );
}

void PygmyRes_List( unsigned int uiPygmyInfo )
{
	printf("\nResource Type: ");
	switch( uiPygmyInfo & PYGRES_TYPEMASK ){
		case PYGRES_IMAGE:
			printf( "IMAGE" );
			break;
		case PYGRES_IMAGESTRIP:
			printf( "IMAGESTRIP" );
			break;
		case PYGRES_ANIMATION:
			printf( "ANIMATION" ); 
			break;
		case PYGRES_VIDEO:
			printf( "VIDEO" );
			break;
		case PYGRES_AUDIO:
			printf( "AUDIO" );
			break;
		case PYGRES_BINARY:
			printf( "BINARY" );
			break;
		case PYGRES_TEXT:
			printf( "TEXT" );
			break;
		case PYGRES_EXECUTABLE:
			printf( "EXECUTABLE" );
			break;
		case PYGRES_BYTECODE:
			printf( "BYTECODE" );
			break;
		case PYGRES_LIBRARY:
			printf( "LIBRARY" );
			break;
		case PYGRES_GUI:
			printf( "GUI" );
			break;
		case PYGRES_SCRIPT:
			printf( "SCRIPT" );
			break;
		case PYGRES_DRIVER:
			printf( "DRIVER" );
			break;
		case PYGRES_ARCHIVE:
			printf( "ARCHIVE" );
			break;
		case PYGRES_STREAM:
			printf( "STREAM" );
			break;
		case PYGRES_FONT:
			printf( "FONT" );
			break;
	} // end of switch
	printf( "\n" );
	printf( "Table Entry Size: " );
	switch( uiPygmyInfo & PYGRES_TABLEMASK ){
		case PYGRES_TABLE16:
			printf( "16 BIT" );
			break;
		case PYGRES_TABLE32:
			printf( "32 BIT" );
			break;
	}
	printf( "\n" );
	printf( "Dimension Size: ");
	switch( uiPygmyInfo & PYGRES_DIMENSIONSIZEMASK ){
		case PYGRES_8BITD:
			printf( "8 BIT" );
			break;
		case PYGRES_16BITD:
			printf( "16 BIT" );
			break;
	}
	printf( "\n" );
	printf( "Draw Style: ");
	switch( uiPygmyInfo & PYGRES_DRAWSTYLEMASK ){
		case PYGRES_RAW:
			printf( "RAW" );
			break;
		case PYGRES_ALPHA:
			printf( "ALPHA" );
			break;
		case PYGRES_SCATTER:
			printf( "SCATTER" );
			break;
		case PYGRES_VECTOR:
			printf( "VECTOR" );
			break;
	}
	printf( "\n" );
	
	if( uiPygmyInfo & PYGRES_CRCMASK )
		printf( "CRC: ENABLED" );
	else
		printf( "CRC: DISABLED" );
	printf( "\n" );
	if( uiPygmyInfo & PYGRES_VERSIONMASK )
		printf( "Version: Extended" );
	else
		printf( "Version: V1" );
	printf( "\n" );
	printf( "Bits Per Pixel: " );
	switch( uiPygmyInfo & PYGRES_BPPMASK ){
		case PYGRES_1BPP:
			printf( "1BPP" );
			break;
		case PYGRES_4BPP:
			printf( "4BPP" );
			break;
		case PYGRES_8BPP:
			printf( "8BPP" );
			break;
		case PYGRES_16BPP:
			printf( "16BPP" );
			break;
		case PYGRES_18BPP:
			printf( "18BPP" );
			break;
		case PYGRES_24BPP:
			printf( "24BPP" );
			break;
		case PYGRES_32BPP:
			printf( "32BPP" );
			break;
	}
	printf( "\n" );
}

