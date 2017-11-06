/**
 * @file
 * <pre>COPYRIGHT(c) 2017
 * -------------------
 *
 * Minesweeper game implemented in C.
 * Board is always square, max board size is 99x99.
 *
 * Last Modified: Wed Nov  8 19:46:12 PST 2017</pre>
 * @author Adam Rizkalla
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define ROWSIZE   10  // Default row size
#define UNPLAYED  '.' // Character displayed for unplayed square
#define MINE      '*' // Character displayed for mine


// Initializes the board and places mines in random squares
void initBoard(char *board, int size, int numMines)
{
  int r;
  time_t t;
  for(int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      board[i*size + j] = UNPLAYED;
  }
  srand((unsigned)time(&t));
  for (int i = 0; i < numMines; i++) {
    do {
      r = rand() % (size*size);
    } while(board[r] == MINE);
    board[r] = MINE;
  }
}

// Prints the board to stdout
// Shows row and column numbers of the board
//
// If 'hidden' is true, mines will be hidden instead
// and shown as UNPLAYED squares
void printBoard(char *board, int size, int hidden)
{
  printf("     ");
  for (int i = 0; i < size; i++) {
    printf("%2d ", i+1);
  }
  printf("\n-----");
  for (int i = 0; i < size; i++) {
    printf("---");
  }
  printf("\n");
  for(int i = 0; i < size; i++) {
    printf("%2d | ", i+1);
    for(int j = 0; j < size; j++) {
      if (hidden && board[i*size + j] == MINE)
        printf("%2c ", UNPLAYED);
      else
        printf("%2c ", board[i*size + j]);
    }
    printf("\n");
  }
}

// Calculates the score of a given square (number of adjacent mines)
int calcSquareScore(char *board, int size, int row, int col)
{
  int score = 0;
  for (int i = (row-1); i < (row+2); i++) {
    for (int j = (col-1); j < (col+2); j++) {
      if (i >= 0 && i < size
          && j >= 0 && j < size
          && board[i*size + j] == MINE)
        score++;
    }
  }
  return score;
}

// Reveals a given square and continues sweeping adjacent squares
// if the given square has a score of 0
void revealSquare(char *board, int size, int row, int col)
{
  int score;
  if (row < 0 || row >= size || col < 0 || col >= size)
    return;

  score = calcSquareScore(board, size, row, col);
  board[row*size + col] = score + '0';

  if (score == 0) {
    for (int i = (row-1); i < (row+2); i++) {
      for (int j = (col-1); j < (col+2); j++) {
        if ((i != row || j != col)
            && i >= 0 && i < size
            && j >= 0 && j < size
            && board[i*size + j] == UNPLAYED)
          revealSquare(board, size, i, j);
      }
    }
  }

}

// Plays a move on the given (row, col) square
// Return 1 if the square is a mine, 0 otherwise
int playMove(char *board, int size, int row, int col)
{
  if (board[row*size + col] == MINE)
    return 1;
  revealSquare(board, size, row, col);
  return 0;
}

// Checks if the board is fully explored
// Returns 1 on success, 0 otherwise
int checkWinner(char *board, int size)
{
  int winner = 1;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (board[i*size + j] == UNPLAYED) {
        return 0;
      }
    }
  }
  return winner;
}

/** The main function.
 * @param argc the number of tokens on the input line.
 * @param argv an array of tokens.
 * @return 0 on success, 1-255 on failure
 */
int
main(int argc, char *argv[])
{
  char *board;
  int playRow, playCol, rowSize = ROWSIZE;

  if (argc != 2) {
    printf("Please enter a valid row size between 5-99\n");
    printf("Ex: ./minesweeper 10\n");
    return EXIT_FAILURE;
  }
  rowSize = strtol(argv[1], NULL, 10);
  if (rowSize < 5 || rowSize > 99) {
    printf("Please enter a valid row size between 5-99\n");
    printf("Ex: ./minesweeper 10\n");
    return EXIT_FAILURE;
  }

#ifdef DEBUG
  printf("Row size: %d\n", rowSize);
#endif

  // Initialize the board with a 10% mine field
  board = (char*)malloc(rowSize*rowSize);
  initBoard(board, rowSize, (rowSize*rowSize)/10);

  while(1)
  {
    // Print the current board
    printBoard(board, rowSize, 1);

    // Ask user to pick a row and a column to play
    printf("\nPick a row (1-%d) and column (1-%d) to play!\nEnter 0 to exit: ", rowSize, rowSize);
    if ((scanf("%d", &playRow) == 0) || playRow <= 0 || playRow > rowSize) {
      printf("\nThanks for playing!\n");
      break;
    }
    if ((scanf("%d", &playCol) == 0) || playCol <= 0 || playCol > rowSize) {
      printf("\nThanks for playing!\n");
      break;
    }

    // Play move
    printf("Playing (%d, %d)...\n\n", playRow, playCol);
    if (playMove(board, rowSize, playRow-1, playCol-1)) {
      // Hit a mine; player lost
      printBoard(board, rowSize, 0);
      printf("\nBOOM! You hit a mine!\n");
      break;
    }
    if (checkWinner(board, rowSize)) {
      // Board fully explored; player won
      printBoard(board, rowSize, 0);
      printf("\nYou Win!\n");
      break;
    }
  }

  // Free board memory and exit
  free(board);
  return EXIT_SUCCESS;
}
