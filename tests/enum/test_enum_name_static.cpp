#include <gtest/gtest.h>
#include <reflect_cpp26/enum.hpp>
#include "./test_cases.hpp"

using namespace reflect_cpp26;

template <class E>
constexpr bool test_foo_signed_common()
{
  static_assert("six" == enum_name(E::six));
  static_assert("zero" == enum_name(E::zero));
  static_assert("seven" == enum_name(E::seven));
  static_assert("four" == enum_name(E::four));
  static_assert("error" == enum_name(E::error));
  static_assert("five" == enum_name(E::five));
  static_assert("invalid" == enum_name(E::invalid));

  static_assert("" == enum_name(static_cast<E>(-3)));
  static_assert("<n/a>" == enum_name(static_cast<E>(3), "<n/a>"));
  static_assert("" == enum_name(static_cast<E>(8)));
  static_assert("<n/a>" == enum_name(static_cast<E>(1 << 31), "<n/a>"));
  return true; // All done
}

static_assert(test_foo_signed_common<foo_signed>());
static_assert("one" == enum_name(foo_signed::one));
static_assert("two" == enum_name(foo_signed::two));

static_assert(test_foo_signed_common<foo_signed_reorder>());
static_assert("one" == enum_name(foo_signed_reorder::one));
static_assert("two" == enum_name(foo_signed_reorder::two));

static_assert(test_foo_signed_common<foo_signed_rep>());
static_assert("one" == enum_name(foo_signed_rep::one)
           || "yi" == enum_name(foo_signed_rep::one));
static_assert("two" == enum_name(foo_signed_rep::two)
           || "er" == enum_name(foo_signed_rep::two));

static_assert("three" == enum_name(bar_unsigned::three));
static_assert("twelve" == enum_name(bar_unsigned::twelve));
static_assert("fourteen" == enum_name(bar_unsigned::fourteen));
static_assert("ten" == enum_name(bar_unsigned::ten));
static_assert("thirteen" == enum_name(bar_unsigned::thirteen));
static_assert("eleven" == enum_name(bar_unsigned::eleven));
static_assert("one" == enum_name(bar_unsigned::one));
static_assert("two" == enum_name(bar_unsigned::two));
static_assert("invalid" == enum_name(bar_unsigned::invalid));
static_assert("zero" == enum_name(bar_unsigned::zero));
static_assert("error" == enum_name(bar_unsigned::error));

static_assert("" == enum_name(static_cast<bar_unsigned>(-4)));
static_assert("<n/a>" == enum_name(static_cast<bar_unsigned>(4), "<n/a>"));
static_assert("" == enum_name(static_cast<bar_unsigned>(9)));
static_assert("<n/a>"
  == enum_name(static_cast<bar_unsigned>(1 << 31), "<n/a>"));

static_assert("fuyi" == enum_name(static_cast<baz_signed>(-1)));
static_assert("ling" == enum_name(baz_signed::ling));
static_assert("fuyi" == enum_name(baz_signed::fuyi));
static_assert("san" == enum_name(baz_signed::san));
static_assert("er" == enum_name(baz_signed::er));
static_assert("yi" == enum_name(baz_signed::yi));

static_assert("" == enum_name(static_cast<baz_signed>(-2)));
static_assert("<n/a>" == enum_name(static_cast<baz_signed>(4), "<n/a>"));

static_assert("san" == enum_name(qux_unsigned::san));
static_assert("fuyi" == enum_name(qux_unsigned::fuyi));
static_assert("fuyi" == enum_name(static_cast<qux_unsigned>(-1)));
static_assert("ling" == enum_name(qux_unsigned::ling));
static_assert("er" == enum_name(qux_unsigned::er));
static_assert("yi" == enum_name(qux_unsigned::yi));
static_assert("si" == enum_name(qux_unsigned::si));

static_assert("" == enum_name(static_cast<qux_unsigned>(-2)));
static_assert("<n/a>" == enum_name(static_cast<qux_unsigned>(5), "<n/a>"));

static_assert("" == enum_name(static_cast<empty>(-1)));
static_assert("<no>" == enum_name(static_cast<empty>(0), "<no>"));
static_assert("<entries>" == enum_name(static_cast<empty>(1), "<entries>"));
static_assert("<inside>" == enum_name(static_cast<empty>(1 << 31), "<inside>"));

