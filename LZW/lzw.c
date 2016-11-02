#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "dic.h"
#define BUF_SIZE (4096)
unsigned short BUF_IN[BUF_SIZE];
int POS_IN = BUF_SIZE;
unsigned short EMPTY_FLAG = 0;
unsigned short BUF_OUT[BUF_SIZE];
int POS_OUT = 0;
void writeInt( unsigned short i ) {
	if( POS_OUT >= BUF_SIZE ) {
		fwrite( BUF_OUT , sizeof( unsigned short int ) , BUF_SIZE , OUT );
		POS_OUT = 0;
	}
	BUF_OUT[POS_OUT++] = i;

}
/////////////////////////////////////////////////
unsigned short readInt() {
	static int SIZE_NOW = 0;
	if( POS_IN >= SIZE_NOW ) {
		SIZE_NOW = fread( BUF_IN , sizeof( unsigned short int ) , BUF_SIZE , IN );
		POS_IN = 0;
		if( SIZE_NOW == 0 )
			EMPTY_FLAG = 1;
	}
	return BUF_IN[ POS_IN++ ];
}
/////////////////////////////////////////////////
void synFile() {
	fwrite( BUF_OUT , sizeof( unsigned short int ) , POS_OUT, OUT );
}
/////////////////////////////////////////////////
int lzw() {
	char str[MAX_STRLEN + 1];
	unsigned char ch;
	int ich = 0;
	str[0] = '\0';
	while( ( ich = getc( IN ) ) != EOF ) {
		ch = ( unsigned char ) ich;
		int length = strlen(str);
		int insert_code;
		int out_code;
		if( length == 0 ) {
			str[0] = ch;
			str[1] = '\0';
			continue;
		}
		if( length == 1 )
			out_code = (int)( str[0] );
		else
			out_code = dicSearchString( str );

		if( length < MAX_STRLEN && length > 0 ) {
			str[ length ] = ch; 
			str[ length + 1 ] = '\0';
			if( dicSearchString( str ) == -1 && ( length != 0 ) ) {	
				if( dicSize() < MAX_DIC_SIZE) {
					if ( ( insert_code = dicInsert( str ) ) != -1 ) {
						writeInt( out_code );
						str[0] = ch;
						str[1] = '\0';
					}
					else {
						fprintf( stderr , "error: cannot insert to dictionary\n" );
						return 0;
					}
				}
			}
		}
		else {
			writeInt( out_code );
			str[ 0 ] = ch; 
			str[ 1 ] = '\0';
		}
	}
	printf("File Compressed\n");
	return 1;
}
///////////////////////////////////////////////////////////////	
int dlzw() {
	unsigned short k;
	char entry[MAX_STRLEN + 1];
	char s[MAX_STRLEN + 1];
	entry [ 0 ] = '\0';
	s [ 0 ] = '\0';
	char *search_rel = NULL;
	while( 1 ) {
		k = readInt();
		if( EMPTY_FLAG )
			break;
		if( k < 256 ) {
			entry[0] = (unsigned char)k;
			entry[1] = '\0'; 
		}
		else {
			search_rel = dicSearchCode( k );
			if( search_rel != NULL )
				strcpy( entry , search_rel );
			else {
				strcpy( entry , s );
				int entry_l = strlen( entry );
				if( entry_l < MAX_STRLEN ) {
					entry[entry_l] = s[0];
					entry[entry_l + 1] = '\0';
				}
			}
		}
		fputs( entry , OUT );
		int s_l = strlen( s );
		if( s_l < MAX_STRLEN && s_l > 0 && dicSize() < MAX_DIC_SIZE ) {
			s[s_l] = entry[0];
			s[s_l + 1] = '\0';
			int insert_rel = dicInsert( s );
			if ( insert_rel == -1 ) {
				fprintf( stderr , "error: cannot insert to dictionary\n" );
				return 0;
			}
		}	
		strcpy( s , entry );
	}
	fputs( "\n" , OUT );
	printf("File Decompressed\n");
	return 1;
}
//////////////////////////////////////////////////////////////////////
