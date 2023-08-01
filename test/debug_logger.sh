#!/bin/bash
BIN=bin
SRC_DIR=../src
SRC="NotifyingIteratorTest.cpp $SRC_DIR/DebugLogger.cpp"
INCLUDE=$SRC_DIR
OPTIONS="-g -Wall"

set -x
g++ $OPTIONS -o $BIN/NotifyingIteratorTest -I$INCLUDE $SRC
