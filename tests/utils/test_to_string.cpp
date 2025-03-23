#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/utils/to_string.hpp>

namespace rfl = reflect_cpp26;

TEST(UtilsToString, CharToString)
{
  EXPECT_EQ("A", rfl::to_string('A'));
  EXPECT_EQ("x", rfl::to_string(L'x'));
  EXPECT_EQ("'+'", rfl::to_string(u8'+', true));
  EXPECT_EQ("6", rfl::to_string(u'6'));
  EXPECT_EQ("' '", rfl::to_string(U' ', true));

  EXPECT_EQ("\\x00", rfl::to_string('\0'));
  EXPECT_EQ("\\x01", rfl::to_string('\1'));
  EXPECT_EQ("\\xfe", rfl::to_string(static_cast<char>(-2)));
  EXPECT_EQ("\\x81", rfl::to_string(static_cast<char8_t>(129)));
  EXPECT_EQ("\\x82", rfl::to_string(L'\u0082'));
  EXPECT_EQ("'\\x83'", rfl::to_string(u'\u0083', true));
  EXPECT_EQ("\\x84", rfl::to_string(U'\U00000084'));

  EXPECT_EQ("\\u0394", rfl::to_string(L'\u0394'));
  EXPECT_EQ("\\u0395", rfl::to_string(u'\u0395'));
  EXPECT_EQ("\\u0396", rfl::to_string(U'\u0396'));
  EXPECT_EQ("'\\U0001f604'", rfl::to_string(U'\U0001F604', true));

  static_assert(rfl::to_string('A') == "A");
  static_assert(rfl::to_string(L'\u0082', true) == "'\\x82'");
  static_assert(rfl::to_string(u'\u0394') == "\\u0394");
  static_assert(rfl::to_string(U'\U0001F604', true) == "'\\U0001f604'");
}

TEST(UtilsToString, IntToString)
{
  EXPECT_EQ("12345", rfl::to_string(12345));
  EXPECT_EQ("-128", rfl::to_string(static_cast<int8_t>(128)));
  EXPECT_EQ("128", rfl::to_string(static_cast<uint8_t>(-128)));

  EXPECT_EQ("1001001100101100000001011010010", rfl::to_string(1234567890, 2));
  EXPECT_EQ("-10012001001112202200", rfl::to_string(-1234567890, 3));
  EXPECT_EQ("11145401322", rfl::to_string(1234567890, 8));
  EXPECT_EQ("-2a5555016", rfl::to_string(-1234567890, 12));
  EXPECT_EQ("499602d2", rfl::to_string(1234567890, 16));
  EXPECT_EQ("-kf12oi", rfl::to_string(-1234567890, 36));

  EXPECT_EQ("-80",
            rfl::to_string(std::numeric_limits<int8_t>::lowest(), 16));
  EXPECT_EQ("-80" + std::string(2, '0'),
            rfl::to_string(std::numeric_limits<int16_t>::lowest(), 16));
  EXPECT_EQ("-80" + std::string(6, '0'),
            rfl::to_string(std::numeric_limits<int32_t>::lowest(), 16));
  EXPECT_EQ("-80" + std::string(14, '0'),
            rfl::to_string(std::numeric_limits<int64_t>::lowest(), 16));

  EXPECT_EQ(std::string(2, 'f'),
            rfl::to_string(std::numeric_limits<uint8_t>::max(), 16));
  EXPECT_EQ(std::string(4, 'f'),
            rfl::to_string(std::numeric_limits<uint16_t>::max(), 16));
  EXPECT_EQ(std::string(8, 'f'),
            rfl::to_string(std::numeric_limits<uint32_t>::max(), 16));
  EXPECT_EQ(std::string(16, 'f'),
            rfl::to_string(std::numeric_limits<uint64_t>::max(), 16));

  EXPECT_EQ("-1" + std::string(7, '0'),
            rfl::to_string(std::numeric_limits<int8_t>::lowest(), 2));
  EXPECT_EQ("-1" + std::string(15, '0'),
            rfl::to_string(std::numeric_limits<int16_t>::lowest(), 2));
  EXPECT_EQ("-1" + std::string(31, '0'),
            rfl::to_string(std::numeric_limits<int32_t>::lowest(), 2));
  EXPECT_EQ("-1" + std::string(63, '0'),
            rfl::to_string(std::numeric_limits<int64_t>::lowest(), 2));

  EXPECT_EQ(std::string(8, '1'),
            rfl::to_string(std::numeric_limits<uint8_t>::max(), 2));
  EXPECT_EQ(std::string(16, '1'),
            rfl::to_string(std::numeric_limits<uint16_t>::max(), 2));
  EXPECT_EQ(std::string(32, '1'),
            rfl::to_string(std::numeric_limits<uint32_t>::max(), 2));
  EXPECT_EQ(std::string(64, '1'),
            rfl::to_string(std::numeric_limits<uint64_t>::max(), 2));

  static_assert("12345" == rfl::to_string(12345));
  static_assert("-kf12oi" == rfl::to_string(-1234567890, 36));
  static_assert("-1" + std::string(63, '0') ==
                rfl::to_string(std::numeric_limits<int64_t>::lowest(), 2));
  static_assert(std::string(16, 'f') ==
                rfl::to_string(std::numeric_limits<uint64_t>::max(), 16));
}

