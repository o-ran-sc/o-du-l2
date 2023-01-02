#!/bin/bash

set -euxo pipefail

echo "---> unit_test.sh"

echo "Present working directory: "; pwd

# Platform-specific install methods
if (grep Ubuntu /etc/os-release > /dev/null 2>&1); then
    echo "Installing libsctp-dev dependency for Ubuntu"
    sudo apt-get update
    sudo apt-get install -y libsctp-dev
elif (grep RedHat /etc/os-release > /dev/null 2>&1) || \
     (grep CentOS /etc/os-release > /dev/null 2>&1); then
    echo "Installing lksctp-tools-devel dependency for CentOS/RedHat"
    sudo yum install -y lksctp-tools-devel
else
    echo "Unmatched OS/Distribution"
    echo "Missing sctp library not installed"
fi

echo "Running job"
cd odu;make clean_odu MACHINE=BIT64 MODE=TDD;make odu MACHINE=BIT64 MODE=TDD COVERAGE=1
