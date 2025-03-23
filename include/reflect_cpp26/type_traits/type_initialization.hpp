#ifndef REFLECT_CPP26_TYPE_TRAITS_TYPE_INITIALIZATION_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TYPE_INITIALIZATION_HPP

namespace reflect_cpp26 {
/**
 * Whether T is value-initializable, i.e. T{} is valid expression.
 */
template <class T>
constexpr auto is_value_initializable_v = requires { T{}; };
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_TYPE_INITIALIZATION_HPP
