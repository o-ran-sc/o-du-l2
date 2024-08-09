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

/*Reference: SCF225_5G_NFAPI_SPECIFICATION, v225.2.1, Issue Date: 23 Nov 2021*/

#ifndef _NFAPI_VNF_FSM_H_
#define _NFAPI_VNF_FSM_H_

typedef enum {
   PNF_STATE_IDLE = 0,
   PNF_STATE_CONFIGURED,
   PNF_STATE_RUNNING,
   PNF_MAX_STATE
}NfapiPnfState;

typedef enum {
   PNF_READY_IND = 0,
   PNF_PARAM_REQ,
   PNF_PARAM_RESP,
   PNF_CONFIG_REQ,
   PNF_CONFIG_RESP,
   PNF_START_REQ,
   PNF_START_RESP,
   PNF_STOP_REQ,
   PNF_STOP_RESP,
   PNF_MAX_EVENT
}NfapiPnfEvent;

typedef struct nfapiTransportInfo
{
   uint32_t         assocId;
   uint32_t         srcIpv4Address;   /*Src :: PNF Address Details*/
   uint16_t         srcIpv4Port;          
   uint32_t         destIpv4Address;  /*Dest :: VNF Address Details*/
   uint16_t         destIpv4Port;
   CmInetFd         sockFd;           /* Socket file descriptor */
   CmInetAddr       srcIpNetAddr;    /* Refers to the Source(VNF) Addr in CmInetNet Addr format */
   CmInetAddr       destIpNetAddr;    /* Refers to the destination(PNF) Addr in CmInetNet Addr format */
   //CmInetNetAddrLst addrLst;      /* Refers to the destinaiton Addr Lst in CmInetNetAddrLst format */
   
   /*TODO: Add the IPV6 support*/

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
   uint16_t         max_phys;
   NfapiTimingInfo  vnfTimingCfg;
}NfapiPnfConfig;

typedef struct nfapiFrameInfo
{
    uint16_t       hSfn;    /*Hyper System Frame Number*/
    uint16_t       sfn;     /*System Frame Number*/
    uint8_t        slot;    /*Sub Frame Number*/
}NfapiFrameInfo;

typedef struct nfapiSyncInfo
{
   uint8_t        phyId;
   NfapiFrameInfo frameInfo; 
   bool           inSync;
   uint32_t       prev_t1;
   uint32_t       prev_t2;
   uint32_t       prev_t3;
   int32_t        delta_sfn_slot;
   struct nfapiSyncInfo *next;
}NfapiSyncInfo;

typedef struct nfapiP7VnfInfo
{
   uint32_t      t_ref_ns;   /*Reference Time of SFN 0/0 in nanosec*/
   NfapiSyncInfo p7SyncInfo;
}NfapiP7VnfInfo;

typedef struct nfapiP5Info
{
   uint8_t seqNum;
}NfapiP5Info;

typedef struct nfapiVnfDb
{
   bool                pnfToRestart;
   bool                pnfToReconfigure;
   NfapiPnfState       pnfStateAtVnf;
   NfapiPnfEvent       pnfEvent;
   NfapiP5Info         p5Info;
   NfapiTransportInfo  p7TransInfo;
   NfapiPnfConfig      pnfConfig;
   NfapiP7VnfInfo      vnfP7Info;
   uint8_t             numerology;
   uint16_t            cellId;
}NfapiVnfDb;

/* Global variables */
extern NfapiVnfDb vnfDb;

void nFapiVnfInit();
typedef uint8_t (*nFapiVnfFsmHdlr)(nFapi_p5_hdr *, nFapi_msg_header *, void *);
void sendEventToNfapiVnfFsm(NfapiPnfEvent msgType, nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);

/*NFAPI Msg Handler Functions*/
uint8_t nfapi_vnf_procPnfReadyIndEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfParamReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfParamRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfConfigReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfConfigRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfStartReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfStartRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfStopReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procPnfStopRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);
uint8_t nfapi_vnf_procInvalidEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg);

/*NFAPI Misc procedure*/
uint8_t TriggerPnfRestartProcedure();
uint8_t TriggerPnfReconfigureProcedure();
#endif
