#include "./test_cases.hpp"
#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>

using namespace reflect_cpp26;
using namespace std::string_view_literals;

TEST(EnumMisc, EnumCount)
{
  EXPECT_EQ(9, enum_count<foo_signed>());
  EXPECT_EQ(9, enum_count<foo_signed_reorder>());
  EXPECT_EQ(11, enum_count<foo_signed_rep>());
  EXPECT_EQ(11, enum_count<bar_unsigned>());
  EXPECT_EQ(5, enum_count<baz_signed>());
  EXPECT_EQ(6, enum_count<qux_unsigned>());
  EXPECT_EQ(0, enum_count<empty>());
  EXPECT_EQ(1, enum_count<single>());
  EXPECT_EQ(2, enum_count<single_rep>());
  EXPECT_EQ(141, enum_count<color>());
  EXPECT_EQ(16, enum_count<terminal_color>());
  EXPECT_EQ(2, enum_count<hash_collision>());
}

TEST(EnumMisc, EnumHash)
{
  EXPECT_TRUE(enum_hash<foo_signed>() == enum_hash<foo_signed_reorder>());
  EXPECT_FALSE(enum_hash<foo_signed>() == enum_hash<foo_signed_rep>());
  EXPECT_FALSE(enum_hash<foo_signed>() == enum_hash<bar_unsigned>());

  EXPECT_FALSE(enum_hash<single>() == enum_hash<empty>());
  EXPECT_FALSE(enum_hash<single>() == enum_hash<single_rep>());
}

TEST(EnumMisc, EnumJson)
{
  auto foo_signed_str_original =
    R"({"zero":0,"one":1,"two":2,"four":4,"five":5,"six":6,"seven":7,"invalid":-1,"error":-2})"sv;
  EXPECT_EQ(foo_signed_str_original,
            enum_json<foo_signed>(enum_entry_order::original));
  EXPECT_EQ(foo_signed_str_original,
            (enum_json_static<foo_signed, enum_entry_order::original>()));

  auto foo_signed_str_by_value =
    R"({"error":-2,"invalid":-1,"zero":0,"one":1,"two":2,"four":4,"five":5,"six":6,"seven":7})"sv;
  EXPECT_EQ(foo_signed_str_by_value,
            enum_json<foo_signed>(enum_entry_order::by_value));
  EXPECT_EQ(foo_signed_str_by_value,
            (enum_json_static<foo_signed, enum_entry_order::by_value>()));

  auto foo_signed_str_by_name =
    R"({"error":-2,"five":5,"four":4,"invalid":-1,"one":1,"seven":7,"six":6,"two":2,"zero":0})"sv;
  EXPECT_EQ(foo_signed_str_by_name,
            enum_json<foo_signed>(enum_entry_order::by_name));
  EXPECT_EQ(foo_signed_str_by_name,
            (enum_json_static<foo_signed, enum_entry_order::by_name>()));

  auto qux_unsigned_str_original =
    R"({"ling":0,"yi":1,"er":2,"san":3,"si":4,"fuyi":65535})"sv;
  EXPECT_EQ(qux_unsigned_str_original,
            enum_json<qux_unsigned>(enum_entry_order::original));
  EXPECT_EQ(qux_unsigned_str_original,
            (enum_json_static<qux_unsigned, enum_entry_order::original>()));

  auto qux_unsigned_str_by_value =
    R"({"ling":0,"yi":1,"er":2,"san":3,"si":4,"fuyi":65535})"sv;
  EXPECT_EQ(qux_unsigned_str_by_value,
            enum_json<qux_unsigned>(enum_entry_order::by_value));
  EXPECT_EQ(qux_unsigned_str_by_value,
            (enum_json_static<qux_unsigned, enum_entry_order::by_value>()));

  auto qux_unsigned_str_by_name =
    R"({"er":2,"fuyi":65535,"ling":0,"san":3,"si":4,"yi":1})"sv;
  EXPECT_EQ(qux_unsigned_str_by_name,
            enum_json<qux_unsigned>(enum_entry_order::by_name));
  EXPECT_EQ(qux_unsigned_str_by_name,
            (enum_json_static<qux_unsigned, enum_entry_order::by_name>()));
}
