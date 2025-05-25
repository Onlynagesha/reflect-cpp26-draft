#ifndef REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_NAME_HPP
#define REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_NAME_HPP

#include <reflect_cpp26/lookup/lookup.hpp>

namespace reflect_cpp26 {
template <class Pointer>
struct identifier_lookup_table_by_name {
  using value_type = Pointer;

  struct entry_t {
    meta_string_view key;
    Pointer value;
  };
  // Sorted in lexicological order of key
  meta_span<entry_t> entries;

  // Returns nullptr if not found
  constexpr auto operator[](std::string_view key) const -> Pointer
  {
    auto [head, tail] = entries;
    while (head < tail) {
      auto mid = head + (tail - head) / 2;
      if (key == mid->key) {
        return mid->value;
      }
      (key < mid->key) ? (tail = mid) : (head = mid + 1);
    }
    return nullptr;
  }

  constexpr auto size() const -> size_t {
    return entries.size();
  }
};

namespace impl::lookup::by_name {
template <class Target, class MemberItems>
consteval auto make_member_table(MemberItems member_items)
  /* -> identifier_lookup_table_by_key<TargetType> */
{
  using ResultType = identifier_lookup_table_by_name<Target>;
  using ResultEntryType = typename ResultType::entry_t;

  auto entries = std::vector<ResultEntryType>{};
  entries.reserve(member_items.size());

  member_items.for_each([&entries](auto item) {
    auto cur = &[:item.value.first:];
    static_assert(std::is_convertible_v<decltype(cur), Target>,
      "Inconsistent types.");
    entries.push_back({.key = item.value.second, .value = cur});
  });
  std::ranges::sort(entries, {}, &ResultEntryType::key);
  // Checks duplicated keys
  auto pos = std::ranges::adjacent_find(entries, {}, &ResultEntryType::key);
  if (entries.end() != pos) {
    compile_error("Duplicated keys detected.");
  }
  return ResultType{.entries = reflect_cpp26::define_static_array(entries)};
}

template <class T, class MemberItems>
consteval auto make_type_member_table(MemberItems member_items)
{
  static_assert(member_items.size() > 0, "Member list can not be empty.");
  constexpr auto first_member = get<0>(member_items).first;
  using MemberType = [: type_of(first_member) :];

  if constexpr (is_static_member(first_member)) {
    using TargetType = MemberType*;
    return make_member_table<TargetType>(member_items);
  } else {
    using TargetType = MemberType T::*;
    return make_member_table<TargetType>(member_items);
  }
}

template <class MemberItems>
consteval auto make_namespace_member_table(MemberItems member_items)
{
  static_assert(member_items.size() > 0, "Member list can not be empty.");
  using MemberType = [: type_of(get<0>(member_items).first) :];
  using TargetType = MemberType*;
  return make_member_table<TargetType>(member_items);
}

consteval auto rename_member_table_entry_by_pattern(
  std::string_view prefix, std::string_view suffix, std::string_view name)
  -> std::optional<std::string>
{
  if (!name.starts_with(prefix) || !name.ends_with(suffix)) {
    return std::nullopt;
  }
  name.remove_prefix(prefix.length());
  name.remove_suffix(suffix.length());
  return std::string{name};
}

consteval auto get_type_member_table_entries(
  std::meta::info T, std::string_view prefix, std::string_view suffix)
{
  return lookup_type_members_by_renaming_meta(T,
    [prefix, suffix](std::string_view name) {
      return rename_member_table_entry_by_pattern(prefix, suffix, name);
    });
}

consteval auto get_namespace_member_table_entries(
  std::meta::info ns, std::string_view prefix, std::string_view suffix)
{
  return lookup_namespace_members_by_renaming_meta(ns,
    [prefix, suffix](std::meta::info member) {
      if (is_type(member) || is_namespace(member) || !has_identifier(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member));
    });
}

consteval auto get_type_member_table_entries(
  std::meta::info T, lookup_filter_flags_t filter, std::string_view prefix,
  std::string_view suffix)
{
   return lookup_type_members_by_renaming_meta(T,
    [filter, prefix, suffix](std::meta::info member) {
      if (!has_identifier(member) || !filter.test_type_member(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member));
    });
}

consteval auto get_namespace_member_table_entries(
  std::meta::info ns, lookup_filter_flags_t filter, std::string_view prefix,
  std::string_view suffix)
{
   return lookup_namespace_members_by_renaming_meta(ns,
    [filter, prefix, suffix](std::meta::info member) {
      if (is_type(member) || is_namespace(member) || !has_identifier(member) ||
          !filter.test_namespace_member(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member));
    });
}

template <member_transform_function<std::string> RenameFn>
consteval auto get_type_member_table_entries(
  std::meta::info T, const RenameFn& rename_fn)
{
  return lookup_type_members_by_renaming_meta(T, rename_fn);
}

template <member_transform_function<std::string> RenameFn>
consteval auto get_namespace_member_table_entries(
  std::meta::info ns, const RenameFn& rename_fn)
{
  return lookup_namespace_members_by_renaming_meta(ns,
    [&rename_fn](std::meta::info member) -> std::optional<std::string> {
      if (is_type(member) || is_namespace(member)) {
        return std::nullopt;
      }
      if constexpr (std::is_invocable_v<RenameFn, std::meta::info>) {
        return rename_fn(member);
      } else {
        static_assert(std::is_invocable_v<RenameFn, std::string_view>);
        if (has_identifier(member)) {
          return rename_fn(identifier_of(member));
        }
        return std::nullopt;
      }
    });
}
} // namespace impl::lookup::by_name
} // namespace reflect_cpp26

/**
 * Makes a lookup table of members of class type T (including those inherited
 * from bases of T).
 * See tests/lookup/test_class_lookup_table_by_name.cpp for examples.
 */
#define REFLECT_CPP26_TYPE_LOOKUP_TABLE_BY_NAME(T, ...)                     \
  []() consteval {                                                          \
    constexpr auto member_items = REFLECT_CPP26_EXPAND(                     \
      reflect_cpp26::impl::lookup::by_name::get_type_member_table_entries(  \
        ^^T, __VA_ARGS__));                                                 \
    return reflect_cpp26::impl::lookup::by_name::make_type_member_table<T>( \
      member_items);                                                        \
  }();

/**
 * Makes a lookup table of direct members of namespace ns.
 * Types are nested namespaces are excluded.
 * See tests/lookup/test_namespace_lookup_table_by_name.cpp for examples.
 */
#define REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_NAME(ns, ...)               \
  []() consteval {                                                          \
    constexpr auto member_items = REFLECT_CPP26_EXPAND(                     \
      reflect_cpp26::impl::lookup::by_name::                                \
        get_namespace_member_table_entries(^^ns, __VA_ARGS__));             \
    return reflect_cpp26::impl::lookup::by_name::                           \
      make_namespace_member_table(member_items);                            \
  }();

#endif // REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_NAME_HPP
