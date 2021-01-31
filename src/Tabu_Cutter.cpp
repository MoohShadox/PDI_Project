#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include <vector>
#include "Tabu_Cutter.h"
#include <algorithm>
#include "VRP_Resolution.h"
#include <unordered_set>

#include <algorithm>    // std::min

#define epsilon 0.00001

using namespace std;
using namespace lemon;



//IloNumVar Tabu_Cutter::getVar(ListDigraph::Node i,ListDigraph::Node j){
//    return (*X)[(*id)[i]][(*id)[j]];
//}

bool Tabu_Cutter::checkEnsemble(vector<int> nodes_indexes){
    sort(nodes_indexes.begin(), nodes_indexes.end());
    auto it = find(checked_ensembles.begin(), checked_ensembles.end(), nodes_indexes) ;
    if(it == checked_ensembles.end()){
        //std::cout << "d: " << d_S(fromIndexes(nodes_indexes)) << std::endl;
        //std::cout << "C: " << C << std::endl;
        //std::cout << "X(delta(S)): " << getVals(delta(fromIndexes(nodes_indexes)))<< std::endl;
        int d = d_S(fromIndexes(nodes_indexes));
        vector<ListDigraph::Arc> delt = delta(fromIndexes(nodes_indexes));
        int X_d = getVals(delt);
        int veh = (int) (d / C) + 1;
        //std::cout << "Testing : " << 2*veh << "<=" << X_d << std::endl; 
        if(2*veh > X_d ){
            //std::cout << "CONTRAINTE VIOLEE !! "  << std::endl;
            //IloConstraint cst = (IloSum(getVars(delta(fromIndexes(nodes_indexes)))) >= 2*veh);
            //violated_constraint = cst;
            //std::cout << "Contrainte : " << cst <<std::endl;
            for (ListDigraph::Arc ar:delt){
                pair<int,int> p((*id)[support_graph.source(ar)],(*id)[support_graph.target(ar)]);
                violated_constraint.push_back(p);
            }
            borne = 2*veh;
            return true;
        }
        else{
            //std::cout << "Checked "<< std::endl;
            checked_ensembles.insert(nodes_indexes);
        }
    }
    return false;
}


//IloNumVar Tabu_Cutter::getVar(ListDigraph::Arc a){
//    ListDigraph::Node i = support_graph.source(a);
//    ListDigraph::Node j = support_graph.target(a);
//    std::cout << "Parcourons X on sait jamais.. " << std::endl;
//    std::cout << "X = " << (*X).getSize() << std::endl;
//    return (*X)[(*id)[i]][(*id)[j]];
//}
//
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
                //std::cout << (*id)[src] << "--> " << (*id)[target] << std::endl;

        }
        if(!inNodeVect(nodes, src) && inNodeVect(nodes,target)){
                arcs.push_back(a);
                //std::cout << (*id)[src] << "--> " << (*id)[target] << std::endl;
        }
    }
    return arcs;
}


int Tabu_Cutter::getVal(ListDigraph::Arc a){
    ListDigraph::Node i = support_graph.source(a);
    ListDigraph::Node j = support_graph.target(a);
    return (values)[(*id)[i]][(*id)[j]];
}


int Tabu_Cutter::getVals(vector<ListDigraph::Arc> as){
    int sum = 0;
    for (auto a:as)
        sum += getVal(a);
    return sum;
}

//IloNumVarArray Tabu_Cutter::getVars(vector<ListDigraph::Arc> arcs){
//    IloNumVarArray arr(env);
//    std::cout << "On test le get Vars ! " << std::endl;
//    std::cout << "Size  of arcs " << arcs.size() <<std::endl;
//    for (ListDigraph::Arc arc:arcs)
//        {
//            std::cout << "In the arc : " << (*id)[support_graph.source(arc)] << " -- > " << (*id)[support_graph.target(arc)] << std::endl;
//            arr.add(getVar(arc));
//        }
//    return arr;
//}


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



Tabu_Cutter::Tabu_Cutter(vector<vector<float>> x1, PRP prp1, int t)
{
    this->values = x1;
    this->prp = &prp1;
    id = new ListDigraph::NodeMap<float>(support_graph);
    d = new ListDigraph::NodeMap<float>(support_graph);
    val = new ListDigraph::ArcMap<float>(support_graph);
    C = prp->Q;
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

    //std::cout << "Testons d'afficher les labels de ces morts  ! " << std::endl;
    //int cpt = 0;
////
    //for(ListDigraph::NodeIt n(support_graph); n != INVALID; ++n){
    //    std::cout << ++cpt << " : " << (*id)[n] << " : " << (*d)[n] << std::endl;
    //}
////
    //ListDigraph::Node n1 = *findVal(0);
    //ListDigraph::Node n2 = *findVal(1);
    //ListDigraph::Node n3 = *findVal(2);
    //vector<ListDigraph::Node> Vnodes;
    ////Vnodes.push_back(n1);
    //Vnodes.push_back(n2);
    //Vnodes.push_back(n3);
//
    //vector<int> indexes{1,2};
//
    ////std::cout << getVals(delta(fromIndexes(indexes))) << std::endl;
    ////std::cout << "d = " << d_S(fromIndexes(indexes)) << std::endl;
//
    //checkEnsemble(indexes);
//
    //this->prp->write_screen_txt();
}


