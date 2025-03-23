#ifndef REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP
#define REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP

#include <reflect_cpp26/type_traits/conditional_switch.hpp>
#include <reflect_cpp26/type_traits/type_comparison.hpp>

namespace reflect_cpp26 {
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
constexpr bool is_char_type_v = is_same_as_one_of_v<std::remove_cv_t<T>,
  char, wchar_t, char8_t, char16_t, char32_t>;

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

template <class T>
struct integral_to_integer {
private:
  static_assert(std::is_integral_v<T>, "Integral types only.");
  using K = std::pair<size_t, bool>;

public:
  static constexpr auto size_bytes = sizeof(T);
  static constexpr auto is_signed = std::is_signed_v<T>;

  using type = conditional_switch_t<K{size_bytes, is_signed},
    condition_case<K{1, true}, int8_t>,
    condition_case<K{2, true}, int16_t>,
    condition_case<K{4, true}, int32_t>,
    condition_case<K{8, true}, int64_t>,
    condition_case<K{1, false}, uint8_t>,
    condition_case<K{2, false}, uint16_t>,
    condition_case<K{4, false}, uint32_t>,
    condition_case<K{8, false}, uint64_t> /* Compile error on mismatch */ >;
};

/**
 * Transforms an arbitrary integral type (including bool and characters)
 * to integer type with the same size and signedness
 */
template <class T>
using integral_to_integer_t = typename integral_to_integer<T>::type;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_ARITHMETIC_TYPES_HPP
