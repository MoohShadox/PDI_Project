#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Resolution.h"


class VRP_Resolution : public Resolution{
    
    public:
        VRP_Resolution(PRP &p, IloEnv &env);
        IloArray<IloIntVarArray> *x; 
        IloNumVarArray *w; 
        //Adding constraints methods
        void addingFlowConstraints();
        void addingMTZConstraints(int timestep);
        void addingCostObjectif();
        void printVariables();
};