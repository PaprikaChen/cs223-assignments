/*----------------------------------------------
 * Author: Parika Chen
 * Date: 2023/2/21
 * read a 2D array and write the content into the a PPM file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

  FILE* outfile;
  outfile = fopen("test.ppm","wb");

  fprintf(outfile, "P6\n");
  fprintf(outfile, "#write a ppm file\n");
  fprintf(outfile, "%d %d\n", w, h);
  fprintf(outfile, "%d\n", 225);

  for (int i = 0; i < h; i ++) {
    fwrite(pixels[i], sizeof(struct ppm_pixel), w, outfile);
  }

  fclose(outfile);

}

