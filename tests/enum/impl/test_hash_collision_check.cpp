#include "enum/test_cases.hpp"
#include "test_options.hpp"
#include <reflect_cpp26/enum/impl/hash_collision_check.hpp>

using reflect_cpp26::impl::enum_name_has_hash_collision_v;

static_assert(! enum_name_has_hash_collision_v<foo_signed>);
static_assert(! enum_name_has_hash_collision_v<foo_signed_reorder>);
static_assert(! enum_name_has_hash_collision_v<foo_signed_rep>);
static_assert(! enum_name_has_hash_collision_v<bar_unsigned>);
static_assert(! enum_name_has_hash_collision_v<baz_signed>);
static_assert(! enum_name_has_hash_collision_v<qux_unsigned>);
static_assert(! enum_name_has_hash_collision_v<empty>);
static_assert(! enum_name_has_hash_collision_v<single>);
static_assert(! enum_name_has_hash_collision_v<single_rep>);
static_assert(! enum_name_has_hash_collision_v<color>);
static_assert(! enum_name_has_hash_collision_v<terminal_color>);
static_assert(enum_name_has_hash_collision_v<hash_collision>);

TEST(EnumImpl, HashCollisionCheck) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
