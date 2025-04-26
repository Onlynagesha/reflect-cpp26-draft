#include "enum/enum_test_options.hpp"
#include "enum/test_cases.hpp"

#ifdef ENABLE_FULL_HEADER_TEST
#include <reflect_cpp26/enum.hpp>
#else
#include <reflect_cpp26/enum/enum_index.hpp>
#endif

using namespace reflect_cpp26;
using enum enum_entry_order;

template <class E>
constexpr bool test_foo_signed_common()
{
  static_assert(2 == enum_index<by_value>(E::zero));
  static_assert(3 == enum_index<by_value>(E::one));
  static_assert(4 == enum_index<by_value>(E::two));
  static_assert(5 == enum_index<by_value>(E::four));
  static_assert(6 == enum_index<by_value>(E::five));
  static_assert(7 == enum_index<by_value>(E::six));
  static_assert(8 == enum_index<by_value>(E::seven));
  static_assert(1 == enum_index<by_value>(E::invalid));
  static_assert(0 == enum_index<by_value>(E::error));

  static_assert(0 == enum_index<by_name>(E::error));
  static_assert(1 == enum_index<by_name>(E::five));
  static_assert(2 == enum_index<by_name>(E::four));
  static_assert(3 == enum_index<by_name>(E::invalid));
  static_assert(4 == enum_index<by_name>(E::one));
  static_assert(5 == enum_index<by_name>(E::seven));
  static_assert(6 == enum_index<by_name>(E::six));
  static_assert(7 == enum_index<by_name>(E::two));
  static_assert(8 == enum_index<by_name>(E::zero));

  static_assert(npos == enum_index(static_cast<E>(-3)));
  static_assert(npos == enum_index<by_value>(static_cast<E>(3)));
  static_assert(npos == enum_index<by_name>(static_cast<E>(1 << 31)));
  return true; // All done
}

// foo_signed
static_assert(test_foo_signed_common<foo_signed>());
static_assert(0 == enum_index(foo_signed::zero));
static_assert(1 == enum_index(foo_signed::one));
static_assert(2 == enum_index(foo_signed::two));
static_assert(3 == enum_index(foo_signed::four));
static_assert(4 == enum_index(foo_signed::five));
static_assert(5 == enum_index(foo_signed::six));
static_assert(6 == enum_index(foo_signed::seven));
static_assert(7 == enum_index(foo_signed::invalid));
static_assert(8 == enum_index(foo_signed::error));

// foo_signed_reorder
static_assert(test_foo_signed_common<foo_signed_reorder>());
static_assert(0 == enum_index(foo_signed_reorder::two));
static_assert(1 == enum_index(foo_signed_reorder::five));
static_assert(2 == enum_index(foo_signed_reorder::zero));
static_assert(3 == enum_index(foo_signed_reorder::seven));
static_assert(4 == enum_index(foo_signed_reorder::one));
static_assert(5 == enum_index(foo_signed_reorder::six));
static_assert(6 == enum_index(foo_signed_reorder::invalid));
static_assert(7 == enum_index(foo_signed_reorder::four));
static_assert(8 == enum_index(foo_signed_reorder::error));

// foo_signed_rep
static_assert(0 == enum_index(foo_signed_rep::zero));
static_assert(1 == enum_index(foo_signed_rep::one)
           || 3 == enum_index(foo_signed_rep::one));
static_assert(2 == enum_index(foo_signed_rep::two)
           || 4 == enum_index(foo_signed_rep::two));
static_assert(5 == enum_index(foo_signed_rep::four));
static_assert(6 == enum_index(foo_signed_rep::five));
static_assert(7 == enum_index(foo_signed_rep::six));
static_assert(8 == enum_index(foo_signed_rep::seven));
static_assert(9 == enum_index(foo_signed_rep::invalid));
static_assert(10 == enum_index(foo_signed_rep::error));

static_assert(2 == enum_index<by_value>(foo_signed_rep::zero));
static_assert(3 == enum_index<by_value>(foo_signed_rep::one));
static_assert(5 == enum_index<by_value>(foo_signed_rep::two));
static_assert(7 == enum_index<by_value>(foo_signed_rep::four));
static_assert(8 == enum_index<by_value>(foo_signed_rep::five));
static_assert(9 == enum_index<by_value>(foo_signed_rep::six));
static_assert(10 == enum_index<by_value>(foo_signed_rep::seven));
static_assert(1 == enum_index<by_value>(foo_signed_rep::invalid));
static_assert(0 == enum_index<by_value>(foo_signed_rep::error));

static_assert(0 == enum_index<by_name>(foo_signed_rep::er)
           || 8 == enum_index<by_name>(foo_signed_rep::er));
