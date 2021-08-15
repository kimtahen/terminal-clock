//used relative path due to the ale linter problem that not detecting the header file using makefile #include <unistd.h> #include <thread>
#include <ncurses.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"

#define NUM_FG 6
Clock::Clock(std::condition_variable* cv, std::condition_variable* menu, int* currentMenu){
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
  this->_menu = menu;
  this->currentMenu = currentMenu;
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
  std::unique_lock<std::mutex> lck(mtx);
  Draw d;
  while(1){
    while(*currentMenu%2!=0){
      _menu->wait(lck);
    }
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
      }
      d.fgArray(timeinfo->tm_sec/10, tmp);
      d.fgWindowDraw(win[6],tmp);
      d.fgArray(timeinfo->tm_sec%10, tmp);
      d.fgWindowDraw(win[7],tmp);

    }while(_cv->wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout);
    //wait_for suspends the thread for specified time. after suspend, it returns cv_status::timeout. the function I expect is that the thread termination during wating of the thread. if I notify the condition variable while waiting of the thread, it will be awakened not returning the cv_status::time out. now the while statement will be exited.
    //if the unique_lock, lck with mutex mtx is already locked with other thread, the thread is blocked and it tries to lock the mutex. if locking the mutex succeed, it will enter wait_for state.
    //wait_for calls unique_lock.lock() calls mutex.lock()
    //Calling lock on a mutex object that has already been locked by other threads causes the current thread to block (wait) until it can own a lock to it.
    for(int i = 0; i<8; i++){
      werase(win[i]);
      wrefresh(win[i]);
    }
    _menu->notify_all();
  }
}


void Clock::stwThread(){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  std::unique_lock<std::mutex> lck(mtx);
  Draw d;
  while(1){
    while(*currentMenu%2!=1){
      _menu->wait(lck);
    }
    d.fgArray(0,tmp);
    d.fgWindowDraw(win[0],tmp);
    d.fgWindowDraw(win[1],tmp);
    d.dotWindowDraw(win[2],dot);
    d.fgWindowDraw(win[3],tmp);
    d.fgWindowDraw(win[4],tmp);
    d.dotWindowDraw(win[5],dot);
    d.fgWindowDraw(win[6],tmp);
    d.fgWindowDraw(win[7],tmp);

    _cv->wait(lck);
    for(int i = 0; i<8; i++){
      werase(win[i]);
      wrefresh(win[i]);
    }

    _menu->notify_all();
  }
}

void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkDisplayThread();});
  std::thread threadStk([&](){Clock::stwThread();});
  threadClk.join();
  threadStk.join();
}
