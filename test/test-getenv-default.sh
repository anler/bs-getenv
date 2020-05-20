#!/bin/sh

source "./assert.sh"

out=$(bsc -c -ppx "$PPX" -e 'let x:string = [%getenv ENV; "unknown"];')

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = \"test\"" "unexpected output"

out=$(bsc -c -ppx "$PPX" -e 'let x:string = [%getenv OTHER_ENV; "unknown"];')

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = \"unknown\"" "unexpected output"
