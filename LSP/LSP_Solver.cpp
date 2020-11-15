#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include "LSP_Resolution.h"


#define epsilon 0.00001

using namespace std;

int main(){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/A_014_ABS1_15_1.prp");
    if (!fic){
    cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/A_014_ABS1_15_1.prp"<<" not found"<<endl;
    return 1;
    }
    PRP I(fic);
    IloEnv   env;
    LSP_Resolution LRSP(I,env);
    LRSP.generateConstraints();
    LRSP.createObjectif();
    LRSP.solve();
    //LRSP.printVariables();
}