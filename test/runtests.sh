#!/bin/sh

set -e

export PPX="../_build/default/bin/bin.exe"
export ENV="test"

pushd test >/dev/null
for t in $(ls test-*); do
    echo "Running test $t"
    sh "$t"
done;
popd >/dev/null
