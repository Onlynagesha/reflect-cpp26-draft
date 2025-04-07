#include <gtest/gtest.h>
#include <reflect_cpp26/type_traits.hpp>
#include <cstdio>

namespace rfl = reflect_cpp26;

using printf_traits = rfl::function_traits<decltype(printf)>;
static_assert(printf_traits::has_ellipsis_parameter);
static_assert(! printf_traits::is_const);
static_assert(! printf_traits::is_volatile);
static_assert(! printf_traits::is_lvalue_reference);
static_assert(! printf_traits::is_rvalue_reference);
static_assert(! printf_traits::is_noexcept);
static_assert(std::is_same_v<printf_traits::result_type, int>);
static_assert(std::is_same_v<
  printf_traits::args_type, rfl::type_tuple<const char*>>);

int fprintf_wrapper(FILE* file, const char* msg, ...) noexcept;

using fprintf_wrapper_traits = rfl::function_traits<decltype(fprintf_wrapper)>;
static_assert(fprintf_wrapper_traits::has_ellipsis_parameter);
static_assert(! fprintf_wrapper_traits::is_const);
static_assert(! fprintf_wrapper_traits::is_volatile);
static_assert(! fprintf_wrapper_traits::is_lvalue_reference);
static_assert(! fprintf_wrapper_traits::is_rvalue_reference);
static_assert(fprintf_wrapper_traits::is_noexcept);
static_assert(std::is_same_v<fprintf_wrapper_traits::result_type, int>);
static_assert(std::is_same_v<
  fprintf_wrapper_traits::args_type, rfl::type_tuple<FILE*, const char*>>);

struct foo_t {
  int x;
  int y;
  void foo(int z);
  void foo_const(int z, int* dest) const;
  char foo_volatile_lref(bool flag) volatile &;
  long foo_const_volatile_rref(int z, bool flag) const volatile &&;
  void foo_const_noexcept(const int* first, const int* last) const noexcept;
  int* foo_volatile_rref_noexcept(int* dest) volatile && noexcept;
  long foo_ellipsis_const(const char* fmt, ...) const;
};

// void foo(int z);
using foo_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo)>>;
static_assert(! foo_traits::has_ellipsis_parameter);
static_assert(! foo_traits::is_const);
static_assert(! foo_traits::is_volatile);
static_assert(! foo_traits::is_lvalue_reference);
static_assert(! foo_traits::is_rvalue_reference);
static_assert(! foo_traits::is_noexcept);
static_assert(std::is_same_v<foo_traits::result_type, void>);
static_assert(std::is_same_v<foo_traits::args_type, rfl::type_tuple<int>>);

// void foo_const(int z, int* dest) const;
using foo_const_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_const)>>;
static_assert(! foo_const_traits::has_ellipsis_parameter);
static_assert(foo_const_traits::is_const);
static_assert(! foo_const_traits::is_volatile);
static_assert(! foo_const_traits::is_lvalue_reference);
static_assert(! foo_const_traits::is_rvalue_reference);
static_assert(! foo_const_traits::is_noexcept);
static_assert(std::is_same_v<foo_const_traits::result_type, void>);
static_assert(std::is_same_v<
  foo_const_traits::args_type, rfl::type_tuple<int, int*>>);

// char foo_volatile_lref(bool flag) volatile &;
using foo_volatile_lref_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_volatile_lref)>>;
static_assert(! foo_volatile_lref_traits::has_ellipsis_parameter);
static_assert(! foo_volatile_lref_traits::is_const);
static_assert(foo_volatile_lref_traits::is_volatile);
static_assert(foo_volatile_lref_traits::is_lvalue_reference);
static_assert(! foo_volatile_lref_traits::is_rvalue_reference);
static_assert(! foo_volatile_lref_traits::is_noexcept);
static_assert(std::is_same_v<foo_volatile_lref_traits::result_type, char>);
static_assert(std::is_same_v<
  foo_volatile_lref_traits::args_type, rfl::type_tuple<bool>>);

// long foo_const_volatile_rref(int z, bool flag) const volatile &&;
using foo_const_volatile_rref_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_const_volatile_rref)>>;
static_assert(! foo_const_volatile_rref_traits::has_ellipsis_parameter);
static_assert(foo_const_volatile_rref_traits::is_const);
static_assert(foo_const_volatile_rref_traits::is_volatile);
static_assert(! foo_const_volatile_rref_traits::is_lvalue_reference);
static_assert(foo_const_volatile_rref_traits::is_rvalue_reference);
static_assert(! foo_const_volatile_rref_traits::is_noexcept);
static_assert(std::is_same_v<
  foo_const_volatile_rref_traits::result_type, long>);
static_assert(std::is_same_v<
  foo_const_volatile_rref_traits::args_type, rfl::type_tuple<int, bool>>);

// void foo_const_noexcept(const int* first, const int* last) const noexcept;
using foo_const_noexcept_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_const_noexcept)>>;
static_assert(! foo_const_noexcept_traits::has_ellipsis_parameter);
static_assert(foo_const_noexcept_traits::is_const);
static_assert(! foo_const_noexcept_traits::is_volatile);
static_assert(! foo_const_noexcept_traits::is_lvalue_reference);
static_assert(! foo_const_noexcept_traits::is_rvalue_reference);
static_assert(foo_const_noexcept_traits::is_noexcept);
static_assert(std::is_same_v<foo_const_noexcept_traits::result_type, void>);
static_assert(std::is_same_v<
  foo_const_noexcept_traits::args_type,
  rfl::type_tuple<const int*, const int*>>);

