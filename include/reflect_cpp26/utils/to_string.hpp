#ifndef REFLECT_CPP26_UTILS_TO_STRING_HPP
#define REFLECT_CPP26_UTILS_TO_STRING_HPP

#include <reflect_cpp26/utils/ctype.h>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <reflect_cpp26/utils/type_traits.hpp>
#include <reflect_cpp26/utils/utility.hpp>
#include <bit>
#include <charconv>

namespace reflect_cpp26 {
namespace impl {
constexpr auto byte_to_hex_table_v =
  "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
  "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
  "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
  "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
  "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
  "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
  "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
  "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

template <class... Args>
constexpr bool try_to_chars(std::string* dest, Args... args)
{
  auto [ptr, ec] = std::to_chars(
    dest->data(), dest->data() + dest->size(), args...);
  if (std::errc{} == ec) {
    dest->resize(ptr - dest->data());
    return true;
  }
  return false;
}

template <class... Args>
constexpr auto try_to_chars(std::span<const size_t> buffer_sizes, Args... args)
  -> std::string
{
  auto res = std::string{};
  for (auto cur_buffer_size: buffer_sizes) {
    res.assign(cur_buffer_size, '\0');
    if (try_to_chars(&res, args...)) {
      return res;
    }
  }
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(
    "Internal error: Failure during floating-to-string conversion.");
  return "<ERROR:internal-error>";
}
} // namespace impl

constexpr auto to_string(bool value) -> std::string {
  return value ? "true" : "false";
}

constexpr auto to_string(char8_t value, bool quoted = false) -> std::string
{
  if (isprint(value)) {
    if (quoted) {
      auto res = std::string(3zU, '\'');
      res[1] = value;
      return res;
    }
    return std::string(1zU, static_cast<char>(value));
  }
  auto res = std::string{quoted ? "'\\x**'" : "\\x**"};
  auto* out = res.data() + quoted + 2; // 2 : Length of backslash and 'x'
  constexpr auto n_xdigits_per_byte = 2;
  std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * value,
              n_xdigits_per_byte, out);
  return res;
}

constexpr auto to_string(char value, bool quoted = false) -> std::string {
  return to_string(static_cast<char8_t>(value), quoted);
}

constexpr auto to_string(char16_t value, bool quoted = false) -> std::string
{
  static_assert(sizeof(char16_t) == 2, "Unsupported char16_t");
  if (in_range<char8_t>(value)) {
    return to_string(static_cast<char8_t>(value), quoted);
  }
  auto bytes = std::bit_cast<std::array<uint8_t, 2>>(value);
  auto res = std::string{quoted ? "'\\u****'" : "\\u****"};
  auto* out = res.data() + quoted + 2; // 2 : Length of backslash and 'u'
  constexpr auto n_xdigits_per_byte = 2;
  if constexpr (std::endian::native == std::endian::little) {
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[1],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[0],
                      n_xdigits_per_byte, out);
  } else if constexpr (std::endian::native == std::endian::big) {
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[0],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[1],
                      n_xdigits_per_byte, out);
  } else {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Internal error: Unknown endian.");
    return "<ERROR:unknown-endian>";
  }
  return res;
}

constexpr auto to_string(char32_t value, bool quoted = false) -> std::string
{
  static_assert(sizeof(char32_t) == 4, "Unsupported char32_t");
  if (in_range<char16_t>(value)) {
    return to_string(static_cast<char16_t>(value), quoted);
  }
  auto bytes = std::bit_cast<std::array<uint8_t, 4>>(value);
  auto res = std::string{quoted ? "'\\U********'" : "\\U********"};
  auto* out = res.data() + quoted + 2; // 2 : Length of backslash and 'U'
  constexpr auto n_xdigits_per_byte = 2;
  if constexpr (std::endian::native == std::endian::little) {
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[3],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[2],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[1],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[0],
                      n_xdigits_per_byte, out);
  } else if constexpr (std::endian::native == std::endian::big) {
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[0],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[1],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[2],
                      n_xdigits_per_byte, out);
    out = std::copy_n(impl::byte_to_hex_table_v + n_xdigits_per_byte * bytes[3],
                      n_xdigits_per_byte, out);
  } else {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Internal error: Unknown endian.");
    return "<ERROR:unknown-endian>";
  }
  return res;
}

