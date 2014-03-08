/* Derived from independent code by
 *	Chris Stone <cstone@hms.com>, High Mountain Software Internet Gateway
 * and  KnighA@tetraworld.com (Andy Knight)
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

static int IsCardNumberValid(const char *CardNum)
{
    unsigned sum = 0 ;
    int dbl = !(strlen(CardNum) % 2) ;
    int c ;

    while ((c = *CardNum++) != 0) {
	if(isspace(c)) {
	    dbl	= !dbl;
	    continue;
	}
	if(!isdigit(c))
	    return(0);

        if (dbl) {
            unsigned n = (c - '0') * 2 ;

            if (n > 9)
                n -= 9 ;

            sum += n ;
            dbl = 0 ;
        }
        else {
            sum += (c - '0') ;
            dbl = 1 ;
        }
    }
    return (sum % 10) == 0 ;
}

static char *DescribeCard(const char *CardNum)
{
    static struct _t {
	char	*prefix;
	int	length;
	char	*name;
	int	uses_checksum;
    } table[] = {
	{ "1800", 15, "Japanese Credit Bureau",		1 },
	{ "2014", 15, "enRoute",			0 },
	{ "2131", 15, "Japanese Credit Bureau",		1 },
	{ "2149", 15, "enRoute",			0 },
	{ "300",  14, "Diners Club/Carte Blanche",	1 },
	{ "301",  14, "Diners Club/Carte Blanche",	1 },
	{ "302",  14, "Diners Club/Carte Blanche",	1 },
	{ "303",  14, "Diners Club/Carte Blanche",	1 },
	{ "304",  14, "Diners Club/Carte Blanche",	1 },
	{ "305",  14, "Diners Club/Carte Blanche",	1 },
	{ "34",   15, "American Express",		1 },
	{ "36",   14, "Diners Club/Carte Blanche",	1 },
	{ "37",   15, "American Express",		1 },
	{ "38",   14, "Diners Club/Carte Blanche",	1 },
	{ "3",    16, "Japanese Credit Bureau",		1 },
	{ "4",    13, "Visa",				1 },
	{ "4",    16, "Visa",				1 },
	{ "51",   16, "Mastercard",			1 },
	{ "52",   16, "Mastercard",			1 },
	{ "53",   16, "Mastercard",			1 },
	{ "54",   16, "Mastercard",			1 },
	{ "55",   16, "Mastercard",			1 },
	{ "56",   16, "Australian Bankcard",		1 },
	{ "6011", 16, "Discover",			1 }
    };
#define	NCARDS	(sizeof(table) / sizeof(table[0]))

    int	n;

    for(n=0 ; n<NCARDS ; ++n)
	if(strncmp(table[n].prefix, CardNum, strlen(table[n].prefix)) == 0 &&
	   strlen(CardNum) == table[n].length)
		break;

    if(n == NCARDS)
	return("unknown");

    if(strlen(CardNum) != table[n].length)
	return("invalid length");

    if(table[n].uses_checksum && !IsCardNumberValid(CardNum))
	return("invalid");

    return(table[n].name);
}

/* ----------------------------------------------------------------------- */


int main(int argc, char **argv)
{
    if(argc == 1) {
	fprintf(stderr,"Usage: %s creditcard-number[s]\n", argv[0]);
	exit(1);
    }
    ++argv; --argc;
    while(argc > 0) {
	printf("%s - %s\n", *argv, DescribeCard(*argv));
	++argv; --argc;
    }
    return(0);
}
