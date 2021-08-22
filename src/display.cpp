#include "../lib/includes/display.hpp"
#include "../lib/includes/draw.hpp"

Display::Display(){
  ele_pos[0]={0,0};
  ele_pos[1]={0,4};
  ele_pos[2]={0,8};
  ele_pos[3]={0,10};
  ele_pos[4]={0,14};
  ele_pos[5]={0,18};
  ele_pos[6]={0,20};
  ele_pos[7]={0,24};
  ele_pos[8]={0,28};
  ele_pos[9]={0,30};
  ele_pos[10]={0,34};
  ele_pos[11]={0,38};
  

  for(int i = 0; i<12; i++){
    if(i==2 || i==5 || i==8){
      win[i]=newwin(5,1,ele_pos[i].row+crit.row,ele_pos[i].col+crit.col);
    }
    else{
      win[i]=newwin(5,3,ele_pos[i].row+crit.row,ele_pos[i].col+crit.col);
    }
  }
  refresh(); //refresh the entire screen so that the ncurses knows that the new window was created
}

void Display::reloClock(int row, int col){
  crit = {row, col};
  for(int i = 0; i<12; i++){
    werase(win[i]);
    wrefresh(win[i]);
    mvwin(win[i], ele_pos[i].row+crit.row, ele_pos[i].col+crit.col);
  }
}

void Display::clr(){
  for(int i = 0; i<11; i++){
    werase(win[i]);
    wrefresh(win[i]);
  }
}

void Display::displayClk(tm* timeinfo, int prevhour, int prevmin, int prevsec){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  int flag = 1;
  if(timeinfo->tm_hour != prevhour || flag){
    d.fgArray(timeinfo->tm_hour/10, tmp);
    d.fgWindowDraw(win[0],tmp, 1);
    d.fgArray(timeinfo->tm_hour%10, tmp);
    d.fgWindowDraw(win[1],tmp, 1);
    d.dotWindowDraw(win[2],dot, 1);
    d.dotWindowDraw(win[5],dot, 1);
  }

  if(timeinfo->tm_min != prevmin || flag){
    d.fgArray(timeinfo->tm_min/10, tmp);
    d.fgWindowDraw(win[3],tmp, 1);
    d.fgArray(timeinfo->tm_min%10, tmp);
    d.fgWindowDraw(win[4],tmp, 1);
  }
  d.fgArray(timeinfo->tm_sec/10, tmp);
  d.fgWindowDraw(win[6],tmp,1);
  d.fgArray(timeinfo->tm_sec%10, tmp);
  d.fgWindowDraw(win[7],tmp,1);
  
}

void Display::displayStw(int hr, int min, int sec, int dec){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  d.fgArray(hr/10, tmp);
  d.fgWindowDraw(win[0],tmp, 2);
  d.fgArray(hr%10, tmp);
  d.fgWindowDraw(win[1],tmp, 2);
  d.dotWindowDraw(win[2],dot, 2);
  d.dotWindowDraw(win[5],dot, 2);
  d.dotWindowDraw(win[8],dot, 2);
  d.fgArray(min/10, tmp);
  d.fgWindowDraw(win[3],tmp, 2);
  d.fgArray(min%10, tmp);
  d.fgWindowDraw(win[4],tmp, 2);
  d.fgArray(sec/10, tmp);
  d.fgWindowDraw(win[6],tmp,2);
  d.fgArray(sec%10, tmp);
  d.fgWindowDraw(win[7],tmp,2);
  d.fgArray(dec/100, tmp);
  d.fgWindowDraw(win[9],tmp,2);
  d.fgArray((dec%100)/10, tmp);
  d.fgWindowDraw(win[10],tmp,2);
  d.fgArray((dec%100)%10, tmp);
  d.fgWindowDraw(win[11],tmp,2);

}

void Display::displayTim(int hr, int min, int sec, int dec){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  d.fgArray(hr/10, tmp);
  d.fgWindowDraw(win[0],tmp, 3);
  d.fgArray(hr%10, tmp);
  d.fgWindowDraw(win[1],tmp, 3);
  d.dotWindowDraw(win[2],dot, 3);
  d.dotWindowDraw(win[5],dot, 3);
  d.dotWindowDraw(win[8],dot, 3);
  d.fgArray(min/10, tmp);
  d.fgWindowDraw(win[3],tmp, 3);
  d.fgArray(min%10, tmp);
  d.fgWindowDraw(win[4],tmp, 3);
  d.fgArray(sec/10, tmp);
  d.fgWindowDraw(win[6],tmp,3);
  d.fgArray(sec%10, tmp);
  d.fgWindowDraw(win[7],tmp,3);
  d.fgArray(dec/100, tmp);
  d.fgWindowDraw(win[9],tmp,3);
  d.fgArray((dec%100)/10, tmp);
  d.fgWindowDraw(win[10],tmp,3);
  d.fgArray((dec%100)%10, tmp);
  d.fgWindowDraw(win[11],tmp,3);
}
void Display::displayTim(int hr, int min, int sec, int dec, int cursor){
  int tmp[5][3];
  int dot[5][1] = {{0},{1},{0},{1},{0}};
  int hrC = cursor==0 ? 1 : 3;
  int minC = cursor==1 ? 1 : 3;
  int secC = cursor==2 ? 1 : 3;

  d.fgArray(hr/10, tmp);
  d.fgWindowDraw(win[0],tmp, hrC);
  d.fgArray(hr%10, tmp);
  d.fgWindowDraw(win[1],tmp, hrC);
  d.dotWindowDraw(win[2],dot, 3);
  d.dotWindowDraw(win[5],dot, 3);
  d.dotWindowDraw(win[8],dot, 3);
  d.fgArray(min/10, tmp);
  d.fgWindowDraw(win[3],tmp, minC);
  d.fgArray(min%10, tmp);
  d.fgWindowDraw(win[4],tmp, minC);
  d.fgArray(sec/10, tmp);
  d.fgWindowDraw(win[6],tmp,secC);
  d.fgArray(sec%10, tmp);
  d.fgWindowDraw(win[7],tmp,secC);
  d.fgArray(dec/100, tmp);
  d.fgWindowDraw(win[9],tmp,3);
  d.fgArray((dec%100)/10, tmp);
  d.fgWindowDraw(win[10],tmp,3);
  d.fgArray((dec%100)%10, tmp);
  d.fgWindowDraw(win[11],tmp,3);
}

void Display::cursor(int index){
  int tmp[5][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  switch(index){
    case 0:
      d.fgWindowDraw(win[0],tmp,2);
      d.fgWindowDraw(win[1],tmp,2);
      break;
    case 1:
      d.fgWindowDraw(win[3],tmp,2);
      d.fgWindowDraw(win[4],tmp,2);
      break;
    case 2:
      d.fgWindowDraw(win[6],tmp,2);
      d.fgWindowDraw(win[7],tmp,2);
      break;
    case 3:
      d.fgWindowDraw(win[9],tmp,2);
      d.fgWindowDraw(win[10],tmp,2);
      d.fgWindowDraw(win[11],tmp,2);
      break;
  } 
}



