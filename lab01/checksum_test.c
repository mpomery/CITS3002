/* 
 *
 * Written by mitchell Pomery <mjpomery@ucc.asn.au>
 *
 * Rewritten by Chris McDonald <chris@csse.uwa.edu.au>
 *
 * Compile me with:
 *		gcc -Wall -std=c99 -pedantic -o creditcard creditcard.c
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "corrupt_frame.c"
#include "checksum_ccitt.c"
#include "checksum_crc16.c"
#include "checksum_internet.c"

int main(int argc, char **argv)
{
	// Usage
	if(argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		fprintf(stderr, "Generates a random string of random length, ");
		fprintf(stderr, "corrupts it, then tests is against some checksums.\n\n");
		exit(1);
	}
	
	// Generate a random string for testing
	srand (time(NULL));
	
	
	int inputLength = rand() % 90 + 10;
	char input[inputLength + 1];
	input[inputLength] = '\0';
	for (int i = 0; i < inputLength; i++) {
		input[i] = (char) (rand() % 94) + 32;
	}
	
	printf("input:             %s\n", input);
	printf("ccitt:             %u\n", checksum_ccitt(input, inputLength));
	printf("crc16:             %u\n", checksum_crc16(input, inputLength));
	//unsigned short *ptr = *((unsigned short*)input);
	printf("internet:          %d\n", checksum_internet((short unsigned int*) input, inputLength));
	
	//int corruptionAmount = rand() % 8 + 2;
	//printf("corruption amount: %d\n", corruptionAmount);
	
	//for (int i = 0; i < corruptionAmount; i++) {
		corrupt_frame(input, 12);
	//}
	
	printf("corrupted output:  %s\n", input);
	printf("ccitt:             %u\n", checksum_ccitt(input, inputLength));
	printf("crc16:             %u\n", checksum_crc16(input, inputLength));
	printf("internet:          %d\n", checksum_internet((short unsigned int*) input, inputLength));
	
	exit(0);
}