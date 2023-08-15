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
   RANfunctionsID_List_t *ranFuncAddedList;
   E2setupResponse_t  *e2SetupRsp;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItemIe;
   E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList;
   E2nodeComponentInterfaceF1_t *f1InterfaceInfo;
   
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
                     ranFuncAddedList= &e2SetupRsp->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                     if(ranFuncAddedList->list.array)
                     {
                        for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
                        {
                           RIC_FREE(ranFuncAddedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                        }
                        RIC_FREE(ranFuncAddedList->list.array, ranFuncAddedList->list.size);
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
 * @brief Build RAN function added list
 *
 * @details
 *
 *    Function : BuildRanFunctionAddedList
 *
 *    Functionality: Build RAN function added list 
 *
 * @params[in] DuDb *duDb, RANfunctionsID_List_t *ranFuncAddedList 
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildRanFunctionAddedList(DuDb *duDb, uint8_t count,  RanFuncInfo *addArr,RANfunctionsID_List_t *ranFuncAddedList)
{
   uint8_t ranFuncIdx = 0;
   RANfunctionID_ItemIEs_t *ranFuncAddedItemIe;
   
   if(!count)
   {
      DU_LOG("\nERROR  -->  E2AP : No RAN function information available");  
      return RFAILED;
   }
   ranFuncAddedList->list.count = count;
   ranFuncAddedList->list.size = ranFuncAddedList->list.count*sizeof(RANfunctionID_ItemIEs_t*);
   RIC_ALLOC(ranFuncAddedList->list.array, ranFuncAddedList->list.size);
   if(ranFuncAddedList->list.array)
   {
      for(ranFuncIdx = 0; ranFuncIdx< ranFuncAddedList->list.count; ranFuncIdx++)
      {
         RIC_ALLOC(ranFuncAddedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
         if(ranFuncAddedList->list.array[ranFuncIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation for RAN function added list array item");
            return RFAILED;
         }
         ranFuncAddedItemIe = (RANfunctionID_ItemIEs_t*)ranFuncAddedList->list.array[ranFuncIdx];
         ranFuncAddedItemIe->id = ProtocolIE_IDE2_id_RANfunctionID_Item;
         ranFuncAddedItemIe->criticality= CriticalityE2_ignore;
         ranFuncAddedItemIe->value.present = RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item;
         ranFuncAddedItemIe->value.choice.RANfunctionID_Item.ranFunctionID = addArr[ranFuncIdx].id;
         ranFuncAddedItemIe->value.choice.RANfunctionID_Item.ranFunctionRevision= addArr[ranFuncIdx].revisionCounter;
         duDb->ranFunction[addArr[ranFuncIdx].id-1].id = addArr[ranFuncIdx].id;
         duDb->ranFunction[addArr[ranFuncIdx].id-1].revisionCounter = addArr[ranFuncIdx].revisionCounter;
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

uint8_t BuildAndSendE2SetupRsp(DuDb *duDb, uint8_t transId, RicTmpRanFunList ricRanFuncList)
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
      if(ricRanFuncList.addCount)
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
      e2SetupRsp->protocolIEs.list.array[idx]->value.present = \
                                                               E2setupResponseIEs__value_PR_GlobalRIC_ID;

      if(BuildGlobalRicId(&(e2SetupRsp->protocolIEs.list.array[idx]->value.choice.GlobalRIC_ID))!=ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build Global Ric Id");
         break;
      }
      
      if(ricRanFuncList.addCount)
      {
         /* Accepted RAN function Id */
         idx++;
         e2SetupRsp->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         e2SetupRsp->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
         e2SetupRsp->protocolIEs.list.array[idx]->value.present = E2setupResponseIEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionAddedList(duDb, ricRanFuncList.addCount, ricRanFuncList.addArr,\
         &e2SetupRsp->protocolIEs.list.array[idx]->value.choice.RANfunctionsID_List)!=ROK)
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
      if(BuildE2nodeComponentConfigAdditionAck(&e2SetupRsp->protocolIEs.list.array[idx]->value.choice.E2nodeComponentConfigAdditionAck_List, duDb)!=ROK)
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
   BuildAndSendRicSubscriptionReq(duDb->duId);
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
      buildPlmnId(ricCb.ricCfgParams.plmn, subsDetails->ricEventTriggerDefinition.buf);
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

uint8_t BuildAndSendRicSubscriptionReq(uint32_t duId)
{

   E2AP_PDU_t                 *e2apRicMsg = NULL;
   RICsubscriptionRequest_t   *ricSubscriptionReq;
   uint8_t         elementCnt;
   uint8_t         idx;
   uint8_t         ieId;
   uint8_t         ret; 
   asn_enc_rval_t  encRetVal;        /* Encoder return value */

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
   if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Sending RIC subscription Request failed");
      return RFAILED;
   }
   return ROK;
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

void ProcRicSubscriptionResponse(uint32_t duId)
{
   uint8_t duIdx = 0;
   DuDb *duDb;

   DU_LOG("\nINFO  -->  E2AP : RICsubscriptionResponse Msg Acknowledged");

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb)
      duDb->ricSubscribedToDu = true;
   BuildAndSendRicServiceQuery(duDb);
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
      break;
   }

   FreeE2SetupFailure(e2apMsg);
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
   RicTmpRanFunList ricRanFuncList;

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
                        ricRanFuncList.addCount =0;
                        memset(&ricRanFuncList.addArr, 0, MAX_RAN_FUNCTION * sizeof(RanFuncInfo));
                        ranFunctionsList = &e2SetupReq->protocolIEs.list.array[arrIdx]->value.choice.RANfunctions_List;

                        if(ranFunctionsList->list.array)
                        {
                           for(ranFuncIdx=0;ranFuncIdx<ranFunctionsList->list.count; ranFuncIdx++)
                           {
                              ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFunctionsList->list.array[ranFuncIdx]; 
                              ranFunItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                              ricRanFuncList.addArr[ricRanFuncList.addCount].id =  ranFunItem->ranFunctionID; 
                              ricRanFuncList.addArr[ricRanFuncList.addCount].revisionCounter =  ranFunItem->ranFunctionRevision; 
                              ricRanFuncList.addCount++;
                           }
                           memset(&duDb->ranFunction, 0, MAX_RAN_FUNCTION * sizeof(RanFunction));
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
                                 e2NodeAddItem = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[e2NodeAddListIdx];
                                 if(e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.\
                                       e2nodeComponentInterfaceTypeF1)
                                 {
                                    duDb->e2NodeComponent.interfaceType = F1; 
                                    duDb->e2NodeComponent.componentId = e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]; 
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
   if(BuildAndSendE2SetupRsp(duDb, transId, ricRanFuncList) !=ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 setup response");
      return RFAILED;
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

uint8_t BuildAndSendE2NodeConfigUpdateAck(uint32_t duId)
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
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
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
         RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
         return RFAILED;
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
         return RFAILED;
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
         return RFAILED;
      }

      ret = ROK;
      break;
   }while(true);

   FreeE2ResetResponse(e2apMsg);
   return ROK;
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
   uint8_t ieIdx = 0, duIdx = 0;
   uint8_t transId = 0, cause = 0;
   DuDb    *duDb = NULLP;

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
 * @brief deallocate the memory allocated in RicServiceUpdateAck(
 *
 * @details
 *
 *    Function : FreeE2SetupRsp 
 *
 *    Functionality: deallocate the memory allocated in RicServiceUpdateAck
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 *
 * @return void
 * ****************************************************************/

void FreeRicServiceUpdateFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t arrIdx = 0, e2NodeConfigIdx=0, ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAddedList;
   RICserviceUpdateFailure_t *ricServiceUpdateFailure;
   
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

uint8_t BuildAndSendRicServiceUpdateFailure(uint32_t duId, uint8_t transId, CauseE2_PR causePresent, uint8_t reason)
{
   
   E2AP_PDU_t         *e2apMsg = NULL;
   RICserviceUpdateFailure_t *ricServiceFailure;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx;
   uint8_t            elementCnt;
   bool  memAllocFailed = false;

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
            memAllocFailed = true;
            break;
         }
      }
      if(memAllocFailed == true)
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
      break;
   }

   FreeRicServiceUpdateFailure(e2apMsg);
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
   uint8_t arrIdx = 0, e2NodeConfigIdx=0, ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAddedList;
   RICserviceUpdateAcknowledge_t *ricServiceUpdateAck;
   
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
                           ranFuncAddedList= &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                           if(ranFuncAddedList->list.array)
                           {
                              for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
                              {
                                 RIC_FREE(ranFuncAddedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                              }
                              RIC_FREE(ranFuncAddedList->list.array, ranFuncAddedList->list.size);
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

uint8_t BuildAndSendRicServiceUpdateAcknowledge(DuDb *duDb, uint8_t transId, RicTmpRanFunList ricRanFuncList)
{
   E2AP_PDU_t         *e2apMsg = NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx;
   uint8_t            elementCnt;
   bool  memAllocFailed = false;
   RICserviceUpdateAcknowledge_t *ricServiceUpdateAck;

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
      if(ricRanFuncList.addCount)
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
            memAllocFailed = true;
            break;
         }
      }
      if(memAllocFailed == true)
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

      if(ricRanFuncList.addCount)
      {
         /* Accepted RAN function Id */
         arrIdx++;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.present = RICserviceUpdateAcknowledge_IEs__value_PR_RANfunctionsID_List;
            DU_LOG("\nERROR  -->  E2AP : Failed to build Ran function added list");
         if(BuildRanFunctionAddedList(duDb, ricRanFuncList.addCount, ricRanFuncList.addArr,\
         &ricServiceUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List)!=ROK)
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
      break;
   }
   FreeRicServiceUpdateAck(e2apMsg);
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
   bool ricServiceUpdateFailed = false, ranFuncFound=false;
   uint8_t arrIdx = 0, e2NodeAddListIdx =0, duIdx = 0;
   uint8_t storeRanFunIdx=0, transId =0, ranFuncIdx =0, ranFuncCount=0;
   RANfunction_ItemIEs_t *ranFuncItemIe;
   RANfunction_Item_t  *ranFuncItem;
   RANfunctionID_Item_t  *ranFuncIdItem;
   RANfunctions_List_t *ranFuncList;
   RANfunctionsID_List_t *deleteList;
   RANfunctionID_ItemIEs_t *delRanFuncItem;

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
                     ricRanFuncList.addArr[ricRanFuncList.addCount].id =  ranFuncItem->ranFunctionID; 
                     ricRanFuncList.addArr[ricRanFuncList.addCount].revisionCounter = ranFuncItem->ranFunctionRevision; 
                     ricRanFuncList.addCount++;
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
                     ranFuncFound = false;
                     ranFuncItemIe = (RANfunction_ItemIEs_t *) ranFuncList->list.array[ranFuncIdx];
                     ranFuncItem = &ranFuncItemIe->value.choice.RANfunction_Item;
                     if(duDb->ranFunction[ranFuncItem->ranFunctionID-1].id != ranFuncItem->ranFunctionID)
                     {
                        DU_LOG("\nERROR  -->  E2AP : RAN function id is not same [%d]",ranFuncItem->ranFunctionID);
                        if(BuildAndSendRicServiceUpdateFailure(duId, transId, CauseE2_PR_ricRequest, CauseE2RICrequest_ran_function_id_invalid) == ROK)
                        {
                           ricServiceUpdateFailed = true;
                           break;
                        }
                     }
                     ricRanFuncList.addArr[ricRanFuncList.addCount].id =  ranFuncItem->ranFunctionID; 
                     ricRanFuncList.addArr[ricRanFuncList.addCount].revisionCounter = ranFuncItem->ranFunctionRevision; 
                     ricRanFuncList.addCount++;
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
                     ranFuncFound = false;
                     delRanFuncItem  = (RANfunctionID_ItemIEs_t*) deleteList->list.array[ranFuncIdx];
                     ranFuncIdItem = &delRanFuncItem->value.choice.RANfunctionID_Item;
                     if(duDb->ranFunction[ranFuncIdItem->ranFunctionID-1].id != ranFuncIdItem->ranFunctionID)
                     {
                        DU_LOG("\nERROR  -->  E2AP : RAN function id is not same [%d]",ranFuncIdItem->ranFunctionID);
                        if(BuildAndSendRicServiceUpdateFailure(duId, transId, CauseE2_PR_ricRequest, CauseE2RICrequest_ran_function_id_invalid) == ROK)
                        {
                           ricServiceUpdateFailed = true;
                           break;
                        }
                     }
                     memset(&duDb->ranFunction[ranFuncIdItem->ranFunctionID-1], 0, sizeof(RanFunction));
                  }
               }
               break;
            }

         default:
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid type of ricServiceUpdate array [%d]",ricServiceUpdate->protocolIEs.list.array[arrIdx]->id);
               return;
            }
      }
   }
   
   if(!ricServiceUpdateFailed)
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
   uint8_t arrIdx = 0, e2NodeConfigIdx=0, ranFuncIdx=0;
   RANfunctionsID_List_t *ranFuncAddedList;
   RICserviceQuery_t *ricServiceQuery;
   
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
                           ranFuncAddedList= &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List;
                           if(ranFuncAddedList->list.array)
                           {
                              for(ranFuncIdx=0;ranFuncIdx<ranFuncAddedList->list.count; ranFuncIdx++)
                              {
                                 RIC_FREE(ranFuncAddedList->list.array[ranFuncIdx], sizeof(RANfunction_ItemIEs_t));
                              }
                              RIC_FREE(ranFuncAddedList->list.array, ranFuncAddedList->list.size);
                           }
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
 * @brief calculate number of Ran Function Info present in DB
 *
 * @details
 *
 *    Function : calculateRanFunctionInfo
 *
 * Functionality: calculate number of Ran Function Info present in DB
 *
 * @params[in] - DuDb *duDb, RicTmpRanFunList *ricRanFuncList
 * @return ROK     - success
 *         RFAILED - Acknowledge
 *
 ******************************************************************/

