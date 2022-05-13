#include <algorithm>
#include "../include/Cube.hpp"

void Cube::rotateSide(RotationType times, Side side) {
    for (int i = 0; i != to_underlying(times); ++i) {
        std::swap(m_CubeString[9 * to_underlying(side)], m_CubeString[9 * to_underlying(side) + 2]);
        std::swap(m_CubeString[9 * to_underlying(side) + 1], m_CubeString[9 * to_underlying(side) + 5]);
        std::swap(m_CubeString[9 * to_underlying(side)], m_CubeString[9 * to_underlying(side) + 8]);
        std::swap(m_CubeString[9 * to_underlying(side) + 3], m_CubeString[9 * to_underlying(side) + 7]);
        std::swap(m_CubeString[9 * to_underlying(side) + 1], m_CubeString[9 * to_underlying(side) + 7]);
        std::swap(m_CubeString[9 * to_underlying(side)], m_CubeString[9 * to_underlying(side) + 6]);
    }
}
void Cube::rotateCube(RotationType times, Axis axis) {
    for (int j = 0; j != to_underlying(times); ++j)
        switch (axis) {
            case Axis::x: {
                for (int i = 0; i != 9; ++i) {std::swap(m_CubeString[i], m_CubeString[18 + i]);}
                for (int i = 18; i != 27; ++i) {std::swap(m_CubeString[i], m_CubeString[27 + i]);}
                std::reverse(m_CubeString.begin() + 36, m_CubeString.begin() + 45);
                std::reverse(m_CubeString.begin() + 45, m_CubeString.begin() + 54);
                for (int i = 36; i != 45; ++i) {std::swap(m_CubeString[i], m_CubeString[9 + i]);}
                rotateSide(RotationType::Anticlockwise, Side::Left);
                rotateSide(RotationType::Clockwise, Side::Right);
                break;
            }
            case Axis::y: {
                for (int k = 1; k != 4; ++k) {
                    for (int i = 0; i != 9; ++i) {std::swap(m_CubeString[9 * k + i], m_CubeString[9 * (k + 1) + i]);}
                }
                rotateSide(RotationType::Clockwise, Side::Top);
                rotateSide(RotationType::Anticlockwise, Side::Down);
                break;
            }
            case Axis::z: {
                for (int i = 0; i != 9; ++i) {std::swap(m_CubeString[i], m_CubeString[i + 9]);}
                for (int i = 9; i != 18; ++i) {std::swap(m_CubeString[i], m_CubeString[i + 36]);}
                for (int i = 27; i != 36; ++i) {std::swap(m_CubeString[i], m_CubeString[i + 18]);}
                rotateSide(RotationType::Clockwise, Side::Front);
                rotateSide(RotationType::Anticlockwise, Side::Back);
                rotateSide(RotationType::Clockwise, Side::Right);
                rotateSide(RotationType::Clockwise, Side::Top);
                rotateSide(RotationType::Clockwise, Side::Down);
                rotateSide(RotationType::Clockwise, Side::Left);
                break;
            }
            default: {throw std::logic_error("Incorrect axis (something probably went wrong)");}
        }
}
void Cube::rotateDown(RotationType times, Column column) {
    for (int k = 0; k != to_underlying(times); ++k) {
        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[3 * i + to_underlying(column)],
                      m_CubeString[3 * i + 18 + to_underlying(column)]);
        }
        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[3 * i + 18 + to_underlying(column)],
                      m_CubeString[3 * i + 45 + to_underlying(column)]);
        }
        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[3 * i + 36 + 2 - to_underlying(column)],
                      m_CubeString[3 * i + 45 + to_underlying(column)]);
        }
        std::swap(m_CubeString[36 + (2 - to_underlying(column))], m_CubeString[42 + (2 - to_underlying(column))]);
        std::swap(m_CubeString[45 + to_underlying(column)], m_CubeString[51 + to_underlying(column)]);

    }
}
void Cube::rotateAcross(RotationType times, Row row) {
    for (int k = 0; k != to_underlying(times); ++k) {
        for (int j = 1; j != 4; ++j) {
            for (int i = 0; i != 3; ++i) {
                std::swap(m_CubeString[9 * j + 3 * to_underlying(row) + i], m_CubeString[9 * (j + 1) + 3 * to_underlying(row) + i]);
            }
        }
    }
}
void Cube::rotateAcrossWithTopSide(RotationType times, Row row) {
    for (int k = 0; k != to_underlying(times); ++k) {
        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[3 * to_underlying(row) + i], m_CubeString[27 + 3 * i + (2 - to_underlying(row))]);
        }

        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[2 + 3 * to_underlying(row) - i], m_CubeString[9 + to_underlying(row) + 3 * i]);
        }

        for (int i = 0; i != 3; ++i) {
            std::swap(m_CubeString[45 + 3 * (2 - to_underlying(row)) + i], m_CubeString[9 + to_underlying(row) + 3 * i]);
        }
    }
}
int Cube::getFitness() const {
    int result = 0;
    for (int i = 0; i != 6; ++i) {
        result += std::count(m_CubeString.begin() + 9 * i, m_CubeString.begin() + 9 * (i + 1), m_CubeString[9 * i + 4]);
    }
    return 54 - result;
}
void Cube::execPermutation(const std::string &permutation) {
    std::stringstream parcer(permutation);
    std::string move;
    while (parcer >> move) {
        makeMove(move);
    }
}

