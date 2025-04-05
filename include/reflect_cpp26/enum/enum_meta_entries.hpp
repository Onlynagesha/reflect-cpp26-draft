#ifndef REFLECT_CPP26_ENUM_ENUM_META_ENTRIES_HPP
#define REFLECT_CPP26_ENUM_ENUM_META_ENTRIES_HPP

#include <reflect_cpp26/enum/enum_count.hpp>
#include <reflect_cpp26/enum/enum_entry_order.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <algorithm>
#include <ranges>
#include <utility>

namespace reflect_cpp26 {
namespace impl {
template <class E>
consteval auto make_enum_meta_entries_original_order()
  /* -> constant<std::meta::info...> */
{
  return REFLECT_CPP26_EXPAND(enumerators_of(^^E));
}

template <class E>
consteval auto make_enum_meta_entries_sorted_by_value_impl()
  /* -> std::array<std::meta::info, N> */
{
  constexpr auto orig_order = make_enum_meta_entries_original_order<E>();
  constexpr auto N = orig_order.size();
  if constexpr (N == 0) {
    return std::array<std::meta::info, N>{};
  } else {
    using iv_pair_t = std::pair<size_t, std::underlying_type_t<E>>;
    auto iv_pairs = std::array<iv_pair_t, N>{};
    orig_order.for_each([&iv_pairs](auto i, auto e) {
      auto value = std::to_underlying([:e:]);
      iv_pairs[i] = {i, value};
    });
    std::ranges::sort(iv_pairs, {}, &iv_pair_t::second);
    auto res = std::array<std::meta::info, N>{};
    for (auto i = 0zU; i < N; i++) {
      res[i] = orig_order.values[iv_pairs[i].first];
    }
    return res;
  }
}

template <class E>
consteval auto make_enum_meta_entries_sorted_by_value()
  /* -> constant<std::meta::info...> */
{
  return REFLECT_CPP26_EXPAND(
    make_enum_meta_entries_sorted_by_value_impl<E>());
}

template <class E>
consteval auto make_enum_meta_entries_sorted_by_name_impl()
  /* -> std::array<std::meta::info, N> */
{
  constexpr auto orig_order = make_enum_meta_entries_original_order<E>();
  constexpr auto N = orig_order.size();
  if constexpr (N == 0) {
    return std::array<std::meta::info, N>{};
  } else {
    using iv_pair_t = std::pair<size_t, std::string_view>;
    auto iv_pairs = std::array<iv_pair_t, N>{};
    orig_order.for_each([&iv_pairs](auto i, auto e) {
      auto value = identifier_of(e.value);
      iv_pairs[i] = {i, value};
    });
    std::ranges::sort(iv_pairs, {}, &iv_pair_t::second);
    auto res = std::array<std::meta::info, N>{};
    for (auto i = 0zU; i < N; i++) {
      res[i] = orig_order.values[iv_pairs[i].first];
    }
    return res;
  }
}

template <class E>
consteval auto make_enum_meta_entries_sorted_by_name()
  /* -> constant<std::meta::info...> */
{
  return REFLECT_CPP26_EXPAND(
    make_enum_meta_entries_sorted_by_name_impl<E>());
}

template <class E, enum_entry_order Order>
constexpr auto enum_meta_entries_v =
  reflect_cpp26_raise_error_if_consteval("Invalid enum entry order.");

template <class E>
constexpr auto enum_meta_entries_v<E, enum_entry_order::original> =
  make_enum_meta_entries_original_order<E>();

template <class E>
constexpr auto enum_meta_entries_v<E, enum_entry_order::by_value> =
  make_enum_meta_entries_sorted_by_value<E>();

template <class E>
constexpr auto enum_meta_entries_v<E, enum_entry_order::by_name> =
  make_enum_meta_entries_sorted_by_name<E>();
} // namespace impl

/**
 * Gets the reflector list of enumerators in E with given order.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
consteval auto enum_meta_entries() /* constants<std::meta::info...> */
{
  return impl::enum_meta_entries_v<std::remove_cv_t<E>, Order>;
}

/**
 * Gets the i-th reflector of enumerators in E with given order.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
consteval auto enum_meta_entry(size_t index) -> std::meta::info
{
  constexpr auto entries = enum_meta_entries<E, Order>();
  return entries.values[index];
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_META_ENTRIES_HPP
