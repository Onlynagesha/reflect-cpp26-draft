#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_entries.hpp>
#endif

using namespace reflect_cpp26;

template <class E>
constexpr auto enum_entry_by_value(size_t index) {
  return enum_entry<E, enum_entry_order::by_value>(index);
}

template <class E>
constexpr auto enum_entry_by_name(size_t index) {
  return enum_entry<E, enum_entry_order::by_name>(index);
}

TEST(EnumEntries, All)
{
  EXPECT_EQ_STATIC(
    std::pair(foo_signed_rep::yi, "yi"),
    enum_entry<foo_signed_rep>(3));
  EXPECT_EQ_STATIC(
    std::pair(foo_signed_rep::one, "one"),
    enum_entry<const volatile foo_signed_rep>(1));
  EXPECT_EQ_STATIC(
    std::pair(foo_signed_rep::four, "four"),
    enum_entry_by_value<const foo_signed_rep>(7));
  EXPECT_EQ_STATIC(
    std::pair(foo_signed_rep::yi, "yi"),
    enum_entry_by_name<volatile foo_signed_rep>(9));
}
