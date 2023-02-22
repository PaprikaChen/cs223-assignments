/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/21
 * test the method write_ppm
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {

  // get the size, read the binary file into a 2d array
  int wA, hA;
  struct ppm_pixel** pixelsA = read_ppm_2d("feep-raw.ppm", &wA, &hA);
  if (pixelsA == NULL) {
    exit(1);
  }

  // write the content of the 2d array into a binary file
  write_ppm_2d("test.ppm", pixelsA, wA, hA);

  // read the written file into a 2d array again
  int wB, hB;
  struct ppm_pixel** pixelsB = read_ppm_2d("test.ppm", &wB, &hB);
  if (pixelsB == NULL) {
    exit(1);
  }

  
  printf("Testing file %s : %d %d \n", "test.ppm", wB, hB);

  // print the grid
  for (int i = 0; i < hB; i ++) {
    for (int j = 0; j < wB; j ++) {
    printf("(%d, %d, %d)  ", pixelsB[i][j].red, pixelsB[i][j].green, pixelsB[i][j].blue);
    }
    printf("\n");
  }
  
  // free pixels
  for (int i = 0; i < hA; i ++) {
    free(pixelsA[i]);
  }
  free(pixelsA);

  for (int i = 0; i < hB; i ++) {
  free(pixelsB[i]);
  }
  free(pixelsB);
  return 0;
}
