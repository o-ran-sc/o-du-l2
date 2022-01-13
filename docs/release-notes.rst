.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0


Release-Notes
**************


This document provides the release notes for D Release of O-DU-L2.

.. contents::
   :depth: 3
   :local:


Version history
---------------

+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2022-01-14         | 5.0.0              | Radisys,           | e-release          |
|                    |                    | HCL Technologies   |                    |
|                    |                    | Ltd.               |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2021-09-06         | 4.0.0              | Radisys,           | D Release          |
|                    |                    | HCL Technologies   |                    |
|                    |                    | Ltd.               |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-12-04         | 3.0.0              | Radisys,           | Cherry Release     |
|                    |                    | HCL Technologies   |                    |
|                    |                    | Ltd.               |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2020-06-17         | 2.0.0              | Radisys            | Bronze Release     |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2019-12-29         | 1.0.0              | Radisys            | Amber Release      |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+


Summary
-------

e-release
^^^^^^^^^^
This release contains the following:

 - Support for multiple bearers per UE

 - Support for multiple UEs per cell. Maximum 3 UEs supported in this release.

 - Enhancement of scheduler for round robin scheduling of UEs
 
 - Enhancement of scheduler to allocate grid resources to UL/DL channels based on slice(RRM Policies), UE and logical channel configurations

 - Support for Network slicing
      
      - Measures the Slice performance and periodically reports the slice performance statistics to O1.
      - Adjusting/Improving Slice performance via Slice Reconfiguration with optimized resource quota from SMO.
 
 - O1 Module

      - Support for cell configuration over O1 interface.
      - Support for RRM policy configuration over O1 interface.
      - Support VES PM data stream for sending slice metrics parameters to SMO.

D
^^^^^^^^
This release contains the following:

 - UL/DL Data transmission on FDD/Mu0/20MHz.

 - Support for static TDD at O-DU High on 100 MHz Bandwidth, numerology 1.

 - Support for Closed Loop automation use case at O-DU High.

 - O-DU low – O-DU high pairwise testing in Radio mode (Broadcast message successfully received at O-DU Low).

 - O1 Module
      
      - Re-structure O1 module to run as a thread in ODU-High.
      - CM Support - IP and Port configuration for DU, CU stub and RIC stub via Netconf interface.
      - VES PNF Registration.
      - Support for Closed Loop Automation use-case.

 - Maintenance release includes :
      
      - Memory handling optimization and fixes.
      - Improvement in logging.
      - K0, K1 and K2 configuration.
      - Fixes in proccessing of RACH Indication and RAR.

Cherry
^^^^^^^^
This release contains the following:

- Implementation of UE attach signalling procedure on single carrier.
  All message exchanges within O-DU High are in line with WG8 AAD spec.

- Enhancements to F1-C interface as required for UE attach procedure.

- Enhancements to FAPI interface towards O-DU Low as required for UE attach procedure.

- Support for all short PRACH formats.

- Integration of FAPI P5 messages with Intel's O-DU Low in Timer mode.

- Code support for alignment with latest FAPI interface files from Intel.

- Implementation of O1 interface.

- Partial implementation of Health Check use-case with get-Alarm list API.


Bronze
^^^^^^^^
This release contains the following:

- Enhancements to F1-C interface for UE attach procedure.

- Implementation of F1-U interface.

- Implementation of E2 interface.

- Support for traffic steering usecase.

- Support for single carrier.

- Implementation of basic scheduler.

- Implementation of Cell broadcast procedure.

- Implementation of UE procedure till msg-4 for single UE. Complete testing of these messages is in progress.

- Implementation of FAPI interface towards O-DU Low using WLS.

- Partial implementation of RLC layer interfaces towards upper and lower layers
  conforming to AAD spec.


Amber
^^^^^
This release contains the following:

- O-DU layer intilaizations

- Implementation of F1-C interface

- Exchange of F1 Setup Request, F1 Setup Response, GNB DU Config Update and GNB DU Config Update ACK between the ODU and CU STUB.


Release Data
------------

e-release
^^^^^^^^^^ 
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH                              |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   | o-du/l2/                             |
|                                      | I4b894c652ef3a3584670a9f26de87c2b2b3b|
|                                      | d8f2                                 |
+--------------------------------------+--------------------------------------+
| **Release designation**              | e-release                            |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     | 2022-01-14                           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | e-release                            |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

D
^^^^^^ 
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH                              |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   | o-du/l2/                             |
|                                      | e8fdaea4192b41240b8c43f48adf92eed0c3 |
|                                      | b99e                                 |
+--------------------------------------+--------------------------------------+
| **Release designation**              | D Release                            |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     | 2021-09-06                           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | D Release                            |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

Cherry
^^^^^^ 
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH                              |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   | o-du/l2/                             |
|                                      | fc0bcf28e944ae7ba2423ad3c9a5c794df2dc|
|                                      | 4ff                                  |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release designation**              | Cherry Release                       |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     | 2020-12-04                           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | Cherry Release                       |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

Bronze
^^^^^^ 
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH                              |
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
| **Purpose of the delivery**          | Bronze Release                       |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

Amber
^^^^^
+--------------------------------------+--------------------------------------+
| **Project**                          | ODUHIGH                              |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   | o-du/l2/                             |
|                                      | d349ae65e1495488772f87e5cfa1ae71d9eab|
|                                      | 075                                  |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release designation**              | Amber Release                        |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     | 2019-12-29                           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | Amber Release                        |
|                                      |                                      |
+--------------------------------------+--------------------------------------+



Feature Additions
------------------

**JIRA BACK-LOG:**

e-release
^^^^^^^^^^

