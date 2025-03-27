#ifndef REFLECT_CPP26_TYPE_TRAITS_REDUCTION_HPP
#define REFLECT_CPP26_TYPE_TRAITS_REDUCTION_HPP

namespace reflect_cpp26 {
/**
 * Whether every UnaryPredicate<A>::value is true for each A in Args.
 */
template <template <class> class UnaryPredicate, class... Args>
constexpr bool all_of_v = (UnaryPredicate<Args>::value && ...);

/**
 * Whether at least one of UnaryPredicate<A>::value is true for each A in Args.
 */
template <template <class> class UnaryPredicate, class... Args>
constexpr bool any_of_v = (UnaryPredicate<Args>::value || ...);

/**
 * Whether none of UnaryPredicate<A>::value is true for each A in Args.
 */
template <template <class> class UnaryPredicate, class... Args>
constexpr bool none_of_v = !any_of_v<UnaryPredicate, Args...>;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_REDUCTION_HPP
