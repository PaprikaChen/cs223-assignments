/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2/28/2023
 * encode a secret message into a ppm file.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }
   
   // read in the file that need to be encoded
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  if (pixels == NULL) {
    exit(1);
  }


  printf("Reading %s with width %d and height %d\n", argv[1], w, h);

  // have to consider the '\0' at the end of the phrase, 
  // the first -1 is for algorithm, the second is for \0
  printf("Max number of characters that can be encoded: %d\n", (w * h * 3) / 8 - 1 - 1);
  printf("Enter a phrase: ");

  char phrase[128];
  scanf("%[^\n]%*c", phrase);
  if (strlen(phrase) > ((w * h * 3) / 8 - 1 - 1)) {
    printf("The phrase is too long!\n");
    exit(1);
  }

  // change the phrase into binary number representations
  int* phraseBinary = malloc((strlen(phrase)+1) * 8 * (sizeof (int)));

  // loop through each char
  unsigned int curChar = 0b00000000;
  for (int i = 0; i < (strlen(phrase)+1); i ++) {
    //phrase[i]
    for (int j = 0; j < 8; j ++) {
      unsigned int bitmask = 0b00000001;
      bitmask = bitmask << (7 - j);
      curChar = (bitmask & phrase[i]);
      curChar = curChar >> (7 - j);
      phraseBinary[j + i * 8] = curChar;
    }
  }
  


  int biCount = 0;
  // loop through the read-in ppm and change the value of each attribute
  // then store the new value into the 2d array
  int newH = ((((strlen(phrase)+ 1) * 8) / 3) + 1) / w + 1;
  for (int i = 0; i < newH; i ++) {
    for (int j = 0; j < w; j ++) {
      // the value for current red has to be odd
      if (phraseBinary[biCount] == 1) {
        // the value for current red is even
        if (pixels[i][j].red % 2 == 0) {
          pixels[i][j].red = pixels[i][j].red + 1;
        } 
      }
      // the value for current red has to be even
      else {
        // the value for current red is odd
        if (pixels[i][j].red % 2 == 1) {
          pixels[i][j].red = pixels[i][j].red - 1;
        }
      }
      biCount ++;
      if (biCount == ((strlen(phrase)+1) * 8)) {
        break;
      }

      // the value for current green has to be odd
      if (phraseBinary[biCount] == 1) {
        // the value for current green is even
        if (pixels[i][j].green % 2 == 0) {
          pixels[i][j].green = pixels[i][j].green + 1;
        } 
      }
      // the value for current green has to be even
      else {
        // the value for current green is odd
        if (pixels[i][j].green % 2 == 1) {
          pixels[i][j].green = pixels[i][j].green - 1;
        }
      }
      biCount ++;      
      if (biCount == ((strlen(phrase)+1) * 8)) {
        break;
      }

      // the value for current blue has to be odd
      if (phraseBinary[biCount] == 1) {
        // the value for current blue is even
        if (pixels[i][j].blue % 2 == 0) {
          pixels[i][j].blue = pixels[i][j].blue + 1;
        } 
      }
      // the value for current blue has to be even
      else {
        // the value for current blue is odd
        if (pixels[i][j].blue % 2 == 1) {
          pixels[i][j].blue = pixels[i][j].blue - 1;
        }
      }
      biCount ++;
      if (biCount == ((strlen(phrase)+1) * 8)) {
        break;
      }
    }
  }

  // get the file name to encode
  char encodeFile[128];
  int nameCount = 0;
  for (int i = 0; i < strlen(argv[1]); i ++) {
    if (argv[1][i] != '.') {
      encodeFile[i] = argv[1][i];
      nameCount ++;
    }
  }
  encodeFile[nameCount] = '\0';
  char suffix[] = "-encoded.ppm";
  strcat(encodeFile, suffix);

  //write the revised array to the new ppm file
  write_ppm_2d(encodeFile, pixels, w, h);
  printf("Writing file %s\n", encodeFile);

  // free the binary representation array
  free(phraseBinary);

  // free read-in 2d array
  for (int i = 0; i < h; i ++) {
  free(pixels[i]);
  }
  free(pixels);

  return 0;
}

