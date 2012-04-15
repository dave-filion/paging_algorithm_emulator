#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define M 1000
#define DISK_SIZE 10000

int MemoryLookupTable[M];
int PhysicalMemory[M];
int MetaTable[M];

int Disk[DISK_SIZE];

int MemoryLock; // Memory routines need lock to access memory
int seq; // Global sequential counter

/* Checks if X_i is present in memory. */
/* Returns address, or -1 if not present */
int Fetch(int i);

/* Implements paging algorithm */
int PageIn(int i);

/* Lets user obtain memory lock */
int obtainMemoryLock();

int main() {


  return 0;
}
