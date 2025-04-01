#ifndef REFLECT_CPP26_ENUM_ENUM_NAME_HPP
#define REFLECT_CPP26_ENUM_ENUM_NAME_HPP

#include <reflect_cpp26/enum/impl/enum_value_entry_search.hpp>
#include <reflect_cpp26/enum/enum_entries.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>

namespace reflect_cpp26 {
namespace impl {
template <class E, enum_entry_order Order>
constexpr auto enum_names()
{
  constexpr auto entries = enum_entries<E, Order>();
  auto res = std::array<std::string_view, enum_count_v<E>>{};
  for (size_t i = 0, n = entries.size(); i < n; i++) {
    res[i] = entries[i].second;
  }
  return res;
}

template <class E, enum_entry_order Order>
constexpr auto enum_names_v = enum_names<E, Order>();
} // namespace impl

/**
 * Gets the list of enum names.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
constexpr auto enum_names() -> std::span<const std::string_view>
{
  return impl::enum_names_v<std::remove_cv_t<E>, Order>;
}

/**
 * Gets the enum name of value, or alt if fails.
 */
template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_name(E value, std::string_view alt = {})
  -> std::string_view
{
  const auto* pos = impl::enum_value_search(value);
  return (pos == nullptr) ? alt : static_cast<std::string_view>(pos->name);
}
} // namespace reflect_cpp26

#endif // REFLECT
