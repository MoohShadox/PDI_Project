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
        IloNumVarArray *p;
        IloIntVarArray *y;
        IloArray<IloNumVarArray> *q;
        IloArray<IloNumVarArray> *I;
        IloArray<IloIntVarArray> *z; 
    //Constructeur principal
    public:
        LSP_Resolution(PRP &p, IloEnv &env);
    //For testing
        void addConstraint(IloExpr& c);
        void generateConstraints();
        void createObjectif();
        void addDistanceToObjectif();
        void printDecisionVariables();
        void printVariables();
};