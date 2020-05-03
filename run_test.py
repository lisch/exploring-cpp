#!/usr/bin/env python3

"""
usage: run_test NAME.test SOURCE_DIR BUILD_DIR
If "NAME.test" is executable, then run
python3 "BUILD_DIR/NAME.test NAME SOURCE_DIR BUILD_DIR"
and the test script performs its own check for pass/fail.

Otherwise, run "BUILD_DIR/NAME" with "SOURCE_DIR/NAME.input" as the
input or no input if the file does not exist.
The program output (merged stdout and stderr) is compared
with SOURCE_DIR/NAME.test and a unified diff is printed to stdout.
Exits with zero status if the results are identical,
otherwise prints the unified  diff to stdout
and exits with non-zero status.

Tested on Linux and Microsoft Windows.
"""

import os, sys

if len(sys.argv) != 4:
	print(__doc__, file=sys.stderr)
	sys.exit(2)

program, test_script, source_dir, build_dir = sys.argv
name,_ = os.path.splitext(test_script)

if os.name == 'nt':
	exe_extension = '.exe'
elif os.name == 'posix':
	exe_extension = ''
else:
	raise ValueError('Unknown operating system: {0}'.format(repr(os.name)))

input_path=os.path.join(source_dir, "{0}.input".format(name))
test_path=os.path.join(source_dir, test_script)
exe_path=os.path.join(build_dir, name + exe_extension)

def is_exec(filename):
	"""
	Tests whether a file is executable. On *NIX, use stat().
	On Windows, look for #! as the first two bytes.
	"""
	try:
		if os.name == 'posix':
			import stat
			st = os.stat(filename)
			mode = stat.S_IMODE(st.st_mode)
			return (mode & stat.S_IXOTH) != 0

		if os.name == 'nt':
			with open(filename, 'rb') as f:
				return f.read(2) == '#!'

		return False
	except OSError:
		return False

if is_exec(test_path):
	from subprocess import check_call
	check_call(['python3', test_path, name, source_dir, build_dir])
else:
	if os.path.exists(input_path):
		with open(input_path, 'rb') as f:
			input_text = f.read()
	else:
		input_text = b''

	from subprocess import check_call, Popen, PIPE, STDOUT
	p = Popen([exe_path], stdin=PIPE, stdout=PIPE, stderr=STDOUT)
	stdout, _ = p.communicate(input_text)
	actual = stdout.splitlines(1)

	with open(test_path, 'rb') as f:
		expected = f.readlines()

	if actual != expected:
		import difflib
		for line in difflib.diff_bytes(difflib.context_diff, expected, actual, b'Expected', b'Actual'):
			sys.stdout.write(line.decode('latin1'))
		sys.exit(1)
