/**************************************************************************
    pygtoh ( Pygmy Operating System Utilities: Converter for: File to C Include File )
    Copyright (C) 2011  Warren D Greenway

    This file is part of pygtoh.

    pygtoh is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    pygtoh is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with pygtoh.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#define MAXFILESIZE	0xFFFF

typedef struct  {
			u16 uiType; 
			u16 uiTableSize; 
			u16 uiDimensionType; 
			u16 uiDimensionSize; 
			u16 uiDrawStyle; 
			u16 uiVersion; 
			u16 uiBPP; 
		}PygResHeader;
		

int main ( int argc, char *argv[] )
{
	u32 i, ulIndex, ulInLen;
	u16 uiPygmyInfo, uiTableEntries, uiTableSize;
	u8 ucByte;
	FILE *fileIn, *fileOut;
	u8 ucpIn[ MAXFILESIZE ];

	
	if( argv[1] ){
		fileIn = fopen( argv[1], "rb" );
		if( !fileIn ){
			printf( "\nFailed to open file %s!", argv[1] );
			exit( 1 );
		} // if
	} else{
		printf( "Please specify Input, usage: Input Output.\n" );
		exit( 1 );
	} // else
	if( argv[2] ){
		fileOut = fopen( argv[2], "w" );
		if( !fileOut ){
			printf( "\nFailed to open file %s!", argv[2] );
			exit( 1 );
		} // if
	} else{
		printf( "Please specify Output, usage: Input Output.\n" );
		exit( 1 );
	} // else
	for( ulInLen = 0; !feof( fileIn ); ulInLen++ ){
		ucByte = fgetc( fileIn );
		if( !feof( fileIn ) ){
			ucpIn[ ulInLen ] = ucByte;
        } // if
	} 
	fprintf( fileOut, "// Pygmy Resource File Header\n" );
	fprintf( fileOut, "const unsigned char PYGMY_" );
		
	for( i = 0; *(argv[2]+i) && *(argv[2]+i) != '.'; i++ ){
		fprintf( fileOut, "%c", (char)*(argv[2]+i) );
	} // for
	fprintf( fileOut, "[] = {\n" );
	
    fprintf( fileOut, "0x%02X,0x%02X,0x%02X,0x%02X,// Length\n", (u8)(ulInLen<<24), (u8)(ulInLen<<16), (u8)(ulInLen<<8), (u8)ulInLen );     
	//for( i = 0, ulInLen = 0; !feof( fileIn ); i++, ulInLen++){
    for( i = 0; i < ulInLen; i++ ){    
		if( !( i % 12 ) ){
			fprintf( fileOut, "\n" );
		} // if	
        fprintf( fileOut, "0x%02X,", ucpIn[ i ] );
	} // for
	fprintf( fileOut, "\n};\n" );
	printf( "Input %s loaded, %d bytes\n", argv[1], ulInLen );

	if( fileIn ){
		fclose( fileIn );
    } // if
	if( fileOut ){
		fclose( fileOut );
    } // if
}
