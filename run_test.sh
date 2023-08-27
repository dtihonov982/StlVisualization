#!/bin/bash
cd debug/programs &&
logger/logger &&
./visual 50 logs/shuffle.txt &&
cd ..
