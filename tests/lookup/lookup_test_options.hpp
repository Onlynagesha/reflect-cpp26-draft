#pragma once

#include <reflect_cpp26/utils/config.h>
#include "tests/test_options.hpp"

void check_member_variable(auto expected, auto&& obj, auto mptr)
{
  ASSERT_NE(nullptr, mptr);
  EXPECT_EQ(expected, obj.*mptr);
}

template <class Obj>
CONSTEVAL_ON_STATIC_TEST void check_member_variable_static(
  auto expected, Obj&& obj, auto mptr)
{
#ifdef ENABLE_STATIC_TEST
  if (mptr == nullptr) {
    reflect_cpp26::compile_error("Result member pointer is nullptr.");
  }
  if (expected != obj.*mptr) {
    reflect_cpp26::compile_error("Result member pointer is incorrect.");
  }
#else
  check_member_variable(expected, std::forward<Obj>(obj), mptr);
#endif
}

void check_member_function(auto expected, auto&& obj, auto mptr, auto... args)
{
  ASSERT_NE(nullptr, mptr);
  EXPECT_EQ(expected, (obj.*mptr)(args...));
}

template <class Obj>
CONSTEVAL_ON_STATIC_TEST void check_member_function_static(
  auto expected, Obj&& obj, auto mptr, auto... args)
{
#ifdef ENABLE_STATIC_TEST
  if (mptr == nullptr) {
    reflect_cpp26::compile_error("Result member pointer is nullptr.");
  }
  if (expected != (obj.*mptr)(args...)) {
    reflect_cpp26::compile_error("Result member function is incorrect.");
  }
#else
  check_member_function(expected, std::forward<Obj>(obj), mptr, args...);
#endif
}

void check_variable(auto expected, auto ptr)
{
  ASSERT_NE(nullptr, ptr);
  EXPECT_EQ(expected, *ptr);
}

CONSTEVAL_ON_STATIC_TEST void check_variable_static(auto expected, auto ptr)
{
#ifdef ENABLE_STATIC_TEST
  if (ptr == nullptr) {
    reflect_cpp26::compile_error("Result pointer is nullptr.");
  }
  if (expected != *ptr) {
    reflect_cpp26::compile_error("Result pointer is incorrect.");
  }
#else
  check_variable(expected, ptr);
#endif
}

void check_function(auto expected, auto fptr, auto... args)
{
  ASSERT_NE(nullptr, fptr);
  EXPECT_EQ(expected, fptr(args...));
}

CONSTEVAL_ON_STATIC_TEST void check_function_static(
  auto expected, auto fptr, auto... args)
{
#ifdef ENABLE_STATIC_TEST
  if (fptr == nullptr) {
    reflect_cpp26::compile_error("Result function pointer is nullptr.");
  }
  if (expected != fptr(args...)) {
    reflect_cpp26::compile_error("Result function pointer is incorrect.");
  }
#else
  check_function(expected, fptr, args...);
#endif
}
