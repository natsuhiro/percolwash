#ifndef _OPTIONS_HH_
#define _OPTIONS_HH_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Options;

class OptData{
  friend class Options;
 protected:
  char opt;
  string longopt;
  bool flag,fval;
  string odesc,desc,def;
 public:
  OptData(char _opt,const char* _longopt,const char *_desc);
  virtual ~OptData(){}
  void help(int max1,int max2);
  bool match(char c);
  bool match(const char *c);
  virtual void parse(const char *c){}
};

template <typename T>
class OptDataC:public OptData{
 protected:
  T *data;
 public:
  OptDataC(char _opt,const char* _longopt,const char *vname,T& _data,const char *_desc)
    :OptData(_opt,_longopt,_desc){
    fval=true;
    data=&_data;
    stringstream ss;
    ss<<"(default: "<<_data<<")";
    def=ss.str();
    ss.str(vname);
    ss<<"="<<vname;
    odesc+=ss.str();
  }
  void parse(const char *c){
    stringstream ss(c);
    ss>>*data;
  }
};

template <>
class OptDataC<std::string>:public OptData{
 protected:
  std::string *data;
 public:
  OptDataC(char _opt,const char* _longopt,const char *vname,std::string& _data,const char *_desc)
    :OptData(_opt,_longopt,_desc){
    fval=true;
    data=&_data;
    stringstream ss;
    ss<<"(default: "<<_data<<")";
    def=ss.str();
    ss.str(vname);
    ss<<"="<<vname;
    odesc+=ss.str();
  }
  void parse(const char *c){
    (*data)=c;
  }
};

template <typename T,typename M>
class OptDataM:public OptData{
 protected:
  T *data1;
  M *data2;
 public:
  OptDataM(char _opt,const char* _longopt,const char *vname,T& _data1,M& _data2,const char *_desc)
    :OptData(_opt,_longopt,_desc){
    fval=true;
    data1=&_data1;
    data2=&_data2;
    stringstream ss;
    ss<<"(default: "<<_data1<<","<<_data2<<")";
    def=ss.str();
    ss.str(vname);
    ss<<"="<<vname;
    odesc+=ss.str();
  }
  void parse(const char *c){
    std::string s(c);
    stringstream ss1,ss2;
    int pos=s.find(',',0);
    ss1.str(s.substr(0,pos));
    ss1>>*data1;
    ss2.str(s.substr(pos+1,s.size()-1));
    ss2>>*data2;
  }
};

class OptLoader{
 public:  
  int argc;
  char **argv;
  OptLoader():argv(0){}
  ~OptLoader();
  void save(ostream& o,int _argc,char **_argv);
  void load(istream& s);
};

class Options{
 protected:
  string desc;
  vector<OptData*> data;
  int argc,argp;
  char **argv;
  OptData *search(char c);
  OptData *search(const char *c);
  OptLoader ol;
 public:
  Options(int _argc,char **_argv,const char *d){argc=_argc;argv=_argv;desc=d;}
  ~Options();
  void add_desc(const char *d){
    desc+="\n";
    desc+=d;
  }
  void save(ostream& o){
    ol.save(o,argc,argv);
  }
  void load(istream& s){
    ol.load(s);
    argc=ol.argc;
    argv=ol.argv;
    parse();
  }
  template <typename T> void set(char opt,const char *longopt,const char *vname,T& d,const char *desc){
    data.push_back(new OptDataC<T>(opt,longopt,vname,d,desc));
  }
  template <typename T,typename M> 
    void set(char opt,const char *longopt,const char *vname,T& d1,M& d2,const char *desc){
    data.push_back(new OptDataM<T,M>(opt,longopt,vname,d1,d2,desc));    
  }
  void set(char opt,const char *longopt,const char *desc){
    data.push_back(new OptData(opt,longopt,desc));    
  }
  void set(const char *desc){
    data.push_back(new OptData(0,"",desc));
  }
  void help(void);
  void parse(void);
  void error(string message);
  bool has_opt(char c);
  bool has_opt(const char *c);
  char *get_arg(int n){
    if(argp+n>=argc){return(NULL);}
    else{return(argv[argp+n]);}
  }
};





#endif
