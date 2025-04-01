#ifndef REFLECT_CPP26_UTILS_CONSTANT_HPP
#define REFLECT_CPP26_UTILS_CONSTANT_HPP

#include <reflect_cpp26/type_traits/is_invocable.hpp>
#include <reflect_cpp26/type_traits/template_instance.hpp>
#include <array>
#include <limits>
#include <span>
#include <tuple>
#include <vector>

namespace reflect_cpp26 {
template <auto... Vs>
struct constant;

/**
 * Null index.
 */
constexpr auto npos = std::numeric_limits<size_t>::max();

namespace impl {
template <auto U, auto V>
constexpr auto constants_are_eq_comparible_v = requires { U == V; };

template <auto U, auto V>
constexpr auto constants_are_equal_v = false;

template <auto U, auto V>
  requires (constants_are_eq_comparible_v<U, V>)
constexpr auto constants_are_equal_v<U, V> = (U == V);

template <auto U, auto V>
  requires (!constants_are_eq_comparible_v<U, V>
          && constants_are_eq_comparible_v<V, U>)
constexpr auto constants_are_equal_v<U, V> = (V == U);

// Specialization for tag types.
template <auto U, auto V>
  requires (!constants_are_eq_comparible_v<U, V>
         && !constants_are_eq_comparible_v<V, U>
         && std::is_empty_v<decltype(U)>
         && std::is_empty_v<decltype(V)>)
constexpr auto constants_are_equal_v<U, V> =
  std::is_same_v<decltype(U), decltype(V)>;

template <template <auto...> class Derived, auto... Vs>
struct constant_base {
private:
  static constexpr auto has_nested_constant =
    (is_nontype_template_instance_of_v<decltype(Vs), Derived> || ...);
  static_assert(!has_nested_constant, "Vs... shall not contain constant<...>.");

  template <auto V, size_t I, class Func>
  static constexpr bool for_each_call_dispatch(const Func& body)
  {
    constexpr auto dispatch_key =
      is_invocable_exactly_r_v<bool, Func, Derived<I>, Derived<V>> * 8 +
      is_invocable_exactly_r_v<void, Func, Derived<I>, Derived<V>> * 4 +
      is_invocable_exactly_r_v<bool, Func, Derived<V>> * 2 +
      is_invocable_exactly_r_v<void, Func, Derived<V>> * 1;
  
    if constexpr (dispatch_key == 0b10'00) {
      return std::invoke(body, Derived<I>{}, Derived<V>{});
    } else if constexpr (dispatch_key == 0b01'00) {
      std::invoke(body, Derived<I>{}, Derived<V>{});
      return true;
    } else if constexpr (dispatch_key == 0b00'10) {
      return std::invoke(body, Derived<V>{});
    } else if constexpr (dispatch_key == 0b00'01) {
      std::invoke(body, Derived<V>{});
      return true;
    } else {
      static_assert(false, "Invalid or ambiguous call signature.");
    }
  }

  template <size_t I, class Func>
  static constexpr bool for_each_impl(const Func& body)
  {
    if constexpr (I < sizeof...(Vs)) {
      return for_each_call_dispatch<Vs...[I], I>(body)
        && for_each_impl<I + 1>(body);
    } else {
      return true;
    }
  }

  template <size_t I, auto Func>
  static constexpr auto filter_impl()
  {
    if constexpr (I >= sizeof...(Vs)) {
      return Derived<>();
    } else {
      constexpr auto cur = Derived<Vs...[I]>{};
      if constexpr (std::invoke(Func, cur)) {
        return Derived<Vs...[I]>::concat(filter_impl<I + 1, Func>());
      } else {
        return filter_impl<I + 1, Func>();
      }
    }
  }

  template <size_t I, auto Func, auto Prev>
  static constexpr auto reduce_impl()
  {
    if constexpr (I >= sizeof...(Vs)) {
      return Prev;
    } else {
      constexpr auto Next = std::invoke(
        Func, Derived<Prev>{}, Derived<Vs...[I]>{});
      static_assert(
        !is_nontype_template_instance_of_v<decltype(Next), Derived>,
        "Result of func shall not be constant<x>. Return x instead.");
      return reduce_impl<I + 1, Func, Next>();
    }
  }

  template <size_t I, size_t D, class T>
  static constexpr auto index_of_impl(const T& value) -> size_t
  {
    if constexpr (I >= sizeof...(Vs)) {
      return npos;
    } else if constexpr (requires { value == Vs...[I]; }) {
      return value == Vs...[I] ? I : index_of_impl<I + D, D>(value);
    } else if constexpr (requires { Vs...[I] == value; }) {
      return Vs...[I] == value ? I : index_of_impl<I + D, D>(value);
    } else if constexpr (std::is_empty_v<T>
                      && std::is_empty_v<decltype(Vs...[I])>) {
      constexpr auto is_same_type = std::is_same_v<decltype(Vs...[I]), T>;
      return is_same_type ? I : index_of_impl<I + D, D>(value);
    } else {
      return index_of_impl<I + D, D>(value);
    }
  }

public:
  /**
   * Performs for-each operation with given function body.
   * Supported call signatures (I refers to index of current value V):
   *   - body(constant<I>, constant<V>) -> same_as_one_of<bool, void>
   *   - body(constant<V>) -> same_as_one_of<bool, void>
   *
   * The return type of Func is constrained to exactly bool or void
   * (implicit conversions, e.g. int -> bool, are banned to prevent ambiguity).
   * If return type is bool and body(...) returns false with the i-th value,
   * for-each operation stops.
   */
  template <class Func>
  static constexpr void for_each(Func&& body) {
    (void)for_each_impl<0>(body);
  }

