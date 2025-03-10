#include "my_set.h"
#include <print>

using std::println;


int main(int argc, char** argv)
{
    My_set<int> set{25, 20, 36, 10, 22, 24, 23, 30, 40, 5,
                    12, 28, 38, 48, 1,  8,  15, 45, 50};

    set.erase(set.find(25));
    set.print_post();


    return 0;
}
