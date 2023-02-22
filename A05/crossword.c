/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/19
 * This program generates a crossword using two words from a user, identify a common letter 
 * between the given words and list one word vertically and the other word horizontally.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node{
  char letter;
  struct node* next;
};

void clear(struct node* head) {
  struct node* point;
  // clean the whole list
  while (head != NULL) {
    point = head;
    head = head -> next;
    free(point);
  }
  return;
}

char getCommon(int count, char* words[]) {
  char common = '0';
  struct node* head = NULL;
  // loop through all words
  for (int i = 1; i < (count); i ++) {
    // for each word, loop through each letter
    for (int j = 0; j < strlen(words[i]); j ++) {

      // for each letter, loop through the linked list and add it to the linked list 
      struct node* k = head;
      while (k != NULL) {
        // if there is already a same letter in the linked list
        if ((k -> letter) == (words[i])[j]) {
          common = k -> letter;
          clear(head);
          return common;
        }

        k = k -> next;
      }

    }
    for (int k = 0; k < strlen(words[i]); k ++) {
      //if not, add it to the linked list
      struct node* showUp = malloc(sizeof(struct node));
      if (showUp == NULL) {
        printf("ERROR:Out of space!\n");
        exit(1);
      }
      showUp -> letter = (words[i])[k];
      showUp -> next = head;
      head = showUp;
    }

  }
  clear(head);
  // if there is no common letter found
  return common;
} 


void print(char** grid, int lengthA, int lengthB) {
  for (int i = 0; i < lengthA; i ++) {
    for (int j = 0; j < lengthB; j ++) {
      printf("%c ", grid[i][j]);
    }
    printf("\n");
  }
}

int main( int argc, char* argv[]) {

  // argc is the number of strings
  if (argc < 3) {
    printf("Please enter at least two words.\n");
    exit(1);
  }

  // find the common letter
  char common = getCommon(argc, argv);
  if (common == '0') {
    printf("No common letter!\n");
    exit(1);
  }

  // find the two words that contains the common letter
  char* wordA;
  char* wordB;
  int lengthA = 0;
  int lengthB = 0;

  int count = 0;
  int aPlace = 0;
  // loop through each word
  for (int i = 1; i < argc; i ++) {
    // loop through each letter
    
    for (int j = 0; j < strlen(argv[i]); j ++) {
      if ((argv[i])[j] == common) {
        if (count == 0) {
          aPlace = i;
          wordA = argv[i];
          lengthA = strlen(wordA);  
          count ++;
        }
        if (count == 1 && i != aPlace) {
          wordB = argv[i];
          lengthB = strlen(wordB);
          count ++;
        }

        // the first two words are found

      }
    }
  }



  char** grid = malloc((lengthA) * sizeof(char*));
  for (int i = 0; i < lengthA; i ++) {
    grid[i] = malloc(sizeof(char) * (lengthB));
  }

  // find the place of word A in the grid
  int placeOfA = 0;
  for (int i = 0; i < lengthB; i ++) {
    if (wordB[i] == common) {
      placeOfA = i;
      break;
    }
  }

  // find the place of word B in the grid
  int placeOfB = 0;
  for (int i = 0; i < lengthA; i ++) {
    if (wordA[i] == common) {
      placeOfB = i;
      break;
    }
  }

  // put '-' in blank space
  for (int i = 0; i < lengthA; i ++) {
    for (int j = 0; j < lengthB; j ++) {
        grid[i][j] = '-';

    }
  }


  // put word A into the grid
  for (int i = 0; i < lengthA; i ++) {
    grid[i][placeOfA] = wordA[i];
  }

  // put word B into the grid
  for (int i = 0; i < lengthB; i ++) {
    grid[placeOfB][i] = wordB[i];
  }



  print(grid, lengthA, lengthB);

  //free grid
  for (int i = 0; i < lengthA; i ++) {
     free(grid[i]);
  }
  free(grid);

// free problem

  return 0;
}

