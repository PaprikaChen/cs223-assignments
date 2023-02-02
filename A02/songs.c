/*
 * songs.c
 * Author: Paprika Chen
 * Date: 2023/2/1
 * store the informations of songs and allow the user to change
 */

#include <stdio.h>
#include <string.h>

struct song {
  char title[32];
  char artist[32];
  int durationMin;
  int durationSec;
  float danceability;
};

// the struct array that contains all information of songs
struct song information[32];
// the integer that count the number of songs the array has stored
int songCount;

/**
 * store a song in the struct array
 * @param int id the index of the current storing songs in the song array
 * @param char titleS the string that contains the title of the song
 * @param char artistS the string that conains the name of the artists of the song
 * @param durationMinS the integer that stores the minutes of duration of the song
 * @param durationSecS the integer that stores the seconds of duration of the song
 * @param danceabilityS the float that stores the danceability of the song
 */
void store(int id, char titleS[32], char artistS[32], int durationMinS, int durationSecS, float danceabilityS) {
  strcpy(information[id].title, titleS);
  strcpy(information[id].artist, artistS);
  information[id].durationMin = durationMinS;
  information[id].durationSec = durationSecS;
  information[id].danceability = danceabilityS;
  songCount ++;
}


/**
 * print the information of all stored songs
 */
void print() {
  printf("\n");
  for (int i = 0; i < songCount; i ++) {
    struct song currentS = information[i];
    printf("%d) %-20s artist: %-20s duration: %d:%d danceability: %.2f\n", i, currentS.title, currentS.artist, currentS.durationMin, currentS.durationSec, currentS.danceability);
  }
  printf("\n=======================\n");
}

/**
 * interact with user to choose a certain attribute to change and store
 * @param int id the index of the editing song
 * @param char* the attribute that the user want to change
 */
int attributeChange(int id, char* inputAttribute) {
  char resultAttribute[32];

  if (!strcmp(inputAttribute, "title")) {
    strcpy(resultAttribute, "title");
    // ask for value that the user want to change
    printf("Enter %s: ", resultAttribute);
    // store the entered value into a string
    char changeTitle[32];
    scanf("%[^\n]%*c", changeTitle);
    // change the original value into the new one
    strcpy(information[id].title, changeTitle);
  }
  else if (!strcmp(inputAttribute, "artist")) {
    strcpy(resultAttribute, "artist");
    printf("Enter %s: ", resultAttribute);
    char changeArtist[32];
    scanf("%[^\n]%*c", changeArtist);
    strcpy(information[id].artist, changeArtist);
  }
  else if (!strcmp(inputAttribute, "duration")) {
    strcpy(resultAttribute, "duration");
    printf("Enter a duration (minutes): ");
    int changeMin;
    scanf("%d%*c", &changeMin);
    information[id].durationMin = changeMin;
    printf("Enter a duration (seconds): ");
    int changeSec;
    scanf("%d%*c", &changeSec);
    information[id].durationSec = changeSec;
  }
  else if (!strcmp(inputAttribute, "danceability")) {
    strcpy(resultAttribute, "danceability");
    printf("Enter %s: ", resultAttribute);
    float changeDance;
    scanf("%f%*c", &changeDance);
    information[id].danceability = changeDance;
  }
  // if an invalid attributed is entered
  else {
    printf("Invalid attribute!");
    strcpy(resultAttribute, "invalid");
    return 0;
  }

  return 1;
}

int main() {
  // initialize three songs
  store(0, "Shout", "Tears for Fears", 4, 11, 0.90);
  store(1, "As it was", "Harry Styles", 2, 47, 0.70);
  store(2, "Wish you were here", "Pink Floyd", 5, 34, 0.30);

  printf("Welcome to Steven Struct's Song List.\n");
  // print the information
  print();
  // ask which id the user want to change
  printf("\nEnter a song id to edit [");
  for (int i = 0; i < songCount - 1; i ++) {
      printf("%d,", i);
  }
  printf("%d]: ", songCount - 1);
  int editId;
  scanf("%d%*c", &editId);

  // if the id is out of range
  if (editId > songCount) {
      printf("Invalid choice!\n");
      return 0;
  }

  // interact with the attribute
  printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
  char editAttribute[32];
  scanf("%s%*c", editAttribute);
  int attributeResult = attributeChange(editId,editAttribute);

  // if the entered attricute is invalid
  if (!attributeResult) {
    return 0;
  }

  // print the information that is changed
  print();

  return 0;
}
