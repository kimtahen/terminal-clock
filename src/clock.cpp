//used relative path due to the ale linter problem that not detecting the header file using makefile #include <unistd.h> #include <thread>
#include <ncurses.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include "../lib/includes/clock.hpp"
#include "../lib/includes/draw.hpp"
#include "../lib/includes/display.hpp"

#define NUM_FG 6
Clock::Clock(int velocity){
  this->velocity = velocity;
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
  std::chrono::system_clock::time_point startClk;
  std::chrono::system_clock::time_point endClk;

  while(1){
    do{
      stwPrevCounter = stwCounter;
      stop.wait(lck);
      startClk = std::chrono::system_clock::now();
    }while(currentMenu%3!=1&&exitFlag!=1);

    pressFlag = 0;

    while(stop.wait_for(lck,std::chrono::milliseconds(velocity))==std::cv_status::timeout || currentMenu!=1 && exitFlag!=1){
      if(pressFlag == 115 && currentMenu%3 ==1){
        pressFlag = 0;
        break;
      }
      if(exitFlag==1) break;

      endClk = std::chrono::system_clock::now();
      std::chrono::milliseconds mill = std::chrono::duration_cast<std::chrono::milliseconds>(endClk-startClk);
      stwCounter = mill.count() + stwPrevCounter;
      stwTime[0] = stwCounter / 3600000;
      stwTime[1] = (stwCounter%3600000)/60000;
      stwTime[2] = ((stwCounter%3600000)%60000)/1000;
      stwTime[3] = ((stwCounter%3600000)%60000)%1000;
      if(*on)
        dis.displayStw(stwTime[0],stwTime[1],stwTime[2],stwTime[3]);
    };
    pressFlag = 0;
    if(exitFlag==1) break;
  }
}


void Clock::stwThread(){
  std::condition_variable stw;
  std::unique_lock<std::mutex> lck(mtx);
  int on = 0;
  std::thread threadStw([&](){Clock::stwSubThread(&on);});
  threadStw.detach();

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

void Clock::timSubThread(int* on, int* time){
  std::unique_lock<std::mutex> lck(mtx);
  std::chrono::system_clock::time_point startClk;
  std::chrono::system_clock::time_point endClk;
  int setTime[4]= {0};
  resumeFlag = 0;

  while(1){
    do{
      timPrevCounter = timCounter;
      stop.wait(lck);
      startClk = std::chrono::system_clock::now();
    }while(currentMenu%3!=2&&exitFlag!=1);

    if(!resumeFlag){
      for(int i = 0; i<4; i++){
        setTime[i] = timTime[i];
      }
      timPrevCounter = timTime[0]*3600 + timTime[1]*60 + timTime[2];
      timPrevCounter *= 1000;
      timCounter = timPrevCounter;
      resumeFlag = 1;
    }

    do{
      if(exitFlag==1) break;

      if(timCounter == 0) {
        resumeFlag = 0;
        for(int i = 0; i<4; i++){
          timTime[i] = setTime[i];
        }
        dis.displayTim(timTime[0],timTime[1],timTime[2],timTime[3],cursor);
        break;
      }
      endClk = std::chrono::system_clock::now();
      std::chrono::milliseconds mill = std::chrono::duration_cast<std::chrono::milliseconds>(endClk-startClk);
      timCounter = timPrevCounter - mill.count();
      timTime[0] = timCounter / 3600000;
      timTime[1] = (timCounter%3600000)/60000;
      timTime[2] = ((timCounter%3600000)%60000)/1000;
      timTime[3] = ((timCounter%3600000)%60000)%1000;

      if(*on)
        dis.displayTim(timTime[0],timTime[1],timTime[2],timTime[3]);
    }while(stop.wait_for(lck,std::chrono::milliseconds(velocity))==std::cv_status::timeout || currentMenu%3 != 2 && exitFlag !=1);
    if(exitFlag==1) break;
  }


}

void Clock::timThread(){
  std::unique_lock<std::mutex> lck(mtx);
  int time[4]={0};
  int on = 0;
  std::thread threadTim([&](){Clock::timSubThread(&on, time);});
  threadTim.detach();
  while(1){
    while(currentMenu%3!=2 && exitFlag != 1){
      menu.wait(lck);
    }
    if(exitFlag == 1) break;

    on = 1;

    dis.reloClock(yMax/2-3, xMax/2-21);
    dis.displayTim(timTime[0],timTime[1],timTime[2], timTime[3]);

    for(;;){
      if(!timCounter)
        dis.displayTim(timTime[0],timTime[1],timTime[2],timTime[3],cursor);
      cv.wait(lck);
      //synchronization using cv
      if(currentMenu%3!=2 || exitFlag ==1){
        break;
      }
    }
    on = 0;
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
        stop.notify_all();
        menu.notify_all();
        break;
      case 115:
        stop.notify_all();
        break;
      case 114:
        if(currentMenu%3==1){
          stwCounter = 0;
          stwPrevCounter = 0;
          for(int i = 0; i<4; i++)
            stwTime[i] = 0;
          dis.displayStw(0,0,0,0);
        }
        if(currentMenu%3==2){
          timCounter = 0;
          stop.notify_all();

        }
        break;
      case 109:
        cursor = (cursor+1)%3;
        cv.notify_all();
        break;

      case 107:
        if(timTime[cursor]==60){
          timTime[cursor] = 0;
        }else{
          timTime[cursor]++;
        }
        cv.notify_all();

        break;
      case 106:
        if(timTime[cursor]==0){
          timTime[cursor] = 60;
        }else{
          timTime[cursor]--;
        }
        cv.notify_all();

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
    if(keyboardInput==120){
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
  threadKey.join();
}
