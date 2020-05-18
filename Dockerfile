FROM nexus3.o-ran-sc.org:10004/o-ran-sc/bldr-ubuntu18-c-go:8-u18.04 as ubuntu

ADD . /opt/o-du-l2
WORKDIR /opt/o-du-l2

RUN apt-get update && apt-get install -y libpcap-dev

RUN cd build/odu && make clean_odu odu MACHINE=BIT64 MODE=FDD

#CMD /opt/o-du-l2/bin/odu/odu -f ../config/ssi_mem
