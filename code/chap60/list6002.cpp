#include <iostream>
#include <unistd.h>
#include "list6002.hh"
int main() {
  unlink("debug.txt");
  debug("This is a test");

  std::ifstream f("debug.txt");
  std::string line;
  while(std::getline(f, line)) {
    std::cout << line << '\n';
  }
}
