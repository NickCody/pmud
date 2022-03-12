#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <locale.h>
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncurses.h>

#define ALIVE 'X'
#define DEAD '.'
#define OUTSIDE '-'

#define OUT_ALIVE L"\u2B24"
#define OUT_DEAD L"\u00B7"

int NUM_ROWS = 15;
int NUM_COLS = 15;
int MAX_GENERATIONS = 10000;
int REFRESH_DELAY_MS = 125;
char FILENAME[4096];

struct results {
  int generations;
  int key_pressed;
};

void process_cli(int argc, char** argv);
struct results play();

// Board Structure
//
// char* = column (array of cells, array of char)
// char** array of columns (array of columns, aka board)
//
// first index = row
// second index = col
// board[0] = row 0
// board[0][0] = cell(0,0)

// Board Allocation/Deallocation
char** allocate_board();
void free_board(char** board);

// Board Operations
void clear_board(char** board, char clearchar);
void print_board(char** board);
void copy_board(char** src, char** dest);
void invert_board(char** board);
int check_board(char** board);
void read_file(char* filename, char** board);
int compare_boards(char** a, char** b);
void randomize_board(char** board);
void set_cell(char** board, char setchar, int row, int col);
char get_cell(char** board, int row, int col);

// Game of life functions
char cell_next(char** board, int row, int col);
int num_neighbors(char** board, int row, int col);
void calc_next_generation(char** nextGen, char** currentGen);

int main(int argc, char** argv) {
  setlocale(LC_ALL, "");

  printf("Welcome to the Game of Life\n");

  process_cli(argc, argv);

  initscr(); /* Start curses mode 		*/
  raw();     /* Line buffering disabled	*/
  nodelay(stdscr, true);

  struct results result = play();

  endwin(); /* End curses mode		  */

  printf("Wow, we experienced %d generations together! This is a special memory that I'll remember.\n", result.generations);
  return 0;
}

//
//
//
void process_cli(int argc, char** argv) {
  int opt;
  strcpy(FILENAME, "");

  while ((opt = getopt(argc, argv, "c:r:g:f:d:")) != -1) {
    switch (opt) {
    case 'c':
      NUM_COLS = atoi(optarg);
      break;
    case 'r':
      NUM_ROWS = atoi(optarg);
      break;
    case 'g':
      MAX_GENERATIONS = atoi(optarg);
      break;
    case 'f':
      strcpy(FILENAME, optarg);
      break;
    case 'd':
      REFRESH_DELAY_MS = atoi(optarg);
      break;
    case '?':
      printf("unknown option: %c\n", opt);
      break;
    }
  }
}

//
// play
//
struct results play() {
  // Create all boards we need
  char** board1 = allocate_board();
  char** board2 = allocate_board();

  // pointers to boards so I can flip between which one is current/next
  char** currentGen = board1;
  char** nextGen = board2;

  // no file reading for now
  if (strlen(FILENAME) > 0)
    read_file(FILENAME, currentGen);
  else
    randomize_board(currentGen);

  printw("Initial generation\n");
  print_board(currentGen);
  refresh(); /* Print it on to the real screen */

  int in = getch();
  int i;

  for (i = 1; i <= MAX_GENERATIONS; i++) {
    move(0, 0);

    calc_next_generation(currentGen, nextGen);

    printw("Generation %d\n", i);
    print_board(nextGen);

    if (compare_boards(currentGen, nextGen) == 0) {
      printw("Next generation identical, aborting!\n");
      break;
    }

    if (!check_board(nextGen)) {
      printw("All dead, aborting!\n");
      break;
    }

    // Standard 3-statement swap of boards using temp variable
    char** tempGen = currentGen;
    currentGen = nextGen;
    nextGen = tempGen;

    refresh(); /* Print it on to the real screen */
    usleep(1000 * REFRESH_DELAY_MS);
    in = getch();
    if (in != ERR && in != KEY_RESIZE)
      break;
  }

  if (in == ERR) {
    nodelay(stdscr, false);
    getch();
  }

  free_board(board1);
  free_board(board2);

  struct results result;
  result.generations = i;
  result.key_pressed = in;

  return result;
}