// Note: Float-to-string tests are validated with the following compilers:
// * x86-64 GCC 14 (where long double is 80-bit)
// * x86-64 Clang 20 (where the underlying std::to_chars is not implemented for
//                    long double and the implementation for double is used
//                    as alternative)
// The tests are unsafe with:
// * Different bit representation of long double
//   (e.g. Some platforms may support 128-bit IEEE quadraple,
//         whose expected result is NOT listed in each case)
// * Different implementation of underlying std::to_chars
// In case of test failure due to the unsafety issues above, you can:
// * Fix the cases manually (as long as the actual behavior is as expected);
// * Disable this test with macro REFLECT_CPP26_DISABLE_FLOAT_TO_STRING_TEST
#ifndef REFLECT_CPP26_DISABLE_FLOAT_TO_STRING_TEST
TEST(UtilsToString, FloatToString)
{
  EXPECT_EQ("9.875", rfl::to_string(9.875));
  EXPECT_EQ("12345", rfl::to_string(12345.0f));
  EXPECT_THAT(rfl::to_string(123456789012345.0L),
              testing::AnyOf("1.23456789012345e+14", "123456789012345"));
  EXPECT_THAT(rfl::to_string(-1234567890.12345),
              testing::AnyOf("-1.23456789012345e+09", "-1234567890.12345"));
  EXPECT_EQ("0.01235",
            rfl::to_string(0.0123456789, std::chars_format::general, 4));
  EXPECT_EQ("-1.235e-10",
            rfl::to_string(-0.000'000'000'123'456'123L,
                           std::chars_format::general, 4));

  EXPECT_EQ("125.125",
            rfl::to_string(125.125, std::chars_format::fixed));
  EXPECT_EQ("-125.12500",
            rfl::to_string(-125.125f, std::chars_format::fixed, 5));
  EXPECT_EQ("125",
            rfl::to_string(125.125, std::chars_format::fixed, 0));
  EXPECT_EQ("987654321098765.0",
            rfl::to_string(987654321098765.0L, std::chars_format::fixed, 1));
  EXPECT_THAT(rfl::to_string(9876543210.98765, std::chars_format::fixed, 10),
              testing::AllOf(testing::StartsWith("9876543210.9876"),
                             testing::SizeIs(21)));
  EXPECT_THAT(rfl::to_string(-9876543210.987654321L,
                             std::chars_format::fixed, 10),
              testing::AllOf(testing::StartsWith("-9876543210.98765"),
                             testing::SizeIs(22)));
  EXPECT_EQ("-0.0000",
            rfl::to_string(-0.000'000'000'123'456'123L,
                           std::chars_format::fixed, 4));

  EXPECT_EQ("1.ec4p+6", rfl::to_string(123.0625, std::chars_format::hex));
  EXPECT_EQ("-1.fp+6", rfl::to_string(-123.0625, std::chars_format::hex, 1));
  EXPECT_EQ("1.ec400p+6", rfl::to_string(123.0625, std::chars_format::hex, 5));

  EXPECT_EQ("-1.81cd6ep+13",
            rfl::to_string(-12345.6789f, std::chars_format::hex));
  EXPECT_EQ("1.81cd6e631f8a1p+13",
            rfl::to_string(12345.6789, std::chars_format::hex));
  // Note: * std::to_chars with long double is not implemented by Clang 20
  //         and the implementation for double is used as alternative.
  //       * The result with long double is obtained with x86-64 GCC 14
  //         where long double is 80 bits (1 sign + 15 exponent + 64 mantissa).
  //         Correct checking with 128-bit IEEE quadraple is NOT IMPLEMENTED.
  EXPECT_THAT(rfl::to_string(-12345.6789L, std::chars_format::hex),
              testing::AnyOf("-c.0e6b7318fc50481p+10", "-1.81cd6e631f8a1p+13"));
  EXPECT_EQ("1.d6f3460000000000p+29",
            rfl::to_string(987654321.987654321f, std::chars_format::hex, 16));
  EXPECT_EQ("-1.d6f3458fe6b75000p+29",
            rfl::to_string(-987654321.987654321, std::chars_format::hex, 16));
  // Note: Same as above.
  EXPECT_THAT(rfl::to_string(987654321.987654321L, std::chars_format::hex, 16),
              testing::AnyOf("e.b79a2c7f35ba7820p+26",
                             "1.d6f3458fe6b75000p+29"));
  EXPECT_EQ("-1.0f7cp-33",
            rfl::to_string(-0.000'000'000'123'456'123L,
                           std::chars_format::hex, 4));

  EXPECT_EQ("-1.fffffep+127",
            rfl::to_string(-FLT_MAX, std::chars_format::hex));
  EXPECT_EQ("1.fffffffffffffp+1023",
            rfl::to_string(DBL_MAX, std::chars_format::hex));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MAX, std::chars_format::hex),
              testing::AnyOf("-f.fffffffffffffffp+16380", "-inf"));
  EXPECT_EQ("-1p-126",
            rfl::to_string(-FLT_MIN, std::chars_format::hex));
  EXPECT_EQ("1p-1022",
            rfl::to_string(DBL_MIN, std::chars_format::hex));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MIN, std::chars_format::hex),
              testing::AnyOf("-8p-16385", "-0p+0"));
  EXPECT_THAT(rfl::to_string(-FLT_TRUE_MIN, std::chars_format::hex),
              testing::AnyOf("-1p-149", "-0.000002p-126"));
  EXPECT_THAT(rfl::to_string(DBL_TRUE_MIN, std::chars_format::hex),
              testing::AnyOf("1p-1074", "0.0000000000001p-1022"));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_TRUE_MIN, std::chars_format::hex),
              testing::AnyOf("-8p-16448", "-0p+0"));

  EXPECT_EQ("-3.4028235e+38",
            rfl::to_string(-FLT_MAX, std::chars_format::general));
  EXPECT_EQ("1.7976931348623157e+308",
            rfl::to_string(DBL_MAX, std::chars_format::general));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MAX, std::chars_format::general),
              testing::AnyOf("-1.189731495357231765e+4932", "-inf"));
  EXPECT_EQ("-1.1754944e-38",
            rfl::to_string(-FLT_MIN, std::chars_format::general));
  EXPECT_EQ("2.2250738585072014e-308",
            rfl::to_string(DBL_MIN, std::chars_format::general));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MIN, std::chars_format::general),
              testing::AnyOf("-3.3621031431120935063e-4932", "-0"));
  EXPECT_THAT(rfl::to_string(-FLT_TRUE_MIN, std::chars_format::general),
              testing::AnyOf("-1e-45"));
  EXPECT_THAT(rfl::to_string(DBL_TRUE_MIN, std::chars_format::general),
              testing::AnyOf("5e-324"));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_TRUE_MIN, std::chars_format::general),
              testing::AnyOf("-4e-4951", "-0"));

  EXPECT_EQ("-340282346638528859811704183484516925440",
            rfl::to_string(-FLT_MAX, std::chars_format::fixed));
  EXPECT_THAT(rfl::to_string(DBL_MAX, std::chars_format::fixed),
              testing::AllOf(
                testing::StartsWith("17976931348623157081452742373170435679"),
                testing::SizeIs(309)));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MAX, std::chars_format::fixed),
              testing::AnyOf(
                testing::AllOf(
                  testing::StartsWith("-1189731495357231765021263853030970205"),
                  testing::SizeIs(4934)),
                "-inf"));
  EXPECT_EQ("-0." + std::string(37, '0') + "11754944",
            rfl::to_string(-FLT_MIN, std::chars_format::fixed));
  EXPECT_EQ("0." + std::string(307, '0') + "22250738585072014",
            rfl::to_string(DBL_MIN, std::chars_format::fixed));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_MIN, std::chars_format::fixed),
              testing::AnyOf(
                "-0." + std::string(4951, '0') + "33621031431120935063",
                "-0"));
  EXPECT_THAT(rfl::to_string(-FLT_TRUE_MIN, std::chars_format::fixed),
              testing::AnyOf(
                "-0." + std::string(44, '0') + "1"));
  EXPECT_THAT(rfl::to_string(DBL_TRUE_MIN, std::chars_format::fixed),
              testing::AnyOf(
                "0." + std::string(323, '0') + "5"));
  // Note: Same as above
  EXPECT_THAT(rfl::to_string(-LDBL_TRUE_MIN, std::chars_format::fixed),
              testing::AnyOf(
                "-0." + std::string(4950, '0') + "4",
                "-0"));
  // Testing extremely large precision
  constexpr auto extremely_large_precision = 10'000'000;
  EXPECT_THAT(rfl::to_string(
    DBL_MIN, std::chars_format::fixed, extremely_large_precision),
              testing::AllOf(
                testing::StartsWith(
                  "0." + std::string(307, '0') + "2225073858507201"),
                testing::SizeIs(extremely_large_precision + 2)));
  EXPECT_THAT(rfl::to_string(
    DBL_MIN, std::chars_format::general, extremely_large_precision),
              testing::AllOf(
                testing::StartsWith("2.225073858507201"),
                testing::EndsWith("e-308")));
}
#endif // #ifndef REFLECT_CPP26_DISABLE_FLOAT_TO_STRING_TEST

TEST(UtilsToString, StringToString)
{
  auto non_const_str = std::string("non-const");
  EXPECT_EQ("non-const", rfl::to_string(non_const_str));

  auto const_str = std::string("const");
  EXPECT_EQ("const", rfl::to_string(const_str));

  EXPECT_EQ("prvalue", rfl::to_string(std::string("prvalue")));

  auto xrvalue_str = std::string("xrvalue");
  EXPECT_EQ("xrvalue", rfl::to_string(std::move(xrvalue_str)));

  EXPECT_EQ("literal", rfl::to_string("literal"));
  EXPECT_EQ("literal", rfl::to_string("literal\0with\0null\0inside"));

  char char_array[] = "char[]";
  EXPECT_EQ("char[]", rfl::to_string(char_array));
  char char_array_with_null[] = "char[]\0with\0null\0inside";
  EXPECT_EQ("char[]", rfl::to_string(char_array_with_null));

  auto sv = std::string_view{"string_view"};
  EXPECT_EQ("string_view", rfl::to_string(sv));

  auto sv_with_null = std::string_view{
    char_array_with_null, std::size(char_array_with_null)};
  EXPECT_EQ("char[]\0with\0null\0inside", rfl::to_string(sv_with_null));
}
