find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
  main_tests
  bstree_test.cpp
)
target_link_libraries(
  main_tests
  # DSA
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(main_tests)

