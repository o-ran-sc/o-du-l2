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

/* This file contains E2AP message handler functions */
#include "common_def.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
#include "odu_common_codec.h"
#include "ric_stub_sctp.h"
#include "ric_stub.h"
#include "ric_e2ap_msg_hdl.h"
#include "GlobalE2node-gNB-ID.h"
#include "ProtocolIE-FieldE2.h"
#include "InitiatingMessageE2.h"
#include "SuccessfulOutcomeE2.h"
#include "UnsuccessfulOutcomeE2.h"
#include "E2AP-PDU.h"
#include "du_log.h"
#include "E2nodeComponentInterfaceF1.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "RANfunction-Name.h"
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-ReportStyle-Item.h"
#include "MeasurementInfo-Action-Item.h"
#include "MeasurementInfoItem.h"
#include "E2SM-KPM-ActionDefinition-Format1.h"
#include "E2SM-KPM-ActionDefinition.h"
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"
#include "E2SM-KPM-EventTriggerDefinition.h"


/*******************************************************************
 *
 * @brief Assigns new transaction id to RIC initiated procedure
 *
 * @details
 *
 *    Function : assignTransactionId
 *
 *    Functionality: Assigns new transaction id to a RIC initiated
 *       procedure
 *
 * @params[in] Region region
 *             Pool pool
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t assignTransactionId(DuDb *duDb)
{
   uint8_t currTransId = duDb->ricTransIdCounter;

   /* Update to next valid value */
   duDb->ricTransIdCounter++;
   if(duDb->ricTransIdCounter == MAX_NUM_TRANSACTION)
      duDb->ricTransIdCounter = 0;

   return currTransId;
}

/*******************************************************************
*
* @brief Sends E2 msg over SCTP
*
* @details
*
*    Function : SendE2APMsg
*
*    Functionality: Sends E2 msg over SCTP
*
* @params[in] Region region
*             Pool pool
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/

uint8_t SendE2APMsg(Region region, Pool pool, uint32_t duId)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);

         if(sctpSend(duId, mBuf) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : SCTP Send for E2  failed");
            ODU_PUT_MSG_BUF(mBuf);
            return RFAILED;
         }
      }
      else
      {
         DU_LOG("\nERROR  -->  E2AP : ODU_ADD_POST_MSG_MULT failed");
         ODU_PUT_MSG_BUF(mBuf);
         return RFAILED;
      }
      ODU_PUT_MSG_BUF(mBuf);
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to allocate memory");
      return RFAILED;
   }

   return ROK;
} /* SendE2APMsg */

/*******************************************************************
 *
 * @brief Fetches RAN Function DB
 *
 * @details
 *
 *    Function : fetchRanFuncFromRanFuncId
 *
 *    Functionality: Fetches RAN function DB from E2AP DB using
 *       RAN function ID
 *
 * @params[in] RAN Function ID
 * @return RAN Function DB
 *         NULL, in case of failure
 *
 * ****************************************************************/
RanFunction *fetchRanFuncFromRanFuncId(DuDb *duDb, uint16_t ranFuncId)
{
   RanFunction *ranFuncDb = NULLP;

   /* Fetch RAN Function DB */
   if(duDb->ranFunction[ranFuncId-1].id == ranFuncId)
   {
      ranFuncDb = &duDb->ranFunction[ranFuncId-1];
   }
   else
   {
      DU_LOG("\nERROR  -->  DU_APP : fetchRanFuncFromRanFuncId: Invalid RAN Function ID[%d]", ranFuncId);
   }

   return ranFuncDb;
}

/*******************************************************************
 *
 * @brief Fetch subscripton DB
 *
 * @details
 *
 *    Function : fetchSubsInfoFromRicReqId
 *
 *    Functionality: Fetches subscription DB from RAN Function DB
 *       using RIC Request ID
 *
 * @params[in] RIC Request ID
 *             RAN Function DB
 *             Pointer to RIC Subscription node to be searched
 * @return RIC Subscription from RAN Function's subcription list
 *         NULL, in case of failure
 *
 * ****************************************************************/
RicSubscription *fetchSubsInfoFromRicReqId(RicRequestId ricReqId, RanFunction *ranFuncDb)
{
   uint8_t subsIdx = 0;
   RicSubscription *ricSubscriptionInfo = NULLP;

   for(subsIdx = 0; subsIdx < ranFuncDb->numOfSubscription; subsIdx++)
   {
      if((ranFuncDb->subscriptionList[subsIdx].requestId.requestorId == ricReqId.requestorId) &&
            (ranFuncDb->subscriptionList[subsIdx].requestId.instanceId == ricReqId.instanceId))
      {
         ricSubscriptionInfo = &ranFuncDb->subscriptionList[subsIdx];
         break;
      }
   }
   return ricSubscriptionInfo;
}

/*******************************************************************
 *
 * @brief Fetch Action details
 *
 * @details
 *
 *    Function : fetchActionInfoFromActionId
 *
 *    Functionality: Fetch action details from RIC subscription DB
 *       using action ID
 *
 * @params[in] Action ID
 *             RIC Subscription DB
 * @return Action Info DB
 *         NULL, in case of failure
 *
 * ****************************************************************/
ActionInfo *fetchActionInfoFromActionId(uint8_t actionId, RicSubscription *ricSubscriptionInfo)
{
   ActionInfo *actionInfoDb = NULLP;
   if(ricSubscriptionInfo->actionSequence[actionId].actionId == actionId)
   {
      actionInfoDb = &ricSubscriptionInfo->actionSequence[actionId];
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : fetchActionInfoFromActionId: Action Id [%d] not found in \
         subscription info [Requestor id : %d] [Instance Id : %d]", actionId,\
         ricSubscriptionInfo->requestId.requestorId, ricSubscriptionInfo->requestId.instanceId);

   }
   return actionInfoDb;
}

/*******************************************************************
 *
 * @brief deallocate memory allocated in E2 Node Config Update Failure
 *
 * @details
 *
 *    Function : FreeE2ConfigUpdateFail
 *
 *    Functionality: deallocate memory allocated in E2 Node Config Update Failure
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/

void FreeE2ConfigUpdateFail(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0;
   E2nodeConfigurationUpdateFailure_t *e2NodeCfgUpdFail=NULL;

   if(e2apMsg)
   {
      if(e2apMsg->choice.unsuccessfulOutcome)
      {
         e2NodeCfgUpdFail = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2nodeConfigurationUpdateFailure;
         if(e2NodeCfgUpdFail->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<e2NodeCfgUpdFail->protocolIEs.list.count; arrIdx++)
            {
               RIC_FREE(e2NodeCfgUpdFail->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdateFailure_IEs_t));
            }
            RIC_FREE(e2NodeCfgUpdFail->protocolIEs.list.array, e2NodeCfgUpdFail->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Node Config Update failure
 *
 * @details
 *
 *    Function : BuildAndSendE2NodeConfigUpdateFailure
 *
 *    Functionality:
 *         - Buld and send the E2 Node Config Update failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2NodeConfigUpdateFailure(uint32_t duId, uint8_t transId, uint8_t causeInfo, uint8_t causeReason)
{
   E2AP_PDU_t         *e2apMsg = NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx=0;
   uint8_t            elementCnt=0;
   bool  memAllocFailed = false;
   E2nodeConfigurationUpdateFailure_t *e2NodeCfgUpdateFail=NULL;

   DU_LOG("\nINFO   -->  E2AP : Building E2 Node Config Update failure\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_unsuccessfulOutcome;
      RIC_ALLOC(e2apMsg->choice.unsuccessfulOutcome , sizeof(struct UnsuccessfulOutcomeE2));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_E2nodeConfigurationUpdate;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcomeE2__value_PR_E2nodeConfigurationUpdateFailure;
      e2NodeCfgUpdateFail = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2nodeConfigurationUpdateFailure;

      elementCnt = 3;
      e2NodeCfgUpdateFail->protocolIEs.list.count = elementCnt;
      e2NodeCfgUpdateFail->protocolIEs.list.size  = elementCnt * sizeof(struct E2nodeConfigurationUpdateFailure_IEs *);

      RIC_ALLOC(e2NodeCfgUpdateFail->protocolIEs.list.array, e2NodeCfgUpdateFail->protocolIEs.list.size);
      if(e2NodeCfgUpdateFail->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2 node config update failure array failed");
         break;
      }

      for(arrIdx=0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx], sizeof(struct E2nodeConfigurationUpdateFailure_IEs));
         if(e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2 node config update failure IEs failed");
            memAllocFailed = true;
            break;
         }
      }

      if(memAllocFailed == true)
      {
          break;
      }

      /* Trans Id */
      arrIdx = 0;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_TransactionID;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.choice.TransactionID  = transId;

      arrIdx++;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_CauseE2;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.choice.CauseE2.present = causeInfo;
      if(causeInfo == CauseE2_PR_e2Node) 
         e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.choice.CauseE2.choice.e2Node = causeReason;
      else
         e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.choice.CauseE2.choice.misc = causeReason;

      arrIdx++;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TimeToWaitE2;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_ignore;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_TimeToWaitE2;
      e2NodeCfgUpdateFail->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2 = TimeToWaitE2_v5s;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 Node Config Update failure structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Node Config Update Failure\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Node Config Update Failure failed");
         break;
      }
      break;
   }

   FreeE2ConfigUpdateFail(e2apMsg);
   return ROK;
}

/*******************************************************************
 *
 * @brief process the E2 node configuration update
 *
 * @details
 *
 *    Function : ProcE2NodeConfigUpdate 
 *
 * Functionality: Process E2 node configuration update
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

void ProcE2NodeConfigUpdate(uint32_t duId, E2nodeConfigurationUpdate_t *e2NodeConfigUpdate)
{
   uint8_t ieIdx = 0;
   uint8_t transId = 0, e2NodeUpdateListIdx=0;
   E2nodeComponentConfigUpdate_List_t *e2NodeUpdateList=NULLP;
   E2nodeComponentConfigUpdate_ItemIEs_t *e2NodeUpdateItemIe=NULLP;
   E2nodeComponentConfigUpdate_Item_t *e2NodeUpdateItem =NULLP;

   if(e2NodeConfigUpdate)
   {
      if(e2NodeConfigUpdate->protocolIEs.list.array)
      {
         for(ieIdx=0; ieIdx < e2NodeConfigUpdate->protocolIEs.list.count; ieIdx++)
         {
            if(e2NodeConfigUpdate->protocolIEs.list.array[ieIdx])
            {
               switch(e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_IDE2_id_TransactionID:
                     transId = e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
                     break;
                  
                  default:
                  {
                     /*TODO - Other IEs will be handling in next gerrit*/
                     break;
                  }
               }
            }
         }
      }
   }
}

/*******************************************************************
 *
 * @brief Builds Global RIC Id Params
 *
 * @details
 *
 *    Function : BuildGlobalRicId
 *
 *    Functionality: Building the Plmn and ric id
 *
 * @params[in] GlobalRIC_ID_t *ricId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildGlobalRicId(GlobalRIC_ID_t *ricId)
{
   uint8_t unused = 4;
   uint8_t byteSize = 3;
   uint8_t ricVal= 1;
   if(ricId != NULLP)
   {
      ricId->pLMN_Identity.size = byteSize * sizeof(uint8_t);
      RIC_ALLOC(ricId->pLMN_Identity.buf,  ricId->pLMN_Identity.size);
      buildPlmnId(ricCb.ricCfgParams.plmn , ricId->pLMN_Identity.buf);
      /* fill ric Id */
      ricId->ric_ID.size = byteSize * sizeof(uint8_t);
      RIC_ALLOC(ricId->ric_ID.buf, ricId->ric_ID.size);
      fillBitString(&ricId->ric_ID, unused, byteSize, ricVal);
   }
   return ROK;   
}

/*******************************************************************
 *
 * @brief deallocate the memory allocated in E2SetupResponse
 *
 * @details
 *
 *    Function : FreeE2SetupRsp 
 *
 *    Functionality: deallocate the memory allocated in E2SetupResponse 
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/
void FreeE2SetupRsp(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0, e2NodeConfigIdx=0, ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAcceptedList=NULL;
   E2setupResponse_t  *e2SetupRsp=NULL;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItemIe=NULL;
   E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList=NULL;
   E2nodeComponentInterfaceF1_t *f1InterfaceInfo=NULL;
   
   if(e2apMsg)
   {
      if(e2apMsg->choice.successfulOutcome)
      {
         e2SetupRsp = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;
         if(e2SetupRsp->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<e2SetupRsp->protocolIEs.list.count; arrIdx++)
            {
               switch(e2SetupRsp->protocolIEs.list.array[arrIdx]->id)
               {
                  case ProtocolIE_IDE2_id_RANfunctionsAccepted:
                  {
                     ranFuncAcceptedList= &e2SetupRsp->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                     if(ranFuncAcceptedList->list.array)
                     {
                        for(ranFuncIdx=0;ranFuncIdx<ranFuncAcceptedList->list.count; ranFuncIdx++)
                        {
                           if(ranFuncAcceptedList->list.array[ranFuncIdx])
                           {
                              RIC_FREE(ranFuncAcceptedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                           }
                        }
                        RIC_FREE(ranFuncAcceptedList->list.array, ranFuncAcceptedList->list.size);
                     }
                     break;
                  }
                  case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
                  {
                     e2NodeConfigAdditionAckList =&e2SetupRsp->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List;
                     if(e2NodeConfigAdditionAckList->list.count)
                     {
                        for(e2NodeConfigIdx=0; e2NodeConfigIdx<e2NodeConfigAdditionAckList->list.count; e2NodeConfigIdx++)
                        {
                           e2NodeAddAckItemIe = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAdditionAckList->list.array[e2NodeConfigIdx];
                           if(e2NodeAddAckItemIe)
                           {
                              f1InterfaceInfo = e2NodeAddAckItemIe->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1; 
                              if(f1InterfaceInfo)
                              {
                                 RIC_FREE(f1InterfaceInfo->gNB_DU_ID.buf, f1InterfaceInfo->gNB_DU_ID.size);
                                 RIC_FREE(f1InterfaceInfo, sizeof(E2nodeComponentInterfaceF1_t));
                              }
                              RIC_FREE(e2NodeAddAckItemIe, sizeof(E2nodeComponentConfigAdditionAck_ItemIEs_t));
                           }
                        }
                        RIC_FREE(e2NodeConfigAdditionAckList->list.array, e2NodeConfigAdditionAckList->list.size);
                     }
                     break;
                  }
               }
               RIC_FREE(e2SetupRsp->protocolIEs.list.array[arrIdx], sizeof(E2setupResponseIEs_t)); 
            }
            RIC_FREE(e2SetupRsp->protocolIEs.list.array, e2SetupRsp->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build E2node Component config addition ack list
 *
 * @details
 *
 *    Function :  BuildE2nodeComponentConfigAdditionAck
 *
 *    Functionality: deallocate the memory allocated in E2SetupResponse 
 *
 * @params[in] E2nodeComponentConfigAdditionAck_List_t
 * *e2NodeConfigAdditionAckList
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2nodeComponentConfigAdditionAck(E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList, DuDb *duDb)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItem;
   
   e2NodeConfigAdditionAckList->list.count = 1;
   e2NodeConfigAdditionAckList->list.size = e2NodeConfigAdditionAckList->list.count * sizeof(E2nodeComponentConfigAdditionAck_ItemIEs_t*);
   RIC_ALLOC(e2NodeConfigAdditionAckList->list.array, e2NodeConfigAdditionAckList->list.size);
   if(e2NodeConfigAdditionAckList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigAdditionAck %d",__LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeConfigAdditionAckList->list.count; arrIdx++)
   {
      RIC_ALLOC(e2NodeConfigAdditionAckList->list.array[arrIdx], sizeof(E2nodeComponentConfigAdditionAck_ItemIEs_t));
      if(e2NodeConfigAdditionAckList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigAdditionAck %d",__LINE__);
         return RFAILED;
      }
   }
   e2NodeAddAckItem = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAdditionAckList->list.array[0];
   e2NodeAddAckItem->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck_Item;
   e2NodeAddAckItem->criticality = CriticalityE2_reject;
   e2NodeAddAckItem->value.present = E2nodeComponentConfigAdditionAck_ItemIEs__value_PR_E2nodeComponentConfigAdditionAck_Item;
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentInterfaceType = duDb->e2NodeComponent.interfaceType;

   /* >E2 Node Component ID */
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.present = E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1;
   RIC_ALLOC(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1,\
         sizeof(E2nodeComponentInterfaceF1_t));
   if(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1 == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigAdditionAck %d",__LINE__);
      return RFAILED;
   }
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size = sizeof(uint8_t);
   RIC_ALLOC(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf,\
         e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);

   if(e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf == NULLP)
   {
      DU_LOG("\nERROR  -->list.  E2AP: Memory allocation failed for BuildE2nodeComponentConfigAdditionAck %d",__LINE__);
      return RFAILED;
   }
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]  = duDb->e2NodeComponent.componentId;
   
   /* >E2 Node Component Configuration Acknowledge*/
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentConfigurationAck.updateOutcome = \
   E2nodeComponentConfigurationAck__updateOutcome_success;
   
   return ROK;  
}

