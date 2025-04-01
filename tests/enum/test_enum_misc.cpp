#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

using namespace reflect_cpp26;

template <auto B>
struct some_type_t {
  static constexpr auto value =
    std::to_underlying(B) * std::to_underlying(B) + 2000;
};

// enum_count
static_assert(9 == enum_count<foo_signed>());
static_assert(9 == enum_count<const foo_signed_reorder>());
static_assert(11 == enum_count<volatile foo_signed_rep>());
static_assert(11 == enum_count<const volatile bar_unsigned>());
static_assert(5 == enum_count<baz_signed>());
static_assert(6 == enum_count<qux_unsigned>());
static_assert(0 == enum_count<empty>());
static_assert(1 == enum_count<const single>());
static_assert(6 == enum_count<volatile single_rep>());
static_assert(141 == enum_count<const volatile color>());
static_assert(16 == enum_count<terminal_color>());
static_assert(2 == enum_count<hash_collision>());

// enum_entries
static_assert(enum_entry<foo_signed_rep>(3)
  == std::pair{foo_signed_rep::yi, "yi"});
static_assert(enum_entry<const volatile foo_signed_rep>(1)
  == std::pair{foo_signed_rep::one, "one"});
static_assert(enum_entry<const foo_signed_rep, enum_entry_order::by_value>(7)
  == std::pair{foo_signed_rep::four, "four"});
static_assert(enum_entry<volatile foo_signed_rep, enum_entry_order::by_name>(9)
  == std::pair{foo_signed_rep::yi, "yi"});

// enum_for_each
constexpr auto some_type_dec_baz_signed_1 = []() {
  auto dec = 0;
  enum_for_each<baz_signed>([&dec](auto v) {
    dec = dec * 10 + std::to_underlying(v.value);
  });
  return dec;
}();
static_assert(some_type_dec_baz_signed_1 == 1229);

constexpr auto some_type_dec_baz_signed_2 = []() {
  auto dec = 0;
  enum_for_each<baz_signed, enum_entry_order::by_name>([&dec](auto v) {
    dec = dec * 10 + std::to_underlying(v.value);
  });
  return dec;
}();
static_assert(some_type_dec_baz_signed_2 == 19031);

constexpr auto some_type_sum_dec_baz_signed = []() {
  auto sum = 0;
  auto dec = 0;
  enum_for_each<baz_signed, enum_entry_order::by_value>([&sum, &dec](auto v) {
    sum += some_type_t<v.value>::value;
    dec = dec * 10 + std::to_underlying(v.value);
  });
  return std::pair{sum, dec};
}();
static_assert(some_type_sum_dec_baz_signed.first == 10015);
static_assert(some_type_sum_dec_baz_signed.second == -9877);

// enum_hash
static_assert(enum_hash<foo_signed>() == enum_hash<foo_signed_reorder>());
static_assert(enum_hash<const foo_signed>()
  == enum_hash<volatile foo_signed_reorder>());
static_assert(enum_hash<foo_signed>() != enum_hash<foo_signed_rep>());
static_assert(enum_hash<foo_signed>() != enum_hash<bar_unsigned>());
static_assert(enum_hash<single>() != enum_hash<empty>());
static_assert(enum_hash<single>() != enum_hash<single_rep>());

// enum_meta_entries
static_assert(std::meta::identifier_of(
  enum_meta_entry<foo_signed_rep>(3)) == "yi");
static_assert(std::meta::identifier_of(
  enum_meta_entry<const volatile foo_signed_rep>(1)) == "one");
static_assert(std::meta::identifier_of(
  enum_meta_entry<const foo_signed_rep, enum_entry_order::by_value>(7))
    == "four");
static_assert(std::meta::identifier_of(
  enum_meta_entry<volatile foo_signed_rep, enum_entry_order::by_name>(9))
    == "yi");

static_assert(std::meta::identifier_of(
  enum_meta_entry<bar_unsigned>(3)) == "three");
static_assert(std::meta::identifier_of(
  enum_meta_entry<const volatile bar_unsigned>(1)) == "one");
static_assert(std::meta::identifier_of(
  enum_meta_entry<const bar_unsigned, enum_entry_order::by_value>(7))
    == "thirteen");
static_assert(std::meta::identifier_of(
  enum_meta_entry<volatile bar_unsigned, enum_entry_order::by_name>(9))
    == "two");

using some_type_twelve = some_type_t<
  [:enum_meta_entry<bar_unsigned, enum_entry_order::by_name>(2):]>;
static_assert(some_type_twelve::value == 2196);

// enum_names
static_assert(enum_names<foo_signed_rep>()[3] == "yi");
static_assert(
  enum_names<const foo_signed_rep, enum_entry_order::by_value>()[7] == "four");
static_assert(
  enum_names<volatile foo_signed_rep, enum_entry_order::by_name>()[9] == "yi");

// enum_values
static_assert(enum_value<foo_signed_rep>(3) == foo_signed_rep::yi);
static_assert(enum_value<const foo_signed_rep, enum_entry_order::by_value>(7)
  == foo_signed_rep::four);
static_assert(enum_value<volatile foo_signed_rep, enum_entry_order::by_name>(9)
  == foo_signed_rep::yi);

// enum_value_constants
static_assert(std::is_same_v<
  decltype(enum_value_constants<baz_signed>()),
  constant<baz_signed::ling, baz_signed::yi, baz_signed::er,
    baz_signed::san, baz_signed::fuyi>>);
static_assert(std::is_same_v<
  decltype(enum_value_constants<baz_signed, enum_entry_order::by_value>()),
  constant<baz_signed::fuyi, baz_signed::ling, baz_signed::yi,
    baz_signed::er, baz_signed::san>>);
static_assert(std::is_same_v<
  decltype(enum_value_constants<baz_signed, enum_entry_order::by_name>()),
  constant<baz_signed::er, baz_signed::fuyi, baz_signed::ling,
    baz_signed::san, baz_signed::yi>>);

static_assert(std::is_same_v<
  decltype(enum_value_constant<2, baz_signed>()), constant<baz_signed::er>>);
static_assert(std::is_same_v<
  decltype(enum_value_constant<2, baz_signed, enum_entry_order::by_value>()),
  constant<baz_signed::yi>>);
static_assert(std::is_same_v<
  decltype(enum_value_constant<2, baz_signed, enum_entry_order::by_name>()),
  constant<baz_signed::ling>>);

TEST(EnumMisc, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