void Cube::makeMove(const std::string &move) {
    if (move == "x") {
        rotateCube(RotationType::Clockwise, Axis::x);
    } else if (move == "x'") {
        rotateCube(RotationType::Anticlockwise, Axis::x);
    } else if (move == "x2") {
        rotateCube(RotationType::Double, Axis::x);
    } else if (move == "y") {
        rotateCube(RotationType::Clockwise, Axis::y);
    } else if (move == "y'") {
        rotateCube(RotationType::Anticlockwise, Axis::y);
    } else if (move == "y2") {
        rotateCube(RotationType::Double, Axis::y);
    } else if (move == "z") {
        rotateCube(RotationType::Clockwise, Axis::z);
    } else if (move == "z'") {
        rotateCube(RotationType::Anticlockwise, Axis::z);
    } else if (move == "z2") {
        rotateCube(RotationType::Double, Axis::z);
    } else if (move == "R") {
        rotateSide(RotationType::Clockwise, Side::Right);
        rotateDown(RotationType::Clockwise, Column::Right);
    } else if (move == "R'") {
        rotateSide(RotationType::Anticlockwise, Side::Right);
        rotateDown(RotationType::Anticlockwise, Column::Right);
    } else if (move == "R2") {
        rotateSide(RotationType::Double, Side::Right);
        rotateDown(RotationType::Double, Column::Right);
    } else if (move == "L") {
        rotateSide(RotationType::Clockwise, Side::Left);
        rotateDown(RotationType::Anticlockwise, Column::Left);
    } else if (move == "L'") {
        rotateSide(RotationType::Anticlockwise, Side::Left);
        rotateDown(RotationType::Clockwise, Column::Left);
    } else if (move == "L2") {
        rotateSide(RotationType::Double, Side::Left);
        rotateDown(RotationType::Double, Column::Left);
    } else if (move == "M") {
        rotateDown(RotationType::Anticlockwise, Column::Middle);
    } else if (move == "M'") {
        rotateDown(RotationType::Clockwise, Column::Middle);
    } else if (move == "M2") {
        rotateDown(RotationType::Double, Column::Middle);
    } else if (move == "U") {
        rotateSide(RotationType::Clockwise, Side::Top);
        rotateAcross(RotationType::Clockwise, Row::Top);
    } else if (move == "U'") {
        rotateSide(RotationType::Anticlockwise, Side::Top);
        rotateAcross(RotationType::Anticlockwise, Row::Top);
    } else if (move == "U2") {
        rotateSide(RotationType::Double, Side::Top);
        rotateAcross(RotationType::Double, Row::Top);
    } else if (move == "D") {
        rotateSide(RotationType::Clockwise, Side::Down);
        rotateAcross(RotationType::Anticlockwise, Row::Down);
    } else if (move == "D'") {
        rotateSide(RotationType::Anticlockwise, Side::Down);
        rotateAcross(RotationType::Clockwise, Row::Down);
    } else if (move == "D2") {
        rotateSide(RotationType::Double, Side::Down);
        rotateAcross(RotationType::Double, Row::Down);
    } else if (move == "F") {
        rotateSide(RotationType::Clockwise, Side::Front);
        rotateAcrossWithTopSide(RotationType::Clockwise, Row::Down);
    } else if (move == "F'") {
        rotateSide(RotationType::Anticlockwise, Side::Front);
        rotateAcrossWithTopSide(RotationType::Anticlockwise, Row::Down);
    } else if (move == "F2") {
        rotateSide(RotationType::Double, Side::Front);
        rotateAcrossWithTopSide(RotationType::Double, Row::Down);
    } else if (move == "B") {
        rotateSide(RotationType::Clockwise, Side::Back);
        rotateAcrossWithTopSide(RotationType::Anticlockwise, Row::Top);
    } else if (move == "B'") {
        rotateSide(RotationType::Anticlockwise, Side::Back);
        rotateAcrossWithTopSide(RotationType::Clockwise, Row::Top);
    } else if (move == "B2") {
        rotateSide(RotationType::Double, Side::Back);
        rotateAcrossWithTopSide(RotationType::Double, Row::Top);
    } else {
        throw std::logic_error("Invalid move type: Your move is " + move);
    }
    m_History.push_back(move);
}

