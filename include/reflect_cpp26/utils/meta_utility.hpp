#ifndef REFLECT_CPP26_UTILS_META_UTILITY_HPP
#define REFLECT_CPP26_UTILS_META_UTILITY_HPP

#include <reflect_cpp26/utils/config.h>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/ranges.hpp>

namespace reflect_cpp26 {
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

/**
 * Note: You need to be cautious when trying to cache member information
 * with constexpr variables. For example:
 *   constexpr auto std_member_count =
 *     all_direct_members_of(^^std).size(); // direct members of namespace std
 *
 * The actual values may be inconsistent if:
 * (1) ODR violation happens: std_member_count is instantiated in multiple
 *     translation units with different visible subsets of namespace std;
 * (2) Context differs.
 */
#define REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT(fn)                \
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
REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT(members)
REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT(bases)
REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT(static_data_members)
REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT(nonstatic_data_members)

#undef REFLECT_CPP26_DEFINE_QUERY_WITH_ACCESS_CONTEXT

// -------- Reflecting pointers-to-member --------

/**
 * Transforms a pointer to member to its corresponding reflection such that
 * reflect_pointer_to_member(&Foo::bar) == ^Foo::bar
 */
template <class T, class Member>
  requires (!std::is_function_v<Member>)
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
    compile_error("Not found.");
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
template <class T, std::same_as<std::meta::info>... Args>
consteval T extract_substituted(std::meta::info templ, Args... templ_params)
{
  return std::meta::extract<T>(
    std::meta::substitute(templ, {templ_params...}));
}

/**
 * Equivalent to extract<bool>(substitute(templ, {templ_params})).
 * Frequently used with type traits and concepts.
 */
template <std::same_as<std::meta::info>... Args>
consteval bool extract_bool(std::meta::info templ, Args... templ_params)
{
  return std::meta::extract<bool>(
    std::meta::substitute(templ, {templ_params...}));
}

template <input_range_of<std::meta::info> MetaRange>
consteval bool extract_bool(std::meta::info templ, MetaRange&& templ_params)
{
  return std::meta::extract<bool>(
    std::meta::substitute(templ, std::forward<MetaRange>(templ_params)));
}
// -------- Substitution helpers --------

template <std::same_as<std::meta::info>... Args>
consteval auto substitute(std::meta::info templ, Args... templ_params)
  -> std::meta::info {
  return std::meta::substitute(templ, {templ_params...});
}

// -------- Splicing helpers --------

struct splice_to_value_t {
  template <std::meta::info V>
  static constexpr auto operator()(constant<V>) {
    return [:V:];
  }
};
constexpr auto splice_to_value = splice_to_value_t{};

} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_META_UTILITY_HPP
