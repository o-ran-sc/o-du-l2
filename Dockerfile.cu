FROM ubuntu:22.04

LABEL description="This docker file isused to build CU Image"


WORKDIR /root

# Update package lists and install required packages
RUN apt-get update  \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        git \
	libsctp-dev \
        libpcap-dev \
        libxml2-dev \
	  vim \
  libxml2-utils \
  zip \
  unzip \
  lksctp-tools \
  python3-dev \
  python3-setuptools \
  libreadline-dev \
  python3-pip \
  build-essential \
  libffi6* \
  libssl-dev \
  python3-paramiko \
  apt-utils \
  openssh-client \
  iproute2 \
  libglib* \
  autoconf automake autotools-dev libtool \
  libhugetlbfs-dev* \
  numactl \
  libnuma-dev \
  python3 \
  iproute2 \
  net-tools \
  libnuma-dev \
  dnsutils \
  python3-pip \
  python3-paramiko \
  pciutils \
  libcurl4 \
  vim \
  net-tools \
  gdb \
  libjansson-dev \
  && rm -rf /var/lib/apt/lists/*


COPY . /root/l2

COPY cu-docker-entrypoint.sh /root/

RUN cd /root/ && chmod +x cu-docker-entrypoint.sh

ENTRYPOINT ["/root/cu-docker-entrypoint.sh"]


