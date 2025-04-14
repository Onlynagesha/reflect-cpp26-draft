#pragma once

#include "test_options.hpp"

#ifdef ENABLE_BINARY_SEARCH_CHECK
#define REFLECT_CPP26_ENUM_ENABLE_HASH_TABLE_LOOKUP_THRESHOLD 9999
#define REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD 0
#endif

#ifdef ENABLE_LINEAR_SEARCH_CHECK
#define REFLECT_CPP26_ENUM_ENABLE_HASH_TABLE_LOOKUP_THRESHOLD 9999
#define REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD 9999
#endif

#include <reflect_cpp26/enum.hpp>

namespace enum_constants = reflect_cpp26::impl::enum_constants;

#if defined(ENABLE_BINARY_SEARCH_CHECK) || defined(ENABLE_LINEAR_SEARCH_CHECK)
static_assert(enum_constants::enable_hash_table_lookup_threshold_is_custom);
static_assert(enum_constants::enable_binary_search_threshold_is_custom);
#endif
