#include <gtest/gtest.h>

#include <iostream>

#include "argparse/argparse.h"
#include "gtest/gtest.h"

using namespace argparse;

TEST(argparse_errors, Basic) {
  ArgumentParser parser;
  parser.add_argument<std::string>({.long_name = "abc"});

  char *argv[] = {"prog", "xyz"};
  int argc = 2;

  testing::internal::CaptureStderr();
  parser.parse(argc, argv);

  std::string err = testing::internal::GetCapturedStderr();

  EXPECT_TRUE(err.find("unrecognized"));
  EXPECT_TRUE(err.find("xyz"));
}
