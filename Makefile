compile:
	ninja -C ./build/debug/
run:
	./build/debug/main
test:
	./build/debug/main_tests
multitest:
	./build/debug/main_tests --gtest_repeat=100
filtered_tests:
	./build/debug/main_tests --gtest_filter=swap*
valgrind:
	valgrind --leak-check=yes --track-origins=yes -s ./build/debug/main
build_again:
	rm -rf build/*
	cmake -G "Ninja" -D CMAKE_BUILD_TYPE=Debug -B ./build/debug -S .
# to_build_release:
#	rm -rf build/*
# 	cmake -G "Ninja" -D CMAKE_BUILD_TYPE=Release -S . -B ./build/release/ 
# 	cmake --build ./build/release/	
# to_run_release:
# 	./build/release/main