#ifndef GAME_HH_
#define GAME_HH_

#ifdef LIB_NCURSES
#include <ncurses.h>
#elif defined(LIB_CURSES)
#include <curses.h>
#endif
#include <string>
#include <map>
#include <sys/time.h>

class Game{
  WINDOW *win_,*subwin_;
  std::string pattern_;
  std::map<char,int> cmap_;
  int depth_;
  double stone_;
  int width_,height_;
  char current_,next_;
  int score_;
  int stage_;
  int posx_;
  int limit_,maxlim_;
  struct timeval start_,stime_;;
 public:
  Game(int x,int y,int width,int height,int level,int maxlim);
  ~Game(){
    if(win_){delwin(win_);}
  }
  char get_char(int x,int y){
    return(mvwinch(win_,y,x)&A_CHARTEXT);
  }
  void set_char(char c,int x,int y){
    #ifdef USE_COLOR
    if(cmap_.find(c)==cmap_.end()){
      wcolor_set(win_,0,NULL);
    }
    else{
      wcolor_set(win_,cmap_[c],NULL);
    }
    #endif

    wmove(win_,y,x);
    waddch(win_,c);
  }
  void initialize(void);
  void difficulty(void){
    if(stone_<0.8){stone_*=1.1;}
    if(maxlim_>1){--maxlim_;}
  }
  int search_match(char c,int x,int y);
  void recover_match(char c,int x,int y);
  void beam(void);
  void clear_beam(void);
  void move_left(void);
  void move_right(void);
  void score(void);
  void next_ball(void);
  int shoot(void);
  void exchange(void);
  void search_drop(void);
  int animate_drop(void);
  void search_top(void);
  bool check_win(void);
  bool check_lose(void);
  void create_subwin(void);
  void delete_subwin(void);
  void youwin(void);
  void youlose(void);
  void grow(void);
};


#endif
