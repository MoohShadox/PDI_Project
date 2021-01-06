/**
* @file main.cpp
* @author vss2sn
* @brief The main function that sets up the problem and runs the solution algorithms
*/

#include "cvrp_algorithms.h"
#include <thread>
#include <iostream>
#include <fstream>

/*int main(int argc, char * argv[]){

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

}*/

std::vector<Vehicle> run_instance(PRP prp, std::string type){

  Problem p(prp);

  if(type=="greedy"){
      std::cout << "Greedy: " << std::endl;
      GreedySolution vrp(p);
      vrp.Solve();
      std::cout << std::endl;
      return vrp.vehicles_;
  }else if("Local Search"){
      std::cout << "Local Search (Within each vehicle separately): " << std::endl;
      LocalSearchIntraSolution vrp(p);
      vrp.Solve();
      std::cout << std::endl;
      return vrp.vehicles_;
  }else if("Local Search AV"){
      std::cout << "Local Search (Within all vehicles): " << std::endl;
      LocalSearchInterIntraSolution vrp(p);
      vrp.Solve();
      std::cout << std::endl;
      return vrp.vehicles_;
  }else if("Tabu"){
      std::cout << "Tabu Search: " << std::endl;
      TabuSearchSolution vrp(p, 10);
      vrp.Solve();
      std::cout << std::endl;
      return vrp.vehicles_;
  }else if("Annealing"){
      std::cout << "Simulated Annealing: " << std::endl;
      SimulatedAnnealingSolution vrp(p, 5000000, 5000, 0.9999);
      vrp.Solve();
      std::cout << std::endl;
      return vrp.vehicles_;
  }

    /*
    std::cout << "Genetic Algorithm: " << std::endl;
    GASolution vrp_ga(p, 5, 100);
    vrp_ga.Solve();
    std::cout << std::endl;
    */

}
