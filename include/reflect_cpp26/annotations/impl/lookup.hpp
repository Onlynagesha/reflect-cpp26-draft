#ifndef REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP
#define REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP

namespace reflect_cpp26::annotations {
consteval auto find_annotation_of_type(
  std::meta::info annotation_type, std::meta::info target,
  policy_on_duplication policy = policy_on_duplication::raises_error)
  -> std::meta::info
{
  auto res = std::meta::info{};
  for (auto cur: annotations_of(target)) {
    if (!std::meta::is_same_type(type_of(cur), annotation_type)) {
      continue;
    }
    if (std::meta::info{} == res) {
      res = cur;
      continue;
    }
    switch (policy) {
    case policy_on_duplication::raises_error:
      throw "Duplicated annotation is not allowed.";
    case policy_on_duplication::returns_first:
      goto find_annotation_of_type_return;
    case policy_on_duplication::returns_last:
      res = cur;
      break;
    default:
      throw "Invalid policy_on_duplication value.";
    }
  }
find_annotation_of_type_return:
  return res;
}
} // namespace reflect_cpp26::annotations

#endif // REFLECT_CPP26_ANNOTATIONS_IMPL_LOOKUP_HPP
