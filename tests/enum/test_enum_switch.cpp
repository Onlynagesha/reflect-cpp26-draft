#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

using namespace reflect_cpp26;
using namespace std::literals;

template <class E, unsigned p>
struct underlying_pow_t {
  static constexpr auto operator()(E value)
  {
    auto x = std::to_underlying(value);
    auto res = static_cast<decltype(x)>(1);
    for (auto i = p; i > 0; i--) {
      res *= x;
    }
    return res;
  }
};

constexpr auto pos_res_1 = enum_switch<int>(
  underlying_pow_t<foo_signed, 2>{}, foo_signed::four);
static_assert(*pos_res_1 == 16);

constexpr auto pos_res_2 = enum_switch<uint32_t>(
  underlying_pow_t<foo_signed, 3>{}, foo_signed::error);
static_assert(*pos_res_2 == 4294967288);

constexpr auto pos_res_3 = enum_switch<int>(
  underlying_pow_t<bar_unsigned, 5>{}, bar_unsigned::error);
static_assert(*pos_res_3 == -32);

constexpr auto pos_res_4 = enum_switch<unsigned>(
  underlying_pow_t<bar_unsigned, 5>{}, static_cast<bar_unsigned>(1234));
static_assert(pos_res_4 == std::nullopt);

constexpr auto constant_tuple = std::tuple{
  short{12},
  "const char*",
  int{34},
  "std::string_view"sv,
  long{56},
};

struct get_from_constant_tuple_t {
  static constexpr auto operator()(auto ec) {
    constexpr auto I = std::to_underlying(ec.value);
    return get<I>(constant_tuple);
  }
};

enum class string_ref_indices {
  const_char_ptr = 1,
  std_string_view = 3,
};

constexpr auto get_res_1 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  string_ref_indices::const_char_ptr,
  "abc");
// common_type_t<const char*, std::string_view, const char[4]>
static_assert(std::is_same_v<decltype(get_res_1), const std::string_view>);
static_assert(get_res_1 == "const char*");

constexpr auto get_res_2 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  string_ref_indices::std_string_view,
  "def"s);
// common_type_t<const char*, std::string_view, std::string>
static_assert(std::is_same_v<decltype(get_res_2), const std::string_view>);
static_assert(get_res_2 == "std::string_view");

constexpr auto get_res_3 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  static_cast<string_ref_indices>(1234),
  "ghi");
// common_type_t<const char*, std::string_view, const char[4]>
static_assert(std::is_same_v<decltype(get_res_3), const std::string_view>);
static_assert(get_res_3 == "ghi");

enum class integer_indices {
  index_short = 0,
  index_int = 2,
  index_long = 4,
};

constexpr auto get_res_4 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  integer_indices::index_short,
  1234u);
// common_type_t<short, int, long, unsigned>
static_assert(std::is_same_v<decltype(get_res_4), const long>);
static_assert(get_res_4 == 12);

constexpr auto get_res_5 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  integer_indices::index_int,
  9.75f);
// common_type_t<short, int, long, float>
static_assert(std::is_same_v<decltype(get_res_5), const float>);
static_assert(get_res_5 == 34);

constexpr auto get_res_6 = enum_switch_to_common(
  get_from_constant_tuple_t{},
  static_cast<integer_indices>(-1),
  constant<1234>{});
// common_type_t<short, int, long, constant<int>>
static_assert(std::is_same_v<decltype(get_res_6), const long>);
static_assert(get_res_6 == 1234);

constexpr auto get_res_7 = enum_switch(
  get_from_constant_tuple_t{},
  integer_indices::index_long,
  '\0');
static_assert(std::is_same_v<decltype(get_res_7), const char>);
static_assert(get_res_7 == '\x38');

constexpr auto get_res_8 = enum_switch(
  get_from_constant_tuple_t{},
  static_cast<integer_indices>(-1),
  L'\0');
static_assert(std::is_same_v<decltype(get_res_8), const wchar_t>);
static_assert(get_res_8 == '\0');

struct get_from_non_constant_tuple_t {
  static constexpr auto& operator()(auto* tuple, auto ec) {
    constexpr auto I = std::to_underlying(ec.value);
    return get<I>(*tuple);
  }
};

enum class array_index {
  zero = 0,
  one = 1,
  two = 2,
  three = 3,
};

TEST(EnumSwitch, Dereference) {
  auto array = std::array{"abc"s, "def"s, "ghi"s, "jkl"s};
  auto default_str = "<n/a>"s;

  get_from_non_constant_tuple_t{}(
    &array, constant<array_index::zero>{}) += "<test>";
  EXPECT_EQ("abc<test>", array[0]) << "Implementation error with test case.";

  auto&& str1 = enum_switch(
    std::bind_front(get_from_non_constant_tuple_t{}, &array),
    array_index::two, default_str);
  str1 += "<changed>";
  EXPECT_EQ("ghi<changed>", str1);
  EXPECT_EQ("ghi", array[2]);

  auto&& str2 = enum_switch(
    std::bind_front(get_from_non_constant_tuple_t{}, &array),
    static_cast<array_index>(4), default_str);
  str2 += "<changed>";
  EXPECT_EQ("<n/a><changed>", str2);
  EXPECT_EQ("<n/a>", default_str);
}