static_assert("value" == enum_name(single::value));
static_assert("value" == enum_name(static_cast<single>(233)));

static_assert("<no>" == enum_name(static_cast<single>(0), "<no>"));
static_assert("<other>" == enum_name(static_cast<single>(-1), "<other>"));
static_assert("<entries>" == enum_name(static_cast<single>(1), "<entries>"));
static_assert("<inside>"
  == enum_name(static_cast<single>(1 << 31), "<inside>"));

static_assert("laugh" == enum_name(single_rep::laugh)
           || "hahaha" == enum_name(single_rep::laugh)
           || "over" == enum_name(single_rep::laugh)
           || "and_over" == enum_name(single_rep::laugh)
           || "again" == enum_name(single_rep::laugh)
           || "unstoppable" == enum_name(single_rep::laugh));

static_assert("<no>" ==
  enum_name(static_cast<single_rep>(0), "<no>"));
static_assert("<other>" ==
  enum_name(static_cast<single_rep>(-1), "<other>"));
static_assert("<entries>" ==
  enum_name(static_cast<single_rep>(1), "<entries>"));
static_assert("<inside>" ==
  enum_name(static_cast<single_rep>(1 << 31), "<inside>"));

static_assert("" == enum_name(static_cast<terminal_color>(0)));
static_assert("" == enum_name(static_cast<terminal_color>(18)));
static_assert("" == enum_name(static_cast<terminal_color>(72)));
static_assert("" == enum_name(static_cast<terminal_color>(108)));
static_assert("" == enum_name(static_cast<terminal_color>(216)));

static_assert("blue" == enum_name(terminal_color::blue));
static_assert("bright_white" == enum_name(terminal_color::bright_white));
static_assert("bright_blue" == enum_name(terminal_color::bright_blue));
static_assert("bright_yellow" == enum_name(terminal_color::bright_yellow));
static_assert("magenta" == enum_name(terminal_color::magenta));
static_assert("yellow" == enum_name(terminal_color::yellow));
static_assert("bright_green" == enum_name(terminal_color::bright_green));
static_assert("cyan" == enum_name(terminal_color::cyan));
static_assert("bright_black" == enum_name(terminal_color::bright_black));
static_assert("white" == enum_name(terminal_color::white));
static_assert("bright_cyan" == enum_name(terminal_color::bright_cyan));
static_assert("black" == enum_name(terminal_color::black));
static_assert("bright_magenta" == enum_name(terminal_color::bright_magenta));
static_assert("bright_red" == enum_name(terminal_color::bright_red));
static_assert("red" == enum_name(terminal_color::red));
static_assert("green" == enum_name(terminal_color::green));

static_assert("" == enum_name(static_cast<color>(0x000001)));
static_assert("" == enum_name(static_cast<color>(0x000100)));
static_assert("" == enum_name(static_cast<color>(0x010000)));
static_assert("" == enum_name(static_cast<color>(0xFFFFFE)));
static_assert("" == enum_name(static_cast<color>(0xFFFFFFFF)));

static_assert("light_pink" == enum_name(color::light_pink));
static_assert("peru" == enum_name(color::peru));
static_assert("bisque" == enum_name(color::bisque));
static_assert("violet" == enum_name(color::violet));
static_assert("lime_green" == enum_name(color::lime_green));
static_assert("olive" == enum_name(color::olive));
static_assert("pale_turquoise" == enum_name(color::pale_turquoise));
static_assert("azure" == enum_name(color::azure));
static_assert("antique_white" == enum_name(color::antique_white));
static_assert("burly_wood" == enum_name(color::burly_wood));
static_assert("red" == enum_name(color::red));
static_assert("tan" == enum_name(color::tan));
static_assert("hot_pink" == enum_name(color::hot_pink));
static_assert("alice_blue" == enum_name(color::alice_blue));
static_assert("medium_turquoise" == enum_name(color::medium_turquoise));
static_assert("light_steel_blue" == enum_name(color::light_steel_blue));
static_assert("light_green" == enum_name(color::light_green));
static_assert("light_yellow" == enum_name(color::light_yellow));
static_assert("yellow_green" == enum_name(color::yellow_green));
static_assert("light_sea_green" == enum_name(color::light_sea_green));
static_assert("white_smoke" == enum_name(color::white_smoke));
static_assert("blanched_almond" == enum_name(color::blanched_almond));
static_assert("peach_puff" == enum_name(color::peach_puff));
static_assert("navy" == enum_name(color::navy));
static_assert("black" == enum_name(color::black));
static_assert("lavender" == enum_name(color::lavender));
static_assert("sienna" == enum_name(color::sienna));
static_assert("sky_blue" == enum_name(color::sky_blue));
static_assert("magenta" == enum_name(color::fuchsia)
           || "fuchsia" == enum_name(color::fuchsia));
