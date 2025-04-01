// No header guard since this file is intended to be included only once.

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

#ifndef TEST_SUITE_NAME
#error "Test suite name is missing."
#endif

// Note: this wrapper is required to convert result to std::string
// due to Google Test bug.
template <class E>
auto enum_name(E value, std::string_view alt = {}) -> std::string
{
  auto sv = reflect_cpp26::enum_name(value,alt);
  return std::string{sv};
}

template <class E>
void test_foo_signed_common()
{
  ASSERT_EQ("six", enum_name(E::six));
  ASSERT_EQ("zero", enum_name(E::zero));
  ASSERT_EQ("seven", enum_name(E::seven));
  ASSERT_EQ("four", enum_name(E::four));
  ASSERT_EQ("error", enum_name(E::error));
  ASSERT_EQ("five", enum_name(E::five));
  ASSERT_EQ("invalid", enum_name(E::invalid));

  ASSERT_EQ("", enum_name(static_cast<E>(-3)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<E>(3), "<n/a>"));
  ASSERT_EQ("", enum_name(static_cast<E>(8)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<E>(1 << 31), "<n/a>"));
}

TEST(TEST_SUITE_NAME, FooSigned)
{
  test_foo_signed_common<foo_signed>();
  ASSERT_EQ("one", enum_name(foo_signed::one));
  ASSERT_EQ("two", enum_name(foo_signed::two));
}

TEST(TEST_SUITE_NAME, FooSignedReorder)
{
  test_foo_signed_common<foo_signed_reorder>();
  ASSERT_EQ("one", enum_name(foo_signed_reorder::one));
  ASSERT_EQ("two", enum_name(foo_signed_reorder::two));
}

TEST(TEST_SUITE_NAME, FooSignedRep)
{
  test_foo_signed_common<foo_signed_rep>();
  ASSERT_THAT(enum_name(foo_signed_rep::one), testing::AnyOf("one", "yi"));
  ASSERT_THAT(enum_name(foo_signed_rep::two), testing::AnyOf("two", "er"));
}

TEST(TEST_SUITE_NAME, BarUnsigned)
{
  ASSERT_EQ("three", enum_name(bar_unsigned::three));
  ASSERT_EQ("twelve", enum_name(bar_unsigned::twelve));
  ASSERT_EQ("fourteen", enum_name(bar_unsigned::fourteen));
  ASSERT_EQ("ten", enum_name(bar_unsigned::ten));
  ASSERT_EQ("thirteen", enum_name(bar_unsigned::thirteen));
  ASSERT_EQ("eleven", enum_name(bar_unsigned::eleven));
  ASSERT_EQ("one", enum_name(bar_unsigned::one));
  ASSERT_EQ("two", enum_name(bar_unsigned::two));
  ASSERT_EQ("invalid", enum_name(bar_unsigned::invalid));
  ASSERT_EQ("zero", enum_name(bar_unsigned::zero));
  ASSERT_EQ("error", enum_name(bar_unsigned::error));

  ASSERT_EQ("", enum_name(static_cast<bar_unsigned>(-4)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<bar_unsigned>(4), "<n/a>"));
  ASSERT_EQ("", enum_name(static_cast<bar_unsigned>(9)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<bar_unsigned>(1 << 31), "<n/a>"));
}

TEST(TEST_SUITE_NAME, BazSigned)
{
  ASSERT_EQ("fuyi", enum_name(static_cast<baz_signed>(-1)));
  ASSERT_EQ("ling", enum_name(baz_signed::ling));
  ASSERT_EQ("fuyi", enum_name(baz_signed::fuyi));
  ASSERT_EQ("san", enum_name(baz_signed::san));
  ASSERT_EQ("er", enum_name(baz_signed::er));
  ASSERT_EQ("yi", enum_name(baz_signed::yi));

  ASSERT_EQ("", enum_name(static_cast<baz_signed>(-2)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<baz_signed>(4), "<n/a>"));
}

