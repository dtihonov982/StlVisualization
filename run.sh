#!/bin/bash

if [ -z $1 ]
then
    exit 0
fi

OPTIONS="-g -Wall"
VISUAL_SRC="src/testApp.cpp src/App.cpp src/Journal.cpp"
LOGGER_SRC="src/logger.cpp src/Common.cpp"

g++ $OPTIONS -o bin/logger $LOGGER_SRC
cd bin
./logger
./visual logs/$1.txt
cd ..