/*******************************************************************
 *
 * @brief Build RAN function accepted list
 *
 * @details
 *
 *    Function : BuildRanFunctionAcceptedList
 *
 *    Functionality: Build RAN function accepted list 
 *     ->For ProcedureCodeE2_id_E2setup or ProcedureCodeE2_id_RICserviceQuery  
 *     we add all the RAN Function list which is present in RIC database.
 *     ->For any other procedures, we just fill the RAN functions whose ID 
 *     is present in the recvList
 *
 * @params[in] 
 *    Stored DU databse 
 *    Count of ran functions to be accepted in the list 
 *    Received list of RAN functions
 *    RAN Function list  
 *    Procedure Code 
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildRanFunctionAcceptedList(DuDb *duDb, uint8_t count, RanFunction *ranFunAcceptedList, RANfunctionsID_List_t *ranFuncAcceptedList, uint8_t procedureCode)
{
   uint8_t ranFuncIdx = 0;
   RANfunctionID_ItemIEs_t *ranFuncAcceptedItemIe=NULL;
   
   /* For ProcedureCodeE2_id_E2setup and ProcedureCodeE2_id_RICserviceQuery, 
    * the number of RAN function list items is equal to the number of 
    * ran function entries stored in the database.
    * For any other procedure, the RAN function list count is equal
    * to the count of ran functions obtained from the function's caller */

   if((procedureCode == ProcedureCodeE2_id_RICserviceQuery)||(procedureCode == ProcedureCodeE2_id_E2setup))
      ranFuncAcceptedList->list.count = duDb->numOfRanFunction;
   else
      ranFuncAcceptedList->list.count = count;
   
   ranFuncAcceptedList->list.size = ranFuncAcceptedList->list.count*sizeof(RANfunctionID_ItemIEs_t*);
   RIC_ALLOC(ranFuncAcceptedList->list.array, ranFuncAcceptedList->list.size);
   if(ranFuncAcceptedList->list.array)
   {
      for(ranFuncIdx = 0; ranFuncIdx< ranFuncAcceptedList->list.count; ranFuncIdx++)
      {
         RIC_ALLOC(ranFuncAcceptedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
         if(ranFuncAcceptedList->list.array[ranFuncIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for RAN function added list array item");
            return RFAILED;
         }
         ranFuncAcceptedItemIe = (RANfunctionID_ItemIEs_t*)ranFuncAcceptedList->list.array[ranFuncIdx];
         ranFuncAcceptedItemIe->id = ProtocolIE_IDE2_id_RANfunctionID_Item;
         ranFuncAcceptedItemIe->criticality= CriticalityE2_ignore;
         ranFuncAcceptedItemIe->value.present = RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item;
         if((procedureCode == ProcedureCodeE2_id_RICserviceQuery)||(procedureCode == ProcedureCodeE2_id_E2setup))
         {
            /* filling the RAN function information with the help of DuDb */
            ranFuncAcceptedItemIe->value.choice.RANfunctionID_Item.ranFunctionID = duDb->ranFunction[ranFuncIdx].id;
            ranFuncAcceptedItemIe->value.choice.RANfunctionID_Item.ranFunctionRevision= duDb->ranFunction[ranFuncIdx].revisionCounter;
         }
         else
         {
            /* filling the the RAN function information with the help received list of RAN functions */
            ranFuncAcceptedItemIe->value.choice.RANfunctionID_Item.ranFunctionID = ranFunAcceptedList[ranFuncIdx].id;
            ranFuncAcceptedItemIe->value.choice.RANfunctionID_Item.ranFunctionRevision= ranFunAcceptedList[ranFuncIdx].revisionCounter;
         }
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for RAN function added list array");
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and sends the E2SetupResponse
 *
 * @details
 *
 *    Function : BuildAndSendE2SetupRsp
 *
 *    Functionality: Constructs the F1SetupResponse message and sends
 *                   it back to the DU through SCTP.
 *
 * @params[in] void **buf,Buffer to which encoded pattern is written into
 * @params[in] int *size,size of buffer
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2SetupRsp(DuDb *duDb, uint8_t transId)
{
   E2AP_PDU_t         *e2apMsg = NULL;
   E2setupResponse_t  *e2SetupRsp;
   asn_enc_rval_t     encRetVal; 
   uint8_t            idx;
   uint8_t            elementCnt;
   bool  memAllocFailed = false;
 
   DU_LOG("\nINFO   -->  E2AP : Building E2 Setup Response\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t)); 
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_successfulOutcome;
      RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;  
      }

      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_E2setup;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = \
                                                         SuccessfulOutcomeE2__value_PR_E2setupResponse;
      e2SetupRsp = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;

      elementCnt = 3;
      /* Fill Accepted RAN function IE If Ran function information is stored in databse */
      if(duDb->numOfRanFunction)
         elementCnt++;

      e2SetupRsp->protocolIEs.list.count = elementCnt;
      e2SetupRsp->protocolIEs.list.size  = elementCnt * sizeof(E2setupResponseIEs_t*);

      RIC_ALLOC(e2SetupRsp->protocolIEs.list.array, e2SetupRsp->protocolIEs.list.size);
      if(e2SetupRsp->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2ResponseIEs failed");
         break;  
      }

      for(idx=0; idx<elementCnt; idx++)
      {
         RIC_ALLOC(e2SetupRsp->protocolIEs.list.array[idx], sizeof(E2setupResponseIEs_t)); 
         if(e2SetupRsp->protocolIEs.list.array[idx] == NULLP)
         { 
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2ResponseIEs failed");
            memAllocFailed = true;
            break;
         }    
      }
      
      if(memAllocFailed == true)
      {
          DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2ResponseIEs failed");    
          break;
      }
      /* Trans Id */
      idx = 0;
      e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = E2setupResponseIEs__value_PR_TransactionID; 
      e2SetupRsp->protocolIEs.list.array[idx]->value.choice.TransactionID  = transId;

      /* Global RIC ID */
      idx++;
      e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_GlobalRIC_ID;
      e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = E2setupResponseIEs__value_PR_GlobalRIC_ID;

      if(BuildGlobalRicId(&(e2SetupRsp->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID))!=ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build Global Ric Id");
         break;
      }
      
      if(duDb->numOfRanFunction)
      {
         /* Accepted RAN function Id */
         idx++;
         e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
         e2SetupRsp->protocolIEs.list.array[idx]->value.present = E2setupResponseIEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionAcceptedList(duDb, 0, NULL, &e2SetupRsp->protocolIEs.list.array[idx]->value.choice.RANfunctionsID_List, ProcedureCodeE2_id_E2setup)!=ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build Ran function added list");
            break;         
         }
      }

      /* E2 Node Component Configuration Addition Acknowledge List*/
      idx++;
      e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck;
      e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = \
         E2setupResponseIEs__value_PR_E2nodeComponentConfigAdditionAck_List;
      if(BuildE2nodeComponentConfigAdditionAck(&e2SetupRsp->protocolIEs.list.array[idx]->\
         value.choice.E2nodeComponentConfigAdditionAck_List, duDb) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config addition ack list");
         break;
      }

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SetupResponse structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;   
      } 
      else 
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2SetupResponse\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         } 
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup Response failed");      
         break;   
      }
      break;
   }

   FreeE2SetupRsp(e2apMsg);
   BuildAndSendRicSubscriptionReq(duDb);
   return ROK;
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Details
 *
 * @details
 *
 *    Function : FreeRicSubsDetails
 *
 *    Functionality: Free the RIC Subscription Details
 *
 * @params[in] RICsubscriptionDetails_t *subsDetails
 * @return void
 *
 * ****************************************************************/
