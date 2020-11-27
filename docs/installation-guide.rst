.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0

.. contents::
   :depth: 3
   :local:

========
Abstract
========

This document describes how to install O-DU High, it's dependencies and required system resources.

.. contents::
   :depth: 3
   :local:

Version history
---------------------

+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-12-04         |  1.0               |  Radisys           |  Cherry Release    |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+


Introduction
============

This document describes the hardware and software requirements along with guidelines on how to install and configure the
O-DU High.

The audience of this document is assumed to have good knowledge in RAN concepts and Linux system.


Preface
=======

Before starting the installation of O-DU High, the source can be built or docker image can be downloaded.


Hardware requirements
=====================

Following minimum hardware requirements must be met for installation of O-DU High

+--------------------+----------------------------------------------------+
| **HW Aspect**      | **Requirement**                                    |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **# of servers**   | 	1	                                          |
+--------------------+----------------------------------------------------+
| **CPU**            | 	5					          |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **RAM**            |  8G					          |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **Disk**           | 	500G				                  |
|                    |                                                    |
+--------------------+----------------------------------------------------+
| **NICs**           | 	3						  |
|                    |                                                    |
+--------------------+----------------------------------------------------+





Software installation and deployment
==========================================

This section describes the installation of the O-DU High on the reference hardware.

Libraries
^^^^^^^^^^

- GCC version 4.6.3 and above

- LKSCTP
   - Ubuntu : sudo apt-get install -y libsctp-dev
   - CentOS : yum install lksctp-tools-devel

- PCAP:
   - Ubuntu : sudo apt-get install -y libpcap-dev
   - CentOS : yum install libpcap-devel


