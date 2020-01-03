#ifndef FIXED_HPP_
#define FIXED_HPP_

#include <istream>
#include <ostream>
#include <string>

/// Implement a fixed-point number class.
/// Values have two places after the decimal point.
/// All arithmetic follows the usual rules.
class fixed
{
public:
  typedef int value_type;                    /// Type of the actual value

  static int const places = 4;               ///< number of decimal places
  static value_type const places10 = 10000;  ///< 10<sup>places_</sup>

  /// Default constructor initializes to zero.
  fixed() : value_(0) {}

  /// Construct from separate integer and fractional parts,
  /// e.g., initialize to 123.45 with fixed(123, 45). Initialize
  /// to 12.07 with fixed(12, 7).
  fixed(value_type integer, value_type fraction = 0);

  /// Construct by rounding off a floating point number.
  fixed(double value)
  : value_(static_cast<value_type>(value * places10 + (value < 0 ? -0.5 : 0.5)))
  {}

  /// Convert to a string.
  /// @returns a string representation of the value, e.g., "123.04"
  std::string as_string() const;
  /// Read from a stream.
  /// Overwrite this value with the value read from the stream.
  /// @param strm the stream to read
  /// @returns true for success or false for failure
  bool read(std::istream& strm);
  /// Convert to long double.
  double as_long_double() const { return static_cast<long double>(value()) / places10; }
  /// Convert to double.
  double as_double() const { return static_cast<double>(value()) / places10; }
  /// Convert to float
  float as_float() const { return static_cast<float>(value()) / places10; }
  /// Return just the integer part, rounded off to the nearest integer.
  /// If the value lies equidistant between two integers, round even
  /// numbers up and odd numbers down (banker's rounding).
  value_type round() const;

  /// Return the integer part (which is the same as trunc()).
  value_type integer() const { return value() / places10; }
  /// Return the fractional part, e.g., 3 for 12.03
  value_type fraction() const;

  /// Addition assignment operator
  fixed& operator+=(fixed f);
  /// Subtraction assignment operator
  fixed& operator-=(fixed f);
  /// Multiplication assignment operator
  fixed& operator*=(fixed f);
  /// Division assignment operator
  fixed& operator/=(fixed f);

  /// Negate this value.
  void negate();

  /// Pre-increment
  fixed& operator++();
  /// Post-increment
  fixed operator++(int);
  /// Pre-decrement
  fixed& operator--();
  /// Post-decrement
  fixed operator--(int);

  /// Return the internal value.
  value_type value()    const { return value_; }
private:
  /// Reduce frac to the range [0, places10) by discarding digits to the right.
  value_type reduce(value_type frac);
  value_type value_;
};

/// Read a fixed value
std::istream& operator>>(std::istream& strm, fixed& f);
/// Write a fixed value
std::ostream& operator<<(std::ostream& strm, fixed f);

/// Add fixed values
fixed operator+(fixed a, fixed b);
/// Subtract fixed values
fixed operator-(fixed a, fixed b);
/// Multiply fixed values
fixed operator*(fixed a, fixed b);
/// Divide fixed values
fixed operator/(fixed a, fixed b);
/// Negate a fixed value
fixed operator-(fixed a);

/// Compare fixed values for equality by comparing the underlying values.
bool operator==(fixed a, fixed b);
/// Compare fixed values for inequality by comparing the underlying values.
bool operator!=(fixed a, fixed b);
/// Compare fixed values for less-than by comparing the underlying values.
bool operator<(fixed a, fixed b);
/// Compare fixed values for greater-than by comparing the underlying values.
bool operator>(fixed a, fixed b);
/// Compare fixed values for less-than-or-equal by comparing the underlying values.
bool operator<=(fixed a, fixed b);
/// Compare fixed values for greater-than-or-equal by comparing the underlying values.
bool operator>=(fixed a, fixed b);

#endif
