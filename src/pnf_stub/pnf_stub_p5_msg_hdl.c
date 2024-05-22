
/*******************************************************************************
 * ################################################################################
 * #   Copyright (c) [2017-2019] [Radisys]
 * #
 * #
 * #
 * #   Licensed under the Apache License, Version 2.0 (the "License");
 * #
 * #   you may not use this file except in compliance with the License.
 * #
 * #   You may obtain a copy of the License at
 * #
 * #
 * #
 * #       http://www.apache.org/licenses/LICENSE-2.0
 * #
 * #
 * #
 * #   Unless required by applicable law or agreed to in writing, software
 * #
 * #   distributed under the License is distributed on an "AS IS" BASIS,
 * #
 * #   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * #
 * #   See the License for the specific language governing permissions and
 * #
 * #   limitations under the License.
 * #
 * ################################################################################
 * *******************************************************************************/

/* This functions contains main() for pnf simulator */
#include "common_def.h"
#include <unistd.h>
#include "pnf_stub_p5_msg_hdl.h"
#include "nfapi_interface.h"
#include "pnf_stub_sctp.h"
#include "pnf_stub_p7_udp.h"
#include "pnf_stub.h"

PnfGlobalCb pnfCb;
/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfReadyInd 
 *
 *
 * @Functionality: 
 *   Builds and Sends PNF_READY_IND(Ref: SCF 225, Sec 3.1.0)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfReadyInd()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_READY_IND, 0);
   CMCHKPK(oduPackPostUInt32, 123, mBuf); //version
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfParamResp 
 *
 * @Functionality: 
 *    Build Pnf Param rsp as per  5G nFAPI Specification 3.1.2 PNF_PARAM.response 
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendPnfParamResp()
{
   uint8_t ret = ROK,idx=0;
   Buffer *mBuf = NULLP;
   nFapi_pnf_param_general paramGeneral;

   memset(&paramGeneral, 0, sizeof(nFapi_pnf_param_general));
   paramGeneral.max_num_phy=1;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_PARAM_RESP, 0);
   CMCHKPK(oduPackPostUInt8, 0, mBuf); 
   CMCHKPK(oduPackPostUInt8, 1, mBuf); 
   CMCHKPK(oduPackPostUInt16, TAG_NFAPI_PNF_PARAM_GENERAL, mBuf);
   CMCHKPK(oduPackPostUInt16, sizeof(nFapi_pnf_param_general), mBuf);
   CMCHKPK(oduPackPostUInt8, paramGeneral.nFapi_sync_mode, mBuf); 
   CMCHKPK(oduPackPostUInt8, paramGeneral.loc_mode, mBuf);
   CMCHKPK(oduPackPostUInt16, paramGeneral.max_num_phy, mBuf); 
   for(idx =0; idx < 3; idx++)
   {
      CMCHKPK(oduPackPostUInt8, paramGeneral.oui[idx], mBuf); 
   }
   CMCHKPK(oduPackPostUInt16, paramGeneral.numRfInstances, mBuf); 
   CMCHKPK(oduPackPostUInt16, paramGeneral.numDfeInstances, mBuf); 

   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfConfigResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_CONFIG_RSP(Ref: SCF 225, Sec 3.1.4)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfConfigResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_CONFIG_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf config rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_CONFIG_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStartResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_START_RSP(Ref: SCF 225, Sec 3.1.6)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStartResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_START_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_RUNNING;
   
   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf start rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_START_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendPnfStopResp
 *
 *
 * @Functionality:
 *   Builds and Sends PNF_STOP_RSP(Ref: SCF 225, Sec 3.1.8)
 *
 *
 * *******************************************************************************/
uint8_t buildAndSendPnfStopResp()
{
   uint8_t ret = ROK;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO   --> NFAPI_PNF: Building PNF_STOP_RSP");
   pnfCb.pnfCfgParams.pnfState=PNF_CONFIGURED;

   if(ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf stop rsp");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, TAG_NFAPI_PNF_STOP_RESP, 0);
   CMCHKPK(oduPackPostUInt32, 0, mBuf); //Error Code
   ret = pnfP5SctpSend(mBuf);
   if(ret == RFAILED)
   {
      ODU_PUT_MSG_BUF(mBuf);
   }
   return ret;
}

/*********************************************************************************
 *
 * @Function Name: buildAndSendParamResp 
 *
 * @Functionality: 
 *    Build  Param rsp as per  5G nFAPI Specification 3.1.2 PARAM.response 
 *    And Send to VNF
 *
 * *******************************************************************************/

uint8_t buildAndSendParamResp()
{
   Buffer *mBuf = NULLP;

   if (ODU_GET_MSG_BUF(PNF_APP_MEM_REG, PNF_POOL, &mBuf) != ROK)
   {
      DU_LOG("\nERROR  --> NFAPI_PNF : Memory allocation failed in pnf_readyInd");
      return RFAILED;
   }
   nfapiFillP5Hdr(mBuf);
   nfapiFillMsgHdr(mBuf, 0, FAPI_PARAM_RESPONSE, 0);
   CMCHKPK(oduPackPostUInt8, 0, mBuf); 
   CMCHKPK(oduPackPostUInt8, 1, mBuf);
   //TODO->
   //Fill TLVs

   return ROK;
}

/*********************************************************************************
 *
 * @Function Name: p5MsgHandlerAtPnf
 *
 *
 * @Functionality: 
 *    Handles the P5 MEssages at PNF SIM
 *
 * @Params [IN]: Message Buffer received at SCTP NFAPI P5 Interface
 *
 * *******************************************************************************/
uint8_t  p5MsgHandlerAtPnf(Buffer *mBuf)
{
   nFapi_p5_hdr p5Hdr;
   nFapi_msg_header msgHdr;
   uint8_t ret = ROK;

   nFapiExtractP5Hdr(&p5Hdr, mBuf);
   nFapiExtractMsgHdr(&msgHdr, mBuf);

   switch(msgHdr.msg_id)
   {
      case TAG_NFAPI_PNF_PARAM_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_PARAM_REQ recevied.");
            ret = buildAndSendPnfParamResp();
            break;
         }
      case TAG_NFAPI_PNF_CONFIG_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_CONFIG_REQ recevied.");
            ret = buildAndSendPnfConfigResp();
            break;
         }
      case TAG_NFAPI_PNF_START_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_START_REQ recevied.");
            ret = buildAndSendPnfStartResp();
            break;
         }
      case TAG_NFAPI_PNF_STOP_REQ:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PNF_STOP_REQ recevied.");
            ret = buildAndSendPnfStopResp();
            break;
         }
      case FAPI_PARAM_REQUEST:
         {
            DU_LOG("\nINFO   --> NFAPI_PNF: PHY_PARAM_REQ recevied.");
            ret = buildAndSendParamResp();
            break;
         }
      default:
      {
         DU_LOG("\nERROR  --> NFAPI_PNF: Wrong MSGID of NFAPI P5 Message:%d",msgHdr.msg_id);
      }
   }

   if(ret == RFAILED)
   {
      return RFAILED;
   }
   return ret;
}

/**********************************************************************
End of file
**********************************************************************/
