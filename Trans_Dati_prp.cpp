#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"PRP.h"


using namespace std;


int main(int argc,char**argv){

  ostringstream fname;
  string name,nameext;
  string r;
  int n,l;
  int i,t;
  float hh;
  int b1,b2;

  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <name.dat>"<<endl;
    return 1;
  }

  fname.str("");
  fname<<argv[1];
  size_t lastdot = fname.str().find_last_of(".");
  if (lastdot == std::string::npos) {
    cout<<"name.dat please"<<endl;
  }
  else {
    nameext=fname.str();
    name=fname.str();
    name.erase(name.find_last_of("."), std::string::npos); 
  }


  cout<<"Read "<<nameext<<endl;

  ifstream fic(nameext.c_str());

  if (!fic){
    cerr<<"file "<<nameext<<" "<<" not found"<<endl;
    return 1;
  }

  do{ fic>>r;} while (r!="DIMENSION:");
  fic>>n;
  n--;
  do{ fic>>r;} while (r!="HORIZON:");
  fic>>l;

  PRP I(n,l);
 
  I.dist=1;

  do{ fic>>r;} while (r!="CAPACITY:");
  fic>>I.Q;

  do{ fic>>r;} while (r!="PROD._COST");
  fic>>r;
  fic>>I.xy[0].first;
  fic>>I.xy[0].second;
  fic>>I.L0[0]; 
  fic>>I.h[0];
  fic>>I.u; 
  fic>>I.f;

  do{ fic>>r;} while (r!="ABS._INV._COST");
  for (i=1;i<=n;i++){
    fic>>r;
    fic>>I.xy[i].first;
    fic>>I.xy[i].second;
    fic>>I.L0[i];
    fic>>I.L[i];
    fic>>r;
    fic>>I.d[i][0];
    for (t=1;t<l;t++) I.d[i][t]=I.d[i][0];
    fic>>I.h[i];
  }

  I.L[0]=1E10;  // Ajout
  I.C=1E10;  // Ajout





  //I.write_screen_txt();

  I.write_instance_file(name);

  return 0;

}