void FreeRicSubsDetails(RICsubscriptionDetails_t *subsDetails)
{
   uint8_t elementIdx = 0;
   RICaction_ToBeSetup_ItemIEs_t *actionItem = NULLP;

   RIC_FREE(subsDetails->ricEventTriggerDefinition.buf, subsDetails->ricEventTriggerDefinition.size);

   if(subsDetails->ricAction_ToBeSetup_List.list.array)
   {
      for(elementIdx = 0; elementIdx < subsDetails->ricAction_ToBeSetup_List.list.count; elementIdx++)
      {
         if(subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx])
         {
            actionItem = (RICaction_ToBeSetup_ItemIEs_t *)subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx];
            if(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition)
            {
               RIC_FREE(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->buf, \
                  actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition->size);
               RIC_FREE(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition, sizeof(RICactionDefinition_t));
            }
            RIC_FREE(subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx], sizeof(RICaction_ToBeSetup_ItemIEs_t))
         }
      }
      RIC_FREE(subsDetails->ricAction_ToBeSetup_List.list.array, subsDetails->ricAction_ToBeSetup_List.list.size);
   }
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Request
 *
 * @details
 *
 *    Function : FreeRicSubscriptionReq
 *
 * Functionality : Free RIC Subscription Request
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
void FreeRicSubscriptionReq(E2AP_PDU_t *e2apRicMsg)
{
   uint8_t idx = 0;
   RICsubscriptionRequest_t   *ricSubscriptionReq;

   if(e2apRicMsg)
   {
      if(e2apRicMsg->choice.initiatingMessage)
      {
         ricSubscriptionReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;
         if(ricSubscriptionReq->protocolIEs.list.array)
         {
            for(idx=0; idx < ricSubscriptionReq->protocolIEs.list.count; idx++)
            {
               switch(ricSubscriptionReq->protocolIEs.list.array[idx]->id)
               {
                  case ProtocolIE_IDE2_id_RICsubscriptionDetails:
                     {
                        FreeRicSubsDetails(&(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails));
                        break;
                     }
               }               
               RIC_FREE(ricSubscriptionReq->protocolIEs.list.array[idx], sizeof(RICsubscriptionRequest_IEs_t));
            }
            RIC_FREE(ricSubscriptionReq->protocolIEs.list.array, ricSubscriptionReq->protocolIEs.list.size);
         }
         RIC_FREE(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Builds Ric Request Id
 *
 * @details
 *
 *    Function : BuildNewRicRequestId
 *
 *    Functionality: Assign new Ric Request ID
 *
 * @params[in] RIC request ID to be sent
 *             RIC request ID stored in DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildNewRicRequestId(RICrequestID_t *ricReqId, RicRequestId *reqIdDb)
{
   static uint16_t requestorId = 0;
   static uint16_t instanceId = 0;

   if(ricReqId != NULLP)
   {
      ricReqId->ricRequestorID = ++requestorId;
      ricReqId->ricInstanceID  = ++instanceId;

      reqIdDb->requestorId = ricReqId->ricRequestorID;
      reqIdDb->instanceId = ricReqId->ricInstanceID;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Free RIC Action Definition
 *
 * @details
 *
 *    Function : FreeRicActionDefinition
 *
 *    Functionality: Free RIC Action Definition
 *
 * @params[in] E2SM-KPM Action definition
 * @return void
 *
 * ****************************************************************/
void  FreeRicActionDefinition(E2SM_KPM_ActionDefinition_t actionDef)
{
   uint8_t  elementIdx = 0;
   E2SM_KPM_ActionDefinition_Format1_t *actionFormat1 = NULLP;
   MeasurementInfoItem_t *measItem = NULLP;

   switch(actionDef.actionDefinition_formats.present)
   {
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format1:
         {
            if(actionDef.actionDefinition_formats.choice.actionDefinition_Format1)
            {
               actionFormat1 = actionDef.actionDefinition_formats.choice.actionDefinition_Format1;
               if(actionFormat1->measInfoList.list.array)
               {
                  for(elementIdx = 0; elementIdx < actionFormat1->measInfoList.list.count; elementIdx++)
                  {
                     if(actionFormat1->measInfoList.list.array[elementIdx])
                     {
                        measItem = actionFormat1->measInfoList.list.array[elementIdx];
                        switch(measItem->measType.present)
                        {
                           case MeasurementType_PR_NOTHING:
                           case MeasurementType_PR_measID:
                              break;
                           case MeasurementType_PR_measName:
                           {
                              RIC_FREE(measItem->measType.choice.measName.buf, measItem->measType.choice.measName.size)
                              break;
                           }
                        }
                        RIC_FREE(measItem, sizeof(MeasurementInfoItem_t));
                     }
                  }
                  RIC_FREE(actionFormat1->measInfoList.list.array, actionFormat1->measInfoList.list.size);
               }
               RIC_FREE(actionFormat1, sizeof(E2SM_KPM_ActionDefinition_Format1_t));
            }
            break;
         }

      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format2:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format3:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format4:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format5:
      case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_NOTHING:
         break;
   }
}

/*******************************************************************
 *
 * @brief Fill RIC Action Definition
 *
 * @details
 *
 *    Function : fillRicActionDef
 *
 *    Functionality: Fill RIC Action Definition
 *
 * @params[in] RIC Action definition
 * @return ROK 
 *         RFAILED
 *
 * ****************************************************************/
uint8_t fillRicActionDef(RICactionDefinition_t *ricActionDef)
{
   uint8_t ret = RFAILED;
   asn_enc_rval_t  encRetVal;
   uint8_t elementCnt = 0, elementIdx = 0;
   char    *measurementTypeName[] = {"RRU.PrbTotDl", "RRU.PrbTotUl"};
   E2SM_KPM_ActionDefinition_t actionDef;
   E2SM_KPM_ActionDefinition_Format1_t *actionFormat1 = NULLP;
   MeasurementInfoItem_t *measItem = NULLP;
   
   while(true)
   {
      /* Fill E2SM-KPM Action Definition Format 1 */

      /* RIC Stype Type */
      actionDef.ric_Style_Type = RIC_STYLE_TYPE;

      /* RIC Action Definition Format 1 */
      actionDef.actionDefinition_formats.present = \
           E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format1;

      RIC_ALLOC(actionDef.actionDefinition_formats.choice.actionDefinition_Format1, \
            sizeof(E2SM_KPM_ActionDefinition_Format1_t));
      if(actionDef.actionDefinition_formats.choice.actionDefinition_Format1 == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      actionFormat1 = actionDef.actionDefinition_formats.choice.actionDefinition_Format1;

      /* Measurement Info List */
      elementCnt = 2;
      actionFormat1->measInfoList.list.count = elementCnt;
      actionFormat1->measInfoList.list.size = elementCnt * sizeof(MeasurementInfoItem_t *);
      RIC_ALLOC(actionFormat1->measInfoList.list.array, actionFormat1->measInfoList.list.size);
      if(actionFormat1->measInfoList.list.array == NULL)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      for(elementIdx = 0; elementIdx < elementCnt; elementIdx++)
      {
         RIC_ALLOC(actionFormat1->measInfoList.list.array[elementIdx], sizeof(MeasurementInfoItem_t));
         if(actionFormat1->measInfoList.list.array[elementIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }

         measItem = actionFormat1->measInfoList.list.array[elementIdx];
         measItem->measType.present = MeasurementType_PR_measName;

         measItem->measType.choice.measName.size = strlen(measurementTypeName[elementIdx]);
         RIC_ALLOC(measItem->measType.choice.measName.buf, measItem->measType.choice.measName.size);
         if(measItem->measType.choice.measName.buf == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
         memcpy(measItem->measType.choice.measName.buf, measurementTypeName[elementIdx], measItem->measType.choice.measName.size);
      }
      if(elementIdx < elementCnt)
         break;

      /* Granularity Period */
      actionFormat1->granulPeriod = RIC_ACTION_GRANULARITY_PERIOD; /* In ms */

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2SM_KPM_ActionDefinition, &actionDef);

      /* Encode E2SM-KPM RIC Action Definition */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_ActionDefinition, 0, &actionDef, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SM-KPM action definition structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }

      /* Copty encoded E2SM-KPM RIC action definition to E2AP octet string buffer */
      ricActionDef->size = encBufSize;
      RIC_ALLOC(ricActionDef->buf, encBufSize);
      if(ricActionDef->buf == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      memcpy(ricActionDef->buf, encBuf, encBufSize);

      ret = ROK;
      break;
   }

   FreeRicActionDefinition(actionDef);
   return ret;
}

/*******************************************************************
 *
 * @brief Fills RIC Action To Be Setup Item
 *
 * @details
 *
 *    Function : fillActionToBeSetup
 *
 *    Functionality: Fill the RIC Action To Be Setup Ite,
 *                   RIC subscription DB
 *
 * @params[in] RICaction_ToBeSetup_ItemIEs_t *items
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillActionToBeSetup(RICaction_ToBeSetup_ItemIEs_t *actionItem, RicSubscription *ricSubsDb)
{
   static uint8_t ricActionId = 0;

   if(actionItem == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   while(true)
   {
      actionItem->id = ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item;
      actionItem->criticality   =  CriticalityE2_ignore;
      actionItem->value.present =  RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;
      
      /* RIC Action ID */
      actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID = ricActionId;
      ricSubsDb->actionSequence[ricActionId].actionId = \
         actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID;
      ricActionId++;

      /* RIC Action Type */
      actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionType = RICactionType_report;

      /* RIC Action Definition */
      RIC_ALLOC(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition, sizeof(RICactionDefinition_t));
      if(!actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      if(fillRicActionDef(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      
      ricSubsDb->numOfActions++;
      return ROK;
   }

   memset(&ricSubsDb->actionSequence[ricActionId], 0, sizeof(ActionInfo));
   ricSubsDb->actionSequence[ricActionId].actionId = -1;
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Free Event Trigger Definition
 *
 * @details
 *
 *    Function : FreeEventTriggerDef
 *
 *    Functionality: Free Event Trigger Definition
 *
 * @params[in] E2SM-KPM Event Trigger Definition
 * @return void
 *
 * ****************************************************************/
void  FreeEventTriggerDef(E2SM_KPM_EventTriggerDefinition_t *eventTiggerDef)
{
   if(eventTiggerDef)
   {
      switch(eventTiggerDef->eventDefinition_formats.present)
      {
         case E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_NOTHING:
            break;
         case E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_eventDefinition_Format1: 
            RIC_FREE(eventTiggerDef->eventDefinition_formats.choice.eventDefinition_Format1, \
                  sizeof(E2SM_KPM_EventTriggerDefinition_Format1_t));
            break;                  
      }         
   }
}

/*******************************************************************
 *
 * @brief Fill Event Trigger Definition
 *
 * @details
 *
 *    Function : fillEventTriggerDef
 *
 *    Functionality: Fill Event Trigger Definition
 *
 * @params[in] RIC Event Trigger Definition
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t fillEventTriggerDef(RICeventTriggerDefinition_t *ricEventTriggerDef)
{
   uint8_t ret = RFAILED;
   asn_enc_rval_t  encRetVal;
   E2SM_KPM_EventTriggerDefinition_t eventTiggerDef;

   while(true)
   {
      /* Fill E2SM-KPM Event Trigger Definition Format 1 */
      eventTiggerDef.eventDefinition_formats.present = \
       E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_eventDefinition_Format1;

      RIC_ALLOC(eventTiggerDef.eventDefinition_formats.choice.eventDefinition_Format1, \
            sizeof(E2SM_KPM_EventTriggerDefinition_Format1_t));
      if(eventTiggerDef.eventDefinition_formats.choice.eventDefinition_Format1 == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      eventTiggerDef.eventDefinition_formats.choice.eventDefinition_Format1->reportingPeriod = 1000; /* In ms */

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2SM_KPM_EventTriggerDefinition, &eventTiggerDef);

      /* Encode E2SM-KPM Event Trigger Definition */
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2SM_KPM_EventTriggerDefinition, 0, &eventTiggerDef, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2SM-KPM event trigger definition structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }

      /* Copy encoded E2SM-KPM event trigger definition to E2AP octet string buffer */
      ricEventTriggerDef->size = encBufSize;
      RIC_ALLOC(ricEventTriggerDef->buf, encBufSize);
      if(ricEventTriggerDef->buf == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      memcpy(ricEventTriggerDef->buf, encBuf, encBufSize);

      ret = ROK;
      break;
   }

   FreeEventTriggerDef(&eventTiggerDef);
   return ret;
}

/*******************************************************************
 *
 * @brief builds RIC Subscription Details
 *
 * @details
 *
 *    Function : BuildsRicSubsDetails
 *
 *    Functionality: Builds the RIC Subscription Details
 *
 * @params[in] RIC Subscription details to be filled
 *             RIC subscriotion DB
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildRicSubsDetails(RICsubscriptionDetails_t *subsDetails, RicSubscription *ricSubsDb)
{
   uint8_t actionIdx = 0;
   uint8_t elementCnt = 0;
   uint8_t elementIdx = 0;

   if(subsDetails == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   while(true)
   {
      /* RIC Event Trigger Definition */
      if(fillEventTriggerDef(&subsDetails->ricEventTriggerDefinition) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      /* RIC Actions To Be Setup List */
      elementCnt = 1;
      subsDetails->ricAction_ToBeSetup_List.list.count = elementCnt;
      subsDetails->ricAction_ToBeSetup_List.list.size = elementCnt * sizeof(RICaction_ToBeSetup_ItemIEs_t *);
      RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array, subsDetails->ricAction_ToBeSetup_List.list.size);
      if(subsDetails->ricAction_ToBeSetup_List.list.array  == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICactionToBeSetup Items failed");
         break;
      } 

      for(elementIdx = 0; elementIdx < elementCnt; elementIdx++)
      {
         RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx], sizeof(RICaction_ToBeSetup_ItemIEs_t));
         if(!subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }
      if(elementIdx < elementCnt)
         break;

      for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
      {
         ricSubsDb->actionSequence[actionIdx].actionId = -1;
      }

      elementIdx = 0;
      if(fillActionToBeSetup((RICaction_ToBeSetup_ItemIEs_t *)subsDetails->ricAction_ToBeSetup_List.list.array[elementIdx], \
         ricSubsDb) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      return ROK;
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Builds and Send the RicSubscriptionReq
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionReq
 *
 * Functionality:Fills the RicSubscriptionReq
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendRicSubscriptionReq(DuDb *duDb)
{
   uint8_t         ret = RFAILED;
   E2AP_PDU_t                 *e2apRicMsg = NULL;
   RICsubscriptionRequest_t   *ricSubscriptionReq;
   uint8_t         elementCnt;
   uint8_t         idx;
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   RanFunction     *ranFuncDb = &duDb->ranFunction[0];

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Request\n");

   while(true)
   {
      RIC_ALLOC(e2apRicMsg, sizeof(E2AP_PDU_t));
      if(e2apRicMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      e2apRicMsg->present = E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apRicMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      e2apRicMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscription;
      e2apRicMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apRicMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionRequest;

      ricSubscriptionReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;

      elementCnt = 3;
      ricSubscriptionReq->protocolIEs.list.count = elementCnt;
      ricSubscriptionReq->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionRequest_IEs_t);

      /* Initialize the subscription members */
      RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array, ricSubscriptionReq->protocolIEs.list.size);
      if(ricSubscriptionReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
         RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array[idx], sizeof(RICsubscriptionRequest_IEs_t));
         if(ricSubscriptionReq->protocolIEs.list.array[idx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }
      if(idx < elementCnt)
         break;

      /* Filling RIC Request Id */
      idx = 0;
      ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionRequest_IEs__value_PR_RICrequestID;
      if(BuildNewRicRequestId(&ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICrequestID, \
         &ranFuncDb->subscriptionList[ranFuncDb->numOfSubscription].requestId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : Line [%d]", __func__, __LINE__);
         break;
      }


      /* Filling RAN Function Id */
      idx++;
      ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
      ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID = ranFuncDb->id;

      /* Filling RIC Subscription Details */
      idx++;
      ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICsubscriptionDetails;
      ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;
      if(BuildRicSubsDetails(&(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails),\
         &ranFuncDb->subscriptionList[ranFuncDb->numOfSubscription]) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : Line [%d]", __func__, __LINE__);
         break;
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RicSubscriptionRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;               
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RicSubscriptionRequest\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         } 
      }

      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending RIC subscription Request failed");
         break;
      }

      ranFuncDb->numOfSubscription++;
      ret = ROK;
      break;
   }

   if(ret == RFAILED)
      memset(&ranFuncDb->subscriptionList[ranFuncDb->numOfSubscription], 0, sizeof(RicSubscription));
   FreeRicSubscriptionReq(e2apRicMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Process RicSubscriptionResponse
 *
 * @details
 *
 *    Function : ProcRicSubscriptionRsp
 *
 * Functionality: Processes RicSubscriptionRsp
 *
 * @return ROK     - void
 *
 ******************************************************************/

void ProcRicSubscriptionResponse(uint32_t duId, RICsubscriptionResponse_t  *ricSubscriptionRsp)
{
   uint8_t duIdx = 0, ieIdx = 0, notAdmitIdx = 0;
   uint8_t ranFuncId = 0, actionId = 0;
   DuDb *duDb = NULLP;
   bool ricReqIdDecoded = false;
   RicRequestId ricReqId;
   RanFunction  *ranFuncDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   ActionInfo *action = NULLP;
   RICsubscriptionResponse_IEs_t *ricSubsRspIe = NULLP;
   RICaction_NotAdmitted_List_t *notAdmitList = NULLP;

   DU_LOG("\nINFO  -->  E2AP : RIC Subscription Response received");

   /* Fetch DU DB */
   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }

   memset(&ricReqId, 0, sizeof(RicRequestId));
   if(ricSubscriptionRsp)
   {
      if(ricSubscriptionRsp->protocolIEs.list.array)
      {
         for(ieIdx=0; ieIdx<ricSubscriptionRsp->protocolIEs.list.count; ieIdx++)
         {
            if(ricSubscriptionRsp->protocolIEs.list.array[ieIdx])
            {
               ricSubsRspIe = ricSubscriptionRsp->protocolIEs.list.array[ieIdx];
               switch(ricSubscriptionRsp->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_IDE2_id_RICrequestID:
                     {
                        ricReqId.requestorId = ricSubsRspIe->value.choice.RICrequestID.ricRequestorID;
                        ricReqId.instanceId = ricSubsRspIe->value.choice.RICrequestID.ricInstanceID;
                        ricReqIdDecoded = true;
                        break;
                     }
                  case ProtocolIE_IDE2_id_RANfunctionID:
                     {
                        ranFuncId = ricSubsRspIe->value.choice.RANfunctionID;
                        ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncId);
                        if(!ranFuncDb)
                        {
                           DU_LOG("\nERROR  -->  E2AP : ProcRicSubscriptionResponse: RAN Function ID [%d] not found", ranFuncId);
                           return;
                        }
                        break; 
                     }
                  case ProtocolIE_IDE2_id_RICactions_Admitted:
                     {
                        break;
                     }
                  case ProtocolIE_IDE2_id_RICactions_NotAdmitted:
                     {
                        if(!(ranFuncDb && ricReqIdDecoded))
                           return;

                        notAdmitList = &ricSubsRspIe->value.choice.RICaction_NotAdmitted_List;
                        for(notAdmitIdx = 0; notAdmitIdx < notAdmitList->list.count; notAdmitIdx++)
                        {
                           actionId = ((RICaction_NotAdmitted_ItemIEs_t *)(notAdmitList->list.array[notAdmitIdx]))->\
                              value.choice.RICaction_NotAdmitted_Item.ricActionID;

                           /* Remove action from RAN Function's subscription list */
                           ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb);
                           if(ricSubs)
                           {
                              action = fetchActionInfoFromActionId(actionId, ricSubs);
                              if(action)
                              {
                                 memset(action, 0, sizeof(ActionInfo));
                                 ricSubs->actionSequence[actionId].actionId = -1;
                                 ricSubs->numOfActions--;
                              }
                           }
                        }
                        break;
                     }
               }
            }
         }
      }
   } 
}

/*******************************************************************
 *
 * @brief deallocate the memory allocated in E2SetupFailure
 *
 * @details
 *
 *    Function : FreeE2SetupFailure 
 *
 *    Functionality: deallocate the memory allocated in E2SetupFailure 
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/
void FreeE2SetupFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0;
   E2setupFailure_t  *e2SetupFail;

   if(e2apMsg)
   {
      if(e2apMsg->choice.unsuccessfulOutcome)
      {
         e2SetupFail = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2setupFailure;
         if(e2SetupFail->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<e2SetupFail->protocolIEs.list.count; arrIdx++)
            {
               RIC_FREE(e2SetupFail->protocolIEs.list.array[arrIdx], sizeof(E2setupFailureIEs_t)); 
            }
            RIC_FREE(e2SetupFail->protocolIEs.list.array, e2SetupFail->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Setup failure
 *
 * @details
 *
 *    Function : BuildAndSendE2SetupFailure
 *
 *    Functionality:
 *         - Buld and send the E2 Setup failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2SetupFailure(uint32_t duId, uint8_t transId)
{
   uint8_t            ret = RFAILED;
   E2AP_PDU_t         *e2apMsg = NULL;
   E2setupFailure_t   *e2SetupFailure;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx;
   uint8_t            elementCnt;
   bool  memAllocFailed = false;

   DU_LOG("\nINFO   -->  E2AP : Building E2 Setup failure\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_unsuccessfulOutcome;
      RIC_ALLOC(e2apMsg->choice.unsuccessfulOutcome , sizeof(struct UnsuccessfulOutcomeE2));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_E2setup;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcomeE2__value_PR_E2setupFailure;
      e2SetupFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2setupFailure;

      elementCnt = 3;
      e2SetupFailure->protocolIEs.list.count = elementCnt;
      e2SetupFailure->protocolIEs.list.size  = elementCnt * sizeof(struct E2setupFailureIEs *);

      RIC_ALLOC(e2SetupFailure->protocolIEs.list.array, e2SetupFailure->protocolIEs.list.size);
      if(e2SetupFailure->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2FailureIEs failed");
         break;
      }

      for(arrIdx=0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(e2SetupFailure->protocolIEs.list.array[arrIdx], sizeof(struct E2setupFailureIEs));
         if(e2SetupFailure->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2FailureIEs failed");
            memAllocFailed = true;
            break;
         }
      }

      if(memAllocFailed == true)
      {
          DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2FailureIEs failed");
          break;
      }

      /* Trans Id */
      arrIdx = 0;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_TransactionID;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.TransactionID  = transId;

      arrIdx++;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_CauseE2;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.CauseE2.present = CauseE2_PR_protocol;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.CauseE2.choice.protocol = CauseE2Protocol_unspecified;

      arrIdx++;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TimeToWaitE2;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_ignore;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.present = E2setupFailureIEs__value_PR_TimeToWaitE2;
      e2SetupFailure->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2 = TimeToWaitE2_v5s;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 Setup failure structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Setup Failure\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup Failure failed");
         break;
      }

      ret = ROK;
      break;
   }

   FreeE2SetupFailure(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief process the e2setup request 
 *
 * @details
 *
 *    Function : ProcE2SetupReq
 *
 * Functionality: process the e2setup request
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t ProcE2SetupReq(uint32_t *duId, E2setupRequest_t  *e2SetupReq)
{
   uint8_t arrIdx = 0, e2NodeAddListIdx =0, duIdx = 0, transId =0, ranFuncIdx;
   DuDb    *duDb = NULLP;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem;
   RANfunction_ItemIEs_t *ranFuncItemIe;
   RANfunction_Item_t  *ranFunItem;
   RANfunctions_List_t *ranFunctionsList;

   if(e2SetupReq)
   {
      if(e2SetupReq->protocolIEs.list.array)      
      {
         for(arrIdx=0; arrIdx<e2SetupReq->protocolIEs.list.count; arrIdx++)
         {
            if(e2SetupReq->protocolIEs.list.array[arrIdx])
            {
               switch(e2SetupReq->protocolIEs.list.array[arrIdx]->id)
               {
                  case ProtocolIE_IDE2_id_TransactionID:
                     {
                        transId = e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.TransactionID; 
                        break;
                     }
                  case ProtocolIE_IDE2_id_GlobalE2node_ID:
                     {
                        if(e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.GlobalE2node_ID.choice.gNB->gNB_DU_ID)
                        {
                            *duId =e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.GlobalE2node_ID.choice.gNB->gNB_DU_ID->buf[0];

                            SEARCH_DU_DB(duIdx, *duId, duDb); 
                            if(duDb == NULLP)
                            {
                               duDb = &ricCb.duInfo[ricCb.numDu];
                               ricCb.numDu++;
                            }
                            memset(duDb, 0, sizeof(DuDb));
                            duDb->duId = *duId;
                        }
                        break;
                     }
                     case ProtocolIE_IDE2_id_RANfunctionsAdded:
                     {
                        ranFunctionsList = &e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List;

                        if(ranFunctionsList->list.array)
                        {
                           for(ranFuncIdx=0;ranFuncIdx<ranFunctionsList->list.count; ranFuncIdx++)
                           {
                              ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFunctionsList->list.array[ranFuncIdx]; 
                              ranFunItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                              duDb->ranFunction[ranFunItem->ranFunctionID-1].id = ranFunItem->ranFunctionID; 
                              duDb->ranFunction[ranFunItem->ranFunctionID-1].revisionCounter = ranFunItem->ranFunctionRevision; 
                              duDb->numOfRanFunction++;
                           }
                        }
                        break;
                     }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
                     {
                        e2NodeAddList = &e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List;      
                        if(e2NodeAddList->list.array)
                        {
                           for(e2NodeAddListIdx = 0; e2NodeAddListIdx< e2NodeAddList->list.count; e2NodeAddListIdx++)
                           {
                              if(e2NodeAddList->list.array[e2NodeAddListIdx])
                              {
                                 e2NodeAddItem = \
                                    (E2nodeComponentConfigAddition_ItemIEs_t *)e2NodeAddList->list.array[e2NodeAddListIdx];
                                 if(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.\
                                    choice.e2nodeComponentInterfaceTypeF1)
                                 {
                                    duDb->e2NodeComponent.interfaceType = F1; 
                                    duDb->e2NodeComponent.componentId = \
                                       e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.\
                                       choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]; 
                                 }
                              }
                           }
                        }
                        break;
                     }
                     default:
                        break;
                  }
               }
            }
        }
   }
   
   if(BuildAndSendE2SetupRsp(duDb, transId) !=ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 setup response");
      return RFAILED;
   }
   return ROK;   
}
/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2 Reset Response
 *
 * @details
 *
 *    Function : FreeE2ResetResponse
 *
 *    Functionality:
 *       - freeing the memory allocated for E2ResetResponse
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeE2ResetResponse(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   ResetResponseE2_t *resetResponse;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         resetResponse = &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2;
         if(resetResponse->protocolIEs.list.array)
         {
            for(ieIdx=0; ieIdx < resetResponse->protocolIEs.list.count; ieIdx++)
            {
               if(resetResponse->protocolIEs.list.array[ieIdx])
               {
                  RIC_FREE(resetResponse->protocolIEs.list.array[ieIdx], sizeof(ResetResponseIEs_t));
               }
            }
            RIC_FREE(resetResponse->protocolIEs.list.array, resetResponse->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Reset Response msg
 *
 * @details
 *
 *    Function : BuildAndSendE2ResetResponse
 *
 *    Functionality:
 *         - Buld and send the E2 Reset Response Message
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendResetResponse(uint32_t duId, uint8_t transId)
{
   uint8_t           ieIdx = 0, elementCnt = 0;
   uint8_t           ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   ResetResponseE2_t *resetResponse;
   asn_enc_rval_t    encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Reset Response Message\n");
   do
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse(): Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_successfulOutcome;

      RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for successfulOutcome");
         break;
      }
 
      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_Reset;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_ResetResponseE2;
      resetResponse = &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2;

      elementCnt = 1;
      resetResponse->protocolIEs.list.count = elementCnt;
      resetResponse->protocolIEs.list.size = elementCnt * sizeof(ResetResponseIEs_t *);
      RIC_ALLOC(resetResponse->protocolIEs.list.array, resetResponse->protocolIEs.list.size);
      if(!resetResponse->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for protocol IE array");
         break;
      }

      for(ieIdx=0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(resetResponse->protocolIEs.list.array[ieIdx], sizeof(ResetResponseIEs_t));
         if(!resetResponse->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetResponse: Memory allocation failed for protocol IE array element");
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0; 
      resetResponse->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_TransactionID;
      resetResponse->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      resetResponse->protocolIEs.list.array[ieIdx]->value.present = ResetResponseIEs__value_PR_TransactionID;
      resetResponse->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 reset response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Reset Response \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Reset Response");
         break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeE2ResetResponse(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief process the E2 Reset Request
 *
 * @details
 *
 *    Function : ProcE2ResetReq
 *
 * Functionality: Process E2 Reset Request
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t ProcE2ResetReq(uint32_t duId, ResetRequestE2_t  *resetReq)
{
   uint8_t ieIdx = 0;
   uint8_t transId = 0, cause = 0;

   if(resetReq)
   {
      if(resetReq->protocolIEs.list.array)
      {
         for(ieIdx=0; ieIdx < resetReq->protocolIEs.list.count; ieIdx++)
         {
            if(resetReq->protocolIEs.list.array[ieIdx])
            {
               switch(resetReq->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_IDE2_id_TransactionID:
                     transId = resetReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
                     break;
                  case ProtocolIE_IDE2_id_CauseE2:
                     DU_LOG("\nDEBUG  -->  E2AP : Reset reason %d", resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.present);
                     switch(resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.present)
                     {
                        case CauseE2_PR_NOTHING:
                           break;
                        case CauseE2_PR_ricRequest:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.ricRequest;
                           break;
                        case CauseE2_PR_ricService:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.ricService;
                           break;
                        case CauseE2_PR_e2Node:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.e2Node;
                           break;
                        case CauseE2_PR_transport:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.transport;
                           break;
                        case CauseE2_PR_protocol:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.protocol;
                           break;
                        case CauseE2_PR_misc:
                           cause = resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2.choice.misc;
                           break;
                     }
                     DU_LOG("\nDEBUG  -->  E2AP : Reset cause %d", cause);
                     break;
               }
            }
         }
      }
   }
   BuildAndSendResetResponse(duId, transId);
   return ROK;
}

/*******************************************************************
 *
 * @brief deallocate the memory allocated in building the
 *    Service Query message
 *
 * @details
 *
 *    Function : FreeRicServiceQuery 
 *
 *    Functionality: deallocate the memory allocated in building
 *    Ric Service Query message
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/

void FreeRicServiceQuery(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0, ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAcceptedList=NULL;
   RICserviceQuery_t *ricServiceQuery=NULL;
   
   if(e2apMsg)
   {
      if(e2apMsg->choice.initiatingMessage)
      {
         ricServiceQuery = &e2apMsg->choice.initiatingMessage->value.choice.RICserviceQuery;
         if(ricServiceQuery->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<ricServiceQuery->protocolIEs.list.count; arrIdx++)
            {
               if(ricServiceQuery->protocolIEs.list.array[arrIdx])
               {
                  switch(ricServiceQuery->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_RANfunctionsAccepted:
                        {
                           ranFuncAcceptedList= &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                           if(ranFuncAcceptedList->list.array)
                           {
                              for(ranFuncIdx=0;ranFuncIdx<ranFuncAcceptedList->list.count; ranFuncIdx++)
                              {
                                 RIC_FREE(ranFuncAcceptedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                              }
                              RIC_FREE(ranFuncAcceptedList->list.array, ranFuncAcceptedList->list.size);
                           }
                           break;
                        }
                     case RICserviceQuery_IEs__value_PR_TransactionID:
                        {
                           break;
                        }
                  }
                  RIC_FREE(ricServiceQuery->protocolIEs.list.array[arrIdx], sizeof(RICserviceQuery_IEs_t)); 
               }
            }
            RIC_FREE(ricServiceQuery->protocolIEs.list.array, ricServiceQuery->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief build and send the ric service Query 
 *
 * @details
 *
 *    Function : BuildAndSendRicServiceQuery
 *
 * Functionality: build and send the ric service Query 
 * @return ROK     - success
 *         RFAILED - Acknowledge
 *
 ******************************************************************/

uint8_t BuildAndSendRicServiceQuery(DuDb *duDb)
{
   uint8_t arrIdx;
   uint8_t elementCnt;
   uint8_t ret = RFAILED;
   bool  memAllocFailed = false;
   E2AP_PDU_t     *e2apMsg = NULL;
   asn_enc_rval_t encRetVal;
   RICserviceQuery_t *ricServiceQuery;

   DU_LOG("\nINFO   -->  E2AP : Building Ric service Query\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apMsg->choice.initiatingMessage , sizeof(struct InitiatingMessageE2));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICserviceQuery;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICserviceQuery;
      ricServiceQuery = &e2apMsg->choice.initiatingMessage->value.choice.RICserviceQuery;

      elementCnt = 1;
      /* Fill Accepted RAN function IE If Ran function information is stored in databse */
      if(duDb->numOfRanFunction)
         elementCnt++;

      ricServiceQuery->protocolIEs.list.count = elementCnt;
      ricServiceQuery->protocolIEs.list.size  = elementCnt * sizeof(RICserviceQuery_IEs_t*);

      RIC_ALLOC(ricServiceQuery->protocolIEs.list.array, ricServiceQuery->protocolIEs.list.size);
      if(ricServiceQuery->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceQueryIEs failed");
         break;
      }

      for(arrIdx=0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(ricServiceQuery->protocolIEs.list.array[arrIdx], sizeof(RICserviceQuery_IEs_t));
         if(ricServiceQuery->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceQueryIEs failed");
            memAllocFailed = true;
            break;
         }
      }
      if(memAllocFailed == true)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceQueryIEs failed");
         break;
      }

      /* Trans Id */
      arrIdx = 0;
      ricServiceQuery->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      ricServiceQuery->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      ricServiceQuery->protocolIEs.list.array[arrIdx]->value.present = RICserviceQuery_IEs__value_PR_TransactionID;
      ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.TransactionID  = assignTransactionId(duDb);
      
      if(duDb->numOfRanFunction)
      {
         /* Accepted RAN function Id */
         arrIdx++;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->value.present = RICserviceQuery_IEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionAcceptedList(duDb, 0, NULL, &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List, ProcedureCodeE2_id_RICserviceQuery)!=ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build Ran function added list");
            break;         
         }
      }
      
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC service Query structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC service Query\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending of RIC service  Query failed");
         break;
      }

      ret =ROK;
      break;
   }
   FreeRicServiceQuery(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief deallocate the memory allocated in RicServiceUpdateFailure
 *
 * @details
 *
 *    Function : FreeRicServiceUpdateFailure 
 *
 *    Functionality: deallocate the memory allocated in RicServiceUpdatefailure
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/

void FreeRicServiceUpdateFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0;
   RICserviceUpdateFailure_t *ricServiceUpdateFailure=NULL;
   
   if(e2apMsg)
   {
      if(e2apMsg->choice.unsuccessfulOutcome)
      {
         ricServiceUpdateFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICserviceUpdateFailure;
         if(ricServiceUpdateFailure->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<ricServiceUpdateFailure->protocolIEs.list.count; arrIdx++)
            {
               RIC_FREE(ricServiceUpdateFailure->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdateFailure_IEs_t)); 
            }
            RIC_FREE(ricServiceUpdateFailure->protocolIEs.list.array, ricServiceUpdateFailure->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief fill E2 failure cause 
 *
 * @details
 *
 *    Function : fillE2FailureCause
 *
 * Functionality: fill E2 failure cause
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

void fillE2FailureCause(CauseE2_t *cause, CauseE2_PR causePresent, uint8_t reason)
{
   cause->present = causePresent;

   switch(cause->present)
   {
      case CauseE2_PR_ricRequest:
         cause->choice.ricRequest = reason;
         break;
      case CauseE2_PR_ricService:
         cause->choice.ricService = reason;
         break;
      case CauseE2_PR_e2Node:
         cause->choice.e2Node = reason;
         break;
      case CauseE2_PR_transport:
         cause->choice.transport = reason;
         break;
      case CauseE2_PR_protocol:
         cause->choice.protocol = reason;
         break;
      case CauseE2_PR_misc:
         cause->choice.misc = reason;
         break;
      default:
         cause->choice.misc = CauseE2Misc_unspecified;
         break;
   }
}

/*******************************************************************
 *
 * @brief build and send the ric service update failure 
 *
 * @details
 *
 *    Function : BuildAndSendRicServiceUpdateFailure
 *
 * Functionality: build and send the ric service update failure 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendRicServiceUpdateFailure(uint32_t duId, int8_t transId, CauseE2_PR causePresent, uint8_t reason)
{

   E2AP_PDU_t         *e2apMsg = NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t            ret = RFAILED;
   uint8_t            arrIdx=0;
   uint8_t            elementCnt=0;
   RICserviceUpdateFailure_t *ricServiceFailure=NULL;

   DU_LOG("\nINFO   -->  E2AP : Building Ric service update failure\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_unsuccessfulOutcome;
      RIC_ALLOC(e2apMsg->choice.unsuccessfulOutcome , sizeof(struct UnsuccessfulOutcomeE2));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_RICserviceUpdate;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcomeE2__value_PR_RICserviceUpdateFailure;
      ricServiceFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICserviceUpdateFailure;

      elementCnt = 3;
      ricServiceFailure->protocolIEs.list.count = elementCnt;
      ricServiceFailure->protocolIEs.list.size  = elementCnt * sizeof(RICserviceUpdateFailure_IEs_t *);

      RIC_ALLOC(ricServiceFailure->protocolIEs.list.array, ricServiceFailure->protocolIEs.list.size);
      if(ricServiceFailure->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceFailureIEs failed");
         break;
      }

      for(arrIdx=0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(ricServiceFailure->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdateFailure_IEs_t));
         if(ricServiceFailure->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceFailureIEs failed");
            break;
         }
      }
      if(arrIdx<elementCnt)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceFailureIEs failed");
         break;
      }

      /* Trans Id */
      arrIdx = 0;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateFailure_IEs__value_PR_TransactionID;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->value.choice.TransactionID  = transId;

      arrIdx++;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateFailure_IEs__value_PR_CauseE2;
      fillE2FailureCause(&ricServiceFailure->protocolIEs.list.array[arrIdx]->value.choice.CauseE2, causePresent, reason);

      arrIdx++;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TimeToWaitE2;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_ignore;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateFailure_IEs__value_PR_TimeToWaitE2;
      ricServiceFailure->protocolIEs.list.array[arrIdx]->value.choice.TimeToWaitE2 = TimeToWaitE2_v5s;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC service update failure structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC service update Failure\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending RIC service update failed");
         break;
      }
      ret = ROK;
      break;
   }

   FreeRicServiceUpdateFailure(e2apMsg);
   return ret;
}


/*******************************************************************
 *
 * @brief deallocate the memory allocated in RicServiceUpdateAck(
 *
 * @details
 *
 *    Function : FreeRicServiceUpdateAck 
 *
 *    Functionality: deallocate the memory allocated in RicServiceUpdateAck
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/

void FreeRicServiceUpdateAck(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0, ranFuncIdx=0;
   RANfunctionsID_List_t *acceptedList=NULL;
   RICserviceUpdateAcknowledge_t *ricServiceUpdateAck=NULL;
   RANfunctionsIDcause_List_t  *rejectedList=NULL;

   if(e2apMsg)
   {
      if(e2apMsg->choice.successfulOutcome)
      {
         ricServiceUpdateAck = &e2apMsg->choice.successfulOutcome->value.choice.RICserviceUpdateAcknowledge;
         if(ricServiceUpdateAck->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<ricServiceUpdateAck->protocolIEs.list.count; arrIdx++)
            {
               if(ricServiceUpdateAck->protocolIEs.list.array[arrIdx])
               {
                  switch(ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_RANfunctionsAccepted:
                        {
                           acceptedList= &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                           if(acceptedList->list.array)
                           {
                              for(ranFuncIdx=0;ranFuncIdx<acceptedList->list.count; ranFuncIdx++)
                              {
                                 RIC_FREE(acceptedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                              }
                              RIC_FREE(acceptedList->list.array, acceptedList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_RANfunctionsRejected:
                        {
                           rejectedList= &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsIDcause_List;
                           if(rejectedList->list.array)
                           {
                              for(ranFuncIdx=0;ranFuncIdx<rejectedList->list.count; ranFuncIdx++)
                              {
                                 RIC_FREE(rejectedList->list.array[ranFuncIdx], sizeof(RANfunctionIDcause_ItemIEs_t));
                              }
                              RIC_FREE(rejectedList->list.array, rejectedList->list.size);
                           }
                           break;
                        }
                  }
                  RIC_FREE(ricServiceUpdateAck->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdateAcknowledge_IEs_t)); 
               }
            }
            RIC_FREE(ricServiceUpdateAck->protocolIEs.list.array, ricServiceUpdateAck->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build RAN function rejected list
 *
 * @details
 *
 *    Function : BuildRanFunctionRejectedList
 *
 *    Functionality: Build RAN function rejected list 
 *
 * @params[in] 
 *    Count of ran functions to be rejected in the list 
 *    Received list of RAN functions
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildRanFunctionRejectedList(uint8_t count, RanFunction *ranFunRejectedList, RANfunctionsIDcause_List_t *ranFuncRejectedList)
{
   uint8_t ranFuncIdx = 0;
   RANfunctionIDcause_ItemIEs_t *ranFuncRejectedItemIe=NULL;
   
   ranFuncRejectedList->list.count = count;
   
   ranFuncRejectedList->list.size = ranFuncRejectedList->list.count*sizeof(RANfunctionIDcause_ItemIEs_t*);
   RIC_ALLOC(ranFuncRejectedList->list.array, ranFuncRejectedList->list.size);
   if(ranFuncRejectedList->list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for RAN function rejected list array");
      return RFAILED;
   }
   
   for(ranFuncIdx = 0; ranFuncIdx< ranFuncRejectedList->list.count; ranFuncIdx++)
   {
      RIC_ALLOC(ranFuncRejectedList->list.array[ranFuncIdx], sizeof(RANfunctionIDcause_ItemIEs_t));
      if(ranFuncRejectedList->list.array[ranFuncIdx] == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for RAN function rejected list array item");
         return RFAILED;
      }
      ranFuncRejectedItemIe = (RANfunctionIDcause_ItemIEs_t*)ranFuncRejectedList->list.array[ranFuncIdx];
      ranFuncRejectedItemIe->id = ProtocolIE_IDE2_id_RANfunctionIEcause_Item;
      ranFuncRejectedItemIe->criticality= CriticalityE2_ignore;
      ranFuncRejectedItemIe->value.present = RANfunctionIDcause_ItemIEs__value_PR_RANfunctionIDcause_Item;
      ranFuncRejectedItemIe->value.choice.RANfunctionIDcause_Item.ranFunctionID = ranFunRejectedList[ranFuncIdx].id;
      fillE2FailureCause(&ranFuncRejectedItemIe->value.choice.RANfunctionIDcause_Item.cause, CauseE2_PR_ricService,\
            CauseE2RICservice_ran_function_not_supported);
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief build and send the ric service update Acknowledge 
 *
 * @details
 *
 *    Function : BuildAndSendRicServiceUpdateAcknowledge
 *
 * Functionality: build and send the ric service update Acknowledge 
 * @return ROK     - success
 *         RFAILED - Acknowledge
 *
 ******************************************************************/

uint8_t BuildAndSendRicServiceUpdateAcknowledge(DuDb *duDb, int8_t transId, RicTmpRanFunList ricRanFuncList)
{
   E2AP_PDU_t         *e2apMsg = NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t  arrIdx=0, elementCnt=0, ret=RFAILED;;
   RICserviceUpdateAcknowledge_t *ricServiceUpdateAck=NULL;

   DU_LOG("\nINFO   -->  E2AP : Building Ric service update Acknowledge\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_successfulOutcome;
      RIC_ALLOC(e2apMsg->choice.successfulOutcome , sizeof(struct SuccessfulOutcomeE2));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_RICserviceUpdate;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_RICserviceUpdateAcknowledge;
      ricServiceUpdateAck = &e2apMsg->choice.successfulOutcome->value.choice.RICserviceUpdateAcknowledge;

      elementCnt = 1;
      if(ricRanFuncList.numOfRanFunAccepted)
         elementCnt++;
      if(ricRanFuncList.numOfRanFuneRejected)
         elementCnt++;
      

      ricServiceUpdateAck->protocolIEs.list.count = elementCnt;
      ricServiceUpdateAck->protocolIEs.list.size  = elementCnt * sizeof(RICserviceUpdateAcknowledge_IEs_t*);

      RIC_ALLOC(ricServiceUpdateAck->protocolIEs.list.array, ricServiceUpdateAck->protocolIEs.list.size);
      if(ricServiceUpdateAck->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceUpdateAckIEs failed");
         break;
      }

      for(arrIdx=0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(ricServiceUpdateAck->protocolIEs.list.array[arrIdx], sizeof(RICserviceUpdateAcknowledge_IEs_t));
         if(ricServiceUpdateAck->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceUpdateAckIEs failed");
            break;
         }
      }
      if(arrIdx<elementCnt)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for ricServiceUpdateAckIEs failed");
         break;
      }

      /* Trans Id */
      arrIdx = 0;
      ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateAcknowledge_IEs__value_PR_TransactionID;
      ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.TransactionID  = transId;

      if(ricRanFuncList.numOfRanFunAccepted)
      {
         /* Accepted RAN function List */
         arrIdx++;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateAcknowledge_IEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionAcceptedList(duDb, ricRanFuncList.numOfRanFunAccepted, ricRanFuncList.ranFunAcceptedList,\
         &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List, ProcedureCodeE2_id_RICserviceUpdate)!=ROK)       
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build Ran function added list");
            break;         
         }
      }
      
      if(ricRanFuncList.numOfRanFuneRejected)
      {
         /* RAN Functions Rejected List */
         arrIdx++;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsRejected;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateAcknowledge_IEs__value_PR_RANfunctionsIDcause_List;
         if(BuildRanFunctionRejectedList(ricRanFuncList.numOfRanFuneRejected, ricRanFuncList.ranFunRejectedList, \
         &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsIDcause_List)!=ROK)       
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build Ran function rejected list");
            break;         
         }
      }
      
      
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC service update Acknowledge structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC service update Acknowledge\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending RIC service update ack failed");
         break;
      }
      ret =ROK;
      break;
   }
   FreeRicServiceUpdateAck(e2apMsg);
   return ret; 
}

/*******************************************************************
 *
 * @brief process the RIC service update 
 *
 * @details
 *
 *    Function : ProcRicserviceUpdate 
 *
 * Functionality: process the RIC service update 
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

void ProcRicServiceUpdate(uint32_t duId, RICserviceUpdate_t *ricServiceUpdate)
{
   RicTmpRanFunList ricRanFuncList;
   DuDb    *duDb = NULLP;
   int8_t transId =-1;
   uint8_t duIdx = 0, elementCnt =0, arrIdx = 0; 
   uint16_t ranFuncIdx = 0, failedRanFuncCount=0, recvdRanFuncCount=0;
   RanFunction *ranFuncDb = NULLP;
   RANfunction_ItemIEs_t *ranFuncItemIe =NULL;
   RANfunction_Item_t  *ranFuncItem =NULL;
   RANfunctionID_Item_t  *ranFuncIdItem=NULL;
   RANfunctions_List_t *ranFuncList=NULL;
   RANfunctionsID_List_t *deleteList=NULL;
   RANfunctionID_ItemIEs_t *delRanFuncItem=NULL;

   SEARCH_DU_DB(duIdx, duId, duDb); 
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }
   memset(&ricRanFuncList, 0, sizeof(RicTmpRanFunList)); 

   if(!ricServiceUpdate)
   {
      DU_LOG("\nERROR  -->  E2AP : ricServiceUpdate pointer is null"); 
      return;
   }

   if(!ricServiceUpdate->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : ricServiceUpdate array pointer is null");
      return;
   }
   elementCnt = ricServiceUpdate->protocolIEs.list.count;
   for(arrIdx=0; arrIdx<ricServiceUpdate->protocolIEs.list.count; arrIdx++)
   {
      if(!ricServiceUpdate->protocolIEs.list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : ricServiceUpdate array idx %d pointer is null",arrIdx);
         return;
      }

      switch(ricServiceUpdate->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;

               if(transId < 0 || transId > 255)
               {
                  DU_LOG("\nERROR  -->  E2AP : Received invalid transId %d",transId);
                  return;
               }
               break;
            }

         case ProtocolIE_IDE2_id_RANfunctionsAdded:
            {
               ranFuncList = &ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List;

               if(ranFuncList->list.array)
               {
                  for(ranFuncIdx=0;ranFuncIdx<ranFuncList->list.count; ranFuncIdx++)
                  {
                     ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFuncList->list.array[ranFuncIdx]; 
                     ranFuncItem = &ranFuncItemIe->value.choice.RANfunction_Item;

                     /* Adding the ran function in temporary list */
                     ricRanFuncList.ranFunAcceptedList[ricRanFuncList.numOfRanFunAccepted].id =  ranFuncItem->ranFunctionID; 
                     ricRanFuncList.ranFunAcceptedList[ricRanFuncList.numOfRanFunAccepted].revisionCounter = ranFuncItem->ranFunctionRevision; 
                     ricRanFuncList.numOfRanFunAccepted++;

                     /* Adding the new ran function in DB*/
                     duDb->ranFunction[ranFuncItem->ranFunctionID-1].id = ranFuncItem->ranFunctionID;
                     duDb->ranFunction[ranFuncItem->ranFunctionID-1].revisionCounter = ranFuncItem->ranFunctionRevision;
                     duDb->numOfRanFunction++;

                     /* Calculating total number of ran fuctions which are received for addition */
                     recvdRanFuncCount++;
                  }
               }
               break;
            }

         case ProtocolIE_IDE2_id_RANfunctionsModified:
            {

               ranFuncList = &ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List; 
               if(ranFuncList->list.array)
               {
                  for(ranFuncIdx = 0; ranFuncIdx< ranFuncList->list.count; ranFuncIdx++)
                  {
                     ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFuncList->list.array[ranFuncIdx];
                     ranFuncItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                     if(fetchRanFuncFromRanFuncId(duDb, ranFuncItem->ranFunctionID) == NULLP)
                     {
                        /* Calculating total number of ran fuctions which are not present */
                        failedRanFuncCount++;

                        /* Adding the ran function in temporary list */
                        ricRanFuncList.ranFunRejectedList[ricRanFuncList.numOfRanFuneRejected].id =  ranFuncItem->ranFunctionID; 
                        ricRanFuncList.numOfRanFuneRejected++;
                     }
                     else
                     {

                        /* Adding the ran function in temporary list */
                        ricRanFuncList.ranFunAcceptedList[ricRanFuncList.numOfRanFunAccepted].id =  ranFuncItem->ranFunctionID; 
                        ricRanFuncList.ranFunAcceptedList[ricRanFuncList.numOfRanFunAccepted].revisionCounter = ranFuncItem->ranFunctionRevision; 
                        ricRanFuncList.numOfRanFunAccepted++;

                        /* Updating the new ran function in DB*/
                        duDb->ranFunction[ranFuncItem->ranFunctionID-1].revisionCounter = ranFuncItem->ranFunctionRevision;
                     }
                     /* Calculating total number of ran fuctions which are received for modification */
                     recvdRanFuncCount++;
                  }
               }
               break;
            }
         case ProtocolIE_IDE2_id_RANfunctionsDeleted:
            {

               deleteList = &ricServiceUpdate->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List; 
               if(deleteList->list.array)
               {
                  for(ranFuncIdx = 0; ranFuncIdx< deleteList->list.count; ranFuncIdx++)
                  {
                     delRanFuncItem  = (RANfunctionID_ItemIEs_t*) deleteList->list.array[ranFuncIdx];
                     ranFuncIdItem = &delRanFuncItem->value.choice.RANfunctionID_Item;
                     ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncIdItem->ranFunctionID);
                     if(ranFuncDb)
                     {
                        memset(ranFuncDb, 0, sizeof(RanFunction));
                        duDb->numOfRanFunction--; 
                     }

                     /* Calculating total number of ran fuctions which are received for deletion */
                     recvdRanFuncCount++;
                  }
               }
               break;
            }

         default:
            {
               DU_LOG("\nERROR  -->  E2AP : IE [%ld] is not supported",ricServiceUpdate->protocolIEs.list.array[arrIdx]->id);
               break;
            }
      }
   }
   
   /* Sending RIC Service Update Failed if all RAN Functions received fail or if any IE processing fails
    * Else sending RIC Service Update Acknowledge */  
   if((elementCnt > arrIdx) ||((recvdRanFuncCount > 0) && (recvdRanFuncCount == failedRanFuncCount)))
   {
      if(BuildAndSendRicServiceUpdateFailure(duDb->duId, transId, CauseE2_PR_misc, CauseE2Misc_unspecified) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send RIC service update Failure");
         return;
      }
   }
   else
   {
      if(BuildAndSendRicServiceUpdateAcknowledge(duDb, transId, ricRanFuncList) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send RIC service update acknowledge");
         return;
      }
   }
}

