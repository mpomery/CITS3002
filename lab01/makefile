#! /usr/bin/make

DIRT = *.o *.i *~ */*~ *.log *.exe

all : intreader

checksumtest:
	gcc -std=c99 checksum_test.c -o checksum_test
	checksum_test

creditcard:
	gcc creditcard.c -o creditcard

intreader:
	clear
	gcc -std=c99 intreader.c -o intreader
	intreader

clean:
	$(RM) $(DIRT)

clobber:
	$(RM) $(DIRT)
