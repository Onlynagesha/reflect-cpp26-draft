// No header guard since this file is intended to be included only once.

#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

#ifndef TEST_SUITE_NAME
#error "Test suite name is missing."
#endif

using namespace reflect_cpp26;

template <class E>
void test_invalid_cases_common()
{
  ASSERT_EQ(std::nullopt, enum_cast<E>("ZERO"));
  ASSERT_EQ(std::nullopt, enum_cast<E>("hello_world"));
  ASSERT_EQ(std::nullopt, enum_cast<E>(" zero "));
  ASSERT_EQ(std::nullopt, enum_cast<E>("0"));
  ASSERT_EQ(std::nullopt, enum_cast<E>(""));
  ASSERT_EQ(std::nullopt, enum_cast<E>("-"));
}

template <class E>
void test_foo_signed_common()
{
  ASSERT_EQ(E::four, enum_cast<E>("four"));
  ASSERT_EQ(E::one, enum_cast<E>("one"));
  ASSERT_EQ(E::error, enum_cast<E>("error"));
  ASSERT_EQ(E::five, enum_cast<E>("five"));
  ASSERT_EQ(E::zero, enum_cast<E>("zero"));
  ASSERT_EQ(E::seven, enum_cast<E>("seven"));
  ASSERT_EQ(E::invalid, enum_cast<E>("invalid"));
  ASSERT_EQ(E::six, enum_cast<E>("six"));
  ASSERT_EQ(E::two, enum_cast<E>("two"));

  test_invalid_cases_common<E>();
}

TEST(TEST_SUITE_NAME, FooSigned) {
  test_foo_signed_common<foo_signed>();
}

TEST(TEST_SUITE_NAME, FooSignedReorder) {
  test_foo_signed_common<foo_signed_reorder>();
}

TEST(TEST_SUITE_NAME, FooSignedRep)
{
  test_foo_signed_common<foo_signed_rep>();
  ASSERT_EQ(foo_signed_rep::one, enum_cast<foo_signed_rep>("yi"));
  ASSERT_EQ(foo_signed_rep::two, enum_cast<foo_signed_rep>("er"));
}

TEST(TEST_SUITE_NAME, BarUnsigned)
{
  ASSERT_EQ(bar_unsigned::fourteen, enum_cast<bar_unsigned>("fourteen"));
  ASSERT_EQ(bar_unsigned::twelve, enum_cast<bar_unsigned>("twelve"));
  ASSERT_EQ(bar_unsigned::one, enum_cast<bar_unsigned>("one"));
  ASSERT_EQ(bar_unsigned::thirteen, enum_cast<bar_unsigned>("thirteen"));
  ASSERT_EQ(bar_unsigned::zero, enum_cast<bar_unsigned>("zero"));
  ASSERT_EQ(bar_unsigned::error, enum_cast<bar_unsigned>("error"));
  ASSERT_EQ(bar_unsigned::ten, enum_cast<bar_unsigned>("ten"));
  ASSERT_EQ(bar_unsigned::eleven, enum_cast<bar_unsigned>("eleven"));
  ASSERT_EQ(bar_unsigned::three, enum_cast<bar_unsigned>("three"));
  ASSERT_EQ(bar_unsigned::invalid, enum_cast<bar_unsigned>("invalid"));
  ASSERT_EQ(bar_unsigned::two, enum_cast<bar_unsigned>("two"));

  test_invalid_cases_common<bar_unsigned>();
}

TEST(TEST_SUITE_NAME, BazSigned)
{
  ASSERT_EQ(baz_signed::san, enum_cast<baz_signed>("san"));
  ASSERT_EQ(baz_signed::er, enum_cast<baz_signed>("er"));
  ASSERT_EQ(baz_signed::yi, enum_cast<baz_signed>("yi"));
  ASSERT_EQ(baz_signed::ling, enum_cast<baz_signed>("ling"));
  ASSERT_EQ(baz_signed::fuyi, enum_cast<baz_signed>("fuyi"));

  test_invalid_cases_common<baz_signed>();
  ASSERT_EQ(std::nullopt, enum_cast<baz_signed>("one"));
  ASSERT_EQ(std::nullopt, enum_cast<baz_signed>("si"));
}

