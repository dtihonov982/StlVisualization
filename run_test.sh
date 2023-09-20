#!/bin/bash
cd build/programs &&
logger/logger &&
./visual 50 logs/shuffle.txt &&
cd ..
