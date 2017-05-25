#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <pthread.h>

 //#define QUEUE_NAME  "/test_queue"
#define MAX_LINE_SIZE 128
#define MAX_MESSAGE_SIZE MAX_LINE_SIZE * 13



void *service(void *r  ){
     char *ptr ,*save;    
    char *requ =(char *)r; 
    printf("%s\n",requ);     
    

    int i = 0 , k = 0 , counter1 ,counter2;
    mqd_t mq2;
    char request[3][MAX_LINE_SIZE], line[MAX_LINE_SIZE] ,words[MAX_LINE_SIZE][MAX_LINE_SIZE] ,buffer[3*MAX_LINE_SIZE];
    char temp1[MAX_LINE_SIZE],temp2[2 * MAX_LINE_SIZE];
      i = 0 ;
        strcpy(request[i]  , strtok_r(requ, " ",&save));
        while (ptr = strtok_r(NULL, " ",&save)) {
          strcpy(request[++i],ptr);
         }   

    /* now request[0] contain the name of reply queue
     *  request[1] contain the word for search
     *  request[2] contain the file name
     */
    FILE *in;
    mq2 = mq_open(request[0], O_WRONLY);
    in = fopen(request[2],"r");
    if( in == NULL){
        mq_send(mq2, "there is a file not opened correctly", MAX_MESSAGE_SIZE, 0);
     }else{
// start reading from the  file
         counter1 = 0;
         counter2 = 0;
         while( fgets(line , MAX_LINE_SIZE, in) != NULL){
             strcpy(&line[(strlen(line) ? strlen(line):1)-1],"  $ \0");
             i =0;
             counter1++;
             strcpy(words[i]  , strtok_r(line, " ",&save));
             while (ptr=strtok_r(NULL, " ",&save)) {
               strcpy(words[++i],ptr);
            }            
             for(k = 0 ; k <= i ; k++){
                 if(strcmp(words[k],request[1]) == 0){
                   counter2++;
                   sprintf(temp1,"%d",counter1);
                   strcat(temp2,temp1);
                   strcat(temp2," ");
                  }
             }

          }
          sprintf(temp1,"< %s > ",request[2]);
          strcpy(buffer,temp1);
          sprintf(temp1," [%d]:",counter2);
          strcat(buffer,temp1);
          strcat(buffer,temp2);
          mq_send(mq2, buffer, MAX_MESSAGE_SIZE, 0);
          



     }
pthread_exit(0);

}

int main(int argc , char *argv[])
{
//create the request queue
    mqd_t requeststate;
    struct mq_attr at;
    char buffer[MAX_MESSAGE_SIZE ];
    at.mq_flags = 0;
    at.mq_maxmsg = 5 ;                                                       // the maximum number of concurrent clients
    at.mq_msgsize = MAX_MESSAGE_SIZE ;                                       //  the maximum size of every message request
    at.mq_curmsgs = 0;
    requeststate = mq_open(argv[1], O_CREAT | O_RDONLY, 0644, &at);

    char request[13][3*MAX_LINE_SIZE];
     char rr[10][128];
    int i , n , k;
    pthread_t pids[10];
    char req[3 * MAX_LINE_SIZE ];
    char *ptr;
    pthread_attr_t attr;
    //mqd_t mn;
//waitting all the time for new request
   while(1){
     ssize_t bytes_read;
      bytes_read = mq_receive(requeststate, buffer, MAX_MESSAGE_SIZE, NULL);
      buffer[bytes_read] = '\0';
      printf("%s \n",buffer);


//str tok the request      
        i = 0 ;
        strcpy(request[i]  , strtok(buffer, " "));
        while (ptr = strtok(NULL, " ")) {
          strcpy(request[++i],ptr);
         }      

        /* now request[0] contain the name of reply queue
        *  request[1] contain the word for search
        *  request[2] contain the number of files
        *  request[3..(N+2)] files names
        */
// create child process for every request

      n = fork();
       if(n == 0){
          pthread_attr_init(&attr);
          for(k = 0 ; k <= i -3; k++){
            pthread_attr_init(&attr);
             strcpy(rr[k],request[0]); //reply queue
             strcat(rr[k]," ");
             strcat(rr[k],request[1] ); // search
              strcat(rr[k]," ");
             strcat(rr[k],request[k+3] ); // file 
             strcat(rr[k],"\0" );
           //  printf("child %s\n",req);
            //  mn  = mq_open(request[0], O_WRONLY);  
              //mq_send(mn, req, MAX_MESSAGE_SIZE, 0);          
             pthread_create(&pids[k],&attr,service,rr[k]);
          }
         for(k = 0 ; k <= i-3 ; k++){
             pthread_join(pids[k],NULL);
          }
         exit(0);
        }
     

    }
 
return 0;
}

