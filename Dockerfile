FROM nexus3.o-ran-sc.org:10004/bldr-ubuntu16-c-go:2-u16.04-nng as ubuntu

WORKDIR .

RUN cd ./build/odu
RUN make clean_all odu cu_stub MACHINE=BIT64 MODE=FDD
#RUN make cu_stub MACHINE=BIT64 MODE=FDD

