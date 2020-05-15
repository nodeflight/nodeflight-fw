#!/bin/bash

BASEDIR=$(git rev-parse --show-toplevel)
cd $BASEDIR

find "src" -name '*.[ch]' -not -path 'src/vendor/*' -exec uncrustify -c uncrustify.cfg --no-backup {} +