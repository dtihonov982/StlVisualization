#!/bin/bash

OPTIONS="-g -Wall"
SRC="src/Logger/main.cpp src/Common/Common.cpp"
HEADERS="-Isrc/Logger -Isrc/Common"

set -x
g++ $OPTIONS -o bin/logger $SRC $HEADERS -lgtest
