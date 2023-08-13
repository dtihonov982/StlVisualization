#!/bin/bash

if [ -z $1 ]
then
    echo "Usage show <algo_name>"
    exit 0
fi

cd bin
./visual 60 logs/$1.txt
cd ..
