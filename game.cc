#include "game.hh"
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <iomanip>

Game::Game(int x,int y,int width,int height,int level,int maxlim)
{
  gettimeofday(&start_,0);
  pattern_="#$&%X";
  stone_=(double)level/10.0;    
  
  #ifdef USE_COLOR
  if(has_colors()){
    start_color();
    use_default_colors();
    /*
    init_pair(1,COLOR_BLACK,COLOR_CYAN);
    init_pair(2,COLOR_RED,COLOR_CYAN);
    init_pair(3,COLOR_BLUE,COLOR_CYAN);
    init_pair(4,COLOR_MAGENTA,COLOR_CYAN);
    init_pair(5,COLOR_YELLOW,COLOR_CYAN);
    */

    init_pair(1,COLOR_BLACK,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_BLUE);
    init_pair(4,COLOR_BLACK,COLOR_YELLOW);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);
    
    init_pair(6,COLOR_BLACK,COLOR_CYAN);    
    

    for(int i=0;i<5;++i){
      cmap_.insert(std::pair<char,int>(pattern_[i],i+1));
    }
    cmap_.insert(std::pair<char,int>(' ',6));
    cmap_.insert(std::pair<char,int>('|',6));
    cmap_.insert(std::pair<char,int>('0',6));    
  }
  #endif
  subwin_=NULL;
  width_=width;
  height_=height;
  win_=subwin(stdscr,height_+2,width_+2,y,x);
  depth_=height_*3/5;
  maxlim_=maxlim;
  stage_=1;
  initialize();
}


void Game::initialize(void)
{
  gettimeofday(&stime_,0);
  int pat[2][width_+1];
  int prev=0,next=1;
  
  current_=pattern_[(int)(drand48()*4)];
  next_=pattern_[(int)(drand48()*4)];
  posx_=width_/2;
  limit_=maxlim_+1;
  score_=0;

  wclear(win_);
  wstandout(win_);
  box(win_,'|','-');
  wstandend(win_);

  for(int i=1;i<=width_;++i){
    for(int j=depth_+1;j<=height_;++j){
      set_char(' ',i,j);
    }
    pat[prev][i]=1;
  }

  bool flag;
  for(int j=depth_;j>=1;--j){
    double s=stone_;
    if(j<=3){s*=0.6;}
    do{
      flag=false;
      for(int i=1;i<=width_;++i){
	if(j!=1&&drand48()<s){
	  set_char(pattern_[4],i,j);
	  pat[next][i]=0;
	}
	else{
	  set_char(pattern_[(int)(drand48()*4)],i,j);
	  if((i>1&&pat[prev][i-1]==1)||
	     (pat[prev][i]==1)||
	     (i<width_&&pat[prev][i+1]==1)){
	    pat[next][i]=1;
	    flag=true;
	  }
	  else{
	    pat[next][i]=0;
	  }
	}
      }
    }while(!flag);
    next^=1;
    prev^=1;
  }

  wstandout(win_);
  wmove(win_,0,width_/2-3);
  wprintw(win_,"stage:%d",stage_);
  wstandend(win_);
  grow();
  next_ball();
  score();
  beam();
}


int Game::search_match(char c,int x,int y)
{
  if(get_char(x,y)!=c){return(0);}
  int ans=1;
  if(y==1){++ans;}
  set_char('0',x,y);  
  for(int i=x-1;i<=x+1;++i){
    for(int j=y-1;j<=y+1;++j){
      if(i>=1&&i<=width_&&j>=1&&j<=width_){
	ans+=search_match(c,i,j);
      }
    }
  }
  return(ans);
}

void Game::recover_match(char c,int x,int y)
{
  if(get_char(x,y)!='0'){return;}
  set_char(c,x,y);
  for(int i=x-1;i<=x+1;++i){
    for(int j=y-1;j<=y+1;++j){
      if(i>=1&&i<=width_&&j>=1&&j<=width_){
	recover_match(c,i,j);
      }
    }
  }
}

void Game::beam(void)
{
  set_char(current_,posx_,height_);
  for(int j=height_-1;j>=1;--j){
    if(get_char(posx_,j)!=' '){break;}
    set_char('|',posx_,j);
  }
  wmove(win_,0,0);
  wrefresh(win_);
}

void Game::clear_beam(void)
{
  set_char(' ',posx_,height_);
  for(int j=height_-1;j>=1;--j){
    if(get_char(posx_,j)=='|'){
      set_char(' ',posx_,j);
    }
  }
}

void Game::move_left(void)
{
  if(posx_>1){
    --posx_;
  }
}

void Game::move_right(void)
{
  if(posx_<width_){
    ++posx_;
  }
}

void Game::score(void)
{
  wstandout(win_);
  wmove(win_,height_+1,1);
  wprintw(win_,"score:%d",score_);
  wstandend(win_);
}

void Game::next_ball(void)
{
  /*
  wmove(win_,height_+1,width_/2-2);
  wstandout(win_);
  wprintw(win_,"next:");
  wstandend(win_);
  set_char(next_,width_/2+3,height_+1);
  */
  char w[]="next";
  wstandout(win_);
  for(int i=0;i<4;++i){
    wmove(win_,height_-4+i,0);
    waddch(win_,w[i]);
  }
  wstandend(win_);
  set_char(next_,0,height_);


}

