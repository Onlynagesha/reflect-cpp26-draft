#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/utils/constant.hpp>
#include <reflect_cpp26/utils/ctype.h>
#include <cctype>
#include <format>
#include <numeric>

namespace rfl = reflect_cpp26;

struct point_t {
  int x;
  int y;
};

template <class T, size_t N>
auto sum(const std::array<T, N>& values) -> T {
  return std::reduce(values.begin(), values.end());
}

TEST(UtilsMisc, Constant)
{
  auto c1 = rfl::constant<12>{};
  EXPECT_EQ(12, c1.value);
  EXPECT_EQ(12, sum(c1.values));
  EXPECT_EQ(12, c1);

  auto c2 = rfl::constant<1u, 2u, 3u, 5u, 8u>{};
  EXPECT_EQ(19u, sum(c2.values));
  constexpr auto c2_reducer_1 = [](unsigned dest, unsigned x) {
    return dest * 10 + x;
  };
  auto c2_reduced = c2.reduce<c2_reducer_1, 0u>();
  EXPECT_EQ(12358u, c2_reduced.value);
  EXPECT_EQ(912358u, c2.reduce(c2_reducer_1, 9u));

  auto str = std::string{};
  c2.for_each([&str](unsigned x) {
    str += std::to_string(x);
    str += ",";
  });
  EXPECT_EQ("1,2,3,5,8,", str);
  str.clear();
  c2.for_each([&str](size_t i, unsigned x) {
    str += std::format("[{}]={},", i, x);
  });
  EXPECT_EQ("[0]=1,[1]=2,[2]=3,[3]=5,[4]=8,", str);
  str.clear();
  c2.for_each([&str](size_t i, unsigned x) {
    str += std::format("[{}]={},", i, x);
    return x <= 2;
  });
  EXPECT_EQ("[0]=1,[1]=2,[2]=3,", str);

  constexpr auto c2_mapper_1 = [](unsigned x) {
    return x / 8.0;
  };
  auto c2_mapped = c2.map<c2_mapper_1>();
  EXPECT_EQ(2.375, sum(c2_mapped.values));
  EXPECT_EQ(2.375, sum(c2.map(c2_mapper_1)));

  auto c2_concat = c2.concat(c2);
  EXPECT_EQ(10, c2_concat.size());
  EXPECT_EQ(1235812358u, c2_concat.reduce(c2_reducer_1, 0u));

  constexpr auto c2_filter_1 = [](unsigned x) {
    return x % 2 == 1;
  };
  constexpr auto c2_concat_filtered = c2_concat.filter<c2_filter_1>();
  EXPECT_EQ(135135u, (c2_concat_filtered.reduce<c2_reducer_1, 0u>()));
  auto index = 0u;
  //    1, 2, 3, 5,  8, 1, 2,  3,  5,  8
  // => 1, 3, 5, 8, 12, 6, 8, 10, 13, 17
  EXPECT_THAT(c2_concat.filter([&index](unsigned x) {
    return ((index++) + x) % 3 != 0;
  }), testing::ElementsAre(1, 3, 5, 2, 3, 5, 8));

  EXPECT_EQ(1, c2_concat.index_of(2));
  EXPECT_EQ(8, c2_concat.last_index_of(5));
  EXPECT_TRUE(c2_concat.includes(8));
  EXPECT_EQ(rfl::npos, c2_concat.index_of(4));
  EXPECT_EQ(rfl::npos, c2_concat.last_index_of(6));
  EXPECT_FALSE(c2_concat.includes(9));
  EXPECT_EQ(rfl::npos, c2_concat.index_of(point_t{.x = 1, .y = 2}));
  EXPECT_EQ(rfl::npos, c2_concat.last_index_of(std::monostate{}));

  auto tags = rfl::tags_constant<
    std::monostate,
    std::remove_cvref<int&>,
    std::remove_cvref<const int&>>();
  EXPECT_EQ(2, tags.index_of(std::remove_cvref<int const&>{}));
  EXPECT_EQ(1, tags.last_index_of(std::remove_cvref<int&>{}));
  EXPECT_EQ(rfl::npos, tags.index_of(std::remove_cvref<int>{}));
  EXPECT_EQ(rfl::npos, tags.last_index_of(std::remove_cvref<int&&>{}));

  auto tags_misuse = rfl::tags_constant<
    std::is_integral<int>,    // -> true
    std::is_integral<float>,  // -> false
    std::is_integral<long>,   // -> true
    std::monostate>();        // Not convertible
  EXPECT_EQ(0, tags_misuse.index_of(std::is_integral<long>{}));
  EXPECT_EQ(1, tags_misuse.last_index_of(std::is_integral<double>{}));
}

TEST(UtilsMisc, CType)
{
  for (unsigned char i = 0; i <= 127; i++) {
#define MAKE_EXPECT_EQ(fn)                            \
    EXPECT_EQ(!!std::fn(i), rfl::fn(i))               \
      << std::format("Wrong " #fn "('\\x{:x}')", i);

    REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(MAKE_EXPECT_EQ)
#undef MAKE_EXPECT_EQ
  }

  for (signed char i = -128; i < 0; i++) {
#define MAKE_EXPECT_EQ(fn)                                    \
    EXPECT_FALSE(rfl::fn(i))                                  \
      << std::format("Wrong " #fn "('\\x{:x}')", (uint8_t)i);

    REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(MAKE_EXPECT_EQ)
#undef MAKE_EXPECT_EQ
  }

  EXPECT_FALSE(rfl::isalpha(256 + 'A')) << "Failed to detect overflow.";
  EXPECT_FALSE(rfl::isalpha(65536u + 'A')) << "Failed to detect overflow.";
}