  /**
   * Maps value list with given unary function in compile-time.
   */
  template <auto Func>
  static constexpr auto map() /* -> constant<mapped_Vs...> */ {
    return Derived<std::invoke(Func, Derived<Vs>{})...>{};
  }

  /**
   * Maps value list with given unary function.
   * Returns an array {body(constant<Vs>)...} with each element
   * converted to common type.
   */
  template <class Func>
  static constexpr auto map(Func&& body)
  {
    using ResultT = std::common_type_t<
      std::invoke_result_t<Func, Derived<Vs>>...>;
    using ResultArrayT = std::array<
      ResultT, sizeof...(Vs)>;
    return ResultArrayT{std::invoke(body, Derived<Vs>{})...};
  }

  /**
   * Filters out all the values that does not match the unary predicate Func.
   */
  template <auto Func>
  static constexpr auto filter() {
    return filter_impl<0, Func>();
  }

  /**
   * Filters out all the values that does not match the unary predicate Func.
   * Returns std::vector of filtered values
   * with each element converted to common type.
   */
  template <class Func>
  static constexpr auto filter(Func&& body)
  {
    using ValueT = std::common_type_t<decltype(Vs)...>;
    auto res = std::vector<ValueT>{};
    res.reserve(sizeof...(Vs));
    auto try_add = [&body, &res](auto cur) {
      if (std::invoke(body, cur)) {
        res.emplace_back(cur.value);
      }
    };
    (try_add(Derived<Vs>{}), ...);
    return res;
  }

  /**
   * Reduces value list with given binary funciton in compile-time.
   * Pseudocode:
   *   let result = init;
   *   for (cur in Vs)
   *     result = Func(result, cur);
   *   return result
   */
  template <auto Func, auto Init>
  static constexpr auto reduce() /* -> reduced_value */ {
    return reduce_impl<0, Func, Init>();
  }

  /**
   * Reduces value list with given binary function.
   * Pseudocode:
   *   let result = init;
   *   for (cur in Vs)
   *     result = Func(result, cur) as T;
   *   return result
   */
  template <class Func, class T>
  static constexpr auto reduce(Func&& body, T init) -> T
  {
    static_assert((std::is_invocable_r_v<T, Func, T, Derived<Vs>> && ...),
      "Invalid call signature.");
    (void)((init = std::invoke(body, init, Derived<Vs>{})), ...);
    return init;
  }

  /**
   * Makes a concatenated value list.
   */
  template <auto... RhsVs>
  static constexpr auto concat(Derived<RhsVs...>) {
    return Derived<Vs..., RhsVs...>{};
  }

  /**
   * Finds target in value list and returns the first index,
   * or npos if not found.
   * Each V in value list should satisfy the following constraints:
   * (1) Either (V == target) or (target == V) is valid expression
   *     that returns bool;
   * (2) If (1) is not satisfied, but both V and target are of empty types,
   *     in which case decltype(V) and decltype(target) are compared.
   *
   * Note: Be cautious with tag types supporting implicit conversion. Example:
   *   auto tags_misuse = tags_constant<
   *     std::is_integral<float>,
   *     std::is_integral<int>,
   *     std::is_integral<long>,
   *     std::is_integral<std::string>>();
   *   static_assert(tags_misuse.index_of(std::is_integral<char>{})
   *     == 1); // You may expect the result to be npos
   *
   * In this example (std::is_integral<T>{} == std::is_integral<U>{})
   * is a valid expression for every T and U where both operands
   * are implicitly converted to bool.
   */
  template <class T>
  static constexpr auto index_of(const T& target) -> size_t {
    return index_of_impl<0, 1>(target);
  }

  /**
   * Finds target in value list and returns the last index,
   * or npos if not found.
   */
  template <class T>
  static constexpr auto last_index_of(const T& target) -> size_t {
    return index_of_impl<sizeof...(Vs) - 1, npos>(target);
  }

  /**
   * Checks whether target equals to some element in value list.
   */
  template <class T>
  static constexpr auto includes(const T& target) -> bool {
    return index_of(target) != npos;
  }

  /**
   * How many values contained.
   */
  static constexpr auto size() {
    return sizeof...(Vs);
  }
};
} // namespace impl

/**
 * Generic constant list.
 * constant<Vs...> matches tuple-like requirements:
 *   - get<I>(c) free function is provided.
 *   - std::tuple_size_v and std::tuple_element_t are specialized.
 */
template <auto... Vs>
struct constant : impl::constant_base<constant, Vs...> {};

template <auto V>
struct constant<V> : impl::constant_base<constant, V> {
  using type = decltype(V);
  static constexpr auto value = V;
  static constexpr auto values = std::array{V};

  constexpr operator type() const {
    return V;
  }
};

template <auto V, auto... Rest>
  requires (std::is_same_v<decltype(V), decltype(Rest)> && ...)
struct constant<V, Rest...> : impl::constant_base<constant, V, Rest...> {
  using type = decltype(V);
  static constexpr auto values = std::array{V, Rest...};
};

/**
 * Helper for making constant with tag types.
 */
template <class... Args>
  requires (std::is_empty_v<Args> && ...)
constexpr auto tags_constant() {
  return constant<Args{}...>{};
}

/**
 * Tuple-get interface.
 */
template <size_t I, auto... Vs>
constexpr auto get(constant<Vs...>) {
  return Vs...[I];
}
} // namespace reflect_cpp26

template <auto... Vs>
struct std::tuple_size<reflect_cpp26::constant<Vs...>>
  : std::integral_constant<size_t, sizeof...(Vs)> {};

template <size_t I, auto... Vs>
struct std::tuple_element<I, reflect_cpp26::constant<Vs...>> {
  using type = decltype(Vs...[I]);
};

#endif // REFLECT_CPP26_UTILS_CONSTANT_HPP
