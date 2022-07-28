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

/* This file converts FAPI messages sent from UE simulator/PHY to MAC */

#include "common_def.h"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"
#include "fapi_interface_ue_sim.h"
#include "fapi_interface.h"
#include "fapi_vendor_extension.h"
#include "lwr_mac_fapi_cl_inf.h"

void procPhyMessages(uint16_t msgType, uint32_t msgSize, void *msg);

/*******************************************************************
 *
 * @brief Converts CONFIG.Response
 *
 * @details
 *
 *    Function :  convertConfigResponse
 *
 *    Functionality:
 *       - Converts CONFIG.response from UE sim's FAPI version to
 *         Mac's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertConfigResponse(void *msg)
{
   uint16_t idx, numTlvs;
   fapi_config_resp_t *macCfgRsp = NULLP;
   fapi_config_resp_t_ue_sim *cfgRsp = (fapi_config_resp_t_ue_sim *)msg;

   DU_LOG("\nINFO  -->  FAPI_CL: Received CONFIG.response from UE simulator");

   FAPI_CL_ALLOC(macCfgRsp, sizeof(fapi_config_resp_t));
   if(!macCfgRsp)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Failed to allocate memory for config response");
      return;
   }
   macCfgRsp->header.msg_id = FAPI_CONFIG_RESPONSE;
   macCfgRsp->header.length = sizeof(fapi_config_resp_t) - sizeof(fapi_msg_t);
   macCfgRsp->error_code = cfgRsp->error_code;
   macCfgRsp->number_of_invalid_tlvs = cfgRsp->number_of_invalid_tlvs;
   macCfgRsp->number_of_inv_tlvs_idle_only = cfgRsp->number_of_inv_tlvs_idle_only;
   macCfgRsp->number_of_inv_tlvs_running_only = cfgRsp->number_of_inv_tlvs_running_only;
   macCfgRsp->number_of_missing_tlvs = cfgRsp->number_of_missing_tlvs;
   numTlvs = macCfgRsp->number_of_invalid_tlvs + macCfgRsp->number_of_inv_tlvs_idle_only + \
      macCfgRsp->number_of_inv_tlvs_running_only + macCfgRsp->number_of_missing_tlvs;
   for(idx = 0; idx < numTlvs; idx++)
   {
      macCfgRsp->tlvs[idx].tl.tag = cfgRsp->tlvs[idx].tl.tag;
      macCfgRsp->tlvs[idx].tl.length = cfgRsp->tlvs[idx].tl.length;
      macCfgRsp->tlvs[idx].value = cfgRsp->tlvs[idx].value;
   }
   procPhyMessages(macCfgRsp->header.msg_id, sizeof(fapi_config_resp_t), (void *)macCfgRsp);
   FAPI_CL_FREE(macCfgRsp, sizeof(fapi_config_resp_t));
}

/*******************************************************************
*
* @brief Builds and sends FAPI Slot indication to MAC
*
* @details
*
*    Function : buildAndSendSlotIndication
*
*    Functionality:
*       - Builds and sends FAPI Slot indication to MAC
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
uint8_t buildAndSendSlotIndication()
{
   Pst pst; 
   Buffer *mBuf;
   fapi_slot_ind_t *slotIndMsg;

   FAPI_CL_ALLOC_SHRABL_BUF(slotIndMsg, sizeof(fapi_slot_ind_t));
   if(!slotIndMsg)
   {
      DU_LOG("\nERROR  -->  FAPI_CL: Memory allocation failed for slot Indication Message");
      return RFAILED;
   }
   else 
   {
      memset(slotIndMsg, 0, sizeof(fapi_slot_ind_t));
      slotIndMsg->sfn = clCb.slotIndInfo.currSfn;
      slotIndMsg->slot = clCb.slotIndInfo.currSlot;
      slotIndMsg->header.msg_id = FAPI_SLOT_INDICATION;
      slotIndMsg->header.length = sizeof(fapi_slot_ind_t) - sizeof(fapi_msg_t);

#ifdef ODU_SLOT_IND_DEBUG_LOG
      DU_LOG("\n\nDEBUG  -->  FAPI_CL: Sending Slot Indication [%d : %d] to MAC", slotIndMsg->sfn, slotIndMsg->slot);
#endif

      /* increment for the next TTI */
      clCb.slotIndInfo.currSlot++;
      if(clCb.slotIndInfo.currSlot >= MAX_SLOTS)
      {
         clCb.slotIndInfo.currSlot = 0;
         clCb.slotIndInfo.currSfn++;
         if(clCb.slotIndInfo.currSfn >= MAX_SFN)
            clCb.slotIndInfo.currSfn = 0;
      }

      memset(&pst, 0, sizeof(Pst));
      FILL_PST_FAPI_CL_TO_LWR_MAC(pst, EVT_FAPI_CL_SLOT_IND);
      ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
      if(!mBuf)
      {
         DU_LOG("\nERROR  --> FAPI_CL: Failed to allocate memory for slot indication buffer");
         FAPI_CL_FREE_SHRABL_BUF(pst.region, pst.pool, slotIndMsg, sizeof(fapi_slot_ind_t));
         return RFAILED;
      }
      CMCHKPK(oduPackPointer, (PTR)slotIndMsg, mBuf);
      ODU_POST_TASK(&pst, mBuf);
   }
   return ROK;
}