TEST(TEST_SUITE_NAME, QuxUnsigned)
{
  ASSERT_EQ("san", enum_name(qux_unsigned::san));
  ASSERT_EQ("fuyi", enum_name(qux_unsigned::fuyi));
  ASSERT_EQ("fuyi", enum_name(static_cast<qux_unsigned>(-1)));
  ASSERT_EQ("ling", enum_name(qux_unsigned::ling));
  ASSERT_EQ("er", enum_name(qux_unsigned::er));
  ASSERT_EQ("yi", enum_name(qux_unsigned::yi));
  ASSERT_EQ("si", enum_name(qux_unsigned::si));

  ASSERT_EQ("", enum_name(static_cast<qux_unsigned>(-2)));
  ASSERT_EQ("<n/a>", enum_name(static_cast<qux_unsigned>(5), "<n/a>"));
}

TEST(TEST_SUITE_NAME, Empty)
{
  ASSERT_EQ("", enum_name(static_cast<empty>(-1)));
  ASSERT_EQ("<no>", enum_name(static_cast<empty>(0), "<no>"));
  ASSERT_EQ("<entries>", enum_name(static_cast<empty>(1), "<entries>"));
  ASSERT_EQ("<inside>", enum_name(static_cast<empty>(1 << 31), "<inside>"));
}

TEST(TEST_SUITE_NAME, Single)
{
  ASSERT_EQ("value", enum_name(single::value));
  ASSERT_EQ("value", enum_name(static_cast<single>(233)));

  ASSERT_EQ("<no>", enum_name(static_cast<single>(0), "<no>"));
  ASSERT_EQ("<other>", enum_name(static_cast<single>(-1), "<other>"));
  ASSERT_EQ("<entries>", enum_name(static_cast<single>(1), "<entries>"));
  ASSERT_EQ("<inside>", enum_name(static_cast<single>(1 << 31), "<inside>"));
}

TEST(TEST_SUITE_NAME, SingleRep)
{
  ASSERT_THAT(enum_name(single_rep::laugh),
    testing::AnyOf("laugh", "hahaha", "over",
                   "and_over", "again", "unstoppable"));
  ASSERT_EQ("<no>",
    enum_name(static_cast<single_rep>(0), "<no>"));
  ASSERT_EQ("<other>",
    enum_name(static_cast<single_rep>(-1), "<other>"));
  ASSERT_EQ("<entries>",
    enum_name(static_cast<single_rep>(1), "<entries>"));
  ASSERT_EQ("<inside>",
    enum_name(static_cast<single_rep>(1 << 31), "<inside>"));
}

TEST(TEST_SUITE_NAME, TerminalColor)
{
  ASSERT_EQ("", enum_name(static_cast<terminal_color>(0)));
  ASSERT_EQ("", enum_name(static_cast<terminal_color>(18)));
  ASSERT_EQ("", enum_name(static_cast<terminal_color>(72)));
  ASSERT_EQ("", enum_name(static_cast<terminal_color>(108)));
  ASSERT_EQ("", enum_name(static_cast<terminal_color>(216)));

  ASSERT_EQ("blue", enum_name(terminal_color::blue));
  ASSERT_EQ("bright_white", enum_name(terminal_color::bright_white));
  ASSERT_EQ("bright_blue", enum_name(terminal_color::bright_blue));
  ASSERT_EQ("bright_yellow", enum_name(terminal_color::bright_yellow));
  ASSERT_EQ("magenta", enum_name(terminal_color::magenta));
  ASSERT_EQ("yellow", enum_name(terminal_color::yellow));
  ASSERT_EQ("bright_green", enum_name(terminal_color::bright_green));
  ASSERT_EQ("cyan", enum_name(terminal_color::cyan));
  ASSERT_EQ("bright_black", enum_name(terminal_color::bright_black));
  ASSERT_EQ("white", enum_name(terminal_color::white));
  ASSERT_EQ("bright_cyan", enum_name(terminal_color::bright_cyan));
  ASSERT_EQ("black", enum_name(terminal_color::black));
  ASSERT_EQ("bright_magenta", enum_name(terminal_color::bright_magenta));
  ASSERT_EQ("bright_red", enum_name(terminal_color::bright_red));
  ASSERT_EQ("red", enum_name(terminal_color::red));
  ASSERT_EQ("green", enum_name(terminal_color::green));
}

