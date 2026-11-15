#!/bin/bash

# Full build: configure and build the default targets. Exits with 0 if
# everything built, 1 otherwise. Arguments are passed on to pb, as with
# ./configure.sh.

cd "$(dirname "$0")"

./configure.sh "$@" || exit 1
ninja || exit 1
