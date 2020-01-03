# Makefile for building code examples in Exploring C++, by Ray Lischner.
# Each code listing is extracted to a file name listCCNN.EEE, where
# CC is the chapter number, NN is the listing number, and EEE is
# an extension that indicates the kind of listing. Unnumbered code
# snippets are extracted to files of the form snipCCNN.EEE, where
# NN is a serial number, starting at one for the first snippet in
# the chapter, and increasing for subsequent snippets.
#
# File name extensions have the following meaning:
#  .assert - each line is an expression that can be tested with assert
#  .cp     - the listing is part of a program and can be compiled separately
#  .cpp    - the listing is a complete program
#  .cxx    - the listing contains errors
#  .func   - the listing is a function or other external entity
#  .cc     - the listing is a list of statements or local declarations
#  .txt    - the listing is not to be compiled
#  .hpp    - the listing is a header file
#
# The default extension for numbered listings is .cpp.
# The default extension for snippets is .cc.
#
# The listing and source files obey the following conventions for
# file name extensions.
#
# .assert
#    Each line is converted to an assert(LINE); statement.
#    The resulting source file is written to a file with the
#    same name but with the extension changed to .cc, so it
#    can be compiled as an ordinary snippet file.
#
# .cp
#    Each file is compiled separately. File names for parts are
#    a little different. They have the form listCCNN.TOKEN.cp,
#    where TOKEN is a unique token. All files in a chapter that
#    have the same TOKEN are linked together into a single program.
#    The program name is TOKEN.
#
#    Program parts can have CODEPREFIX and CODEPOSTFIX macros to
#    include other declarations.
#
# .cpp
#    Each file is compiled as a complete program. The program name
#    is based on the code listing name.
#
# Sometimes, a code listing depends on other declarations and statements
# that are not germane to the text, but are necessary to complete
# the program. These text files reside in this directory. Each special
# file has the same name as the snippet or listing file, but with one
# of the following extensions:
#
#   .code - replaces the list "...omitted for brevity..." in any
#           snippet or listing file. A script replaces the line with
#           #include "listCCNN.code"
#           The runcodex program invokes the script.
#   .pre -  Inserted before the code listing or snippet. For listings,
#           the insertion occurs at the start of the file, before any
#           other #include directives. For a snippet, the insertion
#           occurs inside main(), before the snippet. A script inserts
#           #include stringify(CODEPREFIX)
#           and this makefile defines the CODEPREFIX macro to be the
#           listCCNN.pre file name. The stringify macro encloses the
#           file name in double quotes.
#   .post - Inserted after the code listing or snippet. For listings,
#           the insertion occurs at the end of the file, after all
#           other code. For a snippet, the insertion occurs at the
#           end of main(), after the snippet. A script inserts
#           #include stringify(CODEPOSTFIX)
#           and this makefile defines the CODEPOSTFIX macro to be the
#           listCCNN.post file name.
#   .decl - Inserted before all #include directives, at global scope,
#           for code snippets that need additional declarations.
#           A script inserts
#           #include stringify(CODEDECL)
#           and this makefile defines the CODEDECL macro to be the
#           snipCCNN.decl file name.
#   .main - Inserted as the body of the main() function for .func files.
#           A script inserts
#           #include stringify(MAINBODY)
#           inside main(), and this makefile defines the MAINBODY macro
#           to be the listCCNN.main file name.
#
# Listings and snippets are compiled with special wrapper source files.
# The source files (function.cc, program.cc, and snippet.cc) contain
# #include directives, guarded by conditional compilation. This makefile
# defines the macros named above only when an extra file exists.
#
# Ray Lischner
# 31-July-2006

# All files are compiled on Linux and Windows using this makefile.
# The makefile must therefore detect the platform and set the compiler
# and options accordingly.

# Test whether running on Windows or Linux by checking the command shell.
ifeq ($(SHELL),/bin/sh)
# Default compiler on Linux is g++
LINUX=1
SWITCH=-
EXE=
OBJ=.o
SYNTAX=-fsyntax-only -fmessage-length=0
CXX=g++
CXXFLAGS=-ansi -pedantic -trigraphs -Wall -O1 -I.
REDIRECT_OUTPUT=2>&1
exeoutput=-o $(1)
objoutput=-c -o $(1)
defmacro=-D$(1)$(if $(2),=$(2))
endif

