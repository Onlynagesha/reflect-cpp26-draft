#ifndef REFLECT_CPP26_TYPE_TRAITS_TYPE_COMPARISON_HPP
#define REFLECT_CPP26_TYPE_TRAITS_TYPE_COMPARISON_HPP

#include <type_traits>

namespace reflect_cpp26 {
/**
 * Whether T is exactly the same with one of Args.
 */
template <class T, class... Args>
constexpr bool is_same_as_one_of_v = (std::is_same_v<T, Args> || ...);

template <class T, class... Args>
concept same_as_one_of = is_same_as_one_of_v<T, Args...>;

template <class T, class... Args>
concept same_as_none_of = !is_same_as_one_of_v<T, Args...>;

/**
 * Whether T and Args... are all the exactly same.
 */
template <class T, class... Args>
constexpr bool are_all_same_v = (std::is_same_v<T, Args> && ...);
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_TYPE_COMPARISON_HPP
