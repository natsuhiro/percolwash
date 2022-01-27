#include "game.hh"
#include <cstdlib>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "options.hh"
#include <string>

static void rand_init(void)
{
  struct timeval tv;
  unsigned short s[3];

  gettimeofday(&tv,NULL);
  s[0]=clock()^getpid()^tv.tv_sec^tv.tv_usec;
  gettimeofday(&tv,NULL);
  s[1]=clock()^tv.tv_sec^tv.tv_usec;
  gettimeofday(&tv,NULL);
  s[2]=getpid()^tv.tv_sec^tv.tv_usec;
  seed48(s);
}

int main(int argc,char *argv[])
{
  int width=20,height=15;
  int level=3,maxlim=10;
  std::string diff("normal");
  Options opt(argc,argv,"PERCOLWASH: A Shoot-and-Drop game");
  opt.add_desc("##  Keys   ##");
  opt.add_desc("  Move left:     KEY_LEFT, 'h'");
  opt.add_desc("  Move right:    KEY_RIGHT, 'l'");
  opt.add_desc("  Shoot:         KEY_UP, SPACE, 'k'");
  opt.add_desc("  Exchange next: KEY_DOWN, 'j'");
  opt.add_desc("  New game:      'n'");
  opt.add_desc("  Quit:          'q'");
  opt.add_desc("## Options ##");
  opt.set('s',"size","WIDTH,HEIGHT",width,height,"size of window");
  opt.set('l',"level","{easy,normal,hard}",diff,"Level of game");
  opt.set('m',"maximize","Maximize window");
  opt.set('h',"help","show this help and quit");
  opt.parse();

  if(opt.has_opt('h')){
    opt.help();
    exit(0);
  }

  initscr();
  if(22>COLS||17>LINES){
    std::cerr<<"Too small screen. Give up."<<std::endl;
    endwin();
    opt.help();
    exit(0);
  }
  if(width<20){width=20;}
  if(opt.has_opt('m')||width+2>COLS){width=COLS-2;}
  if(height<15){height=15;}
  if(opt.has_opt('m')||height+2>LINES){height=LINES-2;}
  

  if(diff=="easy"){
    level=1;
    maxlim=10;
  }
  else if(diff=="normal"){
    level=3;
    maxlim=5;
  }
  else{
    level=8;
    maxlim=1;
  }
  
  keypad(stdscr,TRUE);
  nonl();
  cbreak();
  noecho();
  refresh();
  rand_init();

  Game game(0,0,width,height,level,maxlim);
  move(LINES-1,0);

  bool flag=true;
  while(flag){
    int c=getch();
    game.clear_beam();
    switch(c){
    case 'q':
      flag=false;
      break;
    case KEY_LEFT:
    case 'h':
      game.move_left();
      break;
    case KEY_RIGHT:
    case 'l':
      game.move_right();
      break;
    case KEY_UP:
    case 'k':
    case ' ':
      if(game.shoot()<4){
	game.grow();
      }
      break;
    case 'n':
      game.initialize();
      move(LINES-1,0);      
      continue;
    case KEY_DOWN:
    case 'j':
      game.exchange();
      break;
    }
    if(game.check_win()){
      game.create_subwin();      
      game.youwin();
      if(getch()=='q'){
	flag=false;
      }
      else{
	game.delete_subwin();	
	game.difficulty();
	game.initialize();
      }
    }
    else if(game.check_lose()){
      game.beam();
      game.create_subwin();
      game.youlose();
      if(getch()=='q'){
	flag=false;
      }
      else{
	game.delete_subwin();
	game.initialize();
      }
      
    }
    else{
      game.beam();
    }
    move(LINES-1,0);
  }

  endwin();

  return(0);
}
