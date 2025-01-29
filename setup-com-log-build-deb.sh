#!/bin/bash
##############################################################################
#
#   Copyright (c) 2020 AT&T Intellectual Property.
#   Copyright (c) 2020 Nokia.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
##############################################################################

# Installs prerequisites needed to compile & test com/log code
# and build RPM/DEB packages on a Debian/Ubuntu machine.

echo "--> setup-com-log-build-deb.sh"

# Ensure we fail the job if any steps fail.
set -eux -o pipefail

# install prereqs
sudo apt-get update && sudo apt-get -q -y install \
  autoconf-archive libjsoncpp-dev rpm valgrind

echo "--> setup-com-log-build-deb.sh ends"

