#ifndef TEST_HPP_
#define TEST_HPP_

#include <iostream>
#include <ostream>

#define test__stringify(x) #x
#define test__failed(expr,file,line) do { std::cerr << file << ", line " << line << ": test failed: " << expr << '\n'; } while(false)
#define test(x) if (not(x)) test__failed(test__stringify(x), __FILE__, __LINE__)

#endif
