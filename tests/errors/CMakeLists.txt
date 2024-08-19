add_executable(argparse_errors_test)
target_sources(argparse_errors_test PRIVATE
    basic.cpp
)
target_link_libraries(argparse_errors_test PRIVATE gtest gtest_main)
gtest_discover_tests(argparse_errors_test)
