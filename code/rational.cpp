#include <cassert>
#include <cstdlib>
#include <istream>
#include <ostream>
#include <sstream>

#include "rational.hpp"

/// Compute the greatest common divisor of two integers, using Euclid’s algorithm.
int gcd(int n, int m)
{
  n = std::abs(n);
  while (m != 0) {
    int tmp(n % m);
    n = m;
    m = tmp;
  }
  return n;
}

rational::rational(int num, int den)
: numerator_(den < 0 ? -num : num),
  denominator_(init_denominator(den))
{
  normalize();
}

int rational::init_denominator(int den)
{
  if (den == 0)
    throw zero_denominator("zero denominator");
  else
    return std::abs(den);
}

float rational::as_float()
const
{
  return static_cast<float>(numerator()) / denominator();
}

double rational::as_double()
const
{
  return static_cast<double>(numerator()) / denominator();
}

long double rational::as_long_double()
const
{
  return static_cast<long double>(numerator()) /
        denominator();
}

rational& rational::operator=(int num)
{
  numerator_ = num;
  denominator_ = 1;
  return *this;
}

/// Reduce the numerator and denominator by their GCD.
void rational::reduce()
{
  int div(gcd(numerator(), denominator()));
  assert(div != 0);
  numerator_ /= div;
  denominator_ /= div;
}

/// Absolute value of a rational number.
rational abs(rational const& r)
{
  return rational(abs(r.numerator()), r.denominator());
}

/// Unary negation of a rational number.
rational operator-(rational const& r)
{
  return rational(-r.numerator(), r.denominator());
}

rational& rational::operator+=(rational const& rhs)
{
  numerator_ = numerator() * rhs.denominator() + rhs.numerator() * denominator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

rational& rational::operator-=(rational const& rhs)
{
  numerator_ = numerator() * rhs.denominator() - rhs.numerator() * denominator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

/// Multiplication of rational numbers.
rational& rational::operator*=(rational const& rhs)
{
  numerator_ *= rhs.numerator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

/// Division of rational numbers.
rational& rational::operator/=(rational const& rhs)
{
  if (rhs.numerator() == 0)
    throw zero_denominator("divide by zero");
  numerator_ *= rhs.denominator();
  denominator_ *= rhs.numerator();
  normalize();
  return *this;
}

void rational::normalize()
{
  if (denominator_ < 0)
  {
    denominator_ = -denominator_;
    numerator_ = -numerator_;
  }
  reduce();
}

rational operator+(rational const& lhs, rational const& rhs)
{
  rational result(lhs);
  result += rhs;
  return result;
}

/// Subtraction of rational numbers.
rational operator-(rational const& lhs, rational const& rhs)
{
  rational result(lhs);
  result -= rhs;
  return result;
}

/// Multiplication of rational numbers.
rational operator*(rational const& lhs, rational const& rhs)
{
  rational result(lhs);
  result *= rhs;
  return result;
}

/// Division of rational numbers.
rational operator/(rational const& lhs, rational const& rhs)
{
  rational result(lhs);
  result /= rhs;
  return result;
}

/// Compare two rational numbers for equality.
bool operator==(rational const& a, rational const& b)
{
  return a.numerator() == b.numerator() and a.denominator() == b.denominator();
}

/// Compare two rational numbers for less-than.
bool operator<(rational const& a, rational const& b)
{
  return a.numerator() * b.denominator() < b.numerator() * a.denominator();
}

/// Read a rational number.
/// Format is @em integer @c / @em integer.
std::istream& operator>>(std::istream& in, rational& rat)
{
  int n(0), d(0);
  char sep('\0');
  if (not (in >> n >> sep))
    // Error reading the numerator or the separator character.
    in.setstate(in.failbit);
  else if (sep != '/')
  {
    // Push sep back into the input stream, so the next input operation
    // will read it.
    in.unget();
    rat = n;
  }
  else if (in >> d)
    // Successfully read numerator, separator, and denominator.
    rat = rational(n, d);
  else
    // Error reading denominator.
    in.setstate(in.failbit);

  return in;
}

/// Write a rational numbers.
/// Format is @em numerator @c / @en denominator.
std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp;
  tmp << rat.numerator() << '/' << rat.denominator();
  out << tmp.str();

  return out;
}
