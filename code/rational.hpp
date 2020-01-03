#ifndef VER
# define VER 1
#endif

#define STR_(x) #x
#define STR(x) STR_(x)
#define CAT(x,y) x ## y
#define HDR(ver) CAT(rational,ver)

#include STR(HDR(VER).hpp)

#undef HDR
#undef VER
#undef STR_
#undef STR
#undef CAT
