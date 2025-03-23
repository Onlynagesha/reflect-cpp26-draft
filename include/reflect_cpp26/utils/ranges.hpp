#ifndef REFLECT_CPP26_UTILS_RANGES_HPP
#define REFLECT_CPP26_UTILS_RANGES_HPP

#include <ranges>
#include <type_traits>

namespace reflect_cpp26 {
// -------- Extension of Ranges concepts --------

#define REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(F) \
  F(range)                \
  F(borrowed_range)       \
  F(sized_range)          \
  F(view)                 \
  F(input_range)          \
  F(output_range)         \
  F(forward_range)        \
  F(bidirectional_range)  \
  F(random_access_range)  \
  F(contiguous_range)     \
  F(common_range)         \
  F(viewable_range)       \
  F(constant_range)

#define REFLECT_CPP26_DEFINE_RANGE_CONCEPTS(range_concept)                \
  template <class Range, class T>                                         \
  concept range_concept##_of = std::ranges::range_concept<Range>          \
    && std::is_convertible_v<std::ranges::range_value_t<Range>, T>;       \
                                                                          \
  template <class Range, class T>                                         \
  concept range_concept##_of_exactly = std::ranges::range_concept<Range>  \
    && std::is_same_v<std::ranges::range_value_t<Range>, T>;

REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH(REFLECT_CPP26_DEFINE_RANGE_CONCEPTS)

#undef REFLECT_CPP26_DEFINE_RANGE_CONCEPTS
#undef REFLECT_CPP26_RANGE_CONCEPTS_FOR_EACH
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_RANGES_HPP
