#!/bin/bash


g++ -std=c++11 gen.cpp -o gen.exe
g++ -std=c++11 strassen_dac.cpp -o strassen_dac.exe

for size in 16 32 64 128 256 512 1024 2048 10 100 1000 
do  
    echo "Gen matrix"
    ./gen.exe $size $size $size 10
    for THRESHOLD in 32 64 128
    do
        echo "Run strassen algorithm by divide-and-conquer with padding"
        ./strassen_dac.exe $THRESHOLD
    done
done

rm -f *.exe*