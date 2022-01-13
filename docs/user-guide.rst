.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0

User Guide
***********

This is the user guide for e-release of O-DU/l2.
Follow installation-guide to get all the dependencies ready.

.. contents::
   :depth: 3
   :local:


A. Execution:
-------------

I. Execution - On locally compiling O-DU High Source Code
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Assign virtual IP addresses as follows:

   a. ifconfig <interface name>:ODU "192.168.130.81"
   b. ifconfig <interface name>:CU_STUB "192.168.130.82"
   c. ifconfig <interface name>:RIC_STUB "192.168.130.80"

PS: If O1 interface is enabled, IPs should match those configured in "startup_config.xml"
    ( Refer Installation Guide - "Setting up Netconf server" )

2. Execute CU Stub:

   a. Navigate to CU execution folder

      - cd <O-DU High Directory>/l2/bin/cu_stub

   b. Run CU Stub binary

      - ./cu_stub

3. Execute RIC Stub:

   a. Navigate to RIC execution folder

      - cd <O-DU High Directory>/l2/bin/ric_stub

   b. Run RIC Stub binary

      - ./ric_stub

4. Execute O-DU High:

   a. Navigate to ODU execution folder

      - cd <O-DU High Directory>/l2/bin/odu

   b. Run ODU binary

      - ./odu

PS: CU stub and RIC stub must be run (in no particular sequence) before ODU. 
    In case O1 is enabled and SMO is not available follow section E below.

II. Execution - Using Docker Images
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The call flow between O-DU High and CU Stub can be achieved by executing docker containers.

- Pull the last built docker images:
    -	docker pull nexus3.o-ran-sc.org:10004/o-ran-sc/o-du-l2:5.0.3
    -	docker pull nexus3.o-ran-sc.org:10004/o-ran-sc/o-du-l2-cu-stub:5.0.3

- Run CU Stub docker:
    - docker run -it --privileged --net=host --entrypoint bash
      nexus3.o-ran-sc.org:10004/o-ran-sc/o-du-l2-cu-stub:5.0.3   
    - ./cu_stub

- Run ODU docker:
    - docker run -it --privileged --net=host --entrypoint bash
      nexus3.o-ran-sc.org:10004/o-ran-sc/o-du-l2:5.0.3   
    - ./odu


B. Pairwise testing with Intel O-DU Low:
-----------------------------------------

This section describes the changes required in compilation and execution of O-DU High binaries to successfully integrate
with Intel O-DU Low in timer mode.


I. Pre-requisites
^^^^^^^^^^^^^^^^^^

   1. Install O-DU High as per installation-guide . 

   2. Clone O-DU Low code in <O-DU Low Directory> from

      a. https://gerrit.o-ran-sc.org/r/admin/repos/o-du/phy and,

      b. https://github.com/intel/FlexRAN
      
   3. Install O-DU Low as per https://docs.o-ran-sc.org/projects/o-ran-sc-o-du-phy/en/latest/index.html .


II. Compilation
^^^^^^^^^^^^^^^^

   1. Build ODU :

      a. Create folder <O-DU High Directory>/l2/src/wls_lib. Copy wls_lib.h from <O-DU Low Directory>/phy/wls_lib/ to 
         <O-DU High Directory>/l2/src/wls_lib.

      b. Create folder <O-DU High Directory>/l2/src/dpdk_lib. Copy following files from
         <O-DU Low Directory>/dpdk-19.11/x86_64-native-linuxapp-gcc/include/ to <O-DU High Directory>/l2/src/dpdk_lib.
         
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

      c. Navigate to build folder

         - cd <O-DU High Directory>/l2/build/odu

      d. Build ODU Binary:
           
         - make odu PHY=INTEL_L1 MACHINE=BIT64 MODE=FDD


III. Execution
^^^^^^^^^^^^^^^

   1. Execute O-DU Low:

      a. Setup environment:
      
         - cd <O-DU Low Directory>/phy/
         - source ./setupenv.sh

      b. Run O-DU Low binary :
      
         - cd <O-DU Low Directory>/FlexRAN/l1/bin/nr5g/gnb/l1
         - To run in radio mode : ./l1.sh -xran
         - L1 is up when following prints are seen on console:

		| Non BBU threads in application
		| \==================================================================
		| nr5g_gnb_phy2mac_api_proc_stats_thread: [PID: 8659] binding on [CPU 0] [PRIO: 0] [POLICY: 1]
		| wls_rx_handler (non-rt):                [PID: 8663] binding on [CPU 0]
		| \==================================================================
	  
		PHY>welcome to application console

   2. Execute FAPI Translator:

      a. Setup environment:
   
         - cd <O-DU Low Directory>/phy/
         - source ./setupenv.sh

      b. Run FAPI translator binary:

         - cd <O-DU Low Directory>/phy/fapi_5g/bin/
         - ./oran_5g_fapi --cfg=oran_5g_fapi.cfg

   3. Execute CU Stub and RIC Stub:

      a. Run steps in sections A.I.1 through A.I.3 .

   4. Execute DU:
   
      a. DU execution folder
     
         - cd <O-DU High Directory>/l2/bin/odu
      
      b. Export WLS library path

         - export LD_LIBRARY_PATH=<O-DU Low Directory>/phy/wls_lib/lib:$LD_LIBRARY_PATH
      
      c. Run ODU binary

         - ./odu


C. Message Flow:
----------------

O-DU High opens WLS interface during bring up. Message exchanges can begin once the interface is ready.
Following diagram shows P5 messages exchanged with O-DU Low in timer mode.

.. figure:: O-DU_High_Low_Flow.PNG
  :width: 600
  :alt: Figure 7 O-DU High - O-DU Low Message Flow Diagram

  Figure 7 - O-DU High - O-DU Low Message Flow Diagram

Note: UL IQ-Sample request and response are needed by Intel O-DU Low in timer mode(testing mode) only. Code changes for
these are guarded under INTEL_TIMER_MODE flag which can be enabled using compilation option "PHY_MODE=TIMER", as
mentioned in section B.I.1.d .


D. Health Check execution: get alarm-list
-------------------------------------------

To execute the get alarm-list flow, following steps are required to be executed:

   1.	Start Netconf netopeer client 
   
   2. Connect to the server with 

      | user: netconf
      | pwd:  netconf!

   3. Send a Netconf get request for alarms xpath

Here are the steps as executed in the terminal 

     |  netopeer2-cli
     |  > connect --login netconf
     |  Interactive SSH Authentication
     |  Type your password:
     |  Password:
     |  > get --filter-xpath /o-ran-sc-odu-alarm-v1\:odu/alarms
     |  DATA
     |  <odu xmlns=\"urn\:o-ran\:odu\:alarm\:1.0\">
     |    <alarms>
     |      <alarm>
     |        <alarm-id>1009</alarm-id>
     |        <alarm-text>cell id  [1] is up</alarm-text>
     |        <severity>2</severity>
     |        <status>Active</status>
     |        <additional-info>cell UP</additional-info>
     |      </alarm>
     |    </alarms>
     |  </odu>

The XML output is a list of active alarms in the O-DU High system.

E. Push cell and slice configuration over O1 using netopeer-cli
---------------------------------------------------------------

When O-DU High is run with O1 enabled it waits for cell configuration to be pushed by SMO. In case the SMO is not available then these configurations can be pushed via netopeer-cli as follows.

   1. Follow step D.1 and D.2.
   2. update cellConfig.xml and rrmPolicy.xml.

      | $cd <O-DU High Directory>/l2/build/config
      | $edit-config --target candidate --config=cellConfig.xml
      | $edit-config --target candidate --config=rrmPolicy.xml
