#define __builtin_powi(__x, __i)  std::pow((__x), static_cast<double>(__i))
#define __builtin_powif(__x, __n) std::pow(static_cast<double>(__x), static_cast<double>(__n))
#define __builtin_powil(__x, __n) std::pow(static_cast<double>(__x), static_cast<double>(__n))
