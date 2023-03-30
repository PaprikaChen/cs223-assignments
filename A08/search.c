/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/3/28
 * uses two processes to search for a value in a list
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  // read the first line in the file
  FILE* fp = fopen("data.bin", "rb");
  if (fp == NULL) {
    printf("Error: unable to open file data.bin");
  }
  int amount = 0;
  fread(&amount, sizeof(int), 1, fp);
  if (amount == 0) {
    printf("There is nothing to read!");
    exit(0);
  }

  // create a 2d array using malloc
  int* list = malloc(amount * sizeof(int));
  // read the file and store info into the 2d array
  for (int i = 0; i < amount; i ++) {
    fread(&list[i], sizeof(int), 1, fp);
  }

  fclose(fp);
  
  // get the user input
  printf("Enter a value to search: ");
  int target;
  scanf(" %d%*c", &target);

  // split the work using fork() and search
  int parentFound = 0;
  int childFound = 0;
  pid_t p;
  p = fork();
  if (p > 0) { // parent
    for (int i = amount / 2; i < amount; i ++) {
      if (list[i] == target) {
        printf("Parent found %d at index %d\n", target, i);
        parentFound = 1;
      }
    }
    if (parentFound == 0) {
      printf("Not found!\n");
    }
  }
  else { // child
    for (int i = 1; i < amount / 2; i ++) {
      if (list[i] == target) {
        printf("Child found %d at index %d\n", target, i);
        childFound = 1;
      }
    }
    if (childFound == 0) {
      printf("Not found!\n");
    }
  }


  // free list
  free(list);

  return 0;
}
