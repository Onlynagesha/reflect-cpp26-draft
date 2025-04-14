#ifndef REFLECT_CPP26_ENUM_ENUM_HASH_HPP
#define REFLECT_CPP26_ENUM_ENUM_HASH_HPP

#include <reflect_cpp26/enum/enum_json.hpp>
#include <reflect_cpp26/utils/string_hash.hpp>

namespace reflect_cpp26 {
namespace impl {
template <class E>
constexpr auto enum_hash()
{
  constexpr auto Order = enum_entry_order::by_name;
  auto repr_str = reflect_cpp26::enum_json<E, Order>();
  return bkdr_hash64(repr_str);
}

template <class E>
constexpr auto enum_hash_v = enum_hash<E>();
} // namespace impl

/**
 * Gets the 64-bit hash value of enum type E.
 * Any change to the entry set (including insertion, deletion and
 * modification to name or value) will change the hash value.
 * But only changing entry order will not.
 */
template <enum_type E>
constexpr auto enum_hash() -> uint64_t {
  return impl::enum_hash_v<std::remove_cv_t<E>>;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_HASH_HPP
