/**
* @file main.hpp
* @author vss2sn
* @brief Just includes header files.
*/

#include "genetic_algorithm.h"
#include "greedy.h"
#include "local_search_intra.h"
#include "local_search_inter_intra.h"
#include "tabu_search.h"
#include "simulated_annealing.h"

/**
* @brief Main function
* @return int 0
* @details Main function that creates the problem and calls the various solve functions for each of the solution algorithms
*/
int main(int argc, char * argv[]);

std::vector<Vehicle> run_instance(PRP prp, std::string type);
