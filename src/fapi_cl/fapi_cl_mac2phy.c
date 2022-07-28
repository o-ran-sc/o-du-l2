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

/* This file converts FAPI messages sent from MAC to UE simulator/PHY */

#include "common_def.h"
#include "fapi_cl.h"
#include "fapi_cl_utils.h"
#include "fapi_interface_ue_sim.h"
#include "fapi_interface.h"
#include "fapi_vendor_extension.h"

/*******************************************************************
 *
 * @brief Converts CONFIG.Request
 *
 * @details
 *
 *    Function :  convertConfigRequest
 *
 *    Functionality:
 *       - Converts CONFIG.request from MAC's FAPI version to
 *         UE Simulator's FAPI version.
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/
void convertConfigRequest(uint8_t handle, void *msg, uint16_t msgLen)
{
   uint8_t idx = 0;
   p_fapi_api_queue_elem_t configReqElem = (p_fapi_api_queue_elem_t)msg;
   fapi_config_req_t *macConfigReq = (fapi_config_req_t *)(configReqElem +1);
   fapi_config_req_t_ue_sim ueSimConfigReq;

   DU_LOG("\nDEBUG  -->  FAPI_CL: Received CONFIG.request from MAC");

   memset(&ueSimConfigReq, 0, sizeof(fapi_config_req_t_ue_sim));
   ueSimConfigReq.header.numberOfMessagesIncluded = 1;
   ueSimConfigReq.header.handle = handle;
   ueSimConfigReq.header.msg_details.message_type_id = macConfigReq->header.msg_id;
   ueSimConfigReq.header.msg_details.length = sizeof(fapi_config_req_t_ue_sim) - sizeof(fapi_msg_header_t_ue_sim);
   ueSimConfigReq.number_of_tlvs = macConfigReq->number_of_tlvs;

   /* Copying all TLVs
    * Issues:
    * a. DL Frequency value gets truncated when copied to a uint16_t TLV
    * b. UL Frequency value gets truncated when copied to a uint16_t TLV
    * c. UE simulator FAPI interface file does not support DMRS_Type_A_Pos tag
    */
   for(idx = 0; idx < ueSimConfigReq.number_of_tlvs; idx++)
   {
      ueSimConfigReq.tlvs[idx].tl.tag = macConfigReq->tlvs[idx].tl.tag;
      ueSimConfigReq.tlvs[idx].tl.length = macConfigReq->tlvs[idx].tl.length;
      ueSimConfigReq.tlvs[idx].value = macConfigReq->tlvs[idx].value;
   }

   FAPI_CL_FREE(msg, msgLen);

   sendMsgToUeSim(EVT_CNTL_REQUEST, (Data *)&ueSimConfigReq, sizeof(fapi_config_req_t_ue_sim));
}

/*******************************************************************
 *
 * @brief Receives message from MAC and call appropriate handler
 *
 * @details
 *
 *    Function :  fapiClProcessFapiMsg
 *
 *    Functionality:
 *       - Receives message from MAC and calls appropriate handler
 *
 * @params[in] Message pointer
 *
 * @return void
 *
 * ****************************************************************/

void fapiClProcessFapiMsg(void *msg)
{
   uint8_t handle = 0;
   uint16_t msgLen =0;
   p_fapi_api_queue_elem_t currMsg = NULLP;
   p_fapi_api_queue_elem_t nextMsg = NULLP;

   currMsg = (p_fapi_api_queue_elem_t)msg;
   while(currMsg)
   {   
      nextMsg = currMsg->p_next;
      msgLen = currMsg->msg_len + sizeof(fapi_api_queue_elem_t);
      if(currMsg->msg_type == FAPI_VENDOR_MSG_HEADER_IND)
      {
         handle = ((fapi_msg_header_t *)(currMsg +1))->handle;
         FAPI_CL_FREE(currMsg, msgLen);
      }
      else if(currMsg->msg_type == FAPI_VENDOR_MESSAGE)
      {
         FAPI_CL_FREE(currMsg, msgLen);   
      }   
      else
      {
         switch(currMsg->msg_type)
         {
            case FAPI_CONFIG_REQUEST:
               convertConfigRequest(handle, currMsg, msgLen);
         }
      }
      currMsg = nextMsg;
   } 
}

/**********************************************************************
  End of file
 **********************************************************************/
