#ifndef __display_h__
#define __display_h__
#include <ncurses.h>
#include <iostream>
#include "draw.hpp"

typedef struct {
  int row;
  int col;
}term;
class Display{
private:
  WINDOW* win[12];
  term crit = {1,1};
  term ele_pos[12];
  Draw d;

public:
  Display();
  void reloClock(int x, int y);

  void clr();
  void displayClk(tm* timeinfo, int prevhour, int prevmin, int prevsec);

  void displayStw(int hr, int min, int sec, int dec);
  void displayTim(int hr, int min, int sec);
  void displayTim(int hr, int min, int sec, int cursor);
  void cursor(int index);



};

#endif
