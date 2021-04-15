#!/bin/bash

echo "Gen matrix"
g++ -std=c++11 gen.cpp -o gen

./gen 4 4 5 10

echo "Run strassen algorithm by divide-and-conquer"

g++ -std=c++11 strassen_dac.cpp -o strassen_dac
./strassen_dac

echo "Run strassen algorithm by divide-and-conquer with lazy padding"

g++ -std=c++11 strassen_dac_lazy-padding.cpp -o strassen_dac_lazy-padding
./strassen_dac_lazy-padding

rm *.exe