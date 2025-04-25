#ifndef REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_MEMBER_TRAITS_HPP
#define REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_MEMBER_TRAITS_HPP

#include <reflect_cpp26/type_traits/function_types.hpp>
#include <reflect_cpp26/utils/config.h>
#include <reflect_cpp26/utils/concepts.hpp>
#include <reflect_cpp26/utils/type_tuple.hpp>

namespace reflect_cpp26 {
enum class member_category {
  invalid,
  object,
  function,
};

template <auto MemPtr>
constexpr auto is_member_object_pointer_value_v =
  std::is_member_object_pointer_v<decltype(MemPtr)>;

template <auto MemPtr>
constexpr auto is_non_null_member_object_pointer_value_v =
  is_member_object_pointer_value_v<MemPtr> && (MemPtr != nullptr);

template <auto MemPtr>
constexpr auto is_member_function_pointer_value_v =
  std::is_member_function_pointer_v<decltype(MemPtr)>;

template <auto MemPtr>
constexpr auto is_non_null_member_function_pointer_value_v =
  is_member_function_pointer_value_v<MemPtr> && (MemPtr != nullptr);

template <auto MemPtr>
constexpr auto is_member_pointer_value_v =
  is_member_object_pointer_value_v<MemPtr> ||
  is_member_function_pointer_value_v<MemPtr>;

template <auto MemPtr>
constexpr auto is_non_null_member_pointer_value_v =
  is_non_null_member_object_pointer_value_v<MemPtr> ||
  is_non_null_member_function_pointer_value_v<MemPtr>;

template <class MemPtr>
struct member_pointer_traits {
  static constexpr auto category = member_category::invalid;
};

template <non_function_type R, class T>
struct member_pointer_traits<R T::*> {
  static constexpr auto category = member_category::object;
  // Type of the data member
  using target_type = R;
  // Where is the data member defined
  using direct_parent_type = T;
};

template <function_type F, class T>
struct member_pointer_traits<F T::*> {
  static constexpr auto category = member_category::function;
  // Traits of F (including ellipsis and specifier flags).
  using function_traits_type = function_traits<F>;
  // Return type of the member function
  using target_type = typename function_traits_type::result_type;
  // Where is the member function defined
  using direct_parent_type = T;
  // args_type = type_tuple<Args...> where Args... are the arguments type
  // The member function signature (omitting all specifiers) is
  // either Ret(Args...) or Ret(Args..., ...),
  // depending on function_traits_type::has_ellipsis_parameter.
  using args_type = typename function_traits_type::args_type;
};

/**
 * Whether MemPtr points to an accessible non-static data member of type T,
 * either directly defined by T or inherited from public base classes.
 */
template <class T, auto MemPtr>
  requires (is_non_null_member_object_pointer_value_v<MemPtr>)
constexpr auto is_accessible_by_member_object_pointer_v =
  requires (T t) { t.*MemPtr; };

template <class T, auto MemPtr>
concept accessible_by_member_object_pointer =
  is_accessible_by_member_object_pointer_v<T, MemPtr>;

namespace impl {
template <class T, auto MemPtr>
consteval bool is_accessible_by_member_function_pointer()
{
  using Traits = member_pointer_traits<decltype(MemPtr)>;
  using Target = typename Traits::target_type;
  using Args = typename Traits::args_type;
  // Note: cv and ref qualifiers of T may affect the result.
  return type_tuple_is_invocable_r_v<
    Target, decltype(MemPtr), type_tuple_push_front_t<Args, T>>;
}
} // namespace impl

/**
 * Whether MemPtr points to an accessible non-static member function of type T,
 * either directly defined by T or inherited from public base classes.
 */
template <class T, auto MemPtr>
  requires (is_non_null_member_function_pointer_value_v<MemPtr>)
constexpr auto is_accessible_by_member_function_pointer_v =
  impl::is_accessible_by_member_function_pointer<T, MemPtr>();

template <class T, auto MemPtr>
concept accessible_by_member_function_pointer =
  is_accessible_by_member_function_pointer_v<T, MemPtr>;

/**
  * Disjunction of is_accessible_by_member_object_pointer_v<T, MemPtr>
  *            and is_accessible_by_member_function_pointer_v<T, MemPtr>;
  */
template <class T, auto MemPtr>
  requires (is_non_null_member_pointer_value_v<MemPtr>)
constexpr auto is_accessible_by_member_pointer_v = []() {
  if constexpr (std::is_member_object_pointer_v<decltype(MemPtr)>) {
    return is_accessible_by_member_object_pointer_v<T, MemPtr>;
  } else {
    return is_accessible_by_member_function_pointer_v<T, MemPtr>;
  }
}();

template <class T, auto MemPtr>
concept accessible_by_member_pointer =
  is_accessible_by_member_pointer_v<T, MemPtr>;

/**
  * Whether Member is a reflection to an accessible non-static member
  * variable or function of T, either directly defined by T or
  * inherited from public base classes.
  */
template <class T, std::meta::info Member>
constexpr auto is_accessible_by_member_reflection_v = []() {
  if constexpr (is_nonstatic_data_member(Member)) {
    // Note: is_accessible_by_member_object_pointer_v is not usable
    // for reference or bit-field data members
    return requires (T t) { t.[:Member:]; };
  } else {
    static_assert(is_class_member(Member) && !is_static_member(Member),
      "Member must be a non-static class member");
    // Note: cvref qualifiers of T may affect the result.
    return is_accessible_by_member_function_pointer_v<T, &[:Member:]>;
  }
}();

template <class T, std::meta::info Member>
concept accessible_by_member_reflection =
  is_accessible_by_member_reflection_v<T, Member>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPES_MEMBER_TRAITS_HPP
