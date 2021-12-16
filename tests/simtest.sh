#!/bin/sh

# This script tests the biosim4 simulator using the config file named "biosim4-test-1.ini".
# Criteria for a successful test are no stderr output and certain values in the last line
# of logs/epoch-log.txt.

./bin/Release/biosim4 ./tests/configs/biosim4-test-1.ini  > out  2> err

success=1

if test -s err; then
    echo "The simulator output errors:"
    head -10 err
    success=0
fi

result=`tail -1 ./logs/epoch-log.txt`
generation=`echo $result | cut -d" " -f 1`
survivors=`echo $result | cut -d" " -f 2`
diversity=`echo $result | cut -d" " -f 3`
genomeSize=`echo $result | cut -d" " -f 4`
kills=`echo $result | cut -d" " -f 5`

if [ $generation -ne 100 ]; then
    echo "Error: generation number, expected 100, got " $generation
    success=0
fi

if [ $survivors -le 266 -o $survivors -gt 284 ]; then
    echo "Error: survivors, expected 266 to 284, got " $survivors
    success=0
fi

if [ `expr $diversity \<  0.314` -eq 1 -o `expr $diversity \> 0.551` -eq 1 ]; then
    echo "Error: diversity, expected 0.314 to 0.551, got " $diversity
    success=0
fi

if [ $genomeSize -ne 64 ]; then
    echo "Error: genome size, expected 64, got " $genomeSize
    success=0
fi

if [ $kills -ne 0 ]; then
    echo "Error: number of kills, expected 0, got " $kills
    success=0
fi

if [ $success -eq 1 ]; then
    echo "Pass."
else
    echo "Fail."
fi