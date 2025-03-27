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

  EXPECT_EQ("\\0", rfl::to_string('\0'));
  EXPECT_EQ("'\\t'", rfl::to_string(static_cast<wchar_t>(9), true));
  EXPECT_EQ("\\n", rfl::to_string(static_cast<char8_t>(10)));
  EXPECT_EQ("'\\v'", rfl::to_string(static_cast<char16_t>(11), true));
  EXPECT_EQ("\\f", rfl::to_string(static_cast<char32_t>(12)));
  EXPECT_EQ("'\\r'", rfl::to_string(static_cast<char32_t>(13), true));

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

TEST(UtilsToString, FloatToString)
{
  EXPECT_EQ("9.875", rfl::to_string(9.875));
  EXPECT_EQ("12345", rfl::to_string(12345.0f));
  EXPECT_THAT(rfl::to_string(123456789012345.0L),
              testing::AnyOf("1.23456789012345e+14", "123456789012345"));
  EXPECT_THAT(rfl::to_string(-1234567890.12345),
              testing::AnyOf("-1.23456789012345e+09", "-1234567890.12345"));
  EXPECT_EQ("0.01235", rfl::to_string(
    0.0123456789, std::chars_format::general, 4));
  EXPECT_EQ("-1.235e-10", rfl::to_string(
    -0.000'000'000'123'456'123L, std::chars_format::general, 4));

  EXPECT_EQ("-3.4028235e+38", rfl::to_string(-FLT_MAX));
  EXPECT_EQ("1.7976931348623157e+308", rfl::to_string(DBL_MAX));
  EXPECT_EQ("-1.1754944e-38", rfl::to_string(-FLT_MIN));
  EXPECT_EQ("2.2250738585072014e-308", rfl::to_string(DBL_MIN));

  EXPECT_EQ("1.ec4p+6", rfl::to_string(123.0625, std::chars_format::hex));
  EXPECT_EQ("-1.fp+6", rfl::to_string(-123.0625f, std::chars_format::hex, 1));
  EXPECT_EQ("1.ec400p+6", rfl::to_string(123.0625L, std::chars_format::hex, 5));

  EXPECT_EQ("-1.81cd6ep+13",
            rfl::to_string(-12345.6789f, std::chars_format::hex));
  EXPECT_EQ("1.81cd6e631f8a1p+13",
            rfl::to_string(12345.6789, std::chars_format::hex));
  EXPECT_EQ("1.d6f3460000000000p+29",
            rfl::to_string(987654321.987654321f, std::chars_format::hex, 16));
  EXPECT_EQ("-1.d6f3458fe6b75000p+29",
            rfl::to_string(-987654321.987654321, std::chars_format::hex, 16));

  EXPECT_EQ("-1.fffffep+127",
            rfl::to_string(-FLT_MAX, std::chars_format::hex));
  EXPECT_EQ("1.fffffffffffffp+1023",
            rfl::to_string(DBL_MAX, std::chars_format::hex));
  EXPECT_EQ("-1p-126",
            rfl::to_string(-FLT_MIN, std::chars_format::hex));
  EXPECT_EQ("1p-1022",
            rfl::to_string(DBL_MIN, std::chars_format::hex));

  // Testing extremely large precision
  constexpr auto extremely_large_precision = 10'000'000;
  EXPECT_THAT(rfl::to_string(
    DBL_MIN, std::chars_format::general, extremely_large_precision),
    testing::AllOf(testing::StartsWith("2.225073858507"),
                   testing::EndsWith("e-308")));
}

TEST(UtilsToString, StringToString)
{
  auto non_const_str = std::string("non-const");
  EXPECT_EQ("non-const", rfl::to_string(non_const_str));
  const auto const_str = std::string("const");
  EXPECT_EQ("const", rfl::to_string(const_str));
  EXPECT_EQ("prvalue", rfl::to_string(std::string("prvalue")));
  auto xrvalue_str = std::string("xrvalue");
  EXPECT_EQ("xrvalue", rfl::to_string(std::move(xrvalue_str)));
  EXPECT_EQ("literal", rfl::to_string("literal"));
  char char_array[] = "char[]";
  EXPECT_EQ("char[]", rfl::to_string(char_array));
  auto sv = std::string_view{"string_view"};
  EXPECT_EQ("string_view", rfl::to_string(sv));
  auto meta_sv = rfl::meta_string_view{"meta_string_view"};
  EXPECT_EQ("meta_string_view", rfl::to_string(meta_sv));
}

TEST(UtilsToString, StringToDisplayString)
{
  auto non_const_str = std::string("ab\nde\tgh\vjk\fmn\rpq");
  EXPECT_EQ(R"("ab\nde\tgh\vjk\fmn\rpq")",
            rfl::to_string(non_const_str, true));
  const auto const_str = std::string("ab\x01 cd \x02 \nef \x03gh");
  EXPECT_EQ(R"("ab\x01 cd \x02 \nef \x03gh")",
            rfl::to_string(const_str, true));
  EXPECT_EQ(R"("\n\t\xab")",
            rfl::to_string(std::string("\n\t\xAB"), true));
  auto xrvalue_str = std::string(" \f, \\; \v ");
  EXPECT_EQ(R"(" \f, \\; \v ")",
            rfl::to_string(std::move(xrvalue_str), true));
  EXPECT_EQ(R"("'\x04',\"\x80\",'\xff'")",
            rfl::to_string("'\x04',\"\x80\",'\xFF'", true));
  char char_array[] = "char_array[] =\n\t\"asdf\";";
  EXPECT_EQ(R"("char_array[] =\n\t\"asdf\";")",
            rfl::to_string(char_array, true));
  char char_array_with_null[] = "null\0inside";
  auto sv = std::string_view{
    char_array_with_null, std::size(char_array_with_null)};
  EXPECT_EQ(R"("null\0inside\0")", rfl::to_string(sv, true));
  auto meta_sv = rfl::meta_string_view{"\"meta_string_view\""};
  EXPECT_EQ(R"("\"meta_string_view\"")", rfl::to_string(meta_sv, true));

  EXPECT_EQ(R"("\xf0")", rfl::to_string("\xf0", true));
  EXPECT_EQ(R"("\\")", rfl::to_string("\\", true));
  EXPECT_EQ(R"("\\\\\\\\")", rfl::to_string("\\\\\\\\", true));
}
