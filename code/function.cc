/* Source file wrapper to compiler code fragments.
 * The codex program extracts code listings to
 * files. Files that contain functions or other
 * fragments can be compiled through Makefile
 * magic with this source file, defining the
 * FUNCTION macro to be the name of the code
 * listing file. Other macros expand to file
 * names that are #included as follows:
 *
 * - CODEPREFIX  declarations at the start of the file
 * - CODEPOSTFIX declarations at the end of the file
 * - FUNCTION    declarations at global scope
 * - MAINBODY    the body of the main() function
 */

#include "all.hpp"
#if defined(_MSC_VER)
#pragma hdrstop
#endif

#define eval_(x) #x
#define stringify(x)  eval_(x)

#ifdef CODEPREFIX
#include stringify(CODEPREFIX)
#endif

// Keep this header separate, so the CODEPREFIX header
// can define or undefine NDEBUG
#include <cassert>

#ifdef FUNCTION
#include stringify(FUNCTION)
#endif

#ifdef CODEPOSTFIX
#include stringify(CODEPOSTFIX)
#endif

int main()
{
#ifdef CODEMAIN
#include stringify(CODEMAIN)
#endif
}
