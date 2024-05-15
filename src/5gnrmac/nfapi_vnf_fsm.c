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

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lwr_mac.h"
#include "nfapi_vnf_fsm.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "lwr_mac_utils.h"
#include "lwr_mac_sctp_inf.h"
#include "mac_utils.h"

/*******************************************************************
 *
 * @brief Initalization of VNF Database
 *
 * @details
 *
 *    Function : nFapiVnfInit 
 *
 * @params[in]
 * @return void
 *
 * ****************************************************************/

void nFapiVnfInit()
{
   memset(&vnfDb, 0, sizeof(NfapiVnfDb));
   vnfDb.pnfStateAtVnf = PNF_STATE_IDLE;
   vnfDb.pnfEvent      = 0;
}



/*******************************************************************
 *
 * @brief Processes NFAPI PNF_READY_IND from PNF
 *
 * @details
 *
 *    Function : nfapi_vnf_procPnfReadyIndEvt
 *
 *    Functionality:
 *         - Processes PNF_READY_IND(Ref: SCF 225, Sec 3.1.0)
 *         - Starts the PNF Initalization(Ref: SCF 225, Sec 2.1.1.1)
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapi_vnf_procPnfReadyIndEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   uint32_t version = 0;

   DU_LOG("\nINFO   -->  NFAPI_VNF: Received EVENT[%d] at STATE[%d]",\
         vnfDb.pnfEvent, vnfDb.pnfStateAtVnf);

   CMCHKPK(oduUnpackUInt32, &(version), msg);
   
   DU_LOG("\nINFO   --> NFAPI_VNF: PNF_READY_IND version:%d",version);

   sendEventToNfapiVnfFsm(PNF_PARAM_REQ, NULLP, NULLP, NULLP);
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes NFAPI PNF_PARAM_REQ from PNF
 *
 * @details
 *
 *    Function : nfapi_vnf_procPnfParamReqEvt
 *
 *    Functionality:
 *         - Builds and Sends PNF_PARAM_REQ(Ref: SCF 225, Sec 3.1.1)
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapi_vnf_procPnfParamReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   Buffer *mBuf = NULLP;
   Pst pst;
   
   if (ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in packPnfParamReq");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, NFAPI_P5_PHY_ID, TAG_NFAPI_PNF_PARAM_REQ, 0);
   vnfDb.p5Info.seqNum++;
   FILL_PST_LWR_MAC_TO_DUAPP(pst, EVENT_PNF_DATA);
   return ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Processes NFAPI PNF_PARAM_RESP from PNF
 *
 * @details
 *
 *    Function : nfapi_vnf_procPnfParamRespEvt
 *
 *    Functionality:
 *         - Processes PNF_PARAM_RESP(Ref: SCF 225, Sec 3.1.2)
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapi_vnf_procPnfParamRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   uint16_t tag=0, len=0;
   uint8_t errorCode = 1,idx=0,numtlv=0;
   nFapi_pnf_param_general paramGeneral;
   
   DU_LOG("\nINFO   -->  NFAPI_VNF: Received EVENT[%d] at STATE[%d]",vnfDb.pnfEvent, vnfDb.pnfStateAtVnf);
   CMCHKPK(oduUnpackUInt8, &(errorCode), msg);
   if(errorCode != NFAPI_MSG_OK)
   {
      DU_LOG("\nERROR   -->  NFAPI_VNF: Param response error code is not okay, errCode:%d", errorCode);
      return RFAILED;
   }

   CMCHKPK(oduUnpackUInt8, &(numtlv), msg);
   CMCHKPK(oduUnpackUInt16, &(tag), msg);
   CMCHKPK(oduUnpackUInt16, &(len), msg);
   CMCHKPK(oduUnpackUInt8, &paramGeneral.nFapi_sync_mode, msg);
   CMCHKPK(oduUnpackUInt8, &paramGeneral.loc_mode, msg);
   CMCHKPK(oduUnpackUInt16, &paramGeneral.max_num_phy, msg);
   for(idx =0; idx < 3; idx++)
   {
      CMCHKPK(oduUnpackUInt8, &paramGeneral.oui[idx], msg);
   }
   CMCHKPK(oduUnpackUInt16, &paramGeneral.numRfInstances, msg);
   CMCHKPK(oduUnpackUInt16, &paramGeneral.numDfeInstances, msg);
   
   DU_LOG("\nINFO   --> NFAPI_VNF: PNF_PARAM_RESP errCode:%d, numtlv:%d",errorCode,numtlv);
   sendEventToNfapiVnfFsm(PNF_CONFIG_REQ, NULLP, NULLP, NULLP);
   return ROK;
}

/*******************************************************************
 *
 * @brief Processes NFAPI PNF_CONFIG_REQ from PNF
 *
 * @details
 *
 *    Function : nfapi_vnf_procPnfConfigReqEvt
 *
 *    Functionality:
 *         - Builds and Sends PNF_CONFIG_REQ(Ref: SCF 225, Sec 3.1.3)
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapi_vnf_procPnfConfigReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   Buffer *mBuf = NULLP;
   Pst pst;

   if (ODU_GET_MSG_BUF(MAC_MEM_REGION, MAC_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF : Memory allocation failed in packPnfConfigReq");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, NFAPI_P5_PHY_ID, TAG_NFAPI_PNF_CONFIG_REQ, 0);
   CMCHKPK(oduPackPostUInt8, 0, mBuf);
   
   DU_LOG("\nINFO   -->  NFAPI_VNF: Sending config request");
   FILL_PST_LWR_MAC_TO_DUAPP(pst, EVENT_PNF_DATA);
   return ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Processes NFAPI PNF_CONFIG_RSP from PNF
 *
 * @details
 *
 *    Function : nfapi_vnf_procPnfConfigRespEvt
 *
 *    Functionality:
 *         - Processes PNF_CONFIG_RSP(Ref: SCF 225, Sec 3.1.4)
 *         - Starts the PNF Initalization(Ref: SCF 225, Sec 2.1.1.1)
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t nfapi_vnf_procPnfConfigRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   uint8_t errCode = 0;
   DU_LOG("\nINFO   -->  NFAPI_VNF: Received EVENT[%d] at STATE[%d]",\
         vnfDb.pnfEvent, vnfDb.pnfStateAtVnf);

   CMCHKPK(oduUnpackUInt8, &(errCode), msg);
   
   if(errCode == NFAPI_MSG_OK)
   {
      vnfDb.pnfStateAtVnf=PNF_STATE_CONFIGURED;
      DU_LOG("\nINFO   -->  NFAPI_VNF: PNF STATE[%d]",vnfDb.pnfStateAtVnf);
   }
   else
   {
      DU_LOG("\nERROR   -->  NFAPI_VNF: Config response error code is not okay, errCode:%d", errCode);
      return RFAILED;
   }
   return ROK;
}

uint8_t nfapi_vnf_procPnfStartReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   return ROK;
}

uint8_t nfapi_vnf_procPnfStartRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   return ROK;
}

uint8_t nfapi_vnf_procPnfStopReqEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   return ROK;
}

uint8_t nfapi_vnf_procPnfStopRespEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   return ROK;
}

/*******************************************************************
 *
 * @brief Handles Invalid Request Event
 *
 * @details
 *
 *    Function : nfapi_vnf_procInvalidEvt
 *
 *    Functionality:
 *         - Displays the PNF state when the invalid event occurs
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t nfapi_vnf_procInvalidEvt(nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   DU_LOG("\nINFO   -->  NFAPI_VNF: Invalid Msg received EVENT[%d] at STATE[%d]",\
             vnfDb.pnfEvent, vnfDb.pnfStateAtVnf);
   return ROK;
}

/* ********************************************************
 * NFAPI VNF FINITE STATE MACHINE HANDLER
 * ********************************************************/
