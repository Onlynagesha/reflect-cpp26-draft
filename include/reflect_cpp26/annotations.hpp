#ifndef REFLECT_CPP26_ANNOTATIONS_HPP
#define REFLECT_CPP26_ANNOTATIONS_HPP

#include <reflect_cpp26/annotations/predicates.hpp>
#include <reflect_cpp26/annotations/properties.hpp>
#include <reflect_cpp26/utils/preprocessors.h>

namespace reflect_cpp26::annotations {
struct assignment_entry_t
  : predicates_assignment_entry_t
  , properties_assignment_entry_t {};

constexpr auto assignment_entry = assignment_entry_t{};
} // namespace reflect_cpp26::annotations

#define REFLECT_CPP26_ANNOTATE(...) \
  [[ =(::reflect_cpp26::annotations::assignment_entry. __VA_ARGS__) ]]

#define REFLECT_CPP26_ANNOTATIONS(...) \
  REFLECT_CPP26_MACRO_FOREACH(REFLECT_CPP26_ANNOTATE, ##__VA_ARGS__)

#endif // REFLECT_CPP26_ANNOTATIONS_HPP
