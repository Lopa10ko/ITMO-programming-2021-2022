#if !defined( CIRCULAR_HPP )
#define CIRCULAR_HPP
#define CAPACITY 10;
#include <memory>
#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <cstdlib>
#include <iterator>

namespace CB {
    // template <typename T, typename alloc = std::allocator<T>, typename template_index = signed int>
    template <class T>
    class Circular {
    private:
        unsigned capacity = CAPACITY;
        T *values = NULL;
        unsigned head = 0, tail = 0, length = 0, max_fill = capacity;
    public:
        // constructor
        explicit Circular(unsigned capacity, unsigned fill) 
            : capacity(capacity)
            , max_fill(fill)
        {values = new T [capacity];}
        // destructor
        virtual ~Circular() {delete [] values;}
        // logic:
        bool push_back(T values);
        void force_push_back(T values);
        bool push_front(T values);
        void force_push_front(T values);
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
    };
}
#endif