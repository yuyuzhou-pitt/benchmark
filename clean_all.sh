#!/bin/bash

for DIR in B-Tree Hash RB-Tree SDG SPS; do
  cd $DIR;
  make clean
  cd ..
done 

cd STREAM
make clean
rm -rf stream_c*.exe
cd ..
