#ifndef REFLECT_CPP26_TYPE_OPERATIONS_TO_STRUCTURED_HPP
#define REFLECT_CPP26_TYPE_OPERATIONS_TO_STRUCTURED_HPP

#include <reflect_cpp26/type_traits/arithmetic_types.hpp>
#include <reflect_cpp26/type_traits/class_types.hpp>
#include <reflect_cpp26/type_traits/function_types.hpp>
#include <reflect_cpp26/type_traits/tuple_like_types.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/meta_tuple.hpp>

namespace reflect_cpp26 {
template <class T>
consteval auto to_structured(const T& value);

namespace impl {
template <class InputRange>
consteval auto range_to_structured(const InputRange& range)
{
  using ValueT = std::ranges::range_value_t<InputRange>;
  if constexpr (is_char_type_v<ValueT>) {
    return define_static_string(range);
  } else {
    auto converted = range | std::views::transform([](const auto& elem) {
      return to_structured(elem);
    });
    return define_static_array(converted);
  }
}

template <size_t I, class TupleLike>
consteval auto tuple_like_ith_to_structured(const TupleLike& tuple) {
  return to_structured(get<I>(tuple));
}

template <size_t... Is, class TupleLike>
consteval auto tuple_like_to_structured(
  const TupleLike& tuple, std::index_sequence<Is...>)
{
  return meta_tuple{tuple_like_ith_to_structured<Is>(tuple)...};
}

template <class TupleLike>
consteval auto tuple_like_to_structured(const TupleLike& tuple)
{
  constexpr auto N = std::tuple_size_v<TupleLike>;
  return tuple_like_to_structured(tuple, std::make_index_sequence<N>{});
}

template <class Pointer>
  requires (std::is_pointer_v<Pointer>)
consteval auto pointer_to_structured(Pointer ptr)
{
  using T = std::remove_pointer_t<Pointer>;
  if constexpr (is_char_type_v<T>) {
    auto tail = std::ranges::find(ptr, std::unreachable_sentinel, '\0');
    return define_static_string(std::basic_string_view{ptr, tail});
  } else {
    return ptr;
  }
}

consteval auto pointer_to_structured(std::nullptr_t) {
  return nullptr;
}
} // namespace impl

/**
 * Converts T to structured type.
 * Conversion rule:
 * (1) If T is range whose value type is V, then result is:
 *     (a) meta_basic_string_view<V> if V is character type
 *     (b) meta_span<to_structured_t<V>> otherwise.
 * (2) if T is tuple-like type, then result is
 *     meta_tuple<to_structured_t<Vs>...> where Vs... are tuple elements of T
 * (3) if T is const CharT* where CharT is a character type,
 *     then result is meta_basic_string_view<CharT>, assuming T points to a
 *     null-terminated string.
 * (4) Otherwise, result is T identity.
 */
template <class T>
consteval auto to_structured(const T& value)
{
  if constexpr (std::ranges::input_range<T>) {
    return impl::range_to_structured(value);
  } else if constexpr (is_tuple_like_v<T>) {
    return impl::tuple_like_to_structured(value);
  } else if constexpr (std::is_pointer_v<T> || std::is_null_pointer_v<T>) {
    return impl::pointer_to_structured(value);
  } else if constexpr (std::is_function_v<T>) {
    return static_cast<to_function_pointer_t<T>>(value);
  } else {
    static_assert(is_structured_type_v<T>, "T is not structured type.");
    return value;
  }
}

/**
 * Specialization for std::initializer_list.
 */
template <class T>
consteval auto to_structured(std::initializer_list<T> values) {
  return impl::range_to_structured(values);
}

/**
 * Conversion result type of to_structured().
 */
template <class T>
using to_structured_t = decltype(to_structured(std::declval<T>()));
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_OPERATIONS_TO_STRUCTURED_HPP
