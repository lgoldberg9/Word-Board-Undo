/***********************************************************************
 * Name:     Logan Goldberg
 * Homework: Word Board Undo
 ***********************************************************************/ 
/* *********************************************************************
 * Academic honesty certification:
 *   Written/online sources used:
 *     Professor Weinman's website,
 *     http://www.cs.grinnell.edu/~weinman/courses/CSC161/2015S/index.html
 *   Help obtained
 *     Took these procedures from previous lab.
 *     I worked with Henry Fisher and Dylan Murphy in this lab.
 *   My initials below confirms that the above list of sources
 *   is complete AND that I have not talked to anyone else
 *   (e.g., CSC 161 students) about the solution to this problem
 *
 *   Initials:  LWG
 ***********************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include "listStack.h"

int isEmpty (const stringStack* stack) {
/* Purpose: Checks to see if the stack is empty.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Returns 1 if empty, 0 else. */
  return ((*stack)->next == NULL);
}

void initialize (stringStack* stack) {
/* Purpose: Malloc space for a stackNode.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Stack is 'initialized'. */
  *stack = malloc(sizeof(node));
}

int push (stringStack* stack, struct word item) {
/* Purpose: Puts a struct word onto the stack.
   Preconditions: stack is a pointer to a stringStack.
                  item must be a valid struct.
   Postconditions: item is stored on the top of the stack. */
  stringStack newStackNode = malloc(sizeof(node));
  newStackNode->next = (*stack);
  newStackNode->boardWord = item;
  (*stack) = newStackNode;
  return 0;
}

struct word pop (stringStack* stack) {
/* Purpose: Extracts a struct word from the top of the stack.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Returns a struct word. */
  struct word undo_word = (*stack)->boardWord;
  stringStack nextStack = (*stack)->next;
  free(*stack); /* Avoid memory leak. */
  (*stack) = nextStack;
  return undo_word;
}

void free_stack(stringStack* stack) {
/* Purpose: Frees a linked-list stack.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Frees all the memory in the stack. */
  if (isEmpty(stack) == 1) {
    return;
  }
  while ((*stack)->next == NULL) {
    pop(stack); /* While loop steps through stack freeing each stackNode. */
  }
}
