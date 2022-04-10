#include "circular.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace CB;

int test_force_push() {
    Circular <int> stl(5);
    std::cout << "[TEST]: force_push_back()\n";
    std::cout << "data: ";
    for (int i = 0; i < 9; ++i) {
        int value = std::rand() % 100;
        stl.force_push_back(value);
        std::cout << value << " ";
    }
    std::cout << "\nbuilt stl: ";
    stl.get_all();
    Circular <char> stl_2(5);
    stl_2.force_push_back('h');
    stl_2.force_push_front('q');
    stl_2.force_push_front('w');
    stl_2.force_push_back('e');
    stl_2.force_push_back('l');
    stl_2.force_push_front('g');
    stl_2.get_all();
    return 0;
}
int test_push() {
    Circular <int> stl(6);
    assert(stl.get_capacity() == 6);
    std::cout << "[TEST]: push_back()\n";
    std::cout << "data: ";
    for (int i = 0; i < 7; ++i) {
        int value = std::rand() % 100;
        stl.push_back(value);
        std::cout << value << " ";
    }
    std::cout << "\nbuilt stl: ";
    stl.get_all();
    return 0;
}
int test_stl_algo() {
    Circular <int> stl(5);
    std::cout << "[TEST]: std::stl algo\n";
    std::cout << "data: ";
    for (int i = 0; i < 5; ++i) {
        int value = std::rand() % 100;
        stl.force_push_back(value);
        std::cout << value << " ";
    }
    std::cout << "\nbuilt stl: ";
    stl.get_all();
    auto a = stl.end();
    for (auto elem : stl) {
        std::cout << elem << std::endl;
    }
    std::cout << "sorted: ";
    std::sort(stl.begin(), stl.end());
    stl.get_all();
    return 0;
}
int test_by_index() {
    Circular <unsigned long> stl(8);
    //[TEST]: operator[]
    for (unsigned long i = 0; i < stl.get_capacity(); ++i) {
        stl.force_push_back(i);
    }
    for (unsigned long i = 0; i < stl.get_length(); ++i) {
        assert(stl[i] == i);
    }
    return 0;
}
int test_gets() {
    Circular <unsigned long> stl(8);
    //[TEST]: operator[]
    for (unsigned long i = rand() % 100; stl.get_length() < stl.get_capacity(); i = i + rand() % 10) {
        stl.force_push_back(i);
    }
    auto last = stl.end() - 1;
    auto first = stl.begin();
    assert(stl.get_front() == *first);
    assert(stl.get_back() == *last);
    assert(stl.pop_back() == *last);
    assert(stl.get_back() == *(last - 1));
    assert(stl.pop_front() == *first);
    assert(stl.get_front() == *(first + 1));
    return 0;
}

int main() {
    assert(!test_force_push());
    try {test_push();}
    catch (std::overflow_error &err) {
        std::cout << std::endl;
        std::cout << err.what();
        std::cout << "\n-----------------\n";
    }
    assert(!test_stl_algo());
    assert(!test_by_index());
    assert(!test_gets());

    return 0;
}