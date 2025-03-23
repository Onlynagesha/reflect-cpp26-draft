#ifndef REFLECT_CPP26_UTILS_META_TUPLE_HPP
#define REFLECT_CPP26_UTILS_META_TUPLE_HPP

#include <reflect_cpp26/type_traits/template_instance.hpp>
#include <reflect_cpp26/type_traits/tuple_like_types.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <ranges>

namespace reflect_cpp26 {
template <class... Args>
struct meta_tuple;

namespace impl {
template <size_t I, class Tuple>
constexpr decltype(auto) get_from_tuple(Tuple&& tuple)
{
  using TupleNoCVRef = std::remove_cvref_t<Tuple>;
  if constexpr (has_tuple_member_get_ith_v<TupleNoCVRef, I>) {
    return std::forward<Tuple>(tuple).template get<I>();
  } else if constexpr (has_tuple_free_get_ith_v<TupleNoCVRef, I>) {
    return get<I>(std::forward<Tuple>(tuple));
  } else {
    static_assert(false, "Can not get from tuple-like object.");
  }
}
} // namespace impl

template <class... Args>
struct meta_tuple {
  static_assert(sizeof...(Args) >= 1,
    "At least 1 template parameter required.");
  static constexpr auto tuple_size = sizeof...(Args);

  struct underlying_type;
  consteval {
    define_aggregate(^^underlying_type, {data_member_spec(^^Args)...});
  }
  underlying_type values;

private:
  static consteval auto get_nth_field(size_t n) {
    return all_direct_nonstatic_data_members_of(^^underlying_type)[n];
  }

  template <class TupleLike, size_t... Is>
  consteval meta_tuple(TupleLike&& tuple, std::index_sequence<Is...>)
    : values{impl::get_from_tuple<Is>(std::forward<TupleLike>(tuple))...} {}

public:
  template <class... InputArgs>
  consteval meta_tuple(InputArgs&&... args)
    : values{std::forward<InputArgs>(args)...} {}

  template <class TupleLike>
    requires (is_tuple_like_of_elements_v<TupleLike, Args...>)
  consteval meta_tuple(TupleLike&& tuple)
    : meta_tuple(std::forward<TupleLike>(tuple),
                 std::make_index_sequence<tuple_size>{}) {}

  template <class TupleLike>
    requires (is_tuple_like_of_elements_v<TupleLike, Args...>)
  constexpr auto& operator=(TupleLike&& tuple)
  {
    constexpr auto members =
      all_direct_nonstatic_data_members_of(^^underlying_type);
    REFLECT_CPP26_EXPAND(members).for_each(
      [this, &tuple](auto index, auto member) {
        values.[:member:] =
          impl::get_from_tuple<index>(std::forward<TupleLike>(tuple));
      });
    return *this;
  }

  template <size_t I, class Self>
    requires (I < tuple_size)
  constexpr decltype(auto) get(this Self&& self)
  {
    constexpr auto ith_field = get_nth_field(I);
    if constexpr (std::is_lvalue_reference_v<Self>) {
      return self.[:ith_field:];
    } else {
      return std::move(self.[:ith_field:]);
    }
  }
};

/**
 * Whether T is the underlying type of some meta_tuple.
 */
template <class T>
constexpr auto is_underlying_type_of_meta_tuple_v =
  parent_of(^^T) == ^^meta_tuple;

/**
 * Free get function of meta_tuple.
 */
template <size_t I, class MetaTuple>
  requires (is_template_instance_of_v<MetaTuple, meta_tuple>
    && I < std::tuple_size_v<MetaTuple>)
constexpr decltype(auto) get(MetaTuple&& tuple) {
  return std::forward<MetaTuple>(tuple).template get<I>();
}
} // namespace reflect_cpp26

template <class... Args>
struct std::tuple_size<reflect_cpp26::meta_tuple<Args...>>
  : std::integral_constant<size_t, sizeof...(Args)> {};

template <size_t I, class... Args>
struct std::tuple_element<I, reflect_cpp26::meta_tuple<Args...>> {
  using type = Args...[I];
};

#endif // REFLECT_CPP26_UTILS_META_TUPLE_HPP
