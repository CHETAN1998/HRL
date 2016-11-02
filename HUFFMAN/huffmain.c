#include <stdio.h>
#include "huffman.h"
#include <errno.h>
#include <string.h>
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
 	FILE* in;
  	FILE* out;
  	if(argc < 4 ) {
		errno = EINVAL;
		perror("Bad Arguments:     ");
		printuse();
		return errno;
  	}
  	infile = argv[2];
  	outfile = argv[3];
  	in = fopen(infile, "rb");
  	if(in == NULL) {
    		printf("Couldn't open %s for reading\n", infile );
		return errno;
  	}
  	out = fopen(outfile, "wb");
  	if(out == NULL) {
    		printf("Couldn't open %s for writing\n", outfile );
		return errno;
  	}
	if(!strcmp(argv[1] , "-c") )
  		encodeFile(in, out);
	else if(!strcmp(argv[1] , "-x") )
  		decodeFile(in, out);
	else
		return 0;
  	fclose(in);
  	fclose(out);
 	return 0;
}
