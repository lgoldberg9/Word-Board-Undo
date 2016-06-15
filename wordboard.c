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
#include <string.h>
#include "wordboard.h"
#include "listStack.h"

void letter_overflow(void) {
  /* Purpose: To prevent continuous calls to while loops.
     Preconditions: [No additional]
     Postconiditions: Collects all the garbage characters until '\n'.
  */
  while (getchar() != '\n'); /* Collects overflow characters. */
}

void print_board(wordboard gameboard) {
  /* Purpose: Modularity.
     Preconditions: 'wordboard' must be initialized.
     Postconditions: Prints out 'wordboard'.
     Credits: Two-dimensional array lab.
  */
  char tile;
  int row, col;

  for (row = 0; row < (gameboard.boardCols * 2 - 5)/2 - 1; row++) {
    printf("=");
  }
  printf(" Word Board");
  for (row = 0; row < (gameboard.boardCols * 2 - 5)/2 - 1; row++) {
    printf("=");
  }
  putchar('\n');

  for (row = 0; row < gameboard.boardCols * 2 + 3; row++) {
    printf("-"); /* Prints enough dashes to cover top of board. */
  }
  putchar('\n');

  /* I ripped this from the print statement in the two-dimensional
     array lab. It was written in row,col format, and I'm too afraid to touch
     it at this point.*/

  for (row = 0; row < gameboard.boardRows; row++) {
    for (col = 0; col < gameboard.boardCols; col++) {
      if (col == 0) {
        printf("|");/* Prints border around rows. */
      } 
      tile = gameboard.board[row][col];
      if (tile == 0) {
        printf ("  "); /* Empty space for null characters. */
      } else {
        printf (" %c", tile); /* Reads characters from tile and prints. */
      }
    }
    printf (" |\n"); /* Closes off right side of board. */
  }
  for (int row = 0; row < gameboard.boardCols * 2 + 3; row++) {
    printf("-"); /* Prints enough dashes to cover botom of board. */
  }
  printf("\n");
}

void word_swap(wordboard* gameboard, struct word changeWord, char* newWord) {
  /* Purpose: Modularity.
     Preconditions: 'wordboard' must be initialized.
     struct 'changeWord' should be nonempty.
     newWord needs to be a pointer to a string.
     Postconditions: Rewrites the 'gameboard' with the string literal stored
     in 'changeWord', and then stores the overwritten tiles in 'newWord'.
  */

  int wordLen = changeWord.length;

  if (changeWord.orientation == 'v') {
    if (wordLen+changeWord.posy <= gameboard->boardRows) {
      for (int letter = 0; letter < wordLen; letter++) {
        newWord[letter] = 
          gameboard->board[changeWord.posy+letter][changeWord.posx];
        if (changeWord.str[letter] == 0) {
          gameboard->board[changeWord.posy+letter][changeWord.posx] = 0;
        } else {
          gameboard->board[changeWord.posy+letter][changeWord.posx] =
            changeWord.str[letter];
        }
      }
    } else {
      printf("\nWord goes out-of-bounds. Try a different tile.\n");
    }
  } else {
    if (wordLen+changeWord.posx <= gameboard->boardCols) {
      for (int letter = 0; letter < wordLen; letter++) {
        newWord[letter] = 
          gameboard->board[changeWord.posy][changeWord.posx+letter];
        if (changeWord.str[letter] == 0) {
          gameboard->board[changeWord.posy][changeWord.posx+letter] = 0;
        } else {
          gameboard->board[changeWord.posy][changeWord.posx+letter] =
            changeWord.str[letter];
        }
      }
    } else {
      printf("\nWord goes out-of-bounds. Try a different tile.\n");
    }
  }
}

