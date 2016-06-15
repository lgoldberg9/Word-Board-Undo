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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "wordboard.h"
#include "listStack.h"


/* ##################### Utility Functions #####################  */

void menu(void) {
  /* Preconditions: Should only be called in 'main'.
     Postconditions: Prints the main menu of the program.
  */
  printf("\nMenu options:\n");
  printf("   i:  initialize the board\n");
  printf("   a:  add a new word to the board\n");
  printf("   u:  undo the previous word addition(s)\n");
  printf("   r:  redo the previous undo operation\n");
  printf("   q:  quit\n\n");
  printf("Please enter one of the above options: ");
}

/* ##################### Main ##################### */

int main(void) {

  wordboard gameboard;
  stringStack undoStack, redoStack;
  initialize(&undoStack); /* This is only necessary to prevent undo
                             and redo from seg faulting. */
  initialize(&redoStack);

  gameboard.active = 0; /* This variable controls the state of the board.
                           0 means there is no board. 
                           1 means that the board is initialized but no words
                           are currently on the board. 
                           2 means the first word has been placed. This is used 
                           to restrict the scope the next placed word.
                        */
  while(1337) {
    menu(); /* print menu options */
    switch(tolower(getchar())) {/* 'tolower()' makes all inputted chars 
                                    lower case.  */
    case 'i': /* Initializes new board. */
      letter_overflow(); /* Catches excess characters. */
      if (gameboard.active >= 1) {
        free(gameboard.board); /* Because the board is malloced, it's necessary 
                                  to free the previous board. */
        free_stack(&undoStack); /* Frees the entire undo stack. */
        free_stack(&redoStack); /* Frees the entire redo stack. */
        initialize(&undoStack);
        initialize(&redoStack);
        gameboard.boardRows = 0; /* This is to ensure that the rows and 
                                    columns are new. */
        gameboard.boardCols = 0;
      }
      board_initialization(&gameboard);
      break;
    case 'a': /* Adds new word to board. */
      letter_overflow(); /* Catches excess characters. */
      if (gameboard.active >= 1) {
        word_processor(&gameboard, &undoStack);
        if (!isEmpty(&redoStack)) {


          /* printf("Got here!\n"); Testing equipment. */


          free_stack(&redoStack); /* The redoStack should go away as outlined
                                     in the homework page. */
          initialize(&redoStack);


          /* printf("Next: %lu, Str stored: %s, empty: %d\n", 
             redoStack->next, redoStack->boardWord.str, isEmpty(&redoStack)); */


        }
      } else {
        printf("\nThere is no board to play on!\n");
      }
      break;
    case 'u': /* Undo previous operation. */


      /* printf("Result: %d\n", isEmpty(&undoStack)); Testing equipment. */


      letter_overflow(); /* Catches excess characters. */
      if (isEmpty(&undoStack) == 1) {/* Makes sure there are undo operations. */
        printf("\nThere are no operations to undo.\n");
      } else {
        undo_operation(&gameboard, &undoStack, &redoStack);
      }
      break;
    case 'r': /* Redo previous operation. */


      /* printf("Result: %d\n", isEmpty(&undoStack)); Testing equipment. */


      letter_overflow(); /* Catches excess characters. */
      if (isEmpty(&redoStack) == 1) {/* Makes sure there are redo operations. */
        printf("\nThere are no operations to redo.\n");
      } else {
        redo_operation(&gameboard, &undoStack, &redoStack);
      }
      break;
    case 'q': /* Quit program. */
      putchar('\n');
      return 0;
      break;
    default: /* Error catching. */
      printf("\n\nPlease enter a valid option.\n\n");
      break;
    }
  }
}
