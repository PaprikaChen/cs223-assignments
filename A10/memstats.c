/**
  * Desc:
  *  This program keeps track of the total/free/used blocks of malloc memories.
  * It also calculates the current the amounts of total/free/used memories, and
  * calculate the percentage of the underutilized memory.
  * 
  * Modified: April 18, 2023 by @author Paprika Chen
  */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {

  // calculate the number of free blocks and the free memmory
  struct chunk * node = freelist;
  int freeBlock = 0;
  int freeMem = 0;
  while (node != NULL) {
    freeBlock ++;
    freeMem = freeMem + node -> size;
    node = node -> next;
  }

  // calculate the usedBlock, used memory, and the unapplicated memory.
  int usedBlock = 0;
  int usedMem = 0;
  int unappli = 0;
  for (int i = 0; i < len; i ++) {
    if (buffer[i] != NULL) {
      usedBlock ++;
      // jump back to the header to read the memory
      struct chunk* header = (struct chunk*) ((struct chunk*)buffer[i] - 1);
      usedMem = usedMem + header -> size;
      // if there is unused memory
      if (header->size > header->used) {
        unappli = unappli + (header->size - header->used);
      }
    }
  }

  // calculate the total memory, the percentage of unutilized, and the total blocks
  int totalMem = usedMem + freeMem;
  float underUtil = unappli  / (float)usedMem;
  int totalBlock = freeBlock + usedBlock;

  printf("Total blocks: %d Free blocks: %d Used blocks: %d\n", totalBlock, freeBlock, usedBlock);
  printf("Total memory allocated: %d ", totalMem);
  printf("Free memory: %d Used memory: %d\n", freeMem, usedMem);
  printf("Underutilized memory: %.2f\n", underUtil);

}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
