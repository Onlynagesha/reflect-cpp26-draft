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
  // Note: cvref qualifiers of T may affect the result.
  return type_tuple_apply_v<std::is_invocable_r,
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

/**
 * Whether Member is a reflection to an accessible non-static member
 * variable or function of T, either directly defined by T or
 * inherited from public base classes.
 */
template <class T, std::meta::info Member>
constexpr auto is_accessible_by_member_reflection_v = []() {
  if constexpr (is_nonstatic_data_member(Member)) {
    return requires (T t) { t.[:Member:]; };
  } else if constexpr (is_class_member(Member) && !is_static_member(Member)) {
    // Note: cvref qualifiers of T may affect the result.
    return is_accessible_by_member_function_pointer_v<T, &[:Member:]>;
  } else {
    static_assert(false, "Member must be a non-static class member");
  }
}();

template <class T, std::meta::info Member>
concept accessible_by_member_reflection =
  is_accessible_by_member_reflection_v<T, Member>;

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
      // TODO: Use correct base_offset
      // after compiler bug of offset_of being fixed
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
    res &= is_accessible_by_member_reflection_v<T, cur_spec.value.member>;
    // No union member is allowed.
    res &= !is_union_type(type_of(cur_spec.value.member));
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
 *   (2) T has no virtual member functions;
 *   (3) Every direct base class is a flattenable aggregate, recursively.
 */
template <class T>
constexpr auto is_flattenable_aggregate_v = std::is_array_v<T>;

template <class T>
  requires (std::is_class_v<T> && std::is_aggregate_v<T>)
constexpr auto is_flattenable_aggregate_v<T> =
  is_flattenable_v<T>
    && impl::is_flattenable_aggregate<std::remove_cv_t<T>>();

template <class T>
concept flattenable_aggregate = is_flattenable_aggregate_v<T>;

