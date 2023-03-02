/*----------------------------------------------
 * Author: Paprika Chen
 * Date: 2023/2/28
 * reads in a single 64-bit unsigned integerand outputs it as an 8x8 1bpp sprite.
 ----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>


struct node{
  int sign;
  struct node* next;
};

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  // create a linked list to store, and turn to a new line for every 8 bits
  struct node* head = NULL;
  struct node* tail = NULL;

  // read and store each piece
  for (int i = 1; i < 65; i ++) {
    unsigned long bitMask = 0x1ul << (64 - i);
    unsigned long curBit = (bitMask & img);
    int curBitCheck = curBit >> (64 - i);
    struct node* curNode = malloc(sizeof(struct node));
    if (curNode == NULL) {
      printf("ERROR:Out of space!\n");
       exit(1);
    }
    curNode -> sign = curBitCheck;
    curNode -> next = NULL;
    if (head == NULL) {
      head = curNode;
      tail = curNode;
    }
    else {
      tail -> next = curNode;
      tail = curNode;
    }
  }

  //print the image
  int count = 1;
  struct node* printNode = head;
  while(printNode != NULL) {
    if (printNode -> sign == 1) {
      printf(" @");
    }
    else{
      printf("  ");
    }

    if (count == 8) {
      printf("\n");
      count = 0;
    }

    count ++;
    printNode = printNode -> next;
  }
  printf("\n");

  //clean the linked list
  struct node* clean;
  while (head != NULL) {
    clean = head;
    head = head -> next;
    free(clean);
  }

  return 0;
}
