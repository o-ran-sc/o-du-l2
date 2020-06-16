.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0


Release-Notes
=============


This document provides the release notes for Amber Release of O-DU-L2.

.. contents::
   :depth: 3
   :local:


Version history
---------------

+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2019-10-29         | 0.1.0              | Radisys            | Amber Release      |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-04-27         | 0.2.0              | Radisys            | Bronze Release     |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+


Summary
-------

Amber
^^^^^^^^
This release contains O-DU layer intilaizations and  exchange of 
F1 Setup Request, F1 Setup Response, GNB DU Config Update and 
GNB DU Config Update ACK between the ODU and CU STUB.

Bronze
^^^^^^^^
This release contains the following:
- Enhancements to F1-C interface.

- Implementation of F1-U inetrface.

- Implementation of E2 interface.

- Implementation of basic scheduler.

- Implementation of Cell broadcast procedure.

- Implementation of UE procedure till msg-4i for single UE. Complete testing of these messages is in progress.

- Implementation of FAPI interface towards O-DU Low using WLS.

- Partial implementation RLC layer interfaces towards upper and lower layers
  conforming to AAD spec.


Release Data
------------
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH	                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   | o-du/l2/                             |
|                                      | 27844f9c01c08472b86b1a75adaed0e450a88|
|                                      | 907                                  |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release designation**              | Bronze Release                       |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     | 2020-06-17                           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | Bronze Release               	      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+



Feature Additions
^^^^^^^^^^^^^^^^^

**JIRA BACK-LOG:**

+---------------------------------------------+--------------------------------------+
| **JIRA REFERENCE**                          | **SLOGAN**                           |
|                                             |                                      |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-1  |	F1-C enhancement                     |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-5  |	F1-U implementation                  |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-11 |	E2 implementation                    |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-9  |	Cell broadcast procedure             |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-10 |	UE attach procedure will msg-2       |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-8  |	FAPI interface implementation        |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-27 |	RLC layer inetrface enhancements     |
|                                             | 				     |
+---------------------------------------------+--------------------------------------+

Bug Corrections
^^^^^^^^^^^^^^^

**JIRA TICKETS:**

NA


Deliverables
^^^^^^^^^^^^

Software Deliverables
+++++++++++++++++++++

This release contains O-DU code, along with test code as part of CU stub, RIC stub and phy Stub.
Instructions to build and execute ODU, CU and RIC stuib binaries are alos present.
All of the above can be found in the o-du/l2 repo.



Documentation Deliverables
++++++++++++++++++++++++++

This release contains 
- README with instruction to build and execute binaries.

- overview.rst

- release-notes.rst



Known Limitations, Issues and Workarounds
-----------------------------------------

System Limitations
^^^^^^^^^^^^^^^^^^
- Current code contains support for below configuration:

   Duplex mode = FDD

   Freuency Range = FR 1

   numerology = 0

   bandwidth = 20 MHz

	DL Modulation = QPSK

	UL Modulation = QPSK

   single HARQ transmission

- Support for single UE.

- Cell boradcast is for SSB and SIB1 only.

- This release implements a basic scheduler with fixed scheduling.

- F1-C interface has been enhanced upto UE attach related messages.

- E2 interface is limited to Traffic Steering Usecase.

- Forming of RIC event trigger definition, RIC indication header and RIC indication message is unclear in the E2AP draft spec versions. Therefore, implementation does not contain accurate values.

- Contents of RIC indication message are mocked and triggered just once.

- Support for FAPI APIs required till msg-2 processing have been added.

- On the F1-U interface, UE, RB and tunnel configurations are static.



Known Issues
^^^^^^^^^^^^
None

**JIRA TICKETS:**

NA


Workarounds
^^^^^^^^^^^

NA



References
----------
1. ORAN-WG8.AAD.0-v01.00.10

2. ORAN WG3.E2AP v01.00

3. ORAN WG3.E2SM v01.00

4. 3GPP 38.473-f60 v15.3

5. 3GPP TS 38.211 v15.3

6. 3GPP TS 38.212 v15.3

7. 3GPP TS 38.213 v15.3

8. 3GPP TS 38.214 v15.3

9. 3GPP TS 38.321 v15.3

10. 3GPP TS 38.331 v15.3

11. 5G PHY FAPI Specification v1.0.5