namespace impl {
template <class T>
consteval auto is_flattenable_aggregate() -> bool
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

struct reduce_memberwise_result_t {
  size_t true_count;
  size_t false_count;
};

template <template <class...> class Predicate, class... Ts>
consteval auto reduce_direct_memberwise()
{
  auto true_count = 0zU;
  auto false_count = 0zU;
  constexpr auto min_member_size = std::ranges::min({
    currently_accessible_nonstatic_data_members_of(^^Ts).size()...});

  REFLECT_CPP26_EXPAND_I(min_member_size).for_each(
    [&true_count, &false_count](auto I) {
      constexpr auto cur_result = Predicate<[:type_of(
        currently_accessible_nonstatic_data_members_of(^^Ts)[I]):]...>::value;
      (cur_result ? true_count : false_count) += 1;
    });
  return reduce_memberwise_result_t{true_count, false_count};
}

template <template <class...> class Predicate, class... Ts>
constexpr auto reduce_direct_memberwise_v =
  reduce_direct_memberwise<Predicate, std::remove_cv_t<Ts>...>();

template <class Predicate, class... Ts>
consteval auto reduce_direct_memberwise_meta()
{
  constexpr auto invocable_with_info = type_tuple_is_invocable_r_v<
    bool, Predicate, type_tuple_repeat_t<std::meta::info, sizeof...(Ts)>>;
  static_assert(invocable_with_info,
    "Invalid call signature: Expects (std::meta::info...) -> bool");

  auto true_count = 0zU;
  auto false_count = 0zU;
  constexpr auto min_member_size = std::ranges::min({
    currently_accessible_nonstatic_data_members_of(^^Ts).size()...});

  REFLECT_CPP26_EXPAND_I(min_member_size).for_each(
    [&true_count, &false_count](auto I) {
      auto cur_result = std::invoke(Predicate{},
        currently_accessible_nonstatic_data_members_of(^^Ts)[I]...);
      (cur_result ? true_count : false_count) += 1;
    });
  return reduce_memberwise_result_t{true_count, false_count};
}

template <class Predicate, class... Ts>
constexpr auto reduce_direct_memberwise_meta_v =
  reduce_direct_memberwise_meta<Predicate, std::remove_cv_t<Ts>...>();

template <template <class...> class Predicate, class... Ts>
consteval auto reduce_flattened_memberwise()
{
  auto true_count = 0zU;
  auto false_count = 0zU;
  constexpr auto min_member_size = std::ranges::min({
    flattened_accessible_nonstatic_data_members_v<Ts>.size()...});

  REFLECT_CPP26_EXPAND_I(min_member_size).for_each(
    [&true_count, &false_count](auto I) {
      constexpr auto cur_result = Predicate<[:type_of(get<I>(
        flattened_accessible_nonstatic_data_members_v<Ts>).member):]...>::value;
      (cur_result ? true_count : false_count) += 1;
    });
  return reduce_memberwise_result_t{true_count, false_count};
}

template <template <class...> class Predicate, class... Ts>
constexpr auto reduce_flattened_memberwise_v =
  reduce_flattened_memberwise<Predicate, std::remove_cv_t<Ts>...>();

template <class Predicate, class... Ts>
consteval auto reduce_flattened_memberwise_meta()
{
  constexpr auto invocable_with_spec = type_tuple_is_invocable_r_v<
    bool, Predicate, type_tuple_repeat_t<
      flattened_data_member_spec, sizeof...(Ts)>>;
  constexpr auto invocable_with_info = type_tuple_is_invocable_r_v<
    bool, Predicate, type_tuple_repeat_t<std::meta::info, sizeof...(Ts)>>;

  auto true_count = 0zU;
  auto false_count = 0zU;
  constexpr auto min_member_size = std::ranges::min({
    flattened_accessible_nonstatic_data_members_v<Ts>.size()...});

  if constexpr (invocable_with_spec) {
    REFLECT_CPP26_EXPAND_I(min_member_size).for_each(
      [&true_count, &false_count](auto I) {
        auto cur_result = std::invoke(Predicate{}, get<I>(
          flattened_accessible_nonstatic_data_members_v<Ts>)...);
        (cur_result ? true_count : false_count) += 1;
      });
  } else if constexpr (invocable_with_info) {
    REFLECT_CPP26_EXPAND_I(min_member_size).for_each(
      [&true_count, &false_count](auto I) {
        auto cur_result = std::invoke(Predicate{}, get<I>(
          flattened_accessible_nonstatic_data_members_v<Ts>).member...);
        (cur_result ? true_count : false_count) += 1;
      });
  } else {
    static_assert(false, "Invalid call signature: Expects "
      "either predicate(std::meta::info...) -> bool "
      "or predicate(flattened_data_member_spec...) -> bool");
  }
  return reduce_memberwise_result_t{true_count, false_count};
}

template <class Predicate, class... Ts>
constexpr auto reduce_flattened_memberwise_meta_v =
  reduce_flattened_memberwise_meta<Predicate, std::remove_cv_t<Ts>...>();
} // namespace impl

#define REFLECT_CPP26_MEMBERWISE_REDUCTION_V_LIST(F)  \
  F(all, false_count == 0)                            \
  F(any, true_count > 0)                              \
  F(none, true_count == 0)

#define REFLECT_CPP26_MAKE_DIRECT_MEMBERWISE_REDUCTION_V(reduction, pred) \
  template <template <class...> class Predicate, class... Ts>             \
    requires (std::is_class_v<Ts> && ...)                                 \
  constexpr auto reduction##_of_direct_memberwise_v =                     \
    impl::reduce_direct_memberwise_v<Predicate, Ts...>.pred;              \
                                                                          \
  template <class Predicate, class... Ts>                                 \
    requires (std::is_class_v<Ts> && ...)                                 \
  constexpr auto reduction##_of_direct_memberwise_meta_v =                \
    impl::reduce_direct_memberwise_meta_v<Predicate, Ts...>.pred;

