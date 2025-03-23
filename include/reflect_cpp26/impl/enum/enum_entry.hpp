#ifndef REFLECT_CPP26_IMPL_ENUM_ENUM_ENTRY_HPP
#define REFLECT_CPP26_IMPL_ENUM_ENUM_ENTRY_HPP

#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/for_each.hpp>
#include <reflect_cpp26/utils/string_hash.h>
#include <bit>
#include <utility>

namespace reflect_cpp26::impl {
template <class Derived>
struct enum_entry_interface {
  template <class T>
    /* requires std::is_integral_v<T> || std::is_enum_v<T> */
  static constexpr uintmax_t make_value(T input)
  {
    if constexpr (std::is_enum_v<T>) {
      return make_value(std::to_underlying(input));
    } else if constexpr (std::is_signed_v<T>) {
      auto s = static_cast<intmax_t>(input); // Sign bit extension
      return static_cast<uintmax_t>(s);
    } else {
      return static_cast<uintmax_t>(input); // Zero extension
    }
  }

  constexpr intmax_t value_as_signed() const {
    return static_cast<intmax_t>(static_cast<const Derived*>(this)->value);
  }

  template <class T>
    /* requires std::is_integral_v<T> || std::is_enum_v<T> */
  constexpr auto value_as_signedness_of(T) const
  {
    if constexpr (std::is_enum_v<T>) {
      return value_as_signedness_of(std::underlying_type_t<T>{});
    } else if constexpr (std::is_signed_v<T>) {
      return value_as_signed();
    } else {
      return static_cast<const Derived*>(this)->value;
    }
  }
};

constexpr size_t enum_value_entry_alignment = std::bit_ceil(
  sizeof(uintmax_t) + sizeof(meta_string_view));

struct alignas(enum_value_entry_alignment)
enum_value_entry : enum_entry_interface<enum_value_entry> {
  uintmax_t value; // value as primary key
  meta_string_view name;
};

constexpr size_t enum_hash_entry_alignment = std::bit_ceil(
  sizeof(uint64_t) + sizeof(uintmax_t) + sizeof(meta_string_view));

struct alignas(enum_hash_entry_alignment)
enum_hash_entry : enum_entry_interface<enum_hash_entry> {
  uint64_t name_hash; // name hash as primary key
  uintmax_t value;
  meta_string_view name;

  static constexpr auto compare_by_hash_strong_order(
    const enum_hash_entry& x, const enum_hash_entry& y) -> std::strong_ordering
  {
    auto cmp_hash = (x.name_hash <=> y.name_hash);
    if (cmp_hash != 0) {
      return cmp_hash;
    }
    return x.name <=> y.name;
  }

  static constexpr auto less_by_hash_strong_order(
    const enum_hash_entry& x, const enum_hash_entry& y) -> bool
  {
    return compare_by_hash_strong_order(x, y) < 0;
  }
};

template <class E>
consteval auto make_enum_value_entry_list(sorting_policy policy)
  -> std::vector<enum_value_entry>
{
  using U = std::underlying_type_t<E>;
  auto entry_list = std::vector<enum_value_entry>{};
  REFLECT_CPP26_EXPAND(enumerators_of(^^E)).for_each([&entry_list](auto ec) {
    entry_list.push_back({
      .value = enum_value_entry::make_value([:ec:]),
      .name = std::meta::identifier_of(ec),
    });
  });
  if constexpr (std::is_signed_v<U>) {
    std::ranges::sort(entry_list, {}, &enum_value_entry::value_as_signed);
  } else {
    std::ranges::sort(entry_list, {}, &enum_value_entry::value);
  }
  auto [s, t] = std::ranges::unique(entry_list, {}, &enum_value_entry::value);
  entry_list.erase(s, t);
  return entry_list;
}

template <class E>
consteval auto make_enum_hash_entry_list() -> std::vector<enum_hash_entry>
{
  auto entry_list = std::vector<enum_hash_entry>{};
  REFLECT_CPP26_EXPAND(enumerators_of(^^E)).for_each([&entry_list](auto ec) {
    auto name = std::meta::identifier_of(ec);
    entry_list.push_back({
      .name_hash = bkdr_hash64(name),
      .value = enum_hash_entry::make_value([:ec:]),
      .name = name,
    });
  });
  std::ranges::sort(entry_list, &enum_hash_entry::less_by_hash_strong_order);
  return define_static_array(entry_list);
}

template <class E>
constexpr auto enum_value_entry_list_v =
  define_static_array(make_enum_hash_entry_list<E>());

template <class E>
constexpr auto enum_hash_entry_list_v =
  define_static_array(make_enum_hash_entry_list<E>());
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_IMPL_ENUM_ENUM_ENTRY_HPP