ifeq ($(SHELL),sh.exe)
# Default compiler on Windows is MS Visual C++ command line
WINDOWS=1
SWITCH=/
SHELL=zsh
EXE=.exe
OBJ=.obj
RM=del
CXX=cl
CXXFLAGS=/EHsc /Za /nologo /YX /Wall
SYNTAX=/Zs
REDIRECT_OUTPUT=
exeoutput=/Fe$(1)
objoutput=/c /Fo$(1)
defmacro=/D$(1)$(if $(2),=$(2))
endif

CODEX = runcodex
CODEXFLAGS = -v
DOXYGEN = doxygen
DOXYGENFLAGS =

# If run as a submake from the parent directory, $(DOCS) is already set.
# When run from the command line in this directory, it is empty, and must
# be set to the documents in the parent directory. RELATIVE_DOCS is the list
# of documents relative to the current directory. TIMESTAMPS is the list
# of timestamp directories, also relative to the current directory.
# Each document has a timestamp file in the .ts/ directory, to keep track
# of when code listings were extracted. The codex program is run again
# only when the document has changed.
ifeq ($(DOCS),)
include book.mak
DOCS := $(wildcard ../$(ISBN)ch[0-9][0-9]$(DRAFT).odt)
RELATIVE_DOCS = $(DOCS)
TIMESTAMPS = $(patsubst ../%,.ts/%,$(DOCS))
else
RELATIVE_DOCS = $(patsubst %,../%,$(DOCS))
TIMESTAMPS = $(patsubst %,.ts/%,$(DOCS))
endif

# Code listings for complete programs.
SRC := $(wildcard list[0-9][0-9][0-9][0-9].cpp snip[0-9][0-9][0-9][0-9].cpp)
PROG := $(subst .cpp,$(EXE),$(SRC))

# Code listings for programs that produce compile-time errors
ERRSRC := $(wildcard list[0-9][0-9][0-9][0-9].cxx snip[0-9][0-9][0-9][0-9].cxx)
ERROUT := $(subst .cxx,.err,$(ERRSRC))

# Snippets of code that can be compiled as statements or declarations
# within main().
SNIPPETS := $(wildcard snip[0-9][0-9][0-9][0-9].cc list[0-9][0-9][0-9][0-9].cc)
SNIPPROG := $(subst .cc,$(EXE),$(SNIPPETS))

# Expressions that must be true or false, and so can be compiled into
# calls to assert() within main().
ASSERTS := $(wildcard snip[0-9][0-9][0-9][0-9].assert)
ASSSRCS := $(subst .assert,.cc,$(ASSERTS))

# Functions or other global entities that need main() but must
# be compiled outside of main().
FUNCS    := $(wildcard snip[0-9][0-9][0-9][0-9].func list[0-9][0-9][0-9][0-9].func)
FUNCPROG := $(subst .func,$(EXE),$(FUNCS))

# Multi-listing parts. Each listing can be compiled independently.
# All the listing with the same serial token (e.g., .token.cp)
# are linked together to yield a program.
PARTS    := $(wildcard list[0-9][0-9][0-9][0-9].*.cp)
EXTRAPRTS:= $(wildcard extra[0-9][0-9][0-9][0-9].*.cp)
PARTOBJS := $(PARTS:.cp=$(OBJ)) $(EXTRAPARTS:.cp=$(OBJ))
# Extract just the serial (e.g., .token) extensions for all the
# parts by stripping the .cp extension, then keeping just the
# remaining token suffix, and sorting to remove duplicates.
PARTTOKENS := $(sort $(subst .,,$(suffix $(basename $(PARTS)))))
# The token is the program name.
PARTPROG := $(addsuffix $(EXE),$(PARTTOKENS))
# Define a function that yields the object-file dependencies
# for a part-based program, e.g., $(partdeps token) yields
# list0101.token.o list0102.token.o list0102.token.o
partdeps = $(subst .cp,.o,$(wildcard list[0-9][0-9][0-9][0-9].$(1).cp)) $(subst .cp,.o,$(wildcard extra[0-9][0-9][0-9][0-9].$(1).cp))

# If a file (basename).pre exists, specify a command line that
# defines the CODEPREFIX to be the file name. The snippet.cc file
# will #include the CODEPREFIX file.
codeprefix  = $(if $(wildcard $(basename $(1)).pre),\
  $(call defmacro,CODEPREFIX,$(basename $(1)).pre))
codepostfix = $(if $(wildcard $(basename $(1)).post),\
  $(call defmacro,CODEPOSTFIX,$(basename $(1)).post))
codemain    = $(if $(wildcard $(basename $(1)).main),\
  $(call defmacro,CODEMAIN,$(basename $(1)).main))
