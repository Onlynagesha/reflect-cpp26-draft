#ifndef REFLECT_CPP26_TYPE_OPERATIONS_DEFINE_AGGREGATE_HPP
#define REFLECT_CPP26_TYPE_OPERATIONS_DEFINE_AGGREGATE_HPP

#include <reflect_cpp26/type_traits/class_types/flattenable.hpp>
#include <reflect_cpp26/type_traits/is_invocable.hpp>
#include <reflect_cpp26/utils/concepts.hpp>

namespace reflect_cpp26 {
// (std::meta::info...) -> std::meta::info
template <class F, size_t N>
concept transformer_invocable_with_n_metas =
  std::is_default_constructible_v<F>
    && is_invocable_r_n_v<std::meta::info, F, std::meta::info, N>;

// (flattened_data_member_spec...) -> std::meta::info
template <class F, size_t N>
concept transformer_invocable_with_n_specs =
  std::is_default_constructible_v<F>
    && is_invocable_r_n_v<std::meta::info, F, flattened_data_member_spec, N>;

namespace impl {
template <class Transform, class... Ts>
  requires (transformer_invocable_with_n_metas<Transform, sizeof...(Ts)>)
struct aggregate_by_direct_memberwise {
  struct type;

  consteval {
    constexpr auto min_member_size =
      std::ranges::min({public_direct_nsdm_of(^^Ts).size()...});
    auto members = std::vector<std::meta::info>{};
    members.reserve(min_member_size);

    for (auto i = 0zU; i < min_member_size; i++) {
      auto cur = std::invoke(Transform{}, public_direct_nsdm_of(^^Ts)[i]...);
      if (!is_data_member_spec(cur)) {
        compile_error("Transform function result must be data member spec.");
      }
      members.push_back(cur);
    }
    define_aggregate(^^type, members);
  }
};

template <class Transform, class... Ts>
  requires (transformer_invocable_with_n_specs<Transform, sizeof...(Ts)>
         || transformer_invocable_with_n_metas<Transform, sizeof...(Ts)>)
struct aggregate_by_flattened_memberwise {
  static constexpr auto invocable_with_spec =
    transformer_invocable_with_n_specs<Transform, sizeof...(Ts)>;

  static consteval auto ith_member_getter(size_t i) -> std::meta::info
  {
    if constexpr (invocable_with_spec) {
      return std::invoke(
        Transform{}, public_flattened_nsdm_v<Ts>.values[i]...);
    } else {
      return std::invoke(
        Transform{}, public_flattened_nsdm_v<Ts>.values[i].member...);
    }
  }

  struct type;

  consteval {
    constexpr auto min_member_size =
      std::ranges::min({public_flattened_nsdm_v<Ts>.size()...});
    auto members = std::vector<std::meta::info>{};
    members.reserve(min_member_size);

    for (auto i = 0zU; i < min_member_size; i++) {
      auto cur = ith_member_getter(i);
      if (!is_data_member_spec(cur)) {
        compile_error("Transform function result must be data member spec.");
      }
      members.push_back(cur);
    }
    define_aggregate(^^type, members);
  }
};
} // namespace impl

/**
  * Makes aggregate type whose j-th data member spec is
  *   Predicate{}.operator()(
  *     jth-direct-member-of(Ts...[0], j), ...,
  *     jth-direct-member-of(Ts...[K - 1], j)) for j = 0 to N - 1 where
  * N = min(count of direct accessible NSDMs of T) for T in Ts...
  * K = sizeof...(Ts)
  */
template <class Transform, class_type... Ts>
using aggregate_by_direct_memberwise_t =
  typename impl::aggregate_by_direct_memberwise<
    Transform, std::remove_cv_t<Ts>...>::type;

/**
  * Makes aggregate type whose j-th data member spec is
  *   Predicate{}.operator()(
  *     jth-flattened-member-of(Ts...[0], j), ...,
  *     jth-flattened-member-of(Ts...[K - 1], j)) for j = 0 to N - 1 where
  * N = min(count of flattened accessible NSDMs of T) for T in Ts...
  * K = sizeof...(Ts)
  */
template <class Transform, partially_flattenable... Ts>
using aggregate_by_flattened_memberwise_t =
  typename impl::aggregate_by_flattened_memberwise<
    Transform, std::remove_cv_t<Ts>...>::type;
} // namespace reflect_cpp26

#endif // REFLECT_CPP26_TYPE_OPERATIONS_DEFINE_AGGREGATE_HPP
