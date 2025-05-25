#include "tests/enum/enum_test_options.hpp"
#include "tests/enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_values.hpp>
#endif

using namespace reflect_cpp26;

TEST(EnumValues, All)
{
  EXPECT_EQ_STATIC(foo_signed_rep::yi, enum_value<foo_signed_rep>(3));
  EXPECT_EQ_STATIC(foo_signed_rep::four,
    enum_value<const foo_signed_rep, enum_entry_order::by_value>(7));
  EXPECT_EQ_STATIC(foo_signed_rep::yi,
    enum_value<volatile foo_signed_rep, enum_entry_order::by_name>(9));
}

TEST(EnumValueConstants, All)
{
  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constants<baz_signed>()),
    constant<baz_signed::ling, baz_signed::yi, baz_signed::er,
      baz_signed::san, baz_signed::fuyi>>);
  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constants<baz_signed, enum_entry_order::by_value>()),
    constant<baz_signed::fuyi, baz_signed::ling, baz_signed::yi,
      baz_signed::er, baz_signed::san>>);
  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constants<baz_signed, enum_entry_order::by_name>()),
    constant<baz_signed::er, baz_signed::fuyi, baz_signed::ling,
      baz_signed::san, baz_signed::yi>>);

  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constant<2, baz_signed>()), constant<baz_signed::er>>);
  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constant<2, baz_signed, enum_entry_order::by_value>()),
    constant<baz_signed::yi>>);
  EXPECT_TRUE_STATIC(std::is_same_v<
    decltype(enum_value_constant<2, baz_signed, enum_entry_order::by_name>()),
    constant<baz_signed::ling>>);
}
