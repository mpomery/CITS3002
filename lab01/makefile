#! /usr/bin/make

DIRT = *.o *.i *~ */*~ *.log *.exe

all : checksumtest

checksumtest:
	gcc -std=c99 checksum_test.c -o checksum_test
	checksum_test

creditcard:
	gcc creditcard.c -o creditcard

clean:
	$(RM) $(DIRT)

clobber:
	$(RM) $(DIRT)
