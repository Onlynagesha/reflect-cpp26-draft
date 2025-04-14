#ifndef REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP
#define REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP

#include <reflect_cpp26/enum/enum_for_each.hpp>
#include <reflect_cpp26/utils/string_hash.hpp>
#include <algorithm>

namespace reflect_cpp26::impl {
template <class E>
constexpr bool enum_name_has_hash_collision()
{
  auto hash_values = std::array<uint64_t, enum_count_v<E>>{};
  enum_meta_for_each<E>([&hash_values](size_t index, auto ec) {
    hash_values[index] = bkdr_hash64(std::meta::identifier_of(ec));
  });
  std::ranges::sort(hash_values);
  auto zero_pos = std::ranges::find(hash_values, 0);
  auto collision_pos = std::ranges::adjacent_find(hash_values);
  return zero_pos != hash_values.end() || collision_pos != hash_values.end();
}

// Hash collision if either happens:
// (1) Hash value is zero in some entries;
// (2) Multiple entries share the same hash value.
template <class E>
constexpr auto enum_name_has_hash_collision_v =
  enum_name_has_hash_collision<E>();
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_ENUM_IMPL_HASH_COLLISION_CHECK_HPP
