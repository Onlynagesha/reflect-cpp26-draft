#ifndef REFLECT_CPP26_ANNOTATIONS_PREDICATES_HPP
#define REFLECT_CPP26_ANNOTATIONS_PREDICATES_HPP

#include <reflect_cpp26/type_traits/reduction.hpp>
#include <reflect_cpp26/type_traits/template_instance.hpp>
#include <reflect_cpp26/utils/define_static_values.hpp>
#include <reflect_cpp26/utils/expand.hpp>
#include <reflect_cpp26/utils/meta_span.hpp>
#include <reflect_cpp26/utils/meta_string_view.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <reflect_cpp26/utils/to_string.hpp>
#include <reflect_cpp26/utils/utility.hpp>
#include <concepts>
#include <ranges>

namespace reflect_cpp26::annotations {
namespace impl {
template <class T, class U>
constexpr auto make_boundary_test_error_message(
  std::string_view op, const T& boundary, const U& actual_value) -> std::string
{
  if constexpr (requires { to_string(boundary); }) {
    auto res = std::string{"Expects value "};
    res += op;
    res += ' ';
    res += to_string(boundary); // ADL for custom types
    if constexpr (requires { to_string(actual_value); }) {
      res += ", while actual value = ";
      res += to_string(actual_value);
    }
    return res;
  } else {
    auto res = std::string{"Expects value "};
    res += op;
    res += " specified boundary";
    if constexpr (requires { to_string(actual_value); }) {
      res += ", while actual value = ";
      res += to_string(actual_value);
    }
    return res;
  }
}

template <class T, class U>
constexpr auto make_options_test_error_message(
  std::string_view any_or_none, meta_span<T> list, const U& actual_value)
  -> std::string
{
  if constexpr (requires { to_string(list[0]); }) {
    auto res = std::string{"Expects value to be "};
    res += any_or_none;
    res += " of [";
    for (auto i = 0zU, n = list.size(); i < n; i++) {
      if (i != 0) {
        res += ", ";
      }
      res += to_string(list[i]);
    }
    if constexpr (requires { to_string(actual_value); }) {
      res += "], while actual value = ";
      res += to_string(actual_value);
    } else {
      res += ']';
    }
    return res;
  } else {
    auto res = std::string{"Expects value to be "};
    res += any_or_none;
    res += " of specified values";
    if constexpr (requires { to_string(actual_value); }) {
      res += ", while actual value = ";
      res += to_string(actual_value);
    }
    return res;
  }
}

template <class Comp>
struct operand_notation;

#define REFLECT_CPP26_OPERAND_WITH_NOTATION_FOR_EACH(F) \
  F(less_t, "<")                                        \
  F(less_equal_t, "<=")                                 \
  F(greater_t, ">")                                     \
  F(greater_equal_t, ">=")                              \
  F(cmp_less_t, "<")                                    \
  F(cmp_less_equal_t, "<=")                             \
  F(cmp_greater_t, ">")                                 \
  F(cmp_greater_equal_t, ">=")

#define REFLECT_CPP26_SPECIALIZE_OPERAND_NOTATION(Comp, notation) \
  template <>                                                     \
  struct operand_notation<Comp> {                                 \
    static constexpr auto value = std::string_view{notation};     \
  };
REFLECT_CPP26_OPERAND_WITH_NOTATION_FOR_EACH(
  REFLECT_CPP26_SPECIALIZE_OPERAND_NOTATION)

#undef REFLECT_CPP26_OPERAND_WITH_NOTATION_FOR_EACH
#undef REFLECT_CPP26_SPECIALIZE_OPERAND_NOTATION
} // namespace impl

struct predicate_tag_t {};

template <class Annotation>
constexpr auto is_predicate_v = std::derived_from<Annotation, predicate_tag_t>;

template <class IntComp, class GenericComp, class BoundaryT>
struct boundary_test_t : predicate_tag_t {
  BoundaryT boundary;

  template <class InputT>
    requires (std::ranges::input_range<InputT>)
  constexpr bool test_range(const InputT& value_range) const
  {
    using BoundaryValueT = std::ranges::range_value_t<BoundaryT>;
    using InputValueT    = std::ranges::range_value_t<InputT>;
    // Signedness-safe comparison is used for integral ranges as well.
    using Compare3WayT = std::conditional_t<
      all_of_v<std::is_integral, BoundaryValueT, InputValueT>,
      cmp_three_way_t, compare_three_way_t>;

    static_assert(requires (BoundaryValueT b, InputValueT i) { 
      { Compare3WayT::operator()(b, i) };
    }, "Values of boundary and input range are not comparable!");

    auto cmp_res = std::lexicographical_compare_three_way(
      std::ranges::begin(boundary), std::ranges::end(boundary),
      std::ranges::begin(value_range), std::ranges::end(value_range),
      Compare3WayT());
    return GenericComp::operator()(cmp_res, 0);
  }

