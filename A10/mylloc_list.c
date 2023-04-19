/**
  * Desc:
  *  This program rewrites a malloc method and a free method, which implements
  * a header that includes the requested memory, and reuse memory that are freed.
  * It also includes a test in main.
  * 
  * Modified: April 18, 2023 by @author Paprika Chen
  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0) {
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {

    if (next->size >= size) {
      if (prev != NULL) {
        prev -> next = next -> next;
      }
      else {
        flist = next -> next;
      }
      // update the size
      next->used = size;
      return (void*)(next + 1);
    }
    else {
      prev = next;
      next = next -> next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) - 1) {
    return NULL;
  }
  else {
    struct chunk *cnk = (struct chunk *)memory;
    cnk -> size = size;
    cnk -> used = size;
    return (void *)(cnk + 1);
  }

  return NULL;
}

void free(void *memory) {
  if (memory != NULL) {
    struct chunk *cnk = (struct chunk *)((struct chunk *) memory - 1);
    cnk -> next = flist;
    flist = cnk;
    //update the size
    cnk -> used = 0;
  }
  return;
}
