/*
 * acronym.c
 * Author: Paprika Chen
 * Date: 2023/1/30
 * compute an acronym from a given phrase
 */

#include <stdio.h>
#include <string.h>

int main() {
  // the string to store the input phrase
  char phrase[1024];
  // the string to store the acronym of the input phrase
  char acronym[1024];
  // the integer that counts the number of characters in acronym
  int count = 0;

  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);

  // the integer that stores the length of the whole phrase
  int length = strlen(phrase);
  for (int i = 0; i < length; i ++) {
      // if the current character is uppercase
      if (phrase[i] > 64 && phrase[i] <91) {
          // store it into the acronym string
          acronym[count] = phrase[i];
          count ++;
      }
  }
  // make the acronym string end with a null
  acronym[count] = '\0';
  printf("Your acronym is %s\n", acronym);
}
