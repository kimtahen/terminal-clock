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

int keyboardInput;
int currentMenu = 0;
int exitFlag = 0;
int pressFlag = 0;
int stwCounter = 0;
int timCounter = 0;
//cv for terminating each thread
std::condition_variable cv;
//menu for rotating menu
std::condition_variable menu;
//buttons for stopwatch
std::condition_variable stop;
std::condition_variable reset;
int main(int argc, char * const * argv){
  int velocity = 1;
  int op;
  while((op=getopt(argc,argv,"v:"))!=-1){
    switch(op){
      case 'v':
        velocity = std::stoi(optarg);
        if(velocity < 1 || velocity >1000){
          std::cout<<"invalid option"<<std::endl;
          return -1;
        }
        break;
      case '?':
        std::cout<<"Unknown flag : "<<optopt<<std::endl;
        break;
    }
  }
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


  //clock start
  Clock clock1(velocity);
  clock1.tickCurrentTime();
  endwin();

  return 0;
}
