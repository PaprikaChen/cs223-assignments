/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2/28/2023
 * Read in a PPM file and print out the message hidden in the image.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <math.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  if (pixels == NULL) {
    exit(1);
  }
  
  // create an array to store the odd or even of each attribute of each pixel
  int* decode = malloc(w * h * 3 * (sizeof (int)));
  int codeCount = 0;
  unsigned int curNum[3];

  for (int i = 0; i < h; i ++) {
    for (int j = 0; j < w; j ++) {

      curNum[0] = pixels[i][j].red;
      curNum[1] = pixels[i][j].green;
      curNum[2] = pixels[i][j].blue;
      
      for (int k = 0; k < 3; k ++) {
        if (curNum[k] % 2 == 0) {
          decode[codeCount] = 0;
        }
        else decode[codeCount] = 1;
        codeCount++;
      }
    
    }
  }

  int circulation = 1;
  int curChar = 0b00000000;
  int charCount = 0;
  // put 8 ints together to decode one char
  for (int i = 0; i < codeCount; i ++) {

    int bitMask = decode[i] << (8 - circulation);
    curChar = (bitMask ^ curChar);

    if (circulation == 8) {
      if (curChar == '\0') {
        break;
      }
      printf("%c", curChar);
      curChar = 0;
      circulation = 0;
    }
    charCount ++;
    circulation ++;
  }
  
  printf("\n");

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", (charCount / 8));

  for (int i = 0; i < h; i ++) {
  free(pixels[i]);
  }
  free(pixels);

  free(decode);

  
  return 0;
}

