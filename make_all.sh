#!/bin/bash

for DIR in B-Tree Hash RB-Tree SDG SPS; do
  cd $DIR;
  make clean
  make
  cd ..
done 

cd STREAM
sh -x stream_gen.sh
