#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
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
    IloModel model(env);
    IloNumVar x = IloNumVar(env, 0.0, 5.0, ILOINT);
    IloNumVar y = IloNumVar(env, 0.0, 10.0, ILOINT);
    x.setName("X");
    y.setName("Y");
    IloObjective obj=IloAdd(model, IloMaximize(env, 0.0));
    obj.setLinearCoef(x,5);

    obj.setLinearCoef(y,3);
    IloRangeArray CC(env);
	IloExpr c1(env);
    c1+=x+y;
    CC.add(c1<=5);
    model.add(CC);
    IloCplex cplex(model);
    I.write_screen_txt();
}