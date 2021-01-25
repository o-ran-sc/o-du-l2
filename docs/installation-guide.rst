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

Following libraries are required to compile and execute O1 module:
 
- Netconf:
   libssh, libyang, libnetconf2, sysrepo, netopeer2

   Script is provided in the following folder to install these libraries

   - Ubuntu :  
   
       | cd <O-DU High Directory>/l2/build/o1
       | sudo ./install_lib.sh


Cloning code
--------------

- Create a folder to clone the O-DU High code into. The folder is hereafter referred to as <O-DU High Directory>.

- Clone code into <O-DU High Directory> 

  git clone "https://gerrit.o-ran-sc.org/r/o-du/l2"

Compilation
------------

- Build O-DU High:

   - Navigate to Build folder

       cd <O-DU High Directory>/l2/build/odu

   - Clean O-DU High binary

       make clean_odu MACHINE=BIT64 MODE=FDD
       

   - Build O-DU High binary
   
       make odu MACHINE=BIT64 MODE=FDD
       
       To build with O1 interface enabled:

       make odu MACHINE=BIT64 MODE=FDD O1_ENABLE=YES

- Build CU Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean CU Stub binary
   
       make clean_cu NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   - Build CU Stub binary
   
       make cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

- Build RIC Stub :

   - Navigate to Build folder
   
       cd <O-DU High Directory>/l2/build/odu

   - Clean RIC Stub binary
   
       make clean_ric NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   - Build RIC Stub binary
   
       make ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

- Build O-DU High with O1 interface enabled:

   - Navigate to o1 Build folder
   
       cd <O-DU High Directory>/l2/build/o1

   - Clean O1 binary
   
       make clean_o1 MACHINE=BIT64

   - Build O1 binary
   
       make o1 MACHINE=BIT64

   - Navigate to odu Build folder

       cd <O-DU High Directory>/l2/build/odu

   - Clean O-DU High binary

       make clean_odu MACHINE=BIT64 MODE=FDD
       
   - Build O-DU High binary
   
       make odu MACHINE=BIT64 MODE=FDD O1_ENABLE=YES



The above generated images can be found at:

- O-DU High - <O-DU High Directory>/l2/bin/odu

- CU Stub   - <O-DU High Directory>/l2/bin/cu_stub

- RIC Stub  - <O-DU High Directory>/l2/bin/ric_stub

- O1        - <O-DU High Directory>/l2/build/o1/bin/o1
