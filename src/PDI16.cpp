#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>    // std::min
#include"PRP.h"
#include "PDI16.h"

PDI16::PDI16(PRP &p1, IloEnv &env1):Resolution(p1,env1){

    //Used for naming 
    ostringstream varname;
    //

    IloNumVarArray pr(*env); 
    IloBoolVarArray yr(*env); 
    p = pr;
    y = yr;
    //Creating p and y
    for(int t = 0; t < prp->l; t++){
        varname.str("");
        varname <<"p_"<< t+1 ;
        IloNumVar v = IloNumVar(*env, 0, prp->C);
        v.setName(varname.str().c_str());
        p.add(v);

        varname.str("");
        varname <<"y_"<< t+1 ;
        IloBoolVar v2 = IloBoolVar(*env);
        v2.setName(varname.str().c_str());
        y.add(v2);
    }

    IloArray<IloNumVarArray> Ir(*env);
    I = Ir;

    //Creating I [0..n][0..l] -> (n+1, l+1)

    for (int i=0; i<prp->n+1; i++){
        IloNumVarArray Ii(*env);
        for (int t=0; t<prp->l+1;t++){
            //IloNumVar v = 
            //varname.str("");
            //varname <<"I_"<< i << "_" << t ;
            //v.setName(varname.str().c_str());
        }
    }


    std::cout << "Model Created ! " << std::endl;
    IloCplex cplex(*model);
    cplex.exportModel("sortie.lp");
}