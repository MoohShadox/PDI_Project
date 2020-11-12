#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"PRP.h"


using namespace std;


int main(int argc,char**argv){

  ostringstream fname;
  string nfic1,nfic2;
  string r;
  int n,l;
  int i,t;
  float hh;
  int b1,b2;

  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <repname>"<<endl;
    return 1;
  }

  fname.str("");
  fname<<argv[1];
  nfic1=fname.str()+"/instances.txt";
  nfic2=fname.str()+"/parameters.txt";

  cout<<"Read "<<nfic1<<endl;

  ifstream fic1(nfic1.c_str());

  if (fic1){
    cerr<<"file "<<nfic1<<" "<<" not found"<<endl;
    return 1;
  }

  do{ fic1>>r;} while (r!=":");
  fic1>>n;
  do{ fic1>>r;} while (r!=":");
  fic1>>l;

  PRP I(n,l);
 
  I.dist=2;

  do{ fic1>>r;} while (r!=":");
  fic1>>I.mc;

  for (i=1;i<=n;i++){
    fic1>>r;
    fic1>>r;
    fic1>>I.xy[i].first;
    fic1>>I.xy[i].second;

    for (t=0;t<l;t++)
      fic1>>I.d[i][t];
  }


  fic1.close();

  cout<<"Read "<<nfic2<<endl;

  ifstream fic2(nfic2.c_str());

  if (!fic2){
    cerr<<"file "<<nfic2<<" "<<" not found"<<endl;
    return 1;
  }

  do{ fic2>>r;} while (r!=":");
  fic2>>I.C;

  do{ fic2>>r;} while (r!=":");
  fic2>>I.L[0];

  do{ fic2>>r;} while (r!=":");
  fic2>>I.f;

  do{ fic2>>r;} while (r!=":");
  fic2>>hh;
  for (i=1;i<=n;i++){
    I.h[i]=hh;
  }

  do{ fic2>>r;} while (r!=":");
  fic2>>I.Q;

  do{ fic2>>r;} while (r!=":");
  fic2>>I.k;

  do{ fic2>>r;} while (r!="from");
  fic2>>b1;fic2>>r;fic2>>b2;
  do{ fic2>>r;} while (r!="=");
  fic2>>hh;
  for (i=b1;i<=b2;i++) I.L[i]=hh;

  do{ fic2>>r;} while (r!="from");
  fic2>>b1;fic2>>r;fic2>>b2;
  do{ fic2>>r;} while (r!="=");
  fic2>>hh;
  for (i=b1;i<=b2;i++) I.L[i]=hh;

  do{ fic2>>r;} while (r!="from");
  fic2>>b1;fic2>>r;fic2>>b2;
  do{ fic2>>r;} while (r!="=");
  fic2>>hh;
  for (i=b1;i<=b2;i++) I.L[i]=hh;

  fic2.close();

  I.xy[0].first=0;
  I.xy[0].second=0;

  I.u=0; // Ajout pour coller a la remarque sur le cout de stockage

  I.h[0]=1; 

  for (i=1;i<=n;i++)
    I.L0[i]=0;
  I.L0[0]=0;
  for (i=1;i<=n;i++)
    I.L0[0]+=I.d[i][0];


  //I.write_screen_txt();

  I.write_instance_file(fname.str());

  return 0;

}