/**
 * all|any|none_of_direct_memberwise_v<Predicate, Ts...>
 *   Checks whether Predicate<
 *     jth-direct-member-type-of(Ts...[0], j), ...,
 *     jth-direct-member-type-of(Ts...[K - 1], j)>::value
 *   is true for all/any/none of j = 0 to N - 1 where
 *     jth-direct-member-type-of(T, j) gets the type
 *       (qualifiers preserved) of j-th direct accessible NSDM of class T.
 *     N = min(count of direct accessible NSDMs of T) for T in Ts...
 *     K = sizeof...(Ts)
 *
 * all|any|none_of_direct_memberwise_meta_v<Predicate, Ts...>
 *   Checks whether Predicate{}.operator()(
 *     jth-direct-member-of(Ts...[0], j), ...,
 *     jth-direct-member-of(Ts...[K - 1], j))
 *   is true for all/any/none of j = 0 to N - 1 where
 *     jth-direct-member-of(T, j) gets the std::meta::info
 *       of j-th direct accessible non-static data member of class T.
 *     N and K are same as above.
 */
REFLECT_CPP26_MEMBERWISE_REDUCTION_V_LIST(
  REFLECT_CPP26_MAKE_DIRECT_MEMBERWISE_REDUCTION_V)

#define REFLECT_CPP26_MAKE_FLATTENED_MEMBERWISE_REDUCTION_V(reduction, pred)  \
  template <template <class...> class Predicate, class... Ts>                 \
    requires (is_partially_flattenable_v<Ts> && ...)                          \
  constexpr auto reduction##_of_flattened_memberwise_v =                      \
    impl::reduce_flattened_memberwise_v<Predicate, Ts...>.pred;               \
                                                                              \
  template <class Predicate, class... Ts>                                     \
    requires (is_partially_flattenable_v<Ts> && ...)                          \
  constexpr auto reduction##_of_flattened_memberwise_meta_v =                 \
    impl::reduce_flattened_memberwise_meta_v<Predicate, Ts...>.pred;

/**
 * all|any|none_of_flattened_memberwise_v<Predicate, Ts...>
 *   Checks whether Predicate<
 *     jth-flattened-member-type-of(Ts...[0], j), ...,
 *     jth-flattened-member-type-of(Ts...[K - 1], j)>::value
 *   is true for all/any/none of j = 0 to N - 1 where
 *     jth-flattened-member-type-of(T, j) gets the type
 *       (qualifiers preserved) of j-th flattened accessible NSDM of class T.
 *     N = min(count of flattened accessible NSDMs of T) for T in Ts...
 *     K = sizeof...(Ts)
 *
 * all|any|none_of_flattened_memberwise_meta_v<Predicate, Ts...>
 *   Checks whether Predicate{}.operator()(
 *     jth-flattened-member-of(Ts...[0], j), ...,
 *     jth-flattened-member-of(Ts...[K - 1], j))
 *   is true for all/any/none of j = 0 to N - 1 where
 *     jth-flattened-member-of(T, j) gets the
 *       std::meta::info or flattened_data_member_spec
 *       of j-th flattened accessible non-static data member of class T.
 *     N and K are same as above.
 */
 REFLECT_CPP26_MEMBERWISE_REDUCTION_V_LIST(
  REFLECT_CPP26_MAKE_FLATTENED_MEMBERWISE_REDUCTION_V)

#undef REFLECT_CPP26_MEMBERWISE_REDUCTION_V_LIST
#undef REFLECT_CPP26_MAKE_DIRECT_MEMBERWISE_REDUCTION_V
#undef REFLECT_CPP26_MAKE_FLATTENED_MEMBERWISE_REDUCTION_V

