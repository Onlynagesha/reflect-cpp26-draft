#include <gtest/gtest.h>
#include <reflect_cpp26/utils/ctype.h>
#include <cctype>

namespace rfl = reflect_cpp26;

TEST(UtilsMisc, CType)
{
  for (unsigned char i = 0; i <= 127; i++) {
#define MAKE_EXPECT_EQ(fn)                            \
    EXPECT_EQ(!!std::fn(i), rfl::fn(i))               \
      << std::format("Wrong " #fn "('\\x{:x}')", i);

    REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(MAKE_EXPECT_EQ)
#undef MAKE_EXPECT_EQ
  }

  for (signed char i = -128; i < 0; i++) {
#define MAKE_EXPECT_EQ(fn)                                    \
    EXPECT_FALSE(rfl::fn(i))                                  \
      << std::format("Wrong " #fn "('\\x{:x}')", (uint8_t)i);

    REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(MAKE_EXPECT_EQ)
#undef MAKE_EXPECT_EQ
  }

  EXPECT_FALSE(rfl::isalpha(256 + 'A')) << "Failed to detect overflow.";
  EXPECT_FALSE(rfl::isalpha(65536u + 'A')) << "Failed to detect overflow.";
}
