#include "my_set.h"
#include <algorithm>
#include <print>
#include <vector>

using std::println;


int main(int argc, char** argv)
{
    My_set<int> set{25, 20, 50, 10, 22, 24, 23, 30, 40, 5,
                    12, 28, 38, 48, 1,  8,  15, 45, 36};
    // My_set<int> set2{25, 20, 36, 10, 22, 24, 23, 30, 40, 5,
    //                  12, 28, 38, 48, 1,  8,  15, 45, 50};

    // My_set<int> set{1, 2, 3};
    // My_set<int> set2{1, 2};

    // println("set == set2: {}", set.equal(set2));
    // set.print_reverse_infix();
    // println();
    // set.print_post();
    // set
    // set.print_layers();
    // println();
    // set.print_infix();

    // sieve_array(1'000);
    // println();
    // sieve_set(1'000);
    // println();

    // My_set<int> set2{set};
    // set2.print_post();

    auto it = std::find(set.begin(), set.end(), 10);
    println("{}", *it);


    return 0;
}
