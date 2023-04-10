/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/4/5
 * Implement a multi-threaded version of the mandelbrot set that compute a program that outputs a PPM image.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct threadInfo {
  struct ppm_pixel** image;
  struct ppm_pixel* palette;
  int size;
  int threadId;
  int max;
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};

void *thread_generate(void *args) {
  struct threadInfo* info = (struct threadInfo*) args;
  int id = info->threadId;
  struct ppm_pixel** image = info->image;
  struct ppm_pixel* palette = info->palette;
  int size = info->size;
  int maxIterations = info->max;
  int xmin = info->xmin;
  int xmax = info->xmax;
  int ymin = info->ymin;
  int ymax = info->ymax;
  int cStart = 0;
  int cEnd = 0;
  int rStart = 0;
  int rEnd = 0;
  if (id == 0) {
    cEnd = size / 2;
    rEnd = size / 2;
  }
  else if (id == 1) {
    cStart = size / 2;
    cEnd = size;
    rEnd = size / 2;
  }
  else if (id == 2) {
    cEnd = size / 2;
    rStart = size / 2;
    rEnd = size;
  }
  else {
    cStart = size / 2;
    cEnd = size;
    rStart = size / 2;
    rEnd = size;    
  }
  printf("Thread %lu) ", pthread_self());
  printf("Sub-image block: cols (%d, %d) to rows (%d, %d)\n", cStart, cEnd, rStart, rEnd);

  for (int i = rStart; i < rEnd; i ++) {
    for (int j = cStart; j < cEnd; j ++) {
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
  printf("Thread %lu) finished\n", pthread_self());
  return (void*) 0;
}

int main(int argc, char* argv[]) {

  struct timeval tstart, tend;

  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
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

  pthread_t* threads = malloc(4 * sizeof(pthread_t));
  struct threadInfo threadStruct[4];

  for (int i = 0; i < 4; i ++) {
    threadStruct[i].threadId = i;
    threadStruct[i].image = image;
    threadStruct[i].size = size;
    threadStruct[i].max = maxIterations;
    threadStruct[i].palette = palette;
    threadStruct[i].xmin = xmin;
    threadStruct[i].xmax = xmax;
    threadStruct[i].ymin = ymin;
    threadStruct[i].ymax = ymax;
  }

  for (int i = 0; i < 4; i ++) {
    pthread_create(&threads[i], NULL, thread_generate, &threadStruct[i]);
  }

  for (int i = 0; i < 4; i ++) {
    pthread_join(threads[i], NULL);
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
  free(threads);
}
