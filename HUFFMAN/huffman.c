#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
////////////////////////////////////////////////////////////////////////////
void encodeFile(FILE* in, FILE* out) {
	int k, i, l, p;
	NODE *head, *node, *nte;
	unsigned char symbol, byte;
	unsigned long freq;
	unsigned short numSymbols;
	int val, index = 0, mul = 1, tempArr[8];
	int c = fgetc(in);
	charCount = 0;
	while(c != EOF) {
		++charCount;
		if(exists(c) == 0) {
			if(c >= 0 && c < MAX)
				sympos[c] = 1;
			}
		c = fgetc(in);
	}
	head = createTree();   
	numSymbols = numl(head);
	fwrite(&numSymbols, 2, 1, out);
	for(k = 0; k < MAX; k++) {
		if(sympos[k] != 0) {
			nte = findNode(head,k);
			symbol = (unsigned char)nte->value;
			fwrite(&symbol, 1, 1, out);
			freq = nte->count;
			fwrite(&freq, 8, 1, out);
		}
	}
	fwrite(&charCount, 8, 1, out);  
	byte = 0;
	rewind(in);
	val = fgetc(in);
	while(val != EOF) {
		node = findNode(head, val);
		for(l = 0; l < node->size; l++) {
			tempArr[index] = node->codes[l];
			++index;
			if(index % 8 == 0) {
				index = 0;    
				for(i = 0; i < 8; i++) {
					byte += tempArr[i] * mul;
					mul *= 2;
				}
				mul = 1;
				fwrite(&byte, 1, 1, out);
				byte = 0;
			}
		}
		val = fgetc(in);
		if(val == EOF && (index % 8 ) != 0) {
			for(p = 0; p < index; p++) {
				byte += tempArr[p] * mul;
				mul *= 2;
			}
			fwrite(&byte, 1, 1, out);
		}
	}
  	freeTree(head);
	printf("File Compressed\n");
}
/////////////////////////////////////////////////
void decodeFile(FILE* in, FILE* out) {
	unsigned short numSymbols;
	unsigned char symbol, currentByte;
	unsigned long sf, totalNumChar;
	int sc = 0;
	NODE *head = NULL, *temp1 = NULL, *temp2 = NULL;
	NODE *curr;
	unsigned long newCount = 0;
	int shift = 0, bitCount = 0, bit = 0, counter = 0, stop = 0;
	fread(&numSymbols, 2, 1, in);
	while(sc < numSymbols) {
		fread(&symbol, 1, 1, in);
		fread(&sf, 8, 1, in);
		head = fill(head, NULL, NULL, symbol, sf);
		++sc;
	}
	while(qdone(head) == 1) {
		temp1 = pop(&head);
		temp2 = pop(&head);
		newCount = temp1->count + temp2->count;
		head = insert(head, temp1, temp2, -1, newCount);
	}
	fread(&totalNumChar, 8, 1, in);
	fread(&currentByte, 1, 1, in);
	curr = head; 
	while(counter < totalNumChar) {
		while(bitCount < 8 && stop == 0) {
			bit = ((currentByte>>shift) % 2);
			if(bit == 0)
				curr = curr->left;
			else
				curr = curr->right;
			if(curr->left == NULL && curr->right == NULL) {
				++counter; 
				if(counter >= totalNumChar)
					stop = 1;
				fwrite(&(curr->value), 1, 1, out);
				curr = head; 
			}
			++shift;
			++bitCount;
		}
		shift = 0; 
		bitCount = 0; 
		fread(&currentByte, 1, 1, in);
	}
	freeTree(head);
	printf("File Decompressed\n");
}
///////////////////////////////////////////////////
int exists(int c) {
	if(sympos[c] != 0) {
		++sympos[c];
		return 1;
	}
	return 0;
}
//////////////////////////////////////////////////
int qdone(NODE* head) {
	NODE *currNode = head;
	int numNodes = 0;
	while(currNode!= NULL) {
		++numNodes;
		if(numNodes > 1)
			return 1;
		currNode = currNode->next;
	}
	return 0; 
}
///////////////////////////////////////////////////
void buildCodes(NODE *node, int code, int size) {
	int i;
	arr[size-1] = code;
	if(node == NULL)
		return;
	if((node->left == NULL) && (node->right == NULL)) {
		for(i = 0; i < size; i++) {
			node->codes[i] = arr[i];
			node->size = size;
		}
    		return;
	}
	buildCodes(node->left, 0, size + 1);
	buildCodes(node->right, 1, size + 1);
}
///////////////////////////////////////////////////
void initcode(NODE *node) {
	int i;
	for (i = 0; i < 255; i++)
		node->codes[i] = 2;
}
///////////////////////////////////////////////////
NODE* pop(NODE **headRef) { 
	NODE *nextNode, *temp;
	nextNode = (*headRef)->next;
	temp = *headRef;
	temp->next = NULL;
	*headRef = nextNode;
	return temp;
}
///////////////////////////////////////////////////
NODE* createTree() {
	int i;
	unsigned long newCount = 0;
	NODE* head = NULL, *temp1 = NULL, *temp2 = NULL;
	for (i = 0; i < MAX; i++) {
		if(sympos[i] != 0)
			head = fill(head, NULL, NULL, i, sympos[i]);
		}
	while(qdone(head) == 1) {
		temp1 = pop(&head);
		temp2 = pop(&head);
		newCount = temp1->count + temp2->count;
		head = insert(head, temp1, temp2, -1, newCount);
	} 
	buildCodes(head, 2, 0);
	return head;
}
////////////////////////////////////////////////////
NODE* insert(NODE* head, NODE* left, NODE* right, int value, unsigned long count) {
	NODE*  newNode = createNode(value,count,left,right);
	int maxRightLeaf = furright(newNode);
	NODE* lastNode,  *curr;
	int terminate;
	if(head == NULL)
		return newNode;
	else if(count <= head->count) {
		if(furright(head) < maxRightLeaf && count == head->count) { 
			curr = head;
			lastNode = NULL;
			while(curr != NULL && furright(curr) < maxRightLeaf) {
				lastNode = curr;
				curr = curr->next;
			}
			newNode->next = curr;
			lastNode->next = newNode;
      			return head;
    		}
    		else {
      			newNode->next = head;
      			return newNode;
		}     
  	}
  	else {
    		if(newNode->right != NULL && newNode->left != NULL) {
      			terminate = 0;
      			curr = head;
      			lastNode = NULL;
      			while(curr != NULL && curr->count <= count && terminate == 0) {
        			if(curr->count == count) {
          				while(curr != NULL && furright(curr) <= maxRightLeaf && curr->count == count) {
            				lastNode = curr;
            				curr = curr->next;
          				}
          				terminate = 1; 
        			}
        		if(terminate == 0) {
          			lastNode = curr;
          			curr = curr->next;
        		}
      		}
      		newNode->next = curr;
      		lastNode->next = newNode;
    	}
   		else {
      			curr = head;
      			lastNode = NULL;
      			while(curr != NULL && curr->count <= count) {
        			lastNode = curr;
        			curr = curr->next;
      			}
      			newNode->next = curr;
      			lastNode->next = newNode;
    		}  
	}
  	return head;
}
////////////////////////////////////////////////////////////////
NODE* fill(NODE* head, NODE* left, NODE* right, int value, unsigned long count) {
  	NODE*  newNode = createNode(value,count,left,right);
  	NODE* curr, *lastNode;
  	if(head == NULL)
    		return newNode;
  	else if(count < head->count) {
    		newNode->next = head;
    		return newNode;
  	}
  	else {
    		curr = head;
   	 	lastNode = NULL;
    		while(curr != NULL && curr->count <= count) {
      			lastNode = curr;
      			curr = curr->next;
    		}
    		newNode->next = curr;
    		lastNode->next = newNode;
  	}
  	return head;
}
////////////////////////////////////////////////////////////////
int furright(NODE* node) {
  	if(node == NULL)
  		return 0;
  	while(node->right != NULL)
    		node = node->right;
  	return node->value;
}
////////////////////////////////////////////////////////////////
NODE* createNode(int value,unsigned long count, NODE* left, NODE* right) {
  	NODE* node = malloc(sizeof(NODE));
  	node->value = value; 
  	node->count = count;
  	node->left = left;
  	node->right = right;
  	node->next = NULL;
  	initcode(node);
  	return node;
}
////////////////////////////////////////////////////////
NODE* findNode(NODE* root,int value) {
  	NODE* node;
  	if(root != NULL) {
    		if(root->value == value)
      			return root;
    		else {
      			node = findNode(root->left, value);
      			if(node == NULL)
        			node = findNode(root->right,value);
      			return node;
    		}
  	}
  	else
    		return NULL;
}
//////////////////////////////////////////////////////////
unsigned short numl(NODE* head) {
  	if(head == NULL)
		return 0;
  	if(head->left == NULL && head->right == NULL)
    		return 1;
  	return(numl(head->left) + numl(head->right));
}
/////////////////////////////////////////////////////////
void freeTree(NODE* root) {
  	if(root == NULL)
    		return;
  	if(root->left != NULL)
    		freeTree(root->left);
  	if(root->right != NULL)
    		freeTree(root->right);
  	free(root);
}
////////////////////////////////////////////////////////
