#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>


void asyncInputThread();
void clkDisplayThread();

char keyboardInput = 'A';
std::condition_variable cv;
class Clock {
private:
  time_t _time;
  time_t now;
  tm* ltm = localtime(&now);
public:
  Clock(){
    refresh();
  }
  void update(){
    time(&_time);
  }
  tm* currentTime(){
    update();
    struct tm* timeinfo = localtime(&_time);
    return timeinfo;
  }
  /*tm* tickCurrentTime(){
    sleep(1);
    update();
    struct tm* timeinfo = localtime(&_time);
    return timeinfo;
  }*/
void clkDisplayThread(){
  std::mutex mtx;
  std::unique_lock<std::mutex> lck(mtx);
  while(cv.wait_for(lck,std::chrono::seconds(1))==std::cv_status::timeout){
    printw("%s",asctime(currentTime()));
    refresh();
  }
}

  void tickCurrentTime(){
    std::thread threadClk([&](){clkDisplayThread();});
    threadClk.join();
  }
};


int main(){
  initscr();
  noecho();
  Clock clock1;
  char key;
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
