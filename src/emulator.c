#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define N 10000
#define M 10
#define K 10
#define DISK_SIZE 100000

int MemoryLookupTable[M];
int PhysicalMemory[M];
int MetaTable[M];
int Disk[N];

pthread_mutex_t memoryLock;
int seq; // Global sequential counter
int mode; // Eviction mode to use for paging

/* Checks if X_i is present in memory. */
/* Returns address, or -1 if not present */
int Fetch(int i);

/* Implements paging algorithm */
int PageIn(int i);

/* Lets user obtain memory lock */
int obtainMemoryLock();

/* Releases the memory lock */
int releaseMemoryLock();

void removeLeastRecentlyUsed(int val, int i);
void removeMostRecentlyUsed(int val, int i);
void removeRandom(int val, int i);

int memoryIsFull();
int findOpenMemorySlot();
void updateMetaTable(int i);

/* Pages values into memory needed for computations */
int PageIn(int i) {
  int result;
  result = Fetch(i);

  if (result == -1) {
    // Perform paging of memory.

    int val;
    val = Disk[i];

    if (memoryIsFull()) {
      // Evict a cell and replace it with val.
      switch(mode) {
         case 1:
         removeLeastRecentlyUsed(val, i);
         break;
         
         case 2:
         removeMostRecentlyUsed(val, i);
         break;
         
         case 3:
         removeRandom(val, i);
         break;
         
         default:
         printf("Unkown Eviction Mode\n");
         exit(-1);
      }
      return val;

    } else {
       int openSlot = findOpenMemorySlot();
       PhysicalMemory[openSlot] = val;
       MemoryLookupTable[openSlot] = i;

       // Update meta table with time
       updateMetaTable(openSlot);
       return val;
    }
  } else {
    return result;
  }
}

/* Puts current time in metatable slot i */
void updateMetaTable(int i) {
  if (mode != 3) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    MetaTable[i] = tv.tv_sec;
  }
}

/* Gets Physical Memory Value From Table */
int Fetch(int i) {
  int j;
  for (j = 0; j < M; j++) {
    if (i == MemoryLookupTable[j]) {
      updateMetaTable(j);
      return PhysicalMemory[j];
    }
  }
  return -1;
}


void* thread_1(void* data) {
    int i;
    int s = 0;
    int x;

    //    while (TRUE) {
      s = 0;
      for (i = 0; i < K; i++) {
	obtainMemoryLock();
	x = PageIn(i);
	releaseMemoryLock();
	s += x;
      }

      // Add on random value
      obtainMemoryLock();
      x = PageIn((int) (rand() % N));
      releaseMemoryLock();
      s += x;

      printf("1, %d\n", seq);
      //    }
}

void* thread_2(void* data) {
    int i;
    int s;
    int x;
    int round = 0;

    //    while (TRUE) {
      s = 0;

      if (round == N) {
	round = 0;
      }

      int end;
      end = K + round;

      // Calculate common set
      for (i = round; i < end; i++) {
	obtainMemoryLock();
	x = PageIn(i);
	releaseMemoryLock();
	s += x;
      }

      printf("2, %d\n", seq);
      //    }
}

void* thread_3(void* data) {
    int i;
    int s;
    int x;

    //    while (TRUE) {
      s = 0;

      // Calculate common set
      for (i = 0; i < K; i++) {
	obtainMemoryLock();
	x = PageIn((int) rand() % K);
	releaseMemoryLock();
	s += x;
      }

      printf("3, %d\n", seq);
      //    }
}

int obtainMemoryLock() {
  pthread_mutex_lock (&memoryLock);
  seq++;
  return 0;
}

int releaseMemoryLock() {
  pthread_mutex_unlock(&memoryLock);
  return 0;
}

void removeLeastRecentlyUsed(int val, int i) {
   // Check meta table for least recent activity
  int removalIndex;
  int j;

  removalIndex = 0;
  // Get Min time in Meta Table
  for (j = 0; j < M; j++) {
    if (MetaTable[removalIndex] > MetaTable[j]) {
      removalIndex = j;
    }
  }

  MemoryLookupTable[removalIndex] = i;
  PhysicalMemory[removalIndex] = val;
}

void removeMostRecentlyUsed(int val, int i) {
   // Check meta table for most recent activity
  int removalIndex;
  int j;

  removalIndex = 0;

  // Get Max time in Meta Table
  for (j = 0; j < M; j++) {
    if (MetaTable[removalIndex] < MetaTable[j]) {
      removalIndex = j;
    }
  }

  MemoryLookupTable[removalIndex] = i;
  PhysicalMemory[removalIndex] = val;
}

/* Evicts a random cell and replaces it with */
/* new page. */
void removeRandom(int val, int i) {
   int removalIndex;
   removalIndex = (int) rand() % M;
   
   MemoryLookupTable[removalIndex] = i;
   PhysicalMemory[removalIndex] = val;  
}

/* Finds index of open memory slot */
int findOpenMemorySlot() {
   int i;
   for (i = 0; i < M; i++) {
      if (MemoryLookupTable[i] == -1) {
         return i;
      }
   }
   
   printf("SOMETHING HAS GONE HORRIBLY WRONG\n");
   exit(-1);
}

/* Checks if there is an empty */
/* slot in the memory lookup table */
int memoryIsFull() {
   int i;
   for (i = 0; i < M; i++){
      if (MemoryLookupTable[i] == -1) {
         return FALSE;
      }
   }
   return TRUE;
}

int main(int argC, char** argV) {
   
   if (argC != 2) {
      printf("Usage: <Eviction Mode>\n");
      exit(-1);
   }
   
   // Get mode
   mode = atoi(argV[1]);
   
  // Seed random
  srand (time(NULL));

  // Initialize memory
  int i;
  for (i = 0; i < M; i++) {
    MemoryLookupTable[i] = -1;
    PhysicalMemory[i] = -1;
    MetaTable[i] = -1;
  }

  // Initialize disk
  for (i = 0; i < N; i++) {
    Disk[i] = (int) rand() % 100;
  }

  // Initialize threads
  pthread_t thread1, thread2, thread3;
  int iret1, iret2, iret3;

  // Init mutual exclusion variable
  pthread_mutex_init(&memoryLock, NULL);

  // Create threads
  iret1 = pthread_create( &thread1, NULL, thread_1, (void*) NULL);
  iret2 = pthread_create( &thread2, NULL, thread_2, (void*) NULL);
  iret3 = pthread_create( &thread3, NULL, thread_3, (void*) NULL);

  // Join threads so that they wait for each other to finish
  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);
  pthread_join( thread3, NULL);

  return 0;
}
