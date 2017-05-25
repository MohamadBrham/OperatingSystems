#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>


#define FILE_NAME_SIZE  127
#define LINE_SIZE 80




typedef struct resultNode* resultPtr;


struct resultNode{                   // sruct to define the result format
    char      lineText[LINE_SIZE];
    char      fileName[FILE_NAME_SIZE];
    int       lineNum;
    resultPtr next;
};

void printResultToFile(FILE *out , resultPtr res);
int checkLine(char l[], char word[]);
void createFiles(int );
void deleteResultList (resultPtr L);
void insertResultNode(resultPtr r ,char filename[],char textLine[],int num);
void deleteTempFiles(int );
int main(int argc, char *argv[])
{
         

        resultPtr resultList =(resultPtr) malloc(sizeof(struct resultNode)); // linkedlist to save the results
        resultList->next = NULL;

        FILE *in ,*out ;                                                      // file pointers to open files in child & main

        char word[LINE_SIZE] ,buffer[FILE_NAME_SIZE];                                                // the word that we have to search for
        char  tempFileName[10],temp2[LINE_SIZE];                       //buffer to read file names
                                                                             // tempFileName
        int number;
         int i ,counter = 1;                                          //number of files , counters
        /* for(i=0;i<argc;i++){
              printf("argv[%d] = %s\n",i,argv[i]);
          }*/
if(argc > 2 ){                                                                     // temp2 is buffer for reading from files
       	strcpy(word, argv[1]);

        number = atoi(argv[2]);

        //int *pids = (int*)malloc(number * sizeof(int));
        if(number  < 1){
            printf("the number of files must be at least 1  ");
            return 0;
        }else if(number  > 20){
            printf("the maximum number of files must be 20");
            return 0;
        }
        createFiles(number );                                              // create N  temporarely files
        int pid;
    if(argc == (number + 4 )){
        for( i = 1 ; i <= number ; i++ ){                                 // creating child process

         pid = fork();

        if(pid== 0){                                                     // child process

           //free(resultList);                                              // delete the result list node (its just one node)
           strcpy(buffer,argv[i+2]);
           //strcat(buffer,".txt");
           in = fopen(buffer,"r");                                        // opening the file
           if( in == NULL){
              printf("file %s not opened correctly \n",buffer);           // exit child if file not opened
              exit(0);

           }else{
              sprintf(tempFileName,"temp%d",i);                  // if file in opened
              out = fopen(tempFileName,"w");                         // open the temp file for this file// word
              fprintf(out,"%s\n",buffer);
              while( fgets( temp2 , LINE_SIZE, in) != NULL){        // start reading from the original file
                  //printf("%s ",temp2);
                  if(checkLine(temp2,word)) {
                    printf("%s ",temp2);
                    fprintf(out,"%s \n %d\n",strtok(temp2 ,"\n"),counter);
                  }                        // if the line contain the desired word add the line to linkedlist
                 counter++;                                          // count the number of lines to insert it withh line
              }
           }
           //printf("welcome world\n");
           //printf("pids[0]  = %d\npids[1]  = %d \n ",pids[0] ,pids[1] ) &&
           // return iTMInChild(0);
           exit(0);                                       // exit from the child process to anothe child process
         }
          //pids[i-1]=pid;
         }//else if(pid > 0){
          //   pids[i-1] = pid;
          //  printf(" %d welcome %dto my worldddd\n", pids[0],pids[1]);
         //   for( i =0 ; i < number ;i++)
         //     waitpid(pids[i],NULL,0);                                              // wait for all children processes to terminate
              waitpid(-1, NULL, 0);
    

         // printf("  huhuhuu");
          int lineNumber;
            char FileNameBuffer[127];
            for( i = 1 ; i <= number;i++){
                 sprintf(tempFileName,"temp%d",i);            // create the temp file name
                 in = fopen(tempFileName,"r");                    // open the temp file
                 fscanf(in,"%[^\n]s",FileNameBuffer);             // get the original file for this temp file
                 if(in != NULL){                        // to handle the case of no lines in the temp file
                     while( fgets (temp2, LINE_SIZE, in) != NULL  ){
                          fscanf(in,"%d",&lineNumber);
                          insertResultNode(resultList,FileNameBuffer,temp2,lineNumber);
                     }              // gets the line and its numberand insert into the result linked list
                 }
                 fclose(in);        // close the temp file
            }
            strcpy(buffer,argv[argc-1]);
            //strcat(buffer,".txt");
            out = fopen(buffer,"w");                     // open the output file
            //printf("%s \n",buffer);
            printResultToFile(out,resultList);           // print the result to file
            deleteResultList(resultList);                // free result linked list
            deleteTempFiles(number);
                  }
           //       }
       //  }
         }


    return 0;
}


void printResultToFile(FILE *out , resultPtr res){
  resultPtr r = res;
  
  while(r->next != NULL ){
     if( strtok(r->next->lineText,"\n") != NULL )
        fprintf(out," %s : %s ( %d ) \n",r->next->fileName,strtok(r->next->lineText,"\n"),r->next->lineNum);

     r = r->next;
  }
  fclose(out);
}
void insertResultNode(resultPtr r ,char filename[],char textLine[],int num){
    resultPtr p = r;

    while(r->next != NULL && strcmp(filename,r->next->fileName) >= 0 ){ // insertion in the correct order due to file name
        if(strcmp(filename,r->next->fileName) == 0){
            while(r->next != NULL && strcmp(filename,r->next->fileName) == 0 && num < r->next->lineNum){
               r = r->next;
            }
            break;
        }else{
           r = r->next;
        }
    }
    resultPtr temp = (resultPtr)malloc(sizeof(struct resultNode));
    strcpy(temp->lineText,textLine);
    strcpy(temp->fileName,filename);
    temp->lineNum = num;

    temp->next = r->next;            // adding the node
    r->next = temp;
}

void deleteResultList (resultPtr L){
    resultPtr p = L -> next;
    while (L -> next != NULL){
        L -> next = p -> next;
        free(p);
        p = L -> next;
    }
}


 void createFiles(int N){
     int i;
     FILE *in;
     char temp[10];

    for( i=1 ; i <= N ; i++ ){
          sprintf(temp, "temp%d", i);
          in = fopen(temp,"w");
          fclose(in);
        }
 }

int checkLine(char l[], char word[]){               // check the word if its in the line or not
   int i;
   for(i=0; i< strlen(l) ;i++){
     if(l[i] == word[0]){
        if(strncmp(&l[i],word,strlen(word)) == 0)
          return 1;
     }
   }
   return 0;
}

void deleteTempFiles(int n){
	int i ;
 	char temp[10];
	for(i = 1 ;i <= n ;i++){
	   sprintf(temp, "temp%d", i);
	   remove(temp);
 	}

}
