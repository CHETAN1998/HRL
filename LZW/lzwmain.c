#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "dic.h"
void printuse() {
	printf("Mode :\n'-h': help\n");
	printf("'-c' : For compression , Give input and output file name\n");
	printf("'-x' : For decompression , Give input and output file name\n");
	printf("Usage : ./project [-h | -c | -x ] <input file> <output file>\n");
}
int main(int argc, char** argv) {
	if(!strcmp(argv[1] , "-h") ) {
		printuse();
		return 0;
	}
  	char* infile;
  	char* outfile;
  	if(argc < 4 ) {
		errno = EINVAL;
		perror("Bad Arguments:     ");
		printuse();
		return errno;
  	}
  	infile = argv[2];
  	outfile = argv[3];
  	IN = fopen(infile, "rb");
  	if(IN == NULL) {
    		printf("Couldn't open %s for reading\n", infile );
		return errno;
  	}
  	OUT = fopen(outfile, "wb");
  	if(OUT == NULL) {
    		printf("Couldn't open %s for writing\n", outfile );
		return errno;
  	}
	if(!strcmp(argv[1] , "-c") ) {
  		lzw();
		synFile();
	}
	else if(!strcmp(argv[1] , "-x") )
  		dlzw();
	else
		return 0;
  	fclose(IN);
  	fclose(OUT);
 	return 0;
}
