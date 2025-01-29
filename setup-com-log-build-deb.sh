#!/bin/bash
echo "--> setup-com-log-build-deb.sh"
set -eux -o pipefail
sudo apt-get update && sudo apt-get -q -y install \
  autoconf-archive libjsoncpp-dev rpm valgrind
echo "--> setup-com-log-build-deb.sh ends"
