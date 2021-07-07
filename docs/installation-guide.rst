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
| 2021-07-07         |  2.0.0             |  Radisys,          |  D Release         |
|                    |                    |  HCL Technologies  |                    |
|                    |                    |  Ltd.              |                    |
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


Setting up Netconf server (Only if O1 interface enabled)
--------------------------------------------------------
 
  Following steps are required to compile and run ODU with O1 interface enabled.

- Create a new netconf user

      Switch to root user or use sudo and run following commands

   - Ubuntu :
      | cd <O-DU High Directory>/l2/build/scripts
      | sudo ./add_netconf_user.sh

- Install Netconf libraries:

   libssh, libyang, libnetconf2, sysrepo, netopeer2

   Script is provided in the following folder to install these libraries

   - Ubuntu :
       | cd <O-DU High Directory>/l2/build/scripts
       | sudo ./install_lib_O1.sh -c

- Install the YANG modules and load initial configuration

    - Navigate to config folder and update the desired initial configuration

   - Ubuntu :
       | cd <O-DU High Directory>/l2/build/config

      | Open the startup_config.xml and edit the desired IP and Port for CU, DU and RIC.
      | Open the nacm_config.xml and edit the desired user name to provide the access to that user.
      | Open the netconf_server_ipv6.xml and edit the desired netconf server configuration.
      | Open the vesConfig.json and edit the details of VES collector.
      | Open the netconfConfig.json and edit the details of Netopeer server.
      | Install the yang modules and load initial configuration.

   - Ubuntu :
       | cd <O-DU High Directory>/l2/build/scripts
       | sudo ./load_yang.sh

- Start Netopeer2-server:

   - Ubuntu :
       | cd <O-DU High Directory>/l2/build/scripts
       | sudo ./netopeer-server.sh start


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

