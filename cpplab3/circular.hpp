#if !defined( CIRCULAR_HPP )
#define CIRCULAR_HPP
#define CAPACITY 10;
#include <memory>
#include <cmath>

template <typename T, typename alloc = std::allocator<T>, typename template_index = signed int>

class Circular {
private:
    allocator_type::size_type capacity = CAPACITY;
    alloc allocator;
    T *values = nullptr;
    template_index head = 0, tail = 0, length = 0, max_fill = capacity;
public:
    using currentT = Circular <T, alloc, template_index>;
    // constructors
    Circular(allocator_type::size_type capacity = CAPACITY)
        : capacity(capacity)
    {values = allocator.allocate(capacity);}
    Circular(allocator_type::size_type capacity, template_index fill) 
        : capacity(capacity)
        , max_fill(fill)
    {values = allocator.allocate(capacity);}
    // destructor
    virtual ~Circular() {allocator.deallocate(values, capacity);}

    // logic:
    

};

#endif