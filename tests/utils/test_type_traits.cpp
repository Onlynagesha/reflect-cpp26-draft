#include <gtest/gtest.h>
#include <reflect_cpp26/utils/config.h>
#include <reflect_cpp26/utils/meta_span.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <reflect_cpp26/utils/to_string.hpp>
#include <reflect_cpp26/utils/type_traits.hpp>
#include <tuple>

namespace rfl = reflect_cpp26;

enum class wrapped_int_t : int {};

struct with_non_aggregate_members_t {
  std::string name;
  std::string_view category;

private:
  void dump() const {
    std::println("name = {}, category = {}", name, category);
  }
};

struct structured_with_private_mem_fn_t {
  long id;
  double timestamp;

private:
void dump() const {
  std::println("id = {}, timestamp = {}", id, timestamp);
}
};

struct with_user_declared_ctor_t {
  char name[20];
  char category[20];

  with_user_declared_ctor_t() = default;
};

struct with_virtual_member_function_1_t {
  std::string name;
  std::string_view category;

  virtual void dump() const {
    std::println("name = {}, category = {}", name, category);
  }
};

struct with_virtual_member_function_2_t {
  std::string name;
  std::string_view category;

  virtual ~with_virtual_member_function_2_t() {
    std::println("name = {}, category = {}", name, category);
  }
};

struct with_private_members_t {
  std::string name;
private:
  std::string_view category;
};

struct derived_1_t : with_non_aggregate_members_t {
  int rating;
  int usage_count;
  std::byte byte_data[256];
};

struct derived_1_priv_in_base_t : with_private_members_t {
  int rating;
  int usage_count;
  std::byte byte_data[256];
};

struct derived_1_prot_t : protected with_non_aggregate_members_t {
  int rating;
  int usage_count;
  std::byte byte_data[256];
};

struct derived_virt_1a_t : virtual structured_with_private_mem_fn_t {
  int xa;
  int ya;
};

struct derived_virt_1b_t 
  : with_user_declared_ctor_t
  , virtual structured_with_private_mem_fn_t 
{
  double xb;
  double yb;
};

struct derived_virt_2_t : foo_t, derived_virt_1a_t, derived_virt_1b_t {
  char comments[20];

  static constexpr auto make() -> derived_virt_2_t
  {
    auto obj = derived_virt_2_t{};
    // direct members
    std::strcpy(obj.comments, "Hello");
    // inherited from foo_t
    obj.index = 12;
    obj.value = 123;
    obj.prev = nullptr;
    // direct members of derived_virt_1a_t
    obj.xa += 1;
    obj.ya += 2;
    // direct members of derived_virt_1b_t
    obj.xb += 1.5;
    obj.yb -= 1.5;
    // inherited from with_user_declared_ctor_t
    std::strcpy(obj.name, "world");
    std::strcpy(obj.category, "from C++");
    // inherited from with with_non_aggregate_members_t (virtual inheritance)
    obj.id = 123456;
    obj.timestamp = 2025.0322;
    return obj;
  }
};

// Tests multiple inheritance
struct derived_2_t : derived_1_t, foo_t {
  std::string comment;
};

struct derived_2_priv_in_base_t : derived_1_priv_in_base_t, foo_t {
  std::string comment;
};

struct derived_1_with_virtual_mem_fn_t : with_virtual_member_function_1_t {
  int derived_x;
};

TEST(UtilsTypeTraits, IsFlattenable)
{
  // Non-class types
  EXPECT_TRUE(rfl::is_flattenable_v<int>);
  EXPECT_TRUE(rfl::is_flattenable_v<wrapped_int_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<baz_union_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<const foo_t*>);
  EXPECT_TRUE(rfl::is_flattenable_v<
    int struct_not_trivially_destructible_t::*>);
  EXPECT_FALSE(rfl::is_flattenable_v<const foo_t&>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<int>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<wrapped_int_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_union_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const foo_t*>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
    int struct_not_trivially_destructible_t::*>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const foo_t&>);

  // Class types
  static_assert(std::is_aggregate_v<struct_not_destructible_t>,
                "Incorrect test case.");
  EXPECT_TRUE(rfl::is_flattenable_v<foo_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<bar_with_cv_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<with_non_aggregate_members_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<with_private_members_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<std::string>);
  EXPECT_FALSE(rfl::is_flattenable_v<std::string_view>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<foo_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<bar_with_cv_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<with_non_aggregate_members_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<with_private_members_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::string>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<std::string_view>);

  EXPECT_TRUE(rfl::is_flattenable_v<with_user_declared_ctor_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<struct_not_destructible_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<struct_not_trivially_destructible_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<derived_structured_1_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<derived_structured_2_t>);
  // User-declared ctors and dtors are disallowed for aggregates
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<with_user_declared_ctor_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<struct_not_destructible_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
    struct_not_trivially_destructible_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_structured_1_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_structured_2_t>);

  // cv-qualified
  EXPECT_TRUE(rfl::is_flattenable_v<const foo_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<const volatile foo_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const foo_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const volatile foo_t>);

  // Empty
  EXPECT_TRUE(rfl::is_flattenable_v<rfl::constant<42>>);
  EXPECT_TRUE(rfl::is_flattenable_v<std::monostate>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<rfl::constant<42>>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std::monostate>);

  // Arrays and array-like
  using std_array_baz_union_t_4 = std::array<baz_union_t, 4>;
  EXPECT_TRUE(rfl::is_flattenable_v<char[32]>);
  EXPECT_TRUE(rfl::is_flattenable_v<baz_union_t[32]>);
  EXPECT_TRUE(rfl::is_flattenable_v<char[]>);
  EXPECT_TRUE(rfl::is_flattenable_v<baz_union_t[]>);
  EXPECT_TRUE(rfl::is_flattenable_v<std_array_baz_union_t_4>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<char[32]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<baz_union_t[32]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<char[]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<baz_union_t[]>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std_array_baz_union_t_4>);

  // Union members are disallowed
  EXPECT_FALSE(rfl::is_flattenable_v<baz_struct_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_struct_t>);

  // Virtual members
  EXPECT_TRUE(rfl::is_flattenable_v<with_virtual_member_function_1_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<with_virtual_member_function_2_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<derived_1_with_virtual_mem_fn_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
    with_virtual_member_function_1_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
    with_virtual_member_function_2_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<
    derived_1_with_virtual_mem_fn_t>);

  // Inheritance
  static_assert(std::is_aggregate_v<derived_1_priv_in_base_t>,
    "Incorrect test case.");
  static_assert(std::is_aggregate_v<derived_2_priv_in_base_t>,
    "Incorrect test case.");
  EXPECT_TRUE(rfl::is_flattenable_v<derived_1_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<derived_1_prot_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<derived_1_priv_in_base_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<derived_2_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<derived_2_priv_in_base_t>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<derived_1_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_1_prot_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_1_priv_in_base_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<derived_2_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_2_priv_in_base_t>);

  // Virtual inheritance is disallowed.
  EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_1a_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_1b_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<derived_virt_2_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_1a_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_1b_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<derived_virt_2_t>);
}
