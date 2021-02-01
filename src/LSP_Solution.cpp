#include "LSP_Solution.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
LSP_Solution::LSP_Solution(IloNumVarArray& pr, IloBoolVarArray& yr, vector<IloNumVarArray>& Ir, IloCplex& cplx){
    p = read_var_array(pr,cplx);
    y = read_bool_array(yr,cplx);
    I.resize(Ir.size());
    for(int i=0; i<Ir.size(); i++){
        I[i] = read_var_array(Ir[i],cplx);
    }

}

float LSP_Solution::computeCost(PRP& prp){
    float cost;
    cost = cost + std::accumulate(p.begin(), p.end(), 0)*prp.u;
    cost = cost + std::accumulate(y.begin(), y.end(), 0)*prp.f;
    for(int t=0; t<prp.l; t++){
        cost = cost + std::accumulate(I[t].begin()+1, I[t].end(), 0)*prp.h[t];
    }
    return cost;
}


vector<float> LSP_Solution::read_var_array(IloNumVarArray arr, IloCplex& cplx){
    vector<float> fl;
    fl.resize(arr.getSize());
    for(int i=0; i<arr.getSize(); i++){
        fl[i] = cplx.getValue(arr[i]);
    }
    return fl;
}

vector<float> LSP_Solution::read_bool_array(IloBoolVarArray arr, IloCplex& cplx){
    vector<float> fl;
    fl.resize(arr.getSize());
    for(int i=0; i<arr.getSize(); i++){
        fl[i] = cplx.getValue(arr[i]);
    }
    return fl;
}