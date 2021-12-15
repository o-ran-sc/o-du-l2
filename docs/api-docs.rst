.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. http://creativecommons.org/licenses/by/4.0



API-Docs
**********

This is the API-docs for e-release o-du/l2.

.. contents::
   :depth: 3
   :local:


Introduction
-----------------
This document lists the APIs implemented between various modules of ODU-High and their functionality.
These are in line with ORAN-WG8.AAD-v5.00.00, hereafter referred to as AAD Spec.

Note: DU APP module consist functionality related to F1 Handler and O1 Handler

API Functions
-------------
1. RLC-MAC Interface

   - AAD Spec : section 9.2.2

   A. RLC to MAC
   
      a. Data Transfer(DL) : Transfers DL data to MAC as per Section 9.2.2.1 of the AAD Spec

      b. Buffer Status Reporting(DL) : Reports DL data volume pending in the RLC entity, as per Section 9.2.2.4 of the
         AAD Spec

   B. MAC to RLC

      a. Data Transfer(UL) : Transfers UL data to RLC as per Section 9.2.2.2 of the AAD Spec

      b. Schedule Result Reporting (DL) : Reports DL schedule result to RLC as per Section 9.2.2.3 of the AAD Spec

2. MAC-Scheduler Interface

   - AAD Spec : section 9.2.3

   A. MAC to Scheduler

      a. Air Interface Time : Sends timing information over air interface in terms of SFN and slo, as per Section
         9.2.3.1.1 of the AAD Spec

      b. Cell Configuration Request : MAC provides SCH the cell configuration as per Section 9.2.3.2.1 of the AAD Spec

      c. Add UE Configuration Request : MAC provides SCH the configuration/capabilities of a UE as per Section 9.2.3.2.5 of the AAD Spec

      d. Modify UE Reconfiguration Request : MAC provides SCH the re-configuration of UE, also used to
         add/release/modify existing bearers, as per Section 9.2.3.2.6 of the AAD Spec

      e. RACH Indication : MAC receives the RACH Indication and share the contents with SCH as per Section 9.2.3.2.12 of the AAD Spec

      f. DL RLC Buffer Status Information : Reports data volume per LC when it is received at MAC as per Section
         9.2.3.2.16 of the AAD Spec

      g. UL Buffer Status Report Indication : On reception and decoding of received BSR from the UE, MAC provides SCH
         buffer status report for UL scheduling, as per Section 9.2.3.2.18 of the AAD Spec

      h. UE Delete Request : MAC receives UE delete request and forwards the request to SCH as per Section 9.2.3.2.7
         of the AAD spec.

      i. Cell Delete Request :  MAC receives UE delete request and forwards the request to SCH as per Section 9.2.3.2.2
         of the AAD spec.

      j. Slice Configuration Request : MAC receives Slice Configuration request and forwards the request to SCH as per Section
         9.2.3.2.3 of the AAD Spec
	 
      k. Slice Reconfiguration Request : MAC receives Slice Reconfiguration request and forwards the request to SCH as per 
         Section 9.2.3.2.4 of the AAD Spec

   B. Scheduler to MAC

      a. Cell Configuration Response - Response to cell configuration request from MAC, as per Section 9.2.3.3.1 of the AAD Spec

      b. UE Reconfiguration Response - Response to UE reconfiguration request from MAC, as per Section 9.2.3.3.6 of the AAD Spec

      c. DL Scheduling Information - Provides scheduling information for a given slot for downlink data, as per Section
         9.2.3.3.8 of the AAD spec.
    
      d. UL Scheduling Information - Provides scheduled information for MAC to form the UL_TTI.request towards O-DU Low,
         as per Section 9.2.3.3.9 of the AAD Spec.

      e. RAR Information - Informs MAC of uplink scheduling and Msg3 scheduling information, as per Section 9.2.3.3.10 of the AAD Spec.

      f. Downlink Control Channel Information - Provides DCI scheduling information for a given slot on PDCCH, as per
         Section 9.2.3.3.11 of the AAD Spec

      g. Downlink Broadcast Allocation - Provides DCI and DL data scheduling information for SIB1 for a given slot for
         Broadcast Channel, as per Section 9.2.3.3.12 of the AAD Spec.

      h. UE Delete Response - Response to UE delete request from MAC, as per Section 9.2.3.3.7

      i. Cell Delete Response - Response to Cell delete request from MAC, as per Section 9.2.3.3.2

      j. Slice Configuration Response : Response to Slice Configuration request from MAC, as per Section 9.2.3.3.3 of the AAD Spec
	 
      k. Slice Reconfiguration Response : Response to Slice Reconfiguration request from MAC, as per Section 9.2.3.3.4 of the AAD Spec


