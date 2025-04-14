#ifndef REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_HAS_VIRTUAL_INHERITANCE_HPP
#define REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_HAS_VIRTUAL_INHERITANCE_HPP

#include <reflect_cpp26/utils/concepts.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <algorithm>
#include <type_traits>

namespace reflect_cpp26 {
namespace impl {
consteval bool has_virtual_inheritance(std::meta::info T);
} // namespace impl

/**
 * Checks whether a non-union class T has virtual base classes recursively, i.e.
 * (1) T has no direct virtual base class;
 * (2) has_virtual_inheritance_v<B> == true for every direct base class B of T
 *     (regardless of access specifier).
 *
 * Always false for non-class types:
 * scalar types, references, arrays, unions, etc.
 */
template <class T>
constexpr auto has_virtual_inheritance_v = false;

template <class_type T>
constexpr auto has_virtual_inheritance_v<T> =
  impl::has_virtual_inheritance(^^std::remove_cv_t<T>);

template <class T>
concept class_with_virtual_inheritance =
  std::is_class_v<T> && has_virtual_inheritance_v<T>;

template <class T>
concept class_without_virtual_inheritance =
  std::is_class_v<T> && !has_virtual_inheritance_v<T>;

namespace impl {
consteval bool has_virtual_inheritance(std::meta::info T)
{
  return std::ranges::any_of(all_direct_bases_of(T), [](std::meta::info base) {
    return is_virtual(base)
      || extract_bool(^^has_virtual_inheritance_v, type_of(base));
  });
}
} // namespace impl
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_HAS_VIRTUAL_INHERITANCE_HPP
