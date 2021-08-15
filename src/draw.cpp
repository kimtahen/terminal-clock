#include "../lib/includes/draw.hpp"
#include <ncurses.h>
void arr_cp(int from[][3], int to[][3]){
  for(int i = 0; i<5; i++)
    for(int j = 0; j<3; j++)
      to[i][j] = from[i][j];
}
Draw::Draw(){
  init_pair(1,COLOR_CYAN, COLOR_CYAN);
  init_pair(2,COLOR_BLUE, COLOR_BLUE);
}
void Draw::fgArray(int t, int tmp[][3]){
    int f0[5][3] = {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}};
    int f1[5][3] = {{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
    int f2[5][3] = {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}};
    int f3[5][3] = {{1,1,1},{0,0,1},{1,1,1},{0,0,1},{1,1,1}};
    int f4[5][3] = {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}};
    int f5[5][3] = {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}};
    int f6[5][3] = {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}};
    int f7[5][3] = {{1,1,1},{1,0,1},{1,0,1},{0,0,1},{0,0,1}};
    int f8[5][3] = {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}};
    int f9[5][3] = {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}};


  switch(t){
    case 0:
      arr_cp(f0, tmp);
      break;
    case 1:
      arr_cp(f1, tmp);
      break;
    case 2:
      arr_cp(f2, tmp);
      break;
    case 3:
      arr_cp(f3, tmp);
      break;
    case 4:
      arr_cp(f4, tmp);
      break;
    case 5:
      arr_cp(f5, tmp);
      break;
    case 6:
      arr_cp(f6, tmp);
      break;
    case 7:
      arr_cp(f7, tmp);
      break;
    case 8:
      arr_cp(f8, tmp);
      break;
    case 9:
      arr_cp(f9, tmp);
      break;
  }
}
void Draw::fgWindowDraw(WINDOW* win, int tmp[][3]){
  werase(win);
  for(int i = 0; i<5; i++){
    for(int j=0; j<3; j++){
      wattron(win, COLOR_PAIR(1));
      if(tmp[i][j])
        mvwaddch(win, i, j, 'A');
      wattroff(win,COLOR_PAIR(1));
    }
  }
  wrefresh(win);
}
void Draw::dotWindowDraw(WINDOW* win, int tmp[][1]){
  werase(win);
  for(int i = 0; i<5; i++){
    for(int j=0; j<1; j++){
      wattron(win, COLOR_PAIR(1));
      if(tmp[i][j])
        mvwaddch(win, i, j, 'A');
      wattroff(win,COLOR_PAIR(1));
    }
  }
  wrefresh(win);
}
