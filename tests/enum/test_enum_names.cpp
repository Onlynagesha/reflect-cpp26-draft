#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_names.hpp>
#endif

using namespace reflect_cpp26;

TEST(EnumNames, All)
{
  EXPECT_EQ_STATIC("yi", enum_names<foo_signed_rep>()[3]);
  EXPECT_EQ_STATIC("four",
    enum_names<const foo_signed_rep, enum_entry_order::by_value>()[7]);
  EXPECT_EQ_STATIC("yi",
    enum_names<volatile foo_signed_rep, enum_entry_order::by_name>()[9]);
}
