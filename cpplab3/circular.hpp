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
    template <class T> class Circular;
    template <class T> class CIter;
    template <class T>
    class Circular {
    private:
        unsigned long capacity = CAPACITY;
        T *values = NULL;
        unsigned long head = 0, tail = 0, length = 0;
    public:
        // constructor
        explicit Circular(unsigned capacity)
                : capacity(capacity)
        {values = new T [capacity];}
        // destructor
        virtual ~Circular() {delete [] values;}
        // helpers
        bool is_filled() const {return length == capacity;}
        unsigned int increment_index(unsigned int index) const {
            return (++index < capacity) ? index % capacity : 0;
        }
        unsigned long get_tail() const {return tail;}
        unsigned long get_length() const {return length;}
        unsigned long get_capacity() const {return capacity;}
        // logic:
        void push_back(const T &val) {
            if (is_filled()) {
                throw std::overflow_error("buffer overflow, try to force push via \"force_push_back(T value);\"");
            }
            values[tail] = val;
            tail = increment_index(tail);
            ++length;
        }
        void push_back(const T array[], unsigned int length_) {
            for (unsigned long i = 0; i < length_; ++i) {
                try {push_back(array[i]);}
                catch (std::overflow_error &err) {throw err;}
            }
        }
        void force_push_back(const T &val) {
            if (is_filled()) {
                head++;
                head %= capacity;
            } else {
                ++length;
            }
            values[tail] = val;
            tail = increment_index(tail) % capacity;
        }
        void push_front(const T &val) {
            if (is_filled()) {
                throw std::overflow_error("buffer overflow, try to force push via \"force_push_back(T value);\"");
            }
            values[head] = val;
            head = increment_index(head);
            ++length;
        }
        void force_push_front(const T &val) {
            if (head == 0) {
                head = capacity - 1;
            } else {
                head--;
            }
            values[head] = val;
            if (is_filled()) {
                tail = head;
            } else {
                length++;
            }
        }
        T pop_back() {
            head = (!head) ? capacity - 1 : head - 1;
            --length;
            return values[head];
        }
        T pop_front() {
            auto elem = tail;
            tail = increment_index(tail);
            --length;
            return values[elem];
        }
        T get_front() {
            return values[tail];
        }
        const T &get_front() const {
            return operator[](0);
        }
        T get_back() {
            return (head == 0) ? values[capacity - 1] : values[head - 1];
        }
        const T &get_back() const {
            return operator[](get_length() - 1);
        }
        T operator[](unsigned int index) {
            return (index <= length) ? values[(head + index) % capacity] : NULL;
        }
        //resize
        //reserve
        //insert emplace
        //erase
        //shrink to fit
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
        CIter<T> begin() {
            return CIter<T>(values, capacity, head, 0);
        }
        CIter<T> end() {
            return CIter<T>(values, capacity, head, get_length());
        }

    };

    template <class T>
    class CIter : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        T *values = nullptr;
        unsigned long length, index, head;
    public:
         //C++11 field initialization
        T *operator->(){return values + (head + index) % length;};
        T &operator*(){return values[(head + index) % length];};
        explicit CIter(T *values, unsigned long length, unsigned long head, unsigned long index)
            : index(index)
            , values(values)
            , length(length)
            , head(head)
        {}
//        explicit CIter(unsigned long index)
//            : index(index)
//        {}
        CIter(const CIter &other)
            : index(other.index)
            , values(other.values)
            , length(other.length)
            , head (other.head)
        {}
        ~CIter() = default;
//        CIter &operator=(T *dull) {index = dull; return *this;}
//        CIter &operator=(const CIter *dull) {index = dull; return *this;}
        // sizing & iteration operators
        CIter &operator++() {index++; return *this;}
        CIter &operator--() {index--; return *this;}
        // force dependence on in-line
        CIter operator++(int) {
            CIter object(*this);
            operator++();
            return object;
        }
        CIter operator--(int) {
            CIter object(*this);
            operator--();
            return object;
        }
        CIter operator+(typename std::iterator<std::random_access_iterator_tag, T>::difference_type shift) {return CIter(values, length, head, index + shift);}
        CIter operator-(typename std::iterator<std::random_access_iterator_tag, T>::difference_type shift) {return CIter(values, length, head, index - shift);}
        typename std::iterator<std::random_access_iterator_tag, T>::difference_type operator+(const CIter<T> &shift) {return index + shift.index;}
        typename std::iterator<std::random_access_iterator_tag, T>::difference_type operator-(const CIter<T> &shift) {return index - shift.index;}
        CIter &operator+=(typename std::iterator<std::random_access_iterator_tag, T>::difference_type shift) {index += shift; return *this;}
        CIter &operator-=(typename std::iterator<std::random_access_iterator_tag, T>::difference_type shift) {index -= shift; return *this;}
        bool operator==(const CIter &other) const {return index == other.index;}
        bool operator!=(const CIter &other) const {return index != other.index;}
        bool operator>=(const CIter &other) const {return index >= other.index;}
        bool operator<=(const CIter &other) const {return index <= other.index;}
        bool operator>(const CIter &other) const {return index > other.index;}
        bool operator<(const CIter &other) const {return index < other.index;}
        bool operator>=(T *other_index) const {return index >= other_index;}
        bool operator<=(T *other_index) const {return index <= other_index;}
        bool operator>(T *other_index) const {return index > other_index;}
        bool operator<(T *other_index) const {return index < other_index;}
    };
}
#endif