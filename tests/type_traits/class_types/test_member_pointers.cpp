#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits/class_types.hpp>
#include <reflect_cpp26/type_traits/type_comparison.hpp>

namespace rfl = reflect_cpp26;

struct foo_t {
  int x;
  double y;
  char f_no_qualifier(int, float, double);
  char f_const(int, float) const;
  char f_volatile(const char*, const char*, volatile size_t*) volatile;
};

using traits_of_foo_x = rfl::member_pointer_traits<decltype(&foo_t::x)>;
static_assert(traits_of_foo_x::category
  == rfl::member_pointer_category::to_object);
static_assert(std::is_same_v<traits_of_foo_x::target_type, int>);
static_assert(std::is_same_v<traits_of_foo_x::direct_parent_type, foo_t>);

using traits_of_foo_f = rfl::member_pointer_traits<
  decltype(&foo_t::f_no_qualifier)>;
static_assert(traits_of_foo_f::category
  == rfl::member_pointer_category::to_function);
static_assert(std::is_same_v<traits_of_foo_f::target_type, char>);
static_assert(std::is_same_v<traits_of_foo_f::direct_parent_type, foo_t>);
static_assert(std::tuple_size_v<traits_of_foo_f::args_type> == 3);
static_assert(std::is_same_v<
  std::tuple_element_t<0, traits_of_foo_f::args_type>, int>);
static_assert(std::is_same_v<
  std::tuple_element_t<1, traits_of_foo_f::args_type>, float>);
static_assert(std::is_same_v<
  std::tuple_element_t<2, traits_of_foo_f::args_type>, double>);

using traits_of_foo_g = rfl::member_pointer_traits<
  decltype(&foo_t::f_volatile)>;
static_assert(traits_of_foo_g::category
  == rfl::member_pointer_category::to_function);
static_assert(std::is_same_v<traits_of_foo_g::target_type, char>);
static_assert(std::is_same_v<traits_of_foo_g::direct_parent_type, foo_t>);
static_assert(std::is_same_v<traits_of_foo_g::args_type,
  rfl::type_tuple<const char*, const char*, volatile size_t*>>);

// Member object pointer with cv-qualifier
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  foo_t, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  const foo_t, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  volatile foo_t, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  const volatile foo_t, &foo_t::y>);

// Member object pointer with reference
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  foo_t&, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  const foo_t&, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  volatile foo_t&, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  const volatile foo_t&, &foo_t::y>);

// Member function pointer (no qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  foo_t, &foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const foo_t, &foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  volatile foo_t, &foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const volatile foo_t, &foo_t::f_no_qualifier>);

// Member function pointer (const qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  foo_t, &foo_t::f_const>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const foo_t, &foo_t::f_const>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  volatile foo_t, &foo_t::f_const>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const volatile foo_t, &foo_t::f_const>);

// Member function pointer (volatile qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  foo_t, &foo_t::f_volatile>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const foo_t, &foo_t::f_volatile>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  volatile foo_t, &foo_t::f_volatile>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const volatile foo_t, &foo_t::f_volatile>);

struct bar_t : foo_t {
  int arr[12];
  int& g_lvalue_ref(wchar_t* const*, char16_t&&, ...) &; // C-style va-args
  int& g_lvalue_cref(wchar_t*) const &;
  int& g_rvalue_ref(int, int, int) &&;
  int& g_rvalue_cref(const char*, size_t) const &&;
};

using traits_of_bar_x = rfl::member_pointer_traits<decltype(&bar_t::x)>;
static_assert(std::is_same_v<traits_of_bar_x, traits_of_foo_x>);

using traits_of_bar_arr = rfl::member_pointer_traits<decltype(&bar_t::arr)>;
static_assert(traits_of_bar_arr::category
  == rfl::member_pointer_category::to_object);
static_assert(std::is_same_v<traits_of_bar_arr::target_type, int[12]>);
static_assert(std::is_same_v<traits_of_bar_arr::direct_parent_type, bar_t>);

using traits_of_bar_g = rfl::member_pointer_traits<
  decltype(&bar_t::g_lvalue_ref)>;
static_assert(traits_of_bar_g::category
  == rfl::member_pointer_category::to_function);
static_assert(std::is_same_v<traits_of_bar_g::target_type, int&>);
static_assert(std::is_same_v<traits_of_bar_g::direct_parent_type, bar_t>);
static_assert(std::is_same_v<traits_of_bar_g::args_type,
  rfl::type_tuple<wchar_t* const*, char16_t&&>>);
static_assert(traits_of_bar_g::function_traits_type::flags
  == rfl::function_trait_flags{
      .has_ellipsis_parameter = true,
      .is_lvalue_reference = true,
    });

static_assert(rfl::is_accessible_by_member_object_pointer_v<
  volatile bar_t, &bar_t::arr>);

// Member function pointer (lvalue-reference qualifier)
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  bar_t, &bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t, &bar_t::g_lvalue_ref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t&, &bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&, &bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  bar_t&&, &bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&&, &bar_t::g_lvalue_ref>);

// Member function pointer (const lvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t, &bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const bar_t, &bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t&, &bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&, &bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t&&, &bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&&, &bar_t::g_lvalue_cref>);

// Member function pointer (rvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t, &bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t, &bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  bar_t&, &bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&, &bar_t::g_rvalue_ref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t&&, &bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&&, &bar_t::g_rvalue_ref>);

// Member function pointer (const rvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t, &bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const bar_t, &bar_t::g_rvalue_cref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  bar_t&, &bar_t::g_rvalue_cref>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&, &bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t&&, &bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  const bar_t&&, &bar_t::g_rvalue_cref>);

// Derived -> Based is OK
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  bar_t, &foo_t::y>);
static_assert(rfl::is_accessible_by_member_function_pointer_v<
  bar_t, &foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  const bar_t, &foo_t::f_volatile>);
// Based -> Derived is clearly not.
static_assert(! rfl::is_accessible_by_member_object_pointer_v<
  foo_t, &bar_t::arr>);
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  foo_t&, &bar_t::g_lvalue_ref>);

struct baz_t : foo_t {
  const char* str;
};

// Diamond inheritance
struct qux_t : bar_t, baz_t {
  const wchar_t* str; // Shadows baz_t::str
};

using traits_of_baz_str = rfl::member_pointer_traits<decltype(&baz_t::str)>;
static_assert(std::is_same_v<traits_of_baz_str::target_type, char const*>);
static_assert(std::is_same_v<traits_of_baz_str::direct_parent_type, baz_t>);

using traits_of_qux_str = rfl::member_pointer_traits<decltype(&qux_t::str)>;
static_assert(std::is_same_v<traits_of_qux_str::target_type, wchar_t const*>);
static_assert(std::is_same_v<traits_of_qux_str::direct_parent_type, qux_t>);

// Diamond inheritance
static_assert(! rfl::is_accessible_by_member_object_pointer_v<
  qux_t, &bar_t::x>); // Fails due to ambiguity
static_assert(! rfl::is_accessible_by_member_function_pointer_v<
  qux_t, &bar_t::f_const>); // Fails due to ambiguity as well
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  qux_t, &baz_t::str>);
static_assert(rfl::is_accessible_by_member_object_pointer_v<
  qux_t, &qux_t::str>);

TEST(TypeTraitsClassTypes, MemberPointers) {
  EXPECT_TRUE(true); // All test cases done with static-asserts above
}
