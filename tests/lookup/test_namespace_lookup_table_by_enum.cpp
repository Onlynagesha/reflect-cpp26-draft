#include "tests/lookup/lookup_test_options.hpp"
#include <bit>

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/lookup.hpp>
#else
#include <reflect_cpp26/lookup/lookup_table_by_enum.hpp>
#endif

namespace rfl = reflect_cpp26;

#define LOOKUP_TABLE_BY_ENUM(...) \
  REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_ENUM(__VA_ARGS__)
#define LOOKUP_TABLE_BY_ENUM_ALL(...) \
  REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_ENUM_ALL(__VA_ARGS__)

enum class foo_get_fn_key {
  abs,
  even,
  non_zero,
  odd,
};

enum class foo_value_key {
  x,
  z,
};

namespace foo {
namespace bar {
namespace get_options {
constexpr int get_negative(int value) {
  return value >= 0 ? -1 : value;
}
volatile double value_w = -1.0;
} // namespace get_options
class get_dummy {};

constexpr int get_odd(int value) {
  return value % 2 == 0 ? value + 1 : value;
}
constexpr int get_even(int value) {
  return value % 2 == 0 ? value : value + 1;
}
constexpr int get_abs(int value) {
  return value >= 0 ? value : -value;
}
volatile double value_x = 1.0;
volatile double value_y = 2.0;
volatile double value_z = 3.0;
} // namespace bar
constexpr int get_non_zero(int value) {
  return value == 0 ? 1 : value;
}
volatile double value_a = 4.0;
} // namespace foo

TEST(NamespaceLookupTableByEnum, Basic)
{
  constexpr auto table_f = LOOKUP_TABLE_BY_ENUM(
    foo::bar, foo_get_fn_key, "get_", "");
  static_assert(std::is_same_v<int (*)(int), decltype(table_f)::value_type>);
  static_assert(table_f.size() == 3);

  check_function_static(17, table_f[foo_get_fn_key::odd], 16);
  check_function_static(18, table_f[foo_get_fn_key::even], 17);
  check_function_static(19, table_f[foo_get_fn_key::abs], -19);
  EXPECT_EQ_STATIC(nullptr, table_f[foo_get_fn_key::non_zero]);
  EXPECT_EQ_STATIC(nullptr, table_f[static_cast<foo_get_fn_key>(123)]);

  constexpr auto table_v = LOOKUP_TABLE_BY_ENUM(
    foo::bar, foo_value_key, "value_", "");
  static_assert(std::is_same_v<
    volatile double*, decltype(table_v)::value_type>);
  static_assert(table_v.size() == 2);

  check_variable(1.0, table_v[foo_value_key::x]);
  check_variable(3.0, table_v[foo_value_key::z]);
  EXPECT_EQ_STATIC(nullptr, table_v[static_cast<foo_value_key>(-123)]);
}

enum class baz_get_fn_key {
  odd,
  even,
  abs,
  bit_one_count,
};

enum class baz_value_key {
  x,
  y,
};

namespace baz {
namespace get_options_or_count {
constexpr int get_negative(int value) {
  return value >= 0 ? -1 : value;
}
volatile double value_w = -1.0;
} // namespace get_options_or_count
class get_dummy_count {};

constexpr int get_odd(int value) {
  return value % 2 == 0 ? value + 1 : value;
}
constexpr int get_even(int value) {
  return value % 2 == 0 ? value : value + 1;
}
constexpr int get_abs(int value) {
  return value >= 0 ? value : -value;
}
constexpr int get_bit_one_count(int value) {
  return std::popcount(static_cast<unsigned>(value));
}
double get_x_count = 1.0;
double get_y_count = 2.0;
} // namespace baz

TEST(NamespaceLookupTableByName, FilterFlags)
{
  constexpr auto table_f = LOOKUP_TABLE_BY_ENUM_ALL(
    baz, baz_get_fn_key, 'f', "get_", "");
  static_assert(std::is_same_v<int (*)(int), decltype(table_f)::value_type>);
  static_assert(table_f.size() == 4);

  check_function_static(17, table_f[baz_get_fn_key::odd], 16);
  check_function_static(18, table_f[baz_get_fn_key::even], 17);
  check_function_static(19, table_f[baz_get_fn_key::abs], -19);
  check_function_static(6, table_f[baz_get_fn_key::bit_one_count], 0b11101101);
  EXPECT_EQ_STATIC(nullptr, table_f[static_cast<baz_get_fn_key>(123)]);

  constexpr auto table_v = LOOKUP_TABLE_BY_ENUM_ALL(
    baz, baz_value_key, 'v', "get_", "_count");
  static_assert(std::is_same_v<double*, decltype(table_v)::value_type>);
  static_assert(table_v.size() == 2);

  check_variable(1.0, table_v[baz_value_key::x]);
  check_variable(2.0, table_v[baz_value_key::y]);
  EXPECT_EQ_STATIC(nullptr, table_v[static_cast<baz_value_key>(-123)]);
}

namespace ops {
enum class Opcode {
  ADD,
  SUB,
  BITWISE_AND,
  BITWISE_OR,
};

constexpr unsigned HandleAdd(unsigned x, unsigned y) {
  return x + y;
}
constexpr unsigned HandleSub(unsigned x, unsigned y) {
  return x - y;
}
constexpr unsigned HandleBitwiseAnd(unsigned x, unsigned y) {
  return x & y;
}
constexpr unsigned HandleBitwiseOr(unsigned x, unsigned y) {
  return x | y;
}
constexpr bool Handle(unsigned* dest, unsigned x, unsigned y, Opcode opcode)
{
  constexpr auto opcode_naming_rule =
    rfl::identifier_naming_rule::all_caps_snake_case;
  constexpr auto dispatch_table = LOOKUP_TABLE_BY_ENUM_ALL(
    ops, Opcode, "Handle", "", opcode_naming_rule);

  static_assert(std::is_same_v<
    unsigned (*)(unsigned, unsigned), decltype(dispatch_table)::value_type>);
  static_assert(dispatch_table.size() == 4);

  auto handle_fn = dispatch_table[opcode];
  if (dest == nullptr || handle_fn == nullptr) {
    return false;
  }
  *dest = handle_fn(x, y);
  return true;
}
} // namespace ops

TEST(NamespaceLookupTableByName, NameConversion)
{
  auto dest = 0u;
  EXPECT_TRUE(ops::Handle(&dest, 15, 50, ops::Opcode::ADD));
  EXPECT_EQ(65u, dest);
  dest = 0u;
  EXPECT_TRUE(ops::Handle(&dest, 15, 50, ops::Opcode::SUB));
  EXPECT_EQ(-35u, dest);
  dest = 0u;
  EXPECT_TRUE(ops::Handle(&dest, 0b1010u, 0b1100u, ops::Opcode::BITWISE_AND));
  EXPECT_EQ(0b1000u, dest);
  dest = 0u;
  EXPECT_TRUE(ops::Handle(&dest, 0b1010u, 0b1100u, ops::Opcode::BITWISE_OR));
  EXPECT_EQ(0b1110u, dest);

  EXPECT_FALSE_STATIC(ops::Handle(&dest, 1, 2, static_cast<ops::Opcode>(123)));
}
