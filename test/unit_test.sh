#!/bin/bash

set -euxo pipefail

echo "---> unit_test.sh"

echo "Present working directory: "; pwd
echo "Installing libsctp-dev package dependency..."
sudo apt-get update
sudo apt-get install -y libsctp-dev
echo "Running job"
cd odu;make clean_odu MACHINE=BIT64 MODE=TDD;make odu MACHINE=BIT64 MODE=TDD COVERAGE=1
