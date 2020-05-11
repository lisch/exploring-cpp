#!/usr/bin/env python3
# Create trivial headers to #include system headers so the
# system headers can be precompiled as modules.
# This is a hack until g++ ships with precompiled header modules.

import os
import sys

CXX = os.environ.get('CXX', 'mod-g++')
INCLUDE_DIRECTORY = '/usr/local/include/c++/11.0.0'

# List of header names.
HEADERS = [
	'algorithm',
	'array',
	'bit',
	'bitset',
	'charconv',
	'chrono',
	'compare',
	'concepts',
	'deque',
	'exception',
	'filesystem',
	'format',
	'fstream',
	'functional',
	'initializer_list',
	'iomanip',
	'ios',
	'iosfwd',
	'iostream',
	'istream',
	'iterator',
	'limits',
	'list',
	'locale',
	'map',
	'memory',
	'numbers',
	'numeric',
	'optional',
	'ostream',
	'queue',
	'random',
	'ranges',
	'ratio',
	'regex',
	'set',
	'span',
	'sstream',
	'stdexcept',
	'streambuf',
	'string',
	'string_view',
	'system_error',
	'tuple',
	'type_traits',
	'unordered_map',
	'unordered_set',
	'utility',
	'vector',
	'version'
]

def precompile(header):
	import subprocess
	path = os.path.join(INCLUDE_DIRECTORY, header)
	cmd = [CXX, '-std=c++20', '-fmodule-header', '-c', '-x', 'c++', path]
	print(*cmd)
	subprocess.call(cmd)
	module_map = '{0} {1}\n'.format(path, os.path.abspath(os.path.join('gcm.cache' + path)))
	return module_map

def main(headers):
	try:
		module_map = ''
		for name in headers:
			module_map += precompile(name)

		with open('module.map', 'wt') as f:
			f.write(module_map)
	except KeyboardInterrupt:
		print("Interrupted", file=sys.stderr)
		sys.exit(1)

if __name__ == "__main__":
	if len(sys.argv) > 1:
		main(sys.argv[2:])
	else:
		main(HEADERS)
