#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>

#pragma once


class Resolution{
    public:
        IloEnv* env;
        PRP* prp;
        IloModel* model;
        IloObjective obj;
        IloRangeArray *contraintes; 
        IloCplex *cplx; 
    //Constructeur principal
        Resolution(PRP &p, IloEnv &env);
        IloCplex solve();
        void addConstraintName(ostringstream &varname,IloConstraint &constraint);
};