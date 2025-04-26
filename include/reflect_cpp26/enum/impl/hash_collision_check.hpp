#ifndef REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP
#define REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP

#include <reflect_cpp26/enum/enum_for_each.hpp>
#include <reflect_cpp26/utils/string_hash.hpp>
#include <algorithm>

namespace reflect_cpp26::impl {
template <class E>
consteval bool enum_name_has_hash_collision()
{
  if constexpr (enum_count<E>() != 0) {
    auto hash_values = enum_meta_entries<E>().map([](auto ec) {
      return bkdr_hash64(identifier_of(ec.value));
    });
    std::ranges::sort(hash_values);
    return hash_values[0] == 0
      || std::ranges::adjacent_find(hash_values) != hash_values.end();
  } else {
    return false;
  }
}

// Hash collision if either happens:
// (1) Hash value is zero in some entries;
// (2) Multiple entries share the same hash value.
template <class E>
constexpr auto enum_name_has_hash_collision_v =
  enum_name_has_hash_collision<E>();
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP
