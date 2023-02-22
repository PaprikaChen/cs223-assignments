/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/21
 * test the read_ppm_2d method
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
  if (pixels == NULL) {
    exit(1);
  }

  printf("Testing file %s : %d %d \n", "feep-raw.ppm", w, h);

  // print the grid
  for (int i = 0; i < h; i ++) {
    for (int j = 0; j < w; j ++) {
    printf("(%d, %d, %d)  ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }

  // free pixels
  for (int i = 0; i < h; i ++) {
    free(pixels[i]);
  }
  free(pixels);
  return 0;
}
