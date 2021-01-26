#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include <lemon/list_graph.h>
#include <lemon/concepts/digraph.h>
using namespace lemon;

class Tabu_Cutter{
    
    public:
        IloArray<IloNumVarArray> *X;
        vector<vector<int>> values;
        PRP *prp;
        IloEnv env;
        ListDigraph support_graph;
        int dt;

        ListDigraph::NodeMap<int>* id;
        ListDigraph::NodeMap<int>* d;
        ListDigraph::ArcMap<int>* val;

        set<vector<int>> checked_ensembles; 

        Tabu_Cutter(IloArray<IloNumVarArray> x2, vector<vector<int>> x1, PRP prp1, int t, IloEnv env);

        bool inNodeVect(vector<ListDigraph::Node> nodes, ListDigraph::Node node);

        ListDigraph::Node* findVal(int val);
        vector<ListDigraph::Node> fromIndexes(vector<int> indexes);

        int getVal(ListDigraph::Node i,ListDigraph::Node j);
        int getVal(ListDigraph::Arc a);

        IloNumVar getVar(ListDigraph::Node i,ListDigraph::Node j);
        IloNumVar getVar(ListDigraph::Arc a);
        IloNumVarArray getVars(vector<ListDigraph::Arc> arcs);

        vector<ListDigraph::Arc> delta(vector<ListDigraph::Node> nodes);
        int d_S(vector<ListDigraph::Node> S);


};