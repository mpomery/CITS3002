/* 
 * Tests several checksum algorithms.
 *
 * Written by Mitchell Pomery <mjpomery@ucc.asn.au>
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
#include "timing.c"

int main(int argc, char **argv)
{
	// Usage
	if(argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		fprintf(stderr, "Generates a random string of random length, ");
		fprintf(stderr, "corrupts it, then tests is against some checksums.\n\n");
		exit(1);
	}
	
	// Makes our randomness better
	srand (time(NULL));
	// Generate a random string for testing
	int inputLength = rand() % 99990 + 10;
	char input[inputLength + 1];
	input[inputLength] = '\0';
	for (int i = 0; i < inputLength; i++) {
		input[i] = (char) (rand() % 94) + 32;
	}
	
	// Checksum our input
	short ccittInput = checksum_ccitt(input, inputLength);
	short crc16Input = checksum_crc16(input, inputLength);
	int internetInput = checksum_internet((short unsigned int*) input, inputLength);
	// Corrupt the input
	corrupt_frame(input, 12);
	//Checksum our output
	short ccittOutput = checksum_ccitt(input, inputLength);
	short crc16Output = checksum_crc16(input, inputLength);
	int internetOutput = checksum_internet((short unsigned int*) input, inputLength);
	
	
	
	// Print some info
	printf("input:             %s\n", input);
	printf("corrupted output:  %s\n", input);
	printf("\n");
	printf("ccitt:             %u\n", ccittInput);
	printf("crc16:             %u\n", crc16Input);
	printf("internet:          %d\n", internetInput);
	printf("\n");
	printf("ccitt:             %u\n", ccittOutput);
	printf("crc16:             %u\n", crc16Output);
	printf("internet:          %d\n", internetOutput);
	
	// Compare Hashes
	printf("\n");
	printf("compared hashes\n");
	printf("ccitt:             %s\n", (ccittInput == ccittOutput ? "true" : "false"));
	printf("crc16:             %s\n", (crc16Input == crc16Output ? "true" : "false"));
	printf("internet:          %s\n", (internetInput == internetOutput ? "true" : "false"));
	
	// Run each of our checksum functions numcheck times for timing
	// Then print the time taken
	int numcheck = 10000;
	
	printf("\n");
	printf("hashing %d times\n", numcheck);
	
	timing(true);
	for (int i = 0; i < numcheck; i++) {
		checksum_ccitt(input, inputLength);
	}
	printf("ccitt:             %lldusecs\n", timing(false));
	
	timing(true);
	for (int i = 0; i < numcheck; i++) {
		checksum_crc16(input, inputLength);
	}
	printf("crc16:             %lldusecs\n", timing(false));
	
	timing(true);
	for (int i = 0; i < numcheck; i++) {
		checksum_internet((short unsigned int*) input, inputLength);
	}
	printf("internet:          %lldusecs\n", timing(false));
	
	exit(0);
}