#ifndef REFLECT_CPP26_ENUM_IMPL_CONSTANTS_HPP
#define REFLECT_CPP26_ENUM_IMPL_CONSTANTS_HPP

#include <cstddef>

namespace reflect_cpp26::impl::enum_constants {
#ifdef REFLECT_CPP26_ENUM_ENABLE_VALUE_TABLE_LOOKUP_THRESHOLD
constexpr bool   enable_value_table_lookup_threshold_is_custom = true;
constexpr size_t enable_value_table_lookup_threshold =
  REFLECT_CPP26_ENUM_ENABLE_VALUE_TABLE_LOOKUP_THRESHOLD;
#else
constexpr bool   enable_value_table_lookup_threshold_is_custom = false;
constexpr size_t enable_value_table_lookup_threshold = 4;
#endif

#ifdef REFLECT_CPP26_ENUM_ENABLE_HASH_TABLE_LOOKUP_THRESHOLD
constexpr bool   enable_hash_table_lookup_threshold_is_custom = true;
constexpr size_t enable_hash_table_lookup_threshold =
  REFLECT_CPP26_ENUM_ENABLE_HASH_TABLE_LOOKUP_THRESHOLD;
#else
constexpr bool   enable_hash_table_lookup_threshold_is_custom = false;
constexpr size_t enable_hash_table_lookup_threshold = 4;
#endif

#ifdef REFLECT_CPP26_ENUM_DISABLE_HASH_TABLE_LOOKUP_THRESHOLD
constexpr bool   disable_hash_table_lookup_threshold_is_custom = true;
constexpr size_t disable_hash_table_lookup_threshold =
  REFLECT_CPP26_ENUM_DISABLE_HASH_TABLE_LOOKUP_THRESHOLD;
#else
constexpr bool   disable_hash_table_lookup_threshold_is_custom = false;
constexpr size_t disable_hash_table_lookup_threshold = 64;
#endif

#ifdef REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD
constexpr bool   enable_binary_search_threshold_is_custom = true;
constexpr size_t enable_binary_search_threshold =
  REFLECT_CPP26_ENUM_ENABLE_BINARY_SEARCH_THRESHOLD;
#else
constexpr bool   enable_binary_search_threshold_is_custom = false;
constexpr size_t enable_binary_search_threshold = 12;
#endif

#ifdef REFLECT_CPP26_ENUM_HASH_INV_MIN_LOAD_FACTOR
constexpr bool   inv_min_load_factor_is_custom = true;
constexpr size_t inv_min_load_factor =
  REFLECT_CPP26_ENUM_HASH_INV_MIN_LOAD_FACTOR;
#else
constexpr bool   inv_min_load_factor_is_custom = false;
constexpr size_t inv_min_load_factor = 4; // Load factor >= 0.25 by default
#endif
} // namespace reflect_cpp26::impl::enum_constants

#endif // REFLECT_CPP26_ENUM_IMPL_CONSTANTS_HPP
