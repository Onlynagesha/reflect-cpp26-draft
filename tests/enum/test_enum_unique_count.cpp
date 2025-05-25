#include "tests/enum/enum_test_options.hpp"
#include "tests/enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_count.hpp>
#endif

using namespace reflect_cpp26;

TEST(EnumUniqueCount, All)
{
  EXPECT_EQ_STATIC(9, enum_unique_count<foo_signed>());
  EXPECT_EQ_STATIC(9, enum_unique_count<const foo_signed_reorder>());
  EXPECT_EQ_STATIC(9, enum_unique_count<volatile foo_signed_rep>());
  EXPECT_EQ_STATIC(11, enum_unique_count<const volatile bar_unsigned>());
  EXPECT_EQ_STATIC(5, enum_unique_count<baz_signed>());
  EXPECT_EQ_STATIC(6, enum_unique_count<qux_unsigned>());
  EXPECT_EQ_STATIC(0, enum_unique_count<empty>());
  EXPECT_EQ_STATIC(1, enum_unique_count<const single>());
  EXPECT_EQ_STATIC(1, enum_unique_count<volatile single_rep>());
  // 2: Two duplicated pairs: ("aqua", "cyan") and ("magenta", "fuchsia")
  EXPECT_EQ_STATIC(141 - 2, enum_unique_count<const volatile color>());
  EXPECT_EQ_STATIC(16, enum_unique_count<terminal_color>());
  EXPECT_EQ_STATIC(2, enum_unique_count<hash_collision>());
}