int Game::shoot(void)
{
  int j;
  for(j=height_;j>1;--j){
    if(get_char(posx_,j-1)!=' '){
      break;
    }
  }
  set_char(current_,posx_,j);
  if(search_match(current_,posx_,j)<3){
    recover_match(current_,posx_,j);
  }
  search_drop();
  int ans=animate_drop();
  if(ans>0){
    score_+=ans;
    score();
  }
  current_=next_;
  next_=pattern_[(int)(drand48()*4)];
  next_ball();
  return(ans);
}

void Game::exchange(void)
{
  clear_beam();
  char tmp=current_;
  current_=next_;
  next_=tmp;
  next_ball();
}

void Game::search_drop(void)
{
  for(int i=1;i<=width_;++i){
    for(int j=2;j<=height_;++j){
      if(get_char(i,j)!=' '&&get_char(i,j-1)=='0'){
	set_char('0',i,j);
      }
    }
  }
}

int Game::animate_drop(void)
{
  int num=1,sc=0;
  while(num>0){
    num=0;
    for(int i=1;i<=width_;++i){
      if(get_char(i,height_)=='0'){
	set_char(' ',i,height_);
	++num;
      }
      for(int j=height_;j>1;--j){
	if(get_char(i,j-1)=='0'){
	  set_char(' ',i,j-1);
	  if(j!=height_){
	    set_char('0',i,j);
	    ++num;
	  }
	  else{
	    ++sc;
	  }
	}
      }
    }
    wrefresh(win_);
    usleep(10000);
  }
  return(sc+sc/10*(100/(maxlim_+1)+(int)(stone_*100.0)));
}

void Game::search_top(void)
{
  for(int i=1;i<=width_;++i){
    if(get_char(i,1)!=' '&&
       (i==1||get_char(i-1,1)==' ')&&
       (i==width_||get_char(i+1,1)==' ')){
      set_char('0',i,1);
    }
  }
}

bool Game::check_win(void)
{
  for(int i=1;i<=width_;++i){
    if(get_char(i,1)!=' '){
      return(false);
    }
  }
  return(true);
}

bool Game::check_lose(void)
{
  for(int i=1;i<=width_;++i){
    if(get_char(i,height_)!=' '){
      return(true);
    }
  }
  return(false);
}

void Game::create_subwin(void)
{
  delete_subwin();
  subwin_=subwin(win_,8,width_-2,height_/2-3,2);
  wclear(subwin_);
  wstandout(subwin_);
  box(subwin_,'|','-');
  wstandend(subwin_);
  struct timeval now;
  gettimeofday(&now,0);
  long gtime=now.tv_sec-stime_.tv_sec;
  long gmin=gtime/60,ghour=gmin/60,gsec=gtime%60;

  std::stringstream gstr;
  if(ghour>0){
    gstr<<"Time "<<ghour<<':'<<std::setw(2)<<std::setfill('0')<<gmin<<':'<<gsec;
  }
  else{
    gstr<<"Time "<<gmin<<':'<<std::setw(2)<<std::setfill('0')<<gsec;    
  }
  wmove(subwin_,3,(width_-2)/2-gstr.str().size()/2);
  waddstr(subwin_,gstr.str().c_str());
  
  long ttime=now.tv_sec-start_.tv_sec;
  long tmin=ttime/60,thour=tmin/60,tsec=ttime%60;

  std::stringstream tstr;
  if(thour>0){
    tstr<<"Total "<<thour<<':'<<std::setw(2)<<std::setfill('0')<<tmin<<':'<<tsec;
  }
  else{
    tstr<<"Total "<<tmin<<':'<<std::setw(2)<<std::setfill('0')<<tsec;    
  }
  wmove(subwin_,4,(width_-2)/2-tstr.str().size()/2);
  waddstr(subwin_,tstr.str().c_str());
  
  wmove(subwin_,5,(width_-2)/2-5);
  waddstr(subwin_,"Hit Any Key");
}

void Game::delete_subwin(void)
{
  if(subwin_){delwin(subwin_);}
  subwin_=NULL;
}

void Game::youwin(void)
{
  wmove(subwin_,2,(width_-2)/2-4);
  waddstr(subwin_,"You Win!!");
  move(LINES-1,0);
  wrefresh(subwin_);
  ++stage_;
}

void Game::youlose(void)
{
  wmove(subwin_,2,(width_-2)/2-5);
  waddstr(subwin_,"You Lose!!");
  move(LINES-1,0);
  wrefresh(subwin_);
}

void Game::grow(void)
{
  if(limit_==0){
    for(int i=1;i<=width_;++i){
      if(drand48()<0.6){
	int j;
	for(j=height_;j>1;--j){
	  if(get_char(i,j-1)!=' '){
	    break;
	  }
	}
	if(j>1||drand48()<0.01){
	  set_char(pattern_[(int)(drand48()*4)],i,j);
	}
      }
    }
    limit_=maxlim_;
  }
  else{
    --limit_;
  }
  wstandout(win_);
  wmove(win_,height_+1,width_-7);
  wprintw(win_,"limit:%2d",limit_);
  wstandend(win_);
}
