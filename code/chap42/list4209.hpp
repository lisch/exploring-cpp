/** @file list4209.hpp */
/** Listing 42-9. Module b Exports area() */
// export module b;
#include "a.hpp"
/*export*/ double area(double radius)
{
    return pi * radius * radius;
}
