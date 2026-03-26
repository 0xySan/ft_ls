#!/usr/bin/env bash

set -u
set -o pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
FT_LS="$ROOT_DIR/ft_ls"
TMP_DIR=""
FIXTURE_DIR=""
SOCK_PID=""
TOTAL=0
PASSED=0
FAILED=0
USE_VALGRIND=0
KEEP_TMP=0
NO_BUILD=0
HEAD_LINES=0

VALGRIND_ARGS=(
	valgrind
	--quiet
	--leak-check=full
	--show-leak-kinds=all
	--errors-for-leak-kinds=all
	--error-exitcode=99
)

usage() {
	cat <<EOF
Usage: ./tester.sh [options]

Options:
	--valgrind    Run every ft_ls command under valgrind
	--keep        Keep temporary test directory
	--no-build    Skip make step
	--head N      Print first N lines of stdout for ft_ls and ls on each compare case
	--help        Show this help

What it does:
	- Builds ft_ls (unless --no-build)
	- Creates a deterministic fixture tree
	- Compares many flags against system ls (LC_ALL=C)
	- Runs extra smoke tests for ft_ls-specific behavior
	- Optionally checks memory errors/leaks with valgrind
EOF
}

print_status() {
	printf '[%s] %s\n' "$1" "$2"
}

cleanup() {
	if [[ -n "$SOCK_PID" ]]; then
		kill "$SOCK_PID" >/dev/null 2>&1 || true
	fi
	if [[ "$KEEP_TMP" -eq 0 && -n "$TMP_DIR" && -d "$TMP_DIR" ]]; then
		rm -rf "$TMP_DIR"
	fi
}

trap cleanup EXIT

run_ft_ls() {
	local out_file="$1"
	local err_file="$2"
	shift 2
	if [[ "$USE_VALGRIND" -eq 1 ]]; then
		"${VALGRIND_ARGS[@]}" "$FT_LS" --color=always "$@" >"$out_file" 2>"$err_file"
	else
		"$FT_LS" --color=always "$@" >"$out_file" 2>"$err_file"
	fi
	return $?
}

run_ls() {
	local out_file="$1"
	local err_file="$2"
	shift 2
	LC_ALL=C ls --color=always "$@" >"$out_file" 2>"$err_file"
	return $?
}

strip_ansi_to() {
	local in_file="$1"
	local out_file="$2"
	python3 - "$in_file" "$out_file" <<'PY'
import re
import sys

src = sys.argv[1]
dst = sys.argv[2]

with open(src, 'rb') as f:
    data = f.read().decode('utf-8', errors='replace')

data = data.replace('\r', '')
data = re.sub(r'\x1B\[[0-9;]*[A-Za-z]', '', data)

with open(dst, 'w', encoding='utf-8') as f:
    f.write(data)
PY
}

compare_case() {
	local name="$1"
	local args="$2"
	local target="$3"
	local ft_out="$TMP_DIR/ft_${TOTAL}.out"
	local ft_err="$TMP_DIR/ft_${TOTAL}.err"
	local ls_out="$TMP_DIR/ls_${TOTAL}.out"
	local ls_err="$TMP_DIR/ls_${TOTAL}.err"
	local ft_cmp="$TMP_DIR/ft_${TOTAL}.cmp"
	local ls_cmp="$TMP_DIR/ls_${TOTAL}.cmp"
	local ft_rc
	local ls_rc

	TOTAL=$((TOTAL + 1))

	eval "run_ft_ls \"$ft_out\" \"$ft_err\" $args \"$target\""
	ft_rc=$?
	eval "run_ls \"$ls_out\" \"$ls_err\" $args \"$target\""
	ls_rc=$?
	strip_ansi_to "$ft_out" "$ft_cmp"
	strip_ansi_to "$ls_out" "$ls_cmp"

	if [[ "$HEAD_LINES" -gt 0 ]]; then
		echo "----- $name : ft_ls (head -n $HEAD_LINES) -----"
		head -n "$HEAD_LINES" "$ft_out"
		echo "----- $name : ls (head -n $HEAD_LINES) -----"
		head -n "$HEAD_LINES" "$ls_out"
	fi

	if [[ "$ft_rc" -ne "$ls_rc" ]]; then
		FAILED=$((FAILED + 1))
		print_status "FAIL" "$name (exit code ft_ls=$ft_rc ls=$ls_rc)"
		return
	fi

	if ! diff -u "$ft_cmp" "$ls_cmp" >/dev/null 2>&1; then
		FAILED=$((FAILED + 1))
		print_status "FAIL" "$name (stdout differs)"
		diff -u "$ft_cmp" "$ls_cmp" | head -n 40
		return
	fi

	if ! diff -u "$ft_err" "$ls_err" >/dev/null 2>&1; then
		FAILED=$((FAILED + 1))
		print_status "FAIL" "$name (stderr differs)"
		diff -u "$ft_err" "$ls_err" | head -n 40
		return
	fi

	PASSED=$((PASSED + 1))
	print_status "OK" "$name"
}

