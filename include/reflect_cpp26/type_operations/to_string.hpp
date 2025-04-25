#ifndef REFLECT_CPP26_TYPE_OPERATIONS_TO_STRING_HPP
#define REFLECT_CPP26_TYPE_OPERATIONS_TO_STRING_HPP

#include <reflect_cpp26/enum/enum_names.hpp>
#include <reflect_cpp26/enum/enum_type_name.hpp>
#include <reflect_cpp26/type_traits/tuple_like_types.hpp>
#include <reflect_cpp26/utils/meta_tuple.hpp>
#include <reflect_cpp26/utils/to_string.hpp>

namespace reflect_cpp26 {
template <class T>
concept has_to_string = requires (const T& t) {
  { to_string(t) } -> std::convertible_to<std::string>;
};

template <class T>
concept has_to_display_string = requires (const T& t) {
  { to_display_string(t) } -> std::convertible_to<std::string>;
};

namespace impl {
template <class T>
consteval bool is_generic_to_string_invocable();
} // namespace impl

template <class T>
constexpr auto is_generic_to_string_invocable_v =
  impl::is_generic_to_string_invocable<T>();

template <class T>
concept generic_to_string_invocable = is_generic_to_string_invocable_v<T>;

namespace impl {
template <class T>
consteval bool is_generic_to_string_invocable()
{
  if constexpr (has_to_string<T> || std::is_enum_v<T>) {
    return true;
  } else if constexpr (std::ranges::input_range<T>) {
    using V = std::ranges::range_value_t<T>;
    return is_generic_to_string_invocable_v<V>;
  } else if constexpr (is_tuple_like_v<T>) {
    constexpr auto N = std::tuple_size_v<T>;
    auto res = true;
    REFLECT_CPP26_EXPAND_I(N).for_each([&res](auto I) {
      using V = std::tuple_element_t<I, T>;
      return res &= is_generic_to_string_invocable_v<V>;
    });
    return res;
  } else {
    return false;
  }
}

template <class T>
constexpr auto generic_enum_to_string(T input) -> std::string
{
  auto sv = enum_name(input);
  if (!sv.empty()) {
    return std::string{sv};
  }
  auto res = std::string{"("} + enum_type_name<T>() + ')';
  res += to_string(std::to_underlying(input));
  return res;
}

template <class ToStringFn, class T>
constexpr auto generic_range_to_string(const T& input) -> std::string
{
  auto res = std::string{"["};
  auto index = 0zU;
  for (const auto& cur: input) {
    if (index++ != 0) {
      res += ", ";
    }
    res += ToStringFn::operator()(cur);
  }
  res += ']';
  return res;
}

template <class ToStringFn, class T>
constexpr auto generic_tuple_like_to_string(const T& input) -> std::string
{
  constexpr auto N = std::tuple_size_v<T>;
  auto res = std::string{"{"};
  REFLECT_CPP26_EXPAND_I(N).for_each([&res, &input](auto I) {
    if constexpr (I != 0) {
      res += ", ";
    }
    res += ToStringFn::operator()(tuple_get<I>(input));
  });
  res += '}';
  return res;
}

template <class ToStringFn, class T>
constexpr auto generic_to_string(const T& input) -> std::string
{
  if constexpr (has_to_string<T>) {
    return to_string(input);
  } else if constexpr (std::is_enum_v<T>) {
    return generic_enum_to_string(input);
  } else if constexpr (std::ranges::input_range<T>) {
    return generic_range_to_string<ToStringFn>(input);
  } else if constexpr (is_tuple_like_v<T>) {
    return generic_tuple_like_to_string<ToStringFn>(input);
  } else {
    static_assert(false, "Invalid type.");
  }
}
} // namespace impl

struct generic_to_display_string_t {
  using self_type = generic_to_display_string_t;

  template <generic_to_string_invocable T>
  static constexpr auto operator()(const T& input) -> std::string
  {
    if constexpr (has_to_display_string<T>) {
      return to_display_string(input);
    } else {
      return impl::generic_to_string<self_type>(input);
    }
  }

  template <class T>
  static constexpr auto operator()(const T& input, std::string_view alt)
    -> std::string
  {
    if constexpr (is_generic_to_string_invocable_v<T>) {
      return operator()(input);
    } else {
      return std::string{alt};
    }
  }
};

struct generic_to_string_t {
  using self_type = generic_to_string_t;

  template <generic_to_string_invocable T>
  static constexpr auto operator()(const T& input) -> std::string {
    return impl::generic_to_string<self_type>(input);
  }

  template <generic_to_string_invocable T>
  static constexpr auto operator()(const T& input, bool displayed_style)
    -> std::string
  {
    return displayed_style
      ? generic_to_display_string_t::operator()(input)
      : generic_to_string_t::operator()(input);
  }

  template <class T>
  static constexpr auto operator()(const T& input, std::string_view alt)
    -> std::string
  {
    if constexpr (is_generic_to_string_invocable_v<T>) {
      return operator()(input);
    } else {
      return std::string{alt};
    }
  }

  template <class T>
  static constexpr auto operator()(
    const T& input, bool displayed_style, std::string_view alt) -> std::string
  {
    if constexpr (is_generic_to_string_invocable_v<T>) {
      return operator()(input, displayed_style);
    } else {
      return std::string{alt};
    }
  }
};

constexpr auto generic_to_string = generic_to_string_t{};
constexpr auto generic_to_display_string = generic_to_display_string_t{};
};

#endif // REFLECT_CPP26_TYPE_OPERATIONS_TO_STRING_HPP
