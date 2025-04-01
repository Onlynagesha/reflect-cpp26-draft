#define REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD 1
#define TEST_SUITE_NAME EnumNameBinarySearch

#include "./test_enum_name_common.hpp"

namespace constants = reflect_cpp26::impl::enum_constants;
static_assert(
  constants::enable_binary_search_threshold_is_custom,
  "Implementation error.");
