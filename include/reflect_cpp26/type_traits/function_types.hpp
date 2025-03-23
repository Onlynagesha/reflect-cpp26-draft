#ifndef REFLECT_CPP26_TYPE_TRAITS_FUNCTION_TYPES_HPP
#define REFLECT_CPP26_TYPE_TRAITS_FUNCTION_TYPES_HPP

#include <reflect_cpp26/utils/type_tuple.hpp>

namespace reflect_cpp26 {
struct function_trait_flags {
  bool has_ellipsis_parameter;
  bool is_const;
  bool is_volatile;
  bool is_lvalue_reference;
  bool is_rvalue_reference;
  bool is_noexcept;

  constexpr auto operator<=>(const function_trait_flags&) const = default;
  constexpr bool operator==(const function_trait_flags&) const = default;
};

template <class Fn>
struct function_traits {
  static constexpr auto is_function = false;
};

// 2 variants: with ellipsis (C-style va-arg) or without
#define REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS(qualifiers, ...)     \
  template <class Ret, class... Args>                                 \
  struct function_traits<Ret(Args...) qualifiers> {                   \
    static constexpr auto is_function = true;                         \
    static constexpr auto flags = function_trait_flags{__VA_ARGS__};  \
                                                                      \
    using result_type = Ret;                                          \
    using args_type = type_tuple<Args...>;                            \
  };                                                                  \
                                                                      \
  template <class Ret, class... Args>                                 \
  struct function_traits<Ret(Args..., ...) qualifiers> {              \
    static constexpr auto is_function = true;                         \
    static constexpr auto flags = function_trait_flags{               \
      .has_ellipsis_parameter = true,                                 \
      __VA_ARGS__};                                                   \
                                                                      \
    using result_type = Ret;                                          \
    using args_type = type_tuple<Args...>;                            \
  }

// 4 variants: all subsets of {const, volatile}
#define REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_CV(qualifiers, ...)  \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS(qualifiers,                \
    __VA_ARGS__);                                                     \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS(const qualifiers,          \
    .is_const = true, __VA_ARGS__);                                   \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS(volatile qualifiers,       \
    .is_volatile = true, __VA_ARGS__);                                \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS(const volatile qualifiers, \
    .is_const = true, .is_volatile = true, __VA_ARGS__)

// 3 variants: no-ref, lvalue-ref and rvalue-ref
#define REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_REF(qualifiers, ...) \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_CV(qualifiers,             \
    __VA_ARGS__);                                                     \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_CV(& qualifiers,           \
    .is_lvalue_reference = true, __VA_ARGS__);                        \
  REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_CV(&& qualifiers,          \
    .is_rvalue_reference = true, __VA_ARGS__)

// 2 variants: with / without noexcept
REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_REF(/* n/a */, /* no flags */);
REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_REF(noexcept, .is_noexcept = true);

// Total: 2 x 4 x 3 x 2 = 48
#undef REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS
#undef REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_CV
#undef REFLECT_CPP26_SPECIALIZE_FUNCTION_TRAITS_REF
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_FUNCTION_TYPES_HPP
