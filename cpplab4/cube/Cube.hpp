#ifndef ANOTHER_ONE_RUBIK_CUBE_HPP
#define ANOTHER_ONE_RUBIK_CUBE_HPP
#include <sstream>
#include <vector>
#include <list>
#include <random>
#include <iostream>
#include <set>
#include "enums.hpp"
class Cube {
public:
    /*
     * [0 -  8]  --   TOP
     * [9 - 17]  --   LEFT
     * [18- 26]  --   FRONT
     * [27- 35]  --   RIGHT
     * [36- 44]  --   BACK
     * [45- 53]  --   DOWN
     * */

    /*
     *             0  1  2
     *             3  4  5
     *             6  7  8
     *           |----------|
     *   9 10 11 | 18 19 20 | 27 28 29 | 36 37 38
     *  12 13 14 | 21 22 23 | 30 31 32 | 39 40 41
     *  15 16 17 | 24 25 26 | 33 34 35 | 42 43 44
     *           |----------|
     *             45 46 47
     *             48 49 50
     *             51 52 53
     */
    explicit Cube(std::string cubestring);
    Cube();
    template<typename stream>
    friend stream& operator<<(stream& out, const Cube& arg) {
        for (int i = 0; i != 3; ++i) {
            out << "      ";
            for (int j = 0; j != 3; ++j) {
                out << arg.m_CubeString[3 * i + j] << ' ';
            }
            out << std::endl;
        }
        for (int j = 0; j != 3; ++j) {
            for (int i = 1; i != 5; ++i) {
                out << arg.m_CubeString[9 * i + 3 * j] << ' ' << arg.m_CubeString[9 * i + 3 * j + 1] << ' '
                    << arg.m_CubeString[9 * i + 3 * j + 2] << ' ';
            }
            out << std::endl;
        }
        for (int i = 0; i != 3; ++i) {
            out << "      ";
            for (int j = 0; j != 3; ++j) {
                out << arg.m_CubeString[45 + 3 * i + j] << ' ';
            }
            out << std::endl;
        }
        return out;
    }
    template<typename stream>
    friend stream& operator>>(stream& in, Cube& arg) {
        std::string input;
        in >> input;
        try {
            Cube temp(input);
            arg = temp;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            throw;
        }
        return in;
    }
    bool operator<(const Cube& other) const {return this->getFitness() < other.getFitness();}
    void makeMove(const std::string &move);
    void execPermutation (const std::string& permutation);
    [[nodiscard]] int getFitness() const;
    void clearHistory();
    [[nodiscard]] const std::list<std::string>& getHistory() const;
    void shuffle();
    void reset();
private:
    std::string m_CubeString;
    std::list<std::string> m_History;
    void rotateSide(RotationType times, Side side);
    void rotateCube(RotationType times, Axis axis);
    void rotateDown(RotationType times, Column column);
    void rotateAcross(RotationType times, Row row);
    void rotateAcrossWithTopSide(RotationType times, Row row);
    bool isStateCorrect();
};
#endif