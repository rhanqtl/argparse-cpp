add_executable(argparse_test)
target_sources(argparse_test PRIVATE
    basic.cpp
)
target_link_libraries(argparse_test PRIVATE gtest gtest_main)
