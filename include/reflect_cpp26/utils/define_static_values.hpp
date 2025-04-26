#ifndef REFLECT_CPP26_UTILS_DEFINE_STATIC_VALUES_HPP
#define REFLECT_CPP26_UTILS_DEFINE_STATIC_VALUES_HPP

#include <reflect_cpp26/type_traits/arithmetic_types.hpp>
#include <reflect_cpp26/utils/config.h>
#include <reflect_cpp26/utils/meta_span.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <iterator>
#include <ranges>

namespace reflect_cpp26 {
namespace impl {
template <auto V>
using meta_span_type_t = meta_span<
  std::ranges::range_value_t<decltype(V)>>;

template <auto V>
using meta_string_view_type_t = meta_basic_string_view<
  std::ranges::range_value_t<decltype(V)>>;

template <auto V>
constexpr auto array_to_span_v =
  meta_span_type_t<V>::from_array(V);

template <auto V>
constexpr auto array_to_string_view_v =
  meta_string_view_type_t<V>::from_array(V);

template <auto V>
constexpr const auto* meta_object_v = &V;

template <size_t N, class T, class Range>
consteval auto define_static_array_impl(Range& range) -> meta_span<T>
{
  auto arr = std::array<T, N>{};
  std::ranges::copy(range, arr.begin());
  auto span = extract<meta_span<T>>(
    substitute(^^array_to_span_v, { std::meta::reflect_value(arr) }));
  return span;
}

template <size_t N, class T, class Range>
consteval auto define_static_string_impl(Range& range)
  -> meta_basic_string_view<T>
{
  auto arr = std::array<T, N + 1>{};
  std::ranges::copy(range, arr.begin());
  arr.back() = '\0';
  auto sv = extract<meta_basic_string_view<T>>(
    substitute(^^array_to_string_view_v, { std::meta::reflect_value(arr) }));
  return sv;
}

template <class BidirRange>
consteval auto try_remove_trailing_null_characters(
  const BidirRange& range, bool removes_trailing_null_characters)
{
  if (!removes_trailing_null_characters) {
    return std::ranges::subrange{range};
  }
  auto start_pos = std::ranges::begin(range);
  auto end_pos = std::ranges::end(range);
  for (; start_pos != end_pos; ) {
    auto prev = std::ranges::prev(end_pos);
    if (*prev != '\0') {
      break; // end_pos stops at the first '\0'
    }
    end_pos = prev;
  }
  return std::ranges::subrange{start_pos, end_pos};
}
} // namespace impl

/**
 * Alternative to C++26 std::meta::define_static_array (if adopted).
 */
template <class Range>
  requires (std::ranges::input_range<Range>)
consteval auto define_static_array(Range&& range) /* -> meta_span<T> */
{
  using T = std::ranges::range_value_t<Range>;
  using ResultT = meta_span<T>;

  if constexpr (std::ranges::forward_range<Range>) {
    if (std::ranges::empty(range)) {
      return meta_span<T>{};
    }
    using ImplFnSignature = ResultT(*)(Range&);
    auto N = std::meta::reflect_value(std::ranges::distance(range));
    auto R = remove_reference(^^Range);
    auto impl_fn = extract<ImplFnSignature>(
      substitute(^^impl::define_static_array_impl, {N, ^^T, R}));
    return impl_fn(range);
  } else {
    auto vec = range | std::ranges::to<std::vector>();
    return define_static_array(vec);
  }
}

// Specialization to prevent repeated meta-definition.
template <class T>
consteval auto define_static_array(meta_span<T> range) {
  return range;
}

/**
 * Alternative to C++26 std::meta::define_static_string (if adopted).
 * It's guaranteed that the resulted meta_string_view is null-terminated,
 * i.e. *end() == '\0'.
 */
template <class Range>
  requires (std::ranges::input_range<Range>)
consteval auto define_static_string(
  Range&& range, bool removes_trailing_null_characters = false)
  /* -> meta_basic_string_view<T> */
{
  using T = std::ranges::range_value_t<Range>;
  using ResultT = meta_basic_string_view<T>;
  static_assert(is_char_type_v<T>, "Invalid input string type.");

  // If it's an array, check if it's a string literal and adjust accordingly
  if constexpr (requires { is_string_literal(range); }) {
    if (is_string_literal(range)) {
      return define_static_string(std::basic_string_view{range});
    }
  }
  if constexpr (std::ranges::bidirectional_range<Range>) {
    auto subrange = impl::try_remove_trailing_null_characters(
      range, removes_trailing_null_characters);

    using SubrangeT = decltype(subrange);
    using ImplFnSignature = ResultT(*)(SubrangeT&);
    auto N = std::meta::reflect_value(std::ranges::distance(subrange));
    auto impl_fn = extract<ImplFnSignature>(
      substitute(^^impl::define_static_string_impl, {N, ^^T, ^^SubrangeT}));
    return impl_fn(subrange);
  } else {
    auto vec = range | std::ranges::to<std::vector>();
    return define_static_string(vec, removes_trailing_null_characters);
  }
}

// Specialization to prevent repeated meta-definition.
template <class T>
consteval auto define_static_string(
  meta_basic_string_view<T> range,
  bool removes_trailing_null_characters = false) -> meta_basic_string_view<T>
{
  // Makes sure the resulted range is null-terminated
  if (*range.end() != '\0') {
    auto sv = static_cast<std::basic_string_view<T>>(range);
    return define_static_string(sv);
  }
  if (!removes_trailing_null_characters) {
    return range;
  }
  auto [head, tail] = range;
  for (; head < tail && tail[-1] == '\0'; --tail) {}
  return {head, tail};
}

/**
 * Alternative to C++26 std::meta::define_static_object (if adopted).
 */
template <class T>
consteval auto define_static_object(const T& value) -> const T* {
  return extract<const T*>(
    substitute(^^impl::meta_object_v, { std::meta::reflect_value(value) }));
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_DEFINE_STATIC_VALUES_HPP
