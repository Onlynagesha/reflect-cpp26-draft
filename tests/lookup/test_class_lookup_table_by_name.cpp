#include "tests/lookup/lookup_test_options.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/lookup.hpp>
#else
#include <reflect_cpp26/lookup/lookup_table_by_name.hpp>
#endif

#define LOOKUP_TABLE_BY_NAME(...) \
  REFLECT_CPP26_TYPE_LOOKUP_TABLE_BY_NAME(__VA_ARGS__)

struct foo_1_t {
  int value_x;
  int value_y;
  int value_z;
  int size;

  constexpr int get_x_squared_plus_a(int a) const {
    return value_x * value_x + a;
  }
  constexpr int get_y_squared_plus_a(int a) const {
    return value_y * value_y + a;
  }
  constexpr int get_z_plus_a(int a) const {
    return value_z + a;
  }
};

TEST(ClassLookupTableByName, Basic)
{
  constexpr auto table_value = LOOKUP_TABLE_BY_NAME(foo_1_t, "value_", "");
  static_assert(std::is_same_v<
    int foo_1_t::*, decltype(table_value)::value_type>);
  static_assert(table_value.size() == 3);

  constexpr auto foo = foo_1_t{1, 2, 3, 4};
  // Note: Prefix and suffix ignored during lookup.
  check_member_variable_static(1, foo, table_value["x"]);
  check_member_variable_static(2, foo, table_value["y"]);
  check_member_variable_static(3, foo, table_value["z"]);
  EXPECT_EQ_STATIC(nullptr, table_value["a"]);
  EXPECT_EQ_STATIC(nullptr, table_value["size"]);
  EXPECT_EQ_STATIC(nullptr, table_value["value_x"]);

  constexpr auto table_fn =
    LOOKUP_TABLE_BY_NAME(foo_1_t, "get_", "_squared_plus_a");
  static_assert(std::is_same_v<
    int (foo_1_t::*)(int) const, decltype(table_fn)::value_type>);
  static_assert(table_fn.size() == 2);

  check_member_function_static(11, foo, table_fn["x"], 10);
  check_member_function_static(14, foo, table_fn["y"], 10);
  EXPECT_EQ_STATIC(nullptr, table_fn["z"]);
  EXPECT_EQ_STATIC(nullptr, table_fn["get_x_squared_plus_a"]);
  EXPECT_EQ_STATIC(nullptr, table_fn["get_z_plus_a"]);
}

struct foo_2_t {
  static inline double value_a = 123.0;
  static inline double value_b = 456.0;
  double value_x;
  double value_y;
  double value_z;

  static double value_sum(double x, double y) {
    return x + y;
  }
  static double value_product(double x, double y) {
    return x * y;
  }
  constexpr double value_sum() const {
    return value_x + value_y + value_z;
  }
  constexpr double value_fma() const {
    return value_x * value_y + value_z;
  }
  constexpr double value_product() const {
    return value_x * value_y * value_z;
  }
};