smoke_case() {
	local name="$1"
	local args="$2"
	local target="$3"
	local ft_out="$TMP_DIR/smoke_${TOTAL}.out"
	local ft_err="$TMP_DIR/smoke_${TOTAL}.err"

	TOTAL=$((TOTAL + 1))

	eval "run_ft_ls \"$ft_out\" \"$ft_err\" $args \"$target\""
	if [[ $? -ne 0 ]]; then
		FAILED=$((FAILED + 1))
		print_status "FAIL" "$name (ft_ls non-zero exit)"
		return
	fi

	PASSED=$((PASSED + 1))
	print_status "OK" "$name"
}

prepare_fixture() {
	TMP_DIR="$(mktemp -d /tmp/ft_ls_test.XXXXXX)"
	FIXTURE_DIR="$TMP_DIR/fixture"
	mkdir -p "$FIXTURE_DIR/dirA/sub" "$FIXTURE_DIR/dirB" "$FIXTURE_DIR/emptydir"

	printf 'hello\n' > "$FIXTURE_DIR/file.txt"
	printf 'hidden\n' > "$FIXTURE_DIR/.hidden"
	printf 'space\n' > "$FIXTURE_DIR/file with space.txt"
	printf '#!/bin/sh\necho exec\n' > "$FIXTURE_DIR/run.sh"
	chmod +x "$FIXTURE_DIR/run.sh"
	printf 'bz\n' > "$FIXTURE_DIR/archive.bz"
	printf 'gz\n' > "$FIXTURE_DIR/archive.gz"

	ln -s file.txt "$FIXTURE_DIR/link_to_file"
	ln -s missing "$FIXTURE_DIR/broken_link"
	mkfifo "$FIXTURE_DIR/fifo_pipe"

	printf 'nested\n' > "$FIXTURE_DIR/dirA/sub/nested.txt"
	printf '#!/bin/sh\nexit 0\n' > "$FIXTURE_DIR/dirA/sub/nested_exec.sh"
	chmod +x "$FIXTURE_DIR/dirA/sub/nested_exec.sh"

	touch -t 202401010101 "$FIXTURE_DIR/file.txt"
	touch -t 202402020202 "$FIXTURE_DIR/run.sh"
	touch -t 202403030303 "$FIXTURE_DIR/archive.bz"
	touch -t 202404040404 "$FIXTURE_DIR/archive.gz"
	touch -t 202405050505 "$FIXTURE_DIR/dirA/sub/nested.txt"

	python3 - <<'PY' "$FIXTURE_DIR/sock" &
import os
import socket
import sys
import time

path = sys.argv[1]
try:
		os.unlink(path)
except FileNotFoundError:
		pass
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.bind(path)
time.sleep(600)
PY
	SOCK_PID=$!
	sleep 0.1
}

