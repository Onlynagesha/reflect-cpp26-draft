#ifndef REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP
#define REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP

// Root header: include C++ stdlib headers only to prevent circular dependency.
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace reflect_cpp26 {
#define REFLECT_CPP26_CHAR_TYPE_FOR_EACH(F) \
  F(char)                                   \
  F(wchar_t)                                \
  F(char8_t)                                \
  F(char16_t)                               \
  F(char32_t)

template <class T>
struct is_char_type : std::false_type {};

#define REFLECT_CPP26_SPECIALIZE_IS_CHAR_TYPE(CharT)              \
  template <>                                                     \
  struct is_char_type<CharT> : std::true_type {};                 \
  template <>                                                     \
  struct is_char_type<const CharT> : std::true_type {};           \
  template <>                                                     \
  struct is_char_type<volatile CharT> : std::true_type {};        \
  template <>                                                     \
  struct is_char_type<const volatile CharT> : std::true_type {};

REFLECT_CPP26_CHAR_TYPE_FOR_EACH(REFLECT_CPP26_SPECIALIZE_IS_CHAR_TYPE)

#undef REFLECT_CPP26_CHAR_TYPE_FOR_EACH
#undef REFLECT_CPP26_SPECIALIZE_IS_CHAR_TYPE

/**
 * Whether T is a (possibly cv-qualified) character type.
 * All candidates character types are listed below.
 *
 * Note: signed char and unsigned char match integer_type instead of char_type
 * since their semantics are designed to be a character's
 * integral representation rather than the character itself
 * according to C++ standard.
 */
template <class T>
constexpr bool is_char_type_v = is_char_type<T>::value;

template <class T>
concept char_type = is_char_type_v<T>;

/**
 * Whether T is a (possibly cv-qualified) integer type.
 */
template <class T>
constexpr bool is_integer_type_v = std::is_integral_v<T>
  && !std::is_same_v<std::remove_cv_t<T>, bool>
  && !is_char_type_v<T>;

template <class T>
concept integer_type = is_integer_type_v<T>;

namespace impl {
template <size_t SizeBytes, bool IsUnsigned>
struct integral_to_integer_helper {
  static_assert(false, "Invalid (SizeBytes, IsUnsigned) combination.");
};

#define REFLECT_CPP26_INTEGRAL_TO_INTEGER_FOR_EACH(F) \
  F(1, false, int8_t)                                 \
  F(1, true, uint8_t)                                 \
  F(2, false, int16_t)                                \
  F(2, true, uint16_t)                                \
  F(4, false, int32_t)                                \
  F(4, true, uint32_t)                                \
  F(8, false, int64_t)                                \
  F(8, true, uint64_t)

#define REFLECT_CPP26_SPECIALIZE_INTEGRAL_TO_INTEGER_HELPER(N, U, T)  \
  template <>                                                         \
  struct integral_to_integer_helper<N, U> {                           \
    using type = T;                                                   \
  };

REFLECT_CPP26_INTEGRAL_TO_INTEGER_FOR_EACH(
  REFLECT_CPP26_SPECIALIZE_INTEGRAL_TO_INTEGER_HELPER)

#undef REFLECT_CPP26_INTEGRAL_TO_INTEGER_FOR_EACH
#undef REFLECT_CPP26_SPECIALIZE_INTEGRAL_TO_INTEGER_HELPER
} // namespace impl

/**
 * Transforms an arbitrary integral type (including bool and characters)
 * to integer type with the same size and signedness
 */
template <class T>
using integral_to_integer_t = typename
  impl::integral_to_integer_helper<sizeof(T), std::is_unsigned_v<T>>::type;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP
