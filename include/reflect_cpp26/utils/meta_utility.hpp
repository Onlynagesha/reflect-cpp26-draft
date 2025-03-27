#ifndef REFLECT_CPP26_UTILS_META_UTILITY_HPP
#define REFLECT_CPP26_UTILS_META_UTILITY_HPP

#include <reflect_cpp26/utils/config.h>
#include <reflect_cpp26/utils/expand.hpp>

namespace reflect_cpp26 {
enum class policy_on_duplication {
  raises_error,
  returns_first,
  returns_last,
};

// -------- Reflection with access control (P3547) --------

consteval auto current_context() {
  return std::meta::access_context::current();
}

consteval auto unprivileged_context() {
  return std::meta::access_context::unprivileged();
}

consteval auto unchecked_context() {
  return std::meta::access_context::unchecked();
}

#define REFLECT_CPP26_MEMBER_QUERY_WITH_ACCESS_CONTEXT_FOR_EACH(F) \
  F(members)                 \
  F(bases)                   \
  F(static_data_members)     \
  F(nonstatic_data_members)

/**
 * Note: You need to be cautious when trying to cache member information
 * with constexpr variables. For example:
 *   constexpr auto ns_member_count =
 *     all_direct_members_of(^^std).size(); // direct members of namespace std
 *
 * The actual values may be inconsistent if:
 * (1) ODR violation happens: ns_member_count is instantiated in multiple
 *     translation units with different subsets of namespace ns visible;
 * (2) Context differs.
 */
#define REFLECT_CPP26_DEFINE_MEMBER_QUERY_WITH_ACCESS_CONTEXT(fn)         \
  /* Gets all direct members that are accessible in current scope */      \
  consteval auto currently_accessible_##fn##_of(std::meta::info a) {      \
    return std::meta::fn##_of(a, reflect_cpp26::current_context());       \
  }                                                                       \
  /** Gets all direct members that are accessible in global scope */      \
  consteval auto globally_accessible_##fn##_of(std::meta::info a) {       \
    return std::meta::fn##_of(a, reflect_cpp26::unprivileged_context());  \
  }                                                                       \
  /* Gets all direct members regardless of their accessibility */         \
  consteval auto all_direct_##fn##_of(std::meta::info a) {                \
    return std::meta::fn##_of(a, reflect_cpp26::unchecked_context());     \
  }

// Expanded dfinitions see above
REFLECT_CPP26_MEMBER_QUERY_WITH_ACCESS_CONTEXT_FOR_EACH(
  REFLECT_CPP26_DEFINE_MEMBER_QUERY_WITH_ACCESS_CONTEXT)

#undef REFLECT_CPP26_DEFINE_MEMBER_QUERY_WITH_ACCESS_CONTEXT
#undef REFLECT_CPP26_MEMBER_QUERY_WITH_ACCESS_CONTEXT_FOR_EACH

// -------- Reflecting pointers-to-member --------

/**
 * Transforms a pointer to member to its corresponding reflection such that
 * reflect_pointer_to_member(&Foo::bar) == ^Foo::bar
 */
template <class T, class Member>
consteval auto reflect_pointer_to_member(Member T::*mptr) -> std::meta::info
{
  // todo: Is O(1) lookup possible?
  auto res = std::meta::info{};
  auto target = std::meta::reflect_value(mptr);
  REFLECT_CPP26_EXPAND(all_direct_nonstatic_data_members_of(^^T)).for_each(
    [&res, target](auto cur_member) {
      if (std::meta::reflect_value(&[:cur_member:]) == target) {
        res = cur_member;
        return false; // Stops for-each loop
      }
      return true; // Continues
    });
  if (std::meta::info{} == res) {
    throw "Not found.";
  }
  return res;
}

// -------- Extration helpers --------

template <std::meta::info V>
consteval auto extract() {
  return std::meta::extract<[:type_of(V):]>(V);
}

template <std::meta::info V>
consteval auto extract(constant<V>) {
  return std::meta::extract<[:type_of(V):]>(V);
}

/**
 * Equivalent to extract<T>(substitute(templ, {templ_params})).
 */
template <class T, class... Args>
  requires (std::is_same_v<Args, std::meta::info> && ...)
consteval T extract_substituted(std::meta::info templ, Args... templ_params)
{
  return std::meta::extract<T>(
    std::meta::substitute(templ, {templ_params...}));
}

/**
 * Equivalent to extract<bool>(substitute(templ, {templ_params})).
 * Frequently used with type traits and concepts.
 */
template <class... Args>
  requires (std::is_same_v<Args, std::meta::info> && ...)
consteval bool extract_bool(std::meta::info templ, Args... templ_params)
{
  return std::meta::extract<bool>(
    std::meta::substitute(templ, {templ_params...}));
}

template <class MetaRange>
  requires (std::ranges::input_range<MetaRange>)
consteval bool extract_bool(std::meta::info templ, MetaRange&& templ_params)
{
  return std::meta::extract<bool>(
    std::meta::substitute(templ, std::forward<MetaRange>(templ_params)));
}
// -------- Substitution helpers --------

template <class... Args>
  requires (std::is_same_v<Args, std::meta::info> && ...)
consteval auto substitute(std::meta::info templ, Args... templ_params)
  -> std::meta::info {
  return std::meta::substitute(templ, {templ_params...});
}

} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_META_UTILITY_HPP
