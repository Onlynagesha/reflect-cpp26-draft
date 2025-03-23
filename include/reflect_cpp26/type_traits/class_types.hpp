#ifndef REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPE_HPP
#define REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPE_HPP

#include <reflect_cpp26/type_traits/function_types.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <reflect_cpp26/utils/type_tuple.hpp>

namespace reflect_cpp26 {
enum class member_pointer_category {
  invalid,
  to_object,
  to_function,
};

template <class MemPtr>
struct member_pointer_traits {
  static constexpr auto category = member_pointer_category::invalid;
};

template <class R, class T>
  requires (!std::is_function_v<R>)
struct member_pointer_traits<R T::*> {
  static constexpr auto category = member_pointer_category::to_object;
  using target_type = R;
  using direct_parent_type = T;
};

template <class F, class T>
  requires (std::is_function_v<F>)
struct member_pointer_traits<F T::*> {
  static constexpr auto category = member_pointer_category::to_function;
  using function_traits_type = function_traits<F>;
  using target_type = typename function_traits_type::result_type;
  using direct_parent_type = T;
  using args_type = typename function_traits_type::args_type;
};

/**
 * Whether MemPtr points to an accessible non-static data member of type T,
 * either directly defined by T or inherited from public base classes.
 */
template <class T, auto MemPtr>
  requires (std::is_member_object_pointer_v<decltype(MemPtr)>
    && MemPtr != nullptr)
constexpr auto is_accessible_by_member_object_pointer_v =
  requires (T t) { t.*MemPtr; };

template <class T, auto MemPtr>
concept accessible_by_member_object_pointer =
  is_accessible_by_member_object_pointer_v<T, MemPtr>;

/**
 * Whether MemPtr points to an accessible non-static member function of type T,
 * either directly defined by T or inherited from public base classes.
 */
template <class T, auto MemPtr>
  requires (std::is_member_function_pointer_v<decltype(MemPtr)>
    && MemPtr != nullptr)
constexpr auto is_accessible_by_member_function_pointer_v = []() {
  using Traits = member_pointer_traits<decltype(MemPtr)>;
  using Target = typename Traits::target_type;
  using Args = typename Traits::args_type;

  return tuple_apply_v<std::is_invocable_r,
    type_tuple_cat_t<type_tuple<Target, decltype(MemPtr), T>, Args>>;
}();

template <class T, auto MemPtr>
concept accessible_by_member_function_pointer =
  is_accessible_by_member_function_pointer_v<T, MemPtr>;

/**
 * Disjunction of is_accessible_by_member_object_pointer_v<T, MemPtr>
 *            and is_accessible_by_member_function_pointer_v<T, MemPtr>;
 */
template <class T, auto MemPtr>
constexpr auto is_accessible_by_member_pointer_v = []() {
  if constexpr (std::is_member_object_pointer_v<decltype(MemPtr)>) {
    return is_accessible_by_member_object_pointer_v<T, MemPtr>;
  } else {
    static_assert(std::is_member_function_pointer_v<decltype(MemPtr)>,
      "Invalid MemPtr: neither mem-ptr to object nor mem-ptr to function.");
    return is_accessible_by_member_function_pointer_v<T, MemPtr>;
  }
}();

template <class T, auto MemPtr>
concept accessible_by_member_pointer =
  is_accessible_by_member_pointer_v<T, MemPtr>;

namespace impl {
template <class T>
consteval auto has_virtual_inheritance() -> bool;

template <class T>
consteval auto has_virtual_members() -> bool;

template <class T>
consteval auto has_inaccessible_nonstatic_data_members() -> bool;
} // namespace impl

/**
 * Checks whether a non-union class T has virtual base classes recursively, i.e.
 * (1) T has no direct virtual base class;
 * (2) has_virtual_inheritance_v<B> == true for every direct base class B of T
 *     (regardless of access specifier).
 *
 * Note: Always false for non-class types:
 * scalar types, references, arrays, unions, etc.
 */
template <class T>
constexpr auto has_virtual_inheritance_v = false;

template <class T>
  requires (std::is_class_v<T>)
constexpr auto has_virtual_inheritance_v<T> =
  impl::has_virtual_inheritance<T>();

template <class T>
concept type_with_virtual_inheritance = has_virtual_inheritance_v<T>;

template <class T>
concept type_without_virtual_inheritance = !has_virtual_inheritance_v<T>;

/**
 * Checks whether a non-union class T has direct or inherited
 * virtual member functions (including virtual destructors).
 *
 * Note: Always false for non-class types:
 * scalar types, references, arrays, unions, etc.
 */
template <class T>
constexpr auto has_virtual_members_v = false;

template <class T>
  requires (std::is_class_v<T>)
constexpr auto has_virtual_members_v<T> = impl::has_virtual_members<T>();

template <class T>
concept type_with_virtual_members = has_virtual_inheritance_v<T>;

template <class T>
concept type_without_virtual_members = !has_virtual_inheritance_v<T>;

/**
 * Checks whether a non-union class T has inaccessible non-static data members,
 * including:
 * (1) direct protected or private data members;
 * (2) all data members of protected or private direct base classes;
 * (3) protected or private data members from public base classes, recursively.
 *
 * Note: Always false for non-class types:
 * scalar types, references, arrays, unions, etc.
 */
template <class T>
constexpr auto has_inaccessible_nonstatic_data_members_v = false;

template <class T>
  requires (std::is_class_v<T>)
constexpr auto has_inaccessible_nonstatic_data_members_v<T> =
  impl::has_inaccessible_nonstatic_data_members<T>();

template <class T>
concept type_with_inaccessible_nonstatic_data_members =
  has_inaccessible_nonstatic_data_members_v<T>;

template <class T>
concept type_without_inaccessible_nonstatic_data_members =
  !has_inaccessible_nonstatic_data_members_v<T>;

namespace impl {
template <class T>
consteval auto has_virtual_inheritance() -> bool
{
  auto res = false;
  REFLECT_CPP26_EXPAND(all_direct_bases_of(^^T)).for_each([&res](auto base) {
    res |= is_virtual(base.value)
      || has_virtual_inheritance_v<[:type_of(base.value):]>;
    return /* continues if */ !res;
  });
  return res;
}

template <class T>
consteval auto has_virtual_members() -> bool
{
  auto res = false;
  REFLECT_CPP26_EXPAND(all_direct_bases_of(^^T)).for_each([&res](auto base) {
    res |= has_virtual_members_v<[:type_of(base.value):]>;
    return /* continues if */ !res;
  });
  if (res) {
    return true;
  }
  REFLECT_CPP26_EXPAND(all_direct_members_of(^^T)).for_each([&res](auto mem) {
    res |= is_virtual(mem.value);
    return /* continues if */ !res;
  });
  return res;
}

template <class T>
consteval auto has_inaccessible_nonstatic_data_members() -> bool
{
  auto res = false;
  REFLECT_CPP26_EXPAND(all_direct_bases_of(^^T)).for_each([&res](auto base) {
    if constexpr (is_virtual(base.value)) {
      throw "Virtual base class is not allowed.";
    }
    if constexpr (is_public(base.value)) {
      res |= has_inaccessible_nonstatic_data_members_v<[:type_of(base.value):]>;
    } else {
      res |= !is_empty_type(type_of(base.value));
    }
    return /* continues if */ !res;
  });
  if (res) {
    return true;
  }
  REFLECT_CPP26_EXPAND(all_direct_nonstatic_data_members_of(^^T))
    .for_each([&res](auto mem) {
      res |= !is_public(mem.value);
      return /* continues if */ !res;
    });
  return res;
}

template <class T>
consteval auto walk_accessible_nonstatic_data_members() -> std::meta::info;

template <class T>
using flattened_accessible_nonstatic_data_members_t =
  [: walk_accessible_nonstatic_data_members<T>() :];
} // namespace impl

/**
 * Usage (functions below are in namespace std::meta and accessible via ADL):
 * - type_of(member) gets its value type
 * - identifier_of(member) gets its identifier
 * - parent_of(member) gets the direct class that defines this member
 *   - May be some (possibly indirect) base class of T
 * - offset_of(member) gets its offset inside parent_of(member)
 *   - Note: this offset is NOT relative to T. Use actual_offset instead.
 * - alignment_of(member) gets its alignment.
 */
struct flattened_data_member_spec {
  /**
   * Reflection to a public non-static data member of T,
   * either defined by T directly or inherited from some base class.
   */
  std::meta::info member;
  /**
   * Actual offset of member relative to T.
   */
  // TODO: Change to std::meta::member_offset as of P2996r8
  std::meta::member_offsets actual_offset;
};

/**
 * Gets a full list of non-static data members (NSDM) with public access
 * of non-union class T, including:
 * (1) Direct public NSDMs of T;
 * (2) All public NSDMs inherited from public base classes of T.
 *
 * The result NSDM list is of type constant<m1, m2, ...>, where each mi is
 * a flattened_data_member_spec object of the data member.
 *
 * Define the inheritance graph of T as a DAG where each directed edge X -> Y
 * represents direct inheritance relationship (Y is a direct base class of X).
 * Data members are traversed via post-order DFS from the root T so that
 * actual_offset is ascending as long as the inheritance graph is a tree
 * (i.e. no class is base of multiple classes).
 *
 * Note: duplicated data member may exist in the traversed list if the
 * inheritance graph is not a tree (e.g. "diamond inheritance").
 */
template <class T>
  requires (std::is_class_v<T>
    && !has_virtual_inheritance_v<T>)
constexpr auto flattened_accessible_nonstatic_data_members_v =
  impl::flattened_accessible_nonstatic_data_members_t<T>{};

namespace impl {
template <class T>
consteval auto walk_accessible_nonstatic_data_members() -> std::meta::info
{
  auto members = std::vector<std::meta::info>{};
  REFLECT_CPP26_EXPAND(globally_accessible_bases_of(^^T)).for_each(
    [&members](auto base) {
      if (is_virtual(base.value)) {
        throw "Virtual inheritance is disallowed.";
      }
      // TODO: Use correct base_offset after compiler bug of offset_of fixed
      constexpr auto base_offset = 0zU;
      // constexpr auto base_offset = offset_of(base.value).bytes;
      using FromBase = flattened_accessible_nonstatic_data_members_t<
        [:std::meta::type_of(base):]>;
      REFLECT_CPP26_EXPAND(template_arguments_of(dealias(^^FromBase)))
        .for_each([&members](auto sp) {
          auto cur_member_spec = extract<flattened_data_member_spec>(sp.value);
          cur_member_spec.actual_offset.bytes += base_offset;
          members.push_back(std::meta::reflect_value(cur_member_spec));
        });
    });
  REFLECT_CPP26_EXPAND(globally_accessible_nonstatic_data_members_of(^^T))
    .for_each([&members](auto cur_member) {
      auto cur_member_spec = flattened_data_member_spec{
        .member = cur_member,
        .actual_offset = offset_of(cur_member.value)};
      members.push_back(std::meta::reflect_value(cur_member_spec));
    });
  return substitute(^^constant, members);
}

template <class T>
consteval auto check_flattened_members()
{
  constexpr auto members =
    flattened_accessible_nonstatic_data_members_v<T>;
  auto res = true;
  members.for_each([&res](auto cur_spec) {
    // Each member must be accessible
    res &= is_accessible_by_member_object_pointer_v<
      T, &[:cur_spec.value.member:]>;
    // No union member is allowed.
    res &= !is_union_type(cur_spec.value.member);
    return res;
  });
  return res;
}

template <class T>
constexpr auto check_flattened_members_v =
  check_flattened_members<std::remove_cv_t<T>>();
} // namespace impl

/**
 * Whether T is a partially flattenable type.
 * Type T is partially flattenable if either condition below is satisfied:
 * 1. T is a scalar type, including:
 *   (1) arithmetic types;
 *   (2) enum types;
 *   (3) pointers (including std::nullptr_t and function pointers);
 *   (4) pointers to member (variable or function).
 * 2. T is an array type (bounded U[N] or unbounded U[]);
 * 3. T is a non-union class type that satisfies all the constraints below:
 *   (1) No direct non-static data members of union type;
 *   (2) No virtual direct base classes;
 *   (3) Every direct base class is partially flattenable, recursively;
 *   (4) Every member in flattened_accessible_nonstatic_data_members_v<T>
 *       is accessible (no ambiguity due to multiple inheritance).
 *
 * Note: identifier duplication is allowed as long as such ambiguity is
 * not from multiple inheritance. Example:
 *   struct A { int x; int y; };
 *   struct B : A { int* x; int* y; };
 *   static_assert(is_partially_flattenable_v<B>); // PASS
 *
 *   constexpr auto specs = flattened_accessible_nonstatic_data_members_v<B>;
 *   auto b = B{};
 *   auto some_int = 42;
 *   b.[: get<0>(specs).member :] = 42;         // A::x
 *   b.[: get<2>(specs).member :] = &some_int;  // B::x
 */
template <class T>
constexpr auto is_partially_flattenable_v =
  std::is_scalar_v<T> || std::is_array_v<T>;

template <class T>
  requires (std::is_class_v<T>
    && !has_virtual_inheritance_v<T>)
constexpr auto is_partially_flattenable_v<T> =
  impl::check_flattened_members_v<std::remove_cv_t<T>>;

template <class T>
concept partially_flattenable = is_partially_flattenable_v<T>;

/**
 * Whether T is a flattenable type.
 * Type T is flattenable if either condition below is satisfied:
 * 1. T is a scalar type;
 * 2. T is an array type (bounded U[N] or unbounded U[]);
 * 3. T is a partially flattenable class type that satisfies all the
 *    additional constraints below:
 *   (1) No private or protected direct non-static data members;
 *   (2) No private or protected direct base classes that are not empty;
 *   (3) Every direct base class is a flattenable type, recursively.
 */
template <class T>
constexpr auto is_flattenable_v = std::is_scalar_v<T> || std::is_array_v<T>;

template <class T>
  requires (std::is_class_v<T>
    && !has_virtual_inheritance_v<T>)
constexpr auto is_flattenable_v<T> = is_partially_flattenable_v<T>
  && !has_inaccessible_nonstatic_data_members_v<T>;

template <class T>
concept flattenable = is_flattenable_v<T>;

namespace impl {
template <class T>
consteval auto is_flattenable_aggregate() -> bool;
} // namespace impl

/**
 * Whether T is a flattenable aggregate type.
 * Type T is a flattenable aggregate if either condition below is satisfied:
 * 1. T is an array type (bounded U[N] or unbounded U[]);
 * 2. T is a class type that satisfies all the additional constraints below:
 *   (1) T is an aggregate;
 *   (2) Every direct base class is a flattenable aggregate, recursively.
 */
template <class T>
constexpr auto is_flattenable_aggregate_v = std::is_array_v<T>;

template <class T>
  requires (std::is_class_v<T> && std::is_aggregate_v<T>)
constexpr auto is_flattenable_aggregate_v<T> =
  is_flattenable_aggregate<std::remove_cv_t<T>>();

template <class T>
concept flattenable_aggregate = is_flattenable_aggregate_v<T>;

namespace impl {
template <class T>
consteval auto is_flattenable_aggregate() -> T
{
  static_assert(std::is_class_v<T>, "Non-class type not allowed.");
  if constexpr (!std::is_aggregate_v<T>) {
    return false;
  } else {
    auto all_bases_are_flattenable = true;
    REFLECT_CPP26_EXPAND(all_direct_bases_of(^^T)).for_each(
      [&all_bases_are_flattenable](auto base) {
        // Each base should be flattenable **aggregate** as well;
        return all_bases_are_flattenable &=
          is_public(base.value) && !is_virtual(base.value)
            && is_flattenable_aggregate_v<[:std::meta::type_of(base):]>;
      });
    if (!all_bases_are_flattenable) {
      return false;
    }
    // Checks direct members of T
    auto all_members_are_flattenable = std::ranges::all_of(
      all_direct_nonstatic_data_members_of(^^T),
      [](std::meta::info member) {
        return is_public(member) && !is_union_type(type_of(member));
      });
    return all_members_are_flattenable;
  }
}

template <class T>
consteval auto is_structured_type() -> bool;

template <class T>
constexpr auto is_value_initializable_structured_type_v = requires {
  constant<T{}>{};
};
} // namespace impl

/**
 * Tests whether T is a structured type.
 * Structured types are those whose value can be non-type template parameters.
 * See: https://en.cppreference.com/w/cpp/language/template_parameters
 */
template <class T>
constexpr bool is_structured_type_v = impl::is_structured_type<T>();

template <class T>
concept structured_type = is_structured_type_v<T>;

namespace impl {
template <class T>
consteval bool is_structured_class_type(std::is_destructible<T>)
{
  auto all_bases_are_structured = true;
  REFLECT_CPP26_EXPAND(all_direct_bases_of(^^T)).for_each(
    [&all_bases_are_structured](auto base) {
      return all_bases_are_structured &=
        is_public(base.value) && !is_virtual(base.value)
          && is_structured_type_v<[:type_of(base.value):]>;
    });
  if (!all_bases_are_structured) {
    return false;
  }
  auto all_members_are_structured = true;
  REFLECT_CPP26_EXPAND(all_direct_nonstatic_data_members_of(^^T)).for_each(
    [&all_members_are_structured](auto member) {
      if (!is_public(member.value) || is_volatile(member.value) ||
          is_mutable_member(member.value)) {
        return all_members_are_structured = false;
      }
      constexpr auto type = type_of(member.value);
      if constexpr (is_array_type(type)) {
        return all_members_are_structured &=
          is_structured_type_v<[:remove_all_extents(type):]>;
      } else {
        return all_members_are_structured &= is_structured_type_v<[:type:]>;
      }
    });
  return all_members_are_structured;
}

template <class T>
consteval bool is_structured_class_type()
{
  if constexpr (std::is_destructible_v<T>) {
    return is_structured_class_type(std::is_destructible<T>{});
  } else {
    return false;
  }
}

template <class T>
consteval bool is_structured_union_type(std::is_trivially_destructible<T>)
{
  auto at_least_one_member_is_structured = false;
  REFLECT_CPP26_EXPAND(all_direct_nonstatic_data_members_of(^^T)).for_each(
    [&at_least_one_member_is_structured](auto member) {
      if (!is_public(member.value) || is_volatile(member.value) ||
          is_mutable_member(member.value)) {
        return true; // Proceeds to next member
      }
      constexpr auto type = type_of(member.value);
      if constexpr (is_array_type(type)) {
        at_least_one_member_is_structured |=
          is_structured_type_v<[:remove_all_extents(type):]>;
      } else {
        at_least_one_member_is_structured |= is_structured_type_v<[:type:]>;
      }
      return /* Proceeds if */ !at_least_one_member_is_structured;
    });
  return at_least_one_member_is_structured;
}

template <class T>
consteval bool is_structured_union_type()
{
  if constexpr (std::is_trivially_destructible_v<T>) {
    return is_structured_union_type(std::is_trivially_destructible<T>{});
  } else {
    return false;
  }
}

template <class T>
consteval bool is_structured_type()
{
  if constexpr (std::is_reference_v<T>) {
    return std::is_lvalue_reference_v<T>; // Filters out rvalue references
  } else if constexpr (is_value_initializable_structured_type_v<T>) {
    return true; // Including all scalar types
  } else if constexpr (std::is_union_v<T>) {
    return is_structured_union_type<std::remove_cv_t<T>>();
  } else if constexpr (std::is_class_v<T>) {
    return is_structured_class_type<std::remove_cv_t<T>>();
  } else {
    return false;
  }
}
} // namespace impl
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPE_HPP
