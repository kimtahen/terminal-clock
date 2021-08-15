//used relative path due to the ale linter problem that not detecting the header file using makefile #include <unistd.h> #include <thread>
#include <ncurses.h>
#include <condition_variable>
#include <thread>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"

#define NUM_FG 6
Clock::Clock(std::condition_variable* cv){
  ele_pos[0]={0,0};
  ele_pos[1]={0,4};
  ele_pos[2]={0,8};
  ele_pos[3]={0,10};
  ele_pos[4]={0,14};
  ele_pos[5]={0,18};
  ele_pos[6]={0,20};
  ele_pos[7]={0,24};

  for(int i = 0; i<8; i++){
    if(i==2 || i==5){
      win[i]=newwin(5,1,ele_pos[i].row+crit.row,ele_pos[i].col+crit.col);
    }
    else{
      win[i]=newwin(5,3,ele_pos[i].row+crit.row,ele_pos[i].col+crit.col);
    }
  }
  refresh(); //refresh the entire screen so that the ncurses knows that the new window was created
  time(&_time);
  timeinfo = localtime(&_time);
  prevhour = timeinfo->tm_hour;
  prevmin = timeinfo->tm_min;
  prevsec = timeinfo->tm_sec;
  this->_cv = cv;
}

void Clock::update(){
  time(&_time);
  prevhour = timeinfo->tm_hour;
  prevmin = timeinfo->tm_min;
  prevsec = timeinfo->tm_sec;
  timeinfo = localtime(&_time);
}

void Clock::reloClock(int row, int col){
  crit = {row, col};
  for(int i = 0; i<8; i++){
    werase(win[i]);
    wrefresh(win[i]);
    mvwin(win[i], ele_pos[i].row+crit.row, ele_pos[i].col+crit.col);
  }

}

tm* Clock::currentTime(){
  update();
  return timeinfo;
}

void Clock::clkDisplayThread(){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  int flag = 1;
  int clockPos = 1;
  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx);
  Draw d;
  refresh();
  do{
    update();

    if(timeinfo->tm_hour != prevhour || flag){
      d.fgArray(timeinfo->tm_hour/10, tmp);
      d.fgWindowDraw(win[0],tmp);
      d.fgArray(timeinfo->tm_hour%10, tmp);
      d.fgWindowDraw(win[1],tmp);
      d.dotWindowDraw(win[2],dot);
      d.dotWindowDraw(win[5],dot);
    }

    if(timeinfo->tm_min != prevmin || flag){
      d.fgArray(timeinfo->tm_min/10, tmp);
      d.fgWindowDraw(win[3],tmp);
      d.fgArray(timeinfo->tm_min%10, tmp);
      d.fgWindowDraw(win[4],tmp);
      flag = 0;
    }
    d.fgArray(timeinfo->tm_sec/10, tmp);
    d.fgWindowDraw(win[6],tmp);
    d.fgArray(timeinfo->tm_sec%10, tmp);
    d.fgWindowDraw(win[7],tmp);

  }while(_cv->wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout);
}

void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkDisplayThread();});
  threadClk.join();
}
