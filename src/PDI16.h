#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Resolution.h"

#pragma once

class PDI16 : public Resolution{
    public:
        IloNumVarArray p; //Production quantity at period t
        IloBoolVarArray y; //Production launched at period t
        IloArray<IloNumVarArray> I; //Inventory at node i at period t


        PDI16(PRP &p, IloEnv &env);
        //void addConstraint(IloExpr& c);
        //void generateConstraints();
        //void createObjectif();
        //void printDecisionVariables();
        //void printVariables();
};