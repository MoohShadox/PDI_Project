#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "PRP.h"
#include "LSP_Resolution.h"
#include "VRP_Resolution.h"
#include "Graph.h"
#include "cvrp_algorithms.h"

#define epsilon 0.00001

using namespace std;


int LSP_Test(){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1Test_instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/1Test_instance.prp"<<" not found"<<endl;
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
     ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1Test_instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/A_014_ABS1_15_1.prp"<<" not found"<<endl;
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

    string file = "../PRP_instances/A_014_ABS1_15_1.prp";

  ifstream fic(file);

  if (!fic){
  cerr<<"file "<<file<<" not found"<<endl;
  return 1;
  }

  PRP prp(fic);

  #ifdef VISUALIZE
    rclcpp::init(argc, argv);
    mpp = std::make_shared<MinimalPublisher>();
    std::thread thread_ros = std::thread([&]{rclcpp::spin(mpp);rclcpp::shutdown();});
    thread_ros.detach();
  #endif // VISUALIZE

  run_instance(prp,"greedy");

  #ifdef VISUALIZE
    rclcpp::shutdown();
  #endif //VISUALIZE

}

/*

int main (int argc, char**argv){
    ifstream fic("/home/mohamed/Bureau/MAOA_Project/PRP_instances/1Test_instance.prp");
    if (!fic){
        cerr<<"file "<<"/home/mohamed/Bureau/MAOA_Project/PRP_instances/A_014_ABS1_15_1.prp"<<" not found"<<endl;
        return 1;
    }
    PRP I(fic);
    I.write_screen_txt();
    C_Graph graph;
    graph.construct_from_prp(I);
    graph.write_dot_G("random_name");
    test_cvrp();
}

*/
