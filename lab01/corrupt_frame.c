#include <stdlib.h>
#include <time.h>

/*  This is a simple function designed to test checksum algorithms.
    As corruption is random, you'll need to call srand() from somewhere
    to ensure different random sequences.

    Here we randomly corrupt the contents of the given frame of characters.
    You may like to devise other forms of corruption and run tests on them.
 */

void corrupt_frame(unsigned char *frame, int length)
{
	if(length > 1) {
		//srand (time(NULL));
		int which = (rand() % (length-1)) + 1;
		
		unsigned char save = frame[which-1];
		frame[which-1] = frame[which];
		frame[which] = save;
		
		//Alternative way to corrupt
		//frame[which]  = ~(frame[which]) ;
	}
}
