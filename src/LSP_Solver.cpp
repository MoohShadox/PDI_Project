#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include "LSP_Resolution.h"
#include "VRP_Resolution.h"
#include "PDI16.h"
#include "Graph.h"
#include "cvrp_algorithms.h"
#include <typeinfo>

#define epsilon 0.00001

using namespace std;


int LSP_Test(){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp"<<" not found"<<endl;
        return 1;
    }
    PRP I(fic);
    I.write_screen_txt();
    IloEnv   env;
    LSP_Resolution LRSP(I,env);
    LRSP.generateConstraints();
    LRSP.createObjectif();
    //LRSP.addDistanceToObjectif();
    LRSP.solve();
    LRSP.printVariables();
    return 0;
}

int VRP_MTZ_Test(){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp"<<" not found"<<endl;
        return 1;
    }
    PRP I(fic);
    std::cout << "n = " << I.n << std::endl;
    I.write_screen_txt();
    IloEnv   env;
    VRP_Resolution VRP(I,env);
    VRP.addingCostObjectif();
    VRP.addingFlowConstraints();
    VRP.addingMTZConstraints(0);
    VRP.solve();
    VRP.printVariables();
    return 1;
}

int test_cvrp(){
    string file = "../PRP_instances/1CVRP_Instance.prp";
    ifstream fic(file);
    if (!fic){
        cerr<<"file "<<file<<" not found"<<endl;
        return 1;
    }
    PRP prp(fic);
    prp.write_screen_txt();
    Solution V = run_instance(prp,"Tabu");
    V.PrintSolution("route");
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
    I.write_screen_txt();
    IloEnv   env;
    vector<vector<float>> SC;
    SC.resize(1);
    vector<float> optim_val;
    float pred_cost = 0;
    for(int j=0; j<20; j++){
        LSP_Resolution LRSP(I,env);
        LRSP.generateConstraints();
        LRSP.createObjectif();
        //LRSP.addDistanceToObjectif();
        if(SC.size() == 1){
            LRSP.addDistanceToObjectif();
        }
        else{
            LRSP.modifyObjCoeffs(SC);
        }
        IloCplex cplx = LRSP.solve();
        LRSP.printVariables();
        vector<IloNumVarArray> &qr = *(LRSP.q);
        SC.resize(I.l);
        float tour_cost = 0;
        bool validity = 1;
        for (unsigned i = 0; i<qr[0].getSize(); i++ ){
            vector<float> v = LRSP.getDelivryAt(i,cplx);
            Problem p = Problem(I,v);
            LocalSearchIntraSolution vrp(p);
            vrp.Solve();
            vrp.PrintSolution("route");
            vector<float> sc_t = vrp.getSC(I);
            SC[i]=sc_t;
            validity = validity && vrp.CheckSolutionValid();
        }
        float obj_val = cplx.getObjValue();
        pred_cost = obj_val;
        optim_val.push_back(pred_cost);
        for(int a=0; a<SC.size(); a++){
            std::cout << "i = " << a  << " SC size = " << SC[a].size() << std::endl;
            for (int b=0; b<SC[a].size(); b++)
            {
                std::cout << SC[a][b] << " - ";
            }
            std::cout << std::endl;
        }
    }
    for(int i=0; i<optim_val.size(); i++){
        std::cout << i << " : " << optim_val[i] << std::endl;
    }
}
