#ifndef REFLECT_CPP26_ANNOTATIONS_TAGS_HPP
#define REFLECT_CPP26_ANNOTATIONS_TAGS_HPP

#include <concepts>

namespace reflect_cpp26::annotations {
struct property_tag_t {};
struct predicate_tag_t {};

template <class Annotation>
constexpr auto is_property_v = std::derived_from<Annotation, property_tag_t>;

template <class Annotation>
constexpr auto is_predicate_v = std::derived_from<Annotation, predicate_tag_t>;
} // namespace reflect_cpp26::annotations

#endif // REFLECT_CPP26_ANNOTATIONS_TAGS_HPP
