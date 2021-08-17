//used relative path due to the ale linter problem that not detecting the header file using makefile #include <unistd.h> #include <thread>
#include <ncurses.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"
#include "../lib/includes/display.hpp"

#define NUM_FG 6
Clock::Clock(std::condition_variable* cv, std::condition_variable* menu, std::condition_variable* stop, std::condition_variable* reset, int* currentMenu, int* exitFlag, int* pressFlag, int*stwCounter, int*timCounter){
  time(&_time);
  timeinfo = localtime(&_time);
  prevhour = timeinfo->tm_hour;
  prevmin = timeinfo->tm_min;
  prevsec = timeinfo->tm_sec;
  this->_cv = cv;
  this->_menu = menu;
  this->_stop = stop;
  this->_reset = reset;
  this->currentMenu = currentMenu;
  this->exitFlag = exitFlag;
  this->pressFlag = pressFlag;
  this->stwCounter = stwCounter;
  this->timCounter = timCounter;

  getmaxyx(stdscr, yMax, xMax);
}

void Clock::update(){
  time(&_time);
  prevhour = timeinfo->tm_hour;
  prevmin = timeinfo->tm_min;
  prevsec = timeinfo->tm_sec;
  timeinfo = localtime(&_time);
}

tm* Clock::currentTime(){
  update();
  return timeinfo;
}

void Clock::clkThread(){
  std::unique_lock<std::mutex> lck(mtx);
  while(1){
    while(*currentMenu%3!=0&&*exitFlag!=1){
      _menu->wait(lck);
    }
    if(*exitFlag==1) break;

    dis.reloClock(yMax/2-3, xMax/2-14);
    do{
      update();
      dis.displayClk(timeinfo, prevhour, prevmin, prevsec);

    }while(_cv->wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout);
    //wait_for suspends the thread for specified time. after suspend, it returns cv_status::timeout. the function I expect is that the thread termination during wating of the thread. if I notify the condition variable while waiting of the thread, it will be awakened not returning the cv_status::time out. now the while statement will be exited.
    //if the unique_lock, lck with mutex mtx is already locked with other thread, the thread is blocked and it tries to lock the mutex. if locking the mutex succeed, it will enter wait_for state.
    //wait_for calls unique_lock.lock() calls mutex.lock()
    //Calling lock on a mutex object that has already been locked by other threads causes the current thread to block (wait) until it can own a lock to it.
    dis.clr();
    _menu->notify_all();
  }
}
void Clock::stwSubThread(int* on, int* counter, int* time){
  std::unique_lock<std::mutex> lck(mtx);

  while(1){
    do{
      _stop->wait(lck);
    }while(*currentMenu%3!=1);
    *pressFlag = 0;

    while(_stop->wait_for(lck,std::chrono::milliseconds(50))==std::cv_status::timeout || *currentMenu%3 != 1){
      if(*pressFlag == 115){
        *pressFlag = 0;
        break;
      }

      *stwCounter += 1;
      time[0] = *stwCounter / 3600000;
      time[1] = (*stwCounter%3600000)/60000;
      time[2] = ((*stwCounter%3600000)%60000)/1000;
      time[3] = ((*stwCounter%3600000)%60000)%1000;
      if(*on)
        dis.displayStw(time[0],time[1],time[2],time[3]);
    };
    *pressFlag = 0;
  }
}


void Clock::stwThread(){
  std::condition_variable stw;
  std::unique_lock<std::mutex> lck(mtx);
  int on = 0;
  int counter = 0;
  int time[4] = {0};
  std::thread threadStk([&](){Clock::stwSubThread(&on, &counter, time);});
  threadStk.detach();

  while(1){
    while(*currentMenu%3!=1 && *exitFlag!=1){
      _menu->wait(lck);
    }
    if(*exitFlag==1) break;
    on = 1;
    dis.reloClock(yMax/2-3, xMax/2-21);
    dis.displayStw(time[0],time[1],time[2],time[3]);

    _cv->wait(lck);
    on = 0;
    dis.clr();
    _menu->notify_all();
  }
}

void Clock::timThread(){
  std::unique_lock<std::mutex> lck(mtx);
  while(1){
    while(*currentMenu%3!=2 && *exitFlag != 1){
      _menu->wait(lck);
    }
    if(*exitFlag == 1) break;

    dis.reloClock(yMax/2-3, xMax/2-14);
    dis.displayTim(0,0,0);

    _cv->wait(lck);


    dis.clr();

    _menu->notify_all();
  }
}
void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkThread();});
  std::thread threadStk([&](){Clock::stwThread();});
  std::thread threadTim([&](){Clock::timThread();});

  threadClk.join();
  threadStk.join();
  threadTim.join();
}
