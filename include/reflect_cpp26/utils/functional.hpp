#ifndef REFLECT_CPP26_UTILS_FUNCTIONAL_HPP
#define REFLECT_CPP26_UTILS_FUNCTIONAL_HPP

#include <functional>

namespace reflect_cpp26 {
template <auto Fn>
struct to_functor_t {
  template <class... Args>
  static constexpr auto operator()(Args&&... args) {
    return std::invoke(Fn, std::forward<Args>(args)...);
  }
};
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_FUNCTIONAL_HPP
