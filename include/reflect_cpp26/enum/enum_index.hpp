#ifndef REFLECT_CPP26_ENUM_ENUM_INDEX_HPP
#define REFLECT_CPP26_ENUM_ENUM_INDEX_HPP

#include <reflect_cpp26/enum/impl/enum_value_entry_search.hpp>

namespace reflect_cpp26 {
/**
 * Gets the index of given enum value in specified order.
 * Returns npos if not value is not found.
 */
template <enum_entry_order Order = enum_entry_order::original, class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_index(E value) -> size_t
{
  const auto* pos = impl::enum_value_search(value);
  return (pos == nullptr) ? npos : pos->template index_sorted_by<Order>();
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_INDEX_HPP
