#ifndef BUDDY_H
#define BUDDY_H
#define NUM_POINTERS 19
#define FULL_R 2
#define FULL_L 3
#define EMPTY_R 0
#define EMPTY_L 1


int binit(void *, int);
void *balloc(int);
void bfree(void *);
void bprint(void); 
int roundSize(int );

#endif
