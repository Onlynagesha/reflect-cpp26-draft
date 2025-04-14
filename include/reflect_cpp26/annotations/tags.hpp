#ifndef REFLECT_CPP26_ANNOTATIONS_TAGS_HPP
#define REFLECT_CPP26_ANNOTATIONS_TAGS_HPP

#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <concepts>

namespace reflect_cpp26::annotations {
struct property_tag_t {};
struct validator_tag_t {};

template <class Annotation>
constexpr auto is_property_v = std::derived_from<Annotation, property_tag_t>;

template <class Annotation>
constexpr auto is_validator_v = std::derived_from<Annotation, validator_tag_t>;

template <class Annotation>
concept property = is_property_v<Annotation>;

template <class Annotation>
concept validator = is_validator_v<Annotation>;

namespace impl {
struct is_property_fn_t {
  template <std::meta::info M>
  static consteval bool operator()(constant<M>) {
    return is_property_v<[:type_of(M):]>;
  }
};

struct is_validator_fn_t {
  template <std::meta::info M>
  static consteval bool operator()(constant<M>) {
    return is_validator_v<[:type_of(M):]>;
  }
};

constexpr auto is_property_fn = is_property_fn_t{};
constexpr auto is_validator_fn = is_validator_fn_t{};
} // namespace impl

template <std::meta::info M>
constexpr auto properties_of_meta_v = REFLECT_CPP26_EXPAND(annotations_of(M))
  .template filter<impl::is_property_fn>();

template <std::meta::info M>
constexpr auto validators_of_meta_v = REFLECT_CPP26_EXPAND(annotations_of(M))
  .template filter<impl::is_validator_fn>();
} // namespace reflect_cpp26::annotations

#endif // REFLECT_CPP26_ANNOTATIONS_TAGS_HPP
