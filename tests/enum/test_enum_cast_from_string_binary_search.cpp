#define REFLECT_CPP26_ENUM_ENABLE_HASH_TABLE_LOOKUP_THRESHOLD 9999
#define REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD 1
#define TEST_SUITE_NAME EnumCastFromStringBinarySearch

#include "./test_enum_cast_from_string_common.hpp"

static_assert(
  impl::enum_constants::enable_hash_table_lookup_threshold_is_custom,
  "Implementation error.");
static_assert(
  impl::enum_constants::enable_binary_search_threshold_is_custom,
  "Implementation error.");
