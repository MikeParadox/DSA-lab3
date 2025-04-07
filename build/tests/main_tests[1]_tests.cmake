add_test([=[BaseTest.TreeSizeTest]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=BaseTest.TreeSizeTest]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[BaseTest.TreeSizeTest]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[BaseTest.TreesEqualityTest]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=BaseTest.TreesEqualityTest]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[BaseTest.TreesEqualityTest]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[BaseTest.TreeAssignmentTests]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=BaseTest.TreeAssignmentTests]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[BaseTest.TreeAssignmentTests]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetSize]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetSize]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetSize]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetCreation]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetCreation]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetCreation]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetIterators]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetIterators]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetIterators]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetInsertEraseTests]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetInsertEraseTests]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetInsertEraseTests]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetSwapAndCompTests]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetSwapAndCompTests]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetSwapAndCompTests]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[SetTests.SetAlgTests]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=SetTests.SetAlgTests]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[SetTests.SetAlgTests]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[MemoryTests.StringTests]=]  /home/mikhail/programming/cs/dsa/labs/lab3/build/main_tests [==[--gtest_filter=MemoryTests.StringTests]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MemoryTests.StringTests]=]  PROPERTIES WORKING_DIRECTORY /home/mikhail/programming/cs/dsa/labs/lab3/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  main_tests_TESTS BaseTest.TreeSizeTest BaseTest.TreesEqualityTest BaseTest.TreeAssignmentTests SetTests.SetSize SetTests.SetCreation SetTests.SetIterators SetTests.SetInsertEraseTests SetTests.SetSwapAndCompTests SetTests.SetAlgTests MemoryTests.StringTests)
