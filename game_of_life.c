#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 25
#define M 80

void draw(int rows, int columns, int matrix[][columns]);
void filling(int rows, int columns, int father[][columns], int son[][columns]);
int aliveCellsAround(int x, int y, int matrix[][M]);
void newGen(int rows, int columns, int currentGen[][columns],
            int pastGen[][columns]);
int aliveCells(int rows, int columns, int matrix[][columns]);
int isStableConfiguration(int rows, int columns, int currentGen[][columns],
                          int pastGen[][columns]);
int isGameOn(int rows, int columns, int currentGen[][columns],
             int pastGen[][columns]);
void endGameScreen(int genCounter);
void input(int rows, int columns, int currentGen[][columns]);

int main() {

  int currentGen[N][M] = {0};
  int pastGen[N][M] = {0};
  int genCounter = 0;
  int gameSpeed = 180000;

  input(N, M, currentGen);
  int isGame = isGameOn(N, M, currentGen, pastGen);

  FILE *tty;
  tty = freopen("/dev/tty", "r", stdin);
  if (tty == NULL) {
    exit(1);
  }

  initscr();
  noecho();
  clear();
  refresh();

  while (isGame) {
    nodelay(stdscr, TRUE);
    char key = getch();
    if (key == '=' && gameSpeed > 20000) {
      gameSpeed -= 20000;
    } else if (key == '-' && gameSpeed < 220000) {
      gameSpeed += 20000;
    } else if (key == 'q' || key == 'Q') {
      isGame = 0;
      continue;
    }
    
    newGen(N, M, currentGen, pastGen);
    clear();
    draw(N, M, currentGen);
    isGame = isGameOn(N, M, currentGen, pastGen);
    ++genCounter;
    usleep(gameSpeed);
    refresh();
    
  }

  clear();
  echo();
  endwin();
  endGameScreen(genCounter);
  return 0;
}

void input(int rows, int columns, int currentGen[][columns]) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns + 1; ++j) {
        char temp = getchar() - '0';
        if (j != columns + 1) {
            currentGen[i][j] = temp;
        }
    }
  }
}

void draw(int rows, int columns, int currentGen[][columns]) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (currentGen[i][j] == 1) {
        printw("o");
      } else {
        printw(".");
      }
      printw(" ");
    }
    printw("\n");
  }
}

void filling(int rows, int columns, int father[][columns], int son[][columns]) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      son[i][j] = father[i][j];
    }
  }
}

int aliveCellsAround(int x, int y, int currentGen[][M]) {
  int counter = 0;

  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (i != 0 || j != 0) {
        counter += currentGen[(x + N + i) % N][(y + M + j) % M];
      }
    }
  }

  return counter;
}

void newGen(int rows, int columns, int currentGen[][columns],
            int pastGen[][columns]) {

  filling(N, M, currentGen, pastGen);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {

      int num = aliveCellsAround(i, j, pastGen);
      if (pastGen[i][j] == 0) {
        if (num == 3) {
          currentGen[i][j] = 1;
        }
      } else {
        if ((num == 2) || (num == 3)) {
          currentGen[i][j] = 1;
        } else {
          currentGen[i][j] = 0;
        }
      }
    }
  }
}

int aliveCells(int rows, int columns, int currentGen[][columns]) {
  int counter = 0;
  
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (currentGen[i][j] == 1) {
        ++counter;
      }
    }
  }
  return counter;
}

int isStableConfiguration(int rows, int columns, int currentGen[][columns],
                          int pastGen[][columns]) {
  int flag = 1;
  
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (pastGen[i][j] != currentGen[i][j]) {
        flag = 0;
      }
    }
  }
  return flag;
}

int isGameOn(int rows, int columns, int currentGen[][columns],
             int pastGen[][columns]) {

  int flag = 0;

  if ((aliveCells(rows, columns, currentGen) > 0) &&
      (!isStableConfiguration(rows, columns, currentGen, pastGen))) {
    flag = 1;
  }

  return flag;
}

void endGameScreen(int genCounter) {
  printf("\033[0D\033[2J");
  printf("Your colony lasted %d generations.\n", genCounter);
}
