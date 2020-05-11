/** @file list4210.hpp */
/** Listing 42-10. Module c Exports circumference() */
// export module c;
#include "a.hpp"
/*export*/ double circumference(double radius)
{
    return 2.0 * pi * radius;
}
