#ifndef REFLECT_CPP26_ENUM_ENUM_CAST_HPP
#define REFLECT_CPP26_ENUM_ENUM_CAST_HPP

#include <reflect_cpp26/enum/impl/enum_hash_entry_search.hpp>
#include <reflect_cpp26/enum/impl/enum_value_entry_search.hpp>
#include <reflect_cpp26/utils/concepts.hpp>
#include <reflect_cpp26/utils/utility.hpp>

namespace reflect_cpp26 {
/**
 * Returns the value of enum type E whose name is exactly str,
 * or std::nullopt if such value does not exist in E.
 */
template <enum_type E>
constexpr auto enum_cast(std::string_view str)
  -> std::optional<std::remove_cv_t<E>>
{
  auto pos = impl::enum_hash_search<std::remove_cv_t<E>>(str);
  if (pos != nullptr) {
    return static_cast<E>(pos->value);
  }
  return std::nullopt;
}

/**
 * Casts the given integral value to enum type E
 * if value belongs to entries of E, or std::nullopt otherwise.
 */
template <enum_type E, std::integral I>
constexpr auto enum_cast(I value) -> std::optional<std::remove_cv_t<E>>
{
  using UnderlyingT = std::underlying_type_t<E>;
  if (!in_range<UnderlyingT>(value)) {
    return std::nullopt;
  }
  auto value_as_enum = static_cast<std::remove_cv_t<E>>(value);
  if (impl::enum_value_contains(value_as_enum)) {
    return static_cast<E>(value_as_enum);
  }
  return std::nullopt;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_CAST_HPP
