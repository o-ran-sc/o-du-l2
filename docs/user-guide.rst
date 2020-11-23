.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0
.. (c) <optionally add copywriters name>


User Guide
==========

This is the user guide for Cherry release of O-DU/l2.

.. contents::
   :depth: 3
   :local:

How to clean and build:
-----------------------

1. Building ODU binary:

   a. Build folder

      - cd l2/build/odu

   b. Building ODU binary
   
      - If testing with PHY Stub:    

        i. make odu MACHINE=BIT64 MODE=FDD

      - If testing with Intel L1:      

        i. Create folder l2/src/wls_lib. Copy wls_lib.h from <intel_directory>/phy/wls_lib/ to l2/src/wls_lib.
        ii. Create folder l2/src/dpdk_lib. Copy following files from <intel_directory>/dpdk-19.11/x86_64-native-linuxapp-gcc/include/ to l2/src/dpdk_lib.

            - rte_branch_prediction.h

            - rte_common.h

            - rte_config.h

            - rte_dev.h

            - rte_log.h

            - rte_pci_dev_feature_defs.h

            - rte_bus.h

            - rte_compat.h

            - rte_debug.h

            - rte_eal.h

            - rte_os.h

            - rte_per_lcore.h

        iii. make odu PHY=INTEL_L1 MACHINE=BIT64 MODE=FDD

   c. Cleaning ODU binary

      - make clean_odu MACHINE=BIT64 MODE=FDD

2. Building CU Stub binary:

   a. Build folder
   
      - cd l2/build/odu

   b. Building CU Stub binary
   
      - make cu_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   c. Cleaning CU Stub binary
   
      - make clean_cu NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

3. Building RIC Stub binary:

   a. Build folder
   
      - cd l2/build/odu

   b. Building RIC Stub binary
   
      - make ric_stub NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

   c. Cleaning RIC Stub binary
   
      - make clean_ric NODE=TEST_STUB MACHINE=BIT64 MODE=FDD

4. Cleaning ODU, CU Stub and RIC Stub

   a. make clean_all


How to execute:
---------------

1. Assign virtual IP addresses as follows:

   a. ifconfig <interface name>:ODU "192.168.130.81"
   b. ifconfig <interface name>:CU_STUB "192.168.130.82"
   c. ifconfig <interface name>:RIC_STUB "192.168.130.80"

2. Execute CU Stub:

   a. CU execution folder

      - cd l2/bin/cu_stub

   b. Run CU Stub binary

      - ./cu_stub

3. Execute RIC Stub:

   a. RIC execution folder

      - cd l2/bin/ric_stub

   b. Run RIC Stub binary

      - ./ric_stub

4. Execution DU:

   a. DU execution folder

      - cd l2/bin/odu

   b. Run ODU binary

      - ./odu

PS: CU stub and RIC stub must be run (in no particular sequence) before ODU


How to test with Intel L1:
--------------------------

1. Execute L1:

   a. Setup environment:
      
      - cd <intel_directory>/phy/
      - source ./setupenv.sh

   b. Run L1 binary :
      
      - cd <intel_directory>/FlexRAN/l1/bin/nr5g/gnb/l1
      - To run in timer mode : ./l1.sh -e
      - L1 is up when console prints follow:

2. Execute FAPI Translator:

   a. Setup environment:
   
      - cd <intel_directory>/phy/
      - source ./setupenv.sh

   b. Run FAPI translator binary:

      - cd <intel_directory>/phy/fapi_5g/bin/
      - ./oran_5g_fapi --cfg=oran_5g_fapi.cfg

3. Execute CU Stub, RIC Stub and DU:

   a. Run steps 1-4 in "How to execute" section.


Message Flow:
-------------

L2 opens WLS interface during DU bring up. Once the interface is ready, message exchange starts.
Following diagram shows P5 message exchange with L1 in timer mode.

.. image:: L1-L2_Message_Flow.jpg
  :width: 400
  :alt: L1-L2 Message Flow Diagram

Note: UL IQ-Sample request and response are needed by Intel L1 in timer mode only. Hence, L2 code for these are not
merged.
