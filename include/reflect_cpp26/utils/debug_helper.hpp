#ifndef REFLECT_CPP26_UTILS_DEBUG_HELPER_HPP
#define REFLECT_CPP26_UTILS_DEBUG_HELPER_HPP

#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/to_string.hpp>

namespace reflect_cpp26 {
template <class T>
constexpr auto type_name_of() -> std::string_view {
  return std::meta::display_string_of(^^T);
}

template <class T>
constexpr auto type_name_of(const T&) -> std::string_view {
  return type_name_of<T>();
}

namespace impl {
template <class T>
consteval auto make_type_description() -> std::string
{
  auto type_name = type_name_of<T>();
  auto location = source_location_of(^^T);

  auto res = std::string{type_name};
  if (location.line() > 0) {
    res += " (defined in ";
    res += location.file_name();
    res += ':';
    res += reflect_cpp26::to_string(location.line());
    res += ')';
  }
  return res;
}

template <class T>
constexpr auto type_description_v =
  reflect_cpp26::define_static_string(make_type_description<T>());
} // namespace impl

template <class T>
constexpr auto type_description_of() -> std::string_view {
  return impl::type_description_v<T>;
}

template <class T>
constexpr auto type_description_of(const T&) -> std::string_view {
  return type_description_of<T>();
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_DEBUG_HELPER_HPP
