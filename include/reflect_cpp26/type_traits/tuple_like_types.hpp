#ifndef REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP

#include <reflect_cpp26/type_traits/type_comparison.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <concepts>
#include <tuple>
#include <type_traits>

namespace reflect_cpp26 {
namespace impl {
template <class T>
constexpr auto has_tuple_size_v = requires {
  { std::tuple_size_v<T> } -> std::convertible_to<size_t>;
};

template <class T, size_t I>
constexpr auto has_tuple_member_get_ith_v = requires (T t) {
  { t.template get<I>() } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&>;
  { std::as_const(t).template get<I>() } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&>;
  { std::move(t).template get<I>() } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&&>;
  { std::move(std::as_const(t)).template get<I>() } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&&>;
};

// get<I>(t) can be found via ADL.
template <class T, size_t I>
constexpr auto has_tuple_free_get_ith_v = requires (T t) {
  { get<I>(t) } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&>;
  { get<I>(std::as_const(t)) } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&>;
  { get<I>(std::move(t)) } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&&>;
  { get<I>(std::move(std::as_const(t))) } -> same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&&>;
};

template <class T, size_t I>
constexpr auto has_tuple_get_ith_v =
  has_tuple_member_get_ith_v<T, I> || has_tuple_free_get_ith_v<T, I>;

template <class T>
constexpr auto has_tuple_get_v = []() consteval {
  auto res = true;
  REFLECT_CPP26_EXPAND_I(std::tuple_size_v<T>).for_each([&res](auto I) {
    return res &= has_tuple_get_ith_v<T, I>;
  });
  return res;
}();

template <class T, class... Args>
constexpr auto tuple_elements_match_v = []() consteval {
  if (std::tuple_size_v<T> != sizeof...(Args)) {
    return false;
  }
  auto res = true;
  REFLECT_CPP26_EXPAND_I(sizeof...(Args)).for_each([&res](auto i) {
    return res &= std::is_same_v<std::tuple_element_t<i, T>, Args...[i]>;
  });
  return res;
}();
} // namespace impl

/**
 * Whether std::remove_cvref_t<T> is a tuple-like type.
 * See: https://en.cppreference.com/w/cpp/utility/tuple/tuple-like
 */
template <class T>
constexpr auto is_tuple_like_v =
  impl::has_tuple_size_v<std::remove_cvref_t<T>>
    && impl::has_tuple_get_v<std::remove_cvref_t<T>>;

template <class T>
concept tuple_like = is_tuple_like_v<T>;

/**
 * Whether std::remove_cvref_t<T> is a tuple-like type whose element types
 * are exactly Args...
 */
template <class T, class... Args>
constexpr auto is_tuple_like_of_elements_v =
  is_tuple_like_v<std::remove_cvref_t<T>>
    && impl::tuple_elements_match_v<std::remove_cvref_t<T>, Args...>;

template <class T, class... Args>
concept tuple_like_of_elements = is_tuple_like_of_elements_v<T, Args...>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
