#!/bin/bash

rm -f images/log.png
rm -f images/gen-*.avi
    rm -f logs/gen-*.txt
rm -f logs/epoch-log.txt
docker run --rm -ti -v `pwd`:/app --name biosim biosim4 ./__runwithtee.sh
#screen -S biosim4 docker run --rm -ti -v `pwd`:/app --name biosim biosim4 './bin/Release/biosim4 biosim4.ini | tee logs/simulation.log'
