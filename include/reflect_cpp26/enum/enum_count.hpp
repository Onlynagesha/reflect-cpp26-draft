#ifndef REFLECT_CPP26_ENUM_ENUM_COUNT_HPP
#define REFLECT_CPP26_ENUM_ENUM_COUNT_HPP

#include <reflect_cpp26/utils/concepts.hpp>
#include <reflect_cpp26/utils/config.h>

namespace reflect_cpp26 {
template <class E>
constexpr auto enum_count_v = enumerators_of(^^E).size();

/**
 * Gets the number of entries of enum type E.
 */
template <enum_type E>
constexpr auto enum_count() -> size_t {
  return enum_count_v<E>;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_COUNT_HPP
