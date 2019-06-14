#!/bin/bash

rm *.pb

for f in *
do
    if test -f $f
    then
        name=${f%%.*}   # name part
        ext=${f##*.}    # extension part
        if [ "$ext"x = "proto"x ]
        then
            protoc3 -o $name".pb" $f
        fi
    fi
done
