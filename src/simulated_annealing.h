/**
* @file simulated_annealing.hpp
* @author vss2sn
* @brief Contains the SimulatedAnnealingSolution class
*/

#ifndef SA_HPP
#define SA_HPP

#include "utils.h"

class SimulatedAnnealingSolution : public Solution{
public:
  /**
  * @brief Constructor
  * @param nodes Vector of nodes
  * @param vehicles Vector of vehicles
  * @param distanceMatrix Matrix containing distance between each pair of nodes
  * @param stag_limit Number of iterations without an improvement to the best solution
  * @param init_temp Initial temperature
  * @param cooling_rate tempertaure multipluies with this variable to get new temperature at every iteration
  * @return No return parameter
  * @details Constructor for initial setup of problem, and solution using Simulated Annealing Algorithm
  */
  SimulatedAnnealingSolution(std::vector<Node> nodes,
                             std::vector<Vehicle> vehicles,
                             std::vector<std::vector<double>> distanceMatrix,
                             int stag_limit  = 500000,
                             double init_temp = 5000,
                             double cooling_rate = 0.9999);

  /**
  * @brief Constructor
  * @param p Instance of Problem class defining the problem parameters
  * @param stag_limit Number of iterations without an improvement to the best solution
  * @param init_temp Initial temperature
  * @param cooling_rate tempertaure multipluies with this variable to get new temperature at every iteration
  * @return No return parameter
  * @details Constructor for initial setup of problem, and solution using Simulated Annealing Algorithm
  */
  SimulatedAnnealingSolution(Problem p,
                             int stag_limit  = 500000,
                             double init_temp = 5000,
                             double cooling_rate = 0.9999);

  /**
  * @brief Constructor
  * @param s Instance of Solution class containing a valid solution and problem parameters
  * @param stag_limit Number of iterations without an improvement to the best solution
  * @param init_temp Initial temperature
  * @param cooling_rate tempertaure multipluies with this variable to get new temperature at every iteration
  * @return No return parameter
  * @details Constructor for initial setup of problem, and solution using Simulated Annealing Algorithm
  */
  SimulatedAnnealingSolution(Solution s,
                             int stag_limit  = 500000,
                             double init_temp = 5000,
                             double cooling_rate = 0.9999);

  /**
  * @brief Function called to solve the given problem using a simulated annealing algorithm
  * @return void
  * @details Generates random iniitial solutions. Applies selected algorithm. Prints cost of best solution, and its validity.
  */
  void Solve();
private:
  double temp_ = 0, cooling_rate_, best_cost_, current_cost_;
  int max_temp_, n_reheats_ = 20, stag_limit_, stag_;
  /**
  * @brief Checks whether the move is allowable
  * @param delta change in cost of solution due to move
  * @return bool True if move accepted
  * @details Allows move if it improves the solution; also allows move based on probability and temperature at the given time.
  */
  inline bool AllowMove(double delta);
};

#endif // SA_HPP
