#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Resolution.h"

#pragma once

class LSP_Resolution : public Resolution{
    private:
    //Variables allouées dynamiquement qui contiendrons la résolution du probléme

        IloArray<IloIntVarArray> *z; 
    //Constructeur principal
    public:
        IloNumVarArray *p;
        IloBoolVarArray *y;
        vector<IloNumVarArray> *I;
        LSP_Resolution(PRP &p, IloEnv &env);
    //For testing
        vector<IloNumVarArray> *q;
        void addConstraint(IloExpr& c);
        void generateConstraints();
        void createObjectif();
        void addDistanceToObjectif();
        void printDecisionVariables();
        void printVariables();
        vector<float> getDelivryAt(int step,IloCplex &cplx);
        //Cette fonction modifie les coefficients des contraintes liés aux tournées de distribution
        void modifyObjCoeffs(vector<vector<float>> SC);

};
