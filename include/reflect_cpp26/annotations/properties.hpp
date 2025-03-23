#ifndef REFLECT_CPP26_ANNOTATIONS_PROPERTIES_HPP
#define REFLECT_CPP26_ANNOTATIONS_PROPERTIES_HPP

#include <reflect_cpp26/annotations/impl/lookup.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/meta_span.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <concepts>
#include <ranges>

namespace reflect_cpp26::annotations {
struct property_tag_t {};

template <class Annotation>
constexpr auto is_property_v = std::derived_from<Annotation, property_tag_t>;

template <class Annotation>
using property_value_type_t = decltype(std::declval<Annotation>().value);

struct aliases_t : property_tag_t {
  meta_span<meta_string_view> value;
};
struct arg_notation_t : property_tag_t {
  char value;
};
struct author_t : property_tag_t {
  meta_string_view value;
};
struct description_t : property_tag_t {
  meta_string_view value;
};
struct rename_t : property_tag_t {
  meta_string_view value;
};
struct version_t : property_tag_t {
  meta_string_view value;
};

template <class Prop>
struct property_assignment_t {
  using value_type = property_value_type_t<Prop>;

  consteval auto operator=(value_type value) const -> Prop {
    return Prop{.value = value};
  }
};

struct alias_assignment_t : property_assignment_t<aliases_t> {
  consteval auto operator=(meta_string_view value) const -> aliases_t
  {
    auto to_span = define_static_array(std::views::single(value));
    return aliases_t{.value = to_span};
  }

  consteval auto operator=(std::initializer_list<meta_string_view> value) const
    -> aliases_t {
    auto to_span = define_static_array(value);
    return aliases_t{.value = to_span};
  }

  template <class Range>
    requires (std::ranges::input_range<Range> &&
      std::is_convertible_v<
        std::ranges::range_value_t<Range>, meta_string_view>)
  consteval auto operator=(const Range& value) const -> aliases_t
  {
    auto to_span = define_static_array(
      value | std::views::transform([](auto str) {
        return meta_string_view{str};
      }));
    return aliases_t{.value = to_span};
  }
};

struct properties_assignment_entry_t {
  [[no_unique_address]]
  alias_assignment_t alias{};

  [[no_unique_address]]
  alias_assignment_t aliases{};

  [[no_unique_address]]
  property_assignment_t<arg_notation_t> arg_notation{};

  [[no_unique_address]]
  property_assignment_t<author_t> author{};

  [[no_unique_address]]
  property_assignment_t<description_t> description{};

  [[no_unique_address]]
  property_assignment_t<rename_t> rename{};

  [[no_unique_address]]
  property_assignment_t<version_t> version{};
};

template <class Prop, auto MemPtr>
consteval auto property_of_alt(property_value_type_t<Prop> alt)
  -> property_value_type_t<Prop>
{
  constexpr auto meta = find_annotation_of_type(
    ^^Prop, reflect_pointer_to_member(MemPtr));
  if constexpr (std::meta::info{} == meta) {
    return alt;
  }
  return extract<Prop>(meta).value;
}

template <class Prop, auto MemPtr>
constexpr auto property_of_opt() -> std::optional<property_value_type_t<Prop>>
{
  constexpr auto meta = find_annotation_of_type(
    ^^Prop, reflect_pointer_to_member(MemPtr));
  if constexpr (std::meta::info{} == meta) {
    return std::nullopt;
  }
  return extract<Prop>(meta).value;
}

template <auto MemPtr>
constexpr auto aliases_of() -> meta_span<meta_string_view> {
  return property_of_alt<aliases_t, MemPtr>({});
}

template <auto MemPtr>
constexpr auto arg_notation_of() -> char {
  return property_of_alt<arg_notation_t, MemPtr>('\0');
}

template <auto MemPtr>
constexpr auto author_of() -> meta_string_view {
  return property_of_alt<author_t, MemPtr>("");
}

template <auto MemPtr>
constexpr auto description_of() -> meta_string_view {
  return property_of_alt<description_t, MemPtr>("");
}

template <auto MemPtr>
constexpr auto rename_of() -> meta_string_view {
  return property_of_alt<rename_t, MemPtr>("");
}

template <auto MemPtr>
constexpr auto version_of() -> meta_string_view {
  return property_of_alt<version_t, MemPtr>("");
}
} // namespace reflect_cpp26::annotations

#endif // REFLECT_CPP26_ANNOTATIONS_PROPERTIES_HPP
