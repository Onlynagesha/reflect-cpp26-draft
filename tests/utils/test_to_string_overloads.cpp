#include <gtest/gtest.h>

#define REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
#include <reflect_cpp26/utils/to_string.hpp>

namespace rfl = reflect_cpp26;

TEST(UtilsToString, Overloads)
{
  auto non_const_str = std::string("non-const");
  EXPECT_EQ("to_string(const basic_string&)", rfl::to_string(non_const_str));
  const auto const_str = std::string("const");
  EXPECT_EQ("to_string(const basic_string&)", rfl::to_string(const_str));
  EXPECT_EQ("to_string(basic_string&&)",
            rfl::to_string(std::string("prvalue")));
  auto xrvalue_str = std::string("xrvalue");
  EXPECT_EQ("to_string(basic_string&&)",
            rfl::to_string(std::move(xrvalue_str)));
  EXPECT_EQ("to_string(const char*)", rfl::to_string("literal"));
  char char_array[] = "char[]";
  EXPECT_EQ("to_string(const char*)", rfl::to_string(char_array));
  auto sv = std::string_view{non_const_str};
  EXPECT_EQ("to_string(basic_string_view)", rfl::to_string(sv));
  auto meta_sv = rfl::meta_string_view{"meta_string_view"};
  EXPECT_EQ("to_string(meta_string_view)", rfl::to_string(meta_sv));
}