3. DU APP - MAC Interface

   - AAD Spec : section 9.2.4

   A. DU APP to MAC
   
      a. Cell Start - Commands MAC to start cell at L1 and to start broadcasting system information, as per Section
         9.2.4.1 of the AAD Spec.

      b. UE Create Request - Adds UE information at MAC as per Section 9.2.4.3 of the AAD Spec.

      c. UE Reconfiguration Request - Reconfigure UE information for a UE already added at MAC, as per Section 9.2.4.5 of the AAD Spec.

      d. DL CCCH Indication - Sends DL CCCH Message to MAC to be sent to UE, as per Section 9.2.4.16 of the AAD Spec.

      e. Cell Stop - Commands MAC to stop cell at L1, as per Section 9.2.4.2 of the AAD spec.

      f. UE Delete Request - Deletes UE information at MAC as per Section 9.2.4.7  of the AAD Spec.
      

   B. MAC to DU APP

      a. UE Create Response - Response to UE Create Request from DU Manager as per Section 9.2.4.4 of the AAD Spec.

      b. UE Reconfiguration Response - Response to UE Reconfiguration Request from DU APP as per Section 9.2.4.6 of the
         AAD Spec.

      c. UL-CCCH Indication - Sends the UL CCCH Message received from UE to DU APP as per Section 9.2.4.15 of AAD Spec.

      d. UE Delete Response - Response to UE Delete Request from DU Manager as per Section 9.2.4.8 of the AAD Spec.
      
4. DU APP - RLC Interface
   
   - AAD Spec : section 9.2.5

   A. DU APP to RLC

      a. UE Create - Adds UE associated RLC entity and related information as per Section 9.2.5.1 of the AAD Spec.

      b. UE Reconfiguration - Reconfigures the UE associated RLC entity previously added at RLC, as per Section 9.2.5.3 of the AAD Spec.

      c. DL RRC Message Transfer - Requests RLC to deliver RRC message from CU to the UE, as per Section 9.2.5.7 of the AAD Spec.

      d. UE Delete Request - Deletes UE associated RLC entity and related information as per Section 9.2.5.5 of the  AAD Spec.

   B. RLC to DU APP

      a. UE Create Response - Response to UE Create Request from DU APP, as per Section 9.2.5.3

      b. UE Reconfiguration Response - Response to UE Recofiguration Request from DU APP, as per Section 9.2.5.4

      c. UL RRC Message Tranfer - Delivers RRC Message from UE to CU as per Section 9.2.5.8 of the AAD spec.

      d. RRC Message Delivery Report - Informs DU Manager on failed/successful delivery of a DL RRC message to UE, as
         per Section 9.2.5.9 of the AAD Spec.

      e. UE Delete Response - Response to UE Delete Request from DU APP, as per section 9.2.5.6

5. SMO-OAM - DU APP

   - AAD Spec: Section 9.2.1

   A. SMO-OAM to DU APP

      a. Cell Configuration - Sends Basic Cell Configurations to DU APP as per Section 9.2.1.1
      
      b. Slice Configuration - Sends Slice Configurations to DU APP as per Section 9.2.1.2

Additional APIs
----------------

1. DU APP - MAC Interface

   A. DU APP to MAC
     
      a. Cell Configuration Request - Configures cell information at MAC.

      b. Cell Delete Request - Deletes cell information at MAC.
      
      c. Slice Configuration Request : Triggers Slice Configuration request once received from O1 towards MAC.
	 
      d. Slice Reconfiguration Request : Triggers Slice Reconfiguration request once received from O1 towards MAC. 

   B. MAC to DU APP

      a. Cell Configuration Response - Response to Cell Configuration Request from DU APP

      b. Cell Delete Response - Response to Cell Delet request from DU APP
      
      c. Slice Configuration Respone :  Response to Slice Configuration request from DU Manager.
	 
      d. Slice Reconfiguration Request : Response to Slice Reconfiguration request from DU Manager.

2. DU APP - RLC Interface

   A. RLC to DU APP

      a. DL RRC Message Response - Informs DU APP if a DL RRC Message was successfuly processed at RLC and sent to MAC.

      b. Slice Performance Metrics - Sends PM to DU APP every 60sec for every Slice configured.


3. DU APP - SMO-OAM

   a. Bring cell up - Informs DU to bring a specific cell up

   b. Bring cell down - Informs DU to bring specific cell up

   c. Set the cell operational state - Sets the cell state to ACTIVE/INACTIVE

   d. Raise a cell alarm - Raise an alarm when the cell is ACTIVE/INACTIVE

   e. Slice PM - DU APP sends PM for all Slices in 60sec duration.

