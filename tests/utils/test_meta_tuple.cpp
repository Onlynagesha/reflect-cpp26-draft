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
  // Member get()
  EXPECT_EQ('0', t.get<0>());
  EXPECT_EQ(123, t.get<1>());
  EXPECT_EQ(2.5, t.get<2>());
  // Free get() via ADL
  EXPECT_EQ('0', get<0>(t));
  EXPECT_EQ(123, get<1>(t));
  EXPECT_EQ(2.5, get<2>(t));
}
