#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits/tuple_like_types.hpp>
#include <reflect_cpp26/utils/constant.hpp>
#include <reflect_cpp26/utils/meta_tuple.hpp>

namespace rfl = reflect_cpp26;

using std_tuple = std::tuple<int, int&, int**>;
static_assert(rfl::is_tuple_like_v<std_tuple>);
using std_pair = std::pair<std::string&&, size_t&>;
static_assert(rfl::is_tuple_like_v<std_pair>);
using rfl_constant = rfl::constant<1, 2u, '3', 4.0>;
static_assert(rfl::is_tuple_like_v<rfl_constant>);
using rfl_meta_tuple = rfl::meta_tuple<char, char*, char**>;
static_assert(rfl::is_tuple_like_v<rfl_meta_tuple>);

static_assert(rfl::is_tuple_like_of_elements_v<
  std_tuple, int, int&, int**>);
static_assert(rfl::is_tuple_like_of_elements_v<
  std_pair, std::string&&, size_t&>);
static_assert(rfl::is_tuple_like_of_elements_v<
  rfl_constant, int, unsigned, char, double>);
static_assert(rfl::is_tuple_like_of_elements_v<
  rfl_meta_tuple, char, char*, char**>);

// Expects exact match
static_assert(! rfl::is_tuple_like_of_elements_v<
  std_pair, std::string, size_t>);

TEST(TypeTraitsClassTypes, IsTupleLike) {
  EXPECT_TRUE(true); // All test cases done with static-asserts above
}
