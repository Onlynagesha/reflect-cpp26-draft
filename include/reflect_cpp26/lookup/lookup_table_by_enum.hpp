#ifndef REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_ENUM_HPP
#define REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_ENUM_HPP

#include <reflect_cpp26/enum/enum_cast.hpp>
#include <reflect_cpp26/lookup/lookup.hpp>
#include <reflect_cpp26/utils/identifier_naming.hpp>

namespace reflect_cpp26 {
template <enum_type E, class Pointer>
struct identifier_lookup_table_by_enum {
  using key_type = E;
  using value_type = Pointer;

  struct entry_t {
    E key;
    Pointer value;
  };
  // Sorted by underlying value of key
  meta_span<entry_t> entries;

  // Returns nullptr if not found
  constexpr auto operator[](E key) const -> Pointer
  {
    auto [head, tail] = entries;
    while (head < tail) {
      auto mid = head + (tail - head) / 2;
      if (key == mid->key) {
        return mid->value;
      }
      (std::to_underlying(key) < std::to_underlying(mid->key))
        ? (tail = mid) : (head = mid + 1);
    }
    return nullptr;
  }

  constexpr auto size() const -> size_t {
    return entries.size();
  }
};

namespace impl::lookup::by_enum {
template <class E, class Target, class MemberItems>
consteval auto make_member_table(MemberItems member_items)
{
  using ResultType = identifier_lookup_table_by_enum<E, Target>;
  using ResultEntryType = typename ResultType::entry_t;

  auto entries = std::vector<ResultEntryType>{};
  entries.reserve(member_items.size());

  member_items.for_each([&entries](auto item) {
    auto cur = &[:item.value.first:];
    static_assert(std::is_convertible_v<decltype(cur), Target>,
      "Inconsistent types.");
    if constexpr (std::is_enum_v<decltype(item.value.second)>) {
      entries.push_back({.key = item.value.second, .value = cur});
    } else {
      auto e = enum_cast<E>(item.value.second);
      if (e.has_value()) {
        entries.push_back({.key = *e, .value = cur});
      }
    }
  });
  std::ranges::sort(entries, [](const auto& lhs, const auto& rhs) {
    return std::to_underlying(lhs.key) < std::to_underlying(rhs.key);
  });
  // Checks duplicated keys
  auto pos = std::ranges::adjacent_find(entries, {}, &ResultEntryType::key);
  if (entries.end() != pos) {
    compile_error("Duplicated keys detected.");
  }
  return ResultType{.entries = reflect_cpp26::define_static_array(entries)};
}

template <class T, class E, class MemberItems>
consteval auto make_type_member_table(MemberItems member_items)
{
  static_assert(member_items.size() > 0, "Member list can not be empty.");
  constexpr auto first_member = get<0>(member_items).first;
  using MemberType = [: type_of(first_member) :];

  if constexpr (is_static_member(first_member)) {
    using TargetType = MemberType*;
    return make_member_table<E, TargetType>(member_items);
  } else {
    using TargetType = MemberType T::*;
    return make_member_table<E, TargetType>(member_items);
  }
}

template <class E, class MemberItems>
consteval auto make_namespace_member_table(MemberItems member_items)
{
  static_assert(member_items.size() > 0, "Member list can not be empty.");
  using MemberType = [: type_of(get<0>(member_items).first) :];
  using TargetType = MemberType*;
  return make_member_table<E, TargetType>(member_items);
}

consteval auto rename_member_table_entry_by_pattern(
  std::string_view prefix, std::string_view suffix, std::string_view name,
  identifier_naming_rule enum_naming_rule) -> std::optional<std::string>
{
  if (name.length() <= prefix.length() + suffix.length()) {
    return std::nullopt;
  }
  if (!name.starts_with(prefix) || !name.ends_with(suffix)) {
    return std::nullopt;
  }
  name.remove_prefix(prefix.length());
  name.remove_suffix(suffix.length());
  return convert_identifier(name, enum_naming_rule);
}

consteval auto get_type_member_table_entries(
  std::meta::info T, std::string_view prefix, std::string_view suffix,
  identifier_naming_rule enum_naming_rule = identifier_naming_rule::no_change)
{
  return lookup_type_members_by_renaming_meta(T,
    [prefix, suffix, enum_naming_rule](std::string_view name) {
      return rename_member_table_entry_by_pattern(
        prefix, suffix, name, enum_naming_rule);
    });
}

consteval auto get_namespace_member_table_entries(
  std::meta::info ns, std::string_view prefix, std::string_view suffix,
  identifier_naming_rule enum_naming_rule = identifier_naming_rule::no_change)
{
  return lookup_namespace_members_by_renaming_meta(ns,
    [prefix, suffix, enum_naming_rule](std::meta::info member) {
      if (is_type(member) || is_namespace(member) || !has_identifier(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member), enum_naming_rule);
    });
}

consteval auto get_type_member_table_entries(
  std::meta::info T, lookup_filter_flags_t filter,
  std::string_view prefix, std::string_view suffix,
  identifier_naming_rule enum_naming_rule = identifier_naming_rule::no_change)
{
   return lookup_type_members_by_renaming_meta(T,
    [filter, prefix, suffix, enum_naming_rule](std::meta::info member) {
      if (!has_identifier(member) || !filter.test_type_member(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member), enum_naming_rule);
    });
}

consteval auto get_namespace_member_table_entries(
  std::meta::info ns, lookup_filter_flags_t filter,
  std::string_view prefix, std::string_view suffix,
   identifier_naming_rule enum_naming_rule = identifier_naming_rule::no_change)
{
   return lookup_namespace_members_by_renaming_meta(ns,
    [filter, prefix, suffix, enum_naming_rule](std::meta::info member) {
      if (is_type(member) || is_namespace(member) || !has_identifier(member) ||
          !filter.test_namespace_member(member)) {
        return std::optional<std::string>{};
      }
      return rename_member_table_entry_by_pattern(
        prefix, suffix, identifier_of(member), enum_naming_rule);
    });
}
} // namespace impl::lookup::by_enum
} // namespace reflect_cpp26

