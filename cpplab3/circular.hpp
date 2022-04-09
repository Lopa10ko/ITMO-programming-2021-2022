#if !defined( CIRCULAR_HPP )
#define CIRCULAR_HPP
#define CAPACITY 10;
#include <memory>
#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <iostream>

namespace CB {
    // template <typename T, typename alloc = std::allocator<T>, typename template_index = signed int>
    template <typename T>
    class Circular {
    private:
        unsigned long capacity = CAPACITY;
        T *values = NULL;
        unsigned long head = 0, tail = 0, length = 0, max_fill = capacity;
    public:
        // constructor
        explicit Circular(unsigned capacity, unsigned fill) 
            : capacity(capacity)
            , max_fill(fill)
        {values = new T [capacity];}
        explicit Circular(unsigned capacity)
                : capacity(capacity)
        {values = new T [capacity];}
        // destructor
        virtual ~Circular() {delete [] values;}
        // helpers
        bool is_filled() const {return capacity == max_fill;}
        unsigned int increment_index(unsigned int index) const {
            return (++index < max_fill) ? index : 0;
        }
        unsigned long get_tail() const {return tail;}
        unsigned long get_head() const {return head;}
        unsigned long get_capacity() const {return capacity;}
        // logic:
        void push_back(T val) {
            if (is_filled()) {
                throw std::overflow_error("buffer overflow, try to force push via \"force_push_back(T value);\"");
            }
            values[tail] = val;
            tail = increment_index(tail);
        }
        void force_push_back(const T val) {
            if (is_filled()) {
                head++;
                head %= capacity;
            } else {
                max_fill++;
            }
            values[tail] = val;
            tail = increment_index(tail);
            tail %= capacity;
        }

        bool push_front(T val);
        void force_push_front(T val);
        T pop_back();
        T pop_front();
        T get_front();
        T get_back();
        T &operator[](int index);
        //resize
        //reserve
        //insert emplace
        //erase
        //shrink to fit
        //swap
        //sort
        /*[REFACTOR IDEA]
         * change get_all() output to &array of <typename data>
         * abandon std::cout entirely so in tests there will be only err_code output
         * detect errors in main.cpp with test cases by err_code only
         */
        //visualization
        void get_all() const {
            std::cout << "{" << values[tail] << "} <-";
            for (unsigned long i = (tail + 1) % capacity; i != head; i = (++i) % capacity) {
                std::cout << " {" << values[i] << "} <-";
            }
            std::cout << "\n-----------------\n";
        }

    };

    template <typename T>
    class CIter : public std::iterator<std::random_access_iterator_tag, T> {
    public:
        T *index, *data, *head, *tail;
        unsigned long capacity{}; //C++11 field initialization
        T *operator->(){return index;};
        T &operator*(){return *index;};
        CIter(T *index, T *data, unsigned long capacity_, T *head, T *tail)
        : index(index)
        , data(data)
        , capacity(capacity)
        , head(head)
        , tail(tail)
        {}
        explicit CIter(T *index)
        : index(index)
        {}
        ~CIter() = default;
        T &operator=(T *dull) {index = dull; return *this;}
        T &operator=(const CIter *dull) {index = dull; return *this;}
        // sizing & iteration operators
        CIter &operator++() {index++; return *this;}
        // force dependence
        CIter operator++(int) {
            CIter object(*this);
            operator++();
            return object;
        }
    };
}
#endif