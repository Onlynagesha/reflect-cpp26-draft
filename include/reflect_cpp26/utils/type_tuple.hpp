#ifndef REFLECT_CPP26_TYPE_TRAITS_TYPE_TUPLE_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TYPE_TUPLE_HPP

#include <tuple>
#include <type_traits>

namespace reflect_cpp26 {
template <class... Args>
struct type_tuple {
  static constexpr auto size = sizeof...(Args);
};

template <class... Tuples>
struct type_tuple_cat {
  static_assert(!"Both TupleA and TupleB should be instance of type_tuple.");
};

template <class... Args>
struct type_tuple_cat<type_tuple<Args...>> {
  using type = type_tuple<Args...>;
};

template <class... ArgsA, class... ArgsB, class... Args>
struct type_tuple_cat<type_tuple<ArgsA...>, type_tuple<ArgsB...>, Args...> {
  using type = typename type_tuple_cat<
    type_tuple<ArgsA..., ArgsB...>, Args...>::type;
};

/**
 * Concatenates Tuples to a single type_tuple.
 */
template <class... Tuples>
using type_tuple_cat_t = typename type_tuple_cat<Tuples...>::type;

template <template <class...> class Traits, class TypeTuple>
struct tuple_apply {
  static_assert(false, "Tuple is not an instance of type_tuple.");
};

template <template <class...> class Traits, class... Args>
struct tuple_apply<Traits, type_tuple<Args...>> : Traits<Args...> {};

/**
 * Extracts 'type' from Traits<Args...> where TypeTuple = type_tuple<Args...>.
 */
template <template <class...> class Traits, class TypeTuple>
using tuple_apply_t = typename tuple_apply<Traits, TypeTuple>::type;

/**
 * Extracts 'value' from Traits<Args...> where TypeTuple = type_tuple<Args...>.
 */
template <template <class...> class Traits, class TypeTuple>
constexpr auto tuple_apply_v = tuple_apply<Traits, TypeTuple>::value;
} // namespace reflect_cpp26

template <class... Args>
struct std::tuple_size<reflect_cpp26::type_tuple<Args...>>
  : std::integral_constant<size_t, sizeof...(Args)> {};

template <size_t I, class... Args>
struct std::tuple_element<I, reflect_cpp26::type_tuple<Args...>> {
  using type = Args...[I];
};

#endif // REFLECT_CPP26_TYPE_TRAITS_TYPE_TUPLE_HPP
