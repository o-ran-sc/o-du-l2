FROM nexus3.o-ran-sc.org:10004/o-ran-sc/bldr-ubuntu18-c-go:9-u18.04 as ubuntu

# ======================================================================
# add netconf user
RUN \
      adduser --system netconf && \
      echo "netconf:netconf" | chpasswd

# generate ssh keys for netconf user
RUN \
      mkdir -p /home/netconf/.ssh && \
      ssh-keygen -A && \
      ssh-keygen -t dsa -P '' -f /home/netconf/.ssh/id_dsa && \
      cat /home/netconf/.ssh/id_dsa.pub > /home/netconf/.ssh/authorized_keys


ADD . /opt/o-du-l2
WORKDIR /opt/o-du-l2

RUN apt-get update && apt-get install -y libpcap-dev

RUN cd build/odu && make clean_odu odu MACHINE=BIT64 MODE=FDD

#CMD /opt/o-du-l2/bin/odu/odu

#cleanup netconf folder and install libraries
RUN cd build/o1  && rm -rf netconf && /bin/bash install_lib.sh -c

RUN cd build/o1  && make o1 MACHINE=BIT64

# Install the data models based on the ODU yang model
RUN /usr/local/bin/sysrepoctl -i build/o1/yang/o-ran-sc-odu-alarm-v1.yang
