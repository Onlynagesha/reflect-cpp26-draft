#ifndef REFLECT_CPP26_UTILS_META_STRING_VIEW_HPP
#define REFLECT_CPP26_UTILS_META_STRING_VIEW_HPP

#include <algorithm>
#include <compare>
#include <format>
#include <iterator>
#include <string>
#include <string_view>

namespace reflect_cpp26 {
/**
 * Structured alternative to std::basic_string_view<CharT>.
 * Semantic constraints: meta_basic_string_view<CharT> is used for strings
 * with static constant storage only.
 */
template <class CharT>
struct meta_basic_string_view {
  using value_type = CharT;
  using pointer = CharT*;
  using const_pointer = const CharT*;
  using reference = CharT&;
  using const_reference = const CharT&;
  using iterator = const_pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<const_iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

private:
  // Note: Customized char traits is disabled to prevent ambiguity
  // with sementics of different traits.
  template <class Alloc>
  using std_string_of_alloc =
    std::basic_string<CharT, std::char_traits<CharT>, Alloc>;

public:
  const CharT* head;
  const CharT* tail;

  consteval meta_basic_string_view():
    head(nullptr), tail(nullptr) {}

  template <size_t N>
  consteval meta_basic_string_view(const CharT (&literal)[N])
    : head(literal), tail(literal + N - 1) {}

  consteval meta_basic_string_view(const CharT* literal)
    : head(literal)
    , tail(std::ranges::find(literal, std::unreachable_sentinel, '\0')) {}

  consteval meta_basic_string_view(const CharT* head, const CharT* tail)
    : head(head), tail(tail) {}

  consteval meta_basic_string_view(const CharT* head, size_t length)
    : head(head), tail(head + length) {}

  consteval meta_basic_string_view(std::basic_string_view<CharT> sv)
    : head(sv.data()), tail(sv.data() + sv.length()) {}

  constexpr operator std::basic_string_view<CharT>() const {
    return {head, tail};
  }

  constexpr auto operator[](size_t index) const -> CharT {
    return head[index];
  }

  constexpr auto front() const -> CharT {
    return head[0];
  }

  constexpr auto back() const -> CharT {
    return tail[-1];
  }

  constexpr auto size() const -> size_t {
    return tail - head;
  }

  constexpr auto empty() const -> bool {
    return head == tail;
  }

  constexpr auto data() const -> const CharT* {
    return head;
  }

  constexpr auto begin() const -> const CharT* {
    return head;
  }

  constexpr auto end() const -> const CharT* {
    return tail;
  }

  // Ambiguity of overload resolution without the following constraints:
  // (1) meta_string_view::operator<=>(
  //       std::string_view implicitly converted to meta_string_view,
  //       meta_string_view)
  // (2) std::string_view::operator<=>(
  //       meta_string_view implicitly converted to std::string_view,
  //       std::string_view)

  template <class RhsType>
    requires (std::is_same_v<meta_basic_string_view<CharT>, RhsType>)
  constexpr auto operator<=>(RhsType rhs) const
    -> std::strong_ordering
  {
    return std::lexicographical_compare_three_way(
      head, tail, rhs.head, rhs.tail);
  }

  template <class RhsType>
    requires (std::is_same_v<meta_basic_string_view<CharT>, RhsType>)
  constexpr bool operator==(RhsType rhs) const {
    return std::ranges::equal(head, tail, rhs.head, rhs.tail);
  }

  template <class Alloc>
  constexpr auto operator<=>(const std_string_of_alloc<Alloc>& rhs) const
    -> std::strong_ordering
  {
    return std::lexicographical_compare_three_way(
      head, tail, rhs.begin(), rhs.end());
  }

  template <class Alloc>
  constexpr bool operator==(const std_string_of_alloc<Alloc>& rhs) const {
    return std::ranges::equal(head, tail, rhs.begin(), rhs.end());
  }

  constexpr auto operator<=>(const CharT* rhs) const -> std::strong_ordering
  {
    const auto* rhs_tail = rhs;
    for (; *rhs_tail != '\0'; ++rhs_tail) {}
    return std::lexicographical_compare_three_way(head, tail, rhs, rhs_tail);
  }

  constexpr bool operator==(const CharT* rhs) const
  {
    const auto* rhs_tail = rhs;
    for (; *rhs_tail != '\0'; ++rhs_tail) {}
    return std::ranges::equal(head, tail, rhs, rhs_tail);
  }
};

using meta_string_view = meta_basic_string_view<char>;
using meta_u8string_view = meta_basic_string_view<char8_t>;

} // namespace reflect_cpp26

template <class CharT>
struct std::formatter<reflect_cpp26::meta_basic_string_view<CharT>, CharT>
  : std::formatter<std::basic_string_view<CharT>, CharT>
{
  using self_type = reflect_cpp26::meta_basic_string_view<CharT>;

  template <class FormatContext>
  auto format(self_type str, FormatContext& ctx) const
    -> FormatContext::iterator
  {
    return std::ranges::copy(str, ctx.out()).out;
  }
};

#endif // REFLECT_CPP26_UTILS_META_STRING_VIEW_HPP