TEST(TEST_SUITE_NAME, QuxUnsigned)
{
  ASSERT_EQ(qux_unsigned::er, enum_cast<qux_unsigned>("er"));
  ASSERT_EQ(qux_unsigned::fuyi, enum_cast<qux_unsigned>("fuyi"));
  ASSERT_EQ(qux_unsigned::yi, enum_cast<qux_unsigned>("yi"));
  ASSERT_EQ(qux_unsigned::san, enum_cast<qux_unsigned>("san"));
  ASSERT_EQ(qux_unsigned::ling, enum_cast<qux_unsigned>("ling"));
  ASSERT_EQ(qux_unsigned::si, enum_cast<qux_unsigned>("si"));

  test_invalid_cases_common<qux_unsigned>();
  ASSERT_EQ(std::nullopt, enum_cast<qux_unsigned>("3"));
  ASSERT_EQ(std::nullopt, enum_cast<qux_unsigned>("zero"));
}

TEST(TEST_SUITE_NAME, Empty)
{
  test_invalid_cases_common<empty>();
  ASSERT_EQ(std::nullopt, enum_cast<empty>("zero"));
  ASSERT_EQ(std::nullopt, enum_cast<empty>("invalid"));
}

TEST(TEST_SUITE_NAME, Single)
{
  ASSERT_EQ(single::value, enum_cast<single>("value"));
  test_invalid_cases_common<single>();
  ASSERT_EQ(std::nullopt, enum_cast<single>("233"));
}

TEST(TEST_SUITE_NAME, SingleRep)
{
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("laugh"));
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("hahaha"));
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("over"));
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("and_over"));
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("again"));
  ASSERT_EQ(single_rep::hahaha, enum_cast<single_rep>("unstoppable"));

  test_invalid_cases_common<single_rep>();
  ASSERT_EQ(std::nullopt, enum_cast<single_rep>("value"));
}

