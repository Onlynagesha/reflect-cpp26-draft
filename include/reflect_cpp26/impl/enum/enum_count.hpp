#ifndef REFLECT_CPP26_IMPL_ENUM_ENUM_COUNT_HPP
#define REFLECT_CPP26_IMPL_ENUM_ENUM_COUNT_HPP

#include <reflect_cpp26/utils/config.h>

namespace reflect_cpp26::impl {
template <class E>
constexpr auto enum_count_v = enumerators_of(^^E).size();
} // namespace reflect_cpp26::impl

#endif // REFLECT_CPP26_IMPL_ENUM_ENUM_COUNT_HPP
