//used relative path due to the ale linter problem that not detecting the header file using makefile
#include <unistd.h>
#include <thread>
#include <ncurses.h>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"

#define NUM_FG 6

tm* Clock::currentTime(){
  update();
  return timeinfo;
}

void Clock::clkDisplayThread(){
  int tmp[5][3];
  int flag = 1;
  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx);
  Draw d;
  refresh();
  do{
    update();

    if(timeinfo->tm_hour != prevhour || flag){
      d.fgArray(timeinfo->tm_hour/10, tmp);
      d.windowDraw(win[0],tmp);
      d.fgArray(timeinfo->tm_hour%10, tmp);
      d.windowDraw(win[1],tmp);
    }

    if(timeinfo->tm_min != prevmin || flag){
      d.fgArray(timeinfo->tm_min/10, tmp);
      d.windowDraw(win[2],tmp);
      d.fgArray(timeinfo->tm_min%10, tmp);
      d.windowDraw(win[3],tmp);
      flag = 0;
    }
      d.fgArray(timeinfo->tm_sec/10, tmp);
      d.windowDraw(win[4],tmp);
      d.fgArray(timeinfo->tm_sec%10, tmp);
      d.windowDraw(win[5],tmp);

  }while(_cv->wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout);
}

void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkDisplayThread();});
  threadClk.join();
}
