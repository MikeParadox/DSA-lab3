#include "my_set.h"
#include "my_set_checks.h"
#include <print>

using std::println;


int main(int argc, char** argv)
{
    My_set<int> set{6}; //, 4, 8, 3, 5, 7, 9, 10, 2};
    set.print_post();
    set.erase(set.begin());
    set.print_post();

    // set.print_infix();

    return 0;
}