void board_initialization(wordboard* gameboard) {
  /* Purpose: Creates a new 'board'.
     Preconditions: [No additional]
     Postconditions: New board has no letters stored on it.
  */
  
  /* Get the number of rows user will use. */
  while (1337) {
    printf("Please enter the number of rows you would like to play with ");
    printf("(minimum board size is 5, max 40): ");
    scanf("%d", &(*gameboard).boardRows); 
    letter_overflow(); /* Prevents excess junk. */
    if (gameboard->boardRows >= 5 && gameboard->boardRows <= 40) {
      break;
    } else {
      printf("Please enter a valid board size.\n");
    }
  }
  /* Get the number of columns user will use. */
  while (1337) {
    printf("Please enter the number of columns you would like to play with: ");
    printf("(minimum board size is 5, max 40): ");
    scanf("%d", &(*gameboard).boardCols); 
    letter_overflow(); /* Prevents excess junk. */
    if (gameboard->boardCols >= 5 && gameboard->boardCols <= 40) {
      break;
    } else {
      printf("Please enter a valid board size.\n");
    }
  }

  gameboard->board = (char**)malloc(gameboard->boardRows * sizeof(char*));
  /* Mallocs gameboard->boardRows * char* worth of space.
     Creates 'rows' in a transient state to be overwritten later. */ 

  int rows, cols; /* Used as index for iteration 
               over the char* in wordboard. */

  for (rows = 0; rows < gameboard->boardRows; rows++) {
    /* For loop iterates over each row and mallocs BoardColumns * char
       worth of space per row. This creates the entire board as a char
       array, dynamically allocated. */
    gameboard->board[rows] = 
      (char*)malloc(gameboard->boardCols * sizeof(char));
  }

  if (gameboard->active >= 1) {
    for (cols = 0; cols < gameboard->boardCols; cols++) {
      for (rows = 0; rows < gameboard->boardRows; rows++) {
        gameboard->board[rows][cols] = 0;
      }
    }
  }
  /* Eliminates junk that might end up on the gameboard after mallocing. */

  putchar('\n'); /* Atheistic. */

  print_board(*gameboard);

  printf("\nNew board created!\n");

  gameboard->active = 1; /* Actives adding words. */

  return;
}

