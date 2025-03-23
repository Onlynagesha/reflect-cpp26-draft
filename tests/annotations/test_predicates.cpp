#include <gtest/gtest.h>
#include <reflect_cpp26/annotations.hpp>

namespace rfl = reflect_cpp26;
namespace rfl_annots = reflect_cpp26::annotations;

struct foo_integral_t {
  REFLECT_CPP26_ANNOTATIONS(
    min = -10,
    max = 10)
  REFLECT_CPP26_ANNOTATE(
    // Tests safe integer comparison:
    // 4294967295 shall not be equal to int32_t{-1}
    excludes = std::vector<uint32_t>{1, 2, 4, 8, 16, 4294967295u})
  int32_t s;

  REFLECT_CPP26_ANNOTATE(min_exclusive = 10)
  REFLECT_CPP26_ANNOTATE(max_exclusive = 100)
  // Tests safe integer comparison:
  // -1 shall not be equal to uint32_t{4294967295}
  REFLECT_CPP26_ANNOTATE(options = {-1, 1, 10, 11, 99})
  uint32_t u;
};

TEST(AnnotationPredicates, Integral)
{
  auto obj_ok_1 = foo_integral_t{.s = -10, .u = 11};
  auto error_msg_ok = std::string{};
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_1));
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_1, &error_msg_ok));
  EXPECT_EQ("", error_msg_ok);

  auto obj_ok_2 = foo_integral_t{.s = 10, .u = 99};
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_2));
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_2, &error_msg_ok));
  EXPECT_EQ("", error_msg_ok);

  auto obj_ok_3 = foo_integral_t{.s = -1, .u = 99};
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_3));
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_3, &error_msg_ok));
  EXPECT_EQ("", error_msg_ok);

  auto obj_1 = foo_integral_t{.s = 2, .u = static_cast<unsigned>(-1)};
  auto error_msg_1 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_1));
  EXPECT_FALSE(rfl_annots::validate_members(obj_1, &error_msg_1));
  EXPECT_EQ("Invalid member 's': "
            "Expects value to be none of [1, 2, 4, 8, 16, 4294967295], "
            "while actual value = 2", error_msg_1);
  error_msg_1.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_1, &error_msg_1));
  EXPECT_EQ("Invalid member 's':"
            "\n* Expects value to be none of [1, 2, 4, 8, 16, 4294967295], "
            "while actual value = 2"
            "\nInvalid member 'u':"
            "\n* Expects value < 100, while actual value = 4294967295"
            "\n* Expects value to be any of [-1, 1, 10, 11, 99], "
            "while actual value = 4294967295", error_msg_1);

  auto obj_2 = foo_integral_t{.s = 16, .u = 10};
  auto error_msg_2 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_2, &error_msg_2));
  EXPECT_EQ("Invalid member 's': "
            "Expects value <= 10, while actual value = 16", error_msg_2);
  error_msg_2.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_2, &error_msg_2));
  EXPECT_EQ("Invalid member 's':"
            "\n* Expects value <= 10, while actual value = 16"
            "\n* Expects value to be none of [1, 2, 4, 8, 16, 4294967295], "
            "while actual value = 16"
            "\nInvalid member 'u':"
            "\n* Expects value > 10, while actual value = 10", error_msg_2);

  auto obj_3 = foo_integral_t{.s = -11, .u = 100};
  auto error_msg_3 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_3, &error_msg_3));
  EXPECT_EQ("Invalid member 's': "
            "Expects value >= -10, while actual value = -11", error_msg_3);
  error_msg_3.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_3, &error_msg_3));
  EXPECT_EQ("Invalid member 's':"
            "\n* Expects value >= -10, while actual value = -11"
            "\nInvalid member 'u':"
            "\n* Expects value < 100, while actual value = 100"
            "\n* Expects value to be any of [-1, 1, 10, 11, 99], "
            "while actual value = 100", error_msg_3);

  auto obj_4 = foo_integral_t{.s = 5, .u = 5};
  auto error_msg_4 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_4, &error_msg_4));
  EXPECT_EQ("Invalid member 'u': "
            "Expects value > 10, while actual value = 5", error_msg_4);
  error_msg_4.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_4, &error_msg_4));
  EXPECT_EQ("Invalid member 'u':"
            "\n* Expects value > 10, while actual value = 5"
            "\n* Expects value to be any of [-1, 1, 10, 11, 99], "
            "while actual value = 5", error_msg_4);
}

