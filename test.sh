#!/bin/bash

echo "Gen matrix"
g++ -std=c++11 gen.cpp -o gen

./gen 4 10