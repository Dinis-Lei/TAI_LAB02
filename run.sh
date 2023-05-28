#!/bin/bash
# Basic while loop
TIMEFORMAT=%R

for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/jpn_sample.txt  -n 10 -k $k   >> log_jpn.txt
    rm -r examples/preprocess/jpn_sample/
done