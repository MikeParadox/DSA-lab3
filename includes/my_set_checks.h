#ifndef MY_SET_CHECK_H
#define MY_SET_CHECK_H
#pragma once


#include "my_set.h"
#include <print>


template<class T, class Compare = std::less<T>>
    requires std::equality_comparable<T>
bool check_all_links();







#endif
