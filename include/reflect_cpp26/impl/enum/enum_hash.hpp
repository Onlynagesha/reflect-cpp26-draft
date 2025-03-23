#ifndef REFLECT_CPP26_IMPL_ENUM_ENUM_HASH_HPP
#define REFLECT_CPP26_IMPL_ENUM_ENUM_HASH_HPP

#include <reflect_cpp26/impl/enum/enum_json.hpp>
#include <reflect_cpp26/utils/string_hash.h>

namespace reflect_cpp26::impl {
template <class E>
constexpr auto enum_hash()
{
  auto repr_str = enum_json_sorted_by_name<E>();
  return bkdr_hash64(repr_str.data(), repr_str.data() + repr_str.length());
}

template <class E>
constexpr auto enum_hash_v = enum_hash<E>();
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_IMPL_ENUM_ENUM_HASH_HPP
