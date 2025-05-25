#include "tests/enum/enum_test_options.hpp"
#include "tests/enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_json.hpp>
#endif

using namespace reflect_cpp26;

constexpr auto foo_signed_str_original =
  R"__({"zero":0,"one":1,"two":2,"four":4,"five":5,"six":6,)__"
  R"__("seven":7,"invalid":-1,"error":-2})__";
static_assert(foo_signed_str_original
  == enum_json<foo_signed, enum_entry_order::original>());
static_assert(foo_signed_str_original
  == enum_json_static<foo_signed, enum_entry_order::original>());

constexpr auto foo_signed_str_by_value =
  R"__({"error":-2,"invalid":-1,"zero":0,"one":1,"two":2,"four":4,)__"
  R"__("five":5,"six":6,"seven":7})__";
static_assert(foo_signed_str_by_value
  == enum_json<foo_signed, enum_entry_order::by_value>());
static_assert(foo_signed_str_by_value
  == enum_json_static<foo_signed, enum_entry_order::by_value>());

constexpr auto foo_signed_str_by_name =
  R"__({"error":-2,"five":5,"four":4,"invalid":-1,"one":1,"seven":7,)__"
  R"__("six":6,"two":2,"zero":0})__";
static_assert(foo_signed_str_by_name
  == enum_json<foo_signed, enum_entry_order::by_name>());
static_assert(foo_signed_str_by_name
  == enum_json_static<foo_signed, enum_entry_order::by_name>());

constexpr auto qux_unsigned_str_original =
  R"({"ling":0,"yi":1,"er":2,"san":3,"si":4,"fuyi":65535})";
static_assert(qux_unsigned_str_original
  == enum_json<qux_unsigned, enum_entry_order::original>());
static_assert(qux_unsigned_str_original
  == enum_json_static<qux_unsigned, enum_entry_order::original>());

constexpr auto qux_unsigned_str_by_value =
  R"({"ling":0,"yi":1,"er":2,"san":3,"si":4,"fuyi":65535})";
static_assert(qux_unsigned_str_by_value
  == enum_json<qux_unsigned, enum_entry_order::by_value>());
static_assert(qux_unsigned_str_by_value
  == enum_json_static<qux_unsigned, enum_entry_order::by_value>());

constexpr auto qux_unsigned_str_by_name =
  R"({"er":2,"fuyi":65535,"ling":0,"san":3,"si":4,"yi":1})";
static_assert(qux_unsigned_str_by_name
  == enum_json<qux_unsigned, enum_entry_order::by_name>());
static_assert(qux_unsigned_str_by_name
  == enum_json_static<qux_unsigned, enum_entry_order::by_name>());

static_assert("{}" == enum_json<empty, enum_entry_order::original>());
static_assert("{}" == enum_json_static<empty, enum_entry_order::by_name>());

static_assert(R"({"value":233})"
  == enum_json<single, enum_entry_order::original>());
static_assert(R"({"value":233})"
  == enum_json_static<single, enum_entry_order::by_value>());

TEST(EnumJson, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
