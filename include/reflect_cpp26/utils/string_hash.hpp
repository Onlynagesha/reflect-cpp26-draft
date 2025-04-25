#ifndef REFLECT_CPP26_UTILS_STRING_HASH_HPP
#define REFLECT_CPP26_UTILS_STRING_HASH_HPP

#include <cstdint>
#include <string_view>

namespace reflect_cpp26 {
constexpr unsigned bkdr_hash_default_p = 131u;

constexpr uint32_t bkdr_hash32(
  const char* begin, const char* end, uint32_t p = bkdr_hash_default_p)
{
  uint32_t res = 0;
  for (; begin < end; ++begin) {
    res = res * p + (uint32_t)(*begin);
  }
  return res;
}

constexpr uint64_t bkdr_hash64(
  const char* begin, const char* end, uint64_t p = bkdr_hash_default_p)
{
  uint64_t res = 0;
  for (; begin < end; ++begin) {
    res = res * p + (uint64_t)(*begin);
  }
  return res;
}

constexpr uint32_t bkdr_hash32(
  std::string_view str, uint32_t p = bkdr_hash_default_p)
{
  return bkdr_hash32(str.data(), str.data() + str.size(), p);
}

constexpr uint64_t bkdr_hash64(
  std::string_view str, uint64_t p = bkdr_hash_default_p)
{
  return bkdr_hash64(str.data(), str.data() + str.size(), p);
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_STRING_HASH_HPP