struct foo_string_t {
  REFLECT_CPP26_ANNOTATIONS(
    // Input type can be reflect_cpp26::meta_string_view
    min = rfl::meta_string_view{"pineapple"},
    options = std::to_array({
      "apple", "pineapple", "rabbit", "vegetable", "zebra"}))
  std::string std_string;

  // Input type can be std::string_view
  REFLECT_CPP26_ANNOTATE(min_exclusive = std::string_view{"000"})
  REFLECT_CPP26_ANNOTATE(excludes = {"apple", "rabbit", "vegetable", "zebra"})
  std::string_view std_string_view;

  REFLECT_CPP26_ANNOTATIONS(
    // Input type can be string literal (const char*)
    min_exclusive = "aaa",
    // Input type can be std::string
    max_exclusive = std::string(3zU, 'z'))
  const char* c_style_str;
};

TEST(AnnotationPredicates, String)
{
  auto obj_ok_1 = foo_string_t{
    .std_string = "pineapple",
    .std_string_view = "pear",
    .c_style_str = "abc"};
  EXPECT_TRUE(rfl_annots::validate_members(obj_ok_1));
  auto error_msg_ok = std::string{};
  EXPECT_TRUE(rfl_annots::validate_members_full(obj_ok_1, &error_msg_ok));
  EXPECT_EQ("", error_msg_ok);

  auto obj_1 = foo_string_t{
    .std_string = "apple",
    .std_string_view = "apple",
    .c_style_str = "aaa"};
  EXPECT_FALSE(rfl_annots::validate_members(obj_1));
  auto error_msg_1 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_1, &error_msg_1));
  EXPECT_EQ("Invalid member 'std_string': Expects value >= pineapple, "
            "while actual value = apple", error_msg_1);
  error_msg_1.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_1, &error_msg_1));
  EXPECT_EQ("Invalid member 'std_string':"
            "\n* Expects value >= pineapple, while actual value = apple"
            "\nInvalid member 'std_string_view':"
            "\n* Expects value to be none of [apple, rabbit, vegetable, zebra]"
            ", while actual value = apple"
            "\nInvalid member 'c_style_str':"
            "\n* Expects value > aaa, while actual value = aaa", error_msg_1);

  auto obj_2 = foo_string_t{
    .std_string = "yellow",
    .std_string_view = "000",
    .c_style_str = "zzzz"};
  EXPECT_FALSE(rfl_annots::validate_members(obj_2));
  auto error_msg_2 = std::string{};
  EXPECT_FALSE(rfl_annots::validate_members(obj_2, &error_msg_2));
  EXPECT_EQ("Invalid member 'std_string': Expects value to be any of "
            "[apple, pineapple, rabbit, vegetable, zebra], "
            "while actual value = yellow", error_msg_2);
  error_msg_2.clear();
  EXPECT_FALSE(rfl_annots::validate_members_full(obj_2, &error_msg_2));
  EXPECT_EQ("Invalid member 'std_string':"
            "\n* Expects value to be any of [apple, pineapple, rabbit, "
            "vegetable, zebra], while actual value = yellow"
            "\nInvalid member 'std_string_view':"
            "\n* Expects value > 000, while actual value = 000"
            "\nInvalid member 'c_style_str':"
            "\n* Expects value < zzz, while actual value = zzzz", error_msg_2);
}
