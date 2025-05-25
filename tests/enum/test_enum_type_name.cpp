#include "tests/enum/enum_test_options.hpp"
#include "tests/enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_type_name.hpp>
#endif

using namespace reflect_cpp26;

static_assert("foo_signed" == enum_type_name<foo_signed>());
static_assert("foo_signed_reorder"
  == enum_type_name<const foo_signed_reorder>());
static_assert("foo_signed_rep"
  == enum_type_name<volatile foo_signed_rep>());
static_assert("bar_unsigned"
  == enum_type_name<const volatile bar_unsigned>());

template <size_t I>
struct indirect {};

template <>
struct indirect<0> {
  using type = baz_signed;
};

template <>
struct indirect<1> {
  using type = const qux_unsigned;
};

template <>
struct indirect<2> {
  using type = volatile empty;
};

template <>
struct indirect<3> {
  using type = const volatile single;
};

static_assert("baz_signed" == enum_type_name<indirect<0>::type>());
static_assert("baz_signed"
  == enum_type_name<const volatile indirect<0>::type>());
static_assert("qux_unsigned" == enum_type_name<indirect<1>::type>());
static_assert("qux_unsigned"
  == enum_type_name<const volatile indirect<1>::type>());
static_assert("empty" == enum_type_name<indirect<2>::type>());
static_assert("single" == enum_type_name<indirect<3>::type>());

TEST(EnumTypeName, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
