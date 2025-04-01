#ifndef REFLECT_CPP26_ENUM_ENUM_TYPE_NAME_HPP
#define REFLECT_CPP26_ENUM_ENUM_TYPE_NAME_HPP

#include <reflect_cpp26/utils/config.h>

namespace reflect_cpp26 {
/**
 * Gets the enum type name (without cv-qualifiers and namespaces).
 */
template <class E>
  requires (std::is_enum_v<E>)
constexpr auto enum_type_name() -> std::string_view
{
  using ENoCV = std::remove_cv_t<E>;
  return identifier_of(dealias(^^ENoCV));
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_TYPE_NAME_HPP