TEST(TEST_SUITE_NAME, Color)
{
  ASSERT_EQ(color::hot_pink, enum_cast<color>("hot_pink"));
  ASSERT_EQ(color::purple, enum_cast<color>("purple"));
  ASSERT_EQ(color::blanched_almond, enum_cast<color>("blanched_almond"));
  ASSERT_EQ(color::steel_blue, enum_cast<color>("steel_blue"));
  ASSERT_EQ(color::alice_blue, enum_cast<color>("alice_blue"));
  ASSERT_EQ(color::maroon, enum_cast<color>("maroon"));
  ASSERT_EQ(color::medium_blue, enum_cast<color>("medium_blue"));
  ASSERT_EQ(color::fire_brick, enum_cast<color>("fire_brick"));
  ASSERT_EQ(color::yellow_green, enum_cast<color>("yellow_green"));
  ASSERT_EQ(color::light_gray, enum_cast<color>("light_gray"));
  ASSERT_EQ(color::salmon, enum_cast<color>("salmon"));
  ASSERT_EQ(color::medium_slate_blue, enum_cast<color>("medium_slate_blue"));
  ASSERT_EQ(color::dark_turquoise, enum_cast<color>("dark_turquoise"));
  ASSERT_EQ(color::medium_sea_green, enum_cast<color>("medium_sea_green"));
  ASSERT_EQ(color::dark_sea_green, enum_cast<color>("dark_sea_green"));
  ASSERT_EQ(color::pale_green, enum_cast<color>("pale_green"));
  ASSERT_EQ(color::dark_orange, enum_cast<color>("dark_orange"));
  ASSERT_EQ(color::light_blue, enum_cast<color>("light_blue"));
  ASSERT_EQ(color::olive, enum_cast<color>("olive"));
  ASSERT_EQ(color::orange_red, enum_cast<color>("orange_red"));
  ASSERT_EQ(color::mint_cream, enum_cast<color>("mint_cream"));
  ASSERT_EQ(color::tomato, enum_cast<color>("tomato"));
  ASSERT_EQ(color::lavender_blush, enum_cast<color>("lavender_blush"));
  ASSERT_EQ(color::pale_violet_red, enum_cast<color>("pale_violet_red"));
  ASSERT_EQ(color::dark_olive_green, enum_cast<color>("dark_olive_green"));
  ASSERT_EQ(color::moccasin, enum_cast<color>("moccasin"));
  ASSERT_EQ(color::crimson, enum_cast<color>("crimson"));
  ASSERT_EQ(color::deep_sky_blue, enum_cast<color>("deep_sky_blue"));
  ASSERT_EQ(color::pink, enum_cast<color>("pink"));
  ASSERT_EQ(color::burly_wood, enum_cast<color>("burly_wood"));
  ASSERT_EQ(color::sienna, enum_cast<color>("sienna"));
  ASSERT_EQ(color::dark_golden_rod, enum_cast<color>("dark_golden_rod"));
  ASSERT_EQ(color::old_lace, enum_cast<color>("old_lace"));
  ASSERT_EQ(color::pale_golden_rod, enum_cast<color>("pale_golden_rod"));
  ASSERT_EQ(color::gainsboro, enum_cast<color>("gainsboro"));
  ASSERT_EQ(color::honey_dew, enum_cast<color>("honey_dew"));
  ASSERT_EQ(color::dark_green, enum_cast<color>("dark_green"));
  ASSERT_EQ(color::light_coral, enum_cast<color>("light_coral"));
  ASSERT_EQ(color::peach_puff, enum_cast<color>("peach_puff"));
  ASSERT_EQ(color::deep_pink, enum_cast<color>("deep_pink"));
  ASSERT_EQ(color::light_sea_green, enum_cast<color>("light_sea_green"));
  ASSERT_EQ(color::gray, enum_cast<color>("gray"));
  ASSERT_EQ(color::sky_blue, enum_cast<color>("sky_blue"));
  ASSERT_EQ(color::lime, enum_cast<color>("lime"));
  ASSERT_EQ(color::cornsilk, enum_cast<color>("cornsilk"));
  ASSERT_EQ(color::light_golden_rod_yellow,
    enum_cast<color>("light_golden_rod_yellow"));
  ASSERT_EQ(color::blue_violet, enum_cast<color>("blue_violet"));
  ASSERT_EQ(color::light_slate_gray, enum_cast<color>("light_slate_gray"));
  ASSERT_EQ(color::saddle_brown, enum_cast<color>("saddle_brown"));
  ASSERT_EQ(color::navy, enum_cast<color>("navy"));
  ASSERT_EQ(color::aquamarine, enum_cast<color>("aquamarine"));
  ASSERT_EQ(color::olive_drab, enum_cast<color>("olive_drab"));
  ASSERT_EQ(color::sandy_brown, enum_cast<color>("sandy_brown"));
  ASSERT_EQ(color::powder_blue, enum_cast<color>("powder_blue"));
  ASSERT_EQ(color::dark_gray, enum_cast<color>("dark_gray"));
  ASSERT_EQ(color::medium_purple, enum_cast<color>("medium_purple"));
  ASSERT_EQ(color::violet, enum_cast<color>("violet"));
  ASSERT_EQ(color::midnight_blue, enum_cast<color>("midnight_blue"));
  ASSERT_EQ(color::chartreuse, enum_cast<color>("chartreuse"));
  ASSERT_EQ(color::spring_green, enum_cast<color>("spring_green"));
  ASSERT_EQ(color::forest_green, enum_cast<color>("forest_green"));
  ASSERT_EQ(color::cadet_blue, enum_cast<color>("cadet_blue"));
  ASSERT_EQ(color::dim_gray, enum_cast<color>("dim_gray"));
  ASSERT_EQ(color::royal_blue, enum_cast<color>("royal_blue"));
  ASSERT_EQ(color::light_yellow, enum_cast<color>("light_yellow"));
  ASSERT_EQ(color::lemon_chiffon, enum_cast<color>("lemon_chiffon"));
  ASSERT_EQ(color::chocolate, enum_cast<color>("chocolate"));
  ASSERT_EQ(color::thistle, enum_cast<color>("thistle"));
  ASSERT_EQ(color::peru, enum_cast<color>("peru"));
  ASSERT_EQ(color::cornflower_blue, enum_cast<color>("cornflower_blue"));
  ASSERT_EQ(color::gold, enum_cast<color>("gold"));
  ASSERT_EQ(color::cyan, enum_cast<color>("cyan"));
  ASSERT_EQ(color::medium_turquoise, enum_cast<color>("medium_turquoise"));
  ASSERT_EQ(color::turquoise, enum_cast<color>("turquoise"));
  ASSERT_EQ(color::ghost_white, enum_cast<color>("ghost_white"));
  ASSERT_EQ(color::slate_gray, enum_cast<color>("slate_gray"));
  ASSERT_EQ(color::indian_red, enum_cast<color>("indian_red"));
  ASSERT_EQ(color::wheat, enum_cast<color>("wheat"));
  ASSERT_EQ(color::white, enum_cast<color>("white"));
  ASSERT_EQ(color::light_pink, enum_cast<color>("light_pink"));
  ASSERT_EQ(color::medium_spring_green,
    enum_cast<color>("medium_spring_green"));
  ASSERT_EQ(color::medium_aquamarine, enum_cast<color>("medium_aquamarine"));
  ASSERT_EQ(color::magenta, enum_cast<color>("magenta"));
  ASSERT_EQ(color::khaki, enum_cast<color>("khaki"));
  ASSERT_EQ(color::medium_violet_red, enum_cast<color>("medium_violet_red"));
  ASSERT_EQ(color::papaya_whip, enum_cast<color>("papaya_whip"));
  ASSERT_EQ(color::navajo_white, enum_cast<color>("navajo_white"));
  ASSERT_EQ(color::light_green, enum_cast<color>("light_green"));
  ASSERT_EQ(color::green, enum_cast<color>("green"));
  ASSERT_EQ(color::teal, enum_cast<color>("teal"));
  ASSERT_EQ(color::fuchsia, enum_cast<color>("fuchsia"));
  ASSERT_EQ(color::dodger_blue, enum_cast<color>("dodger_blue"));
  ASSERT_EQ(color::medium_orchid, enum_cast<color>("medium_orchid"));
  ASSERT_EQ(color::rebecca_purple, enum_cast<color>("rebecca_purple"));
  ASSERT_EQ(color::tan, enum_cast<color>("tan"));
  ASSERT_EQ(color::red, enum_cast<color>("red"));
  ASSERT_EQ(color::indigo, enum_cast<color>("indigo"));
  ASSERT_EQ(color::ivory, enum_cast<color>("ivory"));
  ASSERT_EQ(color::dark_khaki, enum_cast<color>("dark_khaki"));
  ASSERT_EQ(color::green_yellow, enum_cast<color>("green_yellow"));
  ASSERT_EQ(color::brown, enum_cast<color>("brown"));
  ASSERT_EQ(color::misty_rose, enum_cast<color>("misty_rose"));
  ASSERT_EQ(color::pale_turquoise, enum_cast<color>("pale_turquoise"));
  ASSERT_EQ(color::bisque, enum_cast<color>("bisque"));
  ASSERT_EQ(color::snow, enum_cast<color>("snow"));
  ASSERT_EQ(color::rosy_brown, enum_cast<color>("rosy_brown"));
  ASSERT_EQ(color::dark_magenta, enum_cast<color>("dark_magenta"));
  ASSERT_EQ(color::antique_white, enum_cast<color>("antique_white"));
  ASSERT_EQ(color::light_salmon, enum_cast<color>("light_salmon"));
  ASSERT_EQ(color::orchid, enum_cast<color>("orchid"));
  ASSERT_EQ(color::silver, enum_cast<color>("silver"));
  ASSERT_EQ(color::light_steel_blue, enum_cast<color>("light_steel_blue"));
  ASSERT_EQ(color::linen, enum_cast<color>("linen"));
  ASSERT_EQ(color::floral_white, enum_cast<color>("floral_white"));
  ASSERT_EQ(color::white_smoke, enum_cast<color>("white_smoke"));
  ASSERT_EQ(color::dark_salmon, enum_cast<color>("dark_salmon"));
  ASSERT_EQ(color::orange, enum_cast<color>("orange"));
  ASSERT_EQ(color::black, enum_cast<color>("black"));
  ASSERT_EQ(color::sea_green, enum_cast<color>("sea_green"));
  ASSERT_EQ(color::light_sky_blue, enum_cast<color>("light_sky_blue"));
  ASSERT_EQ(color::dark_violet, enum_cast<color>("dark_violet"));
  ASSERT_EQ(color::dark_slate_gray, enum_cast<color>("dark_slate_gray"));
  ASSERT_EQ(color::aqua, enum_cast<color>("aqua"));
  ASSERT_EQ(color::dark_blue, enum_cast<color>("dark_blue"));
  ASSERT_EQ(color::blue, enum_cast<color>("blue"));
  ASSERT_EQ(color::slate_blue, enum_cast<color>("slate_blue"));
  ASSERT_EQ(color::dark_red, enum_cast<color>("dark_red"));
  ASSERT_EQ(color::lavender, enum_cast<color>("lavender"));
  ASSERT_EQ(color::plum, enum_cast<color>("plum"));
  ASSERT_EQ(color::sea_shell, enum_cast<color>("sea_shell"));
  ASSERT_EQ(color::yellow, enum_cast<color>("yellow"));
  ASSERT_EQ(color::coral, enum_cast<color>("coral"));
  ASSERT_EQ(color::azure, enum_cast<color>("azure"));
  ASSERT_EQ(color::dark_orchid, enum_cast<color>("dark_orchid"));
  ASSERT_EQ(color::lawn_green, enum_cast<color>("lawn_green"));
  ASSERT_EQ(color::light_cyan, enum_cast<color>("light_cyan"));
  ASSERT_EQ(color::lime_green, enum_cast<color>("lime_green"));
  ASSERT_EQ(color::dark_slate_blue, enum_cast<color>("dark_slate_blue"));
  ASSERT_EQ(color::beige, enum_cast<color>("beige"));
  ASSERT_EQ(color::dark_cyan, enum_cast<color>("dark_cyan"));
  ASSERT_EQ(color::golden_rod, enum_cast<color>("golden_rod"));

  ASSERT_EQ(color::fuchsia, enum_cast<color>("magenta"));
  test_invalid_cases_common<color>();
}

