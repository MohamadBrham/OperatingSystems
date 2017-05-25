
/* $Id: app1.c,v 1.1 2015/11/08 18:25:06 korpe Exp $ */

#include <pthread.h>
#include "dp.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void *philosopher(void *p)
{
	time_t start, stop ,s;
	

	int num = (long)p;
        int slp = (rand()%5) + 1 ;      // random number from 1 to 5
	/* think - in THINKING state */
	sleep( slp );
	s = time(NULL);
	time(&start);	          
	dp_get_forks(num);
	
	/* eat – in EATING state */

	sleep(slp);
	dp_put_forks(num);
	 time(&stop);
        printf(" # : %2d \t %d  , \t\t %.3f , \t : %s ",num,slp, difftime(stop, start) , ctime(&s));	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	time_t st;
	st = time(NULL);
	printf("the pro start at time : %s",ctime(&st));
	printf("threadID	eatingTime	duration	startTime\n");
	pthread_t tid[MAXTH];
	int N = atoi(argv[1]);
	int i;

	dp_init(N);

	for (i = 0; i < N; ++i) {
		pthread_create(&tid[i], NULL, &philosopher, (void *)(long)i);
	}

	for (i = 0; i < N; ++i)
		pthread_join(tid[i], NULL);	
	st = time(NULL);
	printf("the pro end at time : %s",ctime(&st));

	return (0);
}
