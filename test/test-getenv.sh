#!/bin/sh

source "./assert.sh"

out=$(bsc -c -ppx "$PPX" -e 'let x:option(string) = [%getenv ENV];')

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = \"test\"" "unexpected output"

out=$(bsc -c -ppx "$PPX" -e 'let x:option(string) = [%getenv OTHER_ENV];')

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = undefined" "unexpected output"
