#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include <lemon/list_graph.h>
#include <lemon/concepts/digraph.h>
#include <set>
using namespace lemon;
#include <map>

class Tabu_Cutter{
    
    public:
        vector<vector<float>> values;
        PRP *prp;
        IloEnv env;
        ListDigraph support_graph;
        int dt;
        int C;
        ListDigraph::NodeMap<float>* id;
        ListDigraph::NodeMap<float>* d;
        ListDigraph::ArcMap<float>* val;

        std::set<vector<int>> checked_ensembles;
        std::map <vector<pair<int,int>>,float> violated_constraint;
        std::map <vector<pair<int,int>>,int> bornes;

        Tabu_Cutter( vector<vector<float>> x1, PRP prp1, int t);

        Tabu_Cutter( IloArray<IloIntVarArray>& x,PRP prp1, int t);

        bool inNodeVect(vector<ListDigraph::Node> nodes, ListDigraph::Node node);

        ListDigraph::Node* findVal(int val);
        vector<ListDigraph::Node> fromIndexes(vector<int> indexes);

        int getVal(ListDigraph::Node i,ListDigraph::Node j);
        int getVal(ListDigraph::Arc a);
        int getVals(vector<ListDigraph::Arc> as);

        void checkComplementary(vector<int> nodes_indexes, int X_d , int d );
        void checkConnexe();

        vector<ListDigraph::Arc> delta(vector<ListDigraph::Node> nodes);
        int d_S(vector<ListDigraph::Node> S);
        bool checkEnsemble(vector<int> nodes_indexes);


};