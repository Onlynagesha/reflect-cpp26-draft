#ifndef REFLECT_CPP26_UTILS_META_SPAN_HPP
#define REFLECT_CPP26_UTILS_META_SPAN_HPP

#include <cstddef>
#include <iterator>
#include <span>

namespace reflect_cpp26 {
namespace impl {
struct meta_subspan_tag_t {};
constexpr auto meta_subspan_tag = meta_subspan_tag_t{};
} // namespace impl

/**
 * Structured alternative to std::span<const T>.
 * Semantic constraints: meta_span<T> is used for contiguous ranges
 * with static constant storage only.
 */
template <class T>
struct meta_span {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using iterator = const_pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<const_iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  const T* head;
  const T* tail;

  consteval meta_span()
    : head(nullptr), tail(nullptr) {}

  consteval meta_span(const T* head, const T* tail)
    : head(head), tail(tail) {}

  consteval meta_span(const T* head, size_t length)
    : head(head), tail(head + length) {}

  consteval meta_span(std::span<const T> span)
    : head(span.data()), tail(span.data() + span.size()) {}

private:
  // Note: making subspan is not forced to be consteval.
  constexpr meta_span(impl::meta_subspan_tag_t, const T* head, const T* tail)
    : head(head), tail(tail) {}

public:
  constexpr operator std::span<const T>() const {
    return {head, tail};
  }

  constexpr auto operator[](size_t index) const -> const T& {
    return head[index];
  }

  constexpr auto front() const -> const T& {
    return head[0];
  }

  constexpr auto back() const -> const T& {
    return tail[-1];
  }

  constexpr auto size() const -> size_t {
    return tail - head;
  }

  constexpr auto empty() const -> bool {
    return tail == head;
  }

  constexpr auto data() const -> const T* {
    return head;
  }

  constexpr auto begin() const -> const T* {
    return head;
  }

  constexpr auto end() const -> const T* {
    return tail;
  }

  constexpr auto first(size_t n) const -> meta_span {
    return {impl::meta_subspan_tag, head, head + n};
  }

  constexpr auto last(size_t n) const -> meta_span {
    return {impl::meta_subspan_tag, tail - n, tail};
  }

  constexpr auto subspan(
    size_t offset, size_t count = std::dynamic_extent) const -> meta_span
  {
    if (count == std::dynamic_extent) {
      return {impl::meta_subspan_tag, head + offset, tail};
    }
    return {impl::meta_subspan_tag, head + offset, head + offset + count};
  }
};
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_UTILS_META_SPAN_HPP
