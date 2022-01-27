#include <string.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "options.hh"

OptData::OptData(char _opt,const char *_longopt,const char *_desc)
{
  flag=false;
  fval=false;
  opt=_opt;longopt=_longopt;
  stringstream ss;
  if(opt!=0){
    ss<<"  -"<<opt<<", --"<<longopt;
  }
  odesc=ss.str();
  desc=_desc;
}

void OptData::help(int max1,int max2)
{
  string d1("  "),d2(" ");
  for(int i=0;i<max1-(int)odesc.length();i++){d1+=" ";}
  for(int i=0;i<max2-(int)desc.length();i++){d2+=" ";}
  cerr<<odesc<<d1<<desc<<d2<<def<<endl;
}

bool OptData::match(char c)
{
  if(c==opt){return(true);}
  else{return(false);}
}

bool OptData::match(const char *c)
{
  if(longopt==string(c)){return(true);}
  else{return(false);}
}

Options::~Options()
{
  for(int i=0;i<(int)data.size();i++){
    delete data[i];
  }
}

void Options::help(void)
{
  int max1=0,max2=0;
  cerr<<desc<<endl;
  for(int i=0;i<(int)data.size();i++){
    if(i==0||max1<(int)data[i]->odesc.length()){max1=data[i]->odesc.length();}
    if(i==0||max2<(int)data[i]->desc.length()){max2=data[i]->desc.length();}
  }
  for(int i=0;i<(int)data.size();i++){
    data[i]->help(max1,max2);
  }  
  exit(-1);
}

OptData *Options::search(char c)
{
  for(int i=0;i<(int)data.size();i++){
    if(data[i]->match(c)){return(data[i]);}
  }
  return(NULL);
}

OptData *Options::search(const char *c)
{
  for(int i=0;i<(int)data.size();i++){
    if(data[i]->match(c)){return(data[i]);}
  }
  return(NULL);
}

void Options::parse(void)
{
  int i,j;
  OptData *d;
  char *arg=NULL;
  for(i=1;i<argc;i++){
    if(argv[i][0]=='-'){
      if(argv[i][1]=='\0')break;
      else if(argv[i][1]=='-'){
        if(argv[i][2]=='\0')break;
        else{
          if(strchr(&argv[i][2],'=')){
            d=search(strtok(&argv[i][2],"="));
            arg=strtok(NULL,"=");
          }
          else d=search(&argv[i][2]);
        }
      }
      else{
        j=1;
        while(argv[i][j]!='\0'){
          d=search(argv[i][j]);
          if(d){
            if(d->fval){
              if(argv[i][j+1]!='\0')arg=&argv[i][j+1];
              break;
            }
            d->flag=true;
          }
          else{break;}
          j++;
        }
      }
    }
    else{break;}

    if(!d){
      stringstream ss;
      ss<<"Illegal option "<<argv[i];
      error(ss.str());
    }
    else{
      d->flag=true;
      if(d->fval){
        if(arg){
          d->parse(arg);
        }
        else{
          i++;
          if(i>=argc){
	    stringstream ss;
	    ss<<"Option "<<argv[i-1]<<" requires arguments";
	    error(ss.str());
	  }
          else d->parse(argv[i]);
        }
      }
    }
    arg=NULL;
  }
  argp=i;
}

void Options::error(string message)
{
  cerr<<message<<endl;
  help();
}

bool Options::has_opt(char c)
{
  OptData *d=search(c);
  if(d&&d->flag){return(true);}
  else{return(false);}
}

bool Options::has_opt(const char *c)
{
  OptData *d=search(c);
  if(d&&d->flag){return(true);}
  else{return(false);}
}

void OptLoader::save(ostream& o,int _argc,char **_argv)
{
  o<<'#';
  for(int i=0;i<_argc;++i){
    if(i!=0){o<<'\t';}
    o<<_argv[i];
  }
  o<<std::endl;
}

void OptLoader::load(istream& s)
{
  vector<string> data;
  string line;
  getline(s,line);
  int start=1;
  for(int i=1;i<line.size();++i){
    if(line[i]=='\t'){
      data.push_back(line.substr(start,i-start));
      start=i+1;
    }
  }
  if(start<line.size()){
    data.push_back(line.substr(start,line.size()-start));
  }
  argc=data.size();
  argv=new char*[data.size()];
  for(int i=0;i<data.size();++i){
    argv[i]=strdup(data[i].c_str());
  }
}

OptLoader::~OptLoader()
{
  if(argv){
    for(int i=0;i<argc;++i){
      free(argv[i]);
    }
    delete[] argv;
  }
}

