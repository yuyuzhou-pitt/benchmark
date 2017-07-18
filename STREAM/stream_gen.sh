#!/bin/bash

#   define STREAM_ARRAY_SIZE  2800  //64KB
#   define STREAM_ARRAY_SIZE  45000  //1MB
#   define STREAM_ARRAY_SIZE  90000  //2MB
#   define STREAM_ARRAY_SIZE  180000  //4MB
#   define STREAM_ARRAY_SIZE  350000  //8MB 
#   define STREAM_ARRAY_SIZE  700000  //16MB 
#   define STREAM_ARRAY_SIZE  1400000  //32MB 
#   define STREAM_ARRAY_SIZE  4500000  //100MB

SRC=stream.c
SUFIX=1MB
rm -rf stream_c_*.exe
for MEM in 2800 45000 90000 180000 350000 700000 1400000 4500000; do
    sed -ie "s/^\#   define STREAM_ARRAY_SIZE/\/\/\#   define STREAM_ARRAY_SIZE/g" $SRC
    sed -ie "s/^\/\/\#   define STREAM_ARRAY_SIZE\t$MEM /\#   define STREAM_ARRAY_SIZE\t$MEM /g" $SRC
    make clean
    make
    grep $MEM $SRC

    case $MEM in
        2800) SUFIX=64KB; ;;
        45000) SUFIX=1MB; ;;
        90000) SUFIX=2MB; ;;
        180000) SUFIX=4MB; ;;
        350000) SUFIX=8MB; ;;
        700000) SUFIX=16MB; ;;
        1400000) SUFIX=32MB; ;;
        4500000) SUFIX=100MB; ;;
    esac;

    mv stream_c.exe stream_c_${SUFIX}.exe
    #./stream_c_${SUFIX}.exe
done
