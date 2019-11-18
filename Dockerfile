FROM nexus3.o-ran-sc.org:10004/bldr-ubuntu16-c-go:2-u16.04-nng as ubuntu

ADD . /opt/o-du-l2
WORKDIR /opt/o-du-l2

RUN apt-get update && apt-get install -y libpcap-dev

RUN cd build/odu && make clean_all odu MACHINE=BIT64 MODE=FDD

#CMD /opt/o-du-l2/bin/odu/odu -f ../config/ssi_mem
