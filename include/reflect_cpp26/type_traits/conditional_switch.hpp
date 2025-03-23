#ifndef REFLECT_CPP26_TYPE_TRAITS_CONDITIONAL_SWITCH_HPP
#define REFLECT_CPP26_TYPE_TRAITS_CONDITIONAL_SWITCH_HPP

#include <type_traits>

namespace reflect_cpp26 {
template <auto C, class T>
struct condition_case {};

template <class T>
struct condition_default {};

template <auto V, class... Args>
struct conditional_switch {
  static_assert(false, "Value mismatch or invalid input.");
};

// Case match
template <auto V, auto C, class T, class... Args>
  requires (V == C)
struct conditional_switch<V, condition_case<C, T>, Args...> {
  using type = T;
};

// Case mismatch
template <auto V, auto C, class T, class... Args>
  requires (V != C)
struct conditional_switch<V, condition_case<C, T>, Args...> {
  using type = typename conditional_switch<V, Args...>::type;
};

// Default case: must be the last one
template <auto V, class T>
struct conditional_switch<V, condition_default<T>> {
  using type = T;
};

/**
 * Type switch-case with key V.
 * Args should be a series of conditional_switch<Ci, Ti> and an optional
 * trailing conditional_default<T>.
 * Example: See struct integral_to_integer below
 */
template <auto V, class... Args>
using conditional_switch_t = typename conditional_switch<V, Args...>::type;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CONDITIONAL_SWITCH_HPP
