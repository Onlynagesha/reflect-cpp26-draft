#include <gtest/gtest.h>
#include <reflect_cpp26/utils/meta_tuple.hpp>

namespace rfl = reflect_cpp26;

TEST(UtilsMetaTuple, Basic)
{
  using tuple_type = rfl::meta_tuple<char, int, double>;
  static_assert(std::tuple_size_v<tuple_type> == 3);
  static_assert(std::is_same_v<std::tuple_element_t<0, tuple_type>, char>);
  static_assert(std::is_same_v<std::tuple_element_t<1, tuple_type>, int>);
  static_assert(std::is_same_v<std::tuple_element_t<2, tuple_type>, double>);

  auto t = tuple_type{'0', 123, 2.5};
  // Free get() via ADL
  EXPECT_EQ('0', get<0>(t));
  EXPECT_EQ(123, get<1>(t));
  EXPECT_EQ(2.5, get<2>(t));
}

TEST(UtilsMetaTuple, CTAD)
{
  constexpr auto x = 1;
  constexpr auto y = 2.0;
  auto t = rfl::meta_tuple{x, y};
  static_assert(std::is_same_v<rfl::meta_tuple<int, double>, decltype(t)>);
  get<0>(t) += 100;
  get<1>(t) += 1.5;
  EXPECT_EQ(101, get<0>(t));
  EXPECT_EQ(3.5, get<1>(t));
}
