#include "predicates.hpp"
#include <vector>
#include <cassert>
bool is_even(int elem) {return (elem) % 2 == 0;}
bool is_odd(int elem) {return !(is_even(elem));}
int gcd(int a, int b) {
    if (!b) return a;
    return gcd(b, a % b);
}
bool is_prime(int elem) {
    for (int i = 2; i <= (int)sqrt(elem); ++i) {
        if(elem % i == 0) return false;
    }
    return true;
}
bool inc(int x, int y) {
    return x <= y;
}
int test_all_any() {
    std::vector<int> stl(5);
    for (int i = 0; i < 5; ++i) {stl.push_back(i);}
    assert(PRED::all_of(stl.begin(), stl.end(), is_even) == 0);
    assert(PRED::all_of(stl.begin(), stl.end(), is_odd) == 0);
    assert(PRED::all_of(stl.begin(), stl.end(), is_prime) == 0);
    assert(PRED::any_of(stl.begin(), stl.end(), is_odd) == 1);
    assert(PRED::any_of(stl.begin(), stl.end(), is_odd) == 1);
    assert(PRED::any_of(stl.begin(), stl.end(), is_prime) == 1);
    stl.clear();
    stl.push_back(6);
    stl.push_back(4);
    stl.push_back(8);
    stl.push_back(10);
    assert(PRED::any_of(stl.begin(), stl.end(), is_prime) == 0);
    assert(PRED::all_of(stl.begin(), stl.end(), is_prime) == 0);
    return 0;
}
int test_one_none() {
    std::vector<int> stl(5);
    for (int i = 0; i < 5; ++i) {stl.push_back(i);}
    assert(PRED::none_of(stl.begin(), stl.end(), is_even) == 0);
    assert(PRED::none_of(stl.begin(), stl.end(), is_odd) == 0);
    assert(PRED::none_of(stl.begin(), stl.end(), is_prime) == 0);
    assert(PRED::one_of(stl.begin(), stl.end(), is_odd) == 0);
    assert(PRED::one_of(stl.begin(), stl.end(), is_odd) == 0);
    assert(PRED::one_of(stl.begin(), stl.end(), is_prime) == 0);
    stl.clear();
    stl.push_back(6);
    stl.push_back(4);
    stl.push_back(8);
    stl.push_back(10);
    assert(PRED::none_of(stl.begin(), stl.end(), is_prime) == 1);
    stl.push_back(3);
    assert(PRED::one_of(stl.begin(), stl.end(), is_prime) == 1);
    return 0;
}
int test_sort() {
    std::vector<int> stl(5);
    for (int i = 0; i < 5; ++i) {stl.push_back(i);}
    assert(PRED::is_sorted(stl.begin(), stl.end(), inc) == 1);
    stl.clear();
    stl.push_back(4);
    for (int i = 0; i < 5; ++i) {stl.push_back(i);}
    assert(PRED::is_sorted(stl.begin(), stl.end(), inc) == 0);
    stl.clear();
    for (int i = 5; i > 0; --i) {stl.push_back(i);}
    assert(PRED::is_sorted(stl.begin(), stl.end(), inc) == 0);
    return 0;
}
int test_partitioned() {
    std::vector<int> stl(5);
    stl.push_back(4);
    stl.push_back(8);
    stl.push_back(3);
    stl.push_back(17);
    stl.push_back(19);
    assert(PRED::is_partitioned(stl.begin(), stl.end(), is_prime) == 0);
    stl.push_back(12);
    assert(PRED::is_partitioned(stl.begin(), stl.end(), is_prime) == 0);
    return 0;
}
int test_find() {
    std::vector<int> stl(5);
    stl.push_back(4);
    stl.push_back(0);
    stl.push_back(3);
    stl.push_back(17);
    stl.push_back(19);
    assert(*PRED::find_not(stl.begin(), stl.end(), 4) == 0);
    assert(*PRED::find_backward(stl.begin(), stl.end(), 17) == 17);
    return 0;
}
int test_palindrome() {
    std::vector<int> stl(5);
    for (int i = 1; i < 6; ++i) {
        stl.push_back(i);
    }
    assert(PRED::is_palindrome(stl.begin(), stl.end(), is_even) == 0);
    stl[0] = 2;
    assert(PRED::is_palindrome(stl.begin(), stl.end(), is_even) == 0);
    return 0;
}

int main() {
    assert(!test_all_any());
    assert(!test_one_none());
    assert(!test_sort());
    assert(!test_partitioned());
    assert(!test_find());
    assert(!test_palindrome());
    return 0;
}