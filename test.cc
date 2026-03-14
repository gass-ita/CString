#include "cstring.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <utility>
#include <random>

int main()
{
    String s1 = "Hello, World!";
    s1 *= 100;
    std::cout << s1.GetLength() << std::endl;
    return 0;
}