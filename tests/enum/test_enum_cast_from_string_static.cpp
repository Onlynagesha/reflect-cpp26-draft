#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

using namespace reflect_cpp26;

template <class E>
constexpr bool test_invalid_cases_common()
{
  static_assert(std::nullopt == enum_cast<E>("ZERO"));
  static_assert(std::nullopt == enum_cast<E>("hello_world"));
  static_assert(std::nullopt == enum_cast<E>(" zero "));
  static_assert(std::nullopt == enum_cast<E>("0"));
  static_assert(std::nullopt == enum_cast<E>(""));
  static_assert(std::nullopt == enum_cast<E>("-"));
  return true;
}

template <class E>
constexpr bool test_foo_signed_common()
{
  static_assert(E::four == enum_cast<E>("four"));
  static_assert(E::one == enum_cast<E>("one"));
  static_assert(E::error == enum_cast<E>("error"));
  static_assert(E::five == enum_cast<E>("five"));
  static_assert(E::zero == enum_cast<E>("zero"));
  static_assert(E::seven == enum_cast<E>("seven"));
  static_assert(E::invalid == enum_cast<E>("invalid"));
  static_assert(E::six == enum_cast<E>("six"));
  static_assert(E::two == enum_cast<E>("two"));
  return test_invalid_cases_common<E>();
}

static_assert(test_foo_signed_common<foo_signed>());
static_assert(test_foo_signed_common<foo_signed_reorder>());
static_assert(test_foo_signed_common<foo_signed_rep>());
static_assert(foo_signed_rep::one == enum_cast<foo_signed_rep>("yi"));
static_assert(foo_signed_rep::two == enum_cast<foo_signed_rep>("er"));

static_assert(bar_unsigned::fourteen == enum_cast<bar_unsigned>("fourteen"));
static_assert(bar_unsigned::twelve == enum_cast<bar_unsigned>("twelve"));
static_assert(bar_unsigned::one == enum_cast<bar_unsigned>("one"));
static_assert(bar_unsigned::thirteen == enum_cast<bar_unsigned>("thirteen"));
static_assert(bar_unsigned::zero == enum_cast<bar_unsigned>("zero"));
static_assert(bar_unsigned::error == enum_cast<bar_unsigned>("error"));
static_assert(bar_unsigned::ten == enum_cast<bar_unsigned>("ten"));
static_assert(bar_unsigned::eleven == enum_cast<bar_unsigned>("eleven"));
static_assert(bar_unsigned::three == enum_cast<bar_unsigned>("three"));
static_assert(bar_unsigned::invalid == enum_cast<bar_unsigned>("invalid"));
static_assert(bar_unsigned::two == enum_cast<bar_unsigned>("two"));

static_assert(baz_signed::san == enum_cast<baz_signed>("san"));
static_assert(baz_signed::er == enum_cast<baz_signed>("er"));
static_assert(baz_signed::yi == enum_cast<baz_signed>("yi"));
static_assert(baz_signed::ling == enum_cast<baz_signed>("ling"));
static_assert(baz_signed::fuyi == enum_cast<baz_signed>("fuyi"));

static_assert(test_invalid_cases_common<baz_signed>());
static_assert(std::nullopt == enum_cast<baz_signed>("one"));
static_assert(std::nullopt == enum_cast<baz_signed>("si"));

static_assert(qux_unsigned::er == enum_cast<qux_unsigned>("er"));
static_assert(qux_unsigned::fuyi == enum_cast<qux_unsigned>("fuyi"));
static_assert(qux_unsigned::yi == enum_cast<qux_unsigned>("yi"));
static_assert(qux_unsigned::san == enum_cast<qux_unsigned>("san"));
static_assert(qux_unsigned::ling == enum_cast<qux_unsigned>("ling"));
static_assert(qux_unsigned::si == enum_cast<qux_unsigned>("si"));

static_assert(test_invalid_cases_common<qux_unsigned>());
static_assert(std::nullopt == enum_cast<qux_unsigned>("3"));
static_assert(std::nullopt == enum_cast<qux_unsigned>("zero"));

static_assert(test_invalid_cases_common<empty>());
static_assert(std::nullopt == enum_cast<empty>("zero"));
static_assert(std::nullopt == enum_cast<empty>("invalid"));

static_assert(single::value == enum_cast<single>("value"));
static_assert(test_invalid_cases_common<single>());
static_assert(std::nullopt == enum_cast<single>("233"));

