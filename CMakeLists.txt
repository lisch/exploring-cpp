# CMakeLists.txt for Exploring C++ 20.
# Run codex on ODT files to extract code listings and snippets.

# Need a fairly new release of Cmake to get C++ 20-ish support.
cmake_minimum_required(VERSION 3.16)

# This is the first version of Cmake, but the third
# release of the code examples, so let's call this version 3.0.
project(ExploringCpp VERSION 3.0)

# There is no reason to build the book's examples in anything
# other than Debug mode.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug")
endif()

# The base file name depends on the ISBN, which changes
# for each update to the book. So make it easy to set
# a new prefix when it is known. Also, editors have been known
# to switch file naming conventions mid-cycle.
set(prefix "9781430261933_Ch" CACHE STRING
	"Base filename except for the chapter number. Typically the ISBN followed by '_Ch'.")

# Many of the listings and snippets have tests that are independent
# of the book. The tests are my way to help ensure that the
# code is more or less correct.
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

set(DOCUMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Testing on Linux with clang++ and g++ prereleases.
# Testing on Windows with Microsoft Visual C++ pending release of support for C++ 20.

if (MSVC)
    # Maximum warnings and treat all warnings as errors.
    # Use standard C++ exception handling.
    # Turn off extensions to the standard.
    add_compile_options(/EHsc /Za /Wall /WX)
else()
    # Maximum warnings and treat all warnings as errors
    # Adhere strictly to language standards.
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Run a test.
# Tests have two forms: executable or text.
# An executable test is just a shell script that is run with
# the target name as the sole argument.
# A text test is the name of a text file that is the expected
# output that results when running ./${name}. The input
# to the program is ${name}.input if the file exists or
# /dev/null otherwise. The run_test script takes the name
# of the test file as an argument, and performs the indicated test.
#
# In order to test on Windows, the command processor must be
# set to a compatible shell, and compatible tools must be on the PATH.
# Recommendation is to use a Linux Subsystem or cygwin.
function(run_test name)
  add_test(NAME test_${name}
    COMMAND python3 ${DOCUMENT_DIR}/run_test.py ${name}.test ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  )
  set_tests_properties(test_${name}
    PROPERTIES
      REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${name}.test
    )
endfunction(run_test)

# Every chapter's CMakeLists.txt file contains mostly a series of
# example() functions calls, one per ".cpp" file.
# The first argument to example() is the name, e.g., list0401.
# Subsequent arguments can be additional dependencies or
# compile options. Options that start with "-" are for UNIX,
# and options that start with "/" are for Microsoft.
# Other options must use target_compile_options().
function(example name)
	set(file_args)
	set(option_args)
	foreach(arg ${ARGN})
		string(SUBSTRING ${arg} 0 1 first_char)
		if("${first_char}" STREQUAL "/")
			if(MSVC)
				list(APPEND option_args ${arg})
			endif()
		elseif("${first_char}" STREQUAL "-")
			if(UNIX)
				list(APPEND option_args ${arg})
			endif()
		else()
			list(APPEND file_args ${arg})
		endif()
	endforeach()
    add_executable(${name} ${name}.cpp ${file_args})
    target_compile_options(${name} PUBLIC ${option_args})

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name}.test)
        run_test(${name})
    endif()
endfunction()

# Add a subdirectory if the document exists.
function(chapter number)
	set(DOCUMENT ${prefix}${number}.odt)
	if(EXISTS "${DOCUMENT_DIR}/${DOCUMENT}")
		message(DEBUG "Found ${DOCUMENT} searching chap${number}")
		add_subdirectory(chap${number})
	endif()
endfunction()

# All the individual chapter files are subdirectories of code/,
# which helps manage the plethora of files.
add_subdirectory(code)
