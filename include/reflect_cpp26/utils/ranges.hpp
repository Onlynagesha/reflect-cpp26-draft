#ifndef REFLECT_CPP26_UTILS_RANGES_HPP
#define REFLECT_CPP26_UTILS_RANGES_HPP

#include <ranges>
#include <type_traits>

namespace reflect_cpp26 {
// -------- Extension of Ranges concepts --------

#if __cpp_lib_ranges_as_const >= 202207L
#define REFLECT_CPP26_CONST_RANGE_CONCEPTS_FOR_EACH(F) F(constant_range)
#else
#define REFLECT_CPP26_CONST_RANGE_CONCEPTS_FOR_EACH(F) // No-op
#endif

#define REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(F)  \
  F(range)                                        \
  F(borrowed_range)                               \
  F(sized_range)                                  \
  F(view)                                         \
  F(input_range)                                  \
  F(forward_range)                                \
  F(bidirectional_range)                          \
  F(random_access_range)                          \
  F(contiguous_range)                             \
  F(common_range)                                 \
  F(viewable_range)                               \
  REFLECT_CPP26_CONST_RANGE_CONCEPTS_FOR_EACH(F)

#define REFLECT_CPP26_DEFINE_RANGE_CONCEPTS(range_concept)                \
  template <class Range, class T>                                         \
  concept range_concept##_of = std::ranges::range_concept<Range>          \
    && std::is_convertible_v<std::ranges::range_value_t<Range>, T>;       \
                                                                          \
  template <class Range, class T>                                         \
  concept range_concept##_of_exactly = std::ranges::range_concept<Range>  \
    && std::is_same_v<std::ranges::range_value_t<Range>, T>;              \
                                                                          \
  template <class... Ranges>                                              \
  constexpr auto are_##range_concept##_v =                                \
    (std::ranges::range_concept<Ranges> && ...);                          \
                                                                          \
  template <class T, class... Ranges>                                     \
  constexpr auto are_##range_concept##_of_v =                             \
    (range_concept##_of<Ranges, T> && ...);                               \
                                                                          \
  template <class T, class... Ranges>                                     \
  constexpr auto are_##range_concept##_of_exactly_v =                     \
    (range_concept##_of_exactly<Ranges, T> && ...);

REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(REFLECT_CPP26_DEFINE_RANGE_CONCEPTS)

#undef REFLECT_CPP26_DEFINE_RANGE_CONCEPTS
#undef REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH

#define REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(F)  \
  F(borrowed_range)                               \
  F(sized_range)                                  \
  F(view)                                         \
  F(input_range)                                  \
  F(forward_range)                                \
  F(bidirectional_range)                          \
  F(random_access_range)                          \
  F(contiguous_range)                             \
  F(viewable_range)                               \
  REFLECT_CPP26_CONST_RANGE_CONCEPTS_FOR_EACH(F)

#define REFLECT_CPP26_DEFINE_RANGE_CONCEPTS(range_concept)                \
  template <class Range, class T>                                         \
  concept common_##range_concept##_of =                                   \
    std::ranges::range_concept<Range>                                     \
      && std::is_convertible_v<std::ranges::range_value_t<Range>, T>      \
      && std::ranges::common_range<Range>;                                \
                                                                          \
  template <class Range, class T>                                         \
  concept common_##range_concept##_of_exactly =                           \
    std::ranges::range_concept<Range>                                     \
      && std::is_same_v<std::ranges::range_value_t<Range>, T>             \
      && std::ranges::common_range<Range>;                                \
                                                                          \
  template <class... Ranges>                                              \
  constexpr auto are_common_##range_concept##_v =                         \
    (std::ranges::range_concept<Ranges> && ...)                           \
      && (std::ranges::common_range<Ranges> && ...);                      \
                                                                          \
  template <class T, class... Ranges>                                     \
  constexpr auto are_common_##range_concept##_of_v =                      \
    (common_##range_concept##_of<Ranges, T> && ...);                      \
                                                                          \
  template <class T, class... Ranges>                                     \
  constexpr auto are_common_##range_concept##_of_exactly_v =              \
    (common_##range_concept##_of_exactly<Ranges, T> && ...);

REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(REFLECT_CPP26_DEFINE_RANGE_CONCEPTS)

#undef REFLECT_CPP26_DEFINE_RANGE_CONCEPTS
#undef REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH
#undef REFLECT_CPP26_CONST_RANGE_CONCEPTS_FOR_EACH

template <class T, class... Ranges>
constexpr auto are_output_range_v =
  (std::ranges::output_range<Ranges, T> && ...);
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_RANGES_HPP
