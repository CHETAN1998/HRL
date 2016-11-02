#include <stdio.h>
#include <errno.h>
#include <string.h>
void compress(FILE *fp_in, FILE *fp_out) {
	int count, ch, ch2;
	ch = getc(fp_in);
	ch2 = ch;
	while (ch2 != EOF) {
        	for (count = 0; ch2 == ch && count < 255; count++) {
                	ch2 = getc(fp_in);
            	}
       		putc(count, fp_out);
        	putc(ch, fp_out);
        	ch = ch2;
        }
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "File Compressed\n");
}
void decompress(FILE *fp_in, FILE *fp_out) {
	int count, ch, ch2;
	for (count = 0; ch2 != EOF; count = 0) {
		ch = getc(fp_in);
		if ( (ch2 = getc(fp_in) ) == EOF) 
                	break;
		do {
                	putc(ch2, fp_out);
                	count++;
            	} while (count < ch );
        }
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "File Decompressed\n");
}
