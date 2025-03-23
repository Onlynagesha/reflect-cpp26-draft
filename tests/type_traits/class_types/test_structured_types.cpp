#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits/class_types.hpp>
#include <reflect_cpp26/utils/meta_span.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
// TODO: The following header triggers experimental compiler bug
// #include <reflect_cpp26/utils/meta_tuple.hpp>
#include <system_error>

namespace rfl = reflect_cpp26;

TEST(TypeTraitsClassTypes, IsStructuredTypeArithmetic)
{
  // integral type
  EXPECT_TRUE(rfl::is_structured_type_v<int>);
  EXPECT_TRUE(rfl::is_structured_type_v<const char16_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<volatile long long>);
  // floating type (since C++20)
  EXPECT_TRUE(rfl::is_structured_type_v<double>);
  EXPECT_TRUE(rfl::is_structured_type_v<const volatile long double>);
}

TEST(TypeTraitsClassTypes, IsStructuredTypePointers)
{
  // pointers and nullptr
  EXPECT_TRUE(rfl::is_structured_type_v<void*>);
  EXPECT_TRUE(rfl::is_structured_type_v<const char*>);
  EXPECT_TRUE(rfl::is_structured_type_v<const char* const>);
  EXPECT_TRUE(rfl::is_structured_type_v<volatile double*>);
  EXPECT_TRUE(rfl::is_structured_type_v<volatile double* volatile>);
  using std_array_int_4 = std::array<int, 4>;
  EXPECT_TRUE(rfl::is_structured_type_v<const volatile std_array_int_4*>);
  using std_vector_int = std::vector<int>;
  EXPECT_TRUE(rfl::is_structured_type_v<const std_vector_int*>);
  EXPECT_TRUE(rfl::is_structured_type_v<void(*)(int)>);
  EXPECT_TRUE(rfl::is_structured_type_v<std::nullptr_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<const volatile std::nullptr_t>);
}

TEST(TypeTraitsClassTypes, IsStructuredTypeReferences)
{
  // lvalue references
  EXPECT_TRUE(rfl::is_structured_type_v<double&>);
  EXPECT_TRUE(rfl::is_structured_type_v<const int&>);
  using std_array_int_4 = std::array<int, 4>;
  EXPECT_TRUE(rfl::is_structured_type_v<const std_array_int_4&>);
  using std_vector_int = std::vector<int>;
  EXPECT_TRUE(rfl::is_structured_type_v<volatile std_vector_int&>);
  EXPECT_TRUE(rfl::is_structured_type_v<const volatile std_vector_int&>);

  // rvalue references are not structured types.
  EXPECT_FALSE(rfl::is_structured_type_v<int&&>);
  EXPECT_FALSE(rfl::is_structured_type_v<const int&&>);
}

TEST(TypeTraitsClassTypes, IsStructuredTypePointerToMembers)
{
  // pointer-to-members
  using std_pair_double = std::pair<double, double>;
  EXPECT_TRUE(rfl::is_structured_type_v<
    double std_pair_double::*>);
  EXPECT_TRUE(rfl::is_structured_type_v<
    const char* rfl::meta_string_view::* volatile>);
}

enum class wrapped_int_t: int {};

TEST(TypeTraitsClassTypes, IsStructuredTypeEnums)
{
  // enumerators
  EXPECT_TRUE(rfl::is_structured_type_v<wrapped_int_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<volatile std::errc>);
}

TEST(TypeTraitsClassTypes, IsStructuredTypeLambdas)
{
  // lambdas with no capture
  auto some_value = 0;
  auto some_lambda = [](int, float, std::string, std::vector<std::string>) {};
  auto some_lambda_with_capture = [&some_value](int) {};
  EXPECT_TRUE(rfl::is_structured_type_v<decltype(some_lambda)>);
  EXPECT_FALSE(rfl::is_structured_type_v<decltype(some_lambda_with_capture)>);
}

TEST(TypeTraitsClassTypes, IsStructuredTypeArrays)
{
  // arrays are not structured types
  EXPECT_FALSE(rfl::is_structured_type_v<int[4]>);
  EXPECT_FALSE(rfl::is_structured_type_v<int[]>);
}

struct foo_t {
  size_t index;
  double value;
  const foo_t* prev;
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesBasic)
{
  // non-closure literal class type
  EXPECT_TRUE(rfl::is_structured_type_v<const foo_t>);
  using std_array_int_4 = std::array<int, 4>;
  EXPECT_TRUE(rfl::is_structured_type_v<volatile std_array_int_4>);
  using std_vector_int = std::vector<int>;
  EXPECT_FALSE(rfl::is_structured_type_v<std_vector_int>);

  EXPECT_TRUE(rfl::is_structured_type_v<rfl::meta_string_view>);
  EXPECT_FALSE(rfl::is_structured_type_v<std::string_view>);
  EXPECT_TRUE(rfl::is_structured_type_v<rfl::meta_span<int>>);
  EXPECT_FALSE(rfl::is_structured_type_v<std::span<const int>>);
  // std::tuple is not structured (since underlying data members are private)
  // in typical C++ stdlib implementation.

  // TODO: the following test case fails to compile
  //       due to experimental compiler bug.
  // using meta_tuple_cilfd = rfl::meta_tuple<char, int, long, float, double>;
  // EXPECT_TRUE(rfl::is_structured_type_v<meta_tuple_cilfd>);
}

struct bar_with_const_t {
  const int c_int;
  const long c_long;
  const double* const c_pcdouble;
};

