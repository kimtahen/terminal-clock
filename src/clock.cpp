//used relative path due to the ale linter problem that not detecting the header file using makefile #include <unistd.h> #include <thread>
#include <ncurses.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"
#include "../lib/includes/display.hpp"

#define NUM_FG 6
Clock::Clock(){
  time(&_time);
  timeinfo = localtime(&_time);
  prevhour = timeinfo->tm_hour;
  prevmin = timeinfo->tm_min;
  prevsec = timeinfo->tm_sec;
  currentMenu = 0;
  exitFlag = 0;
  pressFlag = 0;
  stwCounter = 0;
  timCounter = 0;
  cursor = 0;

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
    while(currentMenu%3!=0&&exitFlag!=1){
      menu.wait(lck);
    }
    if(exitFlag==1) break;

    dis.reloClock(yMax/2-3, xMax/2-14);
    do{
      update();
      dis.displayClk(timeinfo, prevhour, prevmin, prevsec);

    }while(cv.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout);
    //wait_for suspends the thread for specified time. after suspend, it returns cv_status::timeout. the function I expect is that the thread termination during wating of the thread. if I notify the condition variable while waiting of the thread, it will be awakened not returning the cv_status::time out. now the while statement will be exited.
    //if the unique_lock, lck with mutex mtx is already locked with other thread, the thread is blocked and it tries to lock the mutex. if locking the mutex succeed, it will enter wait_for state.
    //wait_for calls unique_lock.lock() calls mutex.lock()
    //Calling lock on a mutex object that has already been locked by other threads causes the current thread to block (wait) until it can own a lock to it.
    dis.clr();
    menu.notify_all();
  }
}
void Clock::stwSubThread(int* on){
  std::unique_lock<std::mutex> lck(mtx);
  double prevcurtime = 0;
  clock_t start;

  while(1){
    do{
      stop.wait(lck);
      start=clock();
    }while(currentMenu%3!=1);
    pressFlag = 0;
    clock_t end;
    double curtime = prevcurtime;

    while(1){
      end = clock();
      curtime = (double)(end - start) /CLOCKS_PER_SEC;

      if(pressFlag == 115){
        pressFlag = 0;
        break;
      }

      stwTime[0] = (int)(stwAccum+curtime) / 3600;
      stwTime[1] = ((int)(stwAccum+curtime)%3600)/60;
      stwTime[2] = ((int)(stwAccum+curtime)%3600)%60;
      stwTime[3] = (int)((stwAccum+curtime) * 1000) %1000;
      if(*on)
        dis.displayStw(stwTime[0],stwTime[1],stwTime[2],stwTime[3]);
      else
        dis.clr();

      prevcurtime = curtime;
    }
    stwAccum += curtime;
    pressFlag = 0;
  }
}


void Clock::stwThread(){
  std::condition_variable stw;
  std::unique_lock<std::mutex> lck(mtx);
  int on = 0;
  std::thread threadStk([&](){Clock::stwSubThread(&on);});
  threadStk.detach();

  while(1){
    while(currentMenu%3!=1 && exitFlag!=1){
      menu.wait(lck);
    }
    if(exitFlag==1) break;
    on = 1;
    dis.reloClock(yMax/2-3, xMax/2-21);
    dis.displayStw(stwTime[0],stwTime[1],stwTime[2],stwTime[3]);

    cv.wait(lck);
    on = 0;
    dis.clr();
    menu.notify_all();
  }
}

void Clock::timSubThread(){


}

void Clock::timThread(){
  std::unique_lock<std::mutex> lck(mtx);
  int time[3]={0};
  while(1){
    while(currentMenu%3!=2 && exitFlag != 1){
      menu.wait(lck);
    }
    if(exitFlag == 1) break;

    dis.reloClock(yMax/2-3, xMax/2-14);
    dis.displayTim(time[0],time[1],time[2]);

    do{
      dis.displayTim(time[0],time[1],time[2]);
      if(cv.wait_for(lck,std::chrono::milliseconds(100))!=std::cv_status::timeout){
        break;
      }
      dis.cursor(cursor);
      if(cv.wait_for(lck,std::chrono::milliseconds(100))!=std::cv_status::timeout){
        break;
      }
    }while(1);



    dis.clr();

    menu.notify_all();
  }
}
void Clock::asyncInputThread(WINDOW* win){
  while(1){
    keyboardInput = getch();
    pressFlag = keyboardInput;
    switch(keyboardInput){
      //terminate the program
      //terminate procedure : set exitflag to 1 -> call cv.notify_all() -> the running thread exits from current state -> it calls menu.notify_all() -> the waiting threads awakened and because of the exitFlag it breaks the while loop -> all threads are terminated
      case 120:
        exitFlag = 1;
        cv.notify_all();
        break;
      case 115:
        stop.notify_all();
        break;
      case 114:
        if(currentMenu%3==1){
          dis.displayStw(0,0,0,0);
          stwAccum = 0;
        }
        if(currentMenu%3==2){
          timCounter = 0;
          dis.displayTim(0,0,0);
        }
        break;
      case 109:
        cursor = (cursor+1)%3;
        break;
      //up
      case 107:
        menu.notify_all();
        break;
      //down
      case 106:
        break;
      //right
      case 108:
        currentMenu++;
        cv.notify_all();
        break;
      //left
      case 104:
        currentMenu--;
        cv.notify_all();
        break;
    }
  }

  return;
}
void Clock::tickCurrentTime(){
  std::thread threadClk([&](){Clock::clkThread();});
  std::thread threadStk([&](){Clock::stwThread();});
  std::thread threadTim([&](){Clock::timThread();});
  std::thread threadKey([&](){Clock::asyncInputThread(stdscr);});

  threadClk.join();
  threadStk.join();
  threadTim.join();
  threadKey.detach();
}
