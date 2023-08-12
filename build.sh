#!/bin/bash

OPTIONS="-g -Wall"
VISUAL_SRC="src/testApp.cpp src/App.cpp src/Script.cpp src/Common.cpp"
LOGGER_SRC="src/logger.cpp src/Common.cpp"

if [ $# = 0 ]
then
    set -x
    g++ $OPTIONS -o bin/visual $VISUAL_SRC -lSDL2
fi

case $1 in
    testv) bin/visual bin/logs/heap_sort.txt 60 ;;
esac
