#include "circular.hpp"
#include <iostream>
#include <cassert>
using namespace CB;

int test_int_inputs() {
    Circular <int> stl_1(5);
    std::cout << "data: ";
    for (int i = 0; i < 8; ++i) {
        int value = std::rand() % 100;
        stl_1.force_push_back(value);
        std::cout << value << " ";
    }
    std::cout << "\nbuilt stl: ";
    stl_1.get_all();
    return 0;
}

int main() {
    test_int_inputs();
    Circular <char> stl_2(5, 4);
    stl_2.force_push_back('h');
    stl_2.force_push_back('e');
    stl_2.force_push_back('l');
    stl_2.force_push_back('l');
    stl_2.force_push_back('0');
//    stl_1.push_back(2222);
    stl_2.get_all();
    return 0;
}