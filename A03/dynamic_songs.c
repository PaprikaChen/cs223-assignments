/*
 * dynamic_songs.c
 * This program reads songs from a text file into an array
 * Author: Paprika Chen
 * Date: 2023/2/9
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song {
  char title[128];
  char artist[128];
  int durationMin;
  char durationSec[32];
  float danceability;
  float energy;
  float tempo;
  float valence;
};

/**
 * store the information of one song into a struct song
 * @param int index the index of the struct song in the song array
 * @param char the whole line containing all information of a song that need to be stored
 * @return struct song return a struct song containing all informations
 */
struct song store(char information[128]) {
  struct song currentSong;
  // split the string by sign
  char* token = strtok (information, ",");
  strcpy(currentSong.title, token);

  if (token) {
    token = strtok(NULL, ",");
    strcpy(currentSong.artist, token);
  }
  if (token) {
    token = strtok(NULL, ",");
    int mill = atoi(token);

    // convert milliseconds into mins and seconds
    int min = mill / 60000;
    int sec = (mill - min*60000) / 1000;
    currentSong.durationMin = min;
    char secondsB[3];
    sprintf(secondsB,"%d", sec);
    if (sec < 10) {
      char seconds[3];
      seconds[0] = '0';
      seconds[1] = secondsB[0];
      seconds[2] = '\0';
      strcpy(currentSong.durationSec, seconds);

    }
    else {

      strcpy(currentSong.durationSec, secondsB);
    }
  }
  if (token) {
    token = strtok(NULL, ",");
    currentSong.danceability = atof(token);
  }
  if (token) {
    token = strtok(NULL, ",");
    currentSong.energy = atof(token);
  }
  if (token) {
    token = strtok(NULL, ",");
    currentSong.tempo = atof(token);
  }
  if (token) {
    token = strtok(NULL, ",");
    currentSong.valence = atof(token);
  }
  return currentSong;
}

/**
 * print the information of all stored songs
 */
void print(struct song* array, int count) {
  printf("Welcome to Dynamic Donna's Danceability Directory.\n\n");
  for (int i = 0; i < count; i ++) {
    struct song currentS = array[i];
    printf("%-2d) %-23s artist: %-20s  duration: %d:%s  D: %.3f  E: %.3f  T: %.3f  V: %.3f \n", i, currentS.title, currentS.artist, currentS.durationMin, currentS.durationSec, currentS.danceability, currentS.energy, currentS.tempo, currentS.valence);
  }
  printf("\n=======================\n");
}

int main() {

  FILE* infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
      printf("Error: unable to open file %s\n", "songlist.csv");
      exit(1);
  }
  // the string to read and store the first string of the file
  char count[32];
  fgets(count, 128, infile);
  int valid = 0;
  while (count[valid] != ',') {
    valid ++;
  }
  count[valid] = '\0';
  int amount = 0;
  amount = atoi(count); // atoi convert string into integer

  // skip the second line
  char skip[128];
  fgets(skip, 128, infile);

  // create a struct song array to store all informations
  struct song* songArray;
  songArray = malloc(sizeof(struct song)*amount);

  // read line by line and store informations into struct song array
  int index = 0;
  char currentString[128];
  while (fgets(currentString, 128, infile)) {
    songArray[index] = store(currentString);
    index ++;
  }

  fclose(infile);
  print(songArray, amount);
  free(songArray);

  return 0;

}
