#include <gtest/gtest.h>

#include <string>

#include "argparse/argparse.h"
#include "bits/ArgumentParser.h"

using namespace argparse;

TEST(argparse, Options_Basic) {
  ArgumentParser parser;
  parser.add_option<std::string>({.long_name = "output", .short_name = "o"});
  parser.add_option<int>({.long_name = "threads", .short_name = "c"});

  char *argv[] = {"prog", "--output", "/path/to/file", "-c", "10"};
  const int argc = 5;
  parser.parse(argc, argv);

  EXPECT_EQ(parser.get<std::string>("output"), "/path/to/file");
  EXPECT_EQ(parser.get<std::string>("o"), "/path/to/file");
  EXPECT_EQ(parser.get<int>("threads"), 10);
  EXPECT_EQ(parser.get<int>("c"), 10);
}

TEST(argparse, Arguments_Basic) {
  ArgumentParser parser;
  parser.add_argument<int>({.name = "x", .required = true});
  parser.add_argument<int>({.name = "y", .required = true});
  parser.add_argument<int>({.name = "z"});

  {
    const int argc = 3;
    char *argv[] = {"prog", "1", "2"};
    parser.parse(argc, argv);

    EXPECT_EQ(parser.get<int>("x"), 1);
    EXPECT_EQ(parser.get<int>("y"), 2);
  }

  {
    const int argc = 4;
    char *argv[] = {"prog", "1", "2", "3"};
    parser.parse(argc, argv);

    EXPECT_EQ(parser.get<int>("x"), 1);
    EXPECT_EQ(parser.get<int>("y"), 2);
    EXPECT_EQ(parser.get<int>("z"), 3);
  }
}
