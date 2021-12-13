#include "geesespotter_lib.h"
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <ctime>      // for time(NULL)
#include "geesespotter_lib.h"
#include "geesespotter.h"

int main()
{
  srand(time(NULL));
  game();
  return 0;
}

bool game()
{
  std::size_t xdim {0};
  std::size_t ydim {0};
  unsigned int numgeese {0};
  char * gameBoard {NULL};

  startGame(gameBoard, xdim, ydim, numgeese);

  char currAction {0};
  while(currAction != 'Q')
  {
    switch (currAction)
    {
      case 'S' :  // show
      {
        actionShow(gameBoard, xdim, ydim, numgeese);
        break;
      }
      case 'M' :  // mark
      {
        actionMark(gameBoard, xdim, ydim);
        break;
      }
      case 'R' :  // restart
      {
        std::cout << "Restarting the game." << std::endl;
        startGame(gameBoard, xdim, ydim, numgeese);
        break;
      }
    }
    printBoard(gameBoard, xdim, ydim);
    if (isGameWon(gameBoard, xdim, ydim))
    {
      std::cout << "You have revealed all the fields without disturbing a goose!" << std::endl;
      std::cout << "YOU WON!!!" << std::endl;
      for (std::size_t reveal_row {0}; reveal_row < ydim; reveal_row++)
      {
        for (std::size_t reveal_col {0}; reveal_col < xdim; reveal_col++)
        {
          gameBoard[reveal_row*xdim + reveal_col] = (gameBoard[reveal_row*xdim + reveal_col] & valueMask());
        }
      }
      printBoard(gameBoard, xdim, ydim);
      std::cout << "Resetting the game board." << std::endl;
      startGame(gameBoard, xdim, ydim, numgeese);
      printBoard(gameBoard, xdim, ydim);
    }
    currAction = getAction();
  }

  cleanBoard(gameBoard);
  return true;
}

void startGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese)
{
  std::cout << "Welcome to GeeseSpotter!" << std::endl;
  do {
    std::cout << "Please enter the x dimension (max " << xdim_max() << "): ";
    std::cin >> xdim;
  } while(xdim < 1 || xdim > xdim_max());
  do {
    std::cout << "Please enter the y dimension (max " << ydim_max() << "): ";
    std::cin >> ydim;
  } while(ydim < 1 || ydim > ydim_max());

  std::cout << "Please enter the number of geese: ";
  std::cin >> numgeese;
  while (numgeese > xdim * ydim)
  {
    std::cout << "That's too many geese!" << std::endl;
    std::cout << "Please enter the number of geese: ";
    std::cin >> numgeese;
  }

  cleanBoard(board);
  board = createBoard(xdim, ydim);
  spreadGeese(board, xdim, ydim, numgeese);
  computeNeighbors(board, xdim, ydim);
  hideBoard(board, xdim, ydim); 
}

char getAction()
{
  char action {0};

  std::cout << "Please enter the action ([S]how, [M]ark, [R]estart, [Q]uit): ";
  std::cin >> action;

  if (islower(action))
    action = toupper(action);

  return action;
}

void actionShow(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numgeese)
{
  std::size_t reveal_x {0};
  std::size_t reveal_y {0};
  std::cout << "Please enter the x location to show: ";
  std::cin >> reveal_x;
  std::cout << "Please enter the y location to show: ";
  std::cin >> reveal_y;

  if (reveal_x >= xdim || reveal_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (board[xdim*reveal_y + reveal_x] & markedBit())
  {
    std::cout << "Location is marked, and therefore cannot be revealed." << std::endl;
    std::cout << "Use Mark on location to unmark." << std::endl;
  }
  else if (reveal(board, xdim, ydim, reveal_x, reveal_y) == 9)
  {
    std::cout << "You disturbed a goose! Your game has ended." << std::endl;
    printBoard(board, xdim, ydim);
    std::cout << "Starting a new game." << std::endl;
    startGame(board, xdim, ydim, numgeese);
  }
}

void actionMark(char * board, std::size_t xdim, std::size_t ydim)
{
  std::size_t mark_x {0};
  std::size_t mark_y {0};
  std::cout << "Please enter the x location to mark: ";
  std::cin >> mark_x;
  std::cout << "Please enter the y location to mark: ";
  std::cin >> mark_y;

  if (mark_x >= xdim || mark_y >= ydim)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (mark(board, xdim, ydim, mark_x, mark_y) == 2)
  {
    std::cout << "Position already revealed, so cannot be marked." << std::endl;
  }
}

std::size_t readSizeT()
{
  return 0;
}

std::size_t xdim_max()
{
  return 60;
}

std::size_t ydim_max()
{
  return 20;
}

char markedBit()
{
  return 0x10;
}

char hiddenBit()
{
  return 0x20;
}

char valueMask()
{
  return 0x0F;
}

void spreadGeese(char * board, std::size_t xdim, std::size_t ydim, unsigned int numgeese)
{
  if (board != NULL)
  {
    for (unsigned int gen_goose {0}; gen_goose < numgeese; gen_goose++)
    {
      std::size_t try_position {0};
      do {
         try_position = rand() % (xdim * ydim);
      } while (board[try_position] != 0);

      board[try_position] = 9;
    }
  }
}
//g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11
char *createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);


