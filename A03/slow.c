/*
 * slow.c
 * This program turns a phrase into ent-speech
 * Author: Paprika Chen
 * Date: 2023/2/6
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // ask for pause length and store
  printf("Pause length: ");
  int length;
  scanf(" %d%*c", &length);

  // ask for text content and store
  printf("Text: ");
  char phrase[32];
  scanf("%s", phrase);

  char* result = NULL;
  // store the length of the input text
  int textLength = strlen(phrase);
  int size = length*textLength + textLength + 1;
  result = malloc(sizeof(char)*size);
  int count = 0;
  for (int i = 0; i < textLength; i ++) {
    // store the text character followed by the pausing
    result[count] = phrase[i];
    count ++;
    for (int j = 0; j < length; j ++) {
      result[count] = '.';
      count ++;
    }
  }
  result[count] = '\0';
  printf("%s\n", result);

  // free the malloc after use
  free(result);

  return 0;

}
