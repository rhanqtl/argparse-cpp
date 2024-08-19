FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
  SOURCE_DIR     ${CMAKE_PROJECT_SOURCE_DIR}/lib/googletest
)
FetchContent_MakeAvailable(googletest)
