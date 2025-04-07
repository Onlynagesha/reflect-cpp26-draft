#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <reflect_cpp26/annotations.hpp>

#define RFL_ANNOTATE(...) REFLECT_CPP26_ANNOTATE(__VA_ARGS__)

namespace rfl = reflect_cpp26;
namespace annots = reflect_cpp26::annotations;

struct foo_t {
  RFL_ANNOTATE(rename, "foo_name")
  std::string name;

  RFL_ANNOTATE(rename, "foo_args")
  RFL_ANNOTATE(description,
    "argv[1:] i.e. list of runtime arguments "
    "excluding argv[0] the program name")
  RFL_ANNOTATE(author, "John Doe (johndoe@example.com)")
  RFL_ANNOTATE(version, "1.2.3")
  std::vector<std::string> args;
};

struct bar_t {
  RFL_ANNOTATE(arg_notation, 'a')
  RFL_ANNOTATE(alias, "bar_alpha")
  RFL_ANNOTATE(version_since, "2.3.4")
  double alpha;

  RFL_ANNOTATE(arg_notation, 'b')
  RFL_ANNOTATE(aliases, {"bar_beta", "bar_β"})
  RFL_ANNOTATE(version_since, "2.4.6")
  double beta;
};

TEST(AnnotationProperties, All)
{
  EXPECT_EQ("foo_name", annots::rename_of<&foo_t::name>());

  EXPECT_EQ("foo_args", annots::rename_of<&foo_t::args>());
  EXPECT_EQ("argv[1:] i.e. list of runtime arguments "
            "excluding argv[0] the program name",
            annots::description_of<&foo_t::args>());
  EXPECT_EQ("John Doe (johndoe@example.com)",
            annots::author_of<&foo_t::args>());
  EXPECT_EQ("1.2.3", annots::version_of<&foo_t::args>());

  EXPECT_EQ('a', annots::arg_notation_of<&bar_t::alpha>());
  EXPECT_EQ("2.3.4", annots::version_since_of<&bar_t::alpha>());
  EXPECT_EQ('b', annots::arg_notation_of<&bar_t::beta>());
  EXPECT_EQ("2.4.6", annots::version_since_of<&bar_t::beta>());
  EXPECT_THAT(annots::aliases_of<&bar_t::alpha>(),
    testing::ElementsAre("bar_alpha"));
  EXPECT_THAT(annots::aliases_of<&bar_t::beta>(),
    testing::ElementsAre("bar_beta", "bar_β"));
}
