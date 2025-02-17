#!/usr/bin/env bash
sudo apt-get -y update
sudo apt-get -y install libnsl-dev libxml2-dev libsctp-dev libpcap-dev
cd build; make
cd -
LOCATION=$(find . -name compile_commands.json)
echo "Build wrapper output file: $LOCATION"
mv $LOCATION .
