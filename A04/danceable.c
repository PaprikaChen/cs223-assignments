/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/12
 * allows users to easily access the most danceable songs in songlist.csv using linked list structure.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct song {
  char title[32];
  char artist[32];
  int durationMin;
  char durationSec[32];
  float danceability;
  float energy;
  float tempo;
  float valence;

};

struct node{
  struct song information;
  struct node* next;
};


/**
 * store the information of one song into a struct song
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
 * @param struct node* head the head of the link list containing informations of songs
 */
void print(struct node* head) {
  if (head == NULL) {
    printf("\nDataset contains 0 songs");
    printf("\n=======================\n");
    return ;
  }
  int count = 0;
  for (struct node* n = head; n !=NULL; n = n->next){
    struct song currentS = n->information;
    printf("%-2d) %-23s artist: %-20s  ", count, currentS.title, currentS.artist);
    printf("duration: %d:%s  ", currentS.durationMin, currentS.durationSec);
    printf("D: %.3f  E: %.3f  ", currentS.danceability, currentS.energy);
    printf("T: %.3f  V: %.3f \n", currentS.tempo, currentS.valence);
    count ++;
  }

  // print the count of the current songset
  printf("\nDataset contains %d songs", count);
  printf("\n==========================\n");

}

/**
 * find the most danceable song in the link list, and then delete it
 * @param struct node* head the head of the link list containing informations of songs
 * @return struct node* head return the new head of the link list
 */
struct node* mostDanceable(struct node* head) {
  // print the empty list if head is null
  if (head == NULL) {
    print(head);
    return head;
  }

  // keep track of the highest danceability while looping
  float highest = 0;
  // keep track of the node pointer of song containing the highest danceability 
  struct node* hiNode = head;
  // create a node which is before the head in the linked list
  struct node* before = NULL;
  before = malloc(sizeof(struct node));
  if (before == NULL) {
    printf("Error!");
    exit(1);
  }
  before -> next = head;
  // keep track of the node pointer of song before the current one
  struct node* beforeSign = before;
  // keep track of the node pointer of song before the song containing the highest danceablility
  struct node* hiBefore = before;
  for (struct node* m = head; m !=NULL; m = m->next){
    struct song currentS = m->information;
    if (currentS.danceability > highest) {
      highest = currentS.danceability;
      hiNode = m;
      hiBefore = beforeSign;
    }
    // update the before node pointer
    beforeSign = beforeSign -> next;
  }
  struct song hiSong = hiNode->information;
  printf("-------------------------------------Most Danceable---------------------------------\n");
  printf("    %-23s artist: %-20s  ", hiSong.title, hiSong.artist);
  printf("duration: %d:%s  ", hiSong.durationMin, hiSong.durationSec);
  printf("D: %.3f  E: %.3f  ", hiSong.danceability, hiSong.energy);
  printf("T: %.3f  V: %.3f \n", hiSong.tempo, hiSong.valence);
  printf("------------------------------------------------------------------------------------\n");
  // if the song containing the highest danceability is the head of the linked list
  if (hiNode == head) {
    // update the head of the linked list
    head = head -> next;
  }
  else {
    // delete the highest song node from the linked list
    hiBefore -> next = hiNode -> next;
  }
  
  // call the print method again
  print(head);
  free(before);
  free(hiNode);

  // return the updated head
  return head;
}

/**
 * add a new song to the linked list
 * @param struct song the song struct that needs to be added into the linked list
 * @param struct node* the head of the linked list
 * @return struct node* head return the updated head of the link list
 */
struct node* addSong(struct song add, struct node* head) {
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR:Out of space!\n");
    exit(1);
  }
  n ->information = add;
  n ->next = head;
  return n;

}

/**
 * free the current linked list
 * @param struct node* the head of the linked list
 */
void clear(struct node* head) {
  struct node* point;
  // clean the whole list
  while (head != NULL) {
    point = head;
    head = head -> next;
    free(point);
  }
}

int main() {
  
  FILE* infile; 
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
      printf("Error: unable to open file %s\n", "songlist.csv");
      exit(1);
  }

  // skip the first line
  char skip[128];
  fgets(skip, 128, infile);

  // create a struct song array to store all informations
  struct node* head = NULL;

  // read line by line and store informations into struct song array
  char currentString[128];
  int index = 0;
  while (fgets(currentString, 128, infile)) {
    head = addSong(store(currentString), head);
    index ++;
  }
  fclose(infile);
  print(head);

  char delete[32];
  printf("Press 'd' to show the most danceable song (any other key to quit):");
  scanf("%s", delete); 

  while (!strcmp(delete, "d")) {
    head = mostDanceable(head);
    printf("Press 'd' to show the most danceable song (any other key to quit):");
    scanf("%s", delete); 
  }

  // free the whole linked list after use
  clear(head);
  return 0;
}