run_suite() {
	local target="$FIXTURE_DIR"

	print_status "INFO" "Running compare suite against system ls"

	compare_case "default" "" "$target"
	compare_case "all (-a)" "-a" "$target"
	compare_case "almost all (-A)" "-A" "$target"
	compare_case "directory (-d)" "-d" "$target"
	compare_case "long (-l)" "-l" "$target"
	compare_case "long all (-la)" "-la" "$target"
	compare_case "recursive (-R)" "-R" "$target"
	compare_case "reverse (-r)" "-r" "$target"
	compare_case "time sort (-t)" "-t" "$target"
	compare_case "time reverse (-tr)" "-tr" "$target"
	compare_case "size blocks (-s)" "-s" "$target"
	compare_case "human (-h)" "-h" "$target"
	compare_case "si (--si)" "--si" "$target"
	compare_case "no sort (-U)" "-U" "$target"
	compare_case "disable sort + all (-f)" "-f" "$target"
	compare_case "one per line (-1)" "-1" "$target"
	compare_case "no owner (-g)" "-g" "$target"
	compare_case "no group (-G)" "-G" "$target"
	compare_case "width long (--width=40)" "--width=40" "$target"
	compare_case "width short (-w 20)" "-w 20" "$target"
	compare_case "color never" "--color=never" "$target"
	compare_case "classify short (-F)" "-F --color=never" "$target"
	compare_case "classify long (--classify)" "--classify --color=never" "$target"
	compare_case "file type (--file-type)" "--file-type --color=never" "$target"
	compare_case "combo long/classify/time" "-lFt --color=never" "$target"
	compare_case "combo recursive/all/long" "-Ral --color=never" "$target"

	print_status "INFO" "Running ft_ls-only smoke tests"

	smoke_case "hyperlink default" "--hyperlink=never" "$target"
	smoke_case "hyperlink auto" "--hyperlink=auto" "$target"
	smoke_case "color auto" "--color=auto" "$target"
	smoke_case "ls_colors extension" "--color=always" "$target"

	local ls_colors_out="$TMP_DIR/ls_colors_check.out"
	local ls_colors_err="$TMP_DIR/ls_colors_check.err"
	LS_COLORS='*.bz=01;31:*.gz=01;35' run_ft_ls "$ls_colors_out" "$ls_colors_err" --color=always "$target"
	if grep -q $'\033\[01;31marchive.bz\033\[0m' "$ls_colors_out" \
		&& grep -q $'\033\[01;35marchive.gz\033\[0m' "$ls_colors_out"; then
		PASSED=$((PASSED + 1))
		TOTAL=$((TOTAL + 1))
		print_status "OK" "ls_colors extension coloring"
	else
		FAILED=$((FAILED + 1))
		TOTAL=$((TOTAL + 1))
		print_status "FAIL" "ls_colors extension coloring"
	fi

	local err_out="$TMP_DIR/error_case.err"
	local err_std="$TMP_DIR/error_case.out"
	run_ft_ls "$err_std" "$err_out" --color=never "$target/missing_file"
	local ft_err_rc=$?
	run_ls "$TMP_DIR/error_case_ls.out" "$TMP_DIR/error_case_ls.err" --color=never "$target/missing_file"
	local ls_err_rc=$?
	TOTAL=$((TOTAL + 1))
	if [[ "$ft_err_rc" -eq "$ls_err_rc" ]]; then
		PASSED=$((PASSED + 1))
		print_status "OK" "missing file exit code parity"
	else
		FAILED=$((FAILED + 1))
		print_status "FAIL" "missing file exit code parity (ft_ls=$ft_err_rc ls=$ls_err_rc)"
	fi
}

main() {
	while [[ $# -gt 0 ]]; do
		case "$1" in
			--valgrind)
				USE_VALGRIND=1
				;;
			--keep)
				KEEP_TMP=1
				;;
			--no-build)
				NO_BUILD=1
				;;
			--head)
				if [[ $# -lt 2 ]] || [[ ! "$2" =~ ^[0-9]+$ ]]; then
					echo "--head requires a non-negative integer"
					exit 1
				fi
				HEAD_LINES="$2"
				shift
				;;
			--help|-h)
				usage
				exit 0
				;;
			*)
				echo "Unknown option: $1"
				usage
				exit 1
				;;
		esac
		shift
	done

	if [[ "$NO_BUILD" -eq 0 ]]; then
		print_status "INFO" "Building ft_ls"
		if ! make -C "$ROOT_DIR" -s; then
			print_status "FAIL" "Build failed"
			exit 1
		fi
	fi

	if [[ ! -x "$FT_LS" ]]; then
		print_status "FAIL" "Binary not found: $FT_LS"
		exit 1
	fi

	if [[ "$USE_VALGRIND" -eq 1 ]] && ! command -v valgrind >/dev/null 2>&1; then
		print_status "FAIL" "valgrind not found in PATH"
		exit 1
	fi

	prepare_fixture
	run_suite

	echo
	print_status "INFO" "Summary: total=$TOTAL passed=$PASSED failed=$FAILED"
	if [[ "$KEEP_TMP" -eq 1 ]]; then
		print_status "INFO" "Fixture kept at: $TMP_DIR"
	fi

	if [[ "$FAILED" -ne 0 ]]; then
		exit 1
	fi
	exit 0
}

main "$@"