constexpr auto to_string(wchar_t value, bool quoted = false) -> std::string
{
  if constexpr (sizeof(wchar_t) == 1) {
    return to_string(static_cast<char8_t>(value), quoted);
  } else if constexpr (sizeof(wchar_t) == 2) {
    return to_string(static_cast<char16_t>(value), quoted);
  } else if constexpr (sizeof(wchar_t) == 4) {
    return to_string(static_cast<char32_t>(value), quoted);
  } else {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Internal error: Unknown size of wchar_t");
    return "<ERROR:unknown-wchar-size>";
  }
}

template <class IntegerT>
  requires (is_integer_type_v<IntegerT>)
constexpr auto to_string(IntegerT value, int radix = 10) -> std::string
{
  if (radix < 2 || radix > 36) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid radix: out of range [2, 36].");
    return std::format("<ERROR:invalid-radix({})>", radix);
  }
  // 8 : One byte for minus sign '-' and other 7 bytes for alignment
  constexpr auto buffer_size = CHAR_BIT * sizeof(IntegerT) + 8;
  char buffer[buffer_size];
  auto [ptr, ec] = std::to_chars(buffer, buffer + buffer_size, value, radix);
  if (std::errc{} != ec) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL(
      "Internal error: Failure during integer-to-string conversion.");
    return "<ERROR:internal-error>";
  }
  return std::string{buffer, ptr};
}

template <class FloatT>
  requires (is_same_as_one_of_v<FloatT, float, double>)
constexpr auto to_string_fixed_format(FloatT value) -> std::string
{
  // Note: -FLT_MIN and -FLT_TRUE_MIN require 48 characters ('-' and 47 digits)
  //       -DBL_TRUE_MIN requires 327 characters ('-' and 326 digits)
  constexpr auto buffer_size = std::is_same_v<FloatT, float> ? 56zU : 336zU;
  char buffer[buffer_size];
  auto [ptr, ec] = std::to_chars(
    buffer, buffer + buffer_size, value, std::chars_format::fixed);
  if (std::errc{} == ec) {
    return std::string{buffer, ptr};
  }
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(
    "Internal error: Failure during float-to-string conversion.");
  return "<ERROR:internal-error>";
}

template <class FloatT>
  requires (std::is_same_v<FloatT, float>)
constexpr auto to_string_fixed_format(FloatT value, int precision)
  -> std::string
{
  if (precision < 0) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid precision: non-negative only.");
    return std::format("<ERROR:invalid-precision({})>", precision);
  }
  // Note: -FLT_MIN and -FLT_TRUE_MIN require 48 characters ('-' and 47 digits)
  constexpr auto integral_part_buffer_size = 56zU;
  auto res = std::string(integral_part_buffer_size + precision, '\0');
  if (impl::try_to_chars(&res, value, std::chars_format::fixed, precision)) {
    return res;
  }
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(
    "Internal error: Failure during float-to-string conversion.");
  return "<ERROR:internal-error>";
}

template <class FloatT>
  requires (std::is_same_v<FloatT, double>)
constexpr auto to_string_fixed_format(FloatT value, int precision)
  -> std::string
{
  if (precision < 0) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid precision: non-negative only.");
    return std::format("<ERROR:invalid-precision({})>", precision);
  }
  // Note: -DBL_TRUE_MIN requires 327 characters ('-' and 326 digits)
  auto buffer_sizes = std::array{56zU + precision,  336zU + precision};
  return impl::try_to_chars(
    buffer_sizes, value, std::chars_format::fixed, precision);
}

constexpr auto to_string_fixed_format(long double value) -> std::string
{
  // Note: -LDBL_TRUE_MIN requires 4954 characters ('-' and 4953 digits)
  constexpr auto buffer_sizes = std::array{56zU, 336zU, 1280zU, 4960zU};
  return impl::try_to_chars(buffer_sizes, value, std::chars_format::fixed);
}

template <class FloatT>
  requires (std::is_same_v<FloatT, long double>)
