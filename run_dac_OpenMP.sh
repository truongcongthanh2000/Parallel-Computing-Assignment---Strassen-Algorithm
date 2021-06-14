#!/bin/bash


g++ -std=c++11 gen.cpp -o gen.exe
g++ -std=c++11 strassen_dac_OpenMP.cpp -o strassen_dac_OpenMP.exe -fopenmp

for size in 64 128 256 512 1024 2048 100 1000 10000
do  
    echo "Gen matrix"
    ./gen.exe $size $size $size 3
    for THRESHOLD in 32 64 128
    do
        for proc in 1 2 4 8 16 32
        do
            echo "Run strassen algorithm by divide-and-conquer with padding and parrallel with OpenMP"
            ./strassen_dac_OpenMP.exe $THRESHOLD $proc
        done
    done
done

rm -f *.exe*