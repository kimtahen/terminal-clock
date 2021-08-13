#ifndef __clock_h__
#define __clock_h__

#include <iostream>

class Clock {
private:
  time_t _time;
  std::condition_variable* _cv;
public:
  Clock(std::condition_variable* cv){
    update();
    this->_cv = cv;
  }
  void update(){
    time(&_time);
  }
  tm* currentTime();

  void clkDisplayThread();

  void tickCurrentTime();
};



#endif