static_assert(1 == enum_index<by_name>(foo_signed_rep::error));
static_assert(2 == enum_index<by_name>(foo_signed_rep::five));
static_assert(3 == enum_index<by_name>(foo_signed_rep::four));
static_assert(4 == enum_index<by_name>(foo_signed_rep::invalid));
static_assert(5 == enum_index<by_name>(foo_signed_rep::one)
           || 9 == enum_index<by_name>(foo_signed_rep::one));
static_assert(6 == enum_index<by_name>(foo_signed_rep::seven));
static_assert(7 == enum_index<by_name>(foo_signed_rep::six));
static_assert(10 == enum_index<by_name>(foo_signed_rep::zero));

static_assert(npos == enum_index(static_cast<foo_signed_rep>(-3)));
static_assert(npos == enum_index<by_value>(static_cast<foo_signed_rep>(3)));
static_assert(npos == enum_index<by_name>(static_cast<foo_signed_rep>(1234)));

// baz_unsigned
static_assert(0 == enum_index(bar_unsigned::zero));
static_assert(1 == enum_index(bar_unsigned::one));
static_assert(2 == enum_index(bar_unsigned::two));
static_assert(3 == enum_index(bar_unsigned::three));
static_assert(4 == enum_index(bar_unsigned::ten));
static_assert(5 == enum_index(bar_unsigned::eleven));
static_assert(6 == enum_index(bar_unsigned::twelve));
static_assert(7 == enum_index(bar_unsigned::thirteen));
static_assert(8 == enum_index(bar_unsigned::fourteen));
static_assert(9 == enum_index(bar_unsigned::invalid));
static_assert(10 == enum_index(bar_unsigned::error));

static_assert(0 == enum_index<by_value>(bar_unsigned::zero));
static_assert(1 == enum_index<by_value>(bar_unsigned::one));
static_assert(2 == enum_index<by_value>(bar_unsigned::two));
static_assert(3 == enum_index<by_value>(bar_unsigned::three));
static_assert(4 == enum_index<by_value>(bar_unsigned::ten));
static_assert(5 == enum_index<by_value>(bar_unsigned::eleven));
static_assert(6 == enum_index<by_value>(bar_unsigned::twelve));
static_assert(7 == enum_index<by_value>(bar_unsigned::thirteen));
static_assert(8 == enum_index<by_value>(bar_unsigned::fourteen));
static_assert(10 == enum_index<by_value>(bar_unsigned::invalid));
static_assert(9 == enum_index<by_value>(bar_unsigned::error));

static_assert(0 == enum_index<by_name>(bar_unsigned::eleven));
static_assert(1 == enum_index<by_name>(bar_unsigned::error));
static_assert(2 == enum_index<by_name>(bar_unsigned::fourteen));
static_assert(3 == enum_index<by_name>(bar_unsigned::invalid));
static_assert(4 == enum_index<by_name>(bar_unsigned::one));
static_assert(5 == enum_index<by_name>(bar_unsigned::ten));
static_assert(6 == enum_index<by_name>(bar_unsigned::thirteen));
static_assert(7 == enum_index<by_name>(bar_unsigned::three));
static_assert(8 == enum_index<by_name>(bar_unsigned::twelve));
static_assert(9 == enum_index<by_name>(bar_unsigned::two));
static_assert(10 == enum_index<by_name>(bar_unsigned::zero));

static_assert(npos == enum_index(static_cast<bar_unsigned>(-3)));
static_assert(npos == enum_index<by_value>(static_cast<bar_unsigned>(9)));
static_assert(npos == enum_index<by_name>(static_cast<bar_unsigned>(1234)));

// baz_signed
static_assert(0 == enum_index(baz_signed::ling));
static_assert(1 == enum_index(baz_signed::yi));
static_assert(2 == enum_index(baz_signed::er));
static_assert(3 == enum_index(baz_signed::san));
static_assert(4 == enum_index(baz_signed::fuyi));

static_assert(1 == enum_index<by_value>(baz_signed::ling));
static_assert(2 == enum_index<by_value>(baz_signed::yi));
static_assert(3 == enum_index<by_value>(baz_signed::er));
static_assert(4 == enum_index<by_value>(baz_signed::san));
static_assert(0 == enum_index<by_value>(baz_signed::fuyi));

static_assert(0 == enum_index<by_name>(baz_signed::er));
static_assert(1 == enum_index<by_name>(baz_signed::fuyi));
static_assert(2 == enum_index<by_name>(baz_signed::ling));
static_assert(3 == enum_index<by_name>(baz_signed::san));
static_assert(4 == enum_index<by_name>(baz_signed::yi));

