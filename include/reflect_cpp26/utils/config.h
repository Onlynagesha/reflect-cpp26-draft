#ifndef REFLECT_CPP26_UTILS_CONFIG_H
#define REFLECT_CPP26_UTILS_CONFIG_H

#if __cplusplus
#if __has_include(<meta>)
#include <meta>
#elif __has_include(<experimental/meta>)
#include <experimental/meta>
#else
#error "<meta> for C++26 Reflection is missing!"
#endif
#endif // __cplusplus

#if __cplusplus
#define REFLECT_CPP26_BOOL bool
#define REFLECT_CPP26_CONSTEXPR constexpr
#define REFLECT_CPP26_EXTERN_C_BEGIN extern "C" {
#define REFLECT_CPP26_EXTERN_C_END }
#define REFLECT_CPP26_INLINE inline // For inline functions only
#else
#define REFLECT_CPP26_BOOL _Bool
#define REFLECT_CPP26_CONSTEXPR static inline
#define REFLECT_CPP26_EXTERN_C_BEGIN
#define REFLECT_CPP26_EXTERN_C_END
#define REFLECT_CPP26_INLINE static inline // For inline functions only
#endif

#if __cplusplus
int reflect_cpp26_raise_error_if_consteval(const char* msg);
#define REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg)       \
do {                                                \
  if consteval {                                    \
    reflect_cpp26_raise_error_if_consteval(msg);    \
  }                                                 \
} while (false)
#else
#define REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg) // No-op
#endif

#define REFLECT_CPP26_UNREACHABLE(msg)    \
  REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg);  \
  __builtin_unreachable()

#endif // REFLECT_CPP26_UTILS_CONFIG_H
