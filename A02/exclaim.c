/*
 * exclaim.c
 * Author: Paprika Chen
 * Date: 2023/1/30
 * Exclaim what the user enter
 */

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main() {

  // ensure that random number would not be the same
  time_t t;
  srand((unsigned) time(&t));

  // the given array that would be randomly chosen
  char chosen[4] = {'@', '!', '#', '*'};

  printf("Enter a word: ");

  // the char array to store the input word
  char word[32];
  scanf("%s", word);

  printf("OMG! ");

  // go through the word array and check the case
  for (int i = 0; i < strlen(word); i ++) {
      // if the current char is uppercase
    if (64 < word[i] && word[i] < 91) {
    printf("%c", word[i]);
    }
      // if the current char is lowercase
    else if (96 < word[i] && word[i] < 123) {
    int random = rand() % 3;
    printf("%c", chosen[random]);
    }

      // if the input word is not a valid word
    else {
    printf("\nPlease enter a valid word");
    return 0;
    }

  }
  printf("\n");
  return 0;
}
