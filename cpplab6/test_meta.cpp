#include "meta.hpp"
#include <iostream>

int main() {
//    auto f = new value<2>;
//    auto a = value<2>();
//    auto term1 = poly0<factor<9>, deg<3>>();
//    auto term2 = poly0<factor<3>, deg<1>>();
//    using func = Poly <a, term1, term2>;
    using func = Poly <
            value<5>,
            poly0<factor<9>, deg<3>>,
            poly0<factor<3>, deg<1>>,
            poly0<factor<2>, deg<1>>,
            poly0<factor<239>, deg<0>>
            >;
    std::cout << func::val << std::endl;
    std::cout << func() << std::endl;
    return 0;
}