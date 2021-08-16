#ifndef __draw_h__
#define __draw_h__

#include <ncurses.h>

class Draw {
public:
  Draw();
  void fgArray(int t, int tmp[][3]);
  void fgWindowDraw(WINDOW* win, int tmp[][3], int color);
  void dotWindowDraw(WINDOW* win, int tmp[][1], int color);

};

#endif
