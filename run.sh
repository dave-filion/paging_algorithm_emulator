#!/bin/bash

echo "Running Eviction Mode 1..."
./bin/emulator1 1 > output/1_1.out
./bin/emulator2 1 > output/1_2.out
./bin/emulator3 1 > output/1_3.out
./bin/emulator4 1 > output/1_4.out
./bin/emulator5 1 > output/1_5.out

echo "Running Eviction Mode 2..."
./bin/emulator1 2 > output/2_1.out
./bin/emulator2 2 > output/2_2.out
./bin/emulator3 2 > output/2_3.out
./bin/emulator4 2 > output/2_4.out
./bin/emulator5 2 > output/2_5.out

echo "Running Eviction Mode 3..."
./bin/emulator1 3 > output/3_1.out
./bin/emulator2 3 > output/3_2.out
./bin/emulator3 3 > output/3_3.out
./bin/emulator4 3 > output/3_4.out
./bin/emulator5 3 > output/3_5.out

echo "All simulations completed, find output in output dir"
