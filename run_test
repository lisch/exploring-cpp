#!/bin/bash -e

if [ "$*" = "--help" ]; then
	cat <<EOF
usage: $0 NAME.test SOURCE_DIR BUILD_DIR
If "NAME.test" is executable, then exec
"BUILD_DIR/NAME.test NAME SOURCE_DIR BUILD_DIR".
Otherwise, run "BUILD_DIR/NAME" with "SOURCE_DIR/NAME.input" as the
input or /dev/null if the file does not exist.
The program output (merged stdout and stderr) is compared
with SOURCE_DIR/NAME.test and a unified diff is printed to stdout.
Exits with zero status if the results are identical,
otherwise prints the unified, colorized diff to stdout
and exits with non-zero status.
EOF
	exit 1
fi

name=$(basename "$1" .test)
source_dir="$2"
build_dir="$3"

input_file="$source_dir/$name.input"
test_file="$source_dir/$1"
exe_file="$build_dir/$name"

if [ -x "$test_file" ]; then
	exec "$test_file" "$name" "$source_dir" "$build_dir"
elif [ -f "$test_file" ]; then
	if [ -f "$input_file" ]; then
		input="$input_file"
	else
		input=/dev/null
	fi

	"$exe_file" < "$input" |& diff --color=always -u "$test_file" -
else
	echo "$test_file: No file or directory"
	exit 1
fi
