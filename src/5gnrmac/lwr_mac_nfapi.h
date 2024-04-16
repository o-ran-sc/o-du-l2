 /*******************************************************************************
 ################################################################################
 #   Copyright (c) [2017-2019] [Radisys]                                        #
 #                                                                              #
 #   Licensed under the Apache License, Version 2.0 (the "License");            #
 #   you may not use this file except in compliance with the License.           #
 #   You may obtain a copy of the License at                                    #
 #                                                                              #
 #       http://www.apache.org/licenses/LICENSE-2.0                             #
 #                                                                              #
 #   Unless required by applicable law or agreed to in writing, software        #
 #   distributed under the License is distributed on an "AS IS" BASIS,          #
 #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
 #   See the License for the specific language governing permissions and        #
 #   limitations under the License.                                             #
 ################################################################################
 *******************************************************************************/

 #ifndef _LWR_MAC_NFAPI_H_
 #define _LWR_MAC_NFAPI_H_

#include "nfapi_interface.h"

typedef struct nfapiTransportInfo
{
   uint32_t         assocId;
   uint8_t          srcIpv4Address[4];   /*Src :: PNF Address Details*/
   uint16_t         srcIpv4Port;          
   uint8_t          destIpv4Address[4];  /*Dest :: VNF Address Details*/
   uint16_t         destIpv4Port;
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetNetAddrLst addrLst;      /* Refers to the destinaiton Addr Lst in CmInetNetAddrLst format */
   CmInetNetAddr    ipNetAddr;    /* Refers to the destination Addr in CmInetNet Addr format */
   
   /*TODO: later also add the IPV6 support*/

}NfapiTransportInfo;

typedef struct nfapiTimingInfo
{
   uint32_t  dl_tti_offset;
   uint32_t  ul_tti_offset;
   uint32_t  ul_dci_offset;
   uint32_t  tx_data_offset;
   uint16_t  timingWindow;
   uint8_t   timingInfoMode;

}NfapiTimingInfo;

typedef struct nfapiPnfConfig
{
   nFapi_pnf_state  pnfState;
   uint16_t         max_phys;
   NfapiTimingInfo  vnfTimingCfg;
}NfapiPnfConfig;

typedef struct nfapiSyncInfo
{
    uint8_t   phyId;
    uint8_t   inSync;
    uint32_t  prev_t1;
    uint32_t  prev_t2;
    uint32_t  prev_t3;
    uint8_t       sfn;
    uint8_t       slot;
    NfapiSyncInfo *next;
}NfapiSyncInfo;

typedef struct nfapiP7VnfInfo
{
   uint32_t      sfnStartTime;
   uint32_t      slotStartTime;
   NfapiSyncInfo *p7SyncInfo;
}NfapiP7VnfInfo;

typedef struct nfapiVnfDb
{
   NfapiTransportInfo  p5TransInfo;
   NfapiTransportInfo  p7TransInfo;
   NfapiPnfConfig      pnfConfig;
   NfapiSyncInfo       vnfSynchInfo;
   NfapiP7VnfInfo      vnfP7Info;   
}NfapiVnfDb;

#endif
