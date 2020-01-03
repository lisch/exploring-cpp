/* Source file wrapper to compile incomplete
 * code listings. The codex program extracts
 * each code listing to a text file. Ordinarily,
 * a code listing is a complete program. Sometimes,
 * however, some external functions and whatnot
 * are omitted for brevity. These external
 * entities are declared in other files.The
 * makefile generates a command line to compile
 * this source file, defining macros to include
 * the listing and related files, directing
 * the output to a file named after the listing
 * file. The macros expand to file names that
 * are #included as follows:
 *
 * - CODEPREFIX  declarations before the program
 * - CODEPOSTFIX declarations after the program
 * - PROGRAM     the program itself
 */

#define eval_(x) #x
#define stringify(x)  eval_(x)

#ifdef CODEPREFIX
#include stringify(CODEPREFIX)
#endif

#include stringify(PROGRAM)

#ifdef CODEPOSTFIX
#include stringify(CODEPOSTFIX)
#endif