Cube::Cube(std::string cubestring)
    : m_CubeString(std::move(cubestring))
{if (!isStateCorrect() || m_CubeString.size() != 54) {throw std::logic_error("Incorrect cube state");}}

void Cube::clearHistory() {m_History.clear();}
const std::list<std::string> &Cube::getHistory() const {return m_History;}
Cube::Cube()
    : m_CubeString("WWWWWWWWWOOOOOOOOOGGGGGGGGGRRRRRRRRRBBBBBBBBBYYYYYYYYY")
{}
void Cube::shuffle() {
    const std::vector<std::string> SINGLE_MOVES {"U", "U'", "U2", "D", "D'", "D2",
                                                 "R", "R'", "R2", "L", "L'", "L2",
                                                 "F", "F'", "F2", "B", "B'", "B2"};
    std::random_device RD;
    std::mt19937 RNG(RD());
    std::uniform_int_distribution<unsigned long long> RandomSingleMoveIndex(0, SINGLE_MOVES.size() - 1);
    for (int i = 0; i != 30; ++i) {makeMove(SINGLE_MOVES.at(RandomSingleMoveIndex(RNG)));}
    clearHistory();
}
void Cube::reset() {m_CubeString = "WWWWWWWWWOOOOOOOOOGGGGGGGGGRRRRRRRRRBBBBBBBBBYYYYYYYYY"; clearHistory();}
bool Cube::isStateCorrect() {
    std::vector<std::vector<int>> cornerFacelet {
            {8, 27, 20},
            {6, 18, 11},
            {0, 9, 38},
            {2, 36, 29},

            {47, 26, 33},
            {45, 17, 24},
            {51, 44, 15},
            {53, 35, 42}
    };
    std::vector<std::vector<int>> cornerColor {
            {4, 31, 22},
            {4, 22, 13},
            {4, 13, 40},
            {4, 40, 31},

            {49, 22, 31},
            {49, 13, 22},
            {49, 40, 13},
            {49, 31, 40}
    };
    std::vector<std::vector<int>> edgeFacelet {
            {5, 28}, {7, 19}, {3, 10}, {1, 37}, {50, 34}, {46, 25},
            {48, 16}, {52, 43}, {23, 30}, {21, 14}, {41, 12}, {39, 32}
    };
    std::vector<std::vector<int>> edgeColor {
            {4, 31}, {4, 22}, {4, 13}, {4, 40}, {49, 31}, {49, 22},
            {49, 13}, {49, 40}, {22, 31}, {22, 13}, {40, 13}, {40, 31}
    };
    std::vector<int> cornerPermutation(8, -1);
    int cornerOrientation = 0;
    std::vector<int> edgePermutation(12, -1);
    int edgeOrientation = 0;
    char colorU = m_CubeString[4];
    char colorD = m_CubeString[49];
    for (int i = 0; i != 8; ++i) {
        auto& fac = cornerFacelet[i];
        int ori = 0;
        for (; ori != 3; ++ori) {
            if (m_CubeString[fac[ori]] == colorU || m_CubeString[fac[ori]] == colorD) {
                break;
            }
        }
        char col1 = m_CubeString[fac[(ori + 1) % 3]];
        char col2 = m_CubeString[fac[(ori + 2) % 3]];
        for (int j = 0; j != 8; ++j) {
            auto& col = cornerColor[j];
            if (col1 == m_CubeString[col[1]] && col2 == m_CubeString[col[2]]) {
                cornerPermutation[i] = j;
                cornerOrientation += ori;
                break;
            }
        }
    }
    for (int i = 0; i != 12; ++i) {
        for (int j = 0; j != 12; ++j) {
            if (m_CubeString[edgeFacelet[i][0]] == m_CubeString[edgeColor[j][0]] &&
                m_CubeString[edgeFacelet[i][1]] == m_CubeString[edgeColor[j][1]]) {
                edgePermutation[i] = j;
                break;
            }
            if (m_CubeString[edgeFacelet[i][0]] == m_CubeString[edgeColor[j][1]] &&
                m_CubeString[edgeFacelet[i][1]] == m_CubeString[edgeColor[j][0]]) {
                edgePermutation[i] = j;
                edgeOrientation += 1;
                break;
            }
        }
    }
    std::set<int>
            edgeSet{edgePermutation.begin(), edgePermutation.end()},
            cornerSet{cornerPermutation.begin(), cornerPermutation.end()};
    if (edgeSet.size() != 12 || edgeSet.count(-1) != 0 ||
        cornerSet.size() != 8 || cornerSet.count(-1) != 0 ||
        edgeOrientation % 2 != 0 || cornerOrientation % 3 != 0) {
        return false;
    }
    return true;
}