#ifndef REFLECT_CPP26_ENUM_ENUM_VALUES_HPP
#define REFLECT_CPP26_ENUM_ENUM_VALUES_HPP

#include <reflect_cpp26/enum/enum_meta_entries.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <ranges>

namespace reflect_cpp26 {
namespace impl {
template <class E, enum_entry_order Order>
constexpr auto enum_values_v =
  enum_meta_entries<E, Order>().template map<extract_meta_value>();
} // namespace impl

/**
 * Gets the list of enum values.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_values() -> std::span<const std::remove_cv_t<E>>
{
  return impl::enum_values_v<std::remove_cv_t<E>, Order>.values;
}

/**
 * Gets the i-th enum value.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_value(size_t index) -> std::remove_cv_t<E>
{
  constexpr auto values = enum_values<E, Order>();
  return values[index];
}

/**
 * Gets the list of enum values as constant<values...>.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_value_constants() /* -> constant<e1, e2...> */
{
  return impl::enum_values_v<std::remove_cv_t<E>, Order>;
}

/**
 * Gets the i-th enum value as constant<value>.
 */
template <size_t I, enum_type E,
          enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_value_constant() /* -> constant<ei> */
{
  return constant<enum_value<E, Order>(I)>{};
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_VALUES_HPP