void word_processor(wordboard* gameboard, stringStack* undoStack) {
  /* Purpose: Function to add word to board.
     Preconditions: 'wordboard' must be initialized.
     Postconditions: 'wordboard' has a word appended to it.
  */

  /* -----------------Procedure Setup----------------- */
  /* Sets the max word length depending on the dimensions of the board. */
  int maxWordLen, letter = 0;

  /* Initializes the struct word 'oldWord' for 'undoStack' and 'gameboard'. */
  struct word oldWord;

  /* While loop ensures correct 'orientation' option is inputted. */
  while (1337) {
    printf("Please select an orientation for this word ('v' for vertical,");
    printf(" 'h' for horizontal): ");
    oldWord.orientation = tolower(getchar());
    letter_overflow();

    if (oldWord.orientation == 'v' || oldWord.orientation == 'h') {
      if (oldWord.orientation == 'v') {
        maxWordLen = gameboard->boardRows;
      } else {
        maxWordLen = gameboard->boardCols;
      }
      break;
    } else {
      printf("Invalid orientation.\n");
    }
  }

  oldWord.str = malloc(maxWordLen * sizeof(char)); 
  /* mallocs the word stored in the struct 'oldWord'. */
  char* newWord = malloc(maxWordLen * sizeof(char));
  /* mallocs the word stored in 'newWord'. */

  char nextLetter;

  printf("Please enter the word you would like to add: ");

  /* This while loop ensures that the correct characters are read onto
     the string 'newWord'. */

  while (letter < maxWordLen) {
    nextLetter = getchar();

    //printf("nextLetter: %c\n", nextLetter);

    if ((nextLetter >= 'A' && nextLetter <= 'Z')
        || (nextLetter >= 'a' && nextLetter <= 'z')) {
      newWord[letter] = toupper(nextLetter);
      letter++;
    } else if (nextLetter == '\n' && letter != 0) {
      break;
    } else {
      letter_overflow();
      letter = 0;
      printf("\nInvalid characters. Please type your word again: ");
    }
  }

  /* Determines cursor starting location. */
  int wordLen = strlen(newWord);
  
  /* printf("isEmpty: %d, wordLenx: %d, wordLeny: %d\n", isEmpty(undoStack),  */
  /*        wordLen <= gameboard->boardRows,  */
  /*        wordLen <= gameboard->boardCols);  
            This tells me how the cursor will initialize. */

  if ((isEmpty(undoStack) == 1) || !(wordLen <= gameboard->boardRows)
      || !(wordLen <= gameboard->boardCols)) {
    oldWord.posx = 0;
    oldWord.posy = 0;
    printf("Initializing cursor at (0,0).\n");
  } else {
    oldWord.posx = (*undoStack)->boardWord.posx;
    oldWord.posy = (*undoStack)->boardWord.posy;
    printf("Initializing cursor at (%d,%d).\n", oldWord.posx, oldWord.posy);
  }

  char temp = (*gameboard).board[oldWord.posy][oldWord.posx];

  /* -----------------Local functions----------------- */
  void word_placement_menu(void) {
    /* Purpose: Prints out UI for user. */
    printf("\n---Word Placement Navigation Pane---\n");
    printf(" a:  Move one column left.\n");
    printf(" w:  Move one row up.\n");
    printf(" s:  Move one row down.\n");
    printf(" d:  Move one column right.\n");
    printf(" j:  Move to any tile.\n");
    printf(" p:  Print the cursor location.\n");
    printf(" #:  Add word to position: (%d,%d)\n", oldWord.posx, 
           oldWord.posy);
    printf(" q:  Return to the main menu.\n");
    printf("Please choose an option: ");
  }

  void cursorBoard_print(void) {
    /* Purpose: Modular function for printing the board with
       the cursor location.
       Preconditions: Should be called within or after the switch statement
       below.
       Postconditions: Prints the gameboard's board with the cursor location.
    */
    temp = gameboard->board[oldWord.posy][oldWord.posx];
    gameboard->board[oldWord.posy][oldWord.posx] = 35;
    /* '35' is #, so it should be distinguishable from other chars
       to see when the board is printed with the cursor. */
    print_board(*gameboard);
    gameboard->board[oldWord.posy][oldWord.posx] = temp;
  }

  /* -----------------Switch Statement----------------- */

  int jumpRow, jumpCol; /* Used in the 'j' proc. */
  int mismatchedLetter = 0; /* Prevents overwriting chars (e.g, change E
                               to A). */
  int NULLCounter = 0; /* Counts the NULL chars to prevent words from being
                          placed in empty spaces on the board. */

  while (1337) {
    word_placement_menu();
    switch(tolower(getchar())) {
    case 'w':
      /* code for arrow up */
      if (oldWord.posy == 0) {
        /* Check to see if the cursor is out-of-bounds. */
        printf("\nCursor out-of-bounds.\n");
      } else {
        oldWord.posy -= 1;
      }
      break;
    case 's':
      /* code for arrow down */
      if (oldWord.posy == gameboard->boardRows - 1) {
        /* Check to see if the cursor is out-of-bounds. */
        printf("\nCursor out-of-bounds.\n");
      } else {
        oldWord.posy += 1;
      }
      break;
    case 'd':
      /* code for arrow right */
      if (oldWord.posx == gameboard->boardCols - 1) {
        /* Check to see if the cursor is out-of-bounds. */
        printf("\nCursor out-of-bounds.\n");
      } else {
        oldWord.posx += 1;
      }
      break;
    case 'a':
      /* code for arrow left */
      if (oldWord.posx == 0) {
        /* Check to see if the cursor is out-of-bounds. */
        printf("\nCursor out-of-bounds.\n");
      } else {
        oldWord.posx -= 1;
      }
      break;
    case 'j': 
      letter_overflow();
      /* code for jumping to any tile on the board. */
      printf("Please enter which tile you want to move to (col,row): ");
      scanf("%d,%d", &jumpCol, &jumpRow); 
      /* Input format is "col,row" or (x,y). */

      /* Check to see if the cursor is out-of-bounds. */
      if ((jumpCol >= 0) && (jumpCol < gameboard->boardCols)) {
        if ((jumpRow >= 0) && (jumpRow < gameboard->boardRows)) {
          /* If conditions satisfies, then move cursor to col,row. */
          oldWord.posx = jumpCol;
          oldWord.posy = jumpRow;
          printf("\nMoved to tile (%d,%d).\n", jumpCol, jumpRow);
          break;
        }
      }
      printf("\nCursor out-of-bounds.\n");
      break;
    case 'p':
      /* Prints out the board with the cursor location marked by '#'. */
      cursorBoard_print();
      break;
    case '#': 
      /* This operation inputs the word onto the board. There are a lot of
         restrictions that come with word placement, however, so a large
         control structure is necessary to manage the restrictions. 
         After that, the word is read onto the board, and the space overwritten
         by the word is saved to the 'undoStack'. */

      printf("\nAdding word to gameboard...\n\n");
      /* The control structure here is just awful.  I'm sorry about this. */
      if (oldWord.orientation == 'v') {/* First check orientation of word. */
        if (wordLen+oldWord.posy <= gameboard->boardRows) {
          /* Makes sure the word fits. */
          if (gameboard->active == 2) {
            /* See if placement restrictions apply. */
            for (letter = 0; letter < wordLen; letter++) {
              /* This first part steps through each possible tile that is 
                 intended to be overwritten, and checks to see if the word
                 satisfies the restrictions. */

              /* printf("What goes here: %d, Letter Comp %d\n", 
                 gameboard->board[oldWord.posy+letter][oldWord.posx] == 0, 
                 newWord[letter] == */
              /*        gameboard->board[oldWord.posy+letter][oldWord.posx]); 
                        Testing equipment. Tells me whether space is NULL
                        or if characters match. */


              if (newWord[letter] ==
                  gameboard->board[oldWord.posy+letter][oldWord.posx]) {
                continue; /* This tile is okay. */
              } else if 
                  (gameboard->board[oldWord.posy+letter][oldWord.posx] == 0) {
                NULLCounter++; /* This tile is empty. */
              } else {
                mismatchedLetter++; /* This tile and the word do not share
                                       a letter in common. */
              }
            }


            /*    printf("nullcounter: %d, strlen: %d\n",  */
            /* NULLCounter,(int)strlen(newWord)); testing equipment. 
               Used this to determine null termination of strs. */

            /* If all the tiles are matched correctly, and not all the tiles
               are NULL, then word is written to board. */
            if (mismatchedLetter == 0 && NULLCounter != wordLen) {
              for (letter = 0; letter < wordLen; letter++) {
                oldWord.str[letter] = 
                  gameboard->board[oldWord.posy+letter][oldWord.posx];
                gameboard->board[oldWord.posy+letter][oldWord.posx] =
                  newWord[letter];
              }
            } else {
              NULLCounter = 0; /* Reset counter for next loop. */
              mismatchedLetter = 0; /* Reset counter for next loop. */
              printf("This word cannot go here!\n");
              break;
            }
          } else {/* Just writes word without restrictions, so the word
                     can go anywhere on the board. */
            for (letter = 0; letter < wordLen; letter++) {
              oldWord.str[letter] = 
                gameboard->board[oldWord.posy+letter][oldWord.posx];
              gameboard->board[oldWord.posy+letter][oldWord.posx] =
                newWord[letter];
            } 
          }
        } else {
          printf("\nWord goes out-of-bounds. Try a different tile.\n");
          break;
        }
      } else {
        if (wordLen+oldWord.posx <= gameboard->boardCols) {
          /* Makes sure the word fits. */
          if (gameboard->active == 2) {
            /* See if placement restrictions apply. */
            for (letter = 0; letter < wordLen; letter++) {
              /* This first part steps through each possible tile that is 
                 intended to be overwritten, and checks to see if the word
                 satisfies the restrictions. */


              /* printf("What goes here: %d, Letter Comp %d\n", 
                 gameboard->board[oldWord.posy][oldWord.posx+letter] == 0, 
                 newWord[letter] ==
                 gameboard->board[oldWord.posy][oldWord.posx+letter]);
                 Testing equipment. Tells me whether space is NULL
                 or if characters match. */


              if (newWord[letter] ==
                  gameboard->board[oldWord.posy][oldWord.posx+letter]) {
                continue; /* This tile is okay. */
              }  else if 
                  (gameboard->board[oldWord.posy][oldWord.posx+letter] == 0) {
                NULLCounter++; /* This tile is empty. */
              } else {
                mismatchedLetter++; /* This tile and the word do not share
                                       a letter in common. */
              }
            }


            /* printf("nullcounter: %d, strlen: %d\n", 
               NULLCounter,(int)strlen(newWord)); testing equipment. 
               Used this to determine null termination of strs. */

            /* If all the tiles are matched correctly, and not all the tiles
               are NULL, then word is written to board. */
            if (mismatchedLetter == 0 && NULLCounter != wordLen) {
              for (letter = 0; letter < wordLen; letter++) {
                oldWord.str[letter] = 
                  gameboard->board[oldWord.posy][oldWord.posx+letter];
                gameboard->board[oldWord.posy][oldWord.posx+letter] =
                  newWord[letter];
              }
            } else {
              NULLCounter = 0; /* Reset counter for next loop. */
              mismatchedLetter = 0; /* Reset counter for next loop. */
              printf("This word cannot go here!\n");
              break;
            }
          } else {/* Just writes word without restrictions, so the word
                     can go anywhere on the board. */
            for (letter = 0; letter < wordLen; letter++) {
              oldWord.str[letter] = 
                gameboard->board[oldWord.posy][oldWord.posx+letter];
              gameboard->board[oldWord.posy][oldWord.posx+letter] =
                newWord[letter];
            } 
          }
        } else {
          printf("\nWord goes out-of-bounds. Try a different tile.\n");
          break;
        }
      }
      print_board(*gameboard);
      /* Loads 'undoStack'. */
      if (gameboard->active == 1) {
        /* active = 1, and active = 2 are detailed in the main function. */
        gameboard->active = 2;
      }
      oldWord.length = wordLen; /* Pushes wordLen onto the struct. */
      push(undoStack, oldWord); /* Writes undo operation onto 'undoStack'. */
      free(newWord); /* Free unnecessary space. */
      printf("\nReturning to main menu...\n");
      letter_overflow();
      return;
      break;
    case 'q':
      /* Allows user to return to main menu, if for example the user
         mistyped their word. */
      free(oldWord.str); /* Prevents memory leaks. */
      free(newWord);
      printf("\n\nReturning to main menu...\n");
      letter_overflow(); /* Prevents overflow. */
      return;
      break;
    default:
      /* Error catching. */
      printf("\nPlease enter a valid option.\n");
      break;
    }
    letter_overflow(); /* Prevents overflow. */
  }
}

