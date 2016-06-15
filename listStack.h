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
 *     [No additional]
 *     The reason that I am not citing my previous lab is because
 *     I overwrote the original stack procedures and had to rewrite them
 *     from scratch. The implementations are completely different, so I only
 *     intend to cite Professor Weinman for providing me with the correct 
 *     signatures.
 *   My initials below confirms that the above list of sources
 *   is complete AND that I have not talked to anyone else
 *   (e.g., CSC 161 students) about the solution to this problem
 *
 *   Initials:  LWG
 ***********************************************************************/ 
#ifndef __LISTSTACK_H__
#define __LISTSTACK_H__

typedef struct stackNode* stringStack;

struct word {
  char* str;
  char orientation;
  int posx;
  int posy;
  int length;
};

struct stackNode {
  struct word boardWord;
  struct stackNode* next;
} node;

int isEmpty (const stringStack* stack);
/* Purpose: Checks to see if the stack is empty.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Returns 1 if empty, 0 else. */

void initialize (stringStack* stack);
/* Purpose: Malloc space for a stackNode.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Stack is 'initialized'. */

void free_stack(stringStack* stack);
/* Purpose: Frees a linked-list stack.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Frees all the memory in the stack. */

struct word pop (stringStack* stack);
/* Purpose: Extracts a struct word from the top of the stack.
   Preconditions: stack is a pointer to a stringStack.
   Postconditions: Returns a struct word. */

int push (stringStack* stack, struct word item);
/* Purpose: Puts a struct word onto the stack.
   Preconditions: stack is a pointer to a stringStack.
                  item must be a valid struct.
   Postconditions: item is stored on the top of the stack. */


#endif
