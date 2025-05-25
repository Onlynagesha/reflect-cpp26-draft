#ifndef REFLECT_CPP26_ENUM_ENUM_COUNT_HPP
#define REFLECT_CPP26_ENUM_ENUM_COUNT_HPP

#include <reflect_cpp26/utils/concepts.hpp>
#include <reflect_cpp26/utils/config.h>
#include <ranges>
#include <utility>

namespace reflect_cpp26 {
template <class E>
constexpr auto enum_count_v = enumerators_of(^^E).size();

template <class E>
consteval auto enum_unique_count_impl() -> size_t
{
  auto values = enumerators_of(^^E)
    | std::views::transform([](std::meta::info e) {
        return std::to_underlying(extract<E>(e));
      })
    | std::ranges::to<std::vector>();
  std::ranges::sort(values);
  auto [pos, _] = std::ranges::unique(values);
  return pos - values.begin();
}

template <class E>
constexpr auto enum_unique_count_v = enum_unique_count_impl<E>();

/**
 * Gets the number of entries of enum type E.
 */
template <enum_type E>
constexpr auto enum_count() -> size_t {
  return enum_count_v<std::remove_cv_t<E>>;
}

/**
 * Gets the number of entries of enum type E with unique value.
 */
template <enum_type E>
constexpr auto enum_unique_count() -> size_t {
  return enum_unique_count_v<std::remove_cv_t<E>>;
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_ENUM_ENUM_COUNT_HPP
