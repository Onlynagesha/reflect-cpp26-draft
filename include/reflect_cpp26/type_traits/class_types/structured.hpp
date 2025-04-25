#ifndef REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_STRUCTURED_HPP
#define REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_STRUCTURED_HPP

#include <reflect_cpp26/type_traits/function_types.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <reflect_cpp26/utils/type_tuple.hpp>

namespace reflect_cpp26 {
namespace impl {
consteval bool is_structured_type(std::meta::info T);

template <auto V>
struct structured_type_test_helper {};

template <class T>
constexpr auto is_value_initializable_structured_type_v =
  requires { structured_type_test_helper<T{}>{}; };
} // namespace impl

/**
 * Tests whether T is a structured type.
 * Structured types are those whose value can be non-type template parameters.
 * See: https://en.cppreference.com/w/cpp/language/template_parameters
 */
template <class T>
constexpr bool is_structured_type_v =
  impl::is_structured_type(^^std::remove_cv_t<T>);

template <class T>
concept structured_type = is_structured_type_v<T>;

namespace impl {
consteval bool is_structured_nsdm(std::meta::info m)
{
  return is_public(m) && !is_volatile(m) && !is_mutable_member(m)
    && extract_bool(^^is_structured_type_v, remove_all_extents(type_of(m)));
}

consteval bool is_structured_class_type(std::meta::info T)
{
  if (!is_destructible_type(T)) {
    return false;
  }
  auto bases_are_structured = std::ranges::all_of(all_direct_bases_of(T),
    [](std::meta::info base) {
      return is_public(base) && !is_virtual(base)
        && extract_bool(^^is_structured_type_v, type_of(base));
    });
  return bases_are_structured && std::ranges::all_of(
    all_direct_nsdm_of(T), is_structured_nsdm);
}

consteval bool is_structured_union_type(std::meta::info T)
{
  return is_trivially_destructible_type(T) &&
    std::ranges::any_of(all_direct_nsdm_of(T), is_structured_nsdm);
}

consteval bool is_structured_type(std::meta::info T)
{
  if (is_reference_type(T)) {
    return is_lvalue_reference_type(T); // Filters out rvalue references
  }
  if (extract_bool(^^is_value_initializable_structured_type_v, T)) {
    return true; // Including all scalar types
  }
  if (is_union_type(T)) {
    return is_structured_union_type(remove_cv(T));
  }
  if (is_class_type(T)) {
    return is_structured_class_type(remove_cv(T));
  }
  return false;
}
} // namespace impl
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_STRUCTURED_HPP
