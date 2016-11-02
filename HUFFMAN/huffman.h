#ifndef HUFFMAN_H
#define HUFFMAN_H
#define MAX 256
typedef struct letter {
	unsigned long count;
	int value;
	struct letter *next;
	struct letter *left, *right;
	int codes[MAX], size;
}NODE;
unsigned long charCount;
int arr[MAX], sympos[MAX];
void encodeFile(FILE* in, FILE* out);
void decodeFile(FILE* in, FILE* out);
NODE* createNode(int value, unsigned long count, NODE* left, NODE* right);
NODE* pop(NODE** headRef);
NODE* createTree();
int exists(int c);
NODE* insert(NODE* head,NODE* left, NODE* right, int value, unsigned long count);
int qdone(NODE* head);
int furright(NODE* head);
void initcode(NODE* node);
void buildCodes(NODE* node, int code, int size);
NODE* fill(NODE* head,NODE* left, NODE* right,
 int value, unsigned long count);
NODE* findNode(NODE* root,int value);
unsigned short numl(NODE* head);
void freeTree(NODE* root);
#endif
