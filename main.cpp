#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "./lib/includes/clock.hpp"
#include "./lib/includes/draw.hpp"
#include "./lib/includes/winrel.hpp"

void asyncInputThread(WINDOW* win);

int keyboardInput;
int currentMenu = 0;
int exitFlag = 0;
//cv for terminating each thread
std::condition_variable cv;
//menu for rotating menu
std::condition_variable menu;
int main(){
  initscr();
  noecho();
  start_color();
  cbreak();
  curs_set(0);
  //configuration
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW* bg = newwin(yMax,xMax,0,0);
  refresh();
  box(bg, 0, 0);
  wrefresh(bg);

  //keyboardInput
  std::thread threadKB(asyncInputThread, bg);
  threadKB.detach(); // 'abort signal' solved by calling detaching

  //clock start
  Clock clock1(&cv, &menu, &currentMenu, &exitFlag);
  clock1.tickCurrentTime();
  endwin();

  return 0;
}

void asyncInputThread(WINDOW* win){
  while(1){
    keyboardInput = getch();
    switch(keyboardInput){
      //terminate the program
      //terminate procedure : set exitflag to 1 -> call cv.notify_all() -> the running thread exits from current state -> it calls menu.notify_all() -> the waiting threads awakened and because of the exitFlag it breaks the while loop -> all threads are terminated
      case 120:
        exitFlag = 1;
        cv.notify_all();
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
