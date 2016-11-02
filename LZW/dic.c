#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic.h"
Trie t = NULL;
char* Dic[MAX_DIC_SIZE];
int Dic_Size = 0;
int flag = 1;
int COUNT = 256;
///////////////////////////////////////////////////
int trieInit() {
	t = ( Trie )malloc ( sizeof( TrieRoot ) );
	if( t == NULL ) {
		fprintf( stderr , " error: cannot initiate a trie \n" );
		return 0;
	}
	else {
		int i;
		for( i = 0 ; i < 256 ; ++i )
			t->child[ i ] = NULL;
		t->size = 0;
		return 1;
	}
}
//////////////////////////////////////////////////
int trieDestruct( Trie root ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	int i;
	for( i = 0 ; i < 256 ; ++i ) {
		if( root->child[ i ] != NULL ) {
			if( nodeDestruct( root->child[ i ] ) != 1 ) {			
				fprintf( stderr , "error: cannot destruct node \n " );
				return 0;
			}
		}
	}
	return 1;
}
//////////////////////////////////////////////////
int nodeDestruct( TrieNode *node ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	int i;
	for( i = 0 ; i < 256 ; ++i ) {	
		if( node->child[ i ] != NULL ) {
			if( nodeDestruct( node->child[ i ] ) != 1 ) {
				fprintf( stderr , "error: cannot destruct node \n " );
				return 0;	
			}
			else {
				free(node);
				return 1;
			}
		}
	}
	return 1;
}
//////////////////////////////////////////////////
TrieNode *trieSearch(char *name ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( t == NULL )
		trieInit();
	Trie root = t;
	TrieNode * node;
	unsigned char target;
	int size = strlen( name );
	if( size == 0 || size > MAX_STRLEN )
		return NULL;
	int i;
	for( i = 0 ; i < size ; ++i ) {
		target = (unsigned char) ( name[ i ] );
		if( (int)target >= 256 || (int)target < 0 ) {
			printf( "bad target: %c\n" , target );
			printf( "name: %s i: %d\n" , name , i );
			exit( 1 );
		}
		if( i == 0 )
			node = root->child[(int)target];
		else
			node = node->child[(int)target];
		if( node == NULL )
			return NULL;
		
	}
	return node;
}
//////////////////////////////////////////////////////
TrieNode* trieInsert(char *name ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( t == NULL )
		trieInit();
	Trie root = t;	
	TrieNode * node = NULL;
	unsigned char target;
	int size = strlen( name );
	if( size == 0 || size > MAX_STRLEN ) {
		fprintf( stderr, "error: wrong length item\n" );
		return NULL;
	}
	int i;
	for( i = 0 ; i < size ; ++i ) {
		target = name[ i ];
		if( i == 0 ) {
			node = root->child[ (int)target ];
			if( node == NULL ) {
				root->child[ (int)target ] = ( TrieNode* )malloc( sizeof( TrieNode ) );
				node = root->child[ (int)target ];
				if( node == NULL ) {
					fprintf( stderr , "error: cannot create a trie node\n" );
					return NULL;
				}
				int i;
				for( i = 0 ; i < 256 ; ++i )
					node->child[ i ] = NULL;
				node->key = -1;
				node->character = target;
			}
		}
		else {
			if( node->child[ (int)target ] == NULL ) {
				node->child[ (int)target ] = ( TrieNode* )malloc( sizeof( TrieNode ) );
				node = node->child[ (int)target ];
				if( node == NULL ) {
					fprintf( stderr , "error: cannot create a trie node\n" );	
					return NULL;
				}
				int i;
				for( i = 0 ; i < 256 ; ++i )
					node->child[ i ] = NULL;		
				node->key = -1;
				node->character = target;
			}
			else
				node = node->child[ (int)target ];
		}
		
	}
	if( node->key == -1 ) {
		node->key = COUNT;
		root->size++;
		COUNT++;
	}
	return node;
}
//////////////////////////////////////////////////////
int trieToFile(FILE *file ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( t == NULL )
		trieInit();
	Trie root = t;
	int i;
	char name[MAX_STRLEN + 1];
	for( i = 0 ; i < 256 ; ++i ) {
		if( root->child[ i ] != NULL ) {
			name[ 0 ] = (char)i;
			name[ 1 ] = '\0';
			if ( nodeToFile( root->child[ i ] , name , file ) == 0) {
				fprintf( stderr , "error: cannot print a node\n" );
				return 0;
			}
		}
	}
	return 1;
}
///////////////////////////////////////////////////////////
int nodeToFile( TrieNode *node , char *name , FILE *file ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( node == NULL )
		return 0;
	else
	{
		if( file == NULL )
			file = stdout;
		if( node->key >= 0 )
			fprintf( file , "string:%s\tkey:%d\n" , name , node->key );
		int i;
		for( i = 0 ; i < 256 ; ++i ) {
			if( node->child[ i ] != NULL ) {
				char tail[2];
				tail[ 0 ] = (unsigned char)i;
				tail[ 1 ] = '\0';
				strcat( name , tail );
				if ( nodeToFile( node->child[ i ] , name , file ) == 0) {
					fprintf( stderr , "error: cannot print a node\n" );
					return 0;
				}
				int l = strlen( name );
				name[ l - 1 ] = '\0';
			}
		}
	}
	return 1;
}
//////////////////////////////////////////////////
int dicSearchString( char *name ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	TrieNode *node = trieSearch( name );
	if( node == NULL )
		return -1;
	else
		return node->key;
	
}
/////////////////////////////////////////////////////
int dicInsert( char *name ) {	
	if( flag ) {
		flag =0;
		memset((void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( Dic_Size < MAX_DIC_SIZE ) {
		TrieNode *node = trieInsert( name );
		if( node == NULL )
			return -1;
		else {
			Dic[ Dic_Size ] = ( char* )malloc( strlen( name ) + 1 );
			if ( Dic[ Dic_Size ] == NULL ) {
				fprintf( stderr , "error: cannot insert to dictionary\n" );
				return -1;
			}
			strcpy( Dic[ Dic_Size ] , name );
			Dic_Size++;
			return node->key;
		}
	}
	else
		return -1;
}
//////////////////////////////////////////////////////
int dicToFile( FILE* file ) {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	return trieToFile( file );
}
/////////////////////////////////////////////////////
char* dicSearchCode( const unsigned short i ){	
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	if( i-256 >= 0 && i-256 < MAX_DIC_SIZE )
		return Dic[ i - 256 ];
	else
		return NULL;
}
////////////////////////////////////////////////////
int dicSize() {
	if( flag ) {
		flag =0;
		memset( (void*)Dic , 0 , MAX_DIC_SIZE );
		trieInit();
	}
	return t->size;
}
//////////////////////////////////////////////////