codedecl    = $(if $(wildcard $(basename $(1)).decl),\
  $(call defmacro,CODEDECL,$(basename $(1)).decl))

define PART_template
 $(1) : $$(call partdeps,$(1))
	$$(CXX) $$(CXXFLAGS) -o $$@ $$(call partdeps,$(1))
endef

ifdef LINUX
%.cc : %.assert
	perl -pe 's@/\*.*\*/@@ or s/^(.*);?\s*$$/assert($$1);\n/' < $< > $@

.ts/% : ../%
	$(CODEX) $(CODEXFLAGS) $<
	@touch $@

%.gch : %
	$(CXX) $(CXXFLAGS) -x c++-header -c $<
endif

%.err : %.cxx
	-$(CXX) $(CXXFLAGS) $(SYNTAX) $(call codeprefix,$<) $(call codepostfix,$<) $(call codemain,$<) $(call defmacro,PROGRAM,$<) program.cc > $@ $(REDIRECT_OUTPUT)

% : %.cpp
	$(CXX) $(CXXFLAGS) $(call codeprefix,$<) $(call codepostfix,$<) $(call codemain,$<) $(call exeoutput,$@) $(call defmacro,PROGRAM,$<) program.cc

% : %.cc
	$(CXX) $(CXXFLAGS) $(call codeprefix,$<) $(call codepostfix,$<) $(call codemain,$<) $(call codedecl,$<) $(call exeoutput,$@) $(call defmacro,SNIPPET,$<) snippet.cc

%.o : %.cp
	$(CXX) $(CXXFLAGS) $(call codeprefix,$<) $(call codepostfix,$<) $(call codemain,$<) $(call objoutput,$@) $<

% : %.func
	$(CXX) $(CXXFLAGS) $(call codeprefix,$<) $(call codepostfix,$<) $(call codemain,$<) $(call exeoutput,$@) $(call defmacro,FUNCTION,$<) function.cc

# Sometimes, the Intel compiler works better than g++.
ICC = icc
ICCFLAGS = -ansi -imacros icc.hh -Wall
icc = $(ICC) $(ICCFLAGS) -o $(1) $(call defmacro,SNIPPET,$(1).cc) snippet.cc

# Targets:
# all : make everything possible on this platform
# code : extract code listings (Linux only)
# html : extract doxygen documentation (Linux only)
# palindrome : extract doxygen example (Linux only)
# programs : compile programs (Linux and Windows)
# clean : delete object files, and programs (Linux only)
# veryclean : delete source files, too (Linux only)
# production : prepare files for production; currently nothing, later maybe make a tarball
# assert-code : recursive rule to ensure .assert snippets are compiled
# recurse-programs : recursive rule to ensure newly generated source files are handled

ifdef LINUX
all : all.hpp.gch code html
	$(MAKE) programs palindrome

intel :
	$(MAKE) CXX=icc CXXFLAGS="-ansi -imacros icc.hh -Wall" programs

snip5306: snip5306.cc
	$(call icc,$@)
snip5304: snip5304.cc
	$(call icc,$@)

endif
ifdef WINDOWS
all : programs
endif

code : $(TIMESTAMPS)
	$(MAKE) assert-code

$(foreach part,$(PARTTOKENS),$(eval $(call PART_template,$(part))))

programs : $(PROG) $(ERROUT) $(SNIPPROG) $(FUNCPROG) $(PARTPROG)

html :
	$(DOXYGEN) $(DOXYGENFLAGS)

palindrome :
	sed '1,2d' < list2401.cpp > palindrome/palindrome.cpp
	$(MAKE) -C palindrome palindrome
	cd palindrome; doxygen

clean :
	$(RM) $(PROG)
	$(RM) $(ERROUT)
	$(RM) $(SNIPPROG)
	$(RM) $(FUNCPROG)
	$(RM) $(PARTPROG) $(PARTOBJS)
	$(RM) $(ASSERTS:.assert=$(EXE))
	$(RM) *.ti *.ii
	$(RM) -r html latex

very-clean : clean
	$(RM) $(SRC) $(ERRSRC) $(SNIPPETS) $(ASSERTS) $(FUNCS) $(PARTS) $(EXTRAPARTS)
	$(RM) {list,snip}[0-9][0-9][0-9][0-9].{hpp,txt}
	$(RM) .ts/*.odt

veryclean : very-clean

assert-code : $(ASSSRCS)

production :

recurse-programs : programs

.PHONY : all code clean veryclean very-clean programs production recurse-programs palindrome
