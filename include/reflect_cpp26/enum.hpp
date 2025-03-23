#ifndef REFLECT_CPP26_ENUM_HPP
#define REFLECT_CPP26_ENUM_HPP

#include "reflect_cpp26/impl/enum/enum_count.hpp"
#include "reflect_cpp26/impl/enum/enum_hash.hpp"
#include <format>

namespace reflect_cpp26 {
enum class enum_entry_order {
  original,
  by_value,
  by_name,
};

template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_count() -> size_t
{
  return impl::enum_count_v<E>;
}

template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_hash() -> uint64_t
{
  return impl::enum_hash_v<E>;
}

template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_json(enum_entry_order order) noexcept -> std::string
{
  switch (order) {
  case enum_entry_order::original:
    return impl::enum_json_original_order<E>();
  case enum_entry_order::by_value:
    return impl::enum_json_sorted_by_value<E>();
  case enum_entry_order::by_name:
    return impl::enum_json_sorted_by_name<E>();
  default:
    return std::format("<ERROR:invalid-enum-entry-order({})>",
                       std::to_underlying(order));
  }
}

template <class E, enum_entry_order order>
  requires (std::is_enum_v<E>)
constexpr auto enum_json_static() -> meta_string_view
{
  if constexpr (order == enum_entry_order::original) {
    return impl::enum_json_original_order_v<E>;
  } else if constexpr (order == enum_entry_order::by_value) {
    return impl::enum_json_sorted_by_value_v<E>;
  } else if constexpr (order == enum_entry_order::by_name) {
    return impl::enum_json_sorted_by_name_v<E>;
  } else {
    static_assert(false, "Invalid enum entry order.");
  }
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_HPP
