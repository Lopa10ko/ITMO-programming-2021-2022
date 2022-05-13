#ifndef ANOTHER_ONE_RUBIK_SOLVER_HPP
#define ANOTHER_ONE_RUBIK_SOLVER_HPP
#include <random>
#include <iostream>
#include "Cube.hpp"
class Solver {
public:
    static std::string Solve(const Cube& cube);
};
#endif