/* $Id: dp.c,v 1.2 2015/11/08 18:58:21 korpe Exp $ */


#include "dp.h"
#include <stdio.h>
#include <pthread.h>

/* 
   - global variable definitions here.
   - define your mutex and condition  variables here (shared) 
   - define your state variables here. (shared)
   - note that the variable you define here are shared threads
   ....
*/
int N;
int *state;
pthread_mutex_t mut;
pthread_cond_t *con;




void dp_init(int n)
{
	//printf("Hello I am DP library\n");
 	 int i;
  	 N = n ;
 	 pthread_mutex_init(&mut, NULL);                                 
 	 state =  malloc( N * sizeof(int));                    
 	 con   = malloc( N * sizeof(pthread_cond_t));  
 	 for( i = 0 ; i < N ; i ++ ){ 
		pthread_cond_init(&con[i], NULL);                    
                state[i] = THINKING;                                  
         }
}

void dp_get_forks(int i)
{
	//printf("getting forks\n");
	 pthread_mutex_lock(&mut);
 	 state[i] = HUNGRY;
         while (state[(i + 1 )% N] == EATING || state[(i + (N-1))% N] == EATING  ) {
	 	   pthread_cond_wait(&con[i],&mut);
	 }
 	 state[i] = EATING;
  	 pthread_mutex_unlock(&mut);
}

void dp_put_forks(int i)
{
	//printf("putting forks\n");
        pthread_mutex_lock(&mut);
  	state[i] = THINKING;
  	if (state[(i+(N-1))%N] == HUNGRY) {
  	  pthread_cond_signal(&con[(i+(N-1))%N]);
 	 }
 	 if ( state[(i+1)%N] == HUNGRY ) {
 	   pthread_cond_signal(&con[(i+1)%N]);
 	 }
 	 pthread_mutex_unlock(&mut);
}
