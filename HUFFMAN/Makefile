project: huffman.o huffmain.o
	cc huffman.o huffmain.o -o project
huffman.o : huffman.h huffman.c
		cc -c huffman.c -Wall
huffmain.o : huffman.c huffman.h huffmain.c
		cc -c huffmain.c -Wall
