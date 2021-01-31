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

void reoptimise(LSP_Resolution LRSP, PRP I, IloCplex cplx){
    vector<IloNumVarArray> &qr = *(LRSP.q);
    vector<vector<float>> SC;
    SC.resize(I.l);
    for (unsigned i = 0; i<qr[0].getSize(); i++ ){
        vector<float> v = LRSP.getDelivryAt(i,cplx);
        Problem p = Problem(I,v);
        TabuSearchSolution vrp(p, 3);
        vrp.Solve();
        vector<float> sc_t = vrp.getSC(I);
        SC[i]=sc_t;
        std::cout << "Return of the function ! i = " << i << std::endl;
    }
    std::cout << "Analyse of the constructed result : " << std::endl;
    std::cout << "Size of SC : "<< SC.size() << std::endl;
    for(int i=0; i< SC.size(); i++)
    {
        std::cout << "Size of SC["<< i <<"]: "<< SC[i].size() << std::endl;
        for(int j=0; j<SC[i].size(); j++)
        {
            std::cout << SC[i][j] << " - ";
        }
        std::cout << std::endl;
    }
    LRSP.modifyObjCoeffs(SC);
    LRSP.solve();
    LRSP.printVariables();
}

void testing_reoptimze(){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/1LSP_Instance.prp"<<" not found"<<endl;
    }
    PRP I(fic);
    I.write_screen_txt();
    IloEnv   env;
    LSP_Resolution LRSP(I,env);
    LRSP.generateConstraints();
    LRSP.createObjectif();
    LRSP.addDistanceToObjectif();
    IloCplex cplx = LRSP.solve();
    LRSP.printVariables();
}




int main_lsp_solver(int argc, char * argv[]){
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
    fic.close();
    printf("prp created\n");
    IloEnv   env;
   // Solution V = run_instance(prp,"Tabu");
   // std::cout << "" << std::endl;
}
