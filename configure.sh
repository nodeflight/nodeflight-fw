#!/bin/bash -e

# Generates build.ninja, and compile_commands.json for the editors. Arguments
# are passed on to pb.

cd "$(dirname "$0")"

pb "$@"
ninja -t compdb > compile_commands.json
