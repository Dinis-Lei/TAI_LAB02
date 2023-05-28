#!/bin/bash
# Basic while loop
TIMEFORMAT=%R

for k in 3 6 9 12 15 18 21 24 
do
    time ./bin/findLang examples/en_sample.txt -n 16 -k $k   >> log_en.txt
    rm -r examples/preprocess/en_sample/
done