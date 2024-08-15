#include <gtest/gtest.h>

#include <string>

#include "argparse/argparse.h"

using namespace argparse;

TEST(argparse, Basic) {
  ArgumentParser parser;
  parser.add_argument<std::string>({.long_name = "output", .short_name = "o"});
  parser.add_argument<int>({.long_name = "threads", .short_name = "c"});

  char *argv[] = {"prog", "--output", "/path/to/file", "-c", "10"};
  int argc = 5;
  parser.parse(argc, argv);

  EXPECT_EQ(parser.get<std::string>("output"), "/path/to/file");
  EXPECT_EQ(parser.get<std::string>("o"), "/path/to/file");
  EXPECT_EQ(parser.get<int>("threads"), 10);
  EXPECT_EQ(parser.get<int>("c"), 10);
}
