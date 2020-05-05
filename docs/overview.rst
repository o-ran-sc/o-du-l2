.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. SPDX-License-Identifier: CC-BY-4.0


O-DU-L2 Architecture
======================


O-DU implements the functional blocks of L2 layer of a 5G NR protocol stacks.
These layers primarily include NR MAC NR Scheduler and NR RLC layers.

O-DU modules are developed as shown in the below diagram.

.. image:: ODUArch.jpg
  :width: 400
  :alt: ODU Arch diagram

O-DU Thread Architecture
--------------------------

As shown in Figure 1, there are multiple entities within O-DU. Modules sharing a
given color belong to one thread. O-DU architecture can be defined at a thread
level as follows:

-	Thread 1: O-DU thread

-	Thread 2: DU APP inclusive of Config Handler, DU Manager, UE Manager, EGTP Handler and ASN.1 Codecs

-	Thread 3: 5G NR RLC DL and MAC (inclusive of 5G NR SCH and Lower MAC)

-	Thread 4: 5G NR RLC UL

-	Thread 5: SCTP Handler


O-DU Modules
--------------------------

DU APP 
^^^^^^^^^^^^^^^^^^
This module configures and manages all the operations of O-DU.
It interfaces with external entities as follows:

-	OAM:  DU APP interacts with OAM on the O1 interface for configuration, alarms and performance management.

-	O-CU: DU APP interacts with O-CU for RAN functionalities over the F1 interface which is built on SCTP. Control messages are exchanged on the F1-C interface and data messages on the F1-U interface.

-	RIC: DU APP interacts with RIC on E2 interface over SCTP.


DU App submodules are as follows:

-	Config Handler manages the configurations received on O1 interfaces andstores them within DU APP context.

-	DU Manager handles all cell operations at the DU APP.

-	UE Manager handles UE contexts at the DU APP.

-	SCTP handler is responsible for establishing SCTP connections with O-CU, RIC.

-	EGTP handler is responsible for establishing EGTP connection with O-CU for data message exchange.

-	ASN.1 Codecs contain ASN.1 encode/decode functions which are used for System information, F1AP and E2AP messages.

5G NR RLC
^^^^^^^^^^^^^^^^^^
This module provides services for transferring the control and data messages
between MAC layer and O-CU (via DU App).

5G NR RLC UL and 5G NR RLC DL are the sub modules of this module that implement
uplink and downlink functionality respectively. 

5G NR MAC
^^^^^^^^^^^^^^^^^^
This module uses the services of the NR physical layer to send and receive data
on the various logical channels.
Functions of the 5G NR MAC module are as follows:

-	5G NR MAC is responsible for multiplexing and de-multiplexing of the data on various logical channels.

-	5G NR SCH schedules resources on UL and DL for cell and UE based procedures.

-	Lower MAC interfaces between the MAC and the lower layers of the O-DU and implements all the messages of FAPI specification.

O-DU Utility and Common Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
These modules help in the working of O-DU and message exchanges between
the O-DU modules.


