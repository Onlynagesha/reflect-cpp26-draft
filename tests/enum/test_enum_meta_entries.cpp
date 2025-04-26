#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_meta_entries.hpp>
#endif

using namespace reflect_cpp26;

template <auto B>
struct some_type_t {
  static constexpr auto value =
    std::to_underlying(B) * std::to_underlying(B) + 2000;
};

TEST(EnumMetaEntries, All)
{
  EXPECT_EQ_STATIC("yi", std::meta::identifier_of(
    enum_meta_entry<foo_signed_rep>(3)));
  EXPECT_EQ_STATIC("one", std::meta::identifier_of(
    enum_meta_entry<const volatile foo_signed_rep>(1)));
  EXPECT_EQ_STATIC("four", std::meta::identifier_of(
    enum_meta_entry<const foo_signed_rep, enum_entry_order::by_value>(7)));
  EXPECT_EQ_STATIC("yi", std::meta::identifier_of(
    enum_meta_entry<volatile foo_signed_rep, enum_entry_order::by_name>(9)));
  
  EXPECT_EQ_STATIC("three", std::meta::identifier_of(
    enum_meta_entry<bar_unsigned>(3)));
  EXPECT_EQ_STATIC("one", std::meta::identifier_of(
    enum_meta_entry<const volatile bar_unsigned>(1)));
  EXPECT_EQ_STATIC("thirteen", std::meta::identifier_of(
    enum_meta_entry<const bar_unsigned, enum_entry_order::by_value>(7)));
  EXPECT_EQ_STATIC("two", std::meta::identifier_of(
    enum_meta_entry<volatile bar_unsigned, enum_entry_order::by_name>(9)));
  
  using some_type_twelve = some_type_t<
    [:enum_meta_entry<bar_unsigned, enum_entry_order::by_name>(2):]>;
  EXPECT_EQ_STATIC(2196, some_type_twelve::value);
}