static_assert(single_rep::hahaha == enum_cast<single_rep>("laugh"));
static_assert(single_rep::laugh == enum_cast<single_rep>("hahaha"));
static_assert(single_rep::laugh == enum_cast<single_rep>("over"));
static_assert(single_rep::laugh == enum_cast<single_rep>("and_over"));
static_assert(single_rep::laugh == enum_cast<single_rep>("again"));
static_assert(single_rep::laugh == enum_cast<single_rep>("unstoppable"));

static_assert(test_invalid_cases_common<single_rep>());
static_assert(std::nullopt == enum_cast<single_rep>("value"));

static_assert(color::hot_pink == enum_cast<color>("hot_pink"));
static_assert(color::purple == enum_cast<color>("purple"));
static_assert(color::blanched_almond == enum_cast<color>("blanched_almond"));
static_assert(color::steel_blue == enum_cast<color>("steel_blue"));
static_assert(color::alice_blue == enum_cast<color>("alice_blue"));
static_assert(color::maroon == enum_cast<color>("maroon"));
static_assert(color::medium_blue == enum_cast<color>("medium_blue"));
static_assert(color::fire_brick == enum_cast<color>("fire_brick"));
static_assert(color::yellow_green == enum_cast<color>("yellow_green"));
static_assert(color::light_gray == enum_cast<color>("light_gray"));
static_assert(color::salmon == enum_cast<color>("salmon"));
static_assert(color::medium_slate_blue
  == enum_cast<color>("medium_slate_blue"));
static_assert(color::dark_turquoise == enum_cast<color>("dark_turquoise"));
static_assert(color::medium_sea_green == enum_cast<color>("medium_sea_green"));
static_assert(color::dark_sea_green == enum_cast<color>("dark_sea_green"));
static_assert(color::pale_green == enum_cast<color>("pale_green"));
static_assert(color::dark_orange == enum_cast<color>("dark_orange"));
static_assert(color::light_blue == enum_cast<color>("light_blue"));
static_assert(color::olive == enum_cast<color>("olive"));
static_assert(color::orange_red == enum_cast<color>("orange_red"));
static_assert(color::mint_cream == enum_cast<color>("mint_cream"));
static_assert(color::tomato == enum_cast<color>("tomato"));
static_assert(color::lavender_blush == enum_cast<color>("lavender_blush"));
static_assert(color::pale_violet_red == enum_cast<color>("pale_violet_red"));
static_assert(color::dark_olive_green == enum_cast<color>("dark_olive_green"));
static_assert(color::moccasin == enum_cast<color>("moccasin"));
static_assert(color::crimson == enum_cast<color>("crimson"));
static_assert(color::deep_sky_blue == enum_cast<color>("deep_sky_blue"));
static_assert(color::pink == enum_cast<color>("pink"));
static_assert(color::burly_wood == enum_cast<color>("burly_wood"));
static_assert(color::sienna == enum_cast<color>("sienna"));
static_assert(color::dark_golden_rod == enum_cast<color>("dark_golden_rod"));
static_assert(color::old_lace == enum_cast<color>("old_lace"));
static_assert(color::pale_golden_rod == enum_cast<color>("pale_golden_rod"));
static_assert(color::gainsboro == enum_cast<color>("gainsboro"));
static_assert(color::honey_dew == enum_cast<color>("honey_dew"));
static_assert(color::dark_green == enum_cast<color>("dark_green"));
static_assert(color::light_coral == enum_cast<color>("light_coral"));
static_assert(color::peach_puff == enum_cast<color>("peach_puff"));
static_assert(color::deep_pink == enum_cast<color>("deep_pink"));
static_assert(color::light_sea_green == enum_cast<color>("light_sea_green"));
static_assert(color::gray == enum_cast<color>("gray"));
static_assert(color::sky_blue == enum_cast<color>("sky_blue"));
static_assert(color::lime == enum_cast<color>("lime"));
static_assert(color::cornsilk == enum_cast<color>("cornsilk"));
static_assert(color::light_golden_rod_yellow
  == enum_cast<color>("light_golden_rod_yellow"));
