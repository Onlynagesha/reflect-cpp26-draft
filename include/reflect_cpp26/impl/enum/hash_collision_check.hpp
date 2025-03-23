#ifndef REFLECT_CPP26_IMPL_ENUM_HASH_COLLISION_CHECK_HPP
#define REFLECT_CPP26_IMPL_ENUM_HASH_COLLISION_CHECK_HPP

#include <reflect_cpp26/impl/enum/enum_count.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/string_hash.h>
#include <algorithm>

namespace reflect_cpp26::impl {
template <class E>
constexpr bool enum_name_has_hash_collision()
{
  auto hash_values = std::array<uint64_t, enum_count_v<E>>{};
  REFLECT_CPP26_EXPAND(enumerators_of(^^E)).for_each(
    [&hash_values](size_t index, auto ec) {
      hash_values[index] = bkdr_hash64(std::meta::identifier_of(ec));
    });
  std::ranges::sort(hash_values);
  auto collision_pos = std::ranges::adjacent_find(hash_values);
  return collision_pos != hash_values.end();
}

template <class E>
constexpr auto enum_name_has_hash_collision_v =
  enum_name_has_hash_collision<E>();
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_IMPL_ENUM_HASH_COLLISION_CHECK_HPP