int test_VRP(PRP prp){
    IloNumVarArray* w;
    IloArray<IloIntVarArray>* x;

    IloEnv env2;
    IloEnv* env = &env2; 
    IloModel model(*env);

    w = new IloNumVarArray(*env, prp.n,0.0,prp.Q);
    x = new IloArray<IloIntVarArray>(*env,prp.n+1);
    ostringstream varname;

    IloNumVarArray &wr = *w;
    IloArray<IloIntVarArray> &xr = *x;
    int t = 1;
    for (unsigned i=0; i<prp.n+1; i++){
        IloIntVarArray XR(*env, prp.n+1,0,1);
        xr[i] = XR;
        for  (unsigned j=0; j<prp.n+1; j++)
        {
            varname.str("");
            varname<<"X_"<<i<<"_"<<j;
            xr[i][j].setName(varname.str().c_str());
        }
    }

    for(int i = 0; i < prp.n; i++) {
        varname.str("");
        varname<<"w_"<<i;
        w->operator[](i).setName(varname.str().c_str());
    }

    IloRangeArray CC(*env);
    IloExpr exp2(*env);
    IloExpr exp1(*env);
    for (int j=1; j< prp.n; j++){
        exp1 = exp1 + xr[0][j];
        exp2 = exp2 + xr[j][0];
    }
    varname.str("");
    varname<<"C1";
    IloRange cst1(*env, -IloInfinity,exp1, prp.k, varname.str().c_str() );

    varname.str("");
    varname<<"C2";
    IloRange cst2(*env, -IloInfinity,exp1, prp.k, varname.str().c_str() );

    CC.add(cst1);
    CC.add(cst2);

    //On genère une contrainte par client 
    for (int client=1; client< prp.n+1; client++){
        IloExpr expr1(*env);
        IloExpr expr2(*env);
        for (int voisin=1; voisin< prp.n+1; voisin++){
            if(client != voisin){
                expr1 = expr1 + xr[client][voisin];
                expr2 = expr2 + xr[voisin][client];
            }
        }
        varname.str("");
        varname<<"Out(" << client << ")";
        IloRange cst3(*env, 1,expr1, 1, varname.str().c_str() );
        varname.str("");
        varname<<"In(" << client << ")";
        IloRange cst4(*env, 1,expr2, 1, varname.str().c_str() );
        CC.add(cst3);
        CC.add(cst4);
    }

    IloObjective obj= IloAdd(model, IloMinimize(*env, 0.0));
    for (unsigned i=1; i<prp.n+1; i++){
        for  (unsigned j=1; j<prp.n+1; j++)
        {
            obj.setLinearCoef(xr[i][j],prp.getDistance(i,j));
        }
    }

    for (unsigned i=0; i<prp.n; i++){
        for(unsigned j=0; j< prp.n; j++)
        {
            //std::cout << "i = " << i << " j = " << j << " t=" << t <<std::endl;
            if(i == j) continue;
            IloExpr exp1(*env);
            //std::cout << "Test : " << prp->d[0][i+1]<< std::endl;
            exp1 = wr[i] - wr[j] - prp.d[i+1][t] + (prp.d[i+1][t]+prp.Q)*(1-xr[i+1][j+1]);

            varname.str("");
            varname<<"MTZ(" << i << " "<< j <<") : ";
            IloRange cst(*env, 0,exp1, IloInfinity, varname.str().c_str() );
            CC.add(cst);
        }
    }

    model.add(CC);
    IloCplex cplex(model);

    if ( !cplex.solve() ) {
        env->error() << "Failed to optimize LP" << endl;
        exit(1);
    }
    env->out() << "Solution status = " << cplex.getStatus() << endl;
    env->out() << "Solution value  = " << cplex.getObjValue() << endl;

    cplex.exportModel("sortie2.lp");
    cplex.writeSolution("sol.txt");


    return 0;
}


int test(int argc, char * argv[]){
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
    IloEnv   env;
    IloCplex cplex(env);
    IloArray<IloNumVarArray> x2(env);
    ostringstream varname;

    for (int i=0; i<I.n + 1 ; i++){
        IloNumVarArray xi(env);
        for (int j=0; j<I.n + 1; j++){
            varname.str("");
            varname <<"x_"<< i << "_" << j ;
            IloNumVar v = IloNumVar(env, 0, 1, ILOINT);
            v.setName(varname.str().c_str());
            xi.add(v);
        }
        x2.add(xi);
    }

    //x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    //x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    //x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    //x2.add(IloNumVarArray(env,5,0,1,ILOINT));
    //x2.add(IloNumVarArray(env,5,0,1,ILOINT));


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
    I.write_screen_txt();
    test_VRP(I);

    //Tabu_Cutter tbc(X,I,1);
    //vector<int> toCheck{1,2};
    //tbc.checkEnsemble(toCheck);
    return 0;
}







