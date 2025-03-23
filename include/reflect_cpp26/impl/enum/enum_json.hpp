#ifndef REFLECT_CPP26_IMPL_ENUM_ENUM_JSON_HPP
#define REFLECT_CPP26_IMPL_ENUM_ENUM_JSON_HPP

#include <reflect_cpp26/impl/enum/enum_count.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <array>
#include <charconv>
#include <string>

namespace reflect_cpp26::impl {
template <class E>
using enum_kv_pair_t = std::pair<meta_string_view, std::underlying_type_t<E>>;

template <class KVPairRange>
constexpr auto enum_json(const KVPairRange& kv_pairs) noexcept -> std::string
{
  auto res = std::string{"{"};
  auto buffer = std::array<char, 24>{}; // 24 : Buffer size enough for int64
  for (auto [k, v]: kv_pairs) {
    if (res.length() > 1) {
      res.push_back(',');
    }
    res.push_back('"');
    res.append_range(k);
    res.append("\":");
    auto [ptr, ec] = std::to_chars(buffer.begin(), buffer.end(), v);
    if (std::errc{} != ec) {
      const auto* err_message =
        "<Internal error: failure during integer to string conversion>";
      if consteval {
        throw err_message;
      } else {
        return err_message;
      }
    }
    res.append(buffer.begin(), ptr);
  }
  res.push_back('}');
  return res;
}

template <class E>
constexpr auto make_enum_kv_pairs() /* -> std::array<enum_kv_pair_t<E>, N> */
{
  auto kv_pairs = std::array<enum_kv_pair_t<E>, enum_count_v<E>>{};
  REFLECT_CPP26_EXPAND(enumerators_of(^^E)).for_each(
    [&kv_pairs](size_t index, auto ec) {
      kv_pairs[index] = {
        std::meta::identifier_of(ec),
        std::to_underlying([:ec:]),
      };
    });
  return kv_pairs;
}

template <class E>
constexpr auto enum_json_original_order() -> std::string
{
  auto kv_pairs = make_enum_kv_pairs<E>();
  return enum_json(kv_pairs);
}

template <class E>
constexpr auto enum_json_sorted_by_value() -> std::string
{
  auto kv_pairs = make_enum_kv_pairs<E>();
  std::ranges::sort(kv_pairs, {}, &decltype(kv_pairs)::value_type::second);
  return enum_json(kv_pairs);
}

template <class E>
constexpr auto enum_json_sorted_by_name() -> std::string
{
  auto kv_pairs = make_enum_kv_pairs<E>();
  std::ranges::sort(kv_pairs, {}, &decltype(kv_pairs)::value_type::first);
  return enum_json(kv_pairs);
}

template <class E>
constexpr auto enum_json_original_order_v =
  define_static_string(enum_json_original_order<E>());

template <class E>
constexpr auto enum_json_sorted_by_value_v =
  define_static_string(enum_json_sorted_by_value<E>());

template <class E>
constexpr auto enum_json_sorted_by_name_v =
  define_static_string(enum_json_sorted_by_name<E>());
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_IMPL_ENUM_ENUM_JSON_HPP
