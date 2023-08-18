#!/bin/bash

OPTIONS="-g -Wall"
SRC="src/Logger/main.cpp src/Logger/Stopwatch.cpp src/Common/Common.cpp src/Common/Script.cpp"
HEADERS="-Isrc/Logger -Isrc/Common"

set -x
g++ $OPTIONS -o bin/logger $SRC $HEADERS -lgtest
