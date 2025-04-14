#ifndef REFLECT_CPP26_ENUM_ENUM_SWITCH_HPP
#define REFLECT_CPP26_ENUM_ENUM_SWITCH_HPP

#include <reflect_cpp26/enum/enum_for_each.hpp>

namespace reflect_cpp26 {
namespace impl {
template <class R, class E, class Func>
constexpr auto enum_switch_is_invocable_r() -> bool
{
  auto res = true;
  enum_meta_for_each<E>([&res](auto e) {
    return res &= std::is_invocable_r_v<R, Func, constant<[:e:]>>;
  });
  return res;
}

template <class R, class E, class Func>
constexpr auto enum_switch_is_invocable_r_v =
  enum_switch_is_invocable_r<R, E, Func>();

template <class E, class Func>
constexpr auto enum_switch_invoke_result() -> std::meta::info
{
  auto results = std::vector<std::meta::info>{};
  enum_meta_for_each<E>([&results](auto e) {
    results.push_back(^^std::invoke_result_t<Func, constant<[:e:]>>);
  });
  return substitute(^^std::common_type, results);
}

template <class E, class Func>
using enum_switch_invoke_result_wrapped_t =
  [: enum_switch_invoke_result<E, Func>() :];

// Note: using result_t = typename [: result<E, Func>() :]::type does not work
// with experimental C++26 compiler, thus wrapped_t above is required.
template <class E, class Func>
using enum_switch_invoke_result_t =
  typename enum_switch_invoke_result_wrapped_t<E, Func>::type;

template <class E, class Func>
constexpr auto enum_switch_void(Func&& func, E value) -> void
{
  enum_meta_for_each<E>([&func, value](auto e) {
    if ([:e:] == value) {
      std::invoke(func, constant<[:e:]>{});
      return false; // false: Does not continue
    }
    return true; // true: Continues
  });
}

template <class T, class E, class Func>
constexpr auto enum_switch_optional(Func&& func, E value) -> std::optional<T>
{
  auto res = std::optional<T>{};
  enum_meta_for_each<E>([&func, value, &res](auto e) {
    if ([:e:] == value) {
      res = std::invoke(func, constant<[:e:]>{});
      return false; // false: Does not continue
    }
    return true; // true: Continues
  });
  return res;
}

template <class T, class E, class Func>
constexpr auto enum_switch_value(Func&& func, E value, T init)
  /* -> ResultT */
{
  enum_meta_for_each<E>([&func, value, &init](auto e) {
    if ([:e:] == value) {
      init = std::invoke(func, constant<[:e:]>{});
      return false; // false: Does not continue
    }
    return true; // true: Continues
  });
  return init;
}
} // namespace impl

/**
 * Enum switch-case. Equivalent to:
 * case E::value1:
 *   return func(constant<E::value1>{}); // Or return nothing if T is void
 * case E::value2:
 *   return func(constant<E::value2>{}); // Or return nothing if T is void
 * ...
 * default:
 *   return std::nullopt; // Or no-op if T is void
 */
template <non_reference_type T = void, enum_type E, class Func>
  requires (impl::enum_switch_is_invocable_r_v<T, E, Func>)
constexpr auto enum_switch(Func&& func, E value)
{
  if constexpr (std::is_same_v<T, void>) {
    return impl::enum_switch_void(std::forward<Func>(func), value);
  } else {
    return impl::enum_switch_optional<T>(std::forward<Func>(func), value);
  }
}

/**
 * Enum switch-case with default value. Equivalent to:
 * case E::value1:
 *   return func(constant<E::value1>{}) as decay(T);
 * case E::value2:
 *   return func(constant<E::value2>{}) as decay(T);
 * ...
 * default:
 *   return init;
 */
template <class T, enum_type E, class Func>
  requires (impl::enum_switch_is_invocable_r_v<std::decay_t<T>, E, Func>)
constexpr auto enum_switch(Func&& func, E value, T&& default_value)
{
  return impl::enum_switch_value<std::decay_t<T>>(
    std::forward<Func>(func), value, std::forward<T>(default_value));
}

/**
 * Enum switch-case with default value. Equivalent to:
 * case E::value1:
 *   return func(constant<E::value1>{}) as ResultT;
 * case E::value2:
 *   return func(constant<E::value2>{}) as ResultT;
 * ...
 * default:
 *   return init as ResultT;
 * ResultT is common type of init and func(constant<E::valueN>{})...
 */
template <class T, enum_type E, class Func>
  requires (impl::enum_switch_is_invocable_r_v<void, E, Func>)
constexpr auto enum_switch_to_common(Func&& func, E value, T&& default_value)
{
  using ResultT = std::common_type_t<
    impl::enum_switch_invoke_result_t<E, Func>, std::decay_t<T>>;
  return impl::enum_switch_value<ResultT>(
    std::forward<Func>(func), value, std::forward<T>(default_value));
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_SWITCH_HPP
