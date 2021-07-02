FROM nexus3.o-ran-sc.org:10002/o-ran-sc/bldr-ubuntu18-c-go:1.9.0

# ======================================================================

ADD . /opt/o-du-l2
WORKDIR /opt/o-du-l2

RUN apt-get update && apt-get install -y libpcap-dev

RUN cd build/odu && make clean_odu odu MACHINE=BIT64 MODE=FDD

RUN cd build/odu && make clean_odu odu MACHINE=BIT64 MODE=TDD
#CMD /opt/o-du-l2/bin/odu/odu

# add netconf user
RUN cd build/scripts && /bin/bash add_netconf_user.sh

#cleanup netconf folder and install libraries
RUN cd build/scripts && /bin/bash install_lib_O1.sh -c

# Install yang models and load initial configuration
RUN cd build/scripts && /bin/bash load_yang.sh

RUN cd build/odu && make clean_odu odu MACHINE=BIT64 MODE=FDD O1_ENABLE=YES
