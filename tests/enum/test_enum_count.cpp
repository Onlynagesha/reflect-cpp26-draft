#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_count.hpp>
#endif

using namespace reflect_cpp26;

TEST(EnumCount, All)
{
  EXPECT_EQ_STATIC(9, enum_count<foo_signed>());
  EXPECT_EQ_STATIC(9, enum_count<const foo_signed_reorder>());
  EXPECT_EQ_STATIC(11, enum_count<volatile foo_signed_rep>());
  EXPECT_EQ_STATIC(11, enum_count<const volatile bar_unsigned>());
  EXPECT_EQ_STATIC(5, enum_count<baz_signed>());
  EXPECT_EQ_STATIC(6, enum_count<qux_unsigned>());
  EXPECT_EQ_STATIC(0, enum_count<empty>());
  EXPECT_EQ_STATIC(1, enum_count<const single>());
  EXPECT_EQ_STATIC(6, enum_count<volatile single_rep>());
  EXPECT_EQ_STATIC(141, enum_count<const volatile color>());
  EXPECT_EQ_STATIC(16, enum_count<terminal_color>());
  EXPECT_EQ_STATIC(2, enum_count<hash_collision>());
}
