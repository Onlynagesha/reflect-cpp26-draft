#define TEST_SUITE_NAME EnumCastFromString

#include "./test_enum_cast_from_string_common.hpp"

static_assert(
  ! impl::enum_constants::enable_hash_table_lookup_threshold_is_custom,
  "Implementation error.");
static_assert(
  ! impl::enum_constants::enable_binary_search_threshold_is_custom,
  "Implementation error.");