void undo_operation(wordboard* gameboard, stringStack* undoStack, 
                    stringStack* redoStack) {
  /* Purpose: Undo the previous words.
     Preconditions: A operation must have already been performed.
     Postconditions: Undo the previous operation.
     The undo command doesn't actually store the word that is being undone,
     but rather the space that was overwritten by the word. When this command
     is called, the space is popped off the stack overwrites the new word.
     After that, the new word is moved onto the redoStack, where it is actually
     stored.
  */

  struct word undoWord = pop(undoStack); 
  /* Grabs the struct containing the undone word. This is done so that
     my program can retrieve the word that needs to be undone. */

  char* redoWord = malloc(undoWord.length * sizeof(char));
  /* Malloc enough space for  */

  /* printf("undoWord: %s, undoLen: %d, undoOrientation: %c\n", Testing */
  /*        undoWord.str, undoWord.length, undoWord.orientation); Equipment */

  word_swap(gameboard, undoWord, redoWord);
  /* Grabs the characters corresponding to the information on the 'undoWord'
     struct and then writes them to 'redoWord'. */

  /* printf("1 undoWord: %s, undoLen: %d, undoOrientation: %c, redoWord: %s\n", 
   Testing equipment. */
  /*        undoWord.str, undoWord.length, undoWord.orientation, redoWord); */

  for (int letter = 0; letter < undoWord.length; letter++) {
    undoWord.str[letter] = redoWord[letter];
  } /* Copies the literal chars onto the 'undoWord.str'. */

  free(redoWord); /* No longer needed, so avoiding memory leak. */

  /* printf("2 undoWord: %s, undoLen: %d, undoOrientation: %c, redoWord: %s\n", 
   Testing equipment. */
  /*        undoWord.str, undoWord.length, undoWord.orientation, redoWord); */


  /* An undo operation must have a corresponding redo operation, so the program
     pushes the undone word onto the redoStack to be redone later (perhaps?). */
  push(redoStack, undoWord);


  /* Here the program prints out the information of this operation. */
  printf("\nThis word has been undone:\n");
  printf("----------------------------------------\n");
  printf("| Word: %s\n", undoWord.str);
  printf("| Orientation: '%c'\n", undoWord.orientation);
  printf("| Position: (%d,%d)\n", undoWord.posx, undoWord.posy);
  printf("\nTo redo this word, please call the redo operation.\n");
  printf("To undo another word, please call the undo operation again.\n");
  print_board(*gameboard);

}

