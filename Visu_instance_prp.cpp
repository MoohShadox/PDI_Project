#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"PRP.h"


using namespace std;


int main(int argc,char**argv){

  ostringstream fname;
  string name,nameext;


  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <instance.prp>"<<endl;
    return 1;
  }


  fname.str("");
  fname<<argv[1];
  size_t lastdot = fname.str().find_last_of(".");
  if (lastdot == std::string::npos) {
    name=fname.str();
    fname<<".prp";
    nameext=fname.str();

  }
  else {
    nameext=fname.str();
    name=fname.str();
    name.erase(name.find_last_of("."), std::string::npos); 
  }


  ifstream fic(nameext.c_str());

  if (!fic){
    cerr<<"file "<<nameext<<" not found"<<endl;
    return 1;
  }

  PRP I(fic);

  I.write_screen_txt();

  return 0;

}
