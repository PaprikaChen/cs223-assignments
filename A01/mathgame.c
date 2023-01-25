/*
 * mathgame.c
 * Author: Paprika Chen
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

  // welcome message
  printf("%s\n", "Welcome to Math Game!"); 
  printf("%s\n", "How many rounds do you want to play?");

  // the number of rounds to play
  int totalRound = 0; 
  // read and store 
  scanf("%d", &totalRound); 

  // the number of correct rounds 
  int correctNum = 0; 

  // ensure that the random numbers are different every time
  time_t t; 
  srand((unsigned) time(&t));

  // the loop to ask each question
  for (int i = 0; i < totalRound; i++) { 

    // produce two random numbers between 1 to 9
    int randA = rand() % 9 + 1;
    int randB = rand() % 9 + 1;
    printf("%d + %d = ?\n", randA, randB);

    // the integer to store the sum of a and b
    int sum = randA + randB;

    // the integer to store the answer from the user
    int answer = 0;
    scanf("%d", &answer);

    // determine whether the user is right or wrong
    if (answer == sum) {
      correctNum ++;
      printf("Correct!\n");
    }
    else {
      printf("Incorrect :(\n");
    }
  }

  // completion message
  printf("You answered %d / %d correctly\n", correctNum, totalRound);

  return 0;
}
