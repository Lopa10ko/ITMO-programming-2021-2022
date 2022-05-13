#include <algorithm>
#include "../include/Solver.hpp"

std::string Solver::Solve(const Cube& cube) {
    const std::vector<std::string> PERMUTATIONS {
    "F' L' B' R' U' R U' B L F R U R' U",
    "F R B L U L' U B' R' F' L' U' L U'",
    "U2 B U2 B' R2 F R' F' U2 F' U2 F R'",
    "U2 R U2 R' F2 L F' L' U2 L' U2 L F'",
    "U' B2 D2 L' F2 D2 B2 R' U'",
    "U B2 D2 R F2 D2 B2 L U",
    "D' R' D R2 U' R B2 L U' L' B2 U R2",
    "D L D' L2 U L' B2 R' U R B2 U' L2",
    "R' U L' U2 R U' L R' U L' U2 R U' L U'",
    "L U' R U2 L' U R' L U' R U2 L' U R' U",
    "F' U B U' F U B' U'",
    "F U' B' U F' U' B U",
    "L' U2 L R' F2 R",
    "R' U2 R L' B2 L",
    "M2 U M2 U2 M2 U M2"
    };
    const std::vector<std::string> ROTATIONS {"x", "x'", "x2", "y", "y'", "y2"};
    const std::vector<std::string> ORIENTATIONS {"z", "z'", "z2"};
    const std::vector<std::string> SINGLE_MOVES {"U", "U'", "U2", "D", "D'", "D2", "R", "R'", "R2", "L", "L'", "L2", "F", "F'", "F2", "B", "B'", "B2"};
    const int POPULATION_SIZE = 500;
    const int ELITISM_NUM = 50;
    const int MAX_GENERATIONS = 300;
    const int MAX_RESETS = 10;
    std::random_device RD;
    std::mt19937 RNG(RD());
    std::uniform_int_distribution<unsigned long long> RandomPermutationIndex(0, PERMUTATIONS.size() - 1);
    std::uniform_int_distribution<unsigned long long> RandomRotationIndex(0, ROTATIONS.size() - 1);
    std::uniform_int_distribution<unsigned long long> RandomSingleMoveIndex(0, SINGLE_MOVES.size() - 1);
    std::uniform_int_distribution<unsigned long long> RandomOrientationIndex(0, ORIENTATIONS.size() - 1);
    std::uniform_int_distribution<unsigned long long> RandomEvolutionIndex(0, 5);
    std::uniform_int_distribution<unsigned long long> RandomEliteCubeIndex(0, ELITISM_NUM - 1);
    for (int r = 0; r != MAX_RESETS; ++r) {
        std::vector<Cube> cubes;
        cubes.reserve(POPULATION_SIZE);
        for (int i = 0; i != POPULATION_SIZE; ++i) {
            cubes.push_back(cube);
            cubes.back().clearHistory();
            cubes.back().makeMove(SINGLE_MOVES.at(RandomSingleMoveIndex(RNG)));
            cubes.back().makeMove(SINGLE_MOVES.at(RandomSingleMoveIndex(RNG)));
        }
        for (int g = 0; g != MAX_GENERATIONS; ++g) {
            std::sort(cubes.begin(), cubes.end());
            if (g % 20 == 0 and g != 0) {std::cout << "World: " << r + 1 << " - Generation: " << g << std::endl;}
            for (int i = 0; i != cubes.size(); ++i) {
                if (cubes[i].getFitness() == 0) {
                    std::string result;
                    for (const auto &elem: cubes[i].getHistory()) {result += elem + " ";}
                    std::cout << "Solution: " << std::endl << result << std::endl;
                    return result;
                }
                if (i > ELITISM_NUM) {
                    cubes[i] = cubes.at(RandomEliteCubeIndex(RNG));
                    unsigned long long evolution_type = RandomEvolutionIndex(RNG);
                    switch (evolution_type) {
                        case 0: {
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        case 1: {
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        case 2: {
                            cubes[i].makeMove(ROTATIONS.at(RandomRotationIndex(RNG)));
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        case 3: {
                            cubes[i].makeMove(ORIENTATIONS.at(RandomOrientationIndex(RNG)));
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        case 4: {
                            cubes[i].makeMove(ROTATIONS.at(RandomRotationIndex(RNG)));
                            cubes[i].makeMove(ORIENTATIONS.at(RandomOrientationIndex(RNG)));
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        case 5: {
                            cubes[i].makeMove(ORIENTATIONS.at(RandomOrientationIndex(RNG)));
                            cubes[i].makeMove(ROTATIONS.at(RandomRotationIndex(RNG)));
                            cubes[i].execPermutation(PERMUTATIONS.at(RandomPermutationIndex(RNG)));
                            break;
                        }
                        default: {
                            throw std::logic_error("Incorrect evolution index: Your index is " + std::to_string(evolution_type));
                        }
                    }
                }
            }
        }
    }
    return {};
}