namespace impl {
template <class Transform, class... Ts>
struct aggregate_by_direct_memberwise_zip_transform {
  static_assert(type_tuple_is_invocable_r_v<
    std::meta::info, Transform, type_tuple_repeat_t<
      std::meta::info, sizeof...(Ts)>>,
    "Invalid call signature of Transform: "
    "Expects (std::meta::info...) -> std::meta::info");

  struct type;
  consteval {
    constexpr auto min_member_size = std::ranges::min({
      currently_accessible_nonstatic_data_members_of(^^Ts).size()...});
    auto members = std::vector<std::meta::info>{};
    members.reserve(min_member_size);

    REFLECT_CPP26_EXPAND_I(min_member_size).for_each([&members](auto I) {
      auto cur_member = std::invoke(Transform{},
        currently_accessible_nonstatic_data_members_of(^^Ts)[I]...);
      if (!is_data_member_spec(cur_member)) {
        throw "Result of transform function must be a data member spec.";
      }
      members.push_back(cur_member);
    });
    define_aggregate(^^type, members);
  }
};

template <class Transform, class... Ts>
struct aggregate_by_flattened_memberwise_zip_transform {
  static constexpr auto invocable_with_info = type_tuple_is_invocable_r_v<
    std::meta::info, Transform, type_tuple_repeat_t<
      std::meta::info, sizeof...(Ts)>>;
  static constexpr auto invocable_with_spec = type_tuple_is_invocable_r_v<
    std::meta::info, Transform, type_tuple_repeat_t<
      flattened_data_member_spec, sizeof...(Ts)>>;

  static_assert(invocable_with_info || invocable_with_spec,
    "Invalid call signature of Transform: Expects "
    "either (std::meta::info...) -> std::meta::info "
    "or (flattened_data_member_spec...) -> std::meta::info");

  static consteval auto ith_member_getter(auto I) -> std::meta::info
  {
    if constexpr (invocable_with_spec) {
      return std::invoke(Transform{}, get<I>(
        flattened_accessible_nonstatic_data_members_v<Ts>)...);
    } else {
      return std::invoke(Transform{}, get<I>(
        flattened_accessible_nonstatic_data_members_v<Ts>).member...);
    }
  }

  struct type;
  consteval {
    constexpr auto min_member_size = std::ranges::min({
      flattened_accessible_nonstatic_data_members_v<Ts>.size()...});
    auto members = std::vector<std::meta::info>{};
    members.reserve(min_member_size);

    REFLECT_CPP26_EXPAND_I(min_member_size).for_each([&members](auto I) {
      auto cur_member = ith_member_getter(I);
      if (!is_data_member_spec(cur_member)) {
        throw "Result of transform function must be a data member spec.";
      }
      members.push_back(cur_member);
    });
    define_aggregate(^^type, members);
  }
};
} // namespace impl

/**
 * Makes aggregate type whose j-th data member spec is
 *   Predicate{}.operator()(
 *     jth-direct-member-of(Ts...[0], j), ...,
 *     jth-direct-member-of(Ts...[K - 1], j)) for j = 0 to N - 1.
 */
template <class Transform, class... Ts>
  requires (std::is_class_v<Ts> && ...)
using aggregate_by_direct_memberwise_zip_transform_t =
  typename impl::aggregate_by_direct_memberwise_zip_transform<
    Transform, std::remove_cv_t<Ts>...>::type;

/**
 * Makes aggregate type whose j-th data member spec is
 *   Predicate{}.operator()(
 *     jth-flattened-member-of(Ts...[0], j), ...,
 *     jth-flattened-member-of(Ts...[K - 1], j)) for j = 0 to N - 1.
 */
template <class Transform, class... Ts>
  requires (is_partially_flattenable_v<Ts> && ...)
using aggregate_by_flattened_memberwise_zip_transform_t =
  typename impl::aggregate_by_flattened_memberwise_zip_transform<
    Transform, std::remove_cv_t<Ts>...>::type;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_CLASS_TYPE_HPP
