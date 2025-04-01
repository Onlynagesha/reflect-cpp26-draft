#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/utils/constant.hpp>
#include <format>

namespace rfl = reflect_cpp26;

struct point_t {
  int x;
  int y;
};

template <class T, size_t N>
constexpr auto sum(const std::array<T, N>& values) -> T {
  auto res = T{};
  for (auto x: values) {
    res += x;
  }
  return res;
}

// Basic usage
auto c1 = rfl::constant<12>{};
static_assert(12 == c1.value);
static_assert(12 == sum(c1.values));
static_assert(12 == c1); // Implicit conversion

// reduce
auto c2 = rfl::constant<1u, 2u, 3u, 5u, 8u>{};
static_assert(19u == sum(c2.values));
// (1) Implicit conversion
constexpr auto c2_reducer_1 = [](unsigned dest, unsigned x) {
  return dest * 10 + x;
};
constexpr auto c2_reduced_1 = c2.reduce<c2_reducer_1, 0u>();
static_assert(std::is_same_v<decltype(c2_reduced_1), const unsigned>);
static_assert(12358u == c2_reduced_1);

// (2) Static initial value
constexpr auto c2_reducer_2 = [](auto dest, auto x) {
  static_assert(rfl::is_nontype_template_instance_of_v<
    decltype(dest), rfl::constant>);
  static_assert(rfl::is_nontype_template_instance_of_v<
    decltype(x), rfl::constant>);
  static_assert(std::is_same_v<decltype(dest.value), const unsigned>);
  static_assert(std::is_same_v<decltype(x.value), const unsigned>);
  return dest * 100 + x;
};
constexpr auto c2_reduced_2 = c2.reduce<c2_reducer_2, 0u>();
static_assert(std::is_same_v<decltype(c2_reduced_2), const unsigned>);
static_assert(102030508u == c2_reduced_2);

// (3) Dynamic initial value
constexpr auto c2_reducer_3 = [](auto dest, auto x) {
  static_assert(rfl::is_nontype_template_instance_of_v<
    decltype(x), rfl::constant>);
  static_assert(std::is_same_v<decltype(x.value), const unsigned>);
  static_assert(std::is_same_v<decltype(dest), unsigned>);
  return dest * 10 + (10 - x);
};
static_assert(998752u == c2.reduce(c2_reducer_3, 9u));

// for_each
TEST(UtilsConstant, ForEach)
{
  auto str = std::string{};
  // (1) func(value)
  c2.for_each([&str](unsigned x) {
    str += std::to_string(x);
    str += ",";
  });
  EXPECT_EQ("1,2,3,5,8,", str);
  str.clear();
  // (2) func(index, value)
  c2.for_each([&str](size_t i, unsigned x) {
    str += std::format("[{}]={},", i, x);
  });
  EXPECT_EQ("[0]=1,[1]=2,[2]=3,[3]=5,[4]=8,", str);
  str.clear();
  // (3) func(index, value) with stop token
  c2.for_each([&str](size_t i, unsigned x) {
    str += std::format("[{}]={},", i, x);
    return x <= 2;
  });
  EXPECT_EQ("[0]=1,[1]=2,[2]=3,", str);
}

// map
// (1) Implicit conversion
constexpr auto c2_mapper_1 = [](unsigned x) {
  return x / 8.0;
};
auto c2_mapped_1 = c2.map<c2_mapper_1>();
static_assert(2.375 == sum(c2_mapped_1.values));
static_assert(2.375 == sum(c2.map(c2_mapper_1)));

// (2) No implicit conversion
constexpr auto c2_mapper_2 = [](auto x) {
  static_assert(rfl::is_nontype_template_instance_of_v<
    decltype(x), rfl::constant>);
  static_assert(std::is_same_v<decltype(x.value), const unsigned>);
  return x / 4.0;
};
auto c2_mapped_2 = c2.map<c2_mapper_2>();
static_assert(4.75 == sum(c2_mapped_2.values));
static_assert(4.75 == sum(c2.map(c2_mapper_2)));

// concat
auto c2_concat = c2.concat(c2);
static_assert(10 == c2_concat.size());
static_assert(1235812358u == c2_concat.reduce(c2_reducer_1, 0u));

// filter
TEST(UtilsConstant, Filter)
{
  // (1) Static predicate with implicit conversion
  constexpr auto c2_filter_1 = [](unsigned x) {
    return x % 2 == 1;
  };
  constexpr auto c2_concat_filtered_1 = c2_concat.filter<c2_filter_1>();
  static_assert(135135u == c2_concat_filtered_1.reduce<c2_reducer_1, 0u>());

  // (2) Static predicate without implicit conversion
  constexpr auto c2_filter_2 = [](auto x) {
    static_assert(rfl::is_nontype_template_instance_of_v<
      decltype(x), rfl::constant>);
    static_assert(std::is_same_v<decltype(x.value), const unsigned>);
    return x % 2 == 0;
  };
  constexpr auto c2_concat_filtered_2 = c2_concat.filter<c2_filter_2>();
  static_assert(2828u == c2_concat_filtered_2.reduce<c2_reducer_1, 0u>());

  // (3) Dynamic predicate without implicit conversion
  auto index = 0u;
  //    1, 2, 3, 5,  8, 1, 2,  3,  5,  8
  // => 1, 3, 5, 8, 12, 6, 8, 10, 13, 17
  EXPECT_THAT(c2_concat.filter([&index](auto x) {
    static_assert(rfl::is_nontype_template_instance_of_v<
      decltype(x), rfl::constant>);
    static_assert(std::is_same_v<decltype(x.value), const unsigned>);
    return ((index++) + x) % 3 != 0;
  }), testing::ElementsAre(1, 3, 5, 2, 3, 5, 8));
}

// index_of, last_index_of, includes
static_assert(1 == c2_concat.index_of(2));
static_assert(8 == c2_concat.last_index_of(5));
static_assert(c2_concat.includes(8));
static_assert(rfl::npos == c2_concat.index_of(4));
static_assert(rfl::npos == c2_concat.last_index_of(6));
static_assert(!c2_concat.includes(9));
// Non-comparable types
static_assert(rfl::npos == c2_concat.index_of(point_t{.x = 1, .y = 2}));
static_assert(rfl::npos == c2_concat.last_index_of(std::monostate{}));

auto tags = rfl::tags_constant<
  std::monostate,
  std::remove_cvref<int&>,
  std::remove_cvref<const int&>>();
// Tags matching
static_assert(2 == tags.index_of(std::remove_cvref<int const&>{}));
static_assert(1 == tags.last_index_of(std::remove_cvref<int&>{}));
static_assert(rfl::npos == tags.index_of(std::remove_cvref<int>{}));
static_assert(rfl::npos == tags.last_index_of(std::remove_cvref<int&&>{}));

// Implicit conversion
auto tags_misuse = rfl::tags_constant<
  std::is_integral<int>,    // -> true
  std::is_integral<float>,  // -> false
  std::is_integral<long>,   // -> true
  std::monostate>();        // Not convertible
static_assert(0 == tags_misuse.index_of(std::is_integral<long>{}));
static_assert(1 == tags_misuse.last_index_of(std::is_integral<double>{}));