/*******************************************************************
 *
 * @brief Processing RIC subscription failure from DU
 *
 * @details
 *
 *    Function : ProcRicSubscriptionFailure
 *
 * Functionality: Processing RIC subscription failure from DU
 *
 * @param  ID of DU from which message was sent
 *         RIC Subscription failure message
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t ProcRicSubscriptionFailure(uint32_t duId, RICsubscriptionFailure_t *ricSubscriptionFailure)
{
   uint8_t ieIdx = 0, duIdx = 0, subsIdx = 0;
   uint8_t ranFuncId = 0;
   DuDb    *duDb = NULLP;
   RanFunction *ranFuncDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   RicRequestId ricReqId;
   RICsubscriptionFailure_IEs_t *ricSubsFailIe = NULLP;

   DU_LOG("\nINFO  -->  E2AP : Received RIC subscription failure");

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return RFAILED;
   }

   memset(&ricReqId, 0, sizeof(RicRequestId));
   if(ricSubscriptionFailure)
   {
      if(ricSubscriptionFailure->protocolIEs.list.array)
      {
         for(ieIdx=0; ieIdx<ricSubscriptionFailure->protocolIEs.list.count; ieIdx++)
         {
            if(ricSubscriptionFailure->protocolIEs.list.array[ieIdx])
            {
               ricSubsFailIe = ricSubscriptionFailure->protocolIEs.list.array[ieIdx];
               switch(ricSubscriptionFailure->protocolIEs.list.array[ieIdx]->id)
               {
                  case ProtocolIE_IDE2_id_RICrequestID:
                  {
                     ricReqId.requestorId = ricSubsFailIe->value.choice.RICrequestID.ricRequestorID;
                     ricReqId.instanceId = ricSubsFailIe->value.choice.RICrequestID.ricInstanceID;
                     break;
                  }
                  case ProtocolIE_IDE2_id_RANfunctionID:
                  {
                     ranFuncId = ricSubsFailIe->value.choice.RANfunctionID;
                     ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncId);
                     if(!ranFuncDb)
                     {
                        DU_LOG("\nERROR  -->  E2AP : ProcRicSubscriptionFailure : RAN Function Id [%d] not found", ranFuncId);
                        return RFAILED;
                     }
                     else
                     {
                        /* Remove subscription entry from RAN Function */
                        ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb);
                        if(ricSubs)
                        {
                           memset(&ranFuncDb->subscriptionList[subsIdx], 0, sizeof(RicSubscription));
                        }
                     }
                     break; 
                  }
                  case ProtocolIE_IDE2_id_CauseE2:
                  default:
                     /* No handling required as of now since this is a stub */
                     break;
               }
            }
         }
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Modification Refuse
 *
 * @details
 *
 *    Function : FreeRicSubsModRefuse
 *
 * Functionality: Free RIC Subscription Modification Refuse
 *
 * @param  E2AP Message PDU to be freed
 * @return void
 *
 ******************************************************************/
