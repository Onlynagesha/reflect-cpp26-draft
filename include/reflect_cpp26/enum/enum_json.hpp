#ifndef REFLECT_CPP26_ENUM_ENUM_JSON_HPP
#define REFLECT_CPP26_ENUM_ENUM_JSON_HPP

#include <reflect_cpp26/enum/enum_entries.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/to_string_utils.h>
#include <charconv>
#include <string>

namespace reflect_cpp26 {
namespace impl {
template <class EntriesSpan>
constexpr auto enum_json_buffer_size(EntriesSpan entries) -> size_t
{
  using Entry = std::ranges::range_value_t<EntriesSpan>;
  using E = std::tuple_element_t<0, Entry>;
  // 4 : 4 punctuation characters "":, per entry
  auto size_per_entry = 4zU + reflect_cpp26_max_decimal_digits(sizeof(E));
  // 2 : 2 punctuation characters {}
  auto res = 2zU + size_per_entry * entries.size();
  for (auto [_, str]: entries) {
    res += str.size();
  }
  return res;
}

template <class EntriesSpan>
constexpr auto enum_json(EntriesSpan entries) -> std::string
{
  auto res = std::string{};
  res.resize_and_overwrite(enum_json_buffer_size(entries),
    [&entries](char* buffer_cur, size_t buffer_size) {
      auto* buffer_begin = buffer_cur;
      auto* buffer_end = buffer_cur + buffer_size;
      *buffer_cur++ = '{';
      for (auto i = 0zU, n = entries.size(); i < n; i++) {
        auto underlying = std::to_underlying(entries[i].first);
        auto name = entries[i].second;
        if (i != 0) {
          *buffer_cur++ = ',';
        }
        *buffer_cur++ = '"';
        buffer_cur = std::ranges::copy(name, buffer_cur).out;
        *buffer_cur++ = '"';
        *buffer_cur++ = ':';
        auto [ptr, ec] = std::to_chars(buffer_cur, buffer_end, underlying);
        if (std::errc{} != ec) {
          REFLECT_CPP26_UNREACHABLE("Internal error");
        }
        buffer_cur = ptr;
      }
      *buffer_cur++ = '}';
      return buffer_cur - buffer_begin;
    });
  return res;
}

template <class E, enum_entry_order Order>
constexpr auto enum_json_v =
  define_static_string(enum_json(enum_entries<E, Order>()));
} // namespace impl

/**
 * Gets the JSON representation of enum type E in compact style.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
  requires (std::is_enum_v<E>)
constexpr auto enum_json() -> std::string
{
  return impl::enum_json(enum_entries<E, Order>());
}

/**
 * Gets the JSON representation of enum type E in compact style
 * as compile-time fixed string.
 */
template <class E, enum_entry_order Order = enum_entry_order::original>
  requires (std::is_enum_v<E>)
constexpr auto enum_json_static() -> std::string_view
{
  return impl::enum_json_v<E, Order>;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_JSON_HPP
