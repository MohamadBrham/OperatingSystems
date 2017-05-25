#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "vsfs.h"

//vars
char diskname[128]; 
char filename[16][MAXFILENAMESIZE]; 
int i,j, n; 
int fd[12];       // file handles
char buf[MAXREADWRITE]; 

union Data {
   int num;
   char str[4];
} Data; 

union Data temp;



void mount2(char *diskname){
	if (vsfs_mount(diskname) != 0) {
		printf ("could not mount %s\n", diskname); 
		exit (1); 
	}
	else 
		printf ("filesystem %s mounted\n", diskname); 
}

void create2(char *filename){
	if (vsfs_create(filename) != 0) {
		printf ("could not create file %s\n", filename); 
		exit (1); 
	}
	else 
		printf ("file %s created\n", filename); 	
}

int open2(char *filename){
	int fd = vsfs_open(filename); 
	if (fd == -1) {
		printf ("file open failed: %s\n", filename); 
		//exit (1); 
	}
	return fd;
}

void delete2(char *filename){
	if (vsfs_delete(filename) == -1) {
		printf("file can't be deleted: %s\n", filename); 
		exit (1); 
	}
	else 
		printf ("file %s deleted\n", filename);	
}

void trunc2(int fd, char *filename, int size){
	if (vsfs_truncate (fd, size)== -1) {
		printf ("file can't truncated: %s\n", filename); 
		exit (1); 
	}
	else 
		printf ("file %s truncated\n", filename);		
}

void close2(int fd, char *filename){
	if (vsfs_close(fd)== -1) {
		printf ("file can't deleted: %s\n", filename); 
		exit (1); 
	}

}

void print_folder_info(){
	printf ("\nFile list:\n");
	vsfs_print_dir();
	int i;
	for (i=0; i <= 11; i++) {
		fd[i] = open2(filename[i]);
		if ( fd[i] != -1 ){
			printf("size of file%d : %d \n", i, vsfs_filesize (fd[i]) ); 			
			close2(fd[i], filename[i]);			
		}
	}		
	printf ("\n");		
	
}


 


int main(int argc, char *argv[])
{
	
	printf ("------------- app4 started ------------------------ \n");
	printf("--------------------------------------------------- \n");
	
	strcpy(filename[ 0], "file0"); 
	strcpy(filename[ 1], "file1"); 
	strcpy(filename[ 2], "file2"); 
	strcpy(filename[ 3], "file3"); 
	strcpy(filename[ 4], "file4"); 
	strcpy(filename[ 5], "file5"); 
	strcpy(filename[ 6], "file6"); 
	strcpy(filename[ 7], "file7"); 
	strcpy(filename[ 8], "file8"); 
	strcpy(filename[ 9], "file9"); 
	strcpy(filename[10], "file10"); 
	strcpy(filename[11], "file11"); 
	
	for (i=0; i<=11; i++) {
		fd[i] = -1;
	}
	
	
	if (argc != 2) {
		printf ("usage: app <diskname>\n"); 
		exit (1);
	}
       
	strcpy (diskname, argv[1]); 
	mount2(diskname);
	
	//----------------------------------------------------------------------------------------
	//----- create file0 & file1

	create2(filename[0]);
	create2(filename[1]);	
	fd[0] = open2(filename[0]); 
	fd[1] = open2(filename[1]); 	

	for (i=0; i<(64*1024)-1; i++) {

		temp.num = 2*i;
		buf[0] = temp.str[0];
		buf[1] = temp.str[1];
		buf[2] = temp.str[2];
		buf[3] = temp.str[3];
		n = vsfs_write(fd[0], buf, 4);  
		if (n != 4) {
			printf ("file %s write failed\n", filename[0]); 
			exit (1); 
		}
		
		temp.num = 2*i+1;
		buf[0] = temp.str[0];
		buf[1] = temp.str[1];
		buf[2] = temp.str[2];
		buf[3] = temp.str[3];
		n = vsfs_write(fd[1], buf, 4);  
		if (n != 4) {
			printf ("file %s write failed\n", filename[1]); 
			exit (1); 
		}		
		
	}
	close2(fd[0], filename[0]);
	close2(fd[1], filename[1]);	
	
	print_folder_info();
	vsfs_print_fat(); 
	
	//----------------------------------------------------------------------------------------	
	//----------------------------------------------------------------------------------------
	//----- read file0 and file 1 and write file2
	create2(filename[2]);
	fd[0] = open2(filename[0]);
	fd[1] = open2(filename[1]);
	fd[2] = open2(filename[2]);
	
	for (i=0; i<(64*1024)-1; i++) {
		
		//read even number from file0
		n = vsfs_read(fd[0], buf, 4); 
		if (n != 4) {
			printf ("file %s read failed\n", filename[0]); 
			exit (1); 
		}		
		temp.str[0] = buf[0];
		temp.str[1] = buf[1];
		temp.str[2] = buf[2];
		temp.str[3] = buf[3];
		int ev = temp.num;
		//printf ("even read value %d \n", ev); 
		
		//read odd number from file1
		n = vsfs_read(fd[1], buf, 4); 
		if (n != 4) {
			printf ("file %s read failed\n", filename[1]); 
			exit (1); 
		}		
		temp.str[0] = buf[0];
		temp.str[1] = buf[1];
		temp.str[2] = buf[2];
		temp.str[3] = buf[3];
		int od = temp.num;
		//printf ("odd read value %d \n", od); 
		
		
		//write even&odd to file2
		temp.num = ev;
		buf[0] = temp.str[0];
		buf[1] = temp.str[1];
		buf[2] = temp.str[2];
		buf[3] = temp.str[3];	
		temp.num = od;
		buf[4] = temp.str[0];
		buf[5] = temp.str[1];
		buf[6] = temp.str[2];
		buf[7] = temp.str[3];
		
		n = vsfs_write(fd[2], buf, 8); 
		if (n != 8) {
			printf ("file %s write failed\n", filename[2]); 
			exit (1); 
		}		
		

	}	
	
	close2(fd[0], filename[0]);
	close2(fd[1], filename[1]);
	close2(fd[2], filename[2]);	
	
	print_folder_info();	
	
	



	//----------------------------------------------------------------------------------------	
	//----------------------------------------------------------------------------------------
	//----- read file2 and check
	fd[2] = open2(filename[2]);
	printf ("print file2 values:\n");
	for (i=0; i<(128*1024)-1; i++) {
		
		//read even number from file3
		n = vsfs_read(fd[2], buf, 4); 
		if (n != 4) {
			printf ("file %s read failed\n", filename[2]); 
			printf ("i:%d\n",i); 
			exit (1); 
		}		
		temp.str[0] = buf[0];
		temp.str[1] = buf[1];
		temp.str[2] = buf[2];
		temp.str[3] = buf[3];
		if ( temp.num != i ) 
			printf ("read result is wrong it is %d but should be %d\n", temp.num, i);
		//printf("%d-", temp.num);
	}
	
	close2(fd[2], filename[2]);	
	

	vsfs_umount(); 
	printf ("------------- app4 finished ----------------------- \n");
	printf ("--------------------------------------------------- \n");		
	
	return (0);		
}