// bar_with_cv_t is not structured due to its volatile members.
struct bar_with_volatile_t {
  const int c_int;
  volatile long v_long;
  const volatile double cv_double;
};

// bar_with_mutable_t is not structured due to its mutable member.
struct bar_with_mutable_t {
  int some_int;
  mutable long some_mutable_long;
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesCVMutable)
{
  EXPECT_TRUE(rfl::is_structured_type_v<bar_with_const_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<volatile bar_with_const_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<bar_with_volatile_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<bar_with_mutable_t>);
}

union baz_union_1_t {
  bar_with_volatile_t as_bar; // not literal type due to its volatile members
  foo_t as_foo;               //  is literal type
};
constexpr auto baz_union_1_constant_1 =
  rfl::constant<baz_union_1_t{}>{};
constexpr auto baz_union_1_constant_2 =
  rfl::constant<baz_union_1_t{.as_foo = {}}>{};

// Not structured since no literal type member.
union baz_union_2_t {
  bar_with_volatile_t as_bar; // not literal type due to its volatile members
};

struct baz_struct_1_t {
  baz_union_1_t baz_union;
  size_t index;
};

struct baz_struct_2_t {
  baz_union_2_t baz_union;
  size_t index;
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesUnion)
{
  EXPECT_TRUE(rfl::is_structured_type_v<baz_union_1_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<baz_union_2_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<baz_struct_1_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<baz_struct_2_t>);
}

struct has_array_1_t {
  int sum;
  int values[16];
};

struct has_array_2_t {
  int sum;
  has_array_1_t items[4][4][4];
};

struct has_array_3_t {
  int sum;
  baz_union_1_t baz_items[16][16];
};

struct has_array_4_t {
  int sum;
  baz_union_2_t baz_items[16]; // Not literal type
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesArrayMembers)
{
  EXPECT_TRUE(rfl::is_structured_type_v<has_array_1_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<has_array_2_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<has_array_3_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<has_array_4_t>);
}

struct struct_not_destructible_t {
  int x;
  int y;
  ~struct_not_destructible_t() = delete;
};

struct struct_not_trivially_destructible_t {
  int x;
  int y;
  const int& some_ref;

  constexpr struct_not_trivially_destructible_t(int x, int y, const int& r)
    : x(x), y(y), some_ref(r) {}

  constexpr ~struct_not_trivially_destructible_t() {
    if !consteval {
      std::println("some_ref = {}", some_ref);
    }
  }
};
static_assert(!std::is_trivially_destructible_v<
  struct_not_trivially_destructible_t>, "Incorrect test case.");
constexpr auto some_global_int = 42;
constexpr auto struct_not_trivially_destructible_constant = rfl::constant<
  struct_not_trivially_destructible_t{10, 20, some_global_int}>{};

union union_not_destructible_1_t {
  struct_not_destructible_t as_struct;
  int as_int;
};

union union_not_destructible_2_t {
  struct_not_trivially_destructible_t as_struct;
  int as_int;
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesDestructibility)
{
  EXPECT_FALSE(rfl::is_structured_type_v<struct_not_destructible_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<struct_not_trivially_destructible_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<union_not_destructible_1_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<union_not_destructible_2_t>);
}

struct derived_structured_1_t : struct_not_trivially_destructible_t {
  int rating;
  int usage_count;
  std::byte byte_data[256];
};

struct derived_structured_2_t : derived_structured_1_t, foo_t {
  baz_union_1_t some_union;
};

struct derived_not_structured_1_t
  : protected struct_not_trivially_destructible_t
{
  int rating;
  int usage_count;
  std::byte byte_data[256];
};

struct derived_not_structured_2_t : derived_not_structured_1_t, foo_t {
  baz_union_1_t some_union;
};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesInheritance)
{
  EXPECT_TRUE(rfl::is_structured_type_v<derived_structured_1_t>);
  EXPECT_TRUE(rfl::is_structured_type_v<derived_structured_2_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<derived_not_structured_1_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<derived_not_structured_2_t>);
}

struct foo_A_t : virtual foo_t {
  size_t a;
};

struct foo_B_t : virtual foo_t {
  size_t b;
};

struct foo_C_t : foo_A_t, foo_B_t {
  size_t c;
};

// error: non-type template parameter has non-literal type 'derived_virt_2_t'
// note: struct with virtual base class is not a literal type
TEST(TypeTraitsClassTypes, IsStructuredClassTypesVirtualInheritance)
{
  EXPECT_FALSE(rfl::is_structured_type_v<foo_A_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<foo_B_t>);
  EXPECT_FALSE(rfl::is_structured_type_v<foo_C_t>);
}

struct default_ctor_not_constexpr_t {
  int x = 0;
  int y = 0;

  default_ctor_not_constexpr_t() {
    std::println("default_ctor_not_constexpr_t() is non-constexpr.");
  }
  constexpr default_ctor_not_constexpr_t(int value): x(value), y(-value) {
    if !consteval {
      std::println("default_ctor_not_constexpr_t(int) is constexpr instead.");
    }
  }
};
// error: non-type template argument is not a constant expression
// constexpr auto default_ctor_not_constexpr_constant_1 =
//   rfl::constant<default_ctor_not_constexpr_t{}>{};
constexpr auto default_ctor_not_constexpr_constant_2 =
  rfl::constant<default_ctor_not_constexpr_t{42}>{};

TEST(TypeTraitsClassTypes, IsStructuredClassTypesNonConstexprCtor) {
  EXPECT_TRUE(rfl::is_structured_type_v<default_ctor_not_constexpr_t>);
}
