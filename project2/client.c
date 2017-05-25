#include <stdio.h>
#include <stdlib.h>
//client
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
//#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE        128                                                 // max number of messages in the queue
#define MAX_MESSAGE_SIZE 13* 128                                            // the max size for every message

int main(int argc,char *argv[]){
    int i ,pid = getpid();

//create the name of the reply queue by pid
    char replyqueue[MAX_SIZE];
    sprintf(replyqueue,"/rq_%d",pid);

//create the request message
    char buffer[MAX_MESSAGE_SIZE];
    strcpy(buffer , replyqueue);                                             // include the name of reply queue
    strcat(buffer," ");
    for(i = 0 ; i < atoi(argv[3])+2 ; i++ ){                                 // put the name of files and ord
         strcat(buffer,argv[i+2]);                                        
         strcat(buffer," ");
     }

// create the replyqueue
    mqd_t replystate;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10 ;                                                    // the maximum number of concurrent clients
    attr.mq_msgsize = MAX_MESSAGE_SIZE ;                                     //  the maximum size of every message request
    attr.mq_curmsgs = 0;                                                     // the current messages in the queue
    replystate = mq_open(replyqueue, O_CREAT | O_RDONLY, 0644, &attr);       // create the reply queue

//open the requestqueue and send the request
        mqd_t mq2;
        mq2 = mq_open(argv[1], O_WRONLY);        
        mq_send(mq2,buffer, MAX_MESSAGE_SIZE , 0);
       // printf("%s\n",buffer);
        ssize_t bytes_read;

// open N messages from reply queue
     for(i = 0 ; i < atoi(argv[3]); i++ ){    
         bytes_read = mq_receive(replystate, buffer, MAX_MESSAGE_SIZE , NULL);// receive N messages ( blocking mode ) 
         buffer[bytes_read] = '\0';
         printf("%s\n",buffer);
     }
return 0;
}
