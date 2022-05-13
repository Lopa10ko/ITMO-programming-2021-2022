#ifndef ANOTHER_ONE_RUBIK_ENUMS_HPP
#define ANOTHER_ONE_RUBIK_ENUMS_HPP
#include <type_traits>
#include <utility>
template <typename E>
constexpr auto to_underlying(E e) noexcept {return static_cast<std::underlying_type_t<E>>(e);}
enum class Side {
    Top = 0,
    Left,
    Front,
    Right,
    Back,
    Down
};
enum class RotationType {
    Clockwise = 1,
    Double = 2,
    Anticlockwise = 3
};
enum class Axis {
    x, y, z
};
enum class Column {
    Left = 0,
    Middle,
    Right
};
enum class Row {
    Top = 0,
    Down = 2
};
#endif