+-----------------------------------------------+-----------------------------------------------+
| **JIRA REFERENCE**                            | **SLOGAN**                                    |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-351  | Support for Multi bearers                     | 
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-352  | Support for Multi UE                          |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-363  | Network Slicing support                       |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-340  | Resource allocation in time domain changes to |
|                                               | meet flexible k0, k1 and k2 values            |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-361  | Support for cell configuration over O1        |
|                                               | interface                                     |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-395  | Optimization, scaling and rework              |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+

D
^^^^^^^

+-----------------------------------------------+-----------------------------------------------+
| **JIRA REFERENCE**                            | **SLOGAN**                                    |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-264  | Support for Mu1                               |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-265  | Support for 100 MHz                           |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-266  | Support for TDD mode                          |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-267  | Integration with O-DU Low in Radio mode       |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-268  | Integration with O-CU                         |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-269  | Support for E2E testing                       |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-299  | Closed Loop Automation use-case               |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-196  | Netconf session for O1 interface for CM       |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-340  | Resource allocation in time domain changes to |
|                                               | meet flexible k0, k1 and k2 values            |
+-----------------------------------------------+-----------------------------------------------+

Cherry
^^^^^^^

+-----------------------------------------------+-----------------------------------------------+
| **JIRA REFERENCE**                            | **SLOGAN**                                    |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-10   | UE attach procedure with basic scheduling     |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-188  | Support for all short PRACH formats           |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-191  | Explore O1 interface                          |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-189  | Integration with O-DU Low                     |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-184  | UE UL Data path                               |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-185  | UE DL Data path                               |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-186  | Applying 64 QAM Modulation in DL              |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-187  | Applying 16 QAM Modulation in UL              |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-190  | Integration with VIAVI Software               |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-214  | get-AlarmList implementation on O1 interface  |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-196  | CM Support on O1 interface                    |
|                                               |                                               |
+-----------------------------------------------+-----------------------------------------------+

Previous Releases
^^^^^^^^^^^^^^^^^^


+---------------------------------------------+-------------------------------------------------+
| **JIRA REFERENCE**                          | **SLOGAN**                                      |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-1  | F1-C enhancement                                |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-5  | F1-U implementation                             |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-11 | E2 implementation                               |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-9  | Cell broadcast procedure                        |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-10 | UE attach procedure till msg-4                  |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-8  | FAPI interface implementation                   |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+
| https://jira.o-ran-sc.org/browse/ODUHIGH-27 | RLC layer interface enhancements                |
|                                             |                                                 |
+---------------------------------------------+-------------------------------------------------+

Bug Corrections
----------------

**JIRA TICKETS:**

NA


Deliverables
-------------

Software Deliverables
^^^^^^^^^^^^^^^^^^^^^^

This release contains O-DU High code, along with test code in the form of CU stub, RIC stub and phy stub.
Instructions to build and execute ODU, CU and RIC stub binaries are also present.
All of the above can be found in the o-du/l2 repo.



Documentation Deliverables
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This release contains 

- README with instruction to build and execute binaries.

- overview.rst

- release-notes.rst

- installation-guide.rst

- user-guide.rst

- api-docs.rst

- developer-guide.rst



Known Limitations, Issues and Workarounds
-----------------------------------------

System Limitations
^^^^^^^^^^^^^^^^^^
- Current code contains support only for below configuration:

   - [TDD] [Mu1] [100MHz]
   - [FDD] [Mu0] [ 20MHz]
   - Freuency Range = FR 1
   - DL/UL Modulation = QPSK

- Current code is locally tested to support upto three UEs.

- NR-MAC supports Round Robin scheduling currently.

- Cell broadcast is for SSB and SIB1 only.

- FAPI files not in-line with SCF FAPI 1.0.5.
  O-DU High currently compatible with FAPI files provided by Intel.

- Implementation of F1 reset is limited to intializing UE contexts.

- E2 interface is limited to Traffic Steering Usecase.

- Forming of RIC event trigger definition, RIC indication header and RIC indication message is unclear in the E2AP draft spec versions. Therefore, implementation does not contain accurate values. Contents of RIC indication message are mocked and triggered just once.

- On the F1-U interface, UE, RB and tunnel configurations are static.

- Cell configuration is supported by CM on O1 interface. All other configurations are static.

- O-DU High has not been integrated with O-CU.(Using Radisys commercial CU as a test fixture)

- Netconf TLS connection is not supported

- Current code supports two Network Slices, One Default and other one Dedicated Slice.

- We have to manually download the 3GPP yang models and install.

Known Issues
^^^^^^^^^^^^^

- PDSCH DMRS must not be interleaved with PDSCH allocations.

- PUSCH DMRS must not be interleaved with PUSCH allocations.

- Frequency domain allocation in DCI is a bit map where:

     - As per spec : the most significant bit corresponds to the group of lowest frequency.
     - As per L1 : the least significant bit corresponds to the lowest frequency group.

- Only Resource allocation type 1 (i.e RB allocation using Start RB and Number of RBs) is supported for PDSCH.

- Only mapping type = 1 (i.e. Type A) supported for PDSCH.

- L1 unable to process SIB1 with hardware accelerator enabled.

**JIRA TICKETS:**

NA


Workarounds
^^^^^^^^^^^

O-DU High uses FAPI interface files provided by Intel and therefore, not completely in-line with SCF FAPI 1.0.5.



References
----------
1. ORAN-WG8.AAD.0-v05.00.00

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

12. 3GPP TS 28.541 Specfication V16.6

13. O-RAN WG1.O1-Interface v04.00

14. O-RAN WG1.OAM-Architecture v04.00
