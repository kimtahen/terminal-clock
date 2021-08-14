#ifndef __clock_h__
#define __clock_h__

#include <iostream>
#include <ncurses.h>

class Clock {
private:
  time_t _time;
  std::condition_variable* _cv;
  WINDOW** win;
  struct tm* timeinfo;
  int prevhour, prevmin, prevsec;
public:
  Clock(std::condition_variable* cv, WINDOW* w[]){
    time(&_time);
    timeinfo = localtime(&_time);
    prevhour = timeinfo->tm_hour;
    prevmin = timeinfo->tm_min;
    prevsec = timeinfo->tm_sec;
    this->_cv = cv;
    this->win = w;
  }
  void update(){
    time(&_time);
    prevhour = timeinfo->tm_hour;
    prevmin = timeinfo->tm_min;
    prevsec = timeinfo->tm_sec;
    timeinfo = localtime(&_time);
  }
  tm* currentTime();

  void clkDisplayThread();

  void tickCurrentTime();
};



#endif
