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
  std::condition_variable* _stop;
  std::condition_variable* _reset;
  int* currentMenu;
  int* exitFlag;
  int* pressFlag;
  int yMax, xMax;

  std::mutex mtx;

  Draw d;
  Display dis;

  int prevhour, prevmin, prevsec;

public:
  Clock(std::condition_variable* cv, std::condition_variable* menu, std::condition_variable* stop,std::condition_variable* reset, int* currentMenu, int* exitFlag, int* pressFlag);
  void update();
  tm* currentTime();

  void clkThread();
  void stwSubThread(int* on, int* counter, int* time);
  void stwThread();
  void timThread();

  void tickCurrentTime();


};



#endif
