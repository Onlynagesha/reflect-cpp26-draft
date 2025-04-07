#ifndef REFLECT_CPP26_UTILS_UTILITY_HPP
#define REFLECT_CPP26_UTILS_UTILITY_HPP

#include <compare>
#include <concepts>
#include <functional>
#include <limits>

namespace reflect_cpp26 {
template <class T>
constexpr bool is_three_way_comparison_result_v =
     std::is_same_v<T, std::strong_ordering>
  || std::is_same_v<T, std::weak_ordering>
  || std::is_same_v<T, std::partial_ordering>;

template <class T>
concept three_way_comparison_result = is_three_way_comparison_result_v<T>;

template <class T, class U>
constexpr bool is_eq_comparable_v = requires (const T& t, const U& u) {
  { t == u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_ne_comparable_v = requires (const T& t, const U& u) {
  { t != u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_lt_comparable_v = requires (const T& t, const U& u) {
  { t < u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_le_comparable_v = requires (const T& t, const U& u) {
  { t <= u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_gt_comparable_v = requires (const T& t, const U& u) {
  { t > u } -> std::same_as<bool>;
};

template <class T, class U>
constexpr bool is_ge_comparable_v = requires (const T& t, const U& u) {
  { t >= u } -> std::same_as<bool>;
};

// Note: is_three_way_comparable_v<T, U> == false if T, U are
// integral types with different signedness.
// Possible compile error message: "argument to 'operator<=>' cannot be
// narrowed from type 'int' to 'unsigned int'"
template <class T, class U>
constexpr bool is_three_way_comparable_v = requires (const T& t, const U& u) {
  { t <=> u } -> three_way_comparison_result;
};

#define REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(type)     \
  template <class T, class U>                                       \
  struct is_##type##_comparable                                     \
    : std::bool_constant<is_##type##_comparable_v<T, U>> {};        \
                                                                    \
  template <class T, class U>                                       \
  concept type##_comparable_with = is_##type##_comparable_v<T, U>;

REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(eq)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(ne)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(lt)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(le)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(gt)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(ge)
REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(three_way)

// -------- Relaxed alternative of integer comparison  --------
// bool and character types are allowed

struct integral_cmp_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept
  {
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
      return t == u;
    else if constexpr (std::is_signed_v<T>)
      return t >= 0 && std::make_unsigned_t<T>(t) == u;
    else
      return u >= 0 && std::make_unsigned_t<U>(u) == t;
  }
};

struct integral_cmp_not_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !integral_cmp_equal_t::operator()(t, u);
  }
};

struct integral_cmp_less_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept
  {
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
      return t < u;
    else if constexpr (std::is_signed_v<T>)
      return t < 0 || std::make_unsigned_t<T>(t) < u;
    else
      return u >= 0 && t < std::make_unsigned_t<U>(u);
  }
};

struct integral_cmp_greater_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return integral_cmp_less_t::operator()(u, t);
  }
};

struct integral_cmp_less_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !integral_cmp_less_t::operator()(u, t);
  }
};

struct integral_cmp_greater_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !integral_cmp_less_t::operator()(t, u);
  }
};

struct integral_cmp_three_way_t {
  template <std::integral T, std::integral U>
  static constexpr auto operator()(T t, U u) noexcept -> std::strong_ordering
  {
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>) {
      return t <=> u;
    } else if constexpr (std::is_signed_v<T>) {
      if (t < 0) {
        return std::strong_ordering::less;
      }
      return std::make_unsigned_t<T>(t) <=> u;
    } else {
      if (u < 0) {
        return std::strong_ordering::greater;
      }
      return t <=> std::make_unsigned_t<U>(u);
    }
  }
};

template <std::integral R>
struct in_range_t {
  static constexpr auto limit_min = std::numeric_limits<R>::min();
  static constexpr auto limit_max = std::numeric_limits<R>::max();

  template<std::integral T>
  static constexpr bool operator()(T t) noexcept
  {
    return integral_cmp_greater_equal_t::operator()(t, limit_min)
      && integral_cmp_less_equal_t::operator()(t, limit_max);
  }
};

