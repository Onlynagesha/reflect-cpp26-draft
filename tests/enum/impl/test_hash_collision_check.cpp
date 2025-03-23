#include "../test_cases.hpp"
#include <gtest/gtest.h>
#include <reflect_cpp26/impl/enum/hash_collision_check.hpp>

using reflect_cpp26::impl::enum_name_has_hash_collision_v;

TEST(EnumImpl, HashCollisionCheck)
{
  EXPECT_FALSE(enum_name_has_hash_collision_v<foo_signed>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<foo_signed_reorder>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<foo_signed_rep>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<bar_unsigned>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<baz_signed>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<qux_unsigned>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<empty>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<single>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<single_rep>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<color>);
  EXPECT_FALSE(enum_name_has_hash_collision_v<terminal_color>);
  EXPECT_TRUE(enum_name_has_hash_collision_v<hash_collision>);
}
