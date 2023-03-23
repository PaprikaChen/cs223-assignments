/*----------------------------------------------
 * Author: Parika Chen
 * Date: 2023/3/22
 * This program outputs a PPM image of the mandelbrot set.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

int main(int argc, char* argv[]) {

  struct timeval tstart, tend;
  
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct ppm_pixel** image = malloc((size) * sizeof(struct ppm_pixel*));
  for (int i = 0; i < size; i ++) {
    image[i] = malloc((size) * sizeof(struct ppm_pixel));
  }
  
  // create a palette containing maxiteration number of colors
  srand(time(0));
  struct ppm_pixel* palette = malloc((maxIterations) * sizeof(struct ppm_pixel));
  
  for (int i = 0; i < maxIterations; i ++) {
    palette[i].red = rand() % 225;
    palette[i].green = rand() % 225;
    palette[i].blue = rand() % 225;
  }

  gettimeofday(&tstart, NULL);
  // calculate the color for each pixel and store
  for (int i = 0; i < size; i ++) {
    for (int j = 0; j < size; j ++) {
      float xfrac = i / (float)size;
      float yfrac = j / (float)size;
      float xA = xmin + xfrac * (xmax - xmin);
      float yA = ymin + yfrac * (ymax - ymin);
      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && ((x * x + y * y) < (2*2))) {
        float xtmp = x * x - y * y + xA;
        y = 2 * x * y + yA;
        x = xtmp;
        iter ++;
      }

      if (iter < maxIterations) { // escaped
        // color = palette[iter]
        image[j][i].blue = palette[iter].blue;
        image[j][i].green = palette[iter].green;
        image[j][i].red = palette[iter].red;
      }
      else {
        // color = black
        image[j][i].blue = 0;
        image[j][i].green = 0;
        image[j][i].red = 0;
      }
        //escaped
    }
  }

  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);
  //get the name of the outfile

  char fileName[128];
  snprintf(fileName, 128, "mandelbrot-%d-%ld.ppm", size, time(0));
  //print output file
  printf("Writing file: %s\n", fileName);

  // write the pixels into the output file
  write_ppm_2d(fileName, image, size, size);

  // free image and palette
  for (int i = 0; i < size; i ++) {
    free(image[i]);
  }
  free(image);

  free(palette);

  return 0;
  
}
