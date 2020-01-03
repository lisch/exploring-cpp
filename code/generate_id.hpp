#ifndef VER
# define VER 3
#endif

#define STR_(x) #x
#define STR(x) STR_(x)
#define CAT(x,y) x ## y
#define HDR(ver) CAT(generate_id,ver)

#include STR(HDR(VER).hpp)

#undef HDR
#undef VER
