#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "./lib/includes/clock.hpp"

void asyncInputThread();

char keyboardInput = 'A';
std::condition_variable cv;
int main(){
  initscr();
  noecho();
  cbreak();
  Clock clock1(&cv);
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
