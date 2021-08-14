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

void asyncInputThread();

char keyboardInput = 'A';
std::condition_variable cv;
int main(){
  initscr();
  noecho();
  start_color();
  curs_set(0);
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW* bg = newwin(yMax,xMax,0,0);
  WINDOW* win[6];
  for(int i = 0; i<6; i++){
    win[i] = newwin(5,3,1,i*3+i+1);
  }
  refresh(); //refresh the entire screen so that the ncurses knows that the new window was created

  box(bg, 0, 0);

  Clock clock1(&cv, win);
  std::thread threadKB(asyncInputThread);
  threadKB.detach(); // 'abort signal' solved by calling detaching
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
