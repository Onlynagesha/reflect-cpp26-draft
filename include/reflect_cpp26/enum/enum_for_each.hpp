#ifndef REFLECT_CPP26_ENUM_ENUM_FOR_EACH_HPP
#define REFLECT_CPP26_ENUM_ENUM_FOR_EACH_HPP

#include <reflect_cpp26/enum/enum_values.hpp>

namespace reflect_cpp26 {
/**
 * Performs for-each operation to values of enum type E with given order.
 * Supported signatures:
 *   func(constant<i>, constant<ei>)
 *     where ei is the i-th value as E
 *   func(constant<ei>)
 * See constant::for_each for more details.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original,
          class Func>
constexpr auto enum_for_each(Func&& func) -> void {
  enum_value_constants<E, Order>().for_each(std::forward<Func>(func));
}

/**
 * Performs for-each operation to values of enum type E with given order.
 * Supported signatures:
 *   func(constant<i>, constant<meta_ei>)
 *     where meta_ei is the i-th enumerator as std::meta::info
 *   func(constant<meta_ei>)
 * See constant::for_each for more details.
 */
template <enum_type E, enum_entry_order Order = enum_entry_order::original,
          class Func>
constexpr auto enum_meta_for_each(Func&& func) -> void {
  enum_meta_entries<E, Order>().for_each(std::forward<Func>(func));
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_FOR_EACH_HPP
