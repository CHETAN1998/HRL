#ifndef _dic_H_
#define _dic_H_
#define MAX_STRLEN (50)
#define MAX_DIC_SIZE (65000)
#include <stdio.h>
typedef struct NodeType {
	struct NodeType *child[ 256 ];
	int key;
	unsigned char character;
} TrieNode;
typedef struct RootType {
	struct NodeType *child[ 256 ];	
	int size;
} TrieRoot;
FILE* IN, *OUT;
typedef TrieRoot* Trie;
char* dicSearchCode( const unsigned short );
int dicSearchString( char* );
int dicInsert( char* );
int dicToFile( FILE* );
int nodeDestruct( TrieNode *node );
int nodeToFile( TrieNode *node , char *name , FILE *file );
int lzw();
int dlzw();
void synFile();
int dicSize();
#endif
