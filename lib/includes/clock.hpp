#ifndef __clock_h__
#define __clock_h__

#include <iostream>
#include <ncurses.h>
typedef struct {
  int row;
  int col;
}term;

class Clock {
private:
  time_t _time;
  struct tm* timeinfo;

  std::condition_variable* _cv;

  WINDOW* win[8];
  term crit = {1,1};
  term ele_pos[8];
  int prevhour, prevmin, prevsec;

public:
  Clock(std::condition_variable* cv);
  void update();
  void reloClock(int x, int y);
  tm* currentTime();

  void clkDisplayThread();

  void tickCurrentTime();


};



#endif