TEST(ClassLookupTableByName, FilterFlags)
{
  // nv: Non-static (member) Variable
  constexpr auto table_nv = LOOKUP_TABLE_BY_NAME(foo_2_t, "nv", "value_", "");
  static_assert(std::is_same_v<
    double foo_2_t::*, decltype(table_nv)::value_type>);
  static_assert(table_nv.size() == 3);

  constexpr auto foo = foo_2_t{.value_x = 11, .value_y = 22, .value_z = 33};
  check_member_variable_static(11.0, foo, table_nv["x"]);
  check_member_variable_static(22.0, foo, table_nv["y"]);
  check_member_variable_static(33.0, foo, table_nv["z"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["a"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["sum"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["A"]);

  // nf: Non-static (member) Function
  constexpr auto table_nf = LOOKUP_TABLE_BY_NAME(foo_2_t, "nf", "value_", "");
  static_assert(std::is_same_v<
    double (foo_2_t::*)() const, decltype(table_nf)::value_type>);
  static_assert(table_nf.size() == 3);

  check_member_function_static(66.0, foo, table_nf["sum"]);
  check_member_function_static(275.0, foo, table_nf["fma"]);
  check_member_function_static(7986.0, foo, table_nf["product"]);
  EXPECT_EQ_STATIC(nullptr, table_nf["x"]);
  EXPECT_EQ_STATIC(nullptr, table_nf["a"]);
  EXPECT_EQ_STATIC(nullptr, table_nf["value_sum"]);
  EXPECT_EQ_STATIC(nullptr, table_nf["valueProduct"]);

  // sv: Static (member) variable
  constexpr auto table_sv = LOOKUP_TABLE_BY_NAME(foo_2_t, "sv", "value_", "");
  static_assert(std::is_same_v<double*, decltype(table_sv)::value_type>);
  static_assert(table_sv.size() == 2);

  check_variable(123.0, table_sv["a"]);
  check_variable(456.0, table_sv["b"]);
  EXPECT_EQ_STATIC(nullptr, table_sv["y"]);
  EXPECT_EQ_STATIC(nullptr, table_sv["add"]);
  EXPECT_EQ_STATIC(nullptr, table_sv["_a"]);

  // sf: Static (member) function
  constexpr auto table_sf = LOOKUP_TABLE_BY_NAME(foo_2_t, "sf", "value_", "");
  static_assert(std::is_same_v<
    double (*)(double, double), decltype(table_sf)::value_type>);
  static_assert(table_sf.size() == 2);

  check_function(15.0, table_sf["sum"], 6.0, 9.0);
  check_function(54.0, table_sf["product"], 6.0, 9.0);
  EXPECT_EQ_STATIC(nullptr, table_sf["fma"]);
  EXPECT_EQ_STATIC(nullptr, table_sf["b"]);
  EXPECT_EQ_STATIC(nullptr, table_sf["y"]);
  EXPECT_EQ_STATIC(nullptr, table_sf[""]);
}

struct foo_3_t {
  static constexpr int32_t a1 = 10;
  static constexpr int32_t a2 = 20;

  const int32_t a3;
  const int32_t a4;
  const int32_t a5;
  volatile int64_t a6;
};

TEST(ClassLookupTableByName, CustomFilter)
{
  constexpr auto table_sv = LOOKUP_TABLE_BY_NAME(foo_3_t,
    [](std::string_view identifier) -> std::optional<std::string> {
      if (identifier[1] <= '2') {
        return std::string{identifier};
      }
      return std::nullopt;
    });
  static_assert(std::is_same_v<const int32_t*, decltype(table_sv)::value_type>);
  static_assert(table_sv.size() == 2);

  check_variable_static(10, table_sv["a1"]);
  check_variable_static(20, table_sv["a2"]);
  EXPECT_EQ_STATIC(nullptr, table_sv[""]);
  EXPECT_EQ_STATIC(nullptr, table_sv["1"]);
  EXPECT_EQ_STATIC(nullptr, table_sv["a4"]);

  constexpr auto table_nv = LOOKUP_TABLE_BY_NAME(foo_3_t,
    [](std::meta::info member) -> std::optional<std::string> {
      if (is_static_member(member) || is_volatile_type(type_of(member))) {
        return std::nullopt;
      }
      return std::string{identifier_of(member).substr(1)};
    });
  static_assert(std::is_same_v<
    const int32_t foo_3_t::*, decltype(table_nv)::value_type>);
  static_assert(table_nv.size() == 3);

  auto foo = foo_3_t{.a3 = 1, .a4 = 3, .a5 = 6, .a6 = 10};
  check_member_variable(1, foo, table_nv["3"]);
  check_member_variable(3, foo, table_nv["4"]);
  check_member_variable(6, foo, table_nv["5"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["a3"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["2"]);
  EXPECT_EQ_STATIC(nullptr, table_nv["6"]);
}

struct bar_1_t : foo_1_t {
  int value_a;
  int value_b;
  int value_c;
  int count;

  constexpr int get_z_squared_plus_a(int a) const {
    return value_z * value_z + a;
  }
  constexpr int get_a_squared_plus_a(int a) const {
    return value_a * value_a + a;
  }
};

TEST(ClassLookupTableByName, Inheritance1)
{
  constexpr auto table_value = LOOKUP_TABLE_BY_NAME(bar_1_t, "value_", "");
  static_assert(std::is_same_v<
    int bar_1_t::*, decltype(table_value)::value_type>);
  static_assert(table_value.size() == 6);

  constexpr auto bar = bar_1_t{{1, 2, 3, 4}, 5, 6, 7};
  check_member_variable_static(1, bar, table_value["x"]);
  check_member_variable_static(2, bar, table_value["y"]);
  check_member_variable_static(3, bar, table_value["z"]);
  check_member_variable_static(5, bar, table_value["a"]);
  check_member_variable_static(6, bar, table_value["b"]);
  check_member_variable_static(7, bar, table_value["c"]);

  EXPECT_EQ_STATIC(nullptr, table_value["count"]);
  EXPECT_EQ_STATIC(nullptr, table_value["value_a"]);
  EXPECT_EQ_STATIC(nullptr, table_value["value_x"]);

  constexpr auto table_fn =
    LOOKUP_TABLE_BY_NAME(bar_1_t, "get_", "_squared_plus_a");
  static_assert(std::is_same_v<
    int (bar_1_t::*)(int) const, decltype(table_fn)::value_type>);
  static_assert(table_fn.size() == 4);

  check_member_function_static(11, bar, table_fn["x"], 10);
  check_member_function_static(14, bar, table_fn["y"], 10);
  check_member_function_static(19, bar, table_fn["z"], 10);
  check_member_function_static(35, bar, table_fn["a"], 10);

  EXPECT_EQ_STATIC(nullptr, table_fn["b"]);
  EXPECT_EQ_STATIC(nullptr, table_fn["get_a_squared_plus_a"]);
  EXPECT_EQ_STATIC(nullptr, table_fn["get_z_plus_a"]);
}

struct A : std::monostate {
  long a_count;
};
struct B {
  long b_count;
};
struct C {
  long c_count;
};
struct D : A, B, C {
  static constexpr auto make(long a, long b, long c) -> D
  {
    auto res = D{};
    res.a_count = a;
    res.b_count = b;
    res.c_count = c;
    return res;
  }
};

TEST(ClassLookupTableByName, Inheritance2)
{
  constexpr auto table_value = LOOKUP_TABLE_BY_NAME(D, "", "_count");
  static_assert(std::is_same_v<long D::*, decltype(table_value)::value_type>);
  static_assert(table_value.size() == 3);

  constexpr auto d = D::make(1, 3, 5);
  check_member_variable_static(1, d, table_value["a"]);
  check_member_variable_static(3, d, table_value["b"]);
  check_member_variable_static(5, d, table_value["c"]);
  EXPECT_EQ_STATIC(nullptr, table_value["a_count"]);
  EXPECT_EQ_STATIC(nullptr, table_value["d"]);
  EXPECT_EQ_STATIC(nullptr, table_value[""]);
}
