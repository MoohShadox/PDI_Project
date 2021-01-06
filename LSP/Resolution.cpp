
#include "Resolution.h"

Resolution::Resolution(PRP &p1, IloEnv &env1){
    prp = &p1;
    env = &env1;
    model = new IloModel(*env);
    obj= IloAdd(*model, IloMinimize(*env, 0.0));
    contraintes = new IloRangeArray(*env);
}

void Resolution::solve(){
    IloRangeArray &cstr = *contraintes; 
    IloCplex cplex(*model);
    if ( !cplex.solve() ) {
        env->error() << "Failed to optimize LP" << endl;
        exit(1);
    }
    env->out() << "Solution status = " << cplex.getStatus() << endl;
    env->out() << "Solution value  = " << cplex.getObjValue() << endl;
    cplex.exportModel("sortie.lp");
    cplx = &cplex;
}