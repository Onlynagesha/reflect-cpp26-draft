#ifndef REFLECT_CPP26_UTILS_META_TUPLE_HPP
#define REFLECT_CPP26_UTILS_META_TUPLE_HPP

// Note: Beware circular dependency when including type_traits/* headers.
#include <reflect_cpp26/type_traits/template_instance.hpp>
#include <reflect_cpp26/type_traits/tuple_like_types.hpp>
#include <reflect_cpp26/utils/utility.hpp>
#include <ranges>

namespace reflect_cpp26 {
/**
 * Generic tuple-get function.
 */
template <size_t I, class Tuple>
  requires (impl::has_tuple_member_get_ith_v<std::remove_cvref_t<Tuple>, I>
         || impl::has_tuple_free_get_ith_v<std::remove_cvref_t<Tuple>, I>)
constexpr decltype(auto) tuple_get(Tuple&& tuple)
{
  constexpr auto uses_member_get =
    impl::has_tuple_member_get_ith_v<std::remove_cvref_t<Tuple>, I>;
  if constexpr (uses_member_get) {
    return std::forward<Tuple>(tuple).template get<I>();
  } else { // uses_free_get
    return get<I>(std::forward<Tuple>(tuple));
  }
}

template <class... Args>
struct meta_tuple {
  static_assert(sizeof...(Args) >= 1,
    "At least 1 template parameter required.");
  static constexpr auto tuple_size = sizeof...(Args);

  struct underlying_type;
  consteval {
    define_aggregate(^^underlying_type, {data_member_spec(^^Args)...});
  }
  // values are exposed as public data member to make meta_tuple
  // structured aggregate.
  underlying_type values;

private:
  static constexpr auto get_nth_field(size_t n) {
    return all_direct_nonstatic_data_members_of(^^underlying_type)[n];
  }

  // Note: Requirements moved to private delegated constructor
  // to prevent compile error: "satisfaction of constraint
  // 'is_tuple_like_of_elements_v<TupleLike, Args...>' depends on itself"
  template <tuple_like_of_elements<Args...> TupleLike, size_t... Is>
  constexpr meta_tuple(TupleLike&& tuple, std::index_sequence<Is...>)
    : values{tuple_get<Is>(std::forward<TupleLike>(tuple))...} {}

public:
  constexpr meta_tuple() = default;

  // cvref dropped during CTAD
  constexpr meta_tuple(const Args&... args) : values{args...} {}

  template <class TupleLike>
    /* requires (is_tuple_like_of_elements_v<TupleLike, Args...>) */
  constexpr meta_tuple(TupleLike&& tuple)
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
        values.[:member:] = tuple_get<index>(std::forward<TupleLike>(tuple));
      });
    return *this;
  }

  /**
   * Free get function of meta_tuple.
   */
  template <size_t I>
    requires (I < tuple_size)
  friend constexpr auto& get(meta_tuple& tuple) {
    return tuple.values.[: get_nth_field(I) :];
  }

  template <size_t I>
    requires (I < tuple_size)
  friend constexpr const auto& get(const meta_tuple& tuple) {
    return tuple.values.[: get_nth_field(I) :];
  }

  template <size_t I>
    requires (I < tuple_size)
  friend constexpr auto&& get(meta_tuple&& tuple) {
    return std::move(tuple.values.[: get_nth_field(I) :]);
  }

  template <size_t I>
    requires (I < tuple_size)
  friend constexpr const auto&& get(const meta_tuple&& tuple) {
    return std::move(tuple.values.[: get_nth_field(I) :]);
  }
};

// Deduction guide (cvref dropped, same behavior as std::tuple)
template <class... Args>
meta_tuple(Args...) -> meta_tuple<Args...>;

/**
 * Whether T is the underlying type of some meta_tuple.
 */
template <class T>
constexpr auto is_underlying_type_of_meta_tuple_v =
  parent_of(^^T) == ^^meta_tuple;
} // namespace reflect_cpp26

template <class... Args>
struct std::tuple_size<reflect_cpp26::meta_tuple<Args...>>
  : std::integral_constant<size_t, sizeof...(Args)> {};

template <size_t I, class... Args>
struct std::tuple_element<I, reflect_cpp26::meta_tuple<Args...>> {
  using type = Args...[I];
};

#endif // REFLECT_CPP26_UTILS_META_TUPLE_HPP
