#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "vsfs.h"

//vars
char diskname[128]; 
char filename[16][MAXFILENAMESIZE]; 
int i, n; 
int fd[12];       // file handles
char buf[MAXREADWRITE]; 


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
	
	printf("------------- app1 started ------------------------ \n");
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
	//----- create file0 of size 1.5MB 
	create2(filename[0]);
	fd[0] = open2(filename[0]); 

	
	//fill file0 with 1.5 MB of data
	for (i=0; i<(3*1024); i++) {
		n = vsfs_write(fd[0], buf, 512);  
		if (n != 512) {
			printf ("file %s write failed\n", filename[0]); 
			exit (1); 
		}
		
	}
	
	

	printf ("\nFile list:\n");
	vsfs_print_dir();
	printf ("size of file0 %d \n", vsfs_filesize(fd[0]) ); 
	printf ("\n");
	
	close2(fd[0], filename[0]); 	
	
	
	
	//----------------------------------------------------------------------------------------
	//----- create file1 of size 1MB (should fail and will be created partially )
	create2(filename[1]);
	fd[1] = open2(filename[1]);
	
	//fill file1 with 1 MB of data
	for (i=0; i<(2*1024); ++i) {
		n = vsfs_write (fd[1], buf, 512);  
		if (n != 512) {
			printf ("file %s write failed as expected\n", filename[1]); 
			break; //exit (1); 
		}
	}
	
	close2(fd[1], filename[1]); 	
	print_folder_info();

	//----------------------------------------------------------------------------------------
	//----- delete incomplete file1 
	delete2(filename[1]);
	print_folder_info();	
	
	//----------------------------------------------------------------------------------------
	//----- truncated file0
	fd[0] = open2(filename[0]); 
	trunc2(fd[0], filename[0], 512*1024);
	close2(fd[0], filename[0]); 	
	print_folder_info();
	
	//----------------------------------------------------------------------------------------
	//----- create file1 again of size 1MB (this time has to be successful)
	create2(filename[1]);
	fd[1] = open2(filename[1]); 

	
	//fill it with 1 MB of data
	for (i=0; i<(2*1024); ++i) {
		n = vsfs_write (fd[1], buf, 512);  
		if (n != 512) {
			printf ("file %s write failed, this time it is NOT expected \n", filename[1]); 
			exit (1); 
		}
	}
	
	close2(fd[1], filename[1]); 		
	print_folder_info();		

	

	vsfs_umount(); 
	printf ("------------- app1 finished ----------------------- \n");
	printf ("--------------------------------------------------- \n");		
	
	return (0);		
}
