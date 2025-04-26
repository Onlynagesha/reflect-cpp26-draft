#include <experimental/meta>
#include <functional>
#include <print>
#include <type_traits>

// -------- reflect_cpp26/utils/config.h --------

[[noreturn]] int compile_error(const char* msg) noexcept;

// -------- reflect_cpp26/utils/constants.hpp --------

template <auto... Vs>
struct constant;

constexpr auto npos = std::numeric_limits<size_t>::max();

namespace impl {
template <template <auto...> class Derived, auto... Vs>
struct constant_base {
private:
  template <size_t I, class Func>
  static constexpr void for_each_impl(const Func& body)
  {
    if constexpr (I < sizeof...(Vs)) {
      std::invoke(body, Derived<Vs...[I]>{});
      for_each_impl<I + 1>(body);
    }
  }

public:
  template <class Func>
  static constexpr void for_each(Func&& body) {
    for_each_impl<0>(body);
  }
};
} // namespace impl

template <auto... Vs>
struct constant : impl::constant_base<constant, Vs...> {};

template <auto V>
struct constant<V> : impl::constant_base<constant, V> {
  using type = decltype(V);
  static constexpr auto value = V;
  static constexpr auto values = std::array{V};

  constexpr operator type() const {
    return V;
  }
};

// -------- reflect_cpp26/utils/expand.hpp --------

#define REFLECT_CPP26_EXPAND(...) \
  [: ::make_for_each_replicator(__VA_ARGS__) :]

namespace impl {
template<auto... Vs>
constexpr auto for_each_replicator = constant<Vs...>{};
} // namespace impl

template<class Range>
  requires (std::ranges::input_range<Range>)
consteval auto make_for_each_replicator(const Range& range)
{
  return substitute(
    ^^impl::for_each_replicator,
    range | std::views::transform([](const auto& r) {
      return std::meta::reflect_value(r);
    }));
}

template <class T>
consteval auto make_for_each_replicator(std::initializer_list<T> range)
{
  return substitute(
    ^^impl::for_each_replicator,
    range | std::views::transform([](const auto& r) {
      return std::meta::reflect_value(r);
    }));
}

// -------- reflect_cpp26/utils/meta_utility.hpp --------

consteval auto public_direct_bases_of(std::meta::info T) {
  return std::meta::bases_of(T, std::meta::access_context::unprivileged());
}

consteval auto public_direct_nsdm_of(std::meta::info T)
{
  return std::meta::nonstatic_data_members_of(
    T, std::meta::access_context::unprivileged());
}

// -------- reflect_cpp26/type_traits/class_types/flattenable.hpp --------

namespace impl {
template <class T>
consteval auto walk_public_nsdm() -> std::meta::info;

template <class T>
using public_flattened_nsdm_t = [: walk_public_nsdm<T>() :];
} // namespace impl

struct flattened_data_member_spec {
  std::meta::info member;
  std::meta::member_offset actual_offset;
};

template <class T>
constexpr auto public_flattened_nsdm_v = impl::public_flattened_nsdm_t<T>{};

namespace impl {
template <class T>
consteval auto walk_public_nsdm() -> std::meta::info
{
  auto members = std::vector<std::meta::info>{};
  REFLECT_CPP26_EXPAND(public_direct_bases_of(^^T)).for_each(
    [&members](auto base) {
      if (is_virtual(base.value)) {
        compile_error("Virtual inheritance is disallowed.");
      }
      constexpr auto base_offset = offset_of(base.value).bytes;

      // The following is OK:
      // constexpr auto from_base =
      //   public_flattened_nsdm_v<[:type_of(base.value):]>;
      // from_base.for_each([&members](flattened_data_member_spec sp) { ... }

      using B = public_flattened_nsdm_t<[:type_of(base.value):]>;
      REFLECT_CPP26_EXPAND(template_arguments_of(dealias(^^B))).for_each(
        [&members](flattened_data_member_spec sp) {
          sp.actual_offset.bytes += base_offset;
          members.push_back(std::meta::reflect_value(sp));
        });
    });
  REFLECT_CPP26_EXPAND(public_direct_nsdm_of(^^T))
    .for_each([&members](auto cur_member) {
      auto cur_member_spec = flattened_data_member_spec{
        .member = cur_member,
        .actual_offset = offset_of(cur_member.value),
      };
      members.push_back(std::meta::reflect_value(cur_member_spec));
    });
  return substitute(^^constant, members);
}
} // namespace impl

// -------- Demo test case --------

struct foo_t {
  int a;
  int b;
};
struct bar_t : foo_t {
  int c;
  int d;
};

int main()
{
  public_flattened_nsdm_v<bar_t>.for_each([](auto m) {
    std::println("{}: {}",
      identifier_of(m.value.member), m.value.actual_offset.bytes);
  });
  return 0;
}
