#ifndef REFLECT_CPP26_UTILS_TO_STRING_UTILS_HPP
#define REFLECT_CPP26_UTILS_TO_STRING_UTILS_HPP

#include <reflect_cpp26/utils/config.h>
#include <cstddef>

namespace reflect_cpp26 {
// Including minus sign '-'
constexpr size_t max_decimal_digits_int8  = 4;  // -128
constexpr size_t max_decimal_digits_int16 = 6;  // -32768
constexpr size_t max_decimal_digits_int32 = 11; // -2147483648
constexpr size_t max_decimal_digits_int64 = 20; // -9223372036854775808

consteval size_t max_decimal_digits(size_t bytes)
{
  switch (bytes) {
    case 1:
      return max_decimal_digits_int8;
    case 2:
      return max_decimal_digits_int16;
    case 4:
      return max_decimal_digits_int32;
    case 8:
      return max_decimal_digits_int64;
    default:
      compile_error("Invalid size bytes.");
      return 0; // dummy
  }
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_TO_STRING_UTILS_HPP