void redo_operation(wordboard* gameboard, stringStack* undoStack,
                    stringStack* redoStack) {
  /* Purpose: Redo an undo operation.
     Preconditions: An undo operation must have been completed previously.
     Postconditions: Redo the previous operation.
  */

  struct word redoWord = pop(redoStack);
  char* redoWordShow = malloc(redoWord.length * sizeof(char));
  strncpy(redoWordShow, redoWord.str, redoWord.length);
  
  char* undoWord = malloc(redoWord.length * sizeof(char));

  /* printf("redoWord: %s, redoLen: %d, redoOrientation: %c\n", Testing */
  /*        redoWord.str, redoWord.length, redoWord.orientation); Equipment */

  word_swap(gameboard, redoWord, undoWord);
  /* Grabs the characters corresponding to the information on the 'redoWord'
     struct and then writes them to 'undoWord'. */

  /* printf("1 redoWord: %s, redoLen: %d, redoOrientation: %c, undoWord: %s\n", 
   Testing equipment. */
  /*        redoWord.str, redoWord.length, redoWord.orientation, undoWord); */

  for (int letter = 0; letter < redoWord.length; letter++) {
    redoWord.str[letter] = undoWord[letter];
  } /* Copies the literal chars onto the 'redoWord.str'. */

  free(undoWord); /* No longer needed, so avoiding memory leak. */

  /* printf("2 redoWord: %s, redoLen: %d, redoOrientation: %c, undoWord: %s\n",
   Testing equipment. */
  /*        redoWord.str, redoWord.length, redoWord.orientation, undoWord); */

  /* A redo operation must have a corresponding undo operation, so the program
     pushes the redone word onto the undoStack to be undone later (perhaps?). */
  push(undoStack, redoWord);

  /* Here the program prints out the information of this operation. */
  printf("\nThis word has been redone:\n");
  printf("----------------------------------------\n");
  printf("| Word: %s\n", redoWordShow);
  printf("| Orientation: '%c'\n", redoWord.orientation);
  printf("| Position: (%d,%d)\n", redoWord.posx, redoWord.posy);
  printf("\nTo undo this word, please call the undo operation.\n");
  printf("To redo another word, please call the redo operation again.\n");
  print_board(*gameboard);

  free(redoWordShow);

}
