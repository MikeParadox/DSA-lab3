if(EXISTS "/home/mikhail/programming/cs/dsa/labs/lab3/build/debug/tests/main_tests[1]_tests.cmake")
  include("/home/mikhail/programming/cs/dsa/labs/lab3/build/debug/tests/main_tests[1]_tests.cmake")
else()
  add_test(main_tests_NOT_BUILT main_tests_NOT_BUILT)
endif()
