/* $Id: hash.c,v 1.2 2015/11/08 18:47:10 korpe Exp $ */


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "hash.h"

typedef struct node* ptr;
struct node {
    int number;    
    ptr next;
};

ptr *table ; 
pthread_mutex_t re[MAXTH];
pthread_mutex_t wr[MAXTH];
int counter[MAXTH];
int N ;


/* 
   hash table definition here - array of linked lists. 
   it is a shared data structure 
*/


void hash_init(int n)
{	
	N = n ;

	int  i = 0;
	table   = malloc( sizeof(struct node) * N);
	for( i = 0 ; i < N ; i++){                
		table[i] =  malloc( sizeof(struct node));
                table[i]->next = NULL ; 		
	}
        for( i = 0 ; i < N ; i++){                
		counter[i]  = 0 ; 		
	} 
        for( i = 0 ; i < 20 ; i++){
                pthread_mutex_init(&re[i], NULL);
                pthread_mutex_init(&wr[i], NULL);       
        }
}
int hash_insert(int k)
{

	int index = k % N;	
	ptr element = malloc( sizeof(struct node));
	if(element != NULL ){

        pthread_mutex_lock(&wr[index]);
	element->number = k ;                        //
	element->next = table[index]->next;          // 
	table[index]->next = element ;               // critical secrion writing
  	pthread_mutex_unlock(&wr[index]); 
   
	return (0); // succsses
	}
	return -1 ; // fail
}

int hash_delete(int k)
{

	int index = k % N;
	ptr p = table[index] ;
	ptr temp;

pthread_mutex_lock(&re[index]);
counter[index]++;
if(counter[index] == 1)    
    pthread_mutex_lock(&wr[index]);
pthread_mutex_unlock(&re[index]);


        	
	while(p->next != NULL && p->next->number != k)          // critical section reading read 
		p = p->next ;                                  //

pthread_mutex_lock(&re[index]);
counter[index]--;
if(counter[index] == 0)
    pthread_mutex_unlock(&wr[index]);
pthread_mutex_unlock(&re[index]);


	if(p->next != NULL){  


                pthread_mutex_lock(&wr[index]);
		temp = p->next;                                 // 
		p->next = temp->next;                           // critical section write
		free(temp);                                     // 
  	        pthread_mutex_unlock(&wr[index]);             

  
		return 0;
	}else                
		return (-1);

}

int hash_get(int k)
{
        //printf("hash getting %d\n", k);
	int index = k % N;
	ptr p = table[index] ;
	p = p->next ;

pthread_mutex_lock(&re[index]);
counter[index]++;
if(counter[index] == 1)    
    pthread_mutex_lock(&wr[index]);
pthread_mutex_unlock(&re[index]);


	while(p != NULL && p->number != k)            //critical section  read
		p = p->next ;

pthread_mutex_lock(&re[index]);
counter[index]--;
if(counter[index] == 0)
    pthread_mutex_unlock(&wr[index]);
pthread_mutex_unlock(&re[index]);

	if(p != NULL)
		return k;
	else
		return (-1);
}
