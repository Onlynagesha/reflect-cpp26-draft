#ifndef REFLECT_CPP26_UTILS_UTILITY_HPP
#define REFLECT_CPP26_UTILS_UTILITY_HPP

#include <reflect_cpp26/utils/meta_utility.hpp>
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

#define REFLECT_CPP26_UTILITY_COMPARISON_TRAITS_FOR_EACH(F)   \
  F(eq_comparable)                                            \
  F(ne_comparable)                                            \
  F(lt_comparable)                                            \
  F(le_comparable)                                            \
  F(gt_comparable)                                            \
  F(ge_comparable)                                            \
  F(three_way_comparable)

template <class T, class U>
constexpr bool is_eq_comparable_v = requires (const T& t, const U& u) {
  { t == u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_ne_comparable_v = requires (const T& t, const U& u) {
  { t != u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_lt_comparable_v = requires (const T& t, const U& u) {
  { t < u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_le_comparable_v = requires (const T& t, const U& u) {
  { t <= u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_gt_comparable_v = requires (const T& t, const U& u) {
  { t > u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_ge_comparable_v = requires (const T& t, const U& u) {
  { t >= u } -> std::convertible_to<bool>;
};

template <class T, class U>
constexpr bool is_three_way_comparable_v = requires (const T& t, const U& u) {
  { t <=> u } -> three_way_comparison_result;
};

#define REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE(type)   \
  template <class T, class U>                                     \
  struct is_##type : std::bool_constant<is_##type##_v<T, U>> {};

REFLECT_CPP26_UTILITY_COMPARISON_TRAITS_FOR_EACH(
  REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE)

#undef REFLECT_CPP26_UTILITY_COMPARISON_TRAITS_FOR_EACH
#undef REFLECT_CPP26_UTILITY_MAKE_COMPARISON_TRAITS_TYPE

template <class T, class U>
using compare_three_way_result_t =
  decltype(std::declval<T>() <=> std::declval<U>());

// -------- Relaxed alternative of integer comparison  --------
// bool and character types are allowed

struct cmp_equal_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
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

struct cmp_not_equal_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
  static constexpr bool operator()(T t, U u) noexcept {
    return cmp_equal_t::operator()(t, u);
  }
};

struct cmp_less_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
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

struct cmp_greater_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
  static constexpr bool operator()(T t, U u) noexcept {
    return cmp_less_t::operator()(u, t);
  }
};

struct cmp_less_equal_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
  static constexpr bool operator()(T t, U u) noexcept {
    return !cmp_less_t::operator()(u, t);
  }
};

struct cmp_greater_equal_t {
  template<class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
  static constexpr bool operator()(T t, U u) noexcept {
    return !cmp_less_t::operator()(t, u);
  }
};

struct cmp_three_way_t {
  template <class T, class U>
    requires (std::is_integral_v<T> && std::is_integral_v<U>)
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

template <class R>
  requires (std::is_integral_v<R>)
struct in_range_t {
  template<class T>
    requires (std::is_integral_v<T>)
  static constexpr bool operator()(T t) noexcept
  {
    return cmp_greater_equal_t::operator()(t, std::numeric_limits<R>::min())
      && cmp_less_equal_t::operator()(t, std::numeric_limits<R>::max());
  }
};

constexpr auto cmp_less = cmp_less_t{};
constexpr auto cmp_greater = cmp_greater_t{};
constexpr auto cmp_less_equal = cmp_less_equal_t{};
constexpr auto cmp_greater_equal = cmp_greater_equal_t{};
constexpr auto cmp_equal = cmp_equal_t{};
constexpr auto cmp_not_equal = cmp_not_equal_t{};
constexpr auto cmp_three_way = cmp_three_way_t{};
template <class R>
constexpr auto in_range = in_range_t<R>{};

// -------- Relaxed alternative of generic comparison --------
// Less constraints than std::ranges::less, etc. and std::compare_three_way.

struct less_t {
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_lt_comparable_v<T, U>) {
      return t < u;
    } else {
      static_assert(false, "t < u not comparable.");
    }
  }
};

struct greater_t {
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_gt_comparable_v<T, U>) {
      return t > u;
    } else if constexpr (is_lt_comparable_v<U, T>) {
      return u < t;
    } else {
      static_assert(false, "t > u not comparable.");
    }
  }
};

struct less_equal_t {
  // Note: !(u < t) may be incorrect behavior for partial ordering.
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_le_comparable_v<T, U>) {
      return t <= u;
    } else if constexpr (is_lt_comparable_v<T, U> && is_eq_comparable_v<T, U>) {
      return t == u || t < u;
    } else {
      static_assert(false, "t <= u not comparable.");
    }
  }
};

struct greater_equal_t {
  // Note: !(t < u) may be incorrect behavior for partial ordering
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_ge_comparable_v<T, U>) {
      return t >= u;
    } else if constexpr (is_le_comparable_v<U, T>) {
      return u <= t;
    } else if constexpr (is_gt_comparable_v<T, U> && is_eq_comparable_v<T, U>) {
      return t == u || t > u;
    } else if constexpr (is_lt_comparable_v<U, T> && is_eq_comparable_v<U, T>) {
      return u == t || u < t;
    } else {
      static_assert(false, "t >= u not comparable.");
    }
  }
};

struct equal_t {
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_eq_comparable_v<T, U>) {
      return t == u;
    } else {
      static_assert(false, "t == u not comparable.");
    }
  }
};

struct not_equal_t {
  template <class T, class U>
  static constexpr bool operator()(const T& t, const U& u) {
    if constexpr (is_ne_comparable_v<T, U>) {
      return t != u;
    } else if constexpr (is_eq_comparable_v<T, U>) {
      return !(t == u);
    } else {
      static_assert(false, "t != u not comparable.");
    }
  }
};

struct compare_three_way_t {
  // Note: For types where operator<=> is not supported,
  // the most conservative comparison policy std::partial_ordering is used
  // since we don't know how the operands are compared semantically.
  template <class T, class U>
  static constexpr auto operator()(const T& t, const U& u) {
    if constexpr (is_three_way_comparable_v<T, U>) {
      return t <=> u;
    } else {
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
  }
};

constexpr auto less = less_t{};
constexpr auto greater = greater_t{};
constexpr auto less_equal = less_equal_t{};
constexpr auto greater_equal = greater_equal_t{};
constexpr auto equal = equal_t{};
constexpr auto not_equal = not_equal_t{};
constexpr auto compare_three_way = compare_three_way_t{};
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_UTILITY_HPP
