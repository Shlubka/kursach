#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "libs/term_tools.h"
#include "libs/decorations.c"
#include "libs/gamefn.h"

int main(void)
{
  srand(time(NULL));
  struct BombCoords *bombCoords = NULL;
  struct FlagCoords *flagCoords = NULL;
  struct OpenCells *opencells = NULL;
  int row, col, corx = 0, cory = 0, diff, x, y, width, height, dor;
  char move;
  term_size(&height, &width);
  indent(&width, &height, &x, &y, col, row);
  welcome(&y, &x);
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
  int num = genCode(&bombCoords, diff, row, col);

  enableRawMode();
  system("clear");
  while(1)
  {
    indent(&width, &height, &x, &y, col, row);
    drowField(&bombCoords, &flagCoords, opencells, row, col, corx, cory, diff, num, &x, &y);
    scanf("\n%c", &move);

    switch(move)
    {
    case 'w':
        corx--;
        if (corx < 0){corx = col-1;}
        break;
    case 's':
        corx++;
        if (corx > col - 1){corx= 0;}
        break;
    case 'a':
        cory--;
        if (cory < 0){cory = row-1;}
        break;
    case 'd':
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
          //printf("%d\n", dor);
          return 0;
        }
        else if (dor == 1)
        {
          enableRawMode();
          break;
        }
    case 'f':
        addFlag(&flagCoords, corx, cory);
        break;
    default:
        open_open_cell(&opencells, corx, cory, col, row);
        //openCell(opencells, bombCoords, cory, corx);
        //system("clear");
        //printf("Incorect input\n");
        //sleep(1);
        break;
    }
  }
}