static_assert(npos == enum_index(static_cast<baz_signed>(-2)));
static_assert(npos == enum_index<by_value>(static_cast<baz_signed>(4)));
static_assert(npos == enum_index<by_name>(static_cast<baz_signed>(1 << 15)));

// qux_unsigned
static_assert(0 == enum_index(qux_unsigned::ling));
static_assert(1 == enum_index(qux_unsigned::yi));
static_assert(2 == enum_index(qux_unsigned::er));
static_assert(3 == enum_index(qux_unsigned::san));
static_assert(4 == enum_index(qux_unsigned::si));
static_assert(5 == enum_index(qux_unsigned::fuyi));

static_assert(0 == enum_index<by_value>(qux_unsigned::ling));
static_assert(1 == enum_index<by_value>(qux_unsigned::yi));
static_assert(2 == enum_index<by_value>(qux_unsigned::er));
static_assert(3 == enum_index<by_value>(qux_unsigned::san));
static_assert(4 == enum_index<by_value>(qux_unsigned::si));
static_assert(5 == enum_index<by_value>(qux_unsigned::fuyi));

static_assert(0 == enum_index<by_name>(qux_unsigned::er));
static_assert(1 == enum_index<by_name>(qux_unsigned::fuyi));
static_assert(2 == enum_index<by_name>(qux_unsigned::ling));
static_assert(3 == enum_index<by_name>(qux_unsigned::san));
static_assert(4 == enum_index<by_name>(qux_unsigned::si));
static_assert(5 == enum_index<by_name>(qux_unsigned::yi));

static_assert(npos == enum_index(static_cast<qux_unsigned>(-2)));
static_assert(npos == enum_index<by_value>(static_cast<qux_unsigned>(5)));
static_assert(npos == enum_index<by_name>(static_cast<qux_unsigned>(1 << 15)));

// empty
static_assert(npos == enum_index(static_cast<empty>(0)));
static_assert(npos == enum_index(static_cast<empty>(1)));
static_assert(npos == enum_index(static_cast<empty>(-1)));
static_assert(npos == enum_index<by_value>(static_cast<empty>(0)));
static_assert(npos == enum_index<by_value>(static_cast<empty>(1)));
static_assert(npos == enum_index<by_value>(static_cast<empty>(-1)));
static_assert(npos == enum_index<by_name>(static_cast<empty>(0)));
static_assert(npos == enum_index<by_name>(static_cast<empty>(1)));
static_assert(npos == enum_index<by_name>(static_cast<empty>(-1)));

// single
static_assert(0 == enum_index(static_cast<single>(233)));
static_assert(0 == enum_index<by_value>(static_cast<single>(233)));
static_assert(0 == enum_index<by_name>(static_cast<single>(233)));

static_assert(npos == enum_index(static_cast<single>(0)));
static_assert(npos == enum_index(static_cast<single>(1)));
static_assert(npos == enum_index(static_cast<single>(-1)));
static_assert(npos == enum_index<by_value>(static_cast<single>(0)));
static_assert(npos == enum_index<by_value>(static_cast<single>(1)));
static_assert(npos == enum_index<by_value>(static_cast<single>(-1)));
static_assert(npos == enum_index<by_name>(static_cast<single>(0)));
static_assert(npos == enum_index<by_name>(static_cast<single>(1)));
static_assert(npos == enum_index<by_name>(static_cast<single>(-1)));

// single_rep
static_assert(0 <= enum_index(static_cast<single_rep>(233))
           && 5 >= enum_index(static_cast<single_rep>(233)));
static_assert(0 <= enum_index<by_value>(static_cast<single_rep>(233))
           && 5 >= enum_index<by_value>(static_cast<single_rep>(233)));
static_assert(0 <= enum_index<by_name>(static_cast<single_rep>(233))
           && 5 >= enum_index<by_name>(static_cast<single_rep>(233)));

static_assert(npos == enum_index(static_cast<single_rep>(0)));
static_assert(npos == enum_index(static_cast<single_rep>(1)));
static_assert(npos == enum_index(static_cast<single_rep>(-1)));
static_assert(npos == enum_index<by_value>(static_cast<single_rep>(0)));
static_assert(npos == enum_index<by_value>(static_cast<single_rep>(1)));
static_assert(npos == enum_index<by_value>(static_cast<single_rep>(-1)));
static_assert(npos == enum_index<by_name>(static_cast<single_rep>(0)));
static_assert(npos == enum_index<by_name>(static_cast<single_rep>(1)));
static_assert(npos == enum_index<by_name>(static_cast<single_rep>(-1)));

// color
static_assert(100 == enum_index(color::orange_red));
static_assert(npos == enum_index<by_value>(static_cast<color>(0x12345678)));
static_assert(npos == enum_index<by_name>(static_cast<color>(0x98765432)));