// int* foo_volatile_rref_noexcept(int* dest) volatile && noexcept;
using foo_volatile_rref_noexcept_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_volatile_rref_noexcept)>>;
static_assert(! foo_volatile_rref_noexcept_traits::has_ellipsis_parameter);
static_assert(! foo_volatile_rref_noexcept_traits::is_const);
static_assert(foo_volatile_rref_noexcept_traits::is_volatile);
static_assert(! foo_volatile_rref_noexcept_traits::is_lvalue_reference);
static_assert(foo_volatile_rref_noexcept_traits::is_rvalue_reference);
static_assert(foo_volatile_rref_noexcept_traits::is_noexcept);
static_assert(std::is_same_v<
  foo_volatile_rref_noexcept_traits::result_type, int*>);
static_assert(std::is_same_v<
  foo_volatile_rref_noexcept_traits::args_type, rfl::type_tuple<int*>>);

// long foo_ellipsis_const(const char* fmt, ...) const;
using foo_ellipsis_const_traits = rfl::function_traits<
  rfl::to_function_t<decltype(&foo_t::foo_ellipsis_const)>>;
static_assert(foo_ellipsis_const_traits::has_ellipsis_parameter);
static_assert(foo_ellipsis_const_traits::is_const);
static_assert(! foo_ellipsis_const_traits::is_volatile);
static_assert(! foo_ellipsis_const_traits::is_lvalue_reference);
static_assert(! foo_ellipsis_const_traits::is_rvalue_reference);
static_assert(! foo_ellipsis_const_traits::is_noexcept);
static_assert(std::is_same_v<
  foo_ellipsis_const_traits::result_type, long>);
static_assert(std::is_same_v<
  foo_ellipsis_const_traits::args_type, rfl::type_tuple<const char*>>);

// Tests to_funtion_pointer_t
constexpr auto printf_fptr = printf;
// Type identity (with cvref)
static_assert(std::is_same_v<
  rfl::to_function_pointer_t<decltype(printf_fptr)>,
  int (*)(const char*, ...)>);
static_assert(std::is_same_v<
  rfl::to_function_pointer_t<volatile decltype(printf_fptr)>,
  int (*)(const char*, ...)>);
static_assert(std::is_same_v<
  rfl::to_function_pointer_t<const decltype(printf_fptr) &>,
  int (*)(const char*, ...)>);
// From function types (with ref)
static_assert(std::is_same_v<
  rfl::to_function_pointer_t<decltype(fprintf_wrapper)>,
  int (*)(FILE*, const char*, ...) noexcept>);
static_assert(std::is_same_v<
  rfl::to_function_pointer_t<void(&)(const int, long, const double*)>,
  void (*)(int, long, const double*)>);

// Non-function types
static_assert(!rfl::is_convertible_to_function_pointer_v<void>);
static_assert(!rfl::is_convertible_to_function_pointer_v<int>);
static_assert(!rfl::is_convertible_to_function_pointer_v<const char*>);

// Member function types with cvref qualifiers
static_assert(!rfl::is_convertible_to_function_pointer_v<int(int) const>);
static_assert(!rfl::is_convertible_to_function_pointer_v<int(int) volatile>);
static_assert(!rfl::is_convertible_to_function_pointer_v<int(int) &>);
static_assert(!rfl::is_convertible_to_function_pointer_v<int(int) &&>);

// Tests to_function_t
// Type identity (with cvref)
static_assert(std::is_same_v<
  rfl::to_function_t<decltype(fprintf_wrapper)>,
  int(FILE*, const char*, ...) noexcept>);
static_assert(std::is_same_v<
  rfl::to_function_t<decltype(fprintf_wrapper) &&>,
  int(FILE*, const char*, ...) noexcept>);
// From function pointer (with cvref)
static_assert(std::is_same_v<
  rfl::to_function_t<const volatile decltype(printf_fptr) &&>,
  int(const char*, ...)>);
// From function pointers (with cvref)
// void foo(int z);
static_assert(std::is_same_v<
  rfl::to_function_t<decltype(&foo_t::foo)>,
  void(int)>);
// void foo_const(int z, int* dest) const;
static_assert(std::is_same_v<
  rfl::to_function_t<volatile decltype(&foo_t::foo_const) &>,
  void(int, int*) const>);
// char foo_volatile_lref(bool flag) volatile &;
static_assert(std::is_same_v<
  rfl::to_function_t<const decltype(&foo_t::foo_volatile_lref) &&>,
  char(bool) volatile &>);
// long foo_const_volatile_rref(int z, bool flag) const volatile &&;
static_assert(std::is_same_v<
  rfl::to_function_t<const volatile decltype(&foo_t::foo_const_volatile_rref)>,
  long(int, bool) const volatile &&>);
// void foo_const_noexcept(const int* first, const int* last) const noexcept;
static_assert(std::is_same_v<
  rfl::to_function_t<const decltype(&foo_t::foo_const_noexcept)>,
  void(const int*, const int*) const noexcept>);
// int* foo_volatile_rref_noexcept(int* dest) volatile && noexcept;
static_assert(std::is_same_v<
  rfl::to_function_t<decltype(&foo_t::foo_volatile_rref_noexcept) &>,
  int*(int*) volatile && noexcept>);
// long foo_ellipsis_const(const char* fmt, ...) const;
static_assert(std::is_same_v<
  rfl::to_function_t<decltype(&foo_t::foo_ellipsis_const) &&>,
  long(const char*, ...) const>);

// Non-function types
static_assert(!rfl::is_convertible_to_function_v<void>);
static_assert(!rfl::is_convertible_to_function_v<int>);
static_assert(!rfl::is_convertible_to_function_v<const char*>);

TEST(TypeTraitsClassTypes, FunctionTypes) {
  EXPECT_TRUE(true); // All test cases done with static-asserts above
}
