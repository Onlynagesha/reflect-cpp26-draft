#include <gtest/gtest.h>
#include <reflect_cpp26/utils/preprocessors.h>

TEST(UtilsPreprocessors, NArgs)
{
  EXPECT_EQ(1, REFLECT_CPP26_MACRO_N_ARGS(x));
  EXPECT_EQ(2, REFLECT_CPP26_MACRO_N_ARGS(x, y));
  EXPECT_EQ(6, REFLECT_CPP26_MACRO_N_ARGS(x, y, z, a, b, c));

  EXPECT_EQ(0, REFLECT_CPP26_MACRO_N_ARGS());
  EXPECT_EQ(2, REFLECT_CPP26_MACRO_N_ARGS(x, ));
  EXPECT_EQ(2, REFLECT_CPP26_MACRO_N_ARGS(, y));
  EXPECT_EQ(2, REFLECT_CPP26_MACRO_N_ARGS(,));
  EXPECT_EQ(8, REFLECT_CPP26_MACRO_N_ARGS(, , , , , , ,)); // 7 commas

  EXPECT_EQ(64, REFLECT_CPP26_MACRO_N_ARGS(
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3));

  using matrix_double_4x4 = std::array<std::array<double, 4>, 4>;
  // A common misusage in C++: Macro arguments that contain commas
  EXPECT_EQ(3,
    REFLECT_CPP26_MACRO_N_ARGS(std::array<std::array<double, 4>, 4>));
  EXPECT_EQ(1, // ____________ | ------------- braced ------------- |
    REFLECT_CPP26_MACRO_N_ARGS((std::array<std::array<double, 4>, 4>)));
  EXPECT_EQ(1,
    REFLECT_CPP26_MACRO_N_ARGS(matrix_double_4x4));
}

template <size_t Index>
struct demo_type {
  static constexpr auto value = std::string_view{"<default>"};
};

#define INSTANTIATE_DEMO_TYPE(k, v)                     \
  template <>                                           \
  struct demo_type<k> {                                 \
    static constexpr auto value = std::string_view{v};  \
  };
#define INSTANTIATE_DEMO_TYPE_KVPAIR(kv_pair) \
  INSTANTIATE_DEMO_TYPE kv_pair // Removes parentheses

REFLECT_CPP26_MACRO_FOREACH(INSTANTIATE_DEMO_TYPE_KVPAIR,
  (10, "ten"),
  (20, "twelve"),
  (30, "thirty"),
  (40, "fourty"))

#undef INSTANTIATE_DEMO_TYPE
#undef INSTANTIATE_DEMO_TYPE_KVPAIR

TEST(UtilsPreprocessors, ForEach)
{
  int32_t sum = 0;
#define INCREMENT(x) sum += (x);
  REFLECT_CPP26_MACRO_FOREACH(INCREMENT, 10, 20, 30, 40, 50, 60, 70, 80, 90)
  EXPECT_EQ(450, sum);

  sum = 0;
  REFLECT_CPP26_MACRO_FOREACH(INCREMENT, 10)
  EXPECT_EQ(10, sum);

  sum = 0;
  REFLECT_CPP26_MACRO_FOREACH(INCREMENT) // No-op
  EXPECT_EQ(0, sum);
#undef INCREMENT

  EXPECT_EQ("ten", demo_type<10>::value);
  EXPECT_EQ("twelve", demo_type<20>::value);
  EXPECT_EQ("thirty", demo_type<30>::value);
  EXPECT_EQ("fourty", demo_type<40>::value);
  EXPECT_EQ("<default>", demo_type<0>::value);
}
