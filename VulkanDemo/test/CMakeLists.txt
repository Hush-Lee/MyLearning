include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_GetProperties(googletest)
# test/CMakeLists.txt

add_custom_target(run_tests
    COMMAND echo "No tests yet."
)

