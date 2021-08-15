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
std::condition_variable cv;
std::condition_variable menu;
int main(){
  initscr();
  noecho();
  start_color();
  cbreak();
  curs_set(0);
  //configuration
  init_color(COLOR_CYAN,55, 150, 131);
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW* bg = newwin(yMax,xMax,0,0);
  refresh();

  box(bg, 0, 0);
  wrefresh(bg);

  std::thread threadKB(asyncInputThread, bg);
  threadKB.detach(); // 'abort signal' solved by calling detaching
  Clock clock1(&cv, &menu, &currentMenu);
  clock1.reloClock(yMax/2-3,xMax/2-14);
  clock1.tickCurrentTime();
  endwin();

  return 0;
}

void asyncInputThread(WINDOW* win){
  while(1){
    keyboardInput = getch();
    switch(keyboardInput){
      case 120:
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
