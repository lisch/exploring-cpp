/** @file list4209.hpp */
/** Listing 42-9. Module b Exports area() */
export module b;
export import a;
export double area(double radius)
{
    return pi * radius * radius;
}