void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {
  for(int k{0}; k < ((xdim*ydim)); k++ ) {
    if ((board[k] & valueMask()) != 9) continue; // nothing to do

    //Lower right 
    if( k+xdim+1 > ((xdim*ydim)-1) || (k+xdim+1) % xdim == 0 || (board[k+xdim+1] & valueMask()) == 9) {}
    else {
      board[k+xdim+1]++;
    }

    //Lower
    if( k+xdim > ((xdim*ydim)-1) || (board[k+xdim] & valueMask()) == 9 ) {}
    else {
      board[k+xdim]++;
    }

    //Lower left
   if( k+xdim-1 > ((xdim*ydim)-1) || (k+xdim-1) % xdim == xdim-1 || (board[k+xdim-1] & valueMask()) == 9) {}
    else {
      board[k+xdim-1]++;
    }

    //Right
    if( k+1 > ((xdim*ydim)-1) || (k+1) % xdim == 0 || (board[k+1] & valueMask()) == 9 ) {}
    else {
      board[k+1]++;
    }

    //Left
    if( (int)(k-1) < 0 || (k-1) % xdim == xdim-1 || (board[k-1] & valueMask()) == 9 ) {}
    else {
      board[k-1]++;
    }

    //Upper Left
    if( (int)(k-xdim-1) < 0 || (k-xdim-1) % xdim == xdim-1 || (board[k-xdim-1] & valueMask()) == 9) {}
    else {
      board[k-xdim-1]++;
    }

    //Upper
    if( (int)(k-xdim) < 0 || (board[k-xdim] & valueMask()) == 9 ) {}
    else {
      board[k-xdim]++;
    }

    //Upper Right
    if( (int)(k-xdim+1) < 0 || (k-xdim+1) % xdim == 0 || (board[k-xdim+1] & valueMask()) == 9) {}
    else {
      board[k-xdim+1]++;
    }
    
  }
return;
}
void hideBoard(char *board, std::size_t xdim, std::size_t ydim){
  for(int k{0}; k < (xdim*ydim); k++ ) {
    if (board[k] & hiddenBit()) continue;
    board[k] = board[k] + hiddenBit();
  }
return;
}

char *createBoard(std::size_t xdim, std::size_t ydim) {
  char *x = new char[xdim*ydim]{};
  return x;
}

void cleanBoard(char *board) {
if (board != NULL) {
  delete[] board;
  board = NULL;
}
return;
}
void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
  for (int k{0}; k < xdim*ydim; k++) {
    if ((board[k] & hiddenBit())) {
      if(board[k] & markedBit()) {
        std::cout << "M ";
    }
    else {
      std::cout << "* ";
    }
    }
    
    else
    {
      std::cout << (int)(board[k]&valueMask()) << " ";
    }
    if (k % xdim == xdim-1){
      std::cout << "\n";
    }

  }
  return;
}
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
  int index;
  index = (((xdim*yloc)+xloc));

  if(board[index]-hiddenBit() == 9) {
    return 9;
  }
  if(board[index] & markedBit()) {
    board[index] -= markedBit()+hiddenBit();
    return 1;
  }
  if(!(board[index] & hiddenBit())) {
    return 2;
  }
  board[index] = board[index] - hiddenBit();
  if(board[index] == 0) {
    //Lower right 
    if( index+xdim+1 > ((xdim*ydim)-1) || (index+xdim+1) % xdim == 0) {}
    else {
      if((board[index+xdim+1] & hiddenBit())) {
        board[index+xdim+1] -= hiddenBit();
      }
    }

    //Lower
    if( index+xdim > ((xdim*ydim)-1)) {}
    else {
      board[index+xdim] -= hiddenBit();
    }

    //Lower left
   if( index+xdim-1 > ((xdim*ydim)-1) || (index+xdim-1) % xdim == xdim-1) {}
    else {
      board[index+xdim-1] -= hiddenBit();
    }

    //Right
    if( index+1 > ((xdim*ydim)-1) || (index+1) % xdim == 0) {}
    else {
      board[index+1] -= hiddenBit();
    }

    //Left
    if( (int)(index-1) < 0 || (index-1) % xdim == xdim-1) {}
    else {
      board[index-1] -= hiddenBit();
    }

    //Upper Left
    if( (int)(index-xdim-1) < 0 || (index-xdim-1) % xdim == xdim-1) {}
    else {
      board[index-xdim-1] -= hiddenBit();
    }

    //Upper
    if( (int)(index-xdim) < 0 ) {}
    else {
      board[index-xdim] -= hiddenBit();
    }

    //Upper Right
    if( (int)(index-xdim+1) < 0 || (index-xdim+1) % xdim == 0) {}
    else {
      board[index-xdim+1] -= hiddenBit();
    }
  }
return 0;
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
  int index;
  index = (((xdim*yloc)+xloc));
  if(board[index] & markedBit()) return 0;
  
  if (board[index] & hiddenBit()) {
    board[index] = board[index] + markedBit();
    return 0;
  }
  
  return 2;
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim){
    
    int length = (xdim*ydim)-1;
    
    for (int k{0}; k < length; k++) {
      if (!(board[k] & hiddenBit())) {
        continue;
      }
      if ((board[k] & valueMask()) != 9 ) {
        return false;
      } 
    }

    return true;
}