#!/usr/bin/env python3

"""
makecpp.py

Makes a .cpp file to correspond to a .hh file from Exploring C++ 20,
and creates a CMakeLists.txt file for all of the *.cpp files.

The codex program reads an Open Document Format (ODF) text file
and extracts code listings and snippets to .hh files or
other files as directed in the document.

For listNN.hh, the standard listNN.cpp file is as follows:
	#include "listNN.hh"
For snipNN.hh, the snipNN.cpp file is slightly more complicated:
	#include <iostream>
	int main()
	{
	#include "snipNN.hh"
	}
If the *.hh file contains a line that begins with an ellipsis
(following any number of space characters), then the .hh file
is modified in place to change the ellipsis line into a comment
and to add an #include directive of the form:
	#include "xxxxNNNN.incM"
where M starts at 0 and increases for each occurrence of an ellipsis.
If the .incM file does not exist, it is created with the line it
replaces inserted as a comment, e.g.,
	// ... insert full definitions here ...
If any file already exists, it is silently skipped and any further
processing of that file is also skipped.

A CMakeLists.txt file is also created, with a line for every
listing or snippet .cpp file, regardless of whether it
already existed or was created:
	example(XXX dependencies compile-options)
An existing CMakeLists.txt file, if it exists, is read and any
lines that do are not of the form example(name...) are preserved.
If the line does not start with example, it is copied verbatim.
If the line does have example(name, everything after name and before
the closing parenthesis is preserved.
"""

import os
import re
import sys

def notify(msg, end_of_line = False):
	sys.stderr.write(msg)
	if end_of_line:
		sys.stderr.write('\n')
	else:
		sys.stderr.write(' ')

def write_file(filename, content, end_of_line=False):
	if not os.path.exists(filename):
		with open(filename, 'wt') as f:
			f.write(content)
			notify(filename, end_of_line)

class Ellipsis:
	def __init__(self, filename, line):
		self.filename = filename
		self.line = line

	def write(self):
		write_file(self.filename, '// {0}'.format(self.line))

class File:
	"""
	Base class for files. Manage the file name.
	"""
	def __init__(self, filename):
		self._source = filename
		head, tail = os.path.split(self._source)
		base, ext = os.path.splitext(tail)
		self._head = head
		self._base = base
		self._ext = ext

	def path(self, ext):
		return os.path.join(self.head, self.base + ext)

	@property
	def head(self):
		return self._head

	@property
	def base(self):
		return self._base

	@property
	def source(self):
		return self._source


class CMakeFile(File):
	"""
	Manages a CMakeLists.txt file.
	Read an existing file and keep track of exceptions to the pattern.
	Write a new file, preserving exceptions, and writing
		example(name)
	for every list or snip name.

	When parsing, exceptions to the rule are also preserved
	in a dict, keyed by name. The key '' keeps a list of
	global exceptions, such as
	  include_directories(.)
	Local exceptions have the form
	  example(listNNNN listMMMM.hh)
	"""
	EXAMPLE  = re.compile('^example\\(([-_\\w]+)(?: +([-_/\\w. ]+))?\\)')

	def __init__(self):
		super().__init__("CMakeLists.txt")

	def process(self, filenames):
		# Parse existing file to find exceptions
		exceptions = self.parse(self.source)
		cmake = self.create_file(filenames, exceptions)

		if cmake != self.oldtext:
			# Rewrite CMakeLists.txt only if it changed.
			import os
			if os.path.exists(self.source):
				os.rename(self.source, '{0}~'.format(self.source))
			write_file(self.source, cmake, True)

	def create_file(self, filenames, exceptions):
		cmake = ''
		filenames.sort()
		for filename in filenames:
			base, ext = os.path.splitext(filename)
			exception = exceptions.get(base, '')
			if os.path.exists(base + ".hh"):
				dependency = base + '.hh'
				if dependency not in exception:
					exception +=  ' ' + dependency
			cmake += "example({0}{1})\n".format(base, exception)
		for exception in exceptions.get('', []):
			cmake += exception
		return cmake

	def parse(self, filename):
		self.oldtext = ''
		exceptions = {}
		try:
			with open(filename, 'rt') as f:
				for line in f:
					self.oldtext += line
					m = self.EXAMPLE.match(line)
					if m:
						if m.group(2):
							exceptions[m.group(1)] = ' ' + m.group(2)
					elif line.strip() and not line.startswith('run_test'):
						exceptions.setdefault('', []).append(line)
		except IOError as ex:
			import errno
			if ex.errno != errno.ENOENT:
				raise
			# Not an error if CMakeLists.txt does not yet exist.
		return exceptions

