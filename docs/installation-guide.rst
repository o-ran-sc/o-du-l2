.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0

.. contents::
   :depth: 3
   :local:

O-DU High Installation Guide
*****************************

Abstract
========

This document describes how to install O-DU High, it's dependencies and required system resources.

.. contents::
   :depth: 3
   :local:

Version history


+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-12-04         |  1.0.1             |  HCL Technologies  |  Cherry Release    |
|                    |                    |  Ltd.              |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-12-04         |  1.0               |  Radisys           |  Cherry Release    |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+


Introduction
============

This document describes the hardware and software requirements along with guidelines on how to install O-DU High.

The audience of this document is assumed to have good knowledge in RAN concepts and Linux system.


Preface
=======

O-DU High images can be built using the source code or corresponding docker images can be downloaded.


Hardware requirements
=====================

Following minimum hardware requirements must be met for installation of O-DU High

+--------------------+----------------------------------------------------+
| **HW Aspect**      | **Requirement**                                    |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **# of servers**   | 	1	                                          |
+--------------------+----------------------------------------------------+
| **CPU**            | 	4					          |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **RAM**            |  8G					          |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **Disk**           | 	500G				                  |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **NICs**           | 	1						  |
|                    |                                                    |
+--------------------+----------------------------------------------------+


Software installation and deployment
==========================================

This section describes the installation of the O-DU High on the reference hardware.

Libraries
----------

Following libraries are required to compile and execute O-DU High:

- GCC 
   - Ubuntu : sudo apt-get install -y build-essential
   - CentOS : sudo yum groups mark install -y “Development Tools”

   Ensure the version is 4.6.3 and above using

   -	gcc --version

- LKSCTP
   - Ubuntu : sudo apt-get install -y libsctp-dev
   - CentOS : sudo yum install -y lksctp-tools-devel

- PCAP:
   - Ubuntu : sudo apt-get install -y libpcap-dev
   - CentOS : sudo yum install -y libpcap-devel



Cloning code
--------------

- Create a folder to clone the O-DU High code into. The folder is hereafter referred to as <O-DU High Directory>.

- Clone code into <O-DU High Directory> 

  git clone "https://gerrit.o-ran-sc.org/r/o-du/l2"


Setting up Netconf server
-------------------------
 
  Following steps are required to compile ODU with O1 interface enabled:

- Install Netconf libraries:
   
   libssh, libyang, libnetconf2, sysrepo, netopeer2

   Script is provided in the following folder to install these libraries

   - Ubuntu :  
   
       | cd <O-DU High Directory>/l2/build/scripts
       | sudo ./install_lib_O1.sh -c

- Start Netopeer2-server:
       
   - Ubuntu :  
       | cd <O-DU High Directory>/l2/build/scripts
       | sudo ./netopeer-server.sh start

- Create a new netconf user
      
      Switch to root user and run following commands
      
   - Ubuntu :  
   
      | adduser --system netconf && \\
      |    echo "netconf:netconf!" | chpasswd

      | mkdir -p /home/netconf/.ssh && \\
      | ssh-keygen -A && \\
      | ssh-keygen -t dsa -P '' -f /home/netconf/.ssh/id_dsa && \\
      |    cat /home/netconf/.ssh/id_dsa.pub > /home/netconf/.ssh/authorized_keys

- Install the YANG modules

   - Ubuntu :

      | cd <O-DU High Directory>/l2/build/yang
      | sysrepoctl -i ./yang/o-ran-sc-odu-alarm-v1.yang
      | sysrepoctl -i ./yang/o-ran-sc-odu-interface-v1.yang
      | sysrepoctl -i ./yang/o-ran-sc-du-hello-world.yang

- Configure the startup IP and Port configurations for DU, CU and RIC

   - Ubuntu :

      | cd <O-DU High Directory>/l2/build/config
      |
      | Open the startup_config.xml and edit the desired IP and Port for CU, DU and RIC.
      | Then load the configuration in the sysrepo running datastore using the command below
      |
      | sysrepocfg --import=startup_config.xml --datastore running --module  o-ran-sc-odu-interface-v1

- Configure the netconf server details for VES PNF Event

   - Ubuntu :

      | cd <O-DU High Directory>/l2/build/config
      |
      | Open the netconfConfig.json and edit the desired MAC address, IP, Port, Username and Password for VES PNF Registration.

-  Configure the VES server details to send VES Events

   - Ubuntu :

      | cd <O-DU High Directory>/l2/build/config
      |
      |  Open the vesConfig.json and edit the desired IP, Port, Username and Password to send VES Event.

- Configure the nacm module to provide access to new user

   - Ubuntu :

      | cd <O-DU High Directory>/l2/build/config
      |
      | Open the nacm_config.xml and edit the desired user-name to provide the access to that user.
      |
      | $sysrepocfg --import=nacm_config.xml --datastore running --module  ietf-netconf-acm


Compilation
------------

- Build O-DU High:

   - Navigate to Build folder

       cd <O-DU High Directory>/l2/build/odu

   - Clean O-DU High binary

       make clean_odu MACHINE=BIT64 MODE=FDD
       

   - Compile O-DU High binary
   
       make odu MACHINE=BIT64 MODE=FDD
       

- Build CU Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean CU Stub binary
   
       make clean_cu NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   - Compile CU Stub binary
   
       make cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

- Build RIC Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean RIC Stub binary
   
       make clean_ric NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   - Compile RIC Stub binary
   
       make ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD


Compilation with O1 interface enabled
--------------------------------------

- Build O-DU High:

   - Navigate to Build folder

       cd <O-DU High Directory>/l2/build/odu

   - Clean O-DU High binary

       make clean_odu MACHINE=BIT64 MODE=FDD O1_ENABLE=YES
       

   - Compile O-DU High binary
   
       make odu MACHINE=BIT64 MODE=FDD O1_ENABLE=YES
       

- Build CU Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean CU Stub binary
   
       make clean_cu NODE=TEST_STUB MACHINE=BIT64 MODE=FDD O1_ENABLE=YES

   - Compile CU Stub binary
   
       make cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD O1_ENABLE=YES

- Build RIC Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean RIC Stub binary
   
       make clean_ric NODE=TEST_STUB MACHINE=BIT64 MODE=FDD O1_ENABLE=YES

   - Compile RIC Stub binary
   
       make ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD O1_ENABLE=YES



The above generated images can be found at:

- O-DU High - <O-DU High Directory>/l2/bin/odu

- CU Stub   - <O-DU High Directory>/l2/bin/cu_stub

- RIC Stub  - <O-DU High Directory>/l2/bin/ric_stub