static_assert("medium_orchid" == enum_name(color::medium_orchid));
static_assert("mint_cream" == enum_name(color::mint_cream));
static_assert("light_sky_blue" == enum_name(color::light_sky_blue));
static_assert("lemon_chiffon" == enum_name(color::lemon_chiffon));
static_assert("moccasin" == enum_name(color::moccasin));
static_assert("dark_sea_green" == enum_name(color::dark_sea_green));
static_assert("honey_dew" == enum_name(color::honey_dew));
static_assert("chocolate" == enum_name(color::chocolate));
static_assert("misty_rose" == enum_name(color::misty_rose));
static_assert("papaya_whip" == enum_name(color::papaya_whip));
static_assert("plum" == enum_name(color::plum));
static_assert("dark_blue" == enum_name(color::dark_blue));
static_assert("slate_gray" == enum_name(color::slate_gray));
static_assert("dark_salmon" == enum_name(color::dark_salmon));
static_assert("aquamarine" == enum_name(color::aquamarine));
static_assert("medium_purple" == enum_name(color::medium_purple));
static_assert("orange" == enum_name(color::orange));
static_assert("powder_blue" == enum_name(color::powder_blue));
static_assert("dark_turquoise" == enum_name(color::dark_turquoise));
static_assert("light_slate_gray" == enum_name(color::light_slate_gray));
static_assert("dark_khaki" == enum_name(color::dark_khaki));
static_assert("beige" == enum_name(color::beige));
static_assert("medium_blue" == enum_name(color::medium_blue));
static_assert("light_gray" == enum_name(color::light_gray));
static_assert("dark_slate_blue" == enum_name(color::dark_slate_blue));
static_assert("teal" == enum_name(color::teal));
static_assert("ivory" == enum_name(color::ivory));
static_assert("aqua" == enum_name(color::aqua)
           || "cyan" == enum_name(color::aqua));
static_assert("dark_cyan" == enum_name(color::dark_cyan));
static_assert("medium_sea_green" == enum_name(color::medium_sea_green));
static_assert("dark_gray" == enum_name(color::dark_gray));
static_assert("linen" == enum_name(color::linen));
static_assert("orange_red" == enum_name(color::orange_red));
static_assert("light_blue" == enum_name(color::light_blue));
static_assert("midnight_blue" == enum_name(color::midnight_blue));
static_assert("sea_green" == enum_name(color::sea_green));
static_assert("maroon" == enum_name(color::maroon));
static_assert("golden_rod" == enum_name(color::golden_rod));
static_assert("purple" == enum_name(color::purple));
static_assert("spring_green" == enum_name(color::spring_green));
static_assert("dark_violet" == enum_name(color::dark_violet));
static_assert("dim_gray" == enum_name(color::dim_gray));
static_assert("turquoise" == enum_name(color::turquoise));
static_assert("lime" == enum_name(color::lime));
static_assert("blue" == enum_name(color::blue));
static_assert("green_yellow" == enum_name(color::green_yellow));
static_assert("olive_drab" == enum_name(color::olive_drab));
static_assert("light_cyan" == enum_name(color::light_cyan));
static_assert("silver" == enum_name(color::silver));
static_assert("medium_violet_red" == enum_name(color::medium_violet_red));
static_assert("lawn_green" == enum_name(color::lawn_green));
static_assert("deep_sky_blue" == enum_name(color::deep_sky_blue));
static_assert("dodger_blue" == enum_name(color::dodger_blue));
static_assert("yellow" == enum_name(color::yellow));
static_assert("aqua" == enum_name(color::cyan)
           || "cyan" == enum_name(color::cyan));
