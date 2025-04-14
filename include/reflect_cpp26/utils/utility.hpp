#ifndef REFLECT_CPP26_UTILS_UTILITY_HPP
#define REFLECT_CPP26_UTILS_UTILITY_HPP

#include <compare>
#include <concepts>
#include <functional>
#include <limits>

namespace reflect_cpp26 {
// -------- Relaxed alternative of integer comparison  --------
// bool and character types are allowed

struct cmp_equal_t {
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

struct cmp_not_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !cmp_equal_t::operator()(t, u);
  }
};

struct cmp_less_t {
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

struct cmp_greater_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return cmp_less_t::operator()(u, t);
  }
};

struct cmp_less_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !cmp_less_t::operator()(u, t);
  }
};

struct cmp_greater_equal_t {
  template <std::integral T, std::integral U>
  static constexpr bool operator()(T t, U u) noexcept {
    return !cmp_less_t::operator()(t, u);
  }
};

// Integral three-way comparison is named with 'cmp_' prefix
// analogous to other integral comparators in C++ standard library.
struct cmp_three_way_t {
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
    return cmp_greater_equal_t::operator()(t, limit_min)
      && cmp_less_equal_t::operator()(t, limit_max);
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
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_UTILITY_HPP
