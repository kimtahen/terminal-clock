#ifndef __clock_h__
#define __clock_h__

#include <iostream>
#include <condition_variable>
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
  std::condition_variable curs;
  int currentMenu;
  int exitFlag;
  int pressFlag;
  int resumeFlag;
  int timCounter;
  int timPrevCounter;
  int stwCounter;
  int stwPrevCounter;
  int stwTime[4]={0};
  int timTime[4]={0};
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
  void stwSubThread(int* on);
  void stwThread();
  void timSubThread(int* on, int* time);
  void timThread();

  void asyncInputThread(WINDOW* win);

  void tickCurrentTime();


};



#endif