static_assert(color::blue_violet == enum_cast<color>("blue_violet"));
static_assert(color::light_slate_gray == enum_cast<color>("light_slate_gray"));
static_assert(color::saddle_brown == enum_cast<color>("saddle_brown"));
static_assert(color::navy == enum_cast<color>("navy"));
static_assert(color::aquamarine == enum_cast<color>("aquamarine"));
static_assert(color::olive_drab == enum_cast<color>("olive_drab"));
static_assert(color::sandy_brown == enum_cast<color>("sandy_brown"));
static_assert(color::powder_blue == enum_cast<color>("powder_blue"));
static_assert(color::dark_gray == enum_cast<color>("dark_gray"));
static_assert(color::medium_purple == enum_cast<color>("medium_purple"));
static_assert(color::violet == enum_cast<color>("violet"));
static_assert(color::midnight_blue == enum_cast<color>("midnight_blue"));
static_assert(color::chartreuse == enum_cast<color>("chartreuse"));
static_assert(color::spring_green == enum_cast<color>("spring_green"));
static_assert(color::forest_green == enum_cast<color>("forest_green"));
static_assert(color::cadet_blue == enum_cast<color>("cadet_blue"));
static_assert(color::dim_gray == enum_cast<color>("dim_gray"));
static_assert(color::royal_blue == enum_cast<color>("royal_blue"));
static_assert(color::light_yellow == enum_cast<color>("light_yellow"));
static_assert(color::lemon_chiffon == enum_cast<color>("lemon_chiffon"));
static_assert(color::chocolate == enum_cast<color>("chocolate"));
static_assert(color::thistle == enum_cast<color>("thistle"));
static_assert(color::peru == enum_cast<color>("peru"));
static_assert(color::cornflower_blue == enum_cast<color>("cornflower_blue"));
static_assert(color::gold == enum_cast<color>("gold"));
static_assert(color::cyan == enum_cast<color>("cyan"));
static_assert(color::medium_turquoise == enum_cast<color>("medium_turquoise"));
static_assert(color::turquoise == enum_cast<color>("turquoise"));
static_assert(color::ghost_white == enum_cast<color>("ghost_white"));
static_assert(color::slate_gray == enum_cast<color>("slate_gray"));
static_assert(color::indian_red == enum_cast<color>("indian_red"));
static_assert(color::wheat == enum_cast<color>("wheat"));
static_assert(color::white == enum_cast<color>("white"));
static_assert(color::light_pink == enum_cast<color>("light_pink"));
static_assert(color::medium_spring_green
  == enum_cast<color>("medium_spring_green"));
static_assert(color::medium_aquamarine
  == enum_cast<color>("medium_aquamarine"));
static_assert(color::magenta == enum_cast<color>("magenta"));
static_assert(color::khaki == enum_cast<color>("khaki"));
static_assert(color::medium_violet_red
  == enum_cast<color>("medium_violet_red"));
