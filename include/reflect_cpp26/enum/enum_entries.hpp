#ifndef REFLECT_CPP26_ENUM_ENUM_ENTRIES_HPP
#define REFLECT_CPP26_ENUM_ENUM_ENTRIES_HPP

#include <reflect_cpp26/enum/enum_count.hpp>
#include <reflect_cpp26/enum/enum_for_each.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <algorithm>
#include <span>
#include <utility>

namespace reflect_cpp26 {
template <enum_type E>
using enum_entry_t = std::pair<std::remove_cv_t<E>, std::string_view>;

namespace impl {
template <class E, enum_entry_order Order>
consteval auto make_enum_entries() /* -> std::array<enum_entry_t<E>, N> */
{
  auto entries = std::array<enum_entry_t<E>, enum_count_v<E>>{};
  enum_meta_for_each<E, Order>([&entries](auto index, auto ec) {
    entries[index].first = [:ec:];
    entries[index].second =
      reflect_cpp26::define_static_string(identifier_of(ec.value));
  });
  return entries;
}

template <class E, enum_entry_order Order>
constexpr auto enum_entries_v = make_enum_entries<E, Order>();
} // namespace impl

/**
 * Gets the list of (value, name) pairs.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_entries() -> std::span<const enum_entry_t<E>>
{
  return impl::enum_entries_v<std::remove_cv_t<E>, Order>;
}

/**
 * Gets the i-th (value, name) pair.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_entry(size_t index) -> enum_entry_t<E>
{
  constexpr auto entries = enum_entries<E, Order>();
  return entries[index];
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_ENTRIES_HPP