nFapiVnfFsmHdlr nFapiEvtHdlr[PNF_MAX_STATE][PNF_MAX_EVENT] =
{
   {
      /* PNF_STATE_IDLE */
      nfapi_vnf_procPnfReadyIndEvt,   /*PNF_READY_IND*/
      nfapi_vnf_procPnfParamReqEvt,   /*PNF_PARAM_REQ*/
      nfapi_vnf_procPnfParamRespEvt,  /*PNF_PARAM_RESP*/
      nfapi_vnf_procPnfConfigReqEvt,  /*PNF_CONFIG_REQ*/
      nfapi_vnf_procPnfConfigRespEvt, /*PNF_CONFIG_RESP*/
      nfapi_vnf_procInvalidEvt,       /*PNF_START_REQ*/
      nfapi_vnf_procPnfStartRespEvt,  /*PNF_START_RESP*/
      nfapi_vnf_procInvalidEvt,       /*PNF_STOP_REQ*/
      nfapi_vnf_procPnfStopRespEvt,   /*PNF_STOP_RESP*/
   },
   {
      /* PNF_STATE_CONFIGURED */
      nfapi_vnf_procInvalidEvt,       /*PNF_READY_IND*/
      nfapi_vnf_procPnfParamReqEvt,   /*PNF_PARAM_REQ*/
      nfapi_vnf_procPnfParamRespEvt,  /*PNF_PARAM_RESP*/
      nfapi_vnf_procPnfConfigReqEvt,  /*PNF_CONFIG_REQ*/
      nfapi_vnf_procPnfConfigRespEvt, /*PNF_CONFIG_RESP*/
      nfapi_vnf_procPnfStartReqEvt,   /*PNF_START_REQ*/
      nfapi_vnf_procPnfStartRespEvt,  /*PNF_START_RESP*/
      nfapi_vnf_procInvalidEvt,       /*PNF_STOP_REQ*/
      nfapi_vnf_procPnfStopRespEvt,   /*PNF_STOP_RESP*/
   },
   {
      /* PNF_STATE_RUNNING */
      nfapi_vnf_procInvalidEvt,       /*PNF_READY_IND*/
      nfapi_vnf_procInvalidEvt,       /*PNF_PARAM_REQ*/
      nfapi_vnf_procPnfParamRespEvt,  /*PNF_PARAM_RESP*/
      nfapi_vnf_procPnfConfigReqEvt,  /*PNF_CONFIG_REQ*/
      nfapi_vnf_procPnfConfigRespEvt, /*PNF_CONFIG_RESP*/
      nfapi_vnf_procInvalidEvt,       /*PNF_START_REQ*/
      nfapi_vnf_procPnfStartRespEvt,  /*PNF_START_RESP*/
      nfapi_vnf_procPnfStopReqEvt,    /*PNF_STOP_REQ*/
      nfapi_vnf_procPnfStopRespEvt,   /*PNF_STOP_RESP*/
   }
};

/*******************************************************************
 *
 * @brief Sends message to NFAPI_VNF Fsm Event Handler
 *
 * @details
 *
 *    Function : sendEventToNfapiVnfFsm
 *
 *    Functionality:
 *         -Sends message to NfapiVnf
 *
 * @params[in] Message Type
 *             Message Hdr and P5 Hdr
 *             Messaga Pointer
 *
 * @return void
 *
 ******************************************************************/
void sendEventToNfapiVnfFsm(NfapiPnfEvent msgType, nFapi_p5_hdr *p5Hdr, nFapi_msg_header *msgHdr, void *msg)
{
   uint8_t ret = ROK;
   vnfDb.pnfEvent = msgType;
   ret = nFapiEvtHdlr[vnfDb.pnfStateAtVnf][vnfDb.pnfEvent](p5Hdr, msgHdr, msg);
   if(ret == RFAILED)
   {
      DU_LOG("\nERROR  --> NFAPI_VNF: NFAPI_VNF FSM failed");
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
