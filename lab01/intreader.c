/*
 * Compile me with:
 *		gcc -Wall -std=c99 -pedantic -o intreader intreader.c
 */


// Info on SPARC
// http://docs.oracle.com/cd/E19205-01/819-5265/bjbdt/index.html


#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *file;
	file = fopen("Q4.output", "rb");
	
	printf("size of int:  %d\n", sizeof(int));
	printf("size of char: %d\n", sizeof(char));
	
	for (int i = -50; i < 50; i++) {
		int number = 0;
		
		char c0, c1, c2, c3;
		
		fread(&c0, sizeof(char), 1, file); // byte0 of sparc
		fread(&c1, sizeof(char), 1, file);
		fread(&c2, sizeof(char), 1, file);
		fread(&c3, sizeof(char), 1, file); // byte3 of sparc
		number = ((((((number | c0) << 8) | c1) << 8) | c2) << 8) | c3;
		printf("%i\n", number);
	}
	
	fclose(file);
	return(0);
}
