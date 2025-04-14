#ifndef REFLECT_CPP26_UTILS_COMPARE_HPP
#define REFLECT_CPP26_UTILS_COMPARE_HPP

#include <compare>

namespace reflect_cpp26 {
template <class T>
constexpr bool is_three_way_comparison_result_v =
     std::is_same_v<T, std::strong_ordering>
  || std::is_same_v<T, std::weak_ordering>
  || std::is_same_v<T, std::partial_ordering>;

template <class T>
concept three_way_comparison_result = is_three_way_comparison_result_v<T>;

template <class T, class U>
constexpr bool is_operator_eq_comparable_v =
  requires (const T& t, const U& u) {
    { t == u } -> std::same_as<bool>;
  };

template <class T, class U>
constexpr bool is_operator_ne_comparable_v =
  requires (const T& t, const U& u) {
    { t != u } -> std::same_as<bool>;
  };

template <class T, class U>
constexpr bool is_operator_lt_comparable_v =
  requires (const T& t, const U& u) {
    { t < u } -> std::same_as<bool>;
  };

template <class T, class U>
constexpr bool is_operator_le_comparable_v =
  requires (const T& t, const U& u) {
    { t <= u } -> std::same_as<bool>;
  };

template <class T, class U>
constexpr bool is_operator_gt_comparable_v =
  requires (const T& t, const U& u) {
    { t > u } -> std::same_as<bool>;
  };

template <class T, class U>
constexpr bool is_operator_ge_comparable_v =
  requires (const T& t, const U& u) {
    { t >= u } -> std::same_as<bool>;
  };

// Note: is_operator_3way_comparable_v<T, U> == false if T, U are
// integral types with different signedness.
// Possible compile error message: "argument to 'operator<=>' cannot be
// narrowed from type 'int' to 'unsigned int'"
template <class T, class U>
constexpr bool is_operator_3way_comparable_v =
  requires (const T& t, const U& u) {
    { t <=> u } -> three_way_comparison_result;
  };

#define REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(type)     \
  template <class T, class U>                                       \
  struct is_##type##_comparable                                     \
    : std::bool_constant<is_##type##_comparable_v<T, U>> {};        \
                                                                    \
  template <class T, class U>                                       \
  concept type##_comparable_with = is_##type##_comparable_v<T, U>;

REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_eq)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_ne)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_lt)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_le)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_gt)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_ge)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(operator_3way)
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_COMPARE_HPP
