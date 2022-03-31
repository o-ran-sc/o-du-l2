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
#include "E2AP-PDU.h"
#include "du_log.h"
#include "E2nodeComponentInterfaceF1.h"


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

uint8_t SendE2APMsg(Region region, Pool pool)
{
   Buffer *mBuf;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);
 
         if(sctpSend(mBuf) != ROK)
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
      buildPlmnId(ricCfgParams.plmn , ricId->pLMN_Identity.buf);
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
   uint8_t arrIdx = 0;
   E2setupResponse_t  *e2SetupRsp;

   if(e2apMsg)
   {
      if(e2apMsg->choice.successfulOutcome)
      {
         e2SetupRsp = &e2apMsg->choice.successfulOutcome->value.choice.E2setupResponse;
         if(e2SetupRsp->protocolIEs.list.array)
         {
            for(arrIdx=0; arrIdx<e2SetupRsp->protocolIEs.list.count; arrIdx++)
            {
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
uint8_t BuildE2nodeComponentConfigAdditionAck(E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList, uint8_t duId)
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
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentInterfaceType = E2nodeComponentInterfaceType_f1;

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
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]  = duId;
   
   /* >E2 Node Component Configuration Acknowledge*/
   e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item.e2nodeComponentConfigurationAck.updateOutcome = \
   E2nodeComponentConfigurationAck__updateOutcome_success;
   
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
uint8_t BuildAndSendE2SetupRsp(uint8_t duId)
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
      e2SetupRsp->protocolIEs.list.count = elementCnt;
      e2SetupRsp->protocolIEs.list.size  = elementCnt * sizeof(E2setupResponseIEs_t);

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
      e2SetupRsp->protocolIEs.list.array[idx]->value.choice.TransactionID  = TRANS_ID;

      /* Global RIC ID */
      idx++;
      e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_GlobalRIC_ID;
      e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                                               E2setupResponseIEs__value_PR_GlobalRIC_ID;

      if(BuildGlobalRicId(&(e2SetupRsp->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID))!=ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build Global Ric Id");
         break;
      }
      
      /* E2 Node Component Configuration Addition Acknowledge List*/
      idx++;
      e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck;
      e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = \
      E2setupResponseIEs__value_PR_E2nodeComponentConfigAdditionAck_List;
      if(BuildE2nodeComponentConfigAdditionAck(&e2SetupRsp->protocolIEs.list.array[idx]->value.choice.E2nodeComponentConfigAdditionAck_List, duId)!=ROK)
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

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending E2 Setup Response failed");      
         break;   
      }
      break;
   }

   FreeE2SetupRsp(e2apMsg);
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds Ric Request Id
 *
 * @details
 *
 *    Function : BuildRicRequestId
 *
 *    Functionality: Building the Ric Request Id
 *
 * @params[in] RICrequestID_t *ricReqId
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildRicRequestId(RICrequestID_t *ricReqId)
{

   if(ricReqId != NULLP)
   {
      ricReqId->ricRequestorID = 1;
      ricReqId->ricInstanceID  = 1;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Fills Ric Action To be Setup Item
 *
 * @details
 *
 *    Function : fillSetupItems
 *
 *    Functionality: Filling ricAction Id, RicActionType
 *
 * @params[in] RICaction_ToBeSetup_Item_t *setupItems
 * @return pointer of type RICaction_ToBeSetup_Item_t
 *
 * ****************************************************************/

RICaction_ToBeSetup_Item_t* fillSetupItems(RICaction_ToBeSetup_Item_t *setupItems)
{
   if(setupItems != NULLP)
   {
      setupItems->ricActionID = 0;
      setupItems->ricActionType = RICactionType_report;
   }

   return (setupItems);
}

/*******************************************************************
 *
 * @brief Fills RIC Subscription Details Item List
 *
 * @details
 *
 *    Function : fillSubsDetails
 *
 *    Functionality: Fill the RIC Subscription Details Items List
 *
 * @params[in] RICaction_ToBeSetup_ItemIEs_t *items
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillSubsDetails(RICaction_ToBeSetup_ItemIEs_t *items)
{
   if(items != NULLP)
   {
      items->id = ProtocolIE_IDE2_id_RICaction_ToBeSetup_Item;
      items->criticality   =  CriticalityE2_ignore;
      items->value.present =  RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;
      fillSetupItems(&(items->value.choice.RICaction_ToBeSetup_Item));
   }
   return ROK;
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
 * @params[in] RICsubscriptionDetails_t *subsDetails
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildRicSubsDetails(RICsubscriptionDetails_t *subsDetails)
{

   uint8_t elementCnt;

   if(subsDetails != NULLP)
   {
      /* Octet string to be build here */
      /* Sending PLMN as Octect string */
      uint8_t byteSize = 3;
      subsDetails->ricEventTriggerDefinition.size = byteSize * sizeof(uint8_t);
      RIC_ALLOC(subsDetails->ricEventTriggerDefinition.buf,  subsDetails->ricEventTriggerDefinition.size);
      buildPlmnId(ricCfgParams.plmn, subsDetails->ricEventTriggerDefinition.buf);
      elementCnt = 1;
      subsDetails->ricAction_ToBeSetup_List.list.count = elementCnt;
      subsDetails->ricAction_ToBeSetup_List.list.size = \
                      elementCnt * sizeof(RICaction_ToBeSetup_ItemIEs_t);
      RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array, \
                subsDetails->ricAction_ToBeSetup_List.list.size);
      if(subsDetails->ricAction_ToBeSetup_List.list.array  == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICactionToBeSetup Items failed");
         return RFAILED;
      } 
      RIC_ALLOC(subsDetails->ricAction_ToBeSetup_List.list.array[0],\
                   sizeof(RICaction_ToBeSetup_ItemIEs_t));
      fillSubsDetails(subsDetails->ricAction_ToBeSetup_List.list.array[0]);
   }
   return ROK;
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

uint8_t BuildAndSendRicSubscriptionReq()
{

   E2AP_PDU_t                 *e2apRicMsg = NULL;
   RICsubscriptionRequest_t   *ricSubscriptionReq;
   uint8_t         elementCnt;
   uint8_t         idx;
   uint8_t         ieId;
   uint8_t         ret; 
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   ricCfgParams.ricSubsStatus = TRUE;

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Request\n");

   RIC_ALLOC(e2apRicMsg, sizeof(E2AP_PDU_t));
   if(e2apRicMsg == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
      return RFAILED;
   }

   e2apRicMsg->present = E2AP_PDU_PR_initiatingMessage;
   RIC_ALLOC(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
   if(e2apRicMsg->choice.initiatingMessage == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
      RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
      return RFAILED;
   }
   e2apRicMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscription;
   e2apRicMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
   e2apRicMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionRequest;
   
   RIC_ALLOC(ricSubscriptionReq, sizeof(RICsubscriptionRequest_t));
   ricSubscriptionReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionRequest;
   
   elementCnt = 3;
   ricSubscriptionReq->protocolIEs.list.count = elementCnt;
   ricSubscriptionReq->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionRequest_IEs_t);

   /* Initialize the subscription members */
   RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array, \
              ricSubscriptionReq->protocolIEs.list.size);
   if(ricSubscriptionReq->protocolIEs.list.array == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation for RICSubscriptionRequestIEs failed");
      RIC_FREE(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      RIC_FREE(e2apRicMsg, (Size)sizeof(E2AP_PDU_t));
      return RFAILED;
   }
   
   for(idx=0; idx<elementCnt; idx++)
   {
      RIC_ALLOC(ricSubscriptionReq->protocolIEs.list.array[idx],\
            sizeof(RICsubscriptionRequest_IEs_t));
      if(ricSubscriptionReq->protocolIEs.list.array[idx] == NULLP)
      {
         for(ieId=0; ieId<idx; ieId++)
         {
            RIC_FREE(ricSubscriptionReq->protocolIEs.list.array[ieId],\
                  sizeof(RICsubscriptionRequest_IEs_t));
         }
         RIC_FREE(ricSubscriptionReq->protocolIEs.list.array,\
                  ricSubscriptionReq->protocolIEs.list.size);
         RIC_FREE(e2apRicMsg->choice.initiatingMessage, \
               sizeof(InitiatingMessageE2_t));
         RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
         return RFAILED;
      }
   }

   /* Filling RIC Request Id */
   idx = 0;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RICrequestID;
 
   BuildRicRequestId(&ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICrequestID);


   /* Filling RAN Function Id */
   idx++;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID = 1;


   /* Filling RIC Subscription Details */
   idx++;
   ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICsubscriptionDetails;
   ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
   ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                    RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;

   BuildRicSubsDetails(&(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails));


   /* Prints the Msg formed */
   xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

   memset(encBuf, 0, ENC_BUF_MAX_LEN);
   encBufSize = 0;
   encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf,\
               encBuf);
   if(encRetVal.encoded == ENCODE_FAIL)
   {
      DU_LOG("\nERROR  -->  E2AP : Could not encode RicSubscriptionRequest structure (at %s)\n",\
      encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
      return RFAILED;
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
   if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Sending RIC subscription Request failed");
      return RFAILED;
   }

   return ROK;
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

uint8_t ProcE2SetupReq(E2setupRequest_t  *e2SetupReq)
{
   uint8_t arrIdx = 0, e2NodeAddListIdx =0;;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem;

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
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
                     {
                        e2NodeAddList = &e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAddition_List;      
                        if(e2NodeAddList->list.array)
                        {
                           for(e2NodeAddListIdx = 0; e2NodeAddListIdx< e2NodeAddList->list.count; e2NodeAddListIdx++)
                           {
                              if(e2NodeAddList->list.array[e2NodeAddListIdx])
                              {
                                 e2NodeAddItem = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[e2NodeAddListIdx];
                                 if(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1)
                                 {
                                    if(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->\
                                    gNB_DU_ID.buf)
                                    {
                                       if(BuildAndSendE2SetupRsp(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.\
                                       e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]) !=ROK)
                                       {
                                           DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 setup response");
                                           return RFAILED;
                                       }
                                    }
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
   return ROK;   
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2nodeConfigurationUpdate msg 
 *
 * @details
 *
 *    Function : FreeE2NodeConfigUpdate 
 *
 *    Functionality:
 *       - freeing the memory allocated for E2nodeConfigurationUpdate
 *
 * @params[in] E2AP_PDU_t *e2apMsg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeE2NodeConfigUpdateAck(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx =0;
   E2nodeConfigurationUpdate_t *e2NodeConfigUpdateAck;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         e2NodeConfigUpdateAck = &e2apMsg->choice.successfulOutcome->value.choice.E2nodeConfigurationUpdateAcknowledge;
         if(e2NodeConfigUpdateAck->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < e2NodeConfigUpdateAck->protocolIEs.list.count; arrIdx++)
            {
               RIC_FREE(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdate_t));
            }
            RIC_FREE(e2NodeConfigUpdateAck->protocolIEs.list.array, e2NodeConfigUpdateAck->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 node config update msg 
 *
 * @details
 *
 *    Function : BuildAndSendE2NodeConfigUpdate
 *
 *    Functionality:
 *         - Buld and send the E2 node config update msg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2NodeConfigUpdateAck()
{
   uint8_t arrIdx = 0,elementCnt = 1;
   uint8_t ret = ROK;
   E2AP_PDU_t        *e2apMsg = NULLP;
   E2nodeConfigurationUpdateAcknowledge_t *e2NodeConfigUpdateAck = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Node config update Ack Message\n");
   do
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_successfulOutcome;
      RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for E2AP-PDU failed");
         RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
         return RFAILED;
      }
      
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_E2nodeConfigurationUpdate;
      e2apMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_E2nodeConfigurationUpdateAcknowledge;
      e2NodeConfigUpdateAck = &e2apMsg->choice.successfulOutcome->value.choice.E2nodeConfigurationUpdateAcknowledge;

      e2NodeConfigUpdateAck->protocolIEs.list.count = elementCnt;
      e2NodeConfigUpdateAck->protocolIEs.list.size  = elementCnt * sizeof(E2nodeConfigurationUpdateAcknowledge_IEs_t*);
      /* Initialize the Ric Indication members */
      RIC_ALLOC(e2NodeConfigUpdateAck->protocolIEs.list.array, \
            e2NodeConfigUpdateAck->protocolIEs.list.size);
      if(e2NodeConfigUpdateAck->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation for e2NodeConfigUpdateAck failed");
         break;
      }
      
      for(arrIdx =0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdateAcknowledge_IEs_t));
         if(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx] == NULLP)
         {
            
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for e2NodeConfigUpdateAck failed");
            break;
         }
      }

      arrIdx = 0;
      /* TransactionID */
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_TransactionID;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = TRANS_ID;


      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 Node config update ack structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         return RFAILED;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Node config update ack \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         } 
      }


      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Node config update ack ");
         return RFAILED;
      }

      break;
   }while(true);
   
   FreeE2NodeConfigUpdateAck(e2apMsg);
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
void E2APMsgHdlr(Buffer *mBuf)
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
               ProcE2SetupReq(&e2apMsg->choice.initiatingMessage->value.choice.E2setupRequest);
	            break;
            }
            case InitiatingMessageE2__value_PR_E2nodeConfigurationUpdate:
            {
               DU_LOG("\nINFO  -->  E2AP : E2 node config update received");
               BuildAndSendE2NodeConfigUpdateAck();
               break;
            }
            case InitiatingMessageE2__value_PR_RICindication:
            {
               DU_LOG("\nINFO  -->  E2AP : RIC Indication Acknowledged");
	       break;
            }
            default:
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid type of intiating message [%d]",e2apMsg->choice.initiatingMessage->value.present);
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
               DU_LOG("\nINFO  -->  E2AP : RICsubscriptionResponse Msg Acknowledged");
               break;
            }
            default:
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid type of successfulOutcome message [%d]",e2apMsg->choice.successfulOutcome->value.present);
               return;
            }
            break;
         }
         break; 
      }
      default:
      {
         DU_LOG("\nERROR  -->  E2AP : Invalid type message type ");
         return;
      }
 
    }/* End of switch(e2apMsg->present) */

    if(!ricCfgParams.ricSubsStatus)
      BuildAndSendRicSubscriptionReq(); 
       
} /* End of E2APMsgHdlr */


/**********************************************************************
  End of file
 **********************************************************************/
