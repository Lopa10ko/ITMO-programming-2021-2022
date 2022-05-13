#include <iostream>
#include "../include/Cube.hpp"
#include "../include/Solver.hpp"

int main() {
//    for (int i = 0; i != 5; ++i) {
//        Cube cub;
//        cub.shuffle();
//        Solver::Solve(cub);
//    }
    Cube cube("WWWWWWWWWOOOOOOOOBGGGGGGYGGRRRRRRYRRBBBBBBYBBRYOYYYYYG");
    Solver::Solve(cube);
    return 0;
}