#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "buddy.h"

int main(int argc, char *argv[])
{
	void *chunkptr;
	void *endptr;
	char *charptr;

	int ret;
	int i;
	int size;
	void *x1, *x2, *x3;	// object pointers
	void *x[10];

	clock_t start , end;
    start = clock();

	if (argc != 2) {
		printf("usage: app <size in KB>\n");
		exit(1);
	}

	size = atoi(argv[1]);

	// allocate a chunk 
	chunkptr = sbrk(0);	// end of data segment
	sbrk(size * 1024);	// extend data segment by indicated amount (bytes)
	endptr = sbrk(0);	// new end of data segment

	printf("chunkstart=%lx, chunkend=%lx, chunksize=%lu bytes\n",
		   (unsigned long)chunkptr,
		   (unsigned long)endptr, (unsigned long)(endptr - chunkptr));

	//test the chunk 
	printf("---starting testing chunk\n");
	charptr = (char *)chunkptr;
	for (i = 0; i < size; ++i)
		charptr[i] = 0;
	printf("---chunk test ended - success\n");

	ret = binit(chunkptr, size);
	if (ret == -1) {
		printf("could not initialize \n");
		exit(1);
	}
	printf("print initial status \n");

	int y = (30720 * 1024) / atoi(512) ; 
    for(i=0 ; i< y ; i++){
    	x[i%10 ] = balloc(512);
    }
	
	 
    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	time_spent = time_spent ; //milliseconds now.

	printf("the pro end at time : %3.8f\n",time_spent);

	return 0;
/*st = time(NULL);
	printf("the pro end at time : %s",ctime(&st));*/
}
