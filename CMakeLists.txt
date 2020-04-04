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
set(prefix "9781430261933_Ch" CACHE STRING "Base filename except for the chapter number, such as the ISBN followed by '_Ch'.")

# Many of the listings and snippets have tests that are independent
# of the book. The tests are my way to helping to ensure that the
# code is more or less correct.
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

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
# All tests have the same form. A single input file and a single
# file of expected output. The test runs the program, redirecting
# input and piping output to diff. In order to test on Windows,
# the command processor must be set to a compatible shell,
# and GNU-compatible diff must be available. Recommendation is to
# use a Linux Subsystem.
function(run_test name)
  add_test(NAME test_${name} 
    COMMAND /bin/sh -c "./${name} < ${name}.input | diff --color=always -u ${name}.expected -"
  )
  set_tests_properties(test_${name}
    PROPERTIES
      REQUIRED_FILES "${name}.input;${name}.expected"
    )
endfunction(run_test)

# Generate code listings from an Open Document Format (ODF) text file.
# The codex program is a C++ program available on gitlab and github.
# It uses a configuration file to guide how it locates and extracts
# code listings from a document. This function performs the extraction
# in a subdirectory, one per document, that is, one per chapter.
#
# Extracted files have the extension ".cc". The cmake dependency
# is for files ending with ".cpp". The ".cpp" files are manually
# created only for extracted ".cc" files that can be successfully
# compiled. Some extracted files are not actually code, for example.
# Some are riddled with errors for pedagogical purposes.
# A few are headers only. Manually-created symbolic links assign
# names to some headers so they can be used in other code listings.
#
# Most code listings are complete programs. Most of the one that
# are not, are still meant to stand alone, and so a separate main()
# is provided. The ".cpp" file uses #include "listNNNN.cc" and
# possibly other #include directives as needed to compile successfully.
#
# Once listing files are successfully extracted, a marker file with
# the extension ".codex" is created. If the document changes, cmake
# knows to rerun codex to generate new listings. The codex tool
# is smart enough not to touch a file if it has not changed,
# so only listings that have changed will result in modified
# source files for cmake to recompile.
function(add_odt_target number docvar)
	set(filename "${CMAKE_CURRENT_SOURCE_DIR}/${prefix}${number}.odt")
	set(codexname "${CMAKE_CURRENT_SOURCE_DIR}/${prefix}${number}.codex")
	set(target "chap${number}_codex")
	set(dirname  "${CMAKE_CURRENT_SOURCE_DIR}/code/chap${number}")
	set(confname "${CMAKE_CURRENT_SOURCE_DIR}/codex.xml")
    if (EXISTS ${filename})
        list(APPEND ${docvar} ${target})
        list(LENGTH ${docvar} doclen)
        set(${docvar} ${${docvar}} PARENT_SCOPE)
        
        add_custom_command(OUTPUT ${codexname}
            COMMAND mkdir -p "${dirname}"
            COMMAND cd ${dirname} && codex -f ${confname} ${filename}
            COMMAND touch ${codexname}
            MAIN_DEPENDENCY ${filename}
            DEPENDENCIES ${filename} ${confname}
            BYPRODUCTS "${CMAKE_CURRENT_SOURCE_DIR}/code/" "${dirname}/"
            COMMENT "Running codex to extract listings from ${filename} into ${dirname}"
        )

        add_custom_target(${target} ALL DEPENDS ${codexname})
    endif()
endfunction()

# Every chapter's CMakeLists.txt file contains only a series of
# example() functions calls, one per ".cpp" file.
function(example name)
    string(SUBSTRING ${name} 4 2 chapter_number)
    set(codexname chap${chapter_number}_codex)

    add_executable(${name} ${name}.cpp)
    add_dependencies(${name}  ${codexname})
    if(EXISTS ${name}.input AND EXISTS ${name}.expected)
        run_test(${name})
    endif()
endfunction()

# All the individual chapter files are subdirectories of code/,
# which helps manage the plethora of files.
add_subdirectory(code)

# Create a target for every chapter. I could not find any glob-type
# functions in the Cmake documentation, but we know the format of
# all the file names, so just run through all the possible numbers
# and look for a matching ".odt" file.
set(docs)
foreach(number RANGE 1 89)
    if(${number} LESS 10)
        string(CONCAT chapter_number "0" ${number})
	else()
        string(CONCAT chapter_number ${number})
    endif()
    add_odt_target(${chapter_number} docs)
endforeach()

# For convenience, "make codex" runs the codex program on all documents.
add_custom_target(codex ALL DEPENDS ${docs})
