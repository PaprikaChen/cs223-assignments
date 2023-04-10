/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/4/5
 * uses N threads to search for a value in a list.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int* list;
int numThread;
int target;
int amount;
int found = 0;

void readIn(int number, int targetNum) {
  numThread = number;
  target = targetNum;
  // read the first line in the file
  FILE* fp = fopen("data.bin", "rb");
  if (fp == NULL) {
    printf("Error: unable to open file data.bin");
  }
  int totalAmount = 0;
  fread(&totalAmount, sizeof(int), 1, fp);
  if (totalAmount == 0) {
    printf("There is nothing to read!");
    exit(0);
  }

  amount = totalAmount;

  // create a 1d array using malloc
  list = malloc(amount * sizeof(int));
  // read the file and store info into the 1d array
  for (int i = 0; i < amount; i ++) {
    fread(&list[i], sizeof(int), 1, fp);
  }

  fclose(fp);
}

// the thread method
void *thread_search(void *args) {
  int id = *((int*)args);
  for (int i = id*(amount/numThread); i < ((id + 1) * (amount/numThread)); i ++) {
    if (list[i] == target) {
      printf("Thread %d found %d at index %d\n",(id + 1), target, i);
      found = 1;
    }
  }
  return (void*) 0;
}


int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  // get the user input
  printf("Enter a value to search: ");
  int target;
  scanf(" %d%*c", &target);

  int num = atoi(argv[1]);
  readIn(num, target);

  pthread_t* threads = malloc(num * sizeof(pthread_t));
  int* input = malloc(num * sizeof(int));

  for (int i = 0; i < num; i ++) {
    input[i] = i;
    pthread_create(&threads[i], NULL, thread_search, &input[i]);
  }

  for (int i = 0; i < num; i ++) {
    pthread_join(threads[i], NULL);
  }
  
  if (found == 0) {
    printf("Not found!\n");
  }

  free(list);
  free(threads);
  free(input);
  return 0;
}
