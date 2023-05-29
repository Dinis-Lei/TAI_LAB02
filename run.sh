#!/bin/bash
# Basic while loop
TIMEFORMAT=%R

echo "cantonese_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/cantonese_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/cantonese_sample/
done

echo "en_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/en_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/en_sample/
done

echo "jpn_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/jpn_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/jpn_sample/
done



echo "latin_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/latin_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/latin_sample/
done

echo "pt_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/pt_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/pt_sample/
done



echo "russian_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/russian_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/russian_sample/
done


echo "spanish_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/spanish_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/spanish_sample/
done


echo "\russian_en_sample.txt"
for k in 3 6 9 12 15 
do
    time ./bin/findLang examples/russian_en_sample.txt  -n 10 -k $k   >> log2.txt
    rm -r examples/preprocess/russian_en_sample/
done
