#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/annotations.hpp>

namespace rfl = reflect_cpp26;
namespace rfl_annots = reflect_cpp26::annotations;

struct foo_t {
  REFLECT_CPP26_ANNOTATE(rename = "foo_name")
  std::string name;

  REFLECT_CPP26_ANNOTATIONS(
    rename = "foo_args",
    description = "argv[1:] i.e. list of runtime arguments "
                  "excluding argv[0] the program name",
    author = "John Doe (johndoe@example.com)",
    version = "1.2.3")
  std::vector<std::string> args;
};

struct bar_t {
  REFLECT_CPP26_ANNOTATIONS(
    arg_notation = 'a',
    alias = "bar_alpha")
  double alpha;

  REFLECT_CPP26_ANNOTATIONS(
    arg_notation = 'b',
    aliases = std::to_array({"bar_beta", "bar_β"}))
  double beta;
};

TEST(AnnotationProperties, All)
{
  EXPECT_EQ("foo_name", rfl_annots::rename_of<&foo_t::name>());

  EXPECT_EQ("foo_args", rfl_annots::rename_of<&foo_t::args>());
  EXPECT_EQ("argv[1:] i.e. list of runtime arguments "
            "excluding argv[0] the program name",
            rfl_annots::description_of<&foo_t::args>());
  EXPECT_EQ("John Doe (johndoe@example.com)",
            rfl_annots::author_of<&foo_t::args>());
  EXPECT_EQ("1.2.3", rfl_annots::version_of<&foo_t::args>());

  EXPECT_EQ('a', rfl_annots::arg_notation_of<&bar_t::alpha>());
  EXPECT_EQ('b', rfl_annots::arg_notation_of<&bar_t::beta>());
  EXPECT_THAT(rfl_annots::aliases_of<&bar_t::alpha>(),
    testing::ElementsAre("bar_alpha"));
  EXPECT_THAT(rfl_annots::aliases_of<&bar_t::beta>(),
    testing::ElementsAre("bar_beta", "bar_β"));
}