  template <class InputT>
  constexpr bool test(const InputT& value) const
  {
    // (1) Range comparison when boundary is a range itself.
    //     Note that basic_meta_string_view, as a special kind of range,
    //     is not in this case.
    if constexpr (is_template_instance_of_v<BoundaryT, meta_span>) {
      static_assert(std::ranges::input_range<InputT>,
        "Input must be another range when boundary is a range.");
      return test_range(value);
    }
    // (2) Integral comparison.
    //     Signedness-safe comparison functionality is used.
    else if constexpr (all_of_v<std::is_integral, BoundaryT, InputT>) {
      return IntComp::operator()(value, boundary);
    }
    // (3) Generic comparison, including basic_meta_string_view
    //     with other string-like types or const CharT*.
    else {
      return GenericComp::operator()(value, boundary);
    }
  }

  template <class InputT>
  constexpr auto make_error_message(const InputT& value) const -> std::string
  {
    constexpr auto op = impl::operand_notation<GenericComp>::value;
    return impl::make_boundary_test_error_message(op, boundary, value);
  }
};

template <bool Negates, class OptionT>
struct generic_options_t : predicate_tag_t {
  static constexpr auto options_are_integral = std::is_integral_v<OptionT>;
  meta_span<OptionT> values;

  template <class InputT>
  constexpr bool test(const InputT& input) const
  {
    if constexpr (options_are_integral && std::is_integral_v<InputT>) {
      auto found = std::ranges::find_if(values, [input](auto v) {
        return cmp_equal(input, v);
      }) != values.end();
      return Negates ^ found;
    } else {
      return Negates ^ std::ranges::contains(values, input);
    }
  }

  template <class InputT>
  constexpr auto make_error_message(const InputT& input) const -> std::string
  {
    constexpr auto any_or_none = Negates ? "none" : "any";
    return impl::make_options_test_error_message(any_or_none, values, input);
  }
};

template <class T>
using min_t = boundary_test_t<cmp_greater_equal_t, greater_equal_t, T>;

template <class T>
using max_t = boundary_test_t<cmp_less_equal_t, less_equal_t, T>;

template <class T>
using min_exclusive_t = boundary_test_t<cmp_greater_t, greater_t, T>;

template <class T>
using max_exclusive_t = boundary_test_t<cmp_less_t, less_t, T>;

template <class T>
using options_t = generic_options_t<false, T>;

template <class T>
using excludes_t = generic_options_t<true, T>;

template <template <class> class Which>
struct boundary_test_assignment_t {
  template <class Range>
    /* requires (std::ranges::input_range<Range>) */
  static consteval auto from_range(Range&& range)
  {
    using ValueT = std::ranges::range_value_t<Range>;
    if constexpr (is_char_type_v<ValueT>) {
      return Which<meta_basic_string_view<ValueT>>{
        .boundary = define_static_string(std::forward<Range>(range), true)};
    } else {
      return Which<meta_span<ValueT>>{
        .boundary = define_static_array(std::forward<Range>(range))};
    }
  }

  template <class T>
  static consteval auto from_pointer(T* pointer) -> Which<T>
  {
    static_assert(is_char_type_v<T> && std::is_const_v<T>,
                  "Pointers other than const CharT* are disallowed.");
    return from_range(meta_basic_string_view{pointer});
  }

  template <class T>
  consteval auto operator=(T&& value) const
  {
    using NoCVRefT = std::remove_cvref_t<T>;
    // (1) Input is a range
    //     (including character range like std::string or std::string_view)
    if constexpr (std::ranges::input_range<T>) {
      return from_range(std::forward<T>(value));
    }
    // (2.1) Input is a pointer
    //       const CharT* represents a string. Other pointers are invalid.
    else if constexpr (std::is_pointer_v<NoCVRefT>) {
      return from_pointer(value);
    }
    // (2.2) nullptr is invalid
    else if constexpr (std::is_same_v<NoCVRefT, std::nullptr_t>) {
      static_assert(false, "nullptr is disallowed.");
    }
    // (3) Others
    else {
      return Which<NoCVRefT>{.boundary = std::forward<T>(value)};
    }
  }
};

struct options_assignment_t {
  template <class T>
  consteval auto operator=(std::initializer_list<T> values) const
    -> options_t<T>
  {
    auto to_span = define_static_array(values);
    return options_t<T>{.values = to_span};
  }

