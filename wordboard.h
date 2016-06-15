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
 *   My initials below confirms that the above list of sources
 *   is complete AND that I have not talked to anyone else
 *   (e.g., CSC 161 students) about the solution to this problem
 *
 *   Initials:  LWG
 ***********************************************************************/ 
#ifndef __WORDBOARD_H__
#define __WORDBOARD_H__

#include "listStack.h"

typedef struct newboard wordboard;

struct newboard {
  int boardRows;
  int boardCols;
  char** board;
  int active;
};

void letter_overflow(void);
/* Credits: Imported from previous homework.
   Purpose: To prevent continuous calls to while loops.
   Preconditions: [No additional]
   Postconiditions: Collects all the garbage characters until '\n'.
*/

void print_board(wordboard gameboard);
/* Purpose: Modularity.
   Preconditions: 'wordboard' must be initialized.
   Postconditions: Prints out 'wordboard'.
*/

void word_swap(wordboard* gameboard, struct word changeWord, char* newWord);
/* Purpose: Modularity.
   Preconditions: 'wordboard' must be initialized.
                   struct 'changeWord' should be nonempty.
                   newWord needs to be a pointer to a string.
   Postconditions: Rewrites the 'gameboard' with the string literal stored
   in 'changeWord', and then stores the overwritten tiles in 'newWord'.
*/

void board_initialization(wordboard* gameboard);
/* Purpose: Creates a new 'board'.
   Preconditions: [No additional]
   Postconditions: New board has no letters stored on it.
*/

void word_processor(wordboard* gameboard, stringStack* undoStack);
/* Purpose: Function to add word to board.
   Preconditions: 'wordboard' must be initialized.
   Postconditions: 'wordboard' has a word appended to it.
                   Clears the redoStack.
*/

void undo_operation(wordboard* gameboard, stringStack* undoStack, 
                    stringStack* redoStack);
/* Purpose: Undo the previous words.
   Preconditions: A operation must have already been performed.
   Postconditions: Undo the previous operation.
*/

void redo_operation(wordboard* gameboard, stringStack* undoStack,
                    stringStack* redoStack);
/* Purpose: Redo an undo operation.
   Preconditions: An undo operation must have been completed previously.
   Postconditions: Redo the previous operation.
*/

#endif