/*******************************************************************
*
* @brief Generates ticks periodically
*
* @details
*
*    Function : generateTicks
*
*    Functionality:
*       - Generates ticks periodically
*
* @params[in] 
* @return NULL
*
* ****************************************************************/
void* generateTicks()
{
   float   milisec = 1;          /* 1ms */
   struct  timespec req = {0};
   uint8_t ratio = 2;

   clCb.slotIndInfo.currSfn = 0;
   clCb.slotIndInfo.currSlot = 0;
   clCb.slotIndInfo.slotIndicationStarted = true;
   req.tv_sec = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */
   req.tv_nsec = milisec * 1000000L * ratio;

   DU_LOG("\nINFO  -->  FAPI_CL: Starting to generate slot indications");

//   while(clCb.slotIndInfo.slotIndicationStarted)
   uint8_t counter = 0;
   while(counter <=180)
   {   
      clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL); 
      /* Send Slot indication indication to lower mac */
      if(buildAndSendSlotIndication() != ROK)
      {   
         DU_LOG("\nERROR  --> FAPI_CL: generateTicks(): Failed to build and send Slot Indication");
         break;
      }  
      counter++;
   }   
   DU_LOG("\nINFO  --> FAPI_CL: Slot indication stopped");
   return 0;
}

/*******************************************************************
*
* @brief Sends Start_slot_ind request to self
*
* @details
*
*    Function : sendStartSlotIndReq
*
*    Functionality:
*       - Sends Start_slot_ind request to self
*
* @params[in] 
* @return
*
* ****************************************************************/
void sendStartSlotIndReq()
{
   Pst pst;
   Buffer *mBuf = NULLP;

   DU_LOG("\nINFO  --> FAPI_CL: Sending start slot indication event to self");
   memset(&pst, 0, sizeof(Pst));
   FILL_PST_CL_TO_CL(pst, EVT_FAPI_CL_START_SLOT_IND);
   ODU_GET_MSG_BUF(pst.region, pst.pool, &mBuf);
   ODU_POST_TASK(&pst, mBuf);
}

/*******************************************************************
 *
 * @brief Process START.Response
 *
 * @details
 *
 *    Function :  processStartResponse
 *
 *    Functionality:
 *       - Calls the handler to start sending slot indication to MAC
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertStartResponse(void *msg)
{  
   DU_LOG("\nINFO  -->  FAPI_CL: Received START.response from UE simulator");
   sendStartSlotIndReq();
}

/*******************************************************************
 *
 * @brief Call appropriate handler for FAPI message from UE sim
 *
 * @details
 *
 *    Function :  fapiClProcessUeSim2MacFapiMsg
 *
 *    Functionality:
 *       - Calls appropriate handler based on message type for 
 *         message sent from UE simulator to MAC.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void fapiClProcessUeSim2MacFapiMsg(Data *msg)
{
   uint8_t numMsg = 0;
   fapi_msg_header_t_ue_sim *msgHdr = (fapi_msg_header_t_ue_sim *)msg;

   while(numMsg < msgHdr->numberOfMessagesIncluded)
   {
      switch(msgHdr->msg_details.message_type_id)
      {
         case FAPI_CONFIG_RESPONSE:
            convertConfigResponse(msg);      
            break;
         case FAPI_START_RESPONSE:
            convertStartResponse(msg);
            break;
         default:
            DU_LOG("\nERROR  -->  FAPI_CL: Invalid message type id [%d] in header", msgHdr->msg_details.message_type_id);
      }
      numMsg++;
   }
}

/**********************************************************************
  End of file
 **********************************************************************/
