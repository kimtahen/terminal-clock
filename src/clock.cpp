//used relative path due to the ale linter problem that not detecting the header file using makefile
#include "../lib/includes/clock.hpp"
#include <thread>
#include <ncurses.h>

tm* Clock::currentTime(){
  update();
  struct tm* timeinfo = localtime(&_time);
  return timeinfo;
}

void Clock::clkDisplayThread(){
  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx);
  while(_cv->wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout){
    printw("%s",asctime(currentTime()));
    refresh();
  }
}

void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkDisplayThread();});
  threadClk.join();
}