TEST(TEST_SUITE_NAME, TerminalColor)
{
  ASSERT_EQ(terminal_color::blue, enum_cast<terminal_color>("blue"));
  ASSERT_EQ(terminal_color::bright_yellow,
    enum_cast<terminal_color>("bright_yellow"));
  ASSERT_EQ(terminal_color::green, enum_cast<terminal_color>("green"));
  ASSERT_EQ(terminal_color::cyan, enum_cast<terminal_color>("cyan"));
  ASSERT_EQ(terminal_color::bright_green,
    enum_cast<terminal_color>("bright_green"));
  ASSERT_EQ(terminal_color::black, enum_cast<terminal_color>("black"));
  ASSERT_EQ(terminal_color::bright_white,
    enum_cast<terminal_color>("bright_white"));
  ASSERT_EQ(terminal_color::yellow, enum_cast<terminal_color>("yellow"));
  ASSERT_EQ(terminal_color::bright_blue,
    enum_cast<terminal_color>("bright_blue"));
  ASSERT_EQ(terminal_color::bright_red,
    enum_cast<terminal_color>("bright_red"));
  ASSERT_EQ(terminal_color::red, enum_cast<terminal_color>("red"));
  ASSERT_EQ(terminal_color::bright_magenta,
    enum_cast<terminal_color>("bright_magenta"));
  ASSERT_EQ(terminal_color::bright_black,
    enum_cast<terminal_color>("bright_black"));
  ASSERT_EQ(terminal_color::white, enum_cast<terminal_color>("white"));
  ASSERT_EQ(terminal_color::magenta, enum_cast<terminal_color>("magenta"));

  test_invalid_cases_common<terminal_color>();
  ASSERT_EQ(std::nullopt, enum_cast<terminal_color>("fuchsia"));
}

TEST(TEST_SUITE_NAME, HashCollision)
{
  ASSERT_EQ(hash_collision::_wSYZDRpiQJf8Rfv,
      enum_cast<hash_collision>("_wSYZDRpiQJf8Rfv"));
  ASSERT_EQ(hash_collision::_cuFFJIHGp_jNJKS,
    enum_cast<hash_collision>("_cuFFJIHGp_jNJKS"));

  test_invalid_cases_common<hash_collision>();
}
