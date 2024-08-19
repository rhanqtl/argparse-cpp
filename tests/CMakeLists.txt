enable_testing()

include(GoogleTest)

add_executable(argparse_test)
target_sources(argparse_test PRIVATE
    basic.cpp
)
target_link_libraries(argparse_test PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(argparse_test)

add_subdirectory(errors)
