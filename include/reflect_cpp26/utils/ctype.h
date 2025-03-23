#ifndef REFLECT_CPP26_UTILS_CTYPE_H
#define REFLECT_CPP26_UTILS_CTYPE_H

#include <reflect_cpp26/utils/config.h>
#include <stdint.h>

/**
 * Constexpr, locale-independent alternative to std::isalnum etc. in <cctype>.
 */

#define REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(F) \
  F(isalnum)  \
  F(isalpha)  \
  F(islower)  \
  F(isupper)  \
  F(isdigit)  \
  F(isxdigit) \
  F(isblank)  \
  F(iscntrl)  \
  F(isgraph)  \
  F(isspace)  \
  F(isprint)  \
  F(ispunct)

static const uint8_t reflect_cpp26_ctype_print_mask = 1u;
static const uint8_t reflect_cpp26_ctype_space_mask = 2u;
static const uint8_t reflect_cpp26_ctype_blank_mask = 4u;
static const uint8_t reflect_cpp26_ctype_punct_mask = 8u;
static const uint8_t reflect_cpp26_ctype_upper_mask = 16u;
static const uint8_t reflect_cpp26_ctype_lower_mask = 32u;
static const uint8_t reflect_cpp26_ctype_digit_mask = 64u;
static const uint8_t reflect_cpp26_ctype_xdigit_mask = 128u;

static const uint8_t reflect_cpp26_ctype_alpha_mask =
  reflect_cpp26_ctype_upper_mask | reflect_cpp26_ctype_lower_mask;
static const uint8_t reflect_cpp26_ctype_alnum_mask =
  reflect_cpp26_ctype_alpha_mask | reflect_cpp26_ctype_digit_mask;
static const uint8_t reflect_cpp26_ctype_graph_mask =
  reflect_cpp26_ctype_alnum_mask | reflect_cpp26_ctype_punct_mask;

#if __cplusplus
constexpr uint8_t reflect_cpp26_ctype_flag_table[128] =
#else
static const uint8_t reflect_cpp26_ctype_flag_table[128] =
#endif
{
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x0,  0x6,  0x2,  0x2,  0x2,  0x2,  0x0,  0x0,
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
  0x7,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
  0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
  0xc1, 0xc1, 0x9,  0x9,  0x9,  0x9,  0x9,  0x9,
  0x9,  0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x11,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
  0x11, 0x11, 0x11, 0x9,  0x9,  0x9,  0x9,  0x9,
  0x9,  0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0x21,
  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
  0x21, 0x21, 0x21, 0x9,  0x9,  0x9,  0x9,  0x0
};

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isalnum(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_alnum_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isalpha(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_alpha_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_islower(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_lower_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isupper(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_upper_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isdigit(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_digit_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isxdigit(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_xdigit_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isblank(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_blank_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_iscntrl(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_print_mask) == 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isgraph(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_graph_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isspace(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_space_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_isprint(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_print_mask) != 0;
}

REFLECT_CPP26_CONSTEXPR REFLECT_CPP26_BOOL
reflect_cpp26_ispunct(uint8_t c) {
  return (c <= 127u)
    && (reflect_cpp26_ctype_flag_table[c]
      & reflect_cpp26_ctype_punct_mask) != 0;
}

#if __cplusplus
namespace reflect_cpp26 {
#define REFLECT_CPP26_DEFINE_CTYPE_FUNCTION_IN_NAMESPACE(func)  \
  constexpr bool func(uint8_t c) {                              \
    return reflect_cpp26_##func(c);                             \
  }                                                             \
  template <class T>                                            \
    requires (std::is_integral_v<T>)                            \
  constexpr bool func(T c) {                                    \
    return c >= 0 && c <= 127                                   \
      && reflect_cpp26_##func(static_cast<uint8_t>(c));         \
  }

REFLECT_CPP26_CTYPE_FUNCTION_FOR_EACH(
  REFLECT_CPP26_DEFINE_CTYPE_FUNCTION_IN_NAMESPACE)

#undef REFLECT_CPP26_DEFINE_CTYPE_FUNCTION_IN_NAMESPACE
} // namespace reflect_cpp26
#endif

#endif // REFLECT_CPP26_UTILS_CTYPE_H
