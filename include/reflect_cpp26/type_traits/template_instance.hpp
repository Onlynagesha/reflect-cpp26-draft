#ifndef REFLECT_CPP26_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP

#include <type_traits>

namespace reflect_cpp26 {
namespace impl {
template <class T, template <class...> class Template>
struct is_template_instance_of : std::false_type {};

template <template <class...> class Template, class... Args>
struct is_template_instance_of<Template<Args...>, Template> : std::true_type {};

template <class T, template <auto...> class Template>
struct is_nontype_template_instance_of : std::false_type {};

template <template <auto...> class Template, auto... Args>
struct is_nontype_template_instance_of<Template<Args...>, Template>
  : std::true_type {};
} // namespace impl

/**
 * Whether std::remove_cvref_t<T> is a template instance of given type template.
 * Note: Templates with mixed type and non-type parameters
 * (e.g. std::array<T, N>) are not supported,
 * in which case std::meta::template_of() can be used instead.
 *
 * Example: static_assert(is_template_instance_of_v<
 *   std::vector<int, MyAllocator<int>>,
 *   std::vector>)
 */
template <class T, template <class...> class Template>
constexpr bool is_template_instance_of_v =
  impl::is_template_instance_of<std::remove_cvref_t<T>, Template>::value;

template <class T, template <class...> class Template>
concept template_instance_of = is_template_instance_of_v<T, Template>;

/**
 * Whether std::remove_cvref_t<T> is a template instance of given
 * non-type template.
 * Note: Templates with mixed type and non-type parameters
 * (e.g. std::array<T, N>) are not supported,
 * in which case std::meta::template_of() can be used instead.
 *
 * Example: static_assert(is_nontype_template_instance_of_v<
 *   std::ratio<2, 5>,
 *   std::ratio)
 */
template <class T, template <auto...> class Template>
constexpr bool is_nontype_template_instance_of_v =
  impl::is_nontype_template_instance_of<
    std::remove_cvref_t<T>, Template>::value;

template <class T, template <auto...> class Template>
concept nontype_template_instance_of =
  is_nontype_template_instance_of_v<T, Template>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_TEMPLATE_INSTANCE_HPP
