#!/bin/sh

source "./assert.sh"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:env = switch%getenv (ENV) {
    | "test" => Dev
    | _ => Prod
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = /* Dev */0;" "unexpected output"

EXPR=$(cat << "RE"
    type env = Dev | Prod;
    let x:env = switch%getenv (OTHER_ENV) {
    | "test" => Dev
    | _ => Prod
    };
RE
)
out=$(bsc -c -ppx "$PPX" -e "$EXPR")

assert_eq "0" "$?" "failed to compile"
assert_contain "$out" "var x = /* Prod */1;" "unexpected output"