/**
 * Makes a lookup table of members of class type T (including those inherited
 * from bases of T).
 * See tests/lookup/test_class_lookup_table_by_enum.cpp for examples.
 */
#define REFLECT_CPP26_TYPE_LOOKUP_TABLE_BY_ENUM(T, E, ...)                  \
  []() consteval {                                                          \
    static_assert(std::is_enum_v<E>, "E must be enum type.");               \
    constexpr auto member_items = REFLECT_CPP26_EXPAND(                     \
      reflect_cpp26::impl::lookup::by_enum::get_type_member_table_entries(  \
        ^^T, __VA_ARGS__));                                                 \
    return reflect_cpp26::impl::lookup::by_enum::                           \
      make_type_member_table<T, E>(member_items);                           \
  }();

#define REFLECT_CPP26_TYPE_LOOKUP_TABLE_BY_ENUM_ALL(T, E, ...)              \
  []() consteval {                                                          \
    constexpr auto res = REFLECT_CPP26_TYPE_LOOKUP_TABLE_BY_ENUM(           \
      T, E, __VA_ARGS__);                                                   \
    static_assert(res.size() == reflect_cpp26::enum_unique_count<E>(),      \
      "Expects all unique entries of E to be covered.");                    \
    return res;                                                             \
  }();

/**
 * Makes a lookup table of direct members of namespace ns.
 * Types are nested namespaces are excluded.
 * See tests/lookup/test_namespace_lookup_table_by_enum.cpp for examples.
 */
#define REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_ENUM(ns, E, ...)            \
  []() consteval {                                                          \
    static_assert(std::is_enum_v<E>, "E must be enum type.");               \
    constexpr auto member_items = REFLECT_CPP26_EXPAND(                     \
      reflect_cpp26::impl::lookup::by_enum::                                \
        get_namespace_member_table_entries(^^ns, __VA_ARGS__));             \
    return reflect_cpp26::impl::lookup::by_enum::                           \
      make_namespace_member_table<E>(member_items);                         \
  }();

#define REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_ENUM_ALL(ns, E, ...)        \
  []() consteval {                                                          \
    constexpr auto res = REFLECT_CPP26_NAMESPACE_LOOKUP_TABLE_BY_ENUM(      \
      ns, E, __VA_ARGS__);                                                  \
    static_assert(res.size() == reflect_cpp26::enum_unique_count<E>(),      \
      "Expects all unique entries of E to be covered.");                    \
    return res;                                                             \
  }();

#endif // REFLECT_CPP26_LOOKUP_LOOKUP_TABLE_BY_ENUM_HPP
