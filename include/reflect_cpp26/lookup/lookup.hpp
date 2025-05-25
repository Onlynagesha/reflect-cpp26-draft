#ifndef REFLECT_CPP26_LOOKUP_LOOKUP_HPP
#define REFLECT_CPP26_LOOKUP_LOOKUP_HPP

#include <reflect_cpp26/enum/enum_index.hpp>
#include <reflect_cpp26/type_traits/is_invocable.hpp>
#include <reflect_cpp26/type_traits/template_instance.hpp>
#include <reflect_cpp26/utils/meta_utility.hpp>
#include <optional>

namespace reflect_cpp26 {
struct lookup_filter_flags_t {
  bool static_only = false;
  bool non_static_only = false;
  bool variable_only = false;
  bool function_only = false;

private:
  consteval void set_with_flag(char flag)
  {
    switch (flag) {
      case 's': static_only = true; break;
      case 'n': non_static_only = true; break;
      case 'v': variable_only = true; break;
      case 'f': function_only = true; break;
      default: compile_error("Invalid flag character.");
    }
  }

public:
  consteval lookup_filter_flags_t() = default;

  consteval lookup_filter_flags_t(char flag) {
    set_with_flag(flag);
  }

  consteval lookup_filter_flags_t(const char* flags) {
    for (; *flags != '\0'; ++flags) {
      set_with_flag(*flags);
    }
  }

  consteval bool test_type_member(std::meta::info member) const
  {
    if (static_only && !is_static_member(member)) { return false; }
    if (non_static_only && is_static_member(member)) { return false; }
    if (variable_only && is_function(member)) { return false; }
    if (function_only && !is_function(member)) { return false; }
    return true;
  }

  consteval bool test_namespace_member(std::meta::info member) const
  {
    if (variable_only && is_function(member)) { return false; }
    if (function_only && !is_function(member)) { return false; }
    return true;
  }
};

namespace impl::lookup {
template <class Predicate>
consteval void lookup_type_members(
  std::vector<std::meta::info>& dest, std::meta::info T,
  const Predicate& predicate)
{
  if (!is_class_type(T) && !is_union_type(T)) {
    compile_error("T must be either of (1) class type; (2) or union type.");
  }
  for (auto member: public_direct_members_of(T)) {
    if (predicate(member)) {
      dest.push_back(member);
    }
  }
  if (is_class_type(T)) {
    for (auto base: public_direct_bases_of(T)) {
      if (is_virtual(base)) {
        compile_error("Virtual inheritance is not supported.");
      }
      lookup_type_members(dest, type_of(base), predicate);
    }
  }
}

template <class Predicate>
consteval void lookup_namespace_members(
  std::vector<std::meta::info>& dest, std::meta::info ns,
  const Predicate& predicate)
{
  if (!is_namespace(ns)) {
    compile_error("ns must be namespace.");
  }
  for (auto member: public_direct_members_of(ns)) {
    if (predicate(member)) {
      dest.push_back(member);
    }
  }
}

template <class Func>
struct member_transform_function_traits {
  static constexpr auto is_valid = false;
};

template <invocable_with<std::meta::info> Func>
struct member_transform_function_traits<Func> {
  static constexpr auto is_valid = true;
  using result_type = std::invoke_result_t<Func, std::meta::info>;
};

template <invocable_with<std::string_view> Func>
struct member_transform_function_traits<Func> {
  static constexpr auto is_valid = true;
  using result_type = std::invoke_result_t<Func, std::string_view>;
};

template <class Func, class T>
concept member_transform_function = []() {
  using Traits = member_transform_function_traits<Func>;
  if constexpr (Traits::is_valid) {
    return std::is_same_v<std::optional<T>, typename Traits::result_type>;
  } else {
    return false;
  }
}();

template <class ValueType, class TransformFn>
consteval void lookup_named_direct_members_by_optional(
  std::vector<std::pair<std::meta::info, ValueType>>& dest, std::meta::info T,
  const TransformFn& transform_fn)
{
  auto named_members =
    public_direct_members_of(T) | std::views::filter(std::meta::has_identifier);
  for (auto member: named_members) {
    auto res = [member, &transform_fn]() {
      if constexpr (std::is_invocable_v<TransformFn, std::meta::info>) {
        return transform_fn(member);
      } else {
        static_assert(std::is_invocable_v<TransformFn, std::string_view>);
        return transform_fn(identifier_of(member));
      }
    }();
    if (res.has_value()) {
      dest.emplace_back(member, *res);
    }
  }
}

template <class ValueType, class TransformFn>
consteval void lookup_named_type_members_by_optional(
  std::vector<std::pair<std::meta::info, ValueType>>& dest, std::meta::info T,
  const TransformFn& transform_fn)
{
  if (!is_class_type(T) && !is_union_type(T)) {
    compile_error("T must be either of (1) class type; (2) or union type.");
  }
  lookup_named_direct_members_by_optional(dest, T, transform_fn);
  if (is_class_type(T)) {
    for (auto base: public_direct_bases_of(T)) {
      if (is_virtual(base)) {
        compile_error("Virtual inheritance is not supported.");
      }
      lookup_named_type_members_by_optional(dest, type_of(base), transform_fn);
    }
  }
}

template <class ValueType, class TransformFn>
consteval void lookup_named_namespace_members_by_optional(
  std::vector<std::pair<std::meta::info, ValueType>>& dest, std::meta::info ns,
  const TransformFn& transform_fn)
{
  if (!is_namespace(ns)) {
    compile_error("ns must be namespace.");
  }
  lookup_named_direct_members_by_optional(dest, ns, transform_fn);
}
} // namespace impl::lookup

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) that satisfy given predicate.
 */