class CodeFile(File):
	"""
	Describes the characteristics of a code file.
	A code file can be a snippet or a listing.
	Snippets are intended to be code fragments and so are
	inserted into a function. Listings are meant to be at
	namespace scope and often contain main().

	A code file may contain any number of ellipsis lines,
	that is, line of code that begin with an ellipsis (...)
	after any white space. An ellipsis signifies code that
	is not relevant but may be necessary for compilation.
	Thus, a line that contains an ellipsis is replaced by
	an #include directive.
	"""
	ELLIPSIS = re.compile('^[ \t]*\\.{3}')
	def __init__(self, filename):
		super().__init__(filename)
		self._serial = 0
		self._ellipses = list()
		self._content = ''    # content of the file

	@property
	def is_snip(self):
		return self.base.startswith('snip')

	@property
	def is_list(self):
		return self.base.startswith('list')

	@property
	def hh_file(self):
		return self.path('.hh')

	@property
	def cpp_file(self):
		return self.path('.cpp')

	@property
	def inc_file(self):
		path = self.path('.inc{0}'.format(self.serial()))
		self.next()
		return path

	def serial(self):
		return self._serial

	def next(self):
		self._serial += 1
		return self._serial

	def process(self):
		# Always parse the .hh file, even if .cpp already exists.
		# The codex tool may rewrite the .hh file, so ellipses
		# may need to be repaired.
		self.parse()
		if self._ellipses:
			for ellipsis in self._ellipses:
				ellipsis.write()
			self.modify_code_file()

	def modify_code_file(self):
		os.rename(self.source, "{0}~".format(self.source))
		write_file(self.source, self._content, True)

	def parse(self):
		self._globals = set()
		self._ellipses = list()
		self._content = ''    # content of file
		with open(self.source, 'rt') as f:
			for line in f:
				self.parse_line(line)

	def parse_line(self, line):
		if self.ELLIPSIS.match(line):
			# Change line to a comment followed by an #include
			# Save the line as a comment in the newly created #include file
			inc_file = self.inc_file
			self._content += '// {0}\n#include "{1}"\n'.format(line, inc_file)
			self._ellipses.append(Ellipsis(inc_file, line))
		else:
			self._content += line

class HHFile(CodeFile):
	"""
	Describes the characteristics of a header file.
	A header file can be a snippet or a listing.
	Snippets are intended to be code fragments and so are
	inserted into a function. Listings are meant to be at
	namespace scope and often contain main().

	A header is a kind of code file and inherits the processing
	of ellipses.

	Also keep track of which functions appear to be defined
	in the code file.
	"""

	GLOBAL = re.compile('^[ \t]*(?:(?:(?:class|struct|namespace|module)\\b.*)|(?:[a-zA-Z_][\\w:]*[ \t]+)?([a-zA-Z_][\\w:]*)\\([^;]*)$')
	def __init__(self, filename):
		super().__init__(filename)
		self._globals = set()

	def parse_line(self, line):
		super().parse_line(line)
		m = self.GLOBAL.match(line)
		if m:
			self._globals.add(m[1])

	def process(self):
		super().process()
		if not os.path.exists(self.cpp_file):
			self.create_cpp_file()

	def create_cpp_file(self):
		if self.is_snip:
			self.create_snip_file()
		elif self.is_list:
			self.create_list_file()
		else:
			raise ValueError("Filename must start with 'snip' or 'list', not {0}".format(self.source))

	def write_cpp_file(self, content_fmt, *args):
		# Now write the .cpp file by passing the name of the
		# .hh file to the caller-supplied format string.
		write_file(self.cpp_file, content_fmt.format(*args), True)

	def create_list_file(self):
		if 'main' in self._globals:
			self.write_cpp_file('#include "{0}"\n', self.hh_file)
		else:
			# The listing does not have its own main(), so add one.
			self.write_cpp_file('#include <iostream>\n#include "{0}"\nint main() {{\n\n}}\n', self.hh_file)

	def create_snip_file(self):
		if 'main' in self._globals:
			self.write_cpp_file('#include "{0}"\n', self.hh_file)
		elif self._globals:
			# The snippet contains global declarations,
			# which must be at namespace-scope
			# so put the #include before main().
			self.write_cpp_file('#include <iostream>\n#include "{0}"\nint main() {{\n\n}}\n', self.hh_file)
		else:
			# The snippet does not appear to contain any global
			# declarations, so put them inside main().
			self.write_cpp_file('#include <iostream>\nint main() {{\n#include "{0}"\n}}\n', self.hh_file)


def main():
	from glob import glob
	# Process all .cpp files to manage ellipses.
	for filename in glob("*.cpp"):
		cpp_file = CodeFile(filename)
		cpp_file.process()

	# Process all .hh files to create corresponding .cpp files
	# and to manage ellipses.
	for filename in glob("*.hh"):
		hh_file = HHFile(filename)
		hh_file.process()

	# Make the CMakeLists.txt file from all .cpp files.
	cmake_file = CMakeFile()
	cmake_file.process(glob("snip*.cpp") + glob("list*.cpp"))

if __name__ == "__main__":
	if len(sys.argv) > 1:
		print(__doc__)
		sys.exit(1)
	else:
		try:
			main()
		except ValueError as ex:
			print(str(ex), file=sys.stderr)
			sys.exit(1)
		except KeyboardInterrupt:
			print('Interrupted', file=sys.stderr)
			sys.exit(257)
