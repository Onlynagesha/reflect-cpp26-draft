#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_for_each.hpp>
#endif

using namespace reflect_cpp26;

template <auto B>
struct some_type_t {
  static constexpr auto value =
    std::to_underlying(B) * std::to_underlying(B) + 2000;
};

TEST(EnumForEach, All)
{
  constexpr auto some_type_dec_baz_signed_1 = []() {
    auto dec = 0;
    enum_for_each<baz_signed>([&dec](auto v) {
      dec = dec * 10 + std::to_underlying(v.value);
    });
    return dec;
  };
  EXPECT_EQ(1229, some_type_dec_baz_signed_1());

  constexpr auto some_type_dec_baz_signed_2 = []() {
    auto dec = 0;
    enum_for_each<baz_signed, enum_entry_order::by_name>([&dec](auto v) {
      dec = dec * 10 + std::to_underlying(v.value);
    });
    return dec;
  };
  EXPECT_EQ(19031, some_type_dec_baz_signed_2());

  constexpr auto some_type_sum_dec_baz_signed = []() {
    auto sum = 0;
    auto dec = 0;
    enum_for_each<baz_signed, enum_entry_order::by_value>([&sum, &dec](auto v) {
      sum += some_type_t<v.value>::value;
      dec = dec * 10 + std::to_underlying(v.value);
    });
    return std::pair{sum, dec};
  }();
  EXPECT_EQ(10015, some_type_sum_dec_baz_signed.first);
  EXPECT_EQ(-9877, some_type_sum_dec_baz_signed.second);
}
