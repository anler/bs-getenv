#!/bin/sh

source "./assert.sh"

out=$(bsc -c -ppx "$PPX" -e 'let x:string = [%getenv.exn ENV];')

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = \"test\"" "unexpected output"

out=$(bsc -c -ppx "$PPX" -e 'let x:string = [%getenv.exn OTHER_ENV];' 2>&1)

assert_eq "2" "$?" "compiled when it shouldn't"
assert_contain "$out" "%getenv environment variable not found: OTHER_ENV" "unexpected output"