static_assert(color::papaya_whip == enum_cast<color>("papaya_whip"));
static_assert(color::navajo_white == enum_cast<color>("navajo_white"));
static_assert(color::light_green == enum_cast<color>("light_green"));
static_assert(color::green == enum_cast<color>("green"));
static_assert(color::teal == enum_cast<color>("teal"));
static_assert(color::fuchsia == enum_cast<color>("fuchsia"));
static_assert(color::dodger_blue == enum_cast<color>("dodger_blue"));
static_assert(color::medium_orchid == enum_cast<color>("medium_orchid"));
static_assert(color::rebecca_purple == enum_cast<color>("rebecca_purple"));
static_assert(color::tan == enum_cast<color>("tan"));
static_assert(color::red == enum_cast<color>("red"));
static_assert(color::indigo == enum_cast<color>("indigo"));
static_assert(color::ivory == enum_cast<color>("ivory"));
static_assert(color::dark_khaki == enum_cast<color>("dark_khaki"));
static_assert(color::green_yellow == enum_cast<color>("green_yellow"));
static_assert(color::brown == enum_cast<color>("brown"));
static_assert(color::misty_rose == enum_cast<color>("misty_rose"));
static_assert(color::pale_turquoise == enum_cast<color>("pale_turquoise"));
static_assert(color::bisque == enum_cast<color>("bisque"));
static_assert(color::snow == enum_cast<color>("snow"));
static_assert(color::rosy_brown == enum_cast<color>("rosy_brown"));
static_assert(color::dark_magenta == enum_cast<color>("dark_magenta"));
static_assert(color::antique_white == enum_cast<color>("antique_white"));
static_assert(color::light_salmon == enum_cast<color>("light_salmon"));
static_assert(color::orchid == enum_cast<color>("orchid"));
static_assert(color::silver == enum_cast<color>("silver"));
static_assert(color::light_steel_blue == enum_cast<color>("light_steel_blue"));
static_assert(color::linen == enum_cast<color>("linen"));
static_assert(color::floral_white == enum_cast<color>("floral_white"));
static_assert(color::white_smoke == enum_cast<color>("white_smoke"));
static_assert(color::dark_salmon == enum_cast<color>("dark_salmon"));
static_assert(color::orange == enum_cast<color>("orange"));
static_assert(color::black == enum_cast<color>("black"));
static_assert(color::sea_green == enum_cast<color>("sea_green"));
static_assert(color::light_sky_blue == enum_cast<color>("light_sky_blue"));
static_assert(color::dark_violet == enum_cast<color>("dark_violet"));
static_assert(color::dark_slate_gray == enum_cast<color>("dark_slate_gray"));
static_assert(color::aqua == enum_cast<color>("aqua"));
static_assert(color::dark_blue == enum_cast<color>("dark_blue"));
static_assert(color::blue == enum_cast<color>("blue"));
static_assert(color::slate_blue == enum_cast<color>("slate_blue"));
static_assert(color::dark_red == enum_cast<color>("dark_red"));
static_assert(color::lavender == enum_cast<color>("lavender"));
static_assert(color::plum == enum_cast<color>("plum"));
static_assert(color::sea_shell == enum_cast<color>("sea_shell"));
static_assert(color::yellow == enum_cast<color>("yellow"));
static_assert(color::coral == enum_cast<color>("coral"));
static_assert(color::azure == enum_cast<color>("azure"));
static_assert(color::dark_orchid == enum_cast<color>("dark_orchid"));
static_assert(color::lawn_green == enum_cast<color>("lawn_green"));
static_assert(color::light_cyan == enum_cast<color>("light_cyan"));
static_assert(color::lime_green == enum_cast<color>("lime_green"));
static_assert(color::dark_slate_blue == enum_cast<color>("dark_slate_blue"));
static_assert(color::beige == enum_cast<color>("beige"));
static_assert(color::dark_cyan == enum_cast<color>("dark_cyan"));
static_assert(color::golden_rod == enum_cast<color>("golden_rod"));
static_assert(color::fuchsia == enum_cast<color>("magenta"));
static_assert(test_invalid_cases_common<color>());

static_assert(terminal_color::blue == enum_cast<terminal_color>("blue"));
static_assert(terminal_color::bright_yellow
  == enum_cast<terminal_color>("bright_yellow"));
static_assert(terminal_color::green == enum_cast<terminal_color>("green"));
static_assert(terminal_color::cyan == enum_cast<terminal_color>("cyan"));
static_assert(terminal_color::bright_green
  == enum_cast<terminal_color>("bright_green"));
static_assert(terminal_color::black == enum_cast<terminal_color>("black"));
static_assert(terminal_color::bright_white
  == enum_cast<terminal_color>("bright_white"));
static_assert(terminal_color::yellow == enum_cast<terminal_color>("yellow"));
static_assert(terminal_color::bright_blue
  == enum_cast<terminal_color>("bright_blue"));
static_assert(terminal_color::bright_red
  == enum_cast<terminal_color>("bright_red"));
static_assert(terminal_color::red == enum_cast<terminal_color>("red"));
static_assert(terminal_color::bright_magenta
  == enum_cast<terminal_color>("bright_magenta"));
static_assert(terminal_color::bright_black
  == enum_cast<terminal_color>("bright_black"));
static_assert(terminal_color::white == enum_cast<terminal_color>("white"));
static_assert(terminal_color::magenta == enum_cast<terminal_color>("magenta"));

static_assert(test_invalid_cases_common<terminal_color>());
static_assert(std::nullopt == enum_cast<terminal_color>("fuchsia"));

static_assert(hash_collision::_wSYZDRpiQJf8Rfv
  == enum_cast<hash_collision>("_wSYZDRpiQJf8Rfv"));
static_assert(hash_collision::_cuFFJIHGp_jNJKS
  == enum_cast<hash_collision>("_cuFFJIHGp_jNJKS"));
static_assert(test_invalid_cases_common<hash_collision>());

TEST(EnumCastFromString, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
