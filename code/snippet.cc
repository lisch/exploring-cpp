/* Source file wrapper to compile short
 * code snippets. The codex program extracts
 * each snippet to a text file. The makefile
 * generates a command line to compile this
 * source file, defining macros to include
 * the snippet and related files, directing
 * the output to a file named after the snippet
 * file. The macros expand to file names that
 * are #included as follows:
 *
 * - CODEDECLS   declarations at global scope
 * - CODEPREFIX  statements in main before the snippet
 * - CODEPOSTFIX statements in main after the snippet
 * - SNIPPET     the snippet itself
 */

#include "all.hpp"
#if defined(_MSC_VER)
#pragma hdrstop
#endif

#define eval_(x) #x
#define stringify(x)  eval_(x)

#ifdef CODEDECL
#include stringify(CODEDECL)
#endif

// Keep this header separate, so the CODEDECL header
// can define or undefine NDEBUG
#include <cassert>

int main()
{

#ifdef CODEPREFIX
#include stringify(CODEPREFIX)
#endif

#ifdef SNIPPET
#include stringify(SNIPPET)
#endif
;

#ifdef CODEPOSTFIX
#include stringify(CODEPOSTFIX)
#endif
;
}