//
// Board Allocation/Deallocation
//
char** allocate_board() {
  char** board = (char**)malloc(NUM_ROWS * sizeof(char*));
  for (int i = 0; i < NUM_ROWS; i++) {
    board[i] = (char*)malloc(NUM_COLS * sizeof(char));
  }

  clear_board(board, DEAD);

  return board;
}

void free_board(char** board) {
  for (int i = 0; i < NUM_ROWS; i++) {
    free(board[i]);
  }

  free(board);
}

//
// Board Operations
//
void clear_board(char** board, char clearchar) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      board[i][j] = clearchar;
    }
  }
}

void print_board(char** board) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (board[i][j] == ALIVE)
        addwstr(OUT_ALIVE);
      else
        addwstr(OUT_DEAD);
      addwstr(L" ");
    }
    printw("\n");
  }
}

void copy_board(char** src, char** dest) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void invert_board(char** board) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (board[i][j] == ALIVE)
        board[i][j] = DEAD;
      else
        board[i][j] = ALIVE;
    }
  }
}

int check_board(char** board) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (board[i][j] == ALIVE)
        return 1;
    }
  }

  return 0;
}

int compare_boards(char** a, char** b) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (a[i][j] != b[i][j])
        return 1;
    }
  }

  return 0;
}

void randomize_board(char** board) {
  srand(time(NULL));
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      board[i][j] = rand() % 2 == 0 ? ALIVE : DEAD;
    }
  }
}

void read_file(char* filename, char** board) {
  int BUF_LEN = 1000;
  char* buf = malloc(BUF_LEN);

  FILE* h = fopen(filename, "r");

  if (!h) {
    printf("Error, bad file. Aborting.");
    exit(1);
  }

  int row = 0;
  fgets(buf, BUF_LEN, h);
  while (!feof(h)) {

    if (buf[NUM_COLS] == '\n')
      buf[NUM_COLS] = '\0';

    if (strlen(buf) > (size_t)NUM_COLS) {
      printf("%s has %d columns (max %d)\n", filename, (int)strlen(buf), NUM_COLS);
      exit(1);
    }

    for (int j = 0; j < (int)NUM_COLS; j++)
      board[row][j] = buf[j];

    row++;
    fgets(buf, BUF_LEN, h);
  }

  if (row != NUM_ROWS) {
    printf("%s has %d rows (must be %d)\n", filename, row, NUM_ROWS);
    exit(1);
  }

  fclose(h);
}

void set_cell(char** board, char setchar, int row, int col) {
  if (row < 0 || row >= NUM_ROWS || col < 0 || col > NUM_COLS) {
    printf("Error: invalid coordinate specified (%d,%d)", row, col);
    exit(1);
  }

  board[row][col] = setchar;
}

char get_cell(char** board, int row, int col) {
  if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS)
    return DEAD;
  else
    return board[row][col];
}

//
// Game of Life Logic
int num_neighbors(char** board, int row, int col) {
  int count = 0;

  if (get_cell(board, row - 1, col - 1) == ALIVE)
    count++;
  if (get_cell(board, row - 1, col) == ALIVE)
    count++;
  if (get_cell(board, row - 1, col + 1) == ALIVE)
    count++;
  if (get_cell(board, row, col - 1) == ALIVE)
    count++;
  if (get_cell(board, row, col + 1) == ALIVE)
    count++;
  if (get_cell(board, row + 1, col - 1) == ALIVE)
    count++;
  if (get_cell(board, row + 1, col) == ALIVE)
    count++;
  if (get_cell(board, row + 1, col + 1) == ALIVE)
    count++;

  return count;
}

char cell_next(char** board, int row, int col) {
  int n = num_neighbors(board, row, col);
  char cell = get_cell(board, row, col);
  if (cell == ALIVE) {
    if (n == 2 || n == 3)
      return ALIVE;
    else
      return DEAD;

  } else if (cell == DEAD) {
    if (n == 3)
      return ALIVE;
    else
      return DEAD;
  }

  return OUTSIDE;
}

void calc_next_generation(char** currentGen, char** nextGen) {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      nextGen[i][j] = cell_next(currentGen, i, j);
    }
  }
}