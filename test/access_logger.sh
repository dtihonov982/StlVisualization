#!/bin/bash
BIN=bin
SRC_DIR=../src
SRC="AccessLoggerTest.cpp"
INCLUDE=$SRC_DIR
OPTIONS="-g -Wall"

set -x
g++ $OPTIONS -o $BIN/AccessLoggerTest -I$INCLUDE $SRC
