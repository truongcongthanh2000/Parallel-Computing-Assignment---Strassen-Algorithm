#!/bin/bash
echo "Gen matrix"
g++ -std=c++11 gen.cpp -o gen.exe
./gen.exe 10 20 30 10

echo "Run strassen algorithm by divide-and-conquer with padding"
g++ -std=c++11 strassen_dac.cpp -o strassen_dac.exe
./strassen_dac.exe 64 > output_dac.txt

echo ""

echo "Run strassen algorithm by divide-and-conquer with padding and parrallel with OpenMP"
g++ -std=c++11 strassen_dac_OpenMP.cpp -o strassen_dac_OpenMP.exe -fopenmp
./strassen_dac_OpenMP.exe 64 4 > output_dac_OpenMP.txt

rm -f *.exe*