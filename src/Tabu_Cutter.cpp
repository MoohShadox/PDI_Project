#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Tabu_Cutter.h"
#include <algorithm>


using namespace lemon;

IloNumVar Tabu_Cutter::getVar(ListDigraph::Node i,ListDigraph::Node j){
    return (*X)[(*id)[i]][(*id)[j]];
}

IloNumVar Tabu_Cutter::getVar(ListDigraph::Arc a){
    ListDigraph::Node i = support_graph.source(a);
    ListDigraph::Node j = support_graph.target(a);
    return (*X)[(*id)[i]][(*id)[j]];
}

int Tabu_Cutter::getVal(ListDigraph::Node i,ListDigraph::Node j){
    return (values)[(*id)[i]][(*id)[j]];
}

bool Tabu_Cutter::inNodeVect(vector<ListDigraph::Node> nodes, ListDigraph::Node node){
    for (auto it=nodes.begin(); it!=nodes.end(); ++it){
        if((*id)[node] == (*id)[*it])
            return true;
    }
    return false;
}

vector<ListDigraph::Node> Tabu_Cutter::fromIndexes(vector<int> indexes){
    vector<ListDigraph::Node> nodes;
    for (auto it=indexes.begin(); it != indexes.end(); ++it){
        //std::cout << "On cherche "<< *it << std::endl;
        int j = *it;
        ListDigraph::Node n = *findVal(j);
        //std::cout << "found : " << (*id)[n]  << std::endl;
        nodes.push_back(n);
    }
    return nodes;
}

vector<ListDigraph::Arc> Tabu_Cutter::delta(vector<ListDigraph::Node> nodes){
    vector<ListDigraph::Arc> arcs;
    for(ListDigraph::ArcIt a(support_graph); a!= INVALID; ++a){
        ListDigraph::Node src = support_graph.source(a);
        ListDigraph::Node target = support_graph.target(a);
        //std::cout << "arc : " << (*id)[src] << "--> " << (*id)[target] << std::endl;
        if(inNodeVect(nodes,src) && !inNodeVect(nodes,target)){
                arcs.push_back(a);
                std::cout << (*id)[src] << "--> " << (*id)[target] << std::endl;

        }
        if(!inNodeVect(nodes, src) && inNodeVect(nodes,target)){
                arcs.push_back(a);
                std::cout << (*id)[src] << "--> " << (*id)[target] << std::endl;
        }
    }
    return arcs;
}


int Tabu_Cutter::getVal(ListDigraph::Arc a){
    ListDigraph::Node i = support_graph.source(a);
    ListDigraph::Node j = support_graph.target(a);
    return (values)[(*id)[i]][(*id)[j]];
}

IloNumVarArray Tabu_Cutter::getVars(vector<ListDigraph::Arc> arcs){
    IloNumVarArray arr(env);
    for (auto arc=arcs.begin(); arc!=arcs.end(); ++arc)
        arr.add(getVar(*arc));
    return arr;
}


ListDigraph::Node* Tabu_Cutter::findVal(int val){
    ListDigraph::Node* ptr; 
    //std::cout << "Val in entering : " << val << std::endl;
    for(ListDigraph::NodeIt x(support_graph); x!=INVALID; ++x){
        if((*id)[x] == val)
            {
                ptr = &x;
                //std::cout << "found because id_x = " << (*id)[*ptr] << " and val = " << val << std::endl;
                return ptr;
            }
    }
    return nullptr;
}

int Tabu_Cutter::d_S(vector<ListDigraph::Node> S){
    int s  = 0;
    for(auto node = S.begin(); node != S.end(); ++node){
        s = s+(*d)[*node];
    }
    return s;
}



Tabu_Cutter::Tabu_Cutter(IloArray<IloNumVarArray> x2, vector<vector<int>> x1, PRP prp1, int t, IloEnv env)
{
    this->X = &x2;
    this->values = x1;
    this->prp = &prp1;
    this->env = env;
    id = new ListDigraph::NodeMap<int>(support_graph);
    d = new ListDigraph::NodeMap<int>(support_graph);
    val = new ListDigraph::ArcMap<int>(support_graph);
    dt = 0;
    for (unsigned i=0; i<prp->n + 1; i++)
    {
        ListDigraph::Node x = support_graph.addNode();
        (*id)[x] = i;
        if( i== 0)
            (*d)[x] = 0;
        else
            {
                (*d)[x] = prp->d[i][t];
                dt = dt + prp->d[i][t];
            }
    }

 
    for(ListDigraph::NodeIt x(support_graph); x!=INVALID; ++x){
        for(ListDigraph::NodeIt y(support_graph); y!=INVALID; ++y){
            //std::cout << "val de l'arc " << (*id)[x] << ", " << (*id)[y] << " : " << x1[(*id)[x]][(*id)[y]] << std::endl;
            if(x1[(*id)[x]][(*id)[y]] > 0){
                ListDigraph::Arc arc = support_graph.addArc(x,y);
                (*val)[arc] = x1[(*id)[x]][(*id)[y]];
            }
        }
    }

    std::cout << "Testons d'afficher les labels de ces morts  ! " << std::endl;
    int cpt = 0;
//
    for(ListDigraph::NodeIt n(support_graph); n != INVALID; ++n){
        std::cout << ++cpt << " : " << (*id)[n] << " : " << (*d)[n] << std::endl;
    }
//
    ListDigraph::Node n1 = *findVal(0);
    ListDigraph::Node n2 = *findVal(1);
    ListDigraph::Node n3 = *findVal(2);
    vector<ListDigraph::Node> Vnodes;
    //Vnodes.push_back(n1);
    Vnodes.push_back(n2);
    Vnodes.push_back(n3);

    vector<int> indexes{3,4};

    std::cout << getVars(delta(fromIndexes(indexes))) << std::endl;
    std::cout << "d = " << d_S(fromIndexes(indexes)) << std::endl;
    //this->prp->write_screen_txt();
}



int main(int argc, char * argv[]){
    string name,nameext, nameextsol;
    if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <PRP file name>   (without .prp)"<<endl;
    return 1;
    }


    name=argv[1];
    nameextsol=name+".prp";

    ifstream fic(nameextsol.c_str());

    if (!fic){
      cerr<<"file "<<nameextsol<<" not found"<<endl;
      return 1;
    }
    PRP I(fic);
    fic.close();
    printf("prp created\n");
    IloEnv   env;
    IloCplex cplex(env);
    IloArray<IloNumVarArray> x2(env);

    x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    x2.add(IloNumVarArray(env,5,0,1,ILOINT));


    //Normalement ça je le récupère du solveur.
    vector<vector<int>> X;
    vector<int> v1{0, 1, 0, 1, 1};
    vector<int> v2{0, 0, 1, 0, 0};
    vector<int> v3{1, 0, 0, 0, 0};
    vector<int> v4{1, 0, 0, 0, 0};
    vector<int> v5{1, 0, 0, 0, 0};

    X.push_back(v1);
    X.push_back(v2);
    X.push_back(v3);
    X.push_back(v4);
    X.push_back(v5);
    //

    Tabu_Cutter tbc(x2,X,I,1,env);
}