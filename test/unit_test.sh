#!/bin/bash


set -euxo pipefail
echo "Hello world!"
sudo apt-get update
sudo apt-cache search libsctp-dev || :
sudo apt-cache showpkg libsctp-dev || :
sudo apt-get install -y libsctp-dev
sudo dpkg-query -l
cd odu;make clean_odu MACHINE=BIT64 MODE=TDD;make odu MACHINE=BIT64 MODE=TDD COVERAGE=1
