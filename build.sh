#!/bin/bash
if [ -z $1 ]
then
    g++ -g -o bin/visual src/testApp.cpp src/App.cpp src/Journal.cpp -lSDL2
fi

case $1 in
    testv) bin/visual bin/logs/heap_sort.txt 60 ;;
esac
