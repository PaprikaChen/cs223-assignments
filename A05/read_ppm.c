/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/21
 * read PPM files stored in binary format and return a 2D array of struct pixel.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  // open the file
  FILE *infile;
  infile = fopen(filename,"rb");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", filename);
  }

  // skip the head lines
  char type[32];
  fgets(type, 128, infile);
  char comments[128];
  fgets(comments, 128, infile);
  char size[128];
  fgets(size, 128, infile);
  char* token = strtok(size, " ");
  *w = atoi(token);
  if (token) {
    token = strtok(NULL, " ");
    *h = atoi(token);
  }
  char maxNum[128];
  fgets(maxNum, 128, infile);

  // create a 2d array using malloc
  struct ppm_pixel** image = malloc((*h) * sizeof(struct ppm_pixel*));
  for (int i = 0; i < *h; i ++) {
    image[i] = malloc((*w) * sizeof(struct ppm_pixel));
  }

  // read the file and store info into the 2d array
  for (int i = 0; i < *h; i ++) {
    fread(image[i], sizeof(struct ppm_pixel), *w, infile);
  }

  // check whether this is a image 
  for (int i = 0; i < *h; i ++) {
    for (int j = 0; j < *w; j ++) {
      if (image[i][j].red > 255 || image[i][j].blue > 255 || image[i][j].green > 255) {
        printf("The file does not represent an image. ");
        return NULL;
      }
    }
  }

  fclose(infile);
  return image;
}
