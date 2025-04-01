#ifndef REFLECT_CPP26_TYPE_TRAITS_IS_INVOCABLE_HPP
#define REFLECT_CPP26_TYPE_TRAITS_IS_INVOCABLE_HPP

// Root header: include C++ stdlib headers only to prevent circular dependency.
#include <type_traits>

namespace reflect_cpp26 {
/**
 * Stronger constraint than std::is_invocable_r_v with implicit conversion
 * of result type banned. Usable if ambiguity is a concern.
 */
template <class R, class Func, class... Args>
constexpr auto is_invocable_exactly_r_v = false;

template <class R, class Func, class... Args>
  requires (std::is_invocable_v<Func, Args...>)
constexpr auto is_invocable_exactly_r_v<R, Func, Args...> =
  std::is_same_v<std::invoke_result_t<Func, Args...>, R>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_IS_INVOCABLE_HPP
