#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits/class_types.hpp>
#include <system_error>

namespace rfl = reflect_cpp26;

TEST(TypeTraitsClassTypes, IsFlattenableArithmetic)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<int>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const long>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<volatile float>);

  EXPECT_TRUE(rfl::is_flattenable_v<int>);
  EXPECT_TRUE(rfl::is_flattenable_v<const long>);
  EXPECT_TRUE(rfl::is_flattenable_v<volatile float>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<int>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const long>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<volatile float>);
}

TEST(TypeTraitsClassTypes, IsFlattenableEnum)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<std::byte>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<std::errc>);

  EXPECT_TRUE(rfl::is_flattenable_v<std::byte>);
  EXPECT_TRUE(rfl::is_flattenable_v<std::errc>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::byte>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::errc>);
}

TEST(TypeTraitsClassTypes, IsFlattenablePointer)
{
  using std_pair = std::pair<double, double>;
  using std_vector = std::vector<double>;
  EXPECT_TRUE(rfl::is_partially_flattenable_v<char*>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const long* volatile>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<std::nullptr_t>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<double std_pair::*>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<void (std_vector::*)(size_t)>);

  EXPECT_TRUE(rfl::is_flattenable_v<char*>);
  EXPECT_TRUE(rfl::is_flattenable_v<const long* volatile>);
  EXPECT_TRUE(rfl::is_flattenable_v<std::nullptr_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<double std_pair::*>);
  EXPECT_TRUE(rfl::is_flattenable_v<void (std_vector::*)(size_t)>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<char*>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const long* volatile>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::nullptr_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<double std_pair::*>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<void (std_vector::*)(size_t)>);
}

TEST(TypeTraitsClassTypes, IsFlattenableReference)
{
  using std_pair = std::pair<int, int>;
  EXPECT_FALSE(rfl::is_partially_flattenable_v<int&>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<const std_pair&>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<float&&>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<const std_pair&&>);

  EXPECT_FALSE(rfl::is_flattenable_v<int&>);
  EXPECT_FALSE(rfl::is_flattenable_v<const std_pair&>);
  EXPECT_FALSE(rfl::is_flattenable_v<float&&>);
  EXPECT_FALSE(rfl::is_flattenable_v<const std_pair&&>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<int&>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const std_pair&>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<float&&>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const std_pair&&>);
}

TEST(TypeTraitsClassTypes, IsFlattenableArray)
{
  using std_vector = std::vector<double>;
  EXPECT_TRUE(rfl::is_partially_flattenable_v<long[]>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<double[16]>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<volatile std_vector[]>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<std_vector[32]>);

  EXPECT_TRUE(rfl::is_flattenable_v<long[]>);
  EXPECT_TRUE(rfl::is_flattenable_v<double[16]>);
  EXPECT_TRUE(rfl::is_flattenable_v<volatile std_vector[]>);
  EXPECT_TRUE(rfl::is_flattenable_v<std_vector[32]>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<long[]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<double[16]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<volatile std_vector[]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std_vector[32]>);
}

union some_union_t {
  int as_int;
  float as_float;
  const char* as_char_literal;
};

TEST(TypeTraitsClassTypes, IsFlattenableUnion)
{
  EXPECT_FALSE(rfl::is_partially_flattenable_v<some_union_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<some_union_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<some_union_t>);
}

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesBasic)
{
  using std_pair = std::pair<std::string, std::string>;
  using std_vector = std::vector<std::string>;
  EXPECT_TRUE(rfl::is_partially_flattenable_v<std::monostate>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const std::to_chars_result>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<volatile std_pair>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const volatile std_vector>);

  EXPECT_TRUE(rfl::is_flattenable_v<std::monostate>);
  EXPECT_TRUE(rfl::is_flattenable_v<const std::to_chars_result>);
  EXPECT_TRUE(rfl::is_flattenable_v<volatile std_pair>);
  EXPECT_FALSE(rfl::is_flattenable_v<const volatile std_vector>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std::monostate>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const std::to_chars_result>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<volatile std_pair>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const volatile std_vector>);
}

  // static_assert(std::is_aggregate_v<struct_not_destructible_t>,
  //               "Incorrect test case.");
  // EXPECT_TRUE(rfl::is_flattenable_v<foo_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<bar_with_cv_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<with_non_aggregate_members_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<with_private_members_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<std::string>);
  // EXPECT_FALSE(rfl::is_flattenable_v<std::string_view>);

  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<foo_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<bar_with_cv_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<with_non_aggregate_members_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<with_private_members_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::string>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::string_view>);

  // EXPECT_TRUE(rfl::is_flattenable_v<with_user_declared_ctor_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<struct_not_destructible_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<struct_not_trivially_destructible_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<derived_structured_1_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<derived_structured_2_t>);
  // // User-declared ctors and dtors are disallowed for aggregates
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<with_user_declared_ctor_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<struct_not_destructible_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
  //   struct_not_trivially_destructible_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_structured_1_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_structured_2_t>);

  // // cv-qualified
  // EXPECT_TRUE(rfl::is_flattenable_v<const foo_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<const volatile foo_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const foo_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const volatile foo_t>);

  // // Empty
  // EXPECT_TRUE(rfl::is_flattenable_v<rfl::constant<42>>);
  // EXPECT_TRUE(rfl::is_flattenable_v<std::monostate>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<rfl::constant<42>>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std::monostate>);

  // // Arrays and array-like
  // using std_array_baz_union_t_4 = std::array<baz_union_t, 4>;
  // EXPECT_TRUE(rfl::is_flattenable_v<char[32]>);
  // EXPECT_TRUE(rfl::is_flattenable_v<baz_union_t[32]>);
  // EXPECT_TRUE(rfl::is_flattenable_v<char[]>);
  // EXPECT_TRUE(rfl::is_flattenable_v<baz_union_t[]>);
  // EXPECT_TRUE(rfl::is_flattenable_v<std_array_baz_union_t_4>);

  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<char[32]>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<baz_union_t[32]>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<char[]>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<baz_union_t[]>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std_array_baz_union_t_4>);

  // // Union members are disallowed
  // EXPECT_FALSE(rfl::is_flattenable_v<baz_struct_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_struct_t>);

  // // Virtual members
  // EXPECT_TRUE(rfl::is_flattenable_v<with_virtual_member_function_1_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<with_virtual_member_function_2_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<derived_1_with_virtual_mem_fn_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
  //   with_virtual_member_function_1_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
  //   with_virtual_member_function_2_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
  //   derived_1_with_virtual_mem_fn_t>);

  // // Inheritance
  // static_assert(std::is_aggregate_v<derived_1_priv_in_base_t>,
  //   "Incorrect test case.");
  // static_assert(std::is_aggregate_v<derived_2_priv_in_base_t>,
  //   "Incorrect test case.");
  // EXPECT_TRUE(rfl::is_flattenable_v<derived_1_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_1_prot_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_1_priv_in_base_t>);
  // EXPECT_TRUE(rfl::is_flattenable_v<derived_2_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_2_priv_in_base_t>);

  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<derived_1_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_1_prot_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_1_priv_in_base_t>);
  // EXPECT_TRUE(rfl::is_flattenable_aggregate_v<derived_2_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_2_priv_in_base_t>);

  // // Virtual inheritance is disallowed.
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_1a_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_1b_t>);
  // EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_2_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_1a_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_1b_t>);
  // EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_2_t>);