void FreeRicSubsModRefuse(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   RICsubscriptionModificationRefuse_t *ricSubsModRefuse = NULLP;

   if(e2apMsg)
   {
      if(e2apMsg->choice.unsuccessfulOutcome)
      {
         ricSubsModRefuse = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionModificationRefuse;
         if(ricSubsModRefuse->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < ricSubsModRefuse->protocolIEs.list.count; ieIdx++)
            {
               RIC_FREE(ricSubsModRefuse->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionModificationRefuse_IEs_t));
            }
            RIC_FREE(ricSubsModRefuse->protocolIEs.list.array, ricSubsModRefuse->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.unsuccessfulOutcome , sizeof(UnsuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build And Send RIC Subscription Modification Refuse
 *
 * @details
 *
 *    Function : BuildAndSendRicSubsModRefuse
 *
 * Functionality: Build And Send RIC Subscription Modification Refuse
 *
 * @param DU ID
 *        RIC Request ID of subscription
 *        RAN Function ID
 *        Type of failure
 *        Cause of failure
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendRicSubsModRefuse(uint32_t duId, RicRequestId ricReqId, uint16_t ranFuncId, CauseE2_PR causeType, \
   uint8_t cause)
{
   uint8_t ieIdx = 0, elementCnt = 0;
   uint8_t ret = RFAILED;
   E2AP_PDU_t *e2apMsg = NULL;
   asn_enc_rval_t encRetVal;
   RICsubscriptionModificationRefuse_t *ricSubsModRefuse = NULLP;
   RICsubscriptionModificationRefuse_IEs_t *ricSubsModRefuseIe = NULLP;

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Modification Refuse\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }
      e2apMsg->present =  E2AP_PDU_PR_unsuccessfulOutcome;
      RIC_ALLOC(e2apMsg->choice.unsuccessfulOutcome , sizeof(UnsuccessfulOutcomeE2_t));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_RICsubscriptionModificationRequired;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = \
         UnsuccessfulOutcomeE2__value_PR_RICsubscriptionModificationRefuse;
      ricSubsModRefuse = &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionModificationRefuse;

      elementCnt = 3;
      ricSubsModRefuse->protocolIEs.list.count = elementCnt;
      ricSubsModRefuse->protocolIEs.list.size = elementCnt * sizeof(RICsubscriptionModificationRefuse_IEs_t *);
      RIC_ALLOC(ricSubsModRefuse->protocolIEs.list.array, ricSubsModRefuse->protocolIEs.list.size);
      if(!ricSubsModRefuse->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(ricSubsModRefuse->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionModificationRefuse_IEs_t));
         if(!ricSubsModRefuse->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
            break;
         }
      }
      
      /* RIC Request ID */
      ieIdx = 0;
      ricSubsModRefuseIe = ricSubsModRefuse->protocolIEs.list.array[ieIdx];
      ricSubsModRefuseIe->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubsModRefuseIe->criticality = CriticalityE2_reject;
      ricSubsModRefuseIe->value.present = RICsubscriptionModificationRefuse_IEs__value_PR_RICrequestID;
      ricSubsModRefuseIe->value.choice.RICrequestID.ricRequestorID = ricReqId.requestorId;
      ricSubsModRefuseIe->value.choice.RICrequestID.ricInstanceID = ricReqId.instanceId;

      /* RAN Function ID */
      ieIdx++;
      ricSubsModRefuseIe = ricSubsModRefuse->protocolIEs.list.array[ieIdx];
      ricSubsModRefuseIe->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubsModRefuseIe->criticality = CriticalityE2_reject;
      ricSubsModRefuseIe->value.present = RICsubscriptionModificationRefuse_IEs__value_PR_RANfunctionID;
      ricSubsModRefuseIe->value.choice.RANfunctionID = ranFuncId;

      /* Cause */
      ieIdx++;
      ricSubsModRefuseIe = ricSubsModRefuse->protocolIEs.list.array[ieIdx];
      ricSubsModRefuseIe->id = ProtocolIE_IDE2_id_CauseE2;
      ricSubsModRefuseIe->criticality = CriticalityE2_reject;
      ricSubsModRefuseIe->value.present = RICsubscriptionModificationRefuse_IEs__value_PR_CauseE2;
      fillE2FailureCause(&ricSubsModRefuseIe->value.choice.CauseE2, causeType, cause); 

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC subscription modification refuse (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC subscription modification refuse\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send RIC Subscription Modification Refused");
         break;
      }

      ret =ROK;
      break;
   }
   FreeRicSubsModRefuse(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Free memory for RIC Subscription Modification Confirm
 *
 * @details
 *
 *    Function : FreeRicSubsModConfirm
 *
 * Functionality: Free memory for RIC subscription modification
 *    confirm
 *
 * @param E2AP Message PDU to be freed
 * @return Void
 *
 ******************************************************************/
void FreeRicSubsModConfirm(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx = 0, arrIdx=0;
   RICsubscriptionModificationConfirm_t *ricSubsModCfm = NULLP;
   RICsubscriptionModificationConfirm_IEs_t *ricSubsModCfmIe = NULLP;
   RICactions_ConfirmedForModification_List_t *modCfmList = NULLP;
   RICactions_RefusedToBeModified_List_t *modRefusedList = NULLP;
   RICactions_ConfirmedForRemoval_List_t *rmvCfmList = NULLP;
   RICactions_RefusedToBeRemoved_List_t *rmvFailList = NULLP;

   if(e2apMsg)
   {
      if(e2apMsg->choice.successfulOutcome)
      {
         ricSubsModCfm = &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionModificationConfirm;
         if(ricSubsModCfm->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < ricSubsModCfm->protocolIEs.list.count; ieIdx++)
            {
               if(ricSubsModCfm->protocolIEs.list.array[ieIdx])
               {
                  ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
                  switch(ricSubsModCfmIe->id)
                  {
                     case ProtocolIE_IDE2_id_RICactionsConfirmedForModification_List:
                        {
                           modCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForModification_List;
                           if(modCfmList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < modCfmList->list.count; arrIdx++)
                              {
                                 RIC_FREE(modCfmList->list.array[arrIdx], \
                                    sizeof(RICaction_ConfirmedForModification_ItemIEs_t));
                              }
                              RIC_FREE(modCfmList->list.array,  modCfmList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_RICactionsRefusedToBeModified_List:
                        {
                           modRefusedList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeModified_List;
                           if(modRefusedList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < modRefusedList->list.count; arrIdx++)
                              {
                                 RIC_FREE(modRefusedList->list.array[arrIdx], \
                                       sizeof(RICaction_RefusedToBeModified_ItemIEs_t));
                              }
                              RIC_FREE(modRefusedList->list.array,  modRefusedList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_RICactionsConfirmedForRemoval_List:
                        {
                           rmvCfmList = &ricSubsModCfmIe->value.choice.RICactions_ConfirmedForRemoval_List;
                           if(rmvCfmList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < rmvCfmList->list.count; arrIdx++)
                              {
                                 RIC_FREE(rmvCfmList->list.array[arrIdx], \
                                       sizeof(RICaction_ConfirmedForRemoval_ItemIEs_t));
                              }
                              RIC_FREE(rmvCfmList->list.array,  rmvCfmList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_RICactionsRefusedToBeRemoved_List:
                        {
                           rmvFailList = &ricSubsModCfmIe->value.choice.RICactions_RefusedToBeRemoved_List;
                           if(rmvFailList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < rmvFailList->list.count; arrIdx++)
                              {
                                 RIC_ALLOC(rmvFailList->list.array[arrIdx], \
                                    sizeof(RICaction_RefusedToBeRemoved_ItemIEs_t));
                              }
                              RIC_FREE(rmvFailList->list.array,  rmvFailList->list.size);
                           }
                           break;
                        }

                     default:
                        break;

                  }
                  RIC_FREE(ricSubsModCfmIe, sizeof(RICsubscriptionModificationConfirm_IEs_t));
               }
            }
            RIC_FREE(ricSubsModCfm->protocolIEs.list.array, ricSubsModCfm->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome , sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Fill the list of actions confirmed for modification
 *
 * @details
 *
 *    Function : fillActionModConfirmedList
 *
 * Functionality: Fill the list of actions confirmed for modification
 *
 * @param List to be filled
 *        Number of actions
 *        Source list of actions
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillActionModConfirmedList(RICactions_ConfirmedForModification_List_t *modCfmList, uint8_t numActions, \
   uint8_t *actionModifiedList)
{
   uint8_t arrIdx = 0;
   RICaction_ConfirmedForModification_ItemIEs_t *modCfmListItem = NULLP;

   modCfmList->list.count = numActions;
   modCfmList->list.size = numActions * sizeof(RICaction_ConfirmedForModification_ItemIEs_t *);
   RIC_ALLOC(modCfmList->list.array,  modCfmList->list.size);
   if(!modCfmList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx < modCfmList->list.count; arrIdx++)
   {
      RIC_ALLOC(modCfmList->list.array[arrIdx], sizeof(RICaction_ConfirmedForModification_ItemIEs_t));
      if(!modCfmList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         return RFAILED;
      }

      modCfmListItem = (RICaction_ConfirmedForModification_ItemIEs_t *)modCfmList->list.array[arrIdx];
      modCfmListItem->id = ProtocolIE_IDE2_id_RICaction_ConfirmedForModification_Item;
      modCfmListItem->criticality = CriticalityE2_ignore;
      modCfmListItem->value.present = \
         RICaction_ConfirmedForModification_ItemIEs__value_PR_RICaction_ConfirmedForModification_Item;
      modCfmListItem->value.choice.RICaction_ConfirmedForModification_Item.ricActionID = actionModifiedList[arrIdx];
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill the list of actions refused to be modified
 *
 * @details
 *
 *    Function : fillActionModRefusedList
 *
 * Functionality: Fill the list of actions refused to be modified
 *
 * @param List to be filled
 *        Number of list
 *        Source list of actions refused tobe modified
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillActionModRefusedList(RICactions_RefusedToBeModified_List_t *modRefusedList, uint8_t numActions, \
   ActionFailed *actionModFailedList)
{
   uint8_t arrIdx = 0;
   RICaction_RefusedToBeModified_ItemIEs_t *modRefusedListItem = NULLP;

   modRefusedList->list.count = numActions;
   modRefusedList->list.size = numActions * sizeof(RICaction_RefusedToBeModified_ItemIEs_t *);
   RIC_ALLOC(modRefusedList->list.array,  modRefusedList->list.size);
   if(!modRefusedList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx < modRefusedList->list.count; arrIdx++)
   {
      RIC_ALLOC(modRefusedList->list.array[arrIdx], sizeof(RICaction_RefusedToBeModified_ItemIEs_t));
      if(!modRefusedList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         return RFAILED;
      }

      modRefusedListItem = (RICaction_RefusedToBeModified_ItemIEs_t *)modRefusedList->list.array[arrIdx];
      modRefusedListItem->id = ProtocolIE_IDE2_id_RICaction_RefusedToBeModified_Item;
      modRefusedListItem->criticality = CriticalityE2_ignore;
      modRefusedListItem->value.present = \
         RICaction_RefusedToBeModified_ItemIEs__value_PR_RICaction_RefusedToBeModified_Item;
      modRefusedListItem->value.choice.RICaction_RefusedToBeModified_Item.ricActionID = \
         actionModFailedList[arrIdx].actionId;
      fillE2FailureCause(&modRefusedListItem->value.choice.RICaction_RefusedToBeModified_Item.cause, \
         actionModFailedList[arrIdx].failureType, actionModFailedList[arrIdx].cause);
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill the list of action confirmed for removal
 *
 * @details
 *
 *    Function : fillActionRemovalConfirmedList
 *
 * Functionality: Fill the list of action confirmed for removal
 *
 * @param List to be filled
 *        Number of actions
 *        Source list of actions removed
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillActionRemovalConfirmedList(RICactions_ConfirmedForRemoval_List_t *rmvCfmList, uint8_t numActions, \
   uint8_t *actionRemovedList)
{
   uint8_t arrIdx = 0;
   RICaction_ConfirmedForRemoval_ItemIEs_t *rmvCfmListItem = NULLP;

   rmvCfmList->list.count = numActions;
   rmvCfmList->list.size = numActions * sizeof(RICaction_ConfirmedForRemoval_ItemIEs_t *);
   RIC_ALLOC(rmvCfmList->list.array,  rmvCfmList->list.size);
   if(!rmvCfmList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx < rmvCfmList->list.count; arrIdx++)
   {
      RIC_ALLOC(rmvCfmList->list.array[arrIdx], sizeof(RICaction_ConfirmedForRemoval_ItemIEs_t));
      if(!rmvCfmList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         return RFAILED;
      }

      rmvCfmListItem = (RICaction_ConfirmedForRemoval_ItemIEs_t *)rmvCfmList->list.array[arrIdx];
      rmvCfmListItem->id = ProtocolIE_IDE2_id_RICaction_ConfirmedForRemoval_Item;
      rmvCfmListItem->criticality = CriticalityE2_ignore;
      rmvCfmListItem->value.present = \
         RICaction_ConfirmedForRemoval_ItemIEs__value_PR_RICaction_ConfirmedForRemoval_Item;
      rmvCfmListItem->value.choice.RICaction_ConfirmedForRemoval_Item.ricActionID = actionRemovedList[arrIdx];
   }

   return ROK;
}

/*******************************************************************
 *
 * @brief Fill the list of actions refused to be removed
 *
 * @details
 *
 *    Function : fillActionRemovalRefusedList
 *
 * Functionality: Fill the list of actions refused to be removed
 *
 * @param List to be filled
 *        Number of list
 *        Source list of actions refused to be removed
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t fillActionRemovalRefusedList(RICactions_RefusedToBeRemoved_List_t *rmvFailList, \
   uint8_t numActions, ActionFailed *actionRmvlFailList)
{
   uint8_t arrIdx = 0;
   RICaction_RefusedToBeRemoved_ItemIEs_t *rmvFailListItem = NULLP;

   rmvFailList->list.count = numActions;
   rmvFailList->list.size = numActions * sizeof(RICaction_RefusedToBeRemoved_ItemIEs_t *);
   RIC_ALLOC(rmvFailList->list.array,  rmvFailList->list.size);
   if(!rmvFailList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx < rmvFailList->list.count; arrIdx++)
   {
      RIC_ALLOC(rmvFailList->list.array[arrIdx], sizeof(RICaction_RefusedToBeRemoved_ItemIEs_t));
      if(!rmvFailList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         return RFAILED;
      }

      rmvFailListItem = (RICaction_RefusedToBeRemoved_ItemIEs_t *)rmvFailList->list.array[arrIdx];
      rmvFailListItem->id = ProtocolIE_IDE2_id_RICaction_RefusedToBeRemoved_Item;
      rmvFailListItem->criticality = CriticalityE2_ignore;
      rmvFailListItem->value.present = \
         RICaction_RefusedToBeRemoved_ItemIEs__value_PR_RICaction_RefusedToBeRemoved_Item;
      rmvFailListItem->value.choice.RICaction_RefusedToBeRemoved_Item.ricActionID = actionRmvlFailList[arrIdx].actionId;
      fillE2FailureCause(&rmvFailListItem->value.choice.RICaction_RefusedToBeRemoved_Item.cause, \
         actionRmvlFailList[arrIdx].failureType, actionRmvlFailList[arrIdx].cause);
   }

   return ROK;

}

/*******************************************************************
 *
 * @brief Build And Send RIC Subscription Modification Confirm
 *
 * @details
 *
 *    Function : BuildAndSendRicSubsModConfirm
 *
 * Functionality: Build And Send RIC Subscription Modification Confirm
 *
 * @param DU ID
 *        RIC Request ID of subscription
 *        RAN Function ID
 *        Temporary source action list
 * @return ROK - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendRicSubsModConfirm(uint32_t duId, RicRequestId ricReqId, uint16_t ranFuncId, RicTmpActionList tmpActionList)
{
   uint8_t ieIdx = 0, elementCnt = 0;
   uint8_t ret = RFAILED;
   E2AP_PDU_t *e2apMsg = NULLP;
   asn_enc_rval_t encRetVal;
   RICsubscriptionModificationConfirm_t *ricSubsModCfm = NULLP;
   RICsubscriptionModificationConfirm_IEs_t *ricSubsModCfmIe = NULLP;

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Modification Confirm\n");
   while(true)
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }

      /* Successful Outcome */
      e2apMsg->present =  E2AP_PDU_PR_successfulOutcome;
      RIC_ALLOC(e2apMsg->choice.successfulOutcome , sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_RICsubscriptionModificationRequired;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = \
         SuccessfulOutcomeE2__value_PR_RICsubscriptionModificationConfirm;
      ricSubsModCfm = &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionModificationConfirm;

      elementCnt = 2;
      if(tmpActionList.numActionModified)
         elementCnt++;
      if(tmpActionList.numActionModFailed)
         elementCnt++;
      if(tmpActionList.numActionRemoved)
         elementCnt++;
      if(tmpActionList.numActionRemovalFailed)
         elementCnt++;

      ricSubsModCfm->protocolIEs.list.count = elementCnt;
      ricSubsModCfm->protocolIEs.list.size = elementCnt * sizeof(RICsubscriptionModificationConfirm_IEs_t *);
      RIC_ALLOC(ricSubsModCfm->protocolIEs.list.array, ricSubsModCfm->protocolIEs.list.size);
      if(!ricSubsModCfm->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(ricSubsModCfm->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionModificationConfirm_IEs_t));
         if(!ricSubsModCfm->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for %s at line %d", __func__, __LINE__);
            break;
         }
      }
      
      /* RIC Request ID */
      ieIdx = 0;
      ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
      ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubsModCfmIe->criticality = CriticalityE2_reject;
      ricSubsModCfmIe->value.present = RICsubscriptionModificationConfirm_IEs__value_PR_RICrequestID;
      ricSubsModCfmIe->value.choice.RICrequestID.ricRequestorID = ricReqId.requestorId;
      ricSubsModCfmIe->value.choice.RICrequestID.ricInstanceID = ricReqId.instanceId;

      /* RAN Function ID */
      ieIdx++;
      ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
      ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubsModCfmIe->criticality = CriticalityE2_reject;
      ricSubsModCfmIe->value.present = RICsubscriptionModificationConfirm_IEs__value_PR_RANfunctionID;
      ricSubsModCfmIe->value.choice.RANfunctionID = ranFuncId;

      /* RIC Actions List confirmed for modification */
      if(tmpActionList.numActionModified)
      {
         ieIdx++;
         ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
         ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RICactionsConfirmedForModification_List;
         ricSubsModCfmIe->criticality = CriticalityE2_ignore;
         ricSubsModCfmIe->value.present = \
            RICsubscriptionModificationConfirm_IEs__value_PR_RICactions_ConfirmedForModification_List;
         if(fillActionModConfirmedList(&ricSubsModCfmIe->value.choice.RICactions_ConfirmedForModification_List, \
            tmpActionList.numActionModified, tmpActionList.actionModifiedList) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill RIC Actions Confirmed for Modification List", __func__);
            break;
         }
      }

      /* RIC Actions List refured to be modified */
      if(tmpActionList.numActionModFailed)
      {
         ieIdx++;
         ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
         ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RICactionsRefusedToBeModified_List;
         ricSubsModCfmIe->criticality = CriticalityE2_ignore;
         ricSubsModCfmIe->value.present = \
            RICsubscriptionModificationConfirm_IEs__value_PR_RICactions_RefusedToBeModified_List;
         if(fillActionModRefusedList(&ricSubsModCfmIe->value.choice.RICactions_RefusedToBeModified_List, \
            tmpActionList.numActionModFailed, tmpActionList.actionModFailedList) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill RIC Actions Refused to be Modified List", __func__);
            break;
         }
      }

      /* RIC Actions List confirmed for removal */
      if(tmpActionList.numActionRemoved)
      {
         ieIdx++;
         ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
         ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RICactionsConfirmedForRemoval_List;
         ricSubsModCfmIe->criticality = CriticalityE2_ignore;
         ricSubsModCfmIe->value.present = \
            RICsubscriptionModificationConfirm_IEs__value_PR_RICactions_ConfirmedForRemoval_List;
         if(fillActionRemovalConfirmedList(&ricSubsModCfmIe->value.choice.RICactions_ConfirmedForRemoval_List, \
            tmpActionList.numActionRemoved, tmpActionList.actionRemovedList) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill RIC Actions Confirmed for Removal List", __func__);
            break;
         }
      }

      /* RIC Actions List Refused to be removed */
      if(tmpActionList.numActionRemovalFailed)
      {
         ieIdx++;
         ricSubsModCfmIe = ricSubsModCfm->protocolIEs.list.array[ieIdx];
         ricSubsModCfmIe->id = ProtocolIE_IDE2_id_RICactionsRefusedToBeRemoved_List;
         ricSubsModCfmIe->criticality = CriticalityE2_ignore;
         ricSubsModCfmIe->value.present = \
            RICsubscriptionModificationConfirm_IEs__value_PR_RICactions_RefusedToBeRemoved_List;
         if(fillActionRemovalRefusedList(&ricSubsModCfmIe->value.choice.RICactions_RefusedToBeRemoved_List, \
            tmpActionList.numActionRemovalFailed, tmpActionList.actionRemovalFailedList) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Failed to fill RIC Actions Failed to be Removed List", __func__);
            break;
         }
      }

      /* Print and encode E2AP Message PDU */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);

      /* Check encode results */
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC subscription modification confirm (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC subscription modification confirm\n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send RIC Subscription Modification Confirm");
         break;
      }

      ret =ROK;
      break;
   }

   FreeRicSubsModConfirm(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Processing of RIC Subscription Modification Required
 *
 * @details
 *
 *    Function : ProcRicSubsModReqd
 *
 * Functionality: Processing of RIC Subscription Modification Required
 *    As of now, we do not identify any scenario where this message
 *    shall be sent by DU. Hence, bare minimum handling has been
 *    done here.
 *
 * @param  DU ID
 *         RIC Subscription Modification Required IEs
 * @return ROK-success
 *         RFAILED-failure
 *
 ******************************************************************/
uint8_t ProcRicSubsModReqd(uint32_t duId, RICsubscriptionModificationRequired_t *ricSubsModReqd)
{
   uint8_t ieIdx = 0, actionIdx = 0, duIdx = 0;
   DuDb    *duDb = NULLP;
   uint16_t ranFuncId;
   uint16_t actionId;
   RicRequestId ricReqId;
   RanFunction *ranFuncDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   ActionInfo *action = NULLP;
   RICsubscriptionModificationRequired_IEs_t *ricSubsModReqdIe = NULLP;
   RICactions_RequiredToBeModified_List_t *actionToBeModList = NULLP;
   RICactions_RequiredToBeRemoved_List_t  *actionToBeRmvList = NULLP;
   RICaction_RequiredToBeModified_ItemIEs_t *actionToBeMod = NULLP;
   RICaction_RequiredToBeRemoved_ItemIEs_t *actionToBeRmv = NULLP;
   RicTmpActionList tmpActionList;

   memset(&ricReqId, 0, sizeof(RicRequestId));
   memset(&tmpActionList, 0, sizeof(RicTmpActionList));

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return RFAILED;
   }

   for(ieIdx = 0; ieIdx < ricSubsModReqd->protocolIEs.list.count; ieIdx++)
   {
      ricSubsModReqdIe = ricSubsModReqd->protocolIEs.list.array[ieIdx];
      switch(ricSubsModReqdIe->id)
      {
         case ProtocolIE_IDE2_id_RICrequestID:
            {
               ricReqId.requestorId = ricSubsModReqdIe->value.choice.RICrequestID.ricRequestorID;
               ricReqId.instanceId = ricSubsModReqdIe->value.choice.RICrequestID.ricInstanceID;
               break;
            }
         case ProtocolIE_IDE2_id_RANfunctionID:
            {
               ranFuncId = ricSubsModReqdIe->value.choice.RANfunctionID;
               ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncId);
               if(!ranFuncDb)
               {
                  /* If RIC Subscription not found, send RIC Subscription modification refuse to DU */
                  DU_LOG("\nERROR  -->  E2AP : ProcRicSubsModReqd: RIC Subscription not found");
                  BuildAndSendRicSubsModRefuse(duId, ricReqId, ranFuncId, CauseE2_PR_ricRequest, \
                     CauseE2RICrequest_ran_function_id_invalid);
                  return RFAILED;
               }

               ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb);
               if(!ricSubs)
               {
                  /* If RAN Function not found, send RIC Subscription modification refuse to DU */
                  DU_LOG("\nERROR  -->  E2AP : ProcRicSubsModReqd: RAN Function ID [%d] not found",ranFuncId);
                  BuildAndSendRicSubsModRefuse(duId, ricReqId, ranFuncId, \
                     CauseE2_PR_ricRequest, CauseE2RICrequest_request_id_unknown);
                  return RFAILED; 
               }
               break;
            }
         case ProtocolIE_IDE2_id_RICactionsRequiredToBeModified_List:
            {
               actionToBeModList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeModified_List;
               for(actionIdx = 0; actionIdx < actionToBeModList->list.count; actionIdx++)
               {
                  actionToBeMod = (RICaction_RequiredToBeModified_ItemIEs_t *)actionToBeModList->list.array[actionIdx];
                  actionId = actionToBeMod->value.choice.RICaction_RequiredToBeModified_Item.ricActionID;
                  action = fetchActionInfoFromActionId(actionId, ricSubs);
                  if(action)
                  {
                     /* No modification required as of now, hence directly adding to the list */
                     tmpActionList.actionModifiedList[tmpActionList.numActionModified++] = actionId;
                  }
                  else
                  {
                     tmpActionList.actionModFailedList[tmpActionList.numActionModFailed].actionId = actionId;
                     tmpActionList.actionModFailedList[tmpActionList.numActionModFailed].failureType = \
                        CauseE2_PR_ricRequest;
                     tmpActionList.actionModFailedList[tmpActionList.numActionModFailed].cause = \
                        CauseE2RICrequest_action_not_supported;
                     tmpActionList.numActionModFailed++;
                  }
               }
               break;
            }
         case ProtocolIE_IDE2_id_RICactionsRequiredToBeRemoved_List:
            {
               actionToBeRmvList = &ricSubsModReqdIe->value.choice.RICactions_RequiredToBeRemoved_List;
               for(actionIdx = 0; actionIdx < actionToBeRmvList->list.count; actionIdx++)
               {
                  actionToBeRmv = (RICaction_RequiredToBeRemoved_ItemIEs_t *)actionToBeRmvList->list.array[actionIdx];
                  actionId = actionToBeRmv->value.choice.RICaction_RequiredToBeRemoved_Item.ricActionID;
                  action = fetchActionInfoFromActionId(actionId, ricSubs);
                  if(action)
                  {
                     tmpActionList.actionRemovedList[tmpActionList.numActionRemoved++] = actionId;
                     memset(action, 0, sizeof(ActionInfo));
                     action->actionId = -1;
                     ricSubs->numOfActions--;
                  }
               }
               break;
            }
         default:
            break;
      }
   }

   /* If none of the action modification/removal is supported, 
    *   send RIC Subscription Modification Refuse
    * Else
    *   send RIC Subscription Modification Confirm
    */
   if(tmpActionList.numActionModified || tmpActionList.numActionRemoved)
   {
      BuildAndSendRicSubsModConfirm(duId, ricReqId, ranFuncId, tmpActionList);
   }
   else
   {
      BuildAndSendRicSubsModRefuse(duId, ricReqId, ranFuncId, CauseE2_PR_ricRequest, \
            CauseE2RICrequest_action_not_supported);
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Free the ErrorIndication Message
 *
 * @details
 *
 *    Function : FreeRicIndication
 *
 * Functionality: Free the ErrorIndication Message
 *
 * @return void
 *
 *
 ******************************************************************/
void FreeErrorIndication(E2AP_PDU_t  *e2apMsg)
{
   uint8_t arrIdx = 0;
   ErrorIndicationE2_t *errorIndicationMsg= NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         errorIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.ErrorIndicationE2;
         if(errorIndicationMsg!= NULLP)
         {
            if(errorIndicationMsg->protocolIEs.list.array != NULLP)
            {
               for(arrIdx=0; arrIdx<errorIndicationMsg->protocolIEs.list.count; arrIdx++)
               {
                  RIC_FREE(errorIndicationMsg->protocolIEs.list.array[arrIdx],sizeof(ErrorIndicationE2_t));
               }
               RIC_FREE(errorIndicationMsg->protocolIEs.list.array,errorIndicationMsg->protocolIEs.list.size);
            }
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}


/*******************************************************************
 *
 * @brief Builds and Send the ErrorIndication Message
 *
 * @details
 *
 *    Function : BuildAndSendErrorIndication
 *
 * Functionality:Fills the ErrorIndication Message
 *
 * @params[in] 
 *    DU id
 *    Trans id
 *    Ric req id
 *    Ran function id
 *    Reason of failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendErrorIndication(uint32_t duId, int8_t transId, RicRequestId requestId, uint16_t ranFuncId, uint8_t reason)
{
   uint8_t elementCnt =0, arrIdx=0, ret = RFAILED;
   E2AP_PDU_t         *e2apMsg = NULLP;
   ErrorIndicationE2_t *errorIndicationMsg=NULLP;
   asn_enc_rval_t     encRetVal;        /* Encoder return value */

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building Error Indication Message\n");

      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed in %s at line %d",__func__, __LINE__);
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed in %s at line %d",__func__, __LINE__);
         break;
      }
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_ErrorIndicationE2;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_ErrorIndicationE2;

      errorIndicationMsg = &e2apMsg->choice.initiatingMessage->value.choice.ErrorIndicationE2;

      /* Element count is 2 for TransactionID/RICrequestID and Cause.
       * If the RAN function id is present, the count will be increased.*/
      elementCnt = 2;
      if(ranFuncId>0)
         elementCnt++;

      errorIndicationMsg->protocolIEs.list.count = elementCnt;
      errorIndicationMsg->protocolIEs.list.size = elementCnt * sizeof(ErrorIndicationE2_IEs_t*);

      /* Initialize the E2Setup members */
      RIC_ALLOC(errorIndicationMsg->protocolIEs.list.array, errorIndicationMsg->protocolIEs.list.size);
      if(errorIndicationMsg->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array elements in %s at line %d",__func__, __LINE__);
         break;
      }
      for(arrIdx = 0; arrIdx < elementCnt; (arrIdx)++)
      {
         RIC_ALLOC(errorIndicationMsg->protocolIEs.list.array[arrIdx], sizeof(ErrorIndicationE2_IEs_t));
         if(errorIndicationMsg->protocolIEs.list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for array Idx %d in %s at line %d",arrIdx,__func__, __LINE__);
            break;
         }
      }
      if(arrIdx < elementCnt)
         break;

      arrIdx = 0;

      if(transId >=0 && transId<=255)
      {
         /* TransactionID */
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_TransactionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;
      }
      else
      {
         /* RICrequestID */
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RICrequestID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_RICrequestID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RICrequestID.ricRequestorID = requestId.requestorId;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RICrequestID.ricInstanceID = requestId.instanceId;
      }
      
      if(ranFuncId>0)
      {
         /* RAN Function ID */
         arrIdx++;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_RANfunctionID;
         errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionID = ranFuncId;
      }
     
      /* Cause */
      arrIdx++;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_ignore;
      errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.present = ErrorIndicationE2_IEs__value_PR_CauseE2;
      fillE2FailureCause(&errorIndicationMsg->protocolIEs.list.array[arrIdx]->value.choice.CauseE2, CauseE2_PR_misc, reason);


      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode Error Indication Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for Error Indication Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nINFO   -->  E2AP : Sending Error Indication Message");

      }
      ret = ROK;
      break;
   }
   FreeErrorIndication(e2apMsg);
   return ret;
}

/*******************************************************************
*
* @brief Handles received E2AP message and sends back response  
*
* @details
*
*    Function : E2APMsgHdlr
*
*    Functionality:
*         - Decodes received E2AP control message
*         - Prepares response message, encodes and sends to SCTP
*
* @params[in] 
* @return ROK     - success
*         RFAILED - failure
*
* ****************************************************************/
void E2APMsgHdlr(uint32_t *duId, Buffer *mBuf)
{
   int             i;
   char            *recvBuf;
   MsgLen          copyCnt;
   MsgLen          recvBufLen;
   E2AP_PDU_t      *e2apMsg;
   asn_dec_rval_t  rval; /* Decoder return value */
   E2AP_PDU_t      e2apasnmsg ;
 
   DU_LOG("\nINFO  -->  E2AP : Received E2AP message buffer");
   ODU_PRINT_MSG(mBuf, 0,0);
 
   /* Copy mBuf into char array to decode it */
   ODU_GET_MSG_LEN(mBuf, &recvBufLen);
   RIC_ALLOC(recvBuf, (Size)recvBufLen);

   if(recvBuf == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed");
      return;
   }
   if(ODU_COPY_MSG_TO_FIX_BUF(mBuf, 0, recvBufLen, (Data *)recvBuf, &copyCnt) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed while copying %d", copyCnt);
      return;
   }

   DU_LOG("\nDEBUG  -->  E2AP : Received flat buffer to be decoded : ");
   for(i=0; i< recvBufLen; i++)
   {
        DU_LOG("%x",recvBuf[i]);
   }

   /* Decoding flat buffer into E2AP messsage */
   e2apMsg = &e2apasnmsg;
   memset(e2apMsg, 0, sizeof(E2AP_PDU_t));

   rval = aper_decode(0, &asn_DEF_E2AP_PDU, (void **)&e2apMsg, recvBuf, recvBufLen, 0, 0);
   RIC_FREE(recvBuf, (Size)recvBufLen);

   if(rval.code == RC_FAIL || rval.code == RC_WMORE)
   {
      DU_LOG("\nERROR  -->  E2AP : ASN decode failed");
      return;
   }
   DU_LOG("\n");
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

   switch(e2apMsg->present)
   {
      case E2AP_PDU_PR_initiatingMessage:
         {
            switch(e2apMsg->choice.initiatingMessage->value.present)
            {
               case InitiatingMessageE2__value_PR_E2setupRequest:
                  {
                     DU_LOG("\nINFO  -->  E2AP : E2 setup request received");
                     ProcE2SetupReq(duId, &e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest);
                     break;
                  }
               case InitiatingMessageE2__value_PR_E2nodeConfigurationUpdate:
                  {
                     DU_LOG("\nINFO  -->  E2AP : E2 node config update received");
                     ProcE2NodeConfigUpdate(*duId, &e2apMsg->choice.initiatingMessage->value.choice.E2nodeConfigurationUpdate);
                     break;
                  }
               case InitiatingMessageE2__value_PR_ResetRequestE2:
                  {
                     DU_LOG("\nINFO  -->  E2AP : E2 Reset Request received");
                     ProcE2ResetReq(*duId,  &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2);
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICindication:
                  {
                     DU_LOG("\nINFO  -->  E2AP : RIC Indication received");
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICserviceUpdate:
                  {
                     DU_LOG("\nINFO  -->  E2AP : RIC Service update received");
                     ProcRicServiceUpdate(*duId,  &e2apMsg->choice.initiatingMessage->value.choice.RICserviceUpdate);
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICsubscriptionModificationRequired:
                  {
                     DU_LOG("\nINFO  -->  E2AP : RIC Subscription Modification Required");
                     ProcRicSubsModReqd(*duId, \
                           &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionModificationRequired);
                     break;
                  }

               case InitiatingMessageE2__value_PR_ErrorIndicationE2:
                  {
                     DU_LOG("\nINFO  -->  E2AP : Error indication received");
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of intiating message [%d]", \
                        e2apMsg->choice.initiatingMessage->value.present);
                     return;
                  }
            }/* End of switch(initiatingMessage) */
            break;
         }
      case E2AP_PDU_PR_successfulOutcome: 
         {
            switch(e2apMsg->choice.successfulOutcome->value.present)
            {
               case SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse:  
                  {
                     ProcRicSubscriptionResponse(*duId, \
                        &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of successfulOutcome message [%d]", \
                        e2apMsg->choice.successfulOutcome->value.present);
                     return;
                  }
                  break;
            }
            break; 
         }
         case E2AP_PDU_PR_unsuccessfulOutcome:
         {
            switch(e2apMsg->choice.successfulOutcome->value.present)
            {
               case UnsuccessfulOutcomeE2__value_PR_RICsubscriptionFailure:
                  {
                     ProcRicSubscriptionFailure(*duId, \
                        &e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionFailure);
                     break;
                  }
               default:
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid type of unsuccessfulOutcome message [%d]", \
                        e2apMsg->choice.unsuccessfulOutcome->value.present);
                     return;
                  }
            }
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  E2AP : Invalid type message type ");
            return;
         }

   }/* End of switch(e2apMsg->present) */
} /* End of E2APMsgHdlr */


/**********************************************************************
  End of file
 **********************************************************************/
