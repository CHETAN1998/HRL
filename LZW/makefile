project : lzw.o dic.o lzwmain.o
	cc lzw.o lzwmain.o dic.o -o project
dic.o : dic.h dic.c
	cc -c dic.c -Wall
lzw.o : dic.h lzw.c dic.c
	cc -c lzw.c -Wall
lzwmain.o : dic.h lzwmain.c lzw.c
	cc -c lzwmain.c -Wall