ILOLAZYCONSTRAINTCALLBACK3(simple_cuts, PRP, prp,
   IloArray<IloIntVarArray>&, x,  IloNumVarArray&, w)
{
    vector<vector<float>> X1;
    X1.resize(prp.n + 1);
    for(int i=0; i<prp.n+1; i++){
        X1[i].resize(prp.n + 1);
    }
    std::cout << "call :  " << prp.n << std::endl;
    ///RECHECK LA MATRICE
    for(int i=0; i<prp.n+1; i++){
        for (int j=0; j<prp.n+1 ; j++){
            if(i != j )
            {
                //std::cout << i << ", " << j << std::endl;
                float v = getValue(x[i][j]);
                X1[i][j] = v;
            }
            else
            {
            X1[i][j] = 0;
            }
        }
    }

    for(int i=1; i<prp.n+1; i++){
        //std::cout << w[i-1] << " : " << getValue(w[i-1]) <<std::endl;
    }



    Tabu_Cutter tbc(X1,prp,1);
    vector<int> toCheck{2,3,4};
    if(tbc.checkEnsemble(toCheck)){
        for(int i=0; i<prp.n+1; i++){
        for (int j=0; j<prp.n+1 ; j++){
            if(i != j && getValue(x[i][j])>0 )
            {
                //std::cout << i << "," << j << ": " <<  getValue(x[i][j]) << std::endl;
            }
            }
        }
        IloExpr s(getEnv());
        for(pair<int,int> p:tbc.violated_constraint){
            //std::cout << "Arc : " << p.first << " , " << p.second <<std::endl;
            s = s + x[p.first][p.second];
            //std::cout << (s==0) << std::endl;
            //s += s + x[p.second][p.first];
        }
        //std::cout << "Fin de El bouclé ! " << s << std::endl;
        IloConstraint cstr = (s >= tbc.borne);
        std::cout << "VIOL DE CONTRAINTE " << cstr << std::endl;
        add(cstr).end();
    }

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
    PRP prp(fic);
    fic.close();
    printf("prp created\n");
    
    IloNumVarArray* w;
    IloArray<IloIntVarArray>* x;

    IloEnv env2;
    IloEnv* env = &env2; 
    IloModel model(*env);

    w = new IloNumVarArray(*env, prp.n,0.0,prp.Q);
    x = new IloArray<IloIntVarArray>(*env,prp.n+1);
    ostringstream varname;

    IloNumVarArray &wr = *w;
    IloArray<IloIntVarArray> &xr = *x;

    int t = 1;

    for (unsigned i=0; i<prp.n+1; i++){
        IloIntVarArray XR(*env, prp.n+1,0,1);
        xr[i] = XR;
        for  (unsigned j=0; j<prp.n+1; j++)
        {
            varname.str("");
            varname<<"X_"<<i<<"_"<<j;
            xr[i][j].setName(varname.str().c_str());
        }
    }

    for(int i = 0; i < prp.n; i++) {
        varname.str("");
        varname<<"w_"<<i;
        wr[i].setName(varname.str().c_str());
    }

    IloObjective obj=IloAdd(model, IloMinimize(*env, 0.0));

    for (int i=0;i<prp.n + 1;i++)
    {
            for (int j=0;j<prp.n + 1;j++)
	        {
                obj.setLinearCoef(xr[i][j],prp.getDistance(i,j));
            }
    }
    IloExpr s1(*env);
    IloExpr s2(*env);

    for(int i=1; i<prp.n+1; i++){
        s1 = s1 + xr[0][i];
        s2 = s2 + xr[i][0];
    }

    std::cout << "constructed : " << s2 << std::endl;

    model.add(s1 <= prp.Q);

    model.add(s2 <= prp.Q);


    for(int i=1; i<prp.n+1; i++){
        IloExpr in(*env);
        IloExpr out(*env);
        for(int j=0; j<prp.n+1; j++){
            if(i != j){
                in = in + xr[j][i];
                out = out + xr[i][j];
            }

        }
        model.add(in == 1);
        model.add(out == 1);
    }

    for (int i=1; i<prp.n +1 ; i++){
        for(int j=1; j<prp.n +1 ; j++){
            model.add((wr[i-1] - wr[j-1] >= (prp.d[t][i] - (prp.Q + prp.d[t][i])*(1- xr[i][j])) ));
        }
    }

    IloCplex cplex(model);

    cplex.exportModel("sortie2.lp");

    cplex.use(simple_cuts(*env, prp , xr, wr));
//
    if ( !cplex.solve() ) {
        env->error() << "Failed to optimize LP" << endl;
        exit(1);
    }
    env->out() << "Solution status = " << cplex.getStatus() << endl;
    env->out() << "Solution value  = " << cplex.getObjValue() << endl;
    cplex.writeSolution("sol.txt");
    return 0;

}