include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_GetProperties(googletest)
find_package(GTest REQUIRED)

add_executable(cpp_test "")
target_sources(cpp_test
	PRIVATE
		test.cpp
)
target_link_libraries(cpp_test
	PRIVATE
		heap
        binarySearchTree
		GTest::gtest_main
)
enable_testing()
include(GoogleTest)
add_test(
	NAME test_heap
	COMMAND cpp_test
)

