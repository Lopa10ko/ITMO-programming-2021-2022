#if !defined( PREDICATES_HPP )
#define PREDICATES_HPP
#include <memory>
#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <vector>
#include <functional>

namespace PRED {
    template<class Index, class P>
    bool all_of(Index head, Index tail, P lambda) {
        for (auto i = head; i != tail; i++) {if (!lambda(*i)) {return false;}}
        return true;
    }
    template<class Index, class P>
    bool any_of(Index head, Index tail, P lambda) {
        for (auto i = head; i != tail; i++) {if (lambda(*i)) {return true;}}
        return false;
    }
    template<class Index, class P>
    bool none_of(Index head, Index tail, P lambda) {return !any_of(head, tail, lambda);}
    template<class Index, class P>
    bool one_of(Index head, Index tail, P lambda) {
        int flag = 0;
        for (auto i = head; i != tail; ++i) {if (lambda(*i)) {flag++;}}
        return flag == 1;
    }
    //https://www.geeksforgeeks.org/stdis_sorted-in-cpp/
    template<class Index, class B>
    bool is_sorted(Index head, Index tail, B lambda) {
        if (head++ == tail) return true;
        if (head == tail) return true;
        while (head != tail) {
            if (!lambda(*head, *(head--))) {return false;}
            head += 2;
        }
        return true;
    }
    template<class Index, class P>
    bool is_partitioned(Index head, Index tail, P lambda) {
        bool check = lambda(*head);
        if (head++ == tail) return false;
        while(head != tail) {
            if (lambda(*head) != check) {check = !check; head++; break;}
            head++;
        }
        if (head == tail) return false;
        while(head != tail) {
            if (lambda(*head) != check) return false;
            head++;
        }
        return true;
    }
    template<class Index, class T>
    Index find_not(Index head, Index tail, const T &check) {
        for (auto i = head; i != tail; ++i) {
            if (*i != check) {return i;}
        }
        return tail;
    }
    template<class Index, class T>
    Index find_backward(Index head, Index tail, const T &check) {
        auto out = tail;
        tail--;
        while(tail != head) {
            if (*tail == check) return tail;
            tail--;
        }
        return (*head == check) ? head : out;
    }
    template<class Index, class P>
    bool is_palindrome(Index head, Index tail, P lambda) {
        for (auto i = head, back_i = tail - 1; i != tail; i++, back_i--) {
            if (lambda(*i) != lambda(*back_i)) return false;
        }
        return true;
    }
}
#endif