  template <class Range>
    requires (std::ranges::input_range<Range>)
  consteval auto operator=(const Range& range) const /* -> options_t<T> */
  {
    using T = std::ranges::range_value_t<Range>;
    auto to_span = define_static_array(range);
    return options_t<T>{.values = to_span};
  }
};

struct excludes_assignment_t {
  template <class T>
  consteval auto operator=(std::initializer_list<T> values) const
    -> excludes_t<T>
  {
    auto to_span = define_static_array(values);
    return excludes_t<T>{.values = to_span};
  }

  template <class Range>
    requires (std::ranges::input_range<Range>)
  consteval auto operator=(const Range& range) const /* -> excludes_t<T> */
  {
    using T = std::ranges::range_value_t<Range>;
    auto to_span = define_static_array(range);
    return excludes_t<T>{.values = to_span};
  }

  template <class T>
  consteval auto operator=(const T& value) const -> excludes_t<T>
  {
    auto to_span = define_static_array(std::views::single(value));
    return excludes_t<T>{.values = to_span};
  }
};

struct predicates_assignment_entry_t {
  [[no_unique_address]]
  boundary_test_assignment_t<min_t> min{};

  [[no_unique_address]]
  boundary_test_assignment_t<max_t> max{};

  [[no_unique_address]]
  boundary_test_assignment_t<min_exclusive_t> min_exclusive{};

  [[no_unique_address]]
  boundary_test_assignment_t<max_exclusive_t> max_exclusive{};

  [[no_unique_address]]
  options_assignment_t options{};

  [[no_unique_address]]
  excludes_assignment_t excludes{};
};

template <class T>
  requires (std::is_class_v<T>)
constexpr bool validate_members(
  const T& obj, std::string* error_output = nullptr)
{
  auto res = true;
  auto members = REFLECT_CPP26_EXPAND(nonstatic_data_members_of(^^T));
  members.for_each([&res, &obj, error_output](auto m) {
    const auto& cur_value = obj.[:m:];

    auto annotations = REFLECT_CPP26_EXPAND(std::meta::annotations_of(m));
    annotations.for_each([&res, &cur_value, error_output, m](auto a) {
      constexpr auto cur_annotation = extract(a);
      if constexpr (is_predicate_v<decltype(cur_annotation)>) {
        res &= cur_annotation.test(cur_value);
        if (!res && error_output != nullptr) {
          *error_output += "Invalid member '";
          *error_output += std::meta::identifier_of(m);
          *error_output += "': ";
          *error_output += cur_annotation.make_error_message(cur_value);
        }
      }
      return res; // Stops on single violation
    });
    return res; // Stops on single violation
  });
  return res;
}

template <class T>
  requires (std::is_class_v<T>)
constexpr bool validate_members_full(const T& obj, std::string* error_output)
{
  if (error_output == nullptr) {
    return validate_members(obj, nullptr);
  }
  auto res = true;
  auto members = REFLECT_CPP26_EXPAND(nonstatic_data_members_of(^^T));
  members.for_each([&res, &obj, error_output](auto m) {
    const auto& cur_value = obj.[:m:];
    auto res_cur_value = true;

    auto annotations = REFLECT_CPP26_EXPAND(std::meta::annotations_of(m));
    annotations.for_each([&res_cur_value, &cur_value, error_output, m](auto a) {
      constexpr auto cur_annotation = extract(a);
      if constexpr (is_predicate_v<decltype(cur_annotation)>) {
        auto cur_res = cur_annotation.test(cur_value);
        if (!cur_res) {
          if (res_cur_value) {
            *error_output += "Invalid member '";
            *error_output += std::meta::identifier_of(m);
            *error_output += "':\n";
          }
          *error_output += "* ";
          *error_output += cur_annotation.make_error_message(cur_value);
          *error_output += '\n';
        }
        res_cur_value &= cur_res;
      }
    });
    res &= res_cur_value;
  });
  if (!res) {
    error_output->pop_back(); // Removes the trailing '\n'
  }
  return res;
}
} // namespace reflect_cpp26::annotations

#endif // REFLECT_CPP26_ANNOTATIONS_PREDICATES_HPP
