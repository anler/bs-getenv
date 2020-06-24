#!/bin/sh

source "./assert.sh"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:option(env) = switch%getenv (ENV) {
    | "test" => Dev
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = /* Dev */0;" "unexpected output"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:option(env) = switch%getenv (OTHER_ENV) {
    | "test" => Dev
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = undefined;" "unexpected output"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:option(env) = switch%getenv (OHTER_ENV) {
    | "test" | "test2"  => Dev
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = undefined;" "unexpected output"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:option(env) = switch%getenv (ENV) {
    | "test" | "test2"  => Dev
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = /* Dev */0;" "unexpected output"
