#ifndef REFLECT_CPP26_TYPE_TRAITS_EXTRACTION_HPP
#define REFLECT_CPP26_TYPE_TRAITS_EXTRACTION_HPP

namespace reflect_cpp26 {
template <class Traits>
struct extract_type {
  using type = typename Traits::type;
};

template <class Traits>
struct extract_value {
  static constexpr auto value = Traits::value;
};
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_TRAITS_EXTRACTION_HPP
