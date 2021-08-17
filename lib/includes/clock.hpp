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
  std::condition_variable cv;
  std::condition_variable menu;
  std::condition_variable stop;
  std::condition_variable reset;
  int currentMenu;
  int exitFlag;
  int pressFlag;
  int timCounter;
  int stwCounter;
  int keyboardInput;
  int cursor;

  int yMax, xMax;

  std::mutex mtx;

  Draw d;
  Display dis;

  int prevhour, prevmin, prevsec;

public:
  Clock();
  void update();
  tm* currentTime();

  void clkThread();
  void stwSubThread(int* on, int* time);
  void stwThread();
  void timSubThread();
  void timThread();

  void asyncInputThread(WINDOW* win);

  void tickCurrentTime();


};



#endif