static_assert("steel_blue" == enum_name(color::steel_blue));
static_assert("light_coral" == enum_name(color::light_coral));
static_assert("medium_slate_blue" == enum_name(color::medium_slate_blue));
static_assert("forest_green" == enum_name(color::forest_green));
static_assert("fire_brick" == enum_name(color::fire_brick));
static_assert("dark_slate_gray" == enum_name(color::dark_slate_gray));
static_assert("dark_magenta" == enum_name(color::dark_magenta));
static_assert("crimson" == enum_name(color::crimson));
static_assert("dark_red" == enum_name(color::dark_red));
static_assert("khaki" == enum_name(color::khaki));
static_assert("old_lace" == enum_name(color::old_lace));
static_assert("gainsboro" == enum_name(color::gainsboro));
static_assert("royal_blue" == enum_name(color::royal_blue));
static_assert("rosy_brown" == enum_name(color::rosy_brown));
static_assert("gray" == enum_name(color::gray));
static_assert("medium_spring_green" == enum_name(color::medium_spring_green));
static_assert("indigo" == enum_name(color::indigo));
static_assert("floral_white" == enum_name(color::floral_white));
static_assert("gold" == enum_name(color::gold));
static_assert("coral" == enum_name(color::coral));
static_assert("wheat" == enum_name(color::wheat));
static_assert("dark_orange" == enum_name(color::dark_orange));
static_assert("cadet_blue" == enum_name(color::cadet_blue));
static_assert("light_salmon" == enum_name(color::light_salmon));
static_assert("slate_blue" == enum_name(color::slate_blue));
static_assert("pink" == enum_name(color::pink));
static_assert("orchid" == enum_name(color::orchid));
static_assert("dark_olive_green" == enum_name(color::dark_olive_green));
static_assert("pale_violet_red" == enum_name(color::pale_violet_red));
static_assert("deep_pink" == enum_name(color::deep_pink));
static_assert("cornflower_blue" == enum_name(color::cornflower_blue));
static_assert("indian_red" == enum_name(color::indian_red));
static_assert("cornsilk" == enum_name(color::cornsilk));
static_assert("chartreuse" == enum_name(color::chartreuse));
static_assert("medium_aquamarine" == enum_name(color::medium_aquamarine));
static_assert("thistle" == enum_name(color::thistle));
static_assert("tomato" == enum_name(color::tomato));
static_assert("salmon" == enum_name(color::salmon));
static_assert("magenta" == enum_name(color::magenta)
           || "fuchsia" == enum_name(color::magenta));
static_assert("sea_shell" == enum_name(color::sea_shell));
static_assert("sandy_brown" == enum_name(color::sandy_brown));
static_assert("pale_golden_rod" == enum_name(color::pale_golden_rod));
static_assert("saddle_brown" == enum_name(color::saddle_brown));
static_assert("rebecca_purple" == enum_name(color::rebecca_purple));
static_assert("lavender_blush" == enum_name(color::lavender_blush));
static_assert("light_golden_rod_yellow" ==
  enum_name(color::light_golden_rod_yellow));
static_assert("white" == enum_name(color::white));
static_assert("dark_golden_rod" == enum_name(color::dark_golden_rod));
static_assert("dark_green" == enum_name(color::dark_green));
static_assert("green" == enum_name(color::green));
static_assert("pale_green" == enum_name(color::pale_green));
static_assert("brown" == enum_name(color::brown));
static_assert("navajo_white" == enum_name(color::navajo_white));
static_assert("snow" == enum_name(color::snow));
static_assert("ghost_white" == enum_name(color::ghost_white));
static_assert("blue_violet" == enum_name(color::blue_violet));
static_assert("dark_orchid" == enum_name(color::dark_orchid));

static_assert("_cuFFJIHGp_jNJKS"
  == enum_name(hash_collision::_cuFFJIHGp_jNJKS));
static_assert("_wSYZDRpiQJf8Rfv"
  == enum_name(hash_collision::_wSYZDRpiQJf8Rfv));

static_assert("<n/a>" == enum_name(static_cast<hash_collision>(-1), "<n/a>"));
static_assert("" == enum_name(static_cast<hash_collision>(1 << 31), ""));

TEST(EnumName, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
