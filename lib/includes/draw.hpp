#ifndef __draw_h__
#define __draw_h__

#include <ncurses.h>

typedef struct {
  int row;
  int col;
} term;
class Draw {
public:
  Draw(){}
  void fgArray(int t, int tmp[][3]);
  void windowDraw(WINDOW* win, int tmp[][3]);
};

#endif