constexpr auto integral_cmp_less = integral_cmp_less_t{};
constexpr auto integral_cmp_greater = integral_cmp_greater_t{};
constexpr auto integral_cmp_less_equal = integral_cmp_less_equal_t{};
constexpr auto integral_cmp_greater_equal = integral_cmp_greater_equal_t{};
constexpr auto integral_cmp_equal = integral_cmp_equal_t{};
constexpr auto integral_cmp_not_equal = integral_cmp_not_equal_t{};
constexpr auto integral_cmp_three_way = integral_cmp_three_way_t{};
template <class R>
constexpr auto in_range = in_range_t<R>{};

// -------- Extension of generic comparison --------
// (1) Less constraints than std::ranges::less, etc. and std::compare_three_way.
// (2) Uses integral comparison above if both operands are integral types.

template <class T, class U>
constexpr auto is_less_comparable_v = is_lt_comparable_v<T, U>;

struct less_t {
  template <class T, class U>
    requires (is_less_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_less(t, u);
    } else {
      return t < u;
    }
  }
};

template <class T, class U>
constexpr auto is_greater_comparable_v =
     is_gt_comparable_v<T, U>
  || is_lt_comparable_v<U, T>;

struct greater_t {
  template <class T, class U>
    requires (is_greater_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_greater(t, u);
    } else if constexpr (is_gt_comparable_v<T, U>) {
      return t > u;
    } else { // if constexpr (is_lt_comparable_v<U, T>)
      return u < t;
    }
  }
};

template <class T, class U>
constexpr auto is_less_equal_comparable_v =
     is_le_comparable_v<T, U>
  || is_lt_comparable_v<T, U> && is_eq_comparable_v<T, U>;

struct less_equal_t {
  // Note: !(u < t) may be incorrect behavior for partial ordering.
  template <class T, class U>
    requires (is_less_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_less_equal(t, u);
    } else if constexpr (is_le_comparable_v<T, U>) {
      return t <= u;
    } else {
      return t < u || t == u;
    }
  }
};

template <class T, class U>
constexpr auto is_greater_equal_comparable_v =
     is_ge_comparable_v<T, U>
  || is_le_comparable_v<U, T>
  || is_gt_comparable_v<T, U> && is_eq_comparable_v<T, U>
  || is_lt_comparable_v<U, T> && is_eq_comparable_v<U, T>;

struct greater_equal_t {
  // Note: !(t < u) may be incorrect behavior for partial ordering
  template <class T, class U>
    requires (is_greater_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_greater_equal(t, u);
    } else if constexpr (is_ge_comparable_v<T, U>) {
      return t >= u;
    } else if constexpr (is_le_comparable_v<U, T>) {
      return u <= t;
    } else if constexpr (is_gt_comparable_v<T, U> && is_eq_comparable_v<T, U>) {
      return t > u || t >= u;
    } else {
      return u < t || u == t;
    }
  }
};

template <class T, class U>
constexpr auto is_equal_comparable_v = is_eq_comparable_v<T, U>;

struct equal_t {
  template <class T, class U>
    requires (is_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_equal(t, u);
    } else {
      return t == u;
    }
  }
};

template <class T, class U>
constexpr auto is_not_equal_comparable_v =
     is_ne_comparable_v<T, U>
  || is_eq_comparable_v<T, U>;

struct not_equal_t {
  template <class T, class U>
    requires (is_not_equal_comparable_v<T, U>)
  static constexpr bool operator()(const T& t, const U& u) noexcept
  {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      return integral_cmp_not_equal(t, u);
    } else if constexpr (is_ne_comparable_v<T, U>) {
      return t != u;
    } else { // if constexpr (is_eq_comparable_v<T, U>)
      return !(t == u);
    }
  }
};

template <class T, class U>
constexpr auto is_compare_three_way_comparable_v =
  // (1) Integral types nees special judging
  std::is_integral_v<T> && std::is_integral_v<U>
  // (2) operator<=> is supported directly
  || is_three_way_comparable_v<T, U>
  // (3) Indirect three-way comparison
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
      return integral_cmp_three_way(t, u);
    } else if constexpr (is_three_way_comparable_v<T, U>) {
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

#endif // REFLECT_CPP26_UTILS_UTILITY_HPP