template <invocable_r_with<bool, std::meta::info> Predicate>
consteval auto lookup_type_members(
  std::meta::info T, const Predicate& pred) -> std::vector<std::meta::info>
{
  auto res = std::vector<std::meta::info>{};
  impl::lookup::lookup_type_members(res, T, pred);
  return res;
}

/**
 * Gets direct members of namespace ns that satisfy given predicate.
 */
template <invocable_r_with<bool, std::meta::info> Predicate>
consteval auto lookup_namespace_members(
  std::meta::info ns, const Predicate& pred) -> std::vector<std::meta::info>
{
  auto res = std::vector<std::meta::info>{};
  impl::lookup::lookup_namespace_members(res, ns, pred);
  return res;
}

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) whose identifier satisfy given predicate.
 */
template <invocable_r_with<bool, std::string_view> Predicate>
consteval auto lookup_type_members_by_name(
  std::meta::info T, const Predicate& pred) -> std::vector<std::meta::info>
{
  auto res = std::vector<std::meta::info>{};
  impl::lookup::lookup_type_members(res, T, [&pred](std::meta::info m) {
    return has_identifier(m) && pred(identifier_of(m));
  });
  return res;
}

/**
 * Gets members of namespace ns whose identifier satisfy given predicate.
 */
template <invocable_r_with<bool, std::string_view> Predicate>
consteval auto lookup_namespace_members_by_name(
  std::meta::info ns, const Predicate& pred) -> std::vector<std::meta::info>
{
  auto res = std::vector<std::meta::info>{};
  impl::lookup::lookup_namespace_members(res, ns,
    [&pred](std::meta::info m) {
      return has_identifier(m) && pred(identifier_of(m));
    });
  return res;
}

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) that are renamed by given function.
 * Pesudocode:
 * res = []
 * for m in members of T
 *   if not has_identifier(m)
 *     continue
 *   rename = rename_fn(m)
 *   if rename.has_value()
 *     res.emplace_back(m, *rename)
 * return res
 */
template <impl::lookup::member_transform_function<std::string> RenameFn>
consteval auto lookup_type_members_by_renaming(
  std::meta::info T, const RenameFn& rename_fn)
  -> std::vector<std::pair<std::meta::info, std::string>>
{
  auto res = std::vector<std::pair<std::meta::info, std::string>>{};
  impl::lookup::lookup_named_type_members_by_optional(res, T, rename_fn);
  return res;
}

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) that are renamed by given function.
 * Pseudocode same as above.
 */
template <impl::lookup::member_transform_function<std::string> RenameFn>
consteval auto lookup_namespace_members_by_renaming(
  std::meta::info ns, const RenameFn& rename_fn)
  -> std::vector<std::pair<std::meta::info, std::string>>
{
  auto res = std::vector<std::pair<std::meta::info, std::string>>{};
  impl::lookup::lookup_named_namespace_members_by_optional(res, ns, rename_fn);
  return res;
}

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) that are renamed by given function.
 * Renaming results are as static string.
 */
template <impl::lookup::member_transform_function<std::string> RenameFn>
consteval auto lookup_type_members_by_renaming_meta(
  std::meta::info T, const RenameFn& rename_fn)
  -> std::vector<std::pair<std::meta::info, meta_string_view>>
{
  return lookup_type_members_by_renaming(T, rename_fn)
    | std::views::transform([](const auto& pair) {
        auto static_rename = reflect_cpp26::define_static_string(pair.second);
        return std::pair{pair.first, static_rename};
      })
    | std::ranges::to<std::vector>();
}

/**
 * Gets members of class or union type T (including those inherited from
 * bases of T if T is a class type) that are renamed by given function.
 * Renaming results are as static string.
 */
template <impl::lookup::member_transform_function<std::string> RenameFn>
consteval auto lookup_namespace_members_by_renaming_meta(
  std::meta::info ns, const RenameFn& rename_fn)
  -> std::vector<std::pair<std::meta::info, meta_string_view>>
{
  return lookup_namespace_members_by_renaming(ns, rename_fn)
    | std::views::transform([](const auto& pair) {
        auto static_rename = reflect_cpp26::define_static_string(pair.second);
        return std::pair{pair.first, static_rename};
      })
    | std::ranges::to<std::vector>();
}
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_LOOKUP_LOOKUP_HPP