void calculateRanFunctionInfo(DuDb *duDb, RicTmpRanFunList *ricRanFuncList)
{
   for(int idx=0;idx<MAX_RAN_FUNCTION; idx++)
   {
      if(duDb->ranFunction[idx].id)
      {
         ricRanFuncList->addArr[ricRanFuncList->addCount].id = duDb->ranFunction[idx].id;
         ricRanFuncList->addArr[ricRanFuncList->addCount].revisionCounter = duDb->ranFunction[idx].revisionCounter;
         ricRanFuncList->addCount++;
      }
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
   RicTmpRanFunList   ricRanFuncList;   
   E2AP_PDU_t         *e2apMsg = NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx;
   uint8_t            elementCnt;
   bool  memAllocFailed = false;
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

      memset(&ricRanFuncList, 0, sizeof(RicTmpRanFunList));
      calculateRanFunctionInfo(duDb, &ricRanFuncList);
      
      elementCnt = 1;
      if(ricRanFuncList.addCount)
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
      
      if(ricRanFuncList.addCount)
      {
         /* Accepted RAN function Id */
         arrIdx++;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_RANfunctionsAccepted;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         ricServiceQuery->protocolIEs.list.array[arrIdx]->value.present = RICserviceQuery_IEs__value_PR_RANfunctionsID_List;
         if(BuildRanFunctionAddedList(duDb, ricRanFuncList.addCount,  ricRanFuncList.addArr, &ricServiceQuery->protocolIEs.list.array[arrIdx]->value.choice.RANfunctionsID_List)!=ROK)
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
      break;
   }
   FreeRicServiceQuery(e2apMsg);
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
                     BuildAndSendE2NodeConfigUpdateAck(*duId);
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
                     DU_LOG("\nINFO  -->  E2AP : RIC Indication Acknowledged");
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICserviceUpdate:
               {
                  DU_LOG("\nINFO  -->  E2AP : RIC Service update received");
                  ProcRicServiceUpdate(*duId, &e2apMsg->choice.initiatingMessage->value.choice.RICserviceUpdate);
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
                     ProcRicSubscriptionResponse(*duId);
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
} /* End of E2APMsgHdlr */


/**********************************************************************
  End of file
 **********************************************************************/
