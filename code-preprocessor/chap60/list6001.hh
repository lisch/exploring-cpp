/** @file list6001.hh */
/** Listing 60-1. Module That Declares a Trivial Debugging Function */
// export module debug;
#include <string>

/** @brief Write a debug message to the file @c "debug.txt"
 * @param msg The message to write
 */
/*export*/ void debug(std::string const& msg);
