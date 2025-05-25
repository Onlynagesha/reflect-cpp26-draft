#include "tests/lookup/lookup_test_options.hpp"
#include <bit>

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/lookup.hpp>
#else
#include <reflect_cpp26/lookup/lookup_table_by_name.hpp>
#endif

namespace rfl = reflect_cpp26;

#define LOOKUP_TABLE_BY_NAME(...) \
  REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_NAME(__VA_ARGS__)

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
} // namespace bar
constexpr int get_non_zero(int value) {
  return value == 0 ? 1 : value;
}
volatile double value_z = 3.0;
} // namespace foo

TEST(NamespaceLookupTableByName, Basic)
{
  constexpr auto table_f = LOOKUP_TABLE_BY_NAME(foo::bar, "get_", "");
  static_assert(std::is_same_v<int (*)(int), decltype(table_f)::value_type>);
  static_assert(table_f.size() == 3);

  check_function_static(17, table_f["odd"], 16);
  check_function_static(18, table_f["even"], 17);
  check_function_static(19, table_f["abs"], -19);
  EXPECT_EQ_STATIC(nullptr, table_f[""]);
  EXPECT_EQ_STATIC(nullptr, table_f["options"]);
  EXPECT_EQ_STATIC(nullptr, table_f["dummy"]);
  EXPECT_EQ_STATIC(nullptr, table_f["negative"]);
  EXPECT_EQ_STATIC(nullptr, table_f["non_zero"]);

  constexpr auto table_v = LOOKUP_TABLE_BY_NAME(foo::bar, "value_", "");
  static_assert(std::is_same_v<
    volatile double*, decltype(table_v)::value_type>);
  static_assert(table_v.size() == 2);

  check_variable(1.0, table_v["x"]);
  check_variable(2.0, table_v["y"]);
  EXPECT_EQ_STATIC(nullptr, table_v[""]);
  EXPECT_EQ_STATIC(nullptr, table_v["w"]);
  EXPECT_EQ_STATIC(nullptr, table_v["z"]);
  EXPECT_EQ_STATIC(nullptr, table_v["options"]);
  EXPECT_EQ_STATIC(nullptr, table_v["dummy"]);
}

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
  constexpr auto table_f = LOOKUP_TABLE_BY_NAME(baz, 'f', "get_", "");
  static_assert(std::is_same_v<int (*)(int), decltype(table_f)::value_type>);
  static_assert(table_f.size() == 4);

  check_function_static(17, table_f["odd"], 16);
  check_function_static(18, table_f["even"], 17);
  check_function_static(19, table_f["abs"], -19);
  check_function_static(6, table_f["bit_one_count"], 0b11101101);
  EXPECT_EQ_STATIC(nullptr, table_f[""]);
  EXPECT_EQ_STATIC(nullptr, table_f["options_or"]);
  EXPECT_EQ_STATIC(nullptr, table_f["options_or_count"]);
  EXPECT_EQ_STATIC(nullptr, table_f["dummy"]);
  EXPECT_EQ_STATIC(nullptr, table_f["dummy_count"]);
  EXPECT_EQ_STATIC(nullptr, table_f["bit_one"]);

  constexpr auto table_v = LOOKUP_TABLE_BY_NAME(baz, 'v', "", "_count");
  static_assert(std::is_same_v<double*, decltype(table_v)::value_type>);
  static_assert(table_v.size() == 2);

  check_variable(1.0, table_v["get_x"]);
  check_variable(2.0, table_v["get_y"]);
  EXPECT_EQ_STATIC(nullptr, table_v[""]);
  EXPECT_EQ_STATIC(nullptr, table_v["x"]);
  EXPECT_EQ_STATIC(nullptr, table_v["y"]);
  EXPECT_EQ_STATIC(nullptr, table_v["get_options_or"]);
  EXPECT_EQ_STATIC(nullptr, table_v["get_options_or_count"]);
  EXPECT_EQ_STATIC(nullptr, table_v["get_dummy"]);
  EXPECT_EQ_STATIC(nullptr, table_v["get_dummy_count"]);
}

namespace qux {
#define ANNOTATE(...) [[ =(__VA_ARGS__) ]]
constexpr long a = 1;
constexpr long b = 2;
constexpr long c = 3;
constexpr long d = 4;
ANNOTATE('?') constexpr long e = 5;

ANNOTATE('+')
constexpr long sum(long x, const long* y) {
  return y == nullptr ? x : x + *y;
}

ANNOTATE('-')
constexpr long subtract(long x, const long* y = nullptr) {
  return y == nullptr ? x : x - *y;
}
} // namespace qux

TEST(NamespaceLookupTableByName, CustomFilter)
{
  constexpr auto table_v = LOOKUP_TABLE_BY_NAME(qux,
    [](std::string_view identifier) -> std::optional<std::string> {
      if (identifier.length() == 1 && (identifier[0] - 'a') % 2 == 0) {
        return std::string{identifier};
      }
      return std::nullopt;
    });
  static_assert(std::is_same_v<const long*, decltype(table_v)::value_type>);
  static_assert(table_v.size() == 3);

  check_variable_static(1, table_v["a"]);
  check_variable_static(3, table_v["c"]);
  check_variable_static(5, table_v["e"]);
  EXPECT_EQ_STATIC(nullptr, table_v["b"]);
  EXPECT_EQ_STATIC(nullptr, table_v["d"]);

  constexpr auto table_f = LOOKUP_TABLE_BY_NAME(qux,
    [](std::meta::info member) -> std::optional<std::string> {
      if (is_variable(member) || annotations_of(member).size() != 1) {
        return std::nullopt;
      }
      auto annotation = extract<char>(annotations_of(member)[0]);
      return std::string(1zU, annotation);
    });
  static_assert(std::is_same_v<
    long (*)(long, const long*), decltype(table_f)::value_type>);
  static_assert(table_f.size() == 2);

  constexpr auto y = 42L;
  check_function_static(45, table_f["+"], 3, &y);
  check_function_static(-39, table_f["-"], 3, &y);
  EXPECT_EQ_STATIC(nullptr, table_f["?"]);
  EXPECT_EQ_STATIC(nullptr, table_f["sum"]);
  EXPECT_EQ_STATIC(nullptr, table_f["subtract"]);
}
