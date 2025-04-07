#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits.hpp>
#include <reflect_cpp26/utils/constant.hpp>
#include <reflect_cpp26/utils/meta_tuple.hpp>

namespace rfl = reflect_cpp26;

// Non-tuple types
static_assert(! rfl::is_tuple_like_v<void>);
static_assert(! rfl::is_tuple_like_v<int>);
static_assert(! rfl::is_tuple_like_v<std::vector<int>>);
static_assert(! rfl::is_tuple_like_v<std::string>);

using std_tuple = std::tuple<int, int&, int**>;
static_assert(rfl::is_tuple_like_v<std_tuple>);
using std_pair = std::pair<std::string&&, size_t&>;
static_assert(rfl::is_tuple_like_v<const std_pair>);
using rfl_constant = rfl::constant<1, 2u, '3', 4.0>;
static_assert(rfl::is_tuple_like_v<volatile rfl_constant&>);
using rfl_meta_tuple = rfl::meta_tuple<char, char*, char**>;
static_assert(rfl::is_tuple_like_v<const volatile rfl_meta_tuple&&>);

using nested_std_tuple_1 = std::tuple<
  std::pair<float, std::pair<int, unsigned>>,
  std::tuple<char, std::tuple<short, int, std::tuple<float, double>>>>;
static_assert(rfl::is_tuple_like_v<nested_std_tuple_1>);

using nested_std_tuple_2 = std::tuple<
  rfl::meta_tuple<char8_t, char16_t, char32_t>>;
static_assert(rfl::is_tuple_like_v<const nested_std_tuple_2>);

using nested_meta_tuple = rfl::meta_tuple<
  rfl::meta_tuple<char, short, int, long>,
  rfl::meta_tuple<float, double, rfl::meta_tuple<float, double>>>;
static_assert(rfl::is_tuple_like_v<volatile nested_meta_tuple&&>);

static_assert(rfl::is_tuple_like_of_elements_v<
  std_tuple, int, int&, int**>);
static_assert(rfl::is_tuple_like_of_elements_v<
  const std_pair, std::string&&, size_t&>);
static_assert(rfl::is_tuple_like_of_elements_v<
  volatile rfl_constant, int, unsigned, char, double>);
static_assert(rfl::is_tuple_like_of_elements_v<
  const volatile rfl_meta_tuple&, char, char*, char**>);
// Expects exact match
static_assert(! rfl::is_tuple_like_of_elements_v<
  std_pair, std::string, size_t>);
static_assert(! rfl::is_tuple_like_of_elements_v<
  rfl_constant, int, unsigned, int, double>);
static_assert(! rfl::is_tuple_like_of_elements_v<
  rfl_meta_tuple, char, char const*, char* const*>);
static_assert(! rfl::is_tuple_like_of_elements_v<
  std_pair, std::string_view, unsigned>);

// Exact match or conversion
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  std_tuple, int, int&, int**>);
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  const std_pair, std::string, size_t>);
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  volatile rfl_constant, int, unsigned, int, double>);
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  const volatile rfl_meta_tuple&, char, char const*, char* const*>);
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  std_pair, std::string_view, unsigned>);
// std::string -> std::string_view is OK, but inverse is not.
static_assert(rfl::is_tuple_like_with_elements_convertible_to_v<
  std::tuple<std::string>, std::string_view>);
static_assert(! rfl::is_tuple_like_with_elements_convertible_to_v<
  std::tuple<std::string_view>, std::string>);

// Unary predicate for-each
static_assert(rfl::all_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int*, const int**, volatile int***, void (*)(int, int, int)>>);
static_assert(rfl::any_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int, const int&, volatile int*>>);
static_assert(rfl::none_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int, const int*&, volatile int*&>>);
static_assert(! rfl::all_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int*, const int**, volatile long, void (*)(int, int, int)>>);
static_assert(! rfl::any_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int, const int&, volatile int*&>>);
static_assert(! rfl::none_of_tuple_elementwise_v<std::is_pointer,
  std::tuple<int, const int*, volatile int*&>>);

// Binary predicate for-each (tuple sizes may be different)
static_assert(rfl::all_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<bool, void*>>);
static_assert(rfl::any_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<void*, int**, std::string_view, std::nullptr_t>>);
static_assert(rfl::none_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<void*, int**, std::u32string_view>>);
static_assert(! rfl::all_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<bool, void**>>);
static_assert(! rfl::any_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<void*, int**, std::u16string_view, std::nullptr_t>>);
static_assert(! rfl::none_of_tuple_elementwise_v<std::is_convertible,
  std::tuple<int, int*, std::string>,
  std::tuple<void*, const int*, std::u32string_view>>);

static_assert(rfl::are_tuple_like_of_same_size_v<
  std::tuple<int, int*, int**>,
  rfl::meta_tuple<float, double, long>,
  rfl::constant<1, '2', 3.0>>);

// zip-substitute
static_assert(std::is_same_v<
  rfl::tuple_elementwise_zip_substitute_t<rfl::meta_tuple,
    std::tuple<short, int, long, double>,
    std::tuple<short*, int**, long***, double****>,
    std::tuple<const char*, const wchar_t*, const char16_t*, const char32_t*>>,
  rfl::type_tuple<
    rfl::meta_tuple<short, short*, const char*>,
    rfl::meta_tuple<int, int**, const wchar_t*>,
    rfl::meta_tuple<long, long***, const char16_t*>,
    rfl::meta_tuple<double, double****, const char32_t*>>>);

// zip-transform
static_assert(std::is_same_v<
  rfl::tuple_elementwise_zip_transform_t<std::common_type,
    std::tuple<std::strong_ordering, int, const void*>,
    std::tuple<std::weak_ordering, long, const int*>,
    std::tuple<std::partial_ordering, short, char**>>,
  rfl::type_tuple<std::partial_ordering, long, const void*>>);

template <class... Args>
struct identifier_length_product {
  static constexpr auto value = (identifier_of(^^Args).size() * ...);
};
static_assert(identifier_length_product<
  std::monostate>::value == 9, "Incorrect test case.");
static_assert(identifier_length_product<
  std::monostate, std::errc>::value == 36, "Incorrect test case.");

static_assert(std::is_same_v<
  decltype(rfl::tuple_elementwise_zip_transform_v<
    identifier_length_product,
    std::tuple<std::strong_ordering, std::weak_ordering, std::partial_ordering>,
    std::tuple<std::errc, std::to_chars_result>>),
  const rfl::constant<
    15zU * 4zU, // len("strong_ordering") * len("errc")
    13zU * 15zU // len("weak_ordering") * len("to_chars_result")
  >>);

TEST(TypeTraitsClassTypes, IsTupleLike) {
  EXPECT_TRUE(true); // All test cases done with static-asserts above
}
