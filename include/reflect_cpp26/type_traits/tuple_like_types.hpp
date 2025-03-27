#ifndef REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP

#include <reflect_cpp26/type_traits/extraction.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <reflect_cpp26/utils/type_tuple.hpp>
#include <algorithm>
#include <concepts>
#include <tuple>
#include <type_traits>

namespace reflect_cpp26 {
namespace impl {
namespace tuple_traits {
// Identical to reflect_cpp26::same_as_one_of
// A separate copy is made here to prevent potential circular dependency.
template <class T, class... Args>
concept same_as_one_of = (std::is_same_v<T, Args> || ...);

// Identical to reflect_cpp26::all_are_same_v
template <class T, class... Args>
concept all_are_same_v = (std::is_same_v<T, Args> && ...);
} // namespace tuple_traits

template <class T>
constexpr auto has_tuple_size_v = requires {
  { std::tuple_size_v<T> } -> std::convertible_to<size_t>;
};

template <class T, size_t I>
constexpr auto has_tuple_member_get_ith_v = requires (T t) {
  { t.template get<I>() } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&>;
  { std::as_const(t).template get<I>() } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&>;
  { std::move(t).template get<I>() } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&&>;
  { std::move(std::as_const(t)).template get<I>() }
    -> tuple_traits::same_as_one_of<
      std::tuple_element_t<I, T>,
      const std::tuple_element_t<I, T>&&>;
};

// get<I>(t) can be found via ADL.
template <class T, size_t I>
constexpr auto has_tuple_free_get_ith_v = requires (T t) {
  { get<I>(t) } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&>;
  { get<I>(std::as_const(t)) } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    const std::tuple_element_t<I, T>&>;
  { get<I>(std::move(t)) } -> tuple_traits::same_as_one_of<
    std::tuple_element_t<I, T>,
    std::tuple_element_t<I, T>&&>;
  { get<I>(std::move(std::as_const(t))) } -> tuple_traits::same_as_one_of<
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

namespace impl {
template <class... Args>
consteval auto tuple_like_have_same_size() -> bool
{
  constexpr auto size = std::tuple_size_v<Args...[0]>;
  auto res = true;
  REFLECT_CPP26_EXPAND_I(sizeof...(Args)).for_each([&res](auto I) {
    auto cur_size = std::tuple_size_v<Args...[I]>;
    return res &= (cur_size == size);
  });
  return res;
}
} // namespace impl

/**
 * Whether std::remove_cvref_t<Args>... are tuple-like types
 * with the same tuple size.
 */
template <class... Args>
  requires (sizeof...(Args) >= 1)
constexpr auto are_tuple_like_of_same_size_v =
  (is_tuple_like_v<Args> && ...) && impl::tuple_like_have_same_size<Args...>();

template <class T, class Tuple>
concept tuple_like_of_same_size_with = are_tuple_like_of_same_size_v<T, Tuple>;

namespace impl {
struct reduce_tuple_like_elementwise_result_t {
  size_t true_count;
  size_t false_count;
};

template <template <class...> class Predicate, class... Tuples>
consteval auto reduce_tuple_like_elementwise()
{
  auto true_count = 0zU;
  auto false_count = 0zU;
  constexpr auto min_tuple_size = std::min({std::tuple_size_v<Tuples>...});
  REFLECT_CPP26_EXPAND_I(min_tuple_size).for_each(
    [&true_count, &false_count](auto I) {
      constexpr auto ith_elements =
        std::array{^^std::tuple_element_t<I, Tuples>...};
      constexpr auto cur_result =
        [: substitute(^^Predicate, ith_elements) :]::value;
      (cur_result ? true_count : false_count) += 1;
    });
  return reduce_tuple_like_elementwise_result_t{true_count, false_count};
}

template <template <class...> class Predicate, class... Tuples>
constexpr auto reduce_tuple_like_elementwise_v =
  reduce_tuple_like_elementwise<Predicate, Tuples...>();

template <template <class...> class Transform, class... Tuples>
consteval auto tuple_like_elementwise_zip_substitute() -> std::meta::info
{
  constexpr auto min_tuple_size = std::min({std::tuple_size_v<Tuples>...});
  constexpr auto tuples_reflected = std::array{^^Tuples...};
  auto results = std::vector<std::meta::info>{};
  results.reserve(min_tuple_size);
  for (auto i = 0zU; i < min_tuple_size; i++) {
    auto ith_elements = tuples_reflected
      | std::views::transform([i](std::meta::info T) {
          return substitute(^^std::tuple_element_t,
            std::meta::reflect_value(i), T);
        });
    results.push_back(substitute(^^Transform, ith_elements));
  }
  return substitute(^^type_tuple, results);
}

template <template <class...> class Transform, class... Tuples>
constexpr auto tuple_like_elementwise_zip_substitute_refl =
  tuple_like_elementwise_zip_substitute<
    Transform, std::remove_cvref_t<Tuples>...>();
} // namespace impl

/**
 * Whether Predicate<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>::value is false
 * for every j = 0 to N - 1 where
 *   N = min(tuple_size_v<Tuples>...), K = sizeof...(Tuples)
 */
template <template <class...> class Predicate, class... Tuples>
  requires (is_tuple_like_v<Tuples> && ...)
constexpr auto all_of_tuple_elementwise_v =
  impl::reduce_tuple_like_elementwise_v<Predicate, Tuples...>.false_count == 0;

/**
 * Whether Predicate<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>::value is false
 * for every j = 0 to N - 1 where
 *   N = min(tuple_size_v<Tuples>...), K = sizeof...(Tuples)
 */
template <template <class...> class Predicate, class... Tuples>
  requires (is_tuple_like_v<Tuples> && ...)
constexpr auto any_of_tuple_elementwise_v =
  impl::reduce_tuple_like_elementwise_v<Predicate, Tuples...>.true_count > 0;

/**
 * Whether Predicate<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>::value is false
 * for every j = 0 to N - 1 where
 *   N = min(tuple_size_v<Tuples>...), K = sizeof...(Tuples)
 */
template <template <class...> class Predicate, class... Tuples>
  requires (is_tuple_like_v<Tuples> && ...)
constexpr auto none_of_tuple_elementwise_v =
  impl::reduce_tuple_like_elementwise_v<Predicate, Tuples...>.true_count == 0;

/**
 * Makes a type_tuple<T0, ..., Tn> where Tj = Template<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>
 * where n = min(tuple_size_v<Tuples...[i]>) - 1, K = sizeof...(Tuples).
 */
template <template <class...> class Template, class... Tuples>
  requires (sizeof...(Tuples) >= 1)
using tuple_elementwise_zip_substitute_t =
  [:impl::tuple_like_elementwise_zip_substitute_refl<Template, Tuples...>:];

/**
 * Makes a type_tuple<T0, ..., Tn> where Tj = Transform<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>::type
 * where n = min(tuple_size_v<Tuples...[i]>) - 1, K = sizeof...(Tuples).
 */
template <template <class...> class Transform, class... Tuples>
  requires (sizeof...(Tuples) >= 1)
using tuple_elementwise_zip_transform_t =
  type_tuple_transform_t<extract_type,
    [:impl::tuple_like_elementwise_zip_substitute_refl<Transform, Tuples...>:]>;

/**
 * Makes a constant<V0, ..., Vn> where Vj = Transform<
 *   tuple_element_t<j, Tuples...[0]>, ...,
 *   tuple_element_t<j, Tuples...[K - 1]>>::value
 * where n = min(tuple_size_v<Tuples...[i]>) - 1, K = sizeof...(Tuples)
 */
template <template <class...> class Transform, class... Tuples>
  requires (sizeof...(Tuples) >= 1)
constexpr auto tuple_elementwise_zip_transform_v =
  type_tuple_transform_v<extract_value,
    [:impl::tuple_like_elementwise_zip_substitute_refl<Transform, Tuples...>:]>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_TUPLE_LIKE_TYPES_HPP