constexpr auto to_string_fixed_format(FloatT value, int precision)
  -> std::string
{
  if (precision < 0) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid precision: non-negative only.");
    return std::format("<ERROR:invalid-precision({})>", precision);
  }
  // Note: -LDBL_TRUE_MIN requires 4954 characters ('-' and 4953 digits)
  auto buffer_sizes = std::array{
      56zU + precision,  336zU + precision,
    1280zU + precision, 4960zU + precision};
  return impl::try_to_chars(
    buffer_sizes, value, std::chars_format::fixed, precision);
}

template <class FloatT>
  requires (std::is_floating_point_v<FloatT>)
constexpr auto to_string(
  FloatT value, std::chars_format fmt = std::chars_format::general)
  -> std::string
{
  if (fmt == std::chars_format::fixed) {
    return to_string_fixed_format(value);
  }
  if (fmt != std::chars_format::hex && fmt != std::chars_format::scientific &&
      fmt != std::chars_format::general) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid argument 'fmt'.");
    return std::format(
      "<ERROR:invalid-chars-format({})>", std::to_underlying(fmt));
  }
  // 56 bytes is enough for hex and scientific mode with 128-bit IEEE quadraple.
  constexpr auto buffer_size = 56zU;
  char buffer[buffer_size];
  auto [ptr, ec] = std::to_chars(
    buffer, buffer + buffer_size, value, fmt);
  if (std::errc{} == ec) {
    return std::string{buffer, ptr};
  }
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(
    "Internal error: Failure during float-to-string conversion.");
  return "<ERROR:internal-error>";
}

template <class FloatT>
  requires (std::is_floating_point_v<FloatT>)
constexpr auto to_string(FloatT value, std::chars_format fmt, int precision)
  -> std::string
{
  if (fmt == std::chars_format::fixed) {
    return to_string_fixed_format(value, precision);
  }
  if (precision < 0) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid precision: non-negative only.");
    return std::format("<ERROR:invalid-precision({})>", precision);
  }
  if (fmt != std::chars_format::hex && fmt != std::chars_format::scientific &&
      fmt != std::chars_format::general) {
    REFLECT_CPP26_ERROR_IF_CONSTEVAL("Invalid argument 'fmt'.");
    return std::format(
      "<ERROR:invalid-chars-format({})>", std::to_underlying(fmt));
  }
  // 12 more bytes is enough for hex and scientific mode.
  auto res = std::string(precision + 12, '\0');
  if (impl::try_to_chars(&res, value, fmt, precision)) {
    return res;
  }
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(
    "Internal error: Failure during float-to-string conversion.");
  return "<ERROR:internal-error>";
}

constexpr auto to_string(const char* string) -> std::string
{
#ifdef REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
  return "to_string(const char*)";
#else
  return std::string{string};
#endif
}

template <class Traits, class Alloc>
constexpr auto to_string(const std::basic_string<char, Traits, Alloc>& string)
  -> std::string
{
#ifdef REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
  return "to_string(const basic_string&)";
#else
  using StringType = std::basic_string<char, Traits, Alloc>;
  if constexpr (std::is_same_v<StringType, std::string>) {
    return string;
  } else {
    return StringType{string.begin(), string.end()};
  }
#endif
}

template <class Traits, class Alloc>
constexpr auto to_string(std::basic_string<char, Traits, Alloc>&& string)
  -> std::string
{
#ifdef REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
  return "to_string(basic_string&&)";
#else
  using StringType = std::basic_string<char, Traits, Alloc>;
  if constexpr (std::is_same_v<StringType, std::string>) {
    return std::move(string);
  } else {
    return StringType{string.begin(), string.end()};
  }
#endif
}

template <class Traits>
constexpr auto to_string(std::basic_string_view<char, Traits> string)
  -> std::string
{
#ifdef REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
  return "to_string(basic_string_view)";
#else
  return std::string{string.begin(), string.end()};
#endif
}

constexpr auto to_string(meta_string_view string) -> std::string
{
#ifdef REFLECT_CPP26_ENABLE_TO_STRING_OVERLOAD_TEST
  return "to_string(meta_string_view)";
#else
  return std::string{string.begin(), string.end()};
#endif
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_TO_STRING_HPP
