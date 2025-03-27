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

struct my_pair {
  std::string first;
  std::string second;
};

struct my_pair_with_ctor {
  std::string first;
  std::string second;
  my_pair_with_ctor() = default;
};
static_assert(!std::is_aggregate_v<my_pair_with_ctor>, "Incorrect test case.");

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesBasic)
{
  using std_pair = std::pair<std::string, std::string>;
  using std_vector = std::vector<std::string>;

  EXPECT_TRUE(rfl::is_partially_flattenable_v<std::monostate>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const std::to_chars_result>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<volatile std_pair>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<volatile my_pair>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_with_ctor>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<const volatile std_vector>);

  EXPECT_TRUE(rfl::is_flattenable_v<std::monostate>);
  EXPECT_TRUE(rfl::is_flattenable_v<const std::to_chars_result>);
  EXPECT_TRUE(rfl::is_flattenable_v<volatile std_pair>);
  EXPECT_TRUE(rfl::is_flattenable_v<volatile my_pair>);
  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_with_ctor>);
  EXPECT_FALSE(rfl::is_flattenable_v<const volatile std_vector>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<std::monostate>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<const std::to_chars_result>);
  EXPECT_EQ(std::is_aggregate_v<std_pair>,
            rfl::is_flattenable_aggregate_v<volatile std_pair>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<volatile my_pair>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<my_pair_with_ctor>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<const volatile std_vector>);
}

struct my_pair_with_ctor_extended : my_pair_with_ctor {
  std::string third;
  std::string fourth;
};
static_assert(std::is_aggregate_v<my_pair_with_ctor_extended>,
  "Incorrect test case.");

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesNonAggregateBase)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_with_ctor_extended>);
  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_with_ctor_extended>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<my_pair_with_ctor_extended>);
}

struct references_t {
  int& i;
  const long& cl;
  volatile float& vf;
  const volatile double& cvd;
};
static_assert(std::is_aggregate_v<references_t>, "Incorrect test case.");

// TEST(TypeTraitsClassTypes, IsFlattenableClassTypesReferenceMembers)
// {
//   EXPECT_TRUE(rfl::is_partially_flattenable_v<references_t>);
//   EXPECT_TRUE(rfl::is_flattenable_v<references_t>);
//   EXPECT_TRUE(rfl::is_flattenable_aggregate_v<references_t>);
// }

struct struct_not_destructible_t {
  int x;
  int y;
  ~struct_not_destructible_t() = delete;
};

struct struct_not_trivially_destructible_t {
  int x;
  int y;
  const int z;
  ~struct_not_trivially_destructible_t() {
    std::println("z = {}", z);
  }
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesDestructibility)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<
    struct_not_destructible_t>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<
    struct_not_trivially_destructible_t>);

  EXPECT_TRUE(rfl::is_flattenable_v<struct_not_destructible_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<struct_not_trivially_destructible_t>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<
    struct_not_destructible_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<
    struct_not_trivially_destructible_t>);
}

struct my_pair_extended_A : my_pair {
  size_t index;
};

struct my_pair_extended_B : my_pair_extended_A {
  double values[16];
  size_t index; // Shadows my_pair_extended_A::index
  size_t size;
};

struct my_pair_extended_C : my_pair_extended_B, std::to_chars_result {
  const double* ptr; // Shadows std::to_chars_result::ptr
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesInheritance)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_extended_A>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_extended_B>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_extended_C>);

  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_extended_A>);
  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_extended_B>);
  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_extended_C>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<my_pair_extended_A>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<my_pair_extended_B>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<my_pair_extended_C>);
}

struct my_pair_extended_D : my_pair {
  double d[12];
  virtual void dump() {
    std::println("{} {}\n", this->first, this->second);
  }
};

struct my_pair_extended_E : my_pair_extended_D, std::to_chars_result {
  double e[12];
  void dump() override {
    std::println("{} {} {}\n",
      this->first, this->second, static_cast<const void*>(this->ptr));
  }
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesVirtualMembers)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_extended_D>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<my_pair_extended_E>);

  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_extended_D>);
  EXPECT_TRUE(rfl::is_flattenable_v<my_pair_extended_E>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<my_pair_extended_D>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<my_pair_extended_E>);
}

struct foo_A_t : virtual my_pair {
  size_t a;
};

struct foo_B_t : virtual my_pair {
  size_t b;
};

struct foo_C_t : foo_A_t, foo_B_t {
  size_t c;
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesVirtualInheritance)
{
  EXPECT_FALSE(rfl::is_partially_flattenable_v<foo_A_t>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<foo_B_t>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<foo_C_t>);

  EXPECT_FALSE(rfl::is_flattenable_v<foo_A_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<foo_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<foo_C_t>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<foo_A_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<foo_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<foo_C_t>);
}

struct bar_A_t : my_pair {
  size_t a;
};

struct bar_B_t : my_pair {
  size_t b;
};

struct bar_C_t : bar_A_t, bar_B_t {
  size_t c;
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesDiamondInheritance)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<bar_A_t>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<bar_B_t>);
  EXPECT_FALSE(rfl::is_partially_flattenable_v<bar_C_t>);

  EXPECT_TRUE(rfl::is_flattenable_v<bar_A_t>);
  EXPECT_TRUE(rfl::is_flattenable_v<bar_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<bar_C_t>);

  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<bar_A_t>);
  EXPECT_TRUE(rfl::is_flattenable_aggregate_v<bar_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<bar_C_t>);
}

struct baz_A_t : protected my_pair {
  size_t a;
};

struct baz_B_t : private my_pair {
  size_t b;
};

struct baz_C_t : baz_A_t, baz_B_t {
  size_t c;
};

TEST(TypeTraitsClassTypes, IsFlattenableClassTypesNonPublicInheritance)
{
  EXPECT_TRUE(rfl::is_partially_flattenable_v<baz_A_t>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<baz_B_t>);
  EXPECT_TRUE(rfl::is_partially_flattenable_v<baz_C_t>);

  EXPECT_FALSE(rfl::is_flattenable_v<baz_A_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<baz_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_v<baz_C_t>);

  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_A_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_B_t>);
  EXPECT_FALSE(rfl::is_flattenable_aggregate_v<baz_C_t>);
}
