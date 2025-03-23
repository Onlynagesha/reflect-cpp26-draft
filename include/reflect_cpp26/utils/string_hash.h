#ifndef REFLECT_CPP26_UTILS_STRING_HASH_H
#define REFLECT_CPP26_UTILS_STRING_HASH_H

#include <reflect_cpp26/utils/config.h>
#include <stdint.h>
#if __cplusplus
#include <string_view>
#endif

#define REFLECT_CPP26_BKDR_HASH_DEFAULT_P 131u

REFLECT_CPP26_EXTERN_C_BEGIN

REFLECT_CPP26_CONSTEXPR uint32_t reflect_cpp26_bkdr_hash32_p(
  const char* begin, const char* end, uint32_t p)
{
  uint32_t res = 0;
  for (; begin < end; ++begin) {
    res = res * p + (uint32_t)(*begin);
  }
  return res;
}

REFLECT_CPP26_CONSTEXPR uint32_t reflect_cpp26_bkdr_hash32(
  const char* begin, const char* end)
{
  return reflect_cpp26_bkdr_hash32_p(
    begin, end, REFLECT_CPP26_BKDR_HASH_DEFAULT_P);
}

REFLECT_CPP26_CONSTEXPR uint64_t reflect_cpp26_bkdr_hash64_p(
  const char* begin, const char* end, uint64_t p)
{
  uint64_t res = 0;
  for (; begin < end; ++begin) {
    res = res * p + (uint64_t)(*begin);
  }
  return res;
}

REFLECT_CPP26_CONSTEXPR uint64_t reflect_cpp26_bkdr_hash64(
  const char* begin, const char* end)
{
  return reflect_cpp26_bkdr_hash64_p(
    begin, end, REFLECT_CPP26_BKDR_HASH_DEFAULT_P);
}

REFLECT_CPP26_EXTERN_C_END

#if __cplusplus
namespace reflect_cpp26 {
constexpr uint32_t bkdr_hash32(
  const char* begin, const char* end,
  uint32_t p = REFLECT_CPP26_BKDR_HASH_DEFAULT_P)
{
  return reflect_cpp26_bkdr_hash32_p(begin, end, p);
}

constexpr uint32_t bkdr_hash32(
  std::string_view str, uint32_t p = REFLECT_CPP26_BKDR_HASH_DEFAULT_P)
{
  return reflect_cpp26_bkdr_hash32_p(str.data(), str.data() + str.size(), p);
}

constexpr uint64_t bkdr_hash64(
  const char* begin, const char* end,
  uint64_t p = REFLECT_CPP26_BKDR_HASH_DEFAULT_P)
{
  return reflect_cpp26_bkdr_hash64_p(begin, end, p);
}

constexpr uint64_t bkdr_hash64(
  std::string_view str, uint64_t p = REFLECT_CPP26_BKDR_HASH_DEFAULT_P)
{
  return reflect_cpp26_bkdr_hash64_p(str.data(), str.data() + str.size(), p);
}
} // namespace reflect_cpp26
#endif // __cplusplus

#endif // REFLECT_CPP26_UTILS_STRING_HASH_H
