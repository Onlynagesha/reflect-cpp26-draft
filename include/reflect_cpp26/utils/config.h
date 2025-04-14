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
#define REFLECT_CPP26_EXTERN_C_BEGIN extern "C" {
#define REFLECT_CPP26_EXTERN_C_END }
#else
#define REFLECT_CPP26_BOOL _Bool
#define REFLECT_CPP26_EXTERN_C_BEGIN
#define REFLECT_CPP26_EXTERN_C_END
#endif

#if __cplusplus
namespace reflect_cpp26 {
[[noreturn]] int compile_error(const char* msg) noexcept;
} // namespace reflect_cpp26
#define REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg) \
  do {                                        \
    if consteval {                            \
      ::reflect_cpp26::compile_error(msg);    \
    }                                         \
  } while (false)
#define REFLECT_CPP26_UNREACHABLE(msg)      \
  do {                                      \
    REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg);  \
    __builtin_unreachable();                \
  } while (false)
#else
#define REFLECT_CPP26_ERROR_IF_CONSTEVAL(msg) // No-op
#define REFLECT_CPP26_UNREACHABLE(msg) __builtin_unreachable()
#endif

#endif // REFLECT_CPP26_UTILS_CONFIG_H
