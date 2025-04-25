#ifndef REFLECT_CPP26_UTILS_FUNCTIONAL_HPP
#define REFLECT_CPP26_UTILS_FUNCTIONAL_HPP

#include <reflect_cpp26/utils/compare.hpp>
#include <reflect_cpp26/utils/utility.hpp>
#include <functional>
#include <type_traits>

namespace reflect_cpp26 {
// -------- Extension of generic comparison --------
// (1) Less constraints than std::ranges::less, etc. and std::compare_three_way.
// (2) Uses integral comparison above if both operands are integral types.

template <class T, class U>
constexpr auto is_less_comparable_v = is_operator_lt_comparable_v<T, U>;

struct less_t {
  template <class T, class U>
    requires (is_less_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_less(t, u);
    } else {
      return t < u;
    }
  }
};

template <class T, class U>
constexpr auto is_greater_comparable_v =
     is_operator_gt_comparable_v<T, U>
  || is_operator_lt_comparable_v<U, T>;

struct greater_t {
  template <class T, class U>
    requires (is_greater_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_greater(t, u);
    } else if constexpr (is_operator_gt_comparable_v<T, U>) {
      return t > u;
    } else { // if constexpr (is_operator_lt_comparable_v<U, T>)
      return u < t;
    }
  }
};

template <class T, class U>
constexpr auto is_less_equal_comparable_v =
     is_operator_le_comparable_v<T, U>
  || is_operator_lt_comparable_v<T, U> && is_operator_eq_comparable_v<T, U>;

struct less_equal_t {
  // Note: !(u < t) may be incorrect behavior for partial ordering.
  template <class T, class U>
    requires (is_less_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_less_equal(t, u);
    } else if constexpr (is_operator_le_comparable_v<T, U>) {
      return t <= u;
    } else {
      return t < u || t == u;
    }
  }
};

template <class T, class U>
constexpr auto is_greater_equal_comparable_v =
     is_operator_ge_comparable_v<T, U>
  || is_operator_le_comparable_v<U, T>
  || is_operator_gt_comparable_v<T, U> && is_operator_eq_comparable_v<T, U>
  || is_operator_lt_comparable_v<U, T> && is_operator_eq_comparable_v<U, T>;

struct greater_equal_t {
  // Note: !(t < u) may be incorrect behavior for partial ordering
  template <class T, class U>
    requires (is_greater_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_greater_equal(t, u);
    } else if constexpr (is_operator_ge_comparable_v<T, U>) {
      return t >= u;
    } else if constexpr (is_operator_le_comparable_v<U, T>) {
      return u <= t;
    } else if constexpr (is_operator_gt_comparable_v<T, U>
                      && is_operator_eq_comparable_v<T, U>) {
      return t > u || t >= u;
    } else {
      return u < t || u == t;
    }
  }
};

template <class T, class U>
constexpr auto is_equal_comparable_v = is_operator_eq_comparable_v<T, U>;

struct equal_t {
  template <class T, class U>
    requires (is_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_equal(t, u);
    } else {
      return t == u;
    }
  }
};

template <class T, class U>
constexpr auto is_not_equal_comparable_v =
     is_operator_ne_comparable_v<T, U>
  || is_operator_eq_comparable_v<T, U>;

struct not_equal_t {
  template <class T, class U>
    requires (is_not_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_not_equal(t, u);
    } else if constexpr (is_operator_ne_comparable_v<T, U>) {
      return t != u;
    } else { // if constexpr (is_operator_eq_comparable_v<T, U>)
      return !(t == u);
    }
  }
};

// Special rule for integral types with different signedness
template <class T, class U>
constexpr auto is_compare_three_way_comparable_v =
     std::is_integral_v<T> && std::is_integral_v<U>
  || is_operator_3way_comparable_v<T, U>
  || is_less_comparable_v<T, U>
        && is_greater_comparable_v<T, U>
        && is_equal_comparable_v<T, U>;

struct compare_three_way_t {
  // Note: For types where operator<=> is not supported,
  // the most conservative comparison policy std::partial_ordering is used.
  template <class T, class U>
  static constexpr auto do_indirect_compare(const T& t, const U& u) noexcept
    -> std::partial_ordering
  {
    if (less_t::operator()(t, u)) {
      return std::partial_ordering::less;
    }
    if (greater_t::operator()(t, u)) {
      return std::partial_ordering::greater;
    }
    if (equal_t::operator()(t, u)) {
      return std::partial_ordering::equivalent;
    }
    return std::partial_ordering::unordered;
  }

  template <class T, class U>
    requires (is_compare_three_way_comparable_v<T, U>)
  static constexpr auto operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return cmp_three_way(t, u);
    } else if constexpr (is_operator_3way_comparable_v<T, U>) {
      return t <=> u;
    } else {
      return do_indirect_compare(t, u);
    }
  }
};

constexpr auto less = less_t{};
constexpr auto greater = greater_t{};
constexpr auto less_equal = less_equal_t{};
constexpr auto greater_equal = greater_equal_t{};
constexpr auto equal = equal_t{};
constexpr auto not_equal = not_equal_t{};
constexpr auto compare_three_way = compare_three_way_t{};

REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(less)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(greater)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(less_equal)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(greater_equal)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(equal)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(not_equal)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(compare_three_way)
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_FUNCTIONAL_HPP
