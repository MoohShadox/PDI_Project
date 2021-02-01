
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Resolution.h"

#pragma once

class LSP_Solution {
    public:
        //IloCplex* cplx;
        vector<float> p;
        vector<float> y;
        vector<vector<float>> I;
        LSP_Solution(IloNumVarArray &pr, IloBoolVarArray &yr, vector<IloNumVarArray>& Ir, IloCplex& cplx2);
        vector<float> read_var_array(IloNumVarArray arr, IloCplex& cplx);
        vector<float> read_bool_array(IloBoolVarArray arr, IloCplex& cplx);
        float computeCost(PRP &prp);

};