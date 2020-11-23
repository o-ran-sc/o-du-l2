.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0



API-Docs
========

This is the API-docs for Cherry relase o-du/l2.

.. contents::
   :depth: 3
   :local:


Introduction
-----------------
This document lists the APIs implemented between various modules of ODU-High and its function.
These are in line with ORAN-WG8.AAD-v2.05.00.docx.

API Functions
-------------
1. RLC-MAC Interface

   - ORAN-WG8.AAD-v2.05.00.docx : section 9.2.1

   A. RLC to MAC

      a. Data Transfer(DL)

         - Section 9.2.1.1
         - Transfers DL data to MAC

      b. Buffer Status Reporting (DL)

         - Section 9.2.1.4
         - Reports DL data volume pending in the RLC entity

   B. MAC to RLC

      a. Data Transfer(UL)
         
	 - Section 9.2.1.2
         - Transfers UL data to RLC

      b. Schedule Result Reporting (DL)

         - Section 9.2.1.3
         - Reports DL schedule result to RLC

2. MAC-Scheduler Interface

   - ORAN-WG8.AAD-v2.05.00.docx : section 9.2.2

   A. MAC to Scheduler

      a. Air Interface Time

         - Section 9.2.2.1.1
	 - Sends timing information over air interface in terms of SFN and slot.

      b. Cell Configuration Request

         - Section 9.2.2.2.1
         - MAC provides SCH the cell configuration

      c. Add UE Configuration Request
         
	 - Section 9.2.2.2.3
	 - MAC provides SCH the configuration/capabilities of a UE

      d. Modify UE Reconfiguration Request

         - Section 9.2.2.2.4
	 - MAC provides SCH the re-configuration of UE, also used to add/release/modify existing bearers

      e. RACH Indication

         - Section 9.2.2.2.10
	 - MAC receives the RACH Indication and share the contents with SCH

      f. DL RLC Buffer Status Information

         - Section 9.2.2.2.14
         - Reports data volume per LC when it is received at MAC

      g. UL Buffer Status Report Indication

         - Section 9.2.2.2.16
	 - On reception and decoding of received BSR from the UE, MAC provides SCH buffer status report for UL
	   scheduling
	 
   B. Scheduler to MAC

      a. Cell Configuration Response
         
	 - Section 9.2.2.3.1
	 - Response to cell configuration request from MAC

      b. UE Reconfiguration Response

         - Section 9.2.2.3.3
         - Response to UE reconfiguration request from MAC

      c. DL Scheduling Information

         - Section 9.2.2.3.5
	 - Provides scheduling information for a given slot for downlink data
    
      d. UL Scheduling Information

         - Section 9.2.2.3.6
	 - Provides scheduled information for MAC to form the UL_TTI.request towards L1

      e. RAR Information

         - Section 9.2.2.3.7
	 - Informs MAC of uplink scheduling and Msg3 scheduling information.

      f. Downlink Control Channel Information

         - Section 9.2.2.3.8
	 - Provides DCI scheduling information for a given slot on PDCCH

      g. Downlink Broadcast Allocation

         - Section 9.2.2.3.9
	 - Provides DCI and DL data scheduling information for SIB1 for a given slot for Broadcast Channel.

3. DU APP - MAC Interface

   - ORAN-WG8.AAD-v2.05.00.docx : section 9.2.3

   A. DU APP to MAC
   
      a. Cell Configuration Request

         - Configures cell information at MAC
      
      b. Cell Start
         
	 - Section 9.2.3.1
	 - Commands MAC to start cell at L1 and to start broadcasting system information.

      c. UE Create Request
         
	 - Section 9.2.3.3
	 - Adds UE information at MAC

      d. UE Reconfiguration Request

         - Section 9.2.3.5
	 - Reconfigure UE information for a UE already added at MAC

      e. DL CCCH Indication

         - Section 9.2.3.16
	 - Sends DL CCCH Message to MAC to be sent to UE

   B. MAC to DU APP

      a. Cell Configuration Response

         - Response to Cell Configuration Request from DU APP
      
      b. UE Create Response
         
	 - Section 9.2.3.4
	 - Response to UE Create Request from DU Manager

      c. UE Reconfiguration Response

         - Section 9.2.3.6
	 - Response to UE Reconfiguration Request from DU APP

      d. UL-CCCH Indication
        
	 - Section 9.2.3.15
	 - Sends the UL CCCH Message received from UE to DU APP

4. DU APP - RLC Interface

   - ORAN-WG8.AAD-v2.05.00.docx : section 9.2.4

   A. DU APP to RLC

      a. UE Create

         - Section 9.2.4.1
         - Adds UE associated RLC entity and related information

      b. UE Reconfiguration

         - Section 9.2.4.1
	 - Reconfigures the UE associated RLC entity previously added at RLC.

      c. DL RRC Message Transfer

         - Section 9.2.4.3
         - Requests RLC to deliver RRC message from CU to the UE

   B. RLC to DU APP

      a. UE Create Response

         - Response to UE Create Request from DU APP

      b. UE Reconfiguration Response

         - Response to UE Reconfiguration Request from DU APP

      c. UL RRC Message Tranfer

         - Section 9.2.4.4
         - Delivers RRC Message from UE to CU

      d. RRC Message Delivery Report

         - Section 9.2.4.5
         - Informs DU Manager on failed/successful delivery of a DL RRC message.