// terminal_color
static_assert(0 == enum_index(terminal_color::black));
static_assert(1 == enum_index(terminal_color::red));
static_assert(2 == enum_index(terminal_color::green));
static_assert(3 == enum_index(terminal_color::yellow));
static_assert(4 == enum_index(terminal_color::blue));
static_assert(5 == enum_index(terminal_color::magenta));
static_assert(6 == enum_index(terminal_color::cyan));
static_assert(7 == enum_index(terminal_color::white));
static_assert(8 == enum_index(terminal_color::bright_black));
static_assert(9 == enum_index(terminal_color::bright_red));
static_assert(10 == enum_index(terminal_color::bright_green));
static_assert(11 == enum_index(terminal_color::bright_yellow));
static_assert(12 == enum_index(terminal_color::bright_blue));
static_assert(13 == enum_index(terminal_color::bright_magenta));
static_assert(14 == enum_index(terminal_color::bright_cyan));
static_assert(15 == enum_index(terminal_color::bright_white));

static_assert(0 == enum_index<by_value>(terminal_color::black));
static_assert(1 == enum_index<by_value>(terminal_color::red));
static_assert(2 == enum_index<by_value>(terminal_color::green));
static_assert(3 == enum_index<by_value>(terminal_color::yellow));
static_assert(4 == enum_index<by_value>(terminal_color::blue));
static_assert(5 == enum_index<by_value>(terminal_color::magenta));
static_assert(6 == enum_index<by_value>(terminal_color::cyan));
static_assert(7 == enum_index<by_value>(terminal_color::white));
static_assert(8 == enum_index<by_value>(terminal_color::bright_black));
static_assert(9 == enum_index<by_value>(terminal_color::bright_red));
static_assert(10 == enum_index<by_value>(terminal_color::bright_green));
static_assert(11 == enum_index<by_value>(terminal_color::bright_yellow));
static_assert(12 == enum_index<by_value>(terminal_color::bright_blue));
static_assert(13 == enum_index<by_value>(terminal_color::bright_magenta));
static_assert(14 == enum_index<by_value>(terminal_color::bright_cyan));
static_assert(15 == enum_index<by_value>(terminal_color::bright_white));

static_assert(0 == enum_index<by_name>(terminal_color::black));
static_assert(1 == enum_index<by_name>(terminal_color::blue));
static_assert(2 == enum_index<by_name>(terminal_color::bright_black));
static_assert(3 == enum_index<by_name>(terminal_color::bright_blue));
static_assert(4 == enum_index<by_name>(terminal_color::bright_cyan));
static_assert(5 == enum_index<by_name>(terminal_color::bright_green));
static_assert(6 == enum_index<by_name>(terminal_color::bright_magenta));
static_assert(7 == enum_index<by_name>(terminal_color::bright_red));
static_assert(8 == enum_index<by_name>(terminal_color::bright_white));
static_assert(9 == enum_index<by_name>(terminal_color::bright_yellow));
static_assert(10 == enum_index<by_name>(terminal_color::cyan));
static_assert(11 == enum_index<by_name>(terminal_color::green));
static_assert(12 == enum_index<by_name>(terminal_color::magenta));
static_assert(13 == enum_index<by_name>(terminal_color::red));
static_assert(14 == enum_index<by_name>(terminal_color::white));
static_assert(15 == enum_index<by_name>(terminal_color::yellow));

static_assert(npos == enum_index(static_cast<terminal_color>(0)));
static_assert(npos == enum_index<by_value>(static_cast<terminal_color>(1)));
static_assert(npos == enum_index<by_name>(static_cast<terminal_color>(40)));

// hash_collision
static_assert(0 == enum_index(hash_collision::_wSYZDRpiQJf8Rfv));
static_assert(1 == enum_index(hash_collision::_cuFFJIHGp_jNJKS));

static_assert(0 == enum_index<by_value>(hash_collision::_wSYZDRpiQJf8Rfv));
static_assert(1 == enum_index<by_value>(hash_collision::_cuFFJIHGp_jNJKS));

static_assert(0 == enum_index<by_name>(hash_collision::_cuFFJIHGp_jNJKS));
static_assert(1 == enum_index<by_name>(hash_collision::_wSYZDRpiQJf8Rfv));

static_assert(npos == enum_index(static_cast<hash_collision>(-1)));
static_assert(npos == enum_index<by_value>(static_cast<hash_collision>(2)));
static_assert(npos == enum_index<by_name>(static_cast<hash_collision>(1234)));

TEST(EnumIndex, StaticAll) {
  EXPECT_TRUE(true); // All test cases done by static assertions above.
}
