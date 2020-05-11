/** @file list3005.cpp */
/** Listing 30-5. Adding the Ability to Initialize a rational Object */
#include <cassert>
import <iostream>;
import <numeric>;

/// Represents a rational number.
struct rational
{
  /// Constructs a rational object, given a numerator and a denominator.
  /// Always reduces to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  /// Assigns a numerator and a denominator, then reduces to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
  void assign(int num, int den)
  {
    numerator = num;
    denominator = den;
    reduce();
  }

  /// Reduces the numerator and denominator by their GCD.
  void reduce()
  {
    assert(denominator != 0);
    int div{std::gcd(numerator, denominator)};
    numerator = numerator / div;
    denominator = denominator / div;
  }

  int numerator;     ///< numerator gets the sign of the rational value
  int denominator;   ///< denominator is always positive
};

int main()
{
  rational pi{1420, 452};
  std::cout << "pi is about " << pi.numerator << "/" << pi.denominator << '\n';
}
