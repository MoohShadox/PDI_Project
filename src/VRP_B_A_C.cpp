#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Tabu_Cutter.h"
#include <algorithm>
#include "VRP_Resolution.h"
#include <unordered_set>
#include <algorithm>    // std::min

#define epsilon 0.00001

float best_gap = 1;
using namespace std;
using namespace lemon;

ILOMIPINFOCALLBACK1(saving_gap_callback, string, path){
 // append instead of overwrite
    float gap = getMIPRelativeGap();
    if(gap < best_gap){
        std::ofstream outfile;
        outfile.open(path, std::ios_base::app);
        best_gap = gap;
        outfile << getNiterations() << ";" <<getIncumbentObjValue() << ";" << getBestObjValue() <<";" << gap << "\n"; 
        std::cout << "Gep : " << best_gap << std::endl;
    }
}


ILOUSERCUTCALLBACK3(simple_cuts, PRP, prp,
   IloArray<IloIntVarArray>&, x,  IloNumVarArray&, w)
{
    vector<vector<float>> X1;
    X1.resize(prp.n + 1);
    for(int i=0; i<prp.n+1; i++){
        X1[i].resize(prp.n + 1);
    }
    std::cout << "call :  " << prp.n << std::endl;
    ///RECHECK LA MATRICE
    for(int i=0; i<prp.n+1; i++){
        for (int j=0; j<prp.n+1 ; j++){
            if(i != j )
            {
                float v = getValue(x[i][j]);
                X1[i][j] = v;
            }
            else
            {
            X1[i][j] = 0;
            }
        }
    }



    Tabu_Cutter tbc(X1,prp,1);

    srand((unsigned) time(0));
    bool v=false;
    for(int i=0; i<10000 ; i++){
        int k = 1 + (rand() % prp.n);
        vector<int> new_v;
        for (int j=0; j<k; j++){
            int node = (rand() % prp.n);
            new_v.push_back(node);
        }
        v = v || tbc.checkEnsemble(new_v);
    }

    if(v){
        for(auto element=tbc.violated_constraint.begin(); element != tbc.violated_constraint.end(); ++element){
            IloExpr s(getEnv());
            for(pair<int,int> p:element->first){
                //std::cout << "Arc : " << p.first << " , " << p.second <<std::endl;
                s = s + x[p.first][p.second];
                //std::cout << (s==0) << std::endl;
                //s += s + x[p.second][p.first];
            }
            //std::cout << "Fin de El bouclé ! " << s << std::endl;
            IloConstraint cstr = (s >= tbc.bornes.at(element->first));
            std::cout << "VIOL DE CONTRAINTE " << cstr << std::endl;
            add(cstr).end();
        }
    }

}


int main(int argc, char * argv[]){
    string name,nameext, nameextsol;
    if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <PRP file name>   (without .prp)"<<endl;
    return 1;
    }


    name=argv[1];
    nameextsol=name+".prp";

    ifstream fic(nameextsol.c_str());

    if (!fic){
      cerr<<"file "<<nameextsol<<" not found"<<endl;
      return 1;
    }
    PRP prp(fic);
    fic.close();
    printf("prp created\n");
    
    IloNumVarArray* w;
    IloArray<IloIntVarArray>* x;

    IloEnv env2;
    IloEnv* env = &env2; 
    IloModel model(*env);

    w = new IloNumVarArray(*env, prp.n,0.0,prp.Q);
    x = new IloArray<IloIntVarArray>(*env,prp.n+1);
    ostringstream varname;

    IloNumVarArray &wr = *w;
    IloArray<IloIntVarArray> &xr = *x;

    int t = 1;

    for (unsigned i=0; i<prp.n+1; i++){
        IloIntVarArray XR(*env, prp.n+1,0,1);
        xr[i] = XR;
        for  (unsigned j=0; j<prp.n+1; j++)
        {
            varname.str("");
            varname<<"X_"<<i<<"_"<<j;
            xr[i][j].setName(varname.str().c_str());
        }
    }

    for(int i = 0; i < prp.n; i++) {
        varname.str("");
        varname<<"w_"<<i;
        wr[i].setName(varname.str().c_str());
    }

    IloObjective obj=IloAdd(model, IloMinimize(*env, 0.0));

    for (int i=0;i<prp.n + 1;i++)
    {
            for (int j=0;j<prp.n + 1;j++)
	        {
                obj.setLinearCoef(xr[i][j],prp.getDistance(i,j));
            }
    }
    IloExpr s1(*env);
    IloExpr s2(*env);

    for(int i=1; i<prp.n+1; i++){
        s1 = s1 + xr[0][i];
        s2 = s2 + xr[i][0];
    }

    std::cout << "constructed : " << s2 << std::endl;

    model.add(s1 <= prp.Q);

    model.add(s2 <= prp.Q);


    for(int i=1; i<prp.n+1; i++){
        IloExpr in(*env);
        IloExpr out(*env);
        for(int j=0; j<prp.n+1; j++){
            if(i != j){
                in = in + xr[j][i];
                out = out + xr[i][j];
            }

        }
        model.add(in == 1);
        model.add(out == 1);
    }

    for (int i=1; i<prp.n +1 ; i++){
        for(int j=1; j<prp.n +1 ; j++){
            model.add((wr[i-1] - wr[j-1] >= (prp.d[t][i] - (prp.Q + prp.d[t][i])*(1- xr[i][j])) ));
        }
    }

    IloCplex cplex(model);

    cplex.exportModel("sortie2.lp");
    std::ofstream outfile;
    outfile.open("data/gap_VRP.dat");
    cplex.use(simple_cuts(*env, prp , xr, wr));
    cplex.use(saving_gap_callback(*env, "data/gap_VRP.dat"));
//
    if ( !cplex.solve() ) {
        env->error() << "Failed to optimize LP" << endl;
        exit(1);
    }
    env->out() << "Solution status = " << cplex.getStatus() << endl;
    env->out() << "Solution value  = " << cplex.getObjValue() << endl;
    cplex.writeSolution("sol.txt");
    return 0;

}