TEST(TEST_SUITE_NAME, Color)
{
  ASSERT_EQ("", enum_name(static_cast<color>(0x000001)));
  ASSERT_EQ("", enum_name(static_cast<color>(0x000100)));
  ASSERT_EQ("", enum_name(static_cast<color>(0x010000)));
  ASSERT_EQ("", enum_name(static_cast<color>(0xFFFFFE)));
  ASSERT_EQ("", enum_name(static_cast<color>(0xFFFFFFFF)));

  ASSERT_EQ("light_pink", enum_name(color::light_pink));
  ASSERT_EQ("peru", enum_name(color::peru));
  ASSERT_EQ("bisque", enum_name(color::bisque));
  ASSERT_EQ("violet", enum_name(color::violet));
  ASSERT_EQ("lime_green", enum_name(color::lime_green));
  ASSERT_EQ("olive", enum_name(color::olive));
  ASSERT_EQ("pale_turquoise", enum_name(color::pale_turquoise));
  ASSERT_EQ("azure", enum_name(color::azure));
  ASSERT_EQ("antique_white", enum_name(color::antique_white));
  ASSERT_EQ("burly_wood", enum_name(color::burly_wood));
  ASSERT_EQ("red", enum_name(color::red));
  ASSERT_EQ("tan", enum_name(color::tan));
  ASSERT_EQ("hot_pink", enum_name(color::hot_pink));
  ASSERT_EQ("alice_blue", enum_name(color::alice_blue));
  ASSERT_EQ("medium_turquoise", enum_name(color::medium_turquoise));
  ASSERT_EQ("light_steel_blue", enum_name(color::light_steel_blue));
  ASSERT_EQ("light_green", enum_name(color::light_green));
  ASSERT_EQ("light_yellow", enum_name(color::light_yellow));
  ASSERT_EQ("yellow_green", enum_name(color::yellow_green));
  ASSERT_EQ("light_sea_green", enum_name(color::light_sea_green));
  ASSERT_EQ("white_smoke", enum_name(color::white_smoke));
  ASSERT_EQ("blanched_almond", enum_name(color::blanched_almond));
  ASSERT_EQ("peach_puff", enum_name(color::peach_puff));
  ASSERT_EQ("navy", enum_name(color::navy));
  ASSERT_EQ("black", enum_name(color::black));
  ASSERT_EQ("lavender", enum_name(color::lavender));
  ASSERT_EQ("sienna", enum_name(color::sienna));
  ASSERT_EQ("sky_blue", enum_name(color::sky_blue));
  ASSERT_THAT(enum_name(color::fuchsia), testing::AnyOf("magenta", "fuchsia"));
  ASSERT_EQ("medium_orchid", enum_name(color::medium_orchid));
  ASSERT_EQ("mint_cream", enum_name(color::mint_cream));
  ASSERT_EQ("light_sky_blue", enum_name(color::light_sky_blue));
  ASSERT_EQ("lemon_chiffon", enum_name(color::lemon_chiffon));
  ASSERT_EQ("moccasin", enum_name(color::moccasin));
  ASSERT_EQ("dark_sea_green", enum_name(color::dark_sea_green));
  ASSERT_EQ("honey_dew", enum_name(color::honey_dew));
  ASSERT_EQ("chocolate", enum_name(color::chocolate));
  ASSERT_EQ("misty_rose", enum_name(color::misty_rose));
  ASSERT_EQ("papaya_whip", enum_name(color::papaya_whip));
  ASSERT_EQ("plum", enum_name(color::plum));
  ASSERT_EQ("dark_blue", enum_name(color::dark_blue));
  ASSERT_EQ("slate_gray", enum_name(color::slate_gray));
  ASSERT_EQ("dark_salmon", enum_name(color::dark_salmon));
  ASSERT_EQ("aquamarine", enum_name(color::aquamarine));
  ASSERT_EQ("medium_purple", enum_name(color::medium_purple));
  ASSERT_EQ("orange", enum_name(color::orange));
  ASSERT_EQ("powder_blue", enum_name(color::powder_blue));
  ASSERT_EQ("dark_turquoise", enum_name(color::dark_turquoise));
  ASSERT_EQ("light_slate_gray", enum_name(color::light_slate_gray));
  ASSERT_EQ("dark_khaki", enum_name(color::dark_khaki));
  ASSERT_EQ("beige", enum_name(color::beige));
  ASSERT_EQ("medium_blue", enum_name(color::medium_blue));
  ASSERT_EQ("light_gray", enum_name(color::light_gray));
  ASSERT_EQ("dark_slate_blue", enum_name(color::dark_slate_blue));
  ASSERT_EQ("teal", enum_name(color::teal));
  ASSERT_EQ("ivory", enum_name(color::ivory));
  ASSERT_THAT(enum_name(color::aqua), testing::AnyOf("aqua", "cyan"));
  ASSERT_EQ("dark_cyan", enum_name(color::dark_cyan));
  ASSERT_EQ("medium_sea_green", enum_name(color::medium_sea_green));
  ASSERT_EQ("dark_gray", enum_name(color::dark_gray));
  ASSERT_EQ("linen", enum_name(color::linen));
  ASSERT_EQ("orange_red", enum_name(color::orange_red));
  ASSERT_EQ("light_blue", enum_name(color::light_blue));
  ASSERT_EQ("midnight_blue", enum_name(color::midnight_blue));
  ASSERT_EQ("sea_green", enum_name(color::sea_green));
  ASSERT_EQ("maroon", enum_name(color::maroon));
  ASSERT_EQ("golden_rod", enum_name(color::golden_rod));
  ASSERT_EQ("purple", enum_name(color::purple));
  ASSERT_EQ("spring_green", enum_name(color::spring_green));
  ASSERT_EQ("dark_violet", enum_name(color::dark_violet));
  ASSERT_EQ("dim_gray", enum_name(color::dim_gray));
  ASSERT_EQ("turquoise", enum_name(color::turquoise));
  ASSERT_EQ("lime", enum_name(color::lime));
  ASSERT_EQ("blue", enum_name(color::blue));
  ASSERT_EQ("green_yellow", enum_name(color::green_yellow));
  ASSERT_EQ("olive_drab", enum_name(color::olive_drab));
  ASSERT_EQ("light_cyan", enum_name(color::light_cyan));
  ASSERT_EQ("silver", enum_name(color::silver));
  ASSERT_EQ("medium_violet_red", enum_name(color::medium_violet_red));
  ASSERT_EQ("lawn_green", enum_name(color::lawn_green));
  ASSERT_EQ("deep_sky_blue", enum_name(color::deep_sky_blue));
  ASSERT_EQ("dodger_blue", enum_name(color::dodger_blue));
  ASSERT_EQ("yellow", enum_name(color::yellow));
  ASSERT_THAT(enum_name(color::cyan), testing::AnyOf("aqua", "cyan"));
  ASSERT_EQ("steel_blue", enum_name(color::steel_blue));
  ASSERT_EQ("light_coral", enum_name(color::light_coral));
  ASSERT_EQ("medium_slate_blue", enum_name(color::medium_slate_blue));
  ASSERT_EQ("forest_green", enum_name(color::forest_green));
  ASSERT_EQ("fire_brick", enum_name(color::fire_brick));
  ASSERT_EQ("dark_slate_gray", enum_name(color::dark_slate_gray));
  ASSERT_EQ("dark_magenta", enum_name(color::dark_magenta));
  ASSERT_EQ("crimson", enum_name(color::crimson));
  ASSERT_EQ("dark_red", enum_name(color::dark_red));
  ASSERT_EQ("khaki", enum_name(color::khaki));
  ASSERT_EQ("old_lace", enum_name(color::old_lace));
  ASSERT_EQ("gainsboro", enum_name(color::gainsboro));
  ASSERT_EQ("royal_blue", enum_name(color::royal_blue));
  ASSERT_EQ("rosy_brown", enum_name(color::rosy_brown));
  ASSERT_EQ("gray", enum_name(color::gray));
  ASSERT_EQ("medium_spring_green", enum_name(color::medium_spring_green));
  ASSERT_EQ("indigo", enum_name(color::indigo));
  ASSERT_EQ("floral_white", enum_name(color::floral_white));
  ASSERT_EQ("gold", enum_name(color::gold));
  ASSERT_EQ("coral", enum_name(color::coral));
  ASSERT_EQ("wheat", enum_name(color::wheat));
  ASSERT_EQ("dark_orange", enum_name(color::dark_orange));
  ASSERT_EQ("cadet_blue", enum_name(color::cadet_blue));
  ASSERT_EQ("light_salmon", enum_name(color::light_salmon));
  ASSERT_EQ("slate_blue", enum_name(color::slate_blue));
  ASSERT_EQ("pink", enum_name(color::pink));
  ASSERT_EQ("orchid", enum_name(color::orchid));
  ASSERT_EQ("dark_olive_green", enum_name(color::dark_olive_green));
  ASSERT_EQ("pale_violet_red", enum_name(color::pale_violet_red));
  ASSERT_EQ("deep_pink", enum_name(color::deep_pink));
  ASSERT_EQ("cornflower_blue", enum_name(color::cornflower_blue));
  ASSERT_EQ("indian_red", enum_name(color::indian_red));
  ASSERT_EQ("cornsilk", enum_name(color::cornsilk));
  ASSERT_EQ("chartreuse", enum_name(color::chartreuse));
  ASSERT_EQ("medium_aquamarine", enum_name(color::medium_aquamarine));
  ASSERT_EQ("thistle", enum_name(color::thistle));
  ASSERT_EQ("tomato", enum_name(color::tomato));
  ASSERT_EQ("salmon", enum_name(color::salmon));
  ASSERT_THAT(enum_name(color::magenta), testing::AnyOf("magenta", "fuchsia"));
  ASSERT_EQ("sea_shell", enum_name(color::sea_shell));
  ASSERT_EQ("sandy_brown", enum_name(color::sandy_brown));
  ASSERT_EQ("pale_golden_rod", enum_name(color::pale_golden_rod));
  ASSERT_EQ("saddle_brown", enum_name(color::saddle_brown));
  ASSERT_EQ("rebecca_purple", enum_name(color::rebecca_purple));
  ASSERT_EQ("lavender_blush", enum_name(color::lavender_blush));
  ASSERT_EQ("light_golden_rod_yellow",
    enum_name(color::light_golden_rod_yellow));
  ASSERT_EQ("white", enum_name(color::white));
  ASSERT_EQ("dark_golden_rod", enum_name(color::dark_golden_rod));
  ASSERT_EQ("dark_green", enum_name(color::dark_green));
  ASSERT_EQ("green", enum_name(color::green));
  ASSERT_EQ("pale_green", enum_name(color::pale_green));
  ASSERT_EQ("brown", enum_name(color::brown));
  ASSERT_EQ("navajo_white", enum_name(color::navajo_white));
  ASSERT_EQ("snow", enum_name(color::snow));
  ASSERT_EQ("ghost_white", enum_name(color::ghost_white));
  ASSERT_EQ("blue_violet", enum_name(color::blue_violet));
  ASSERT_EQ("dark_orchid", enum_name(color::dark_orchid));
}

TEST(TEST_SUITE_NAME, HashCollision)
{
  ASSERT_EQ("_cuFFJIHGp_jNJKS", enum_name(hash_collision::_cuFFJIHGp_jNJKS));
  ASSERT_EQ("_wSYZDRpiQJf8Rfv", enum_name(hash_collision::_wSYZDRpiQJf8Rfv));

  ASSERT_EQ("<n/a>", enum_name(static_cast<hash_collision>(-1), "<n/a>"));
  ASSERT_EQ("", enum_name(static_cast<hash_collision>(1 << 31), ""));
}
