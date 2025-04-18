#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits.hpp>

namespace rfl = reflect_cpp26;

struct foo_t {
  int x;
  double y;
  char f_no_qualifier(int, float, double);
  char f_const(int, float) const;
  char f_volatile(const char*, const char*, volatile size_t*) volatile;
};

// Member object reflection with cv-qualifier
static_assert(rfl::is_accessible_by_member_reflection_v<
  foo_t, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const foo_t, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  volatile foo_t, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const volatile foo_t, ^^foo_t::y>);

// Member object reflection with reference
static_assert(rfl::is_accessible_by_member_reflection_v<
  foo_t&, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const foo_t&, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  volatile foo_t&, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const volatile foo_t&, ^^foo_t::y>);

// Member function reflection (no qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  foo_t, ^^foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const foo_t, ^^foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  volatile foo_t, ^^foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const volatile foo_t, ^^foo_t::f_no_qualifier>);

// Member function reflection (const qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  foo_t, ^^foo_t::f_const>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const foo_t, ^^foo_t::f_const>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  volatile foo_t, ^^foo_t::f_const>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const volatile foo_t, ^^foo_t::f_const>);

// Member function reflection (volatile qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  foo_t, ^^foo_t::f_volatile>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const foo_t, ^^foo_t::f_volatile>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  volatile foo_t, ^^foo_t::f_volatile>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const volatile foo_t, ^^foo_t::f_volatile>);

struct bar_t : foo_t {
  int arr[12];
  int& g_lvalue_ref(wchar_t* const*, char16_t&&, ...) &; // C-style va-args
  int& g_lvalue_cref(wchar_t*) const &;
  int& g_rvalue_ref(int, int, int) &&;
  int& g_rvalue_cref(const char*, size_t) const &&;
};

static_assert(rfl::is_accessible_by_member_reflection_v<
  volatile bar_t, ^^bar_t::arr>);

// Member function pointer (lvalue-reference qualifier)
static_assert(! rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t, ^^bar_t::g_lvalue_ref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t&, ^^bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t&, ^^bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  bar_t&&, ^^bar_t::g_lvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t&&, ^^bar_t::g_lvalue_ref>);

// Member function pointer (const lvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const bar_t, ^^bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t&, ^^bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const bar_t&, ^^bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t&&, ^^bar_t::g_lvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const bar_t&&, ^^bar_t::g_lvalue_cref>);

// Member function pointer (rvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t, ^^bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  bar_t&, ^^bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t&, ^^bar_t::g_rvalue_ref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t&&, ^^bar_t::g_rvalue_ref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t&&, ^^bar_t::g_rvalue_ref>);

// Member function pointer (const rvalue-reference qualifier)
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const bar_t, ^^bar_t::g_rvalue_cref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  bar_t&, ^^bar_t::g_rvalue_cref>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t&, ^^bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t&&, ^^bar_t::g_rvalue_cref>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  const bar_t&&, ^^bar_t::g_rvalue_cref>);

// Derived -> Based is OK
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^foo_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bar_t, ^^foo_t::f_no_qualifier>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  const bar_t, ^^foo_t::f_volatile>);
// Based -> Derived is clearly not.
static_assert(! rfl::is_accessible_by_member_reflection_v<
  foo_t, ^^bar_t::arr>);
static_assert(! rfl::is_accessible_by_member_reflection_v<
  foo_t&, ^^bar_t::g_lvalue_ref>);

struct baz_t : foo_t {
  const char* str;
};

// Diamond inheritance
struct qux_t : bar_t, baz_t {
  const wchar_t* str; // Shadows baz_t::str
};

static_assert(! rfl::is_accessible_by_member_reflection_v<
  qux_t, ^^bar_t::x>); // Fails due to ambiguity
static_assert(! rfl::is_accessible_by_member_reflection_v<
  qux_t, ^^bar_t::f_const>); // Fails due to ambiguity as well
static_assert(rfl::is_accessible_by_member_reflection_v<
  qux_t, ^^baz_t::str>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  qux_t, ^^qux_t::str>);

// References
struct references_A_t {
  int& i;
  const long& cl;
};

struct references_B_t : references_A_t {
  volatile float& vf;
  const volatile double& cvd;
};

// Note cannot form a pointer-to-member to member of reference type,
// thus is_accessible_by_member_pointer_v is not usable with reference members.
static_assert(rfl::is_accessible_by_member_reflection_v<
  references_B_t, ^^references_A_t::i>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  references_B_t, ^^references_A_t::cl>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  references_B_t, ^^references_B_t::vf>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  references_B_t, ^^references_B_t::cvd>);

// Bit-fields
struct bit_fields_A_t {
  int32_t x: 1;
  int32_t y: 2;
};

struct bit_fields_B_t : bit_fields_A_t {
  int32_t a: 4;
  int32_t b: 8;
};

// Note: Address of bit-field can not be requested,
// thus is_accessible_by_member_pointer_v is not usable with bit-field members.
static_assert(rfl::is_accessible_by_member_reflection_v<
  bit_fields_B_t, ^^bit_fields_A_t::x>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bit_fields_B_t, ^^bit_fields_A_t::y>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bit_fields_B_t, ^^bit_fields_B_t::a>);
static_assert(rfl::is_accessible_by_member_reflection_v<
  bit_fields_B_t, ^^bit_fields_B_t::b>);

TEST(TypeTraitsClassTypes, MemberReflections) {
  EXPECT_TRUE(true); // All test cases done with static-asserts above
}
