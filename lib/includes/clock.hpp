#ifndef __clock_h__
#define __clock_h__

#include <iostream>
#include <ncurses.h>
#include <mutex>
#include "display.hpp"
#include "draw.hpp"

class Clock {
private:
  time_t _time;
  struct tm* timeinfo;
  std::condition_variable* _cv;
  std::condition_variable* _menu;
  int* currentMenu;
  int* exitFlag;
  int yMax, xMax;

  std::mutex mtx;

  Draw d;
  Display dis;

  int prevhour, prevmin, prevsec;

public:
  Clock(std::condition_variable* cv, std::condition_variable* menu, int* currentMenu, int* exitFlag);
  void update();
  tm* currentTime();

  void clkThread();
  void stwThread();
  void timThread();

  void tickCurrentTime();


};



#endif
