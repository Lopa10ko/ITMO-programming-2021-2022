#if !defined( META_HPP )
#define META_HPP
#include <cmath>
#include <iostream>
template <int x> struct factor {};
template <int x> struct deg {};
//template wrapper
template <int N> class value {
public:
    static constexpr int x = N;
};
//poly0() functor represents one term of poly in the (factor, deg) params
template <typename Fac, typename Deg> class poly0;
template <int Fac, int Deg> class poly0<factor<Fac>, deg<Deg>> {
public:
    static constexpr int pow(int x, int N) {
        if(N < 0) return 1 / pow(x, -N);
        else if(N == 0) return 1;
        else if(N % 2 == 0){
            int p = pow(x, N / 2);
            return p * p;
        } else return pow(x, N - 1) * x;
    }
    static constexpr int at(int val) noexcept {return (int)(Fac * pow(val, Deg));}
};

//template<typename... T> class Poly {
//public:
//    static constexpr int value = 0;
////    [[nodiscard]] constexpr int at(int) const noexcept {return 0;}
//    //no exception specification (compiler automatization)
//    //nodiscard attribute to point that value can't be ignored
//};

//template<value Value, poly0 Poly0, poly0... Set_poly0> class Poly {
////private:
//////    static poly0<factor<Fac>, deg<Deg>> my_poly0;
//public:
//    //recursive calls to other poly0s:
//    // ANSWER = poly0_0.at() + poly0_1.at() + ...
//    //virtual functions can't be constexpr:
//    // (Poly : Poly<...>)::at() doesn't override Poly::at()
//    static constexpr int value = Poly0.at(Value.x) + Poly<Value, Set_poly0...>::value;
////    [[nodiscard]] constexpr int at(int x) const noexcept {
////        return my_poly0.at(x) + Poly<Set_poly0...>::at(x);
////    }
//    friend std::ostream& operator << <> (std::ostream&, const Poly<poly0<factor<Fac>, deg<Deg>>, Set_poly0...> &q);
//};
//template<value Value, poly0 Poly0> class Poly<Value, Poly0> {
//public:
//    static constexpr int value = Poly0.at(Value.x);
//
////    friend std::ostream& operator << <> (std::ostream&, const Poly<poly0<factor<Fac>, deg<Deg>>, Set_poly0...> &q);
//};

template<typename ... T> class Poly {
public:
    static constexpr int val = 0;
    friend std::ostream& operator<<(std::ostream& out, const Poly<T...> &q) {throw std::invalid_argument("gavno!");}
};
template<int N, int Fac, int Deg, typename... Set_poly0> class Poly<value<N>, poly0<factor<Fac>, deg<Deg>>, Set_poly0...> {
public:
    static constexpr int val = poly0<factor<Fac>, deg<Deg>>::at(N) + Poly<value<N>, Set_poly0...>::val;
    friend std::ostream& operator<< (std::ostream& out, const Poly<value<N>, poly0<factor<Fac>, deg<Deg>>, Set_poly0...> &) {
        out << "(" << std::to_string(Fac) << "x^" << std::to_string(Deg) << ") + " << Poly<value<N>, Set_poly0...>();
        return out;
    }
};
template<int N, int Fac, int Deg> class Poly<value<N>, poly0<factor<Fac>, deg<Deg>>> {
public:
    static constexpr int val = poly0<factor<Fac>, deg<Deg>>::at(N);
    friend std::ostream& operator<<(std::ostream& out, const Poly<value<N>, poly0<factor<Fac>, deg<Deg>>> &) {
        out << "(" << std::to_string(Fac) << "x^" << std::to_string(Deg) << ")";
        return out;
    }
};
#endif