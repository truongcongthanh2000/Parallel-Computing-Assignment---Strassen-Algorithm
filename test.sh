#!/bin/bash

echo "Gen matrix"
g++ -std=c++11 gen.cpp -o gen

./gen 4 4 5 10

echo "Run strassen algorithm by divide-and-conquer"

g++ -std=c++11 strassen_divide-and-conquer.cpp -o strassen_divide-and-conquer
./strassen_divide-and-conquer

echo "Run strassen algorithm by divide-and-conquer with another padding"

g++ -std=c++11 strassen_divide-and-conquer_temp.cpp -o strassen_divide-and-conquer_temp
./strassen_divide-and-conquer_temp