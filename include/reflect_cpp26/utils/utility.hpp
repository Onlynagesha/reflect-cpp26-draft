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
concept three_way_comparision_result = is_three_way_comparison_result_v<T>;

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
  { t <=> u } -> three_way_comparision_result;
};

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

// -------- Relaxed alternative of generic comparision --------
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

// -------- Memberwise comparision --------

namespace impl {
consteval bool is_memberwise_three_way_comparable_type(
  std::meta::info T, bool recursion_flag = false)
{
  // if (!extract_bool(^^std::is_class_v, T) ||
  //     !extract_bool(^^std::is_aggregate_v, T)) {
  //   return false;
  // }
  // auto all_bases_are_comparable = std::ranges::all_of(all_direct_bases_of(T),
  //   [](std::meta::info base) {
  //     return is_memberwise_three_way_comparable_type(base);
  //   });
  // if (!all_bases_are_comparable) {
  //   return false;
  // }
  // if (!recursion_flag) {
  //   auto no_member_is_union = std::ranges::none_of(
  //     all_direct_nonstatic_data_members_of(T),
  //     [](std::meta::info member) {
  //       return is_union(type_of(member));
  //     });
  //   return no_member_is_union;
  // }
  return true;
}

template <class T>
constexpr auto is_memberwise_three_way_comparable_type_v =
  is_memberwise_three_way_comparable_type(^^T);

consteval auto get_memberwise_compare_three_way_result_type(
  std::meta::info T, std::meta::info U) -> std::meta::info
{
  if (!extract_bool(^^is_memberwise_three_way_comparable_type_v, T) ||
      !extract_bool(^^is_memberwise_three_way_comparable_type_v, U)) {
    return std::meta::info{};
  }
  auto to_type_of = std::views::transform(std::meta::type_of)
    | std::ranges::to<std::vector>();
  auto Ts = nonstatic_data_members_of(T, unchecked_context()) | to_type_of;
  auto Us = nonstatic_data_members_of(U, unchecked_context()) | to_type_of;
  if (size(Ts) != size(Us)) {
    return std::meta::info{};
  }
  auto results = std::vector<std::meta::info>{};
  results.reserve(size(Ts));
  for (auto [t, u]: std::views::zip(Ts, Us)) {
    if (!extract_bool(^^is_three_way_comparable_v, t, u)) {
      return std::meta::info{};
    }
    results.push_back(substitute(^^compare_three_way_result_t, t, u));
  }
  return substitute(^^std::common_type_t, results);
}

template <class T, class U>
constexpr auto memberwise_compare_three_way_result_type_meta_v =
  get_memberwise_compare_three_way_result_type(^^T, ^^U);
} // namespace impl

/**
 * Whether T and U can be three-way compared memberwise:
 * (1) Both T and U must be aggregate class types that:
 *     * Every base class satisfies condition (1) recursively;
 *     * 
 * Let MemT[NT] be the list of members
 */
template <class T, class U>
constexpr auto is_memberwise_three_way_comparable_v =
  impl::memberwise_compare_three_way_result_type_meta_v<T, U>
    != std::meta::info{};

template <class T, class U>
using memberwise_compare_three_way_result_t =
  [: impl::memberwise_compare_three_way_result_type_meta_v<T, U> :];

struct memberwise_compare_three_way_t {
  template <class T, class U>
    requires (is_memberwise_three_way_comparable_v<T, U>)
  static constexpr auto operator()(const T& t, const U& u)
    /* -> decltype(res) */
  {
    auto res = static_cast<memberwise_compare_three_way_result_t<T, U>>(
      std::strong_ordering::equal);
    REFLECT_CPP26_EXPAND(nonstatic_data_members_of(^^T, unchecked_context()))
      .for_each([&res, &t, &u](auto index, auto mt) {
        constexpr auto mu =
          nonstatic_data_members_of(^^U, unchecked_context())[index];
        res = t.[:mt:] <=> u.[:mu:];
        return res == 0; // Stops at first unequal result
      });
    return res;
  }
};

constexpr auto memberwise_compare_three_way = memberwise_compare_three_way_t{};
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_UTILITY_HPP
