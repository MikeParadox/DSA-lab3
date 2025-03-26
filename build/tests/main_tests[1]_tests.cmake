add_test([=[BaseTest.TreeSizeTest]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=BaseTest.TreeSizeTest]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[BaseTest.TreeSizeTest]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  main_tests_TESTS BaseTest.TreeSizeTest)
