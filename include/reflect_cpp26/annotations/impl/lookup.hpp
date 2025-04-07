#ifndef REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP
#define REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP

#include <reflect_cpp26/utils/config.h>

namespace reflect_cpp26::annotations::impl {
consteval auto find_annotation_of_type(
  std::meta::info annotation_type, std::meta::info target) -> std::meta::info
{
  auto res = std::meta::info{};
  for (auto cur: annotations_of(target)) {
    if (!is_same_type(type_of(cur), annotation_type)) {
      continue;
    }
    if (std::meta::info{} != res) {
      compile_error("Duplicated annotation type disallowed.");
    }
    res = cur;
  }
  return res;
}
} // namespace reflect_cpp26::annotations::impl

#endif // REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP
