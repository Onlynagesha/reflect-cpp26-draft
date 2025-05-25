#include "tests/enum/enum_test_options.hpp"
#include "tests/enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_hash.hpp>
#endif

using namespace reflect_cpp26;

TEST(EnumHash, All)
{
  EXPECT_EQ_STATIC(enum_hash<foo_signed>(), enum_hash<foo_signed_reorder>());
  EXPECT_EQ_STATIC(enum_hash<const foo_signed>(),
                   enum_hash<volatile foo_signed_reorder>());
  EXPECT_NE_STATIC(enum_hash<foo_signed>(), enum_hash<foo_signed_rep>());
  EXPECT_NE_STATIC(enum_hash<foo_signed>(), enum_hash<bar_unsigned>());
  EXPECT_NE_STATIC(enum_hash<single>(), enum_hash<empty>());
  EXPECT_NE_STATIC(enum_hash<single>(), enum_hash<single_rep>());
}
