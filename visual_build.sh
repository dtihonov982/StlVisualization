#!/bin/bash

OPTIONS="-g -Wall"
VISUAL_SRC="src/Visual/main.cpp src/Visual/App.cpp src/Common/Script.cpp src/Common/Common.cpp"
HEADERS="-Isrc/Visual -Isrc/Common"

set -x
g++ $OPTIONS -o bin/visual $VISUAL_SRC $HEADERS -lSDL2
