#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "libs/term_tools.h"
#include "libs/decorations.c"
#include "libs/gamefn.h"

// обозначение светов
#define Reset "\x1b[0m"//белый
#define ColRed "\x1b[31m"
#define ColCors "\x1b[32m"
#define ColNum "\x1b[33m"
//#define reset_curs printf("")

void genCode(char **START_GAME_FIELD, int diff, int row, int col)
{
  int totalCells = row * col;
  int numBombs = 0;
  double threshold = 0.0;

  switch (diff) {
    case CUSTOM:
      threshold = thcust();
      break;
    case EASY:
      threshold = 0.1;
      break;
    case NORMAL:
      threshold = 0.3;
      break;
    case HARD:
      threshold = 0.5;
      break;
    case VERY_HARD:
      threshold = 0.7;
      break;
    case IMPOSSIBLE:
      threshold = 0.9;
      break;
  }

  //int tex = row - 1;
  //int tey = col - 1;
  numBombs = totalCells * threshold;

  for (int i = 0; i < numBombs; )
  {
    int x = (rand() % row);
    int y = (rand() % col);
    printf("%d, %d", x, y);
    if (x < 0 || x >= col || y < 0 || y >= row)
    {
      continue;
    }
    else
    {
      if (START_GAME_FIELD[y][x] != '*')
      {
        START_GAME_FIELD[y][x] = '*';
        i++;
      }
    }
  }
}


int main()
{
  srand(time(NULL));
  int row, col, corx = 0, cory = 0, diff, x = 0, y = 0, width, height, dor, dbf = 0;
  char move;
  term_size(&height, &width);
  indent(&width, &height, &x, &y, col, row);
  //welcome(&width, &height);
  printf("Enter the field dimensions (format: 30x30) > ");
  scanf("%dx%d", &row ,&col);
  if (row > height || col > width)
  {
    if (confirmInput(width, height, "The size of your field is larger than the size of the terminal. Are you sure? (Yes/no) > ", width / 4, height / 2) == 0)
    {
        row = height / 4;
        col = width / 4;
    }
  } else if (row < 30 && col < 30)
  {
    if (confirmInput(width, height, "The size of your field is less than 30x30. Are you sure? (Yes/no) > ", 30, 30) == 0)
    {
        row = 30;
        col = 30;
    }
  }

  diff = choseDifficulty();

  //allocation memmory to fields
  char **START_GAME_FIELD = (char **)malloc(col * sizeof(char *));
  for (int i = 0; i < col; i++)
  {
    START_GAME_FIELD[i] = (char *)malloc(row * sizeof(char));
  }

  for (int i = 0; i < col; i++)
  {
    for (int j = 0; j < row; j++)
    {
      START_GAME_FIELD[i][j] = '9';
    }
  }

  char **WORK_FIELD = (char **)malloc(col * sizeof(char *));
  for (int i = 0; i < col; i++)
  {
    WORK_FIELD[i] = (char *)malloc(row * sizeof(char));
  }
  char **FLAG_FIELD = (char **)malloc(row * sizeof(char *));
  for (int i = 0; i < row; i++)
  {
    FLAG_FIELD[i] = (char *)malloc(col * sizeof(char));
  }

  for (int i = 0; i < col; i++)
  {
    for (int j = 0; j < row; j++)
    {
      //START_GAME_FIELD[i][j] = 9;
      WORK_FIELD[i][j] = 9;
      FLAG_FIELD[j][i] = 9;
    }
  }
  //allocation memmory to fields

  //#############################################
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      //printf("%c ", START_GAME_FIELD[i][j]);
      //START_GAME_FIELD[i][j] = '1';
    }
  }
  //#############################################
  //genCode(START_GAME_FIELD, diff, row, col);
  genCode(START_GAME_FIELD, diff, col, row);

  //#############################################
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      //printf("%c ", START_GAME_FIELD[j][i]);
    }
  }
  //#############################################
  enableRawMode();
  system("clear");
  while(1)
  {
    indent(&width, &height, &x, &y, col, row);
    dryFd(START_GAME_FIELD, WORK_FIELD, FLAG_FIELD, &y, &x, row, col, corx, cory, diff, dbf);
    //dryFd(START_GAME_FIELD, WORK_FIELD, FLAG_FIELD, &y, &x, col, row, corx, cory, diff);
    scanf("\n%c", &move);

    switch(move)
    {
    case 'w':
        corx--;
        if (corx < 0){corx = col-1;}
        break;
    case 'k':
        corx--;
        if (corx < 0){corx = col-1;}
        break;
    case 's':
        corx++;
        if (corx > col - 1){corx= 0;}
        break;
    case 'j':
        corx++;
        if (corx > col - 1){corx= 0;}
        break;
    case 'a':
        cory--;
        if (cory < 0){cory = row-1;}
        break;
    case 'h':
        cory--;
        if (cory < 0){cory = row-1;}
        break;
    case 'd':
        cory++;
        if (cory > row - 1){cory= 0;}
        break;
    case 'l':
        cory++;
        if (cory > row - 1){cory= 0;}
        break;
    case 'r':
        system("clear");
        term_size(&height, &width);
        break;
    case 'q':
        system("clear");
        disableRawMode();
        dor = doureal();
        if (dor == 2)
        {
          printf("%d\n", dor);
          system("killall mpv; clear; cowsay spasiba za igru!!");
          //printf("spasiba za igru!!\n");
          return 0;
        }
        else if (dor == 1)
        {
          enableRawMode();
          break;
        }
    case 'f':
        addFlag(FLAG_FIELD, corx, cory);
        break;
    case '`':
        //printf("\npisun\n");
        clc__console__(&dbf);
        break;
    case '0':
      system("mpv --loop=inf --quiet --no-video --no-terminal libs/0.mp3");
      break;
    case '2':
      system("killall mpv;clear");
      break;
    default:
        open_cell(START_GAME_FIELD, WORK_FIELD, cory, corx, col, row);
        break;
  }
}
  system("killall mpv");
}

