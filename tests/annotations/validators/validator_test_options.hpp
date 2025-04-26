#pragma once

#include "test_options.hpp"
#include <reflect_cpp26/annotations.hpp>

#define VALIDATOR(...) REFLECT_CPP26_VALIDATOR(__VA_ARGS__)

namespace rfl = reflect_cpp26;
namespace annots = reflect_cpp26::annotations;
using namespace std::literals;

#define LAZY_OBJECT(name, ...)                          \
  constexpr auto name = []() { return (__VA_ARGS__); }

template <class LazyFn>
constexpr bool validate_members(LazyFn fn) {
  return annots::validate_members(std::invoke(fn));
}

template <class LazyFn>
constexpr auto validation_error_message(LazyFn fn) -> std::string
{
  auto msg = std::string{};
  auto obj = std::invoke(fn);
  annots::validate_members(obj, &msg);
  return msg;
}

template <class LazyFn>
constexpr auto validation_full_error_message(LazyFn fn) -> std::string
{
  auto msg = std::string{};
  auto obj = std::invoke(fn);
  annots::validate_members_full(obj, &msg);
  return msg;
}
