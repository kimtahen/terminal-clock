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

void asyncInputThread();

char keyboardInput = 'A';
std::condition_variable cv;
int main(){
  initscr();
  noecho();
  start_color();
  curs_set(0);
  //configuration
  init_color(COLOR_CYAN,55, 150, 131);
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW* bg = newwin(yMax,xMax,0,0);
  refresh();



  box(bg, 0, 0);
  wrefresh(bg);

  std::thread threadKB(asyncInputThread);
  threadKB.detach(); // 'abort signal' solved by calling detaching

  Clock clock1(&cv);
  clock1.reloClock(yMax/2-3,xMax/2-14);
  clock1.tickCurrentTime();

  endwin();

  return 0;
}

void asyncInputThread(){
  while(1){
    keyboardInput = getch();
    if(keyboardInput == 'x'){
      cv.notify_all();
    }
  }

  return;
}
