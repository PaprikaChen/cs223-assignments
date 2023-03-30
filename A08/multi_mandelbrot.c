/*----------------------------------------------
 * Author: Parika Chen
 * Date: 2023/3/30
 * compute a program that outputs a PPM image of the mandelbrot set 
 * using multiple processes spawned with fork().
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {

  struct timeval tstart, tend;
  
  int size = 2000;
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
  printf("Num processes = 4\n");
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // shared memory setting
  int shmidA;
  shmidA = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * size, 0644 | IPC_CREAT);
  if (shmidA == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel** image = shmat(shmidA, NULL, 0);
  if (image == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  int shmidC;
  shmidC = shmget(IPC_PRIVATE, sizeof(int) * size, 0644 | IPC_CREAT);
  if (shmidC == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  int* shmid = shmat(shmidC, NULL, 0);
  if (image == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }

  for (int i = 0; i < size; i ++) {
    shmid[i] = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size, 0644 | IPC_CREAT);
    if (shmid[i] == -1) {
      perror("Error: cannot initialize shared memory\n");
      exit(1);
    }
    
    image[i] = shmat(shmid[i], NULL, 0);
    if (image == (void*) -1) {
      perror("Error: cannot access shared memory\n");
      exit(1);
    }
  }
  
  // create a palette containing maxiteration number of colors
  srand(time(0));
  int shmidB;
  shmidB = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0644 | IPC_CREAT);
  if (shmidB == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* palette = shmat(shmidB, NULL, 0);
  if (palette == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }
  
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

  for (int k = 0; k < 4; k++) {
    int pid = fork();
    if (pid == 0) {
      // child work
      int cStart = 0;
      int cEnd = 0;
      int rStart = 0;
      int rEnd = 0;
      if (k == 0) {
        cEnd = size / 2;
        rEnd = size / 2;
      }
      else if (k == 1) {
        cStart = size / 2;
        cEnd = size;
        rEnd = size / 2;
      }
      else if (k == 2) {
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
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), cStart, cEnd, rStart, rEnd);
      
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
      exit(0);
    } else {
      printf("Launched child process: %d\n", pid);
    }
  }

  // wait and complete
  for (int i = 0; i < 4; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
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

  //clean up
  for (int i = 0; i < size; i ++) {
    if (shmdt(image[i]) == -1) {
      perror("Error: cannot detatch from shared memory\n");
      exit(1);
    }

    if (shmctl(shmid[i], IPC_RMID, 0) == -1) {
      perror("Error: cannot remove shared memory\n");
      exit(1);
    }
  }

  if (shmdt(shmid) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmidC, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }


  if (shmdt(palette) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmidB, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  if (shmdt(image) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmidA, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  return 0;
  
}