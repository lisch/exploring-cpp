/** @file list4201.hpp */
/** Listing 42-1. Writing a Module */
// export module hello;
#include <iostream>
/*export*/ void world()
{
    std::cout << "hello, world\n";
}
