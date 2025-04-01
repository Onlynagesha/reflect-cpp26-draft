#ifndef REFLECT_CPP26_ENUM_ENUM_CONTAINS_HPP
#define REFLECT_CPP26_ENUM_ENUM_CONTAINS_HPP

#include <reflect_cpp26/enum/impl/enum_hash_entry_search.hpp>
#include <reflect_cpp26/enum/impl/enum_value_entry_search.hpp>
#include <reflect_cpp26/utils/utility.hpp>

namespace reflect_cpp26 {
/**
 * Whether value is an entry of enum type E.
 */
template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_contains(E value) -> bool {
  return impl::enum_value_contains(value);
}

/**
 * Whether an entry of enum type E with given underlying value exists.
 */
template <class E, class I>
  requires (std::is_enum_v<E> && std::is_integral_v<I>)
constexpr auto enum_contains(I value) -> bool
{
  using UnderlyingT = std::underlying_type_t<E>;
  if (!in_range<UnderlyingT>(value)) {
    return false;
  }
  using ENoCV = std::remove_cv_t<E>;
  return enum_contains(static_cast<ENoCV>(value));
}

/**
 * Whether an entry of enum type E with given name exists.
 */
template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_contains(std::string_view str) -> bool {
  return impl::enum_hash_search<E>(str) != nullptr;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_CONTAINS_HPP
