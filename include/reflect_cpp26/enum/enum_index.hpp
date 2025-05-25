#ifndef REFLECT_CPP26_ENUM_ENUM_INDEX_HPP
#define REFLECT_CPP26_ENUM_ENUM_INDEX_HPP

#include <reflect_cpp26/enum/impl/enum_value_entry_search.hpp>
#include <reflect_cpp26/utils/concepts.hpp>

namespace reflect_cpp26 {
/**
 * Gets the index of given enum value in specified order.
 * Returns npos if the value is not found.
 */
template <enum_entry_order Order = enum_entry_order::original, enum_type E>
constexpr auto enum_index(E value) -> size_t
{
  const auto* pos = impl::enum_value_search(value);
  return (pos == nullptr) ? npos : pos->template index_sorted_by<Order>();
}

/**
 * Gets the index of given enum value in value order, duplicated value removed.
 * Returns npos if the value is not found.
 */
template <enum_type E>
constexpr auto enum_unique_index(E value) -> size_t
{
  const auto* pos = impl::enum_value_search(value);
  return (pos == nullptr) ? npos : pos->index_sorted_by_value_unique;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_INDEX_HPP
