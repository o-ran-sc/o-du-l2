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
 * @brief fill E2 failure cause 
 *
 * @details
 *
 *    Function : fillE2FailureCause
 *
 * Functionality: fill E2 failure cause
 *
 * @params[in] 
 *    Cause Parameter which needs to be filled 
 *    CauseE2 Present 
 *    Reason to fill
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

/************************************************************************
 *
 * @brief Converts E2nodeComponentInterfaceType value into enum
 *
 * @details
 *
 *    Function : convertE2ComponentInterfaceTypeToEnum
 *
 *    Functionality: Convert E2nodeComponentInterfaceType into enum
 *    value
 *
 * @params[in] uint8_t value of interface
 * @return Enum value of e2 interface
 *
 * **********************************************************************/

uint8_t convertE2ComponentInterfaceTypeToEnum(uint8_t interface)
{
   switch(interface)
   {
      case E2nodeComponentInterfaceType_ng:
         return NG;

      case E2nodeComponentInterfaceType_xn:
         return XN;

      case E2nodeComponentInterfaceType_e1:
         return E1;

      case E2nodeComponentInterfaceType_f1:
         return F1;

      case E2nodeComponentInterfaceType_w1:
         return W1;

      case E2nodeComponentInterfaceType_s1:
         return S1;

      case E2nodeComponentInterfaceType_x2:
         return X2;
   }
   return RFAILED;
}

/************************************************************************
 *
 * @brief Converts InterfaceType into E2nodeComponentInterfaceType value
 *
 * @details
 *
 *    Function : convertInterfaceToE2ComponentInterfaceType
 *
 *    Functionality: Convert InterfaceType into E2nodeComponentInterfaceType
 *    value
 *
 * @params[in] Enum value of interface
 * @return actual value of e2 interface
 *
 * **********************************************************************/

uint8_t convertInterfaceToE2ComponentInterfaceType(uint8_t interface)
{
   switch(interface)
   {
      case NG:
         return E2nodeComponentInterfaceType_ng;

      case XN:
         return E2nodeComponentInterfaceType_xn;

      case E1:
         return E2nodeComponentInterfaceType_e1;

      case F1:
         return E2nodeComponentInterfaceType_f1;

      case W1:
         return E2nodeComponentInterfaceType_w1;

      case S1:
         return E2nodeComponentInterfaceType_s1;

      case X2:
         return E2nodeComponentInterfaceType_x2;
   }
   return RFAILED;
}

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
 * @brief Deallocate memory allocated for E2nodeConfigurationUpdate 
 *
 * @details
 *
 *    Function : freeE2NodeConfigItem 
 *
 *    Functionality:
 *       - freeing the memory allocated for E2nodeConfigurationUpdate
 *
 * @params[in]
 *       uint8_t protocolIe
 *       PTR to e2NodeCfg which is to be freed
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void freeE2NodeConfigItem(uint8_t protocolIe, PTR e2NodeCfg)
{
   E2nodeComponentConfigurationAck_t *cfgAck =NULLP;
   E2nodeComponentInterfaceF1_t *f1InterfaceInfo=NULLP;
   E2nodeComponentConfigAdditionAck_Item_t *e2NodeupdateAddItemIe=NULLP;
   E2nodeComponentConfigRemovalAck_Item_t *e2NodeRemovalAckItemIe=NULLP;
   E2nodeComponentConfigUpdateAck_Item_t *e2NodeupdateAckItemIe=NULLP;

   switch(protocolIe)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
      {
         e2NodeupdateAddItemIe= (E2nodeComponentConfigAdditionAck_Item_t*)e2NodeCfg;
         f1InterfaceInfo = e2NodeupdateAddItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
         cfgAck = &e2NodeupdateAddItemIe->e2nodeComponentConfigurationAck;
      }

      case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck:
      {
         e2NodeupdateAckItemIe = (E2nodeComponentConfigUpdateAck_Item_t*)e2NodeCfg;
         f1InterfaceInfo = e2NodeupdateAckItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
         cfgAck = &e2NodeupdateAckItemIe->e2nodeComponentConfigurationAck;
      }

      case ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck:
      {
         e2NodeRemovalAckItemIe= (E2nodeComponentConfigRemovalAck_Item_t*)e2NodeCfg;
         f1InterfaceInfo = e2NodeRemovalAckItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
         cfgAck = &e2NodeRemovalAckItemIe->e2nodeComponentConfigurationAck;
      }
   }

   if(f1InterfaceInfo)
   {
      RIC_FREE(f1InterfaceInfo->gNB_DU_ID.buf, f1InterfaceInfo->gNB_DU_ID.size);
      RIC_FREE(f1InterfaceInfo, sizeof(E2nodeComponentInterfaceF1_t));
   }

   switch(cfgAck->updateOutcome)
   {
      case E2nodeComponentConfigurationAck__updateOutcome_success:
         break;
      case E2nodeComponentConfigurationAck__updateOutcome_failure:
         {
            RIC_FREE(cfgAck->failureCauseE2, sizeof(CauseE2_t));
            break;
         }
   }

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
   uint8_t arrIdx =0, e2NodeConfigIdx=0;
   E2nodeConfigurationUpdateAcknowledge_t *e2NodeConfigUpdateAck=NULL;
   E2nodeComponentConfigUpdateAck_ItemIEs_t *e2NodeupdateAckItemIe=NULL;
   E2nodeComponentConfigUpdateAck_List_t *e2NodeConfigUpdateAckList=NULL;
   E2nodeComponentConfigRemovalAck_ItemIEs_t *e2NodeRemovalAckItemIe=NULL;
   E2nodeComponentConfigRemovalAck_List_t *e2NodeConfigRemovalAckList=NULL;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeupdateAddItemIe=NULL;
   E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList=NULL;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         e2NodeConfigUpdateAck = &e2apMsg->choice.successfulOutcome->value.choice.E2nodeConfigurationUpdateAcknowledge;
         if(e2NodeConfigUpdateAck->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < e2NodeConfigUpdateAck->protocolIEs.list.count; arrIdx++)
            {
               if(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx])
               {
                  switch(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
                        {
                           e2NodeConfigAdditionAckList =&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List;
                           if(e2NodeConfigAdditionAckList->list.count)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<e2NodeConfigAdditionAckList->list.count; e2NodeConfigIdx++)
                              {
                                 e2NodeupdateAddItemIe = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAdditionAckList->list.array[e2NodeConfigIdx];
                                 if(e2NodeupdateAddItemIe)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck, (PTR)&e2NodeupdateAddItemIe->value.choice.E2nodeComponentConfigAdditionAck_Item);
                                    RIC_FREE(e2NodeupdateAddItemIe, sizeof(E2nodeComponentConfigAdditionAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(e2NodeConfigAdditionAckList->list.array, e2NodeConfigAdditionAckList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck:
                        {
                           e2NodeConfigUpdateAckList =&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigUpdateAck_List;
                           if(e2NodeConfigUpdateAckList->list.count)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<e2NodeConfigUpdateAckList->list.count; e2NodeConfigIdx++)
                              {
                                 e2NodeupdateAckItemIe = (E2nodeComponentConfigUpdateAck_ItemIEs_t*) e2NodeConfigUpdateAckList->list.array[e2NodeConfigIdx];
                                 if(e2NodeupdateAckItemIe)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck, (PTR)&e2NodeupdateAckItemIe->value.choice.E2nodeComponentConfigUpdateAck_Item);
                                    RIC_FREE(e2NodeupdateAckItemIe, sizeof(E2nodeComponentConfigUpdateAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(e2NodeConfigUpdateAckList->list.array, e2NodeConfigUpdateAckList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck:
                        {
                           e2NodeConfigRemovalAckList =&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemovalAck_List;
                           if(e2NodeConfigRemovalAckList->list.count)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<e2NodeConfigRemovalAckList->list.count; e2NodeConfigIdx++)
                              {
                                 e2NodeRemovalAckItemIe = (E2nodeComponentConfigRemovalAck_ItemIEs_t*) e2NodeConfigRemovalAckList->list.array[e2NodeConfigIdx];
                                 if(e2NodeRemovalAckItemIe)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck, (PTR)&e2NodeRemovalAckItemIe->value.choice.E2nodeComponentConfigRemovalAck_Item);
                                    RIC_FREE(e2NodeRemovalAckItemIe, sizeof(E2nodeComponentConfigRemovalAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(e2NodeConfigRemovalAckList->list.array, e2NodeConfigRemovalAckList->list.size);
                           }
                           break;
                        }
                  }
                  RIC_FREE(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdate_t));
               }
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
 * @brief fill e2 node configuration for ack msg 
 *
 * @details
 *
 *    Function : fillE2NodeCOnfigAck
 *
 *    Functionality:
 *       - fill e2 node configuration for ack msg
 *
 * @params[in] 
 *    Pointer to e2NodeCfg to be filled
 *    procedure Code
 *    E2 Node Component information
 *    Is successful or failure response
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillE2NodeCOnfigAck(PTR e2NodeCfg, uint8_t procedureCode, E2NodeComponent componentInfo, bool isSuccessful)
{
	E2nodeComponentID_t	 *e2nodeComponentID=NULLP;
	E2nodeComponentInterfaceType_t	 *e2nodeComponentInterfaceType=NULLP;
	E2nodeComponentConfigurationAck_t	 *e2nodeComponentConfigurationAck=NULLP;
   E2nodeComponentConfigRemovalAck_Item_t *removalAckItem=NULLP;
   E2nodeComponentConfigUpdateAck_Item_t *updateAckItem=NULLP;
   E2nodeComponentConfigAdditionAck_Item_t *addAckItem=NULLP;

   switch(procedureCode)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
      {
         addAckItem = (E2nodeComponentConfigAdditionAck_Item_t *)e2NodeCfg;
         e2nodeComponentInterfaceType = &addAckItem->e2nodeComponentInterfaceType;
         e2nodeComponentID = &addAckItem->e2nodeComponentID;
         e2nodeComponentConfigurationAck = &addAckItem->e2nodeComponentConfigurationAck;
         break;
      }
      case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck:
      {
         updateAckItem = (E2nodeComponentConfigUpdateAck_Item_t*) e2NodeCfg;
         e2nodeComponentInterfaceType = &updateAckItem->e2nodeComponentInterfaceType;
         e2nodeComponentID = &updateAckItem->e2nodeComponentID;
         e2nodeComponentConfigurationAck = &updateAckItem->e2nodeComponentConfigurationAck;
         break;
      }
      case  ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck:
      {
         removalAckItem= (E2nodeComponentConfigRemovalAck_Item_t*)e2NodeCfg;
         e2nodeComponentInterfaceType = &removalAckItem->e2nodeComponentInterfaceType;
         e2nodeComponentID = &removalAckItem->e2nodeComponentID;
         e2nodeComponentConfigurationAck = &removalAckItem->e2nodeComponentConfigurationAck;
         break;
      }
   }
   
      /* >E2 Node Component interface type */
  *e2nodeComponentInterfaceType = convertInterfaceToE2ComponentInterfaceType(componentInfo.interfaceType);

   if(*e2nodeComponentInterfaceType == E2nodeComponentInterfaceType_f1)
   {
      /* >E2 Node Component ID */
      e2nodeComponentID->present = E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1;
      RIC_ALLOC(e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1, sizeof(E2nodeComponentInterfaceF1_t));
      if(e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1 == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigUpdateAck %d",__LINE__);
         return RFAILED;
      }
      e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size = sizeof(uint8_t);
      RIC_ALLOC(e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf, e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.size);

      if(e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf == NULLP)
      {
         DU_LOG("\nERROR  -->list.  E2AP: Memory allocation failed for BuildE2nodeComponentConfigUpdateAck %d",__LINE__);
         return RFAILED;
      }
      e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]  = componentInfo.componentId;
   }

   if(isSuccessful)
   {
      /* >E2 Node Component Configuration Acknowledge*/
      e2nodeComponentConfigurationAck->updateOutcome = E2nodeComponentConfigurationAck__updateOutcome_success;
   }
   else
   {
      /* >E2 Node Component Configuration Acknowledge*/
      e2nodeComponentConfigurationAck->updateOutcome = E2nodeComponentConfigurationAck__updateOutcome_failure;
      RIC_ALLOC(e2nodeComponentConfigurationAck->failureCauseE2, sizeof(struct CauseE2));
      if(e2nodeComponentConfigurationAck->failureCauseE2)
      {
         fillE2FailureCause(e2nodeComponentConfigurationAck->failureCauseE2, CauseE2_PR_e2Node, CauseE2node_e2node_component_unknown);
      }
      else
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigUpdateAck %d",__LINE__);
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Build E2node Component config Removal ack list
 *
 * @details
 *
 *    Function :  BuildE2nodeComponentConfigRemovalAck
 *
 *    Functionality: Build the e2 node remove ack 
 *
 * @params[in] 
 *    E2nodeComponentConfigRemovalAck_List_t to be filled
 *    Count of e2 node to be removed
 *    list of e2 node cfg to be removed
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2nodeComponentConfigRemovalAck(E2nodeComponentConfigRemovalAck_List_t *e2NodeConfigRemovalAckList, uint16_t removaldE2NodeCount, E2NodeConfigItem *removaldE2Node)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigRemovalAck_ItemIEs_t *e2NodeRemovalAckItem=NULL;
   
   e2NodeConfigRemovalAckList->list.count = removaldE2NodeCount;
   e2NodeConfigRemovalAckList->list.size = e2NodeConfigRemovalAckList->list.count * sizeof(E2nodeComponentConfigRemovalAck_ItemIEs_t*);
   RIC_ALLOC(e2NodeConfigRemovalAckList->list.array, e2NodeConfigRemovalAckList->list.size);
   if(e2NodeConfigRemovalAckList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigRemovalAck %d",__LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeConfigRemovalAckList->list.count; arrIdx++)
   {
      RIC_ALLOC(e2NodeConfigRemovalAckList->list.array[arrIdx], sizeof(E2nodeComponentConfigRemovalAck_ItemIEs_t));
      if(e2NodeConfigRemovalAckList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigRemovalAck %d",__LINE__);
         return RFAILED;
      }
      e2NodeRemovalAckItem = (E2nodeComponentConfigRemovalAck_ItemIEs_t*) e2NodeConfigRemovalAckList->list.array[arrIdx];
      e2NodeRemovalAckItem->id = ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck_Item;
      e2NodeRemovalAckItem->criticality = CriticalityE2_reject;
      e2NodeRemovalAckItem->value.present = E2nodeComponentConfigRemovalAck_ItemIEs__value_PR_E2nodeComponentConfigRemovalAck_Item;
      fillE2NodeCOnfigAck((PTR)&e2NodeRemovalAckItem->value.choice.E2nodeComponentConfigRemovalAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck,\
      removaldE2Node[arrIdx].componentInfo, removaldE2Node[arrIdx].isSuccessful);
   }
   return ROK;  
}

/*******************************************************************
 *
 * @brief Build E2node Component config update ack list
 *
 * @details
 *
 *    Function :  BuildE2nodeComponentConfigUpdateAck
 *
 *    Functionality: Build E2node Component config update ack list 
 *
 * @params[in] 
 *    E2nodeComponentConfigUpdateAck_List to be filled
 *    Count of e2 node to be update
 *    list of e2 node cfg to be update
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2nodeComponentConfigUpdateAck(E2nodeComponentConfigUpdateAck_List_t *e2NodeConfigUpdateAckList, uint16_t updatedE2NodeCount, E2NodeConfigItem *updatedE2Node)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigUpdateAck_ItemIEs_t *e2NodeUpdateAckItem=NULL;
   
   e2NodeConfigUpdateAckList->list.count = updatedE2NodeCount;
   e2NodeConfigUpdateAckList->list.size = e2NodeConfigUpdateAckList->list.count * sizeof(E2nodeComponentConfigUpdateAck_ItemIEs_t*);
   RIC_ALLOC(e2NodeConfigUpdateAckList->list.array, e2NodeConfigUpdateAckList->list.size);
   if(e2NodeConfigUpdateAckList->list.array == NULLP)
   {
      DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigUpdateAck %d",__LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< e2NodeConfigUpdateAckList->list.count; arrIdx++)
   {
      RIC_ALLOC(e2NodeConfigUpdateAckList->list.array[arrIdx], sizeof(E2nodeComponentConfigUpdateAck_ItemIEs_t));
      if(e2NodeConfigUpdateAckList->list.array[arrIdx] == NULLP)
      {
         DU_LOG("\nERROR  --> E2AP: Memory allocation failed for BuildE2nodeComponentConfigUpdateAck %d",__LINE__);
         return RFAILED;
      }
      e2NodeUpdateAckItem = (E2nodeComponentConfigUpdateAck_ItemIEs_t*) e2NodeConfigUpdateAckList->list.array[arrIdx];
      e2NodeUpdateAckItem->id = ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck_Item;
      e2NodeUpdateAckItem->criticality = CriticalityE2_reject;
      e2NodeUpdateAckItem->value.present = E2nodeComponentConfigUpdateAck_ItemIEs__value_PR_E2nodeComponentConfigUpdateAck_Item;
      
      fillE2NodeCOnfigAck((PTR)&e2NodeUpdateAckItem->value.choice.E2nodeComponentConfigUpdateAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck,\
      updatedE2Node[arrIdx].componentInfo, updatedE2Node[arrIdx].isSuccessful);

   }
   return ROK;  
}

/*******************************************************************
 *
 * @brief Build E2node Component config addition ack list
 *
 * @details
 *
 *    Function :  BuildE2nodeComponentConfigAdditionAck
 *
 *    Functionality:  Build E2node Component config addition ack list 
 *
 * @params[in] 
 *    E2nodeComponentConfigAdditionAck_List to be filled
 *    Count of e2 node to be added
 *    list of e2 node cfg to be added
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2nodeComponentConfigAdditionAck(E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList, DuDb *duDb,  uint16_t addedE2NodeCount, E2NodeConfigItem *addedE2Node, uint8_t procedureCode)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItem=NULLP;
  
   if(procedureCode == ProcedureCodeE2_id_E2setup)
   { 
      e2NodeConfigAdditionAckList->list.count = duDb->numOfE2NodeComponent;
   }
   else
   { 
      e2NodeConfigAdditionAckList->list.count = addedE2NodeCount;
   }
   
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
      e2NodeAddAckItem = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAdditionAckList->list.array[arrIdx];
      e2NodeAddAckItem->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck_Item;
      e2NodeAddAckItem->criticality = CriticalityE2_reject;
      e2NodeAddAckItem->value.present = E2nodeComponentConfigAdditionAck_ItemIEs__value_PR_E2nodeComponentConfigAdditionAck_Item;
      if(procedureCode == ProcedureCodeE2_id_E2setup)
      {
         fillE2NodeCOnfigAck((PTR)&e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck,\
         duDb->e2NodeComponent[arrIdx], true);
      }
      else
      {
         fillE2NodeCOnfigAck((PTR)&e2NodeAddAckItem->value.choice.E2nodeComponentConfigAdditionAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck,\
         addedE2Node[arrIdx].componentInfo, addedE2Node[arrIdx].isSuccessful);
      }
   }
   return ROK;  
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
 * @params[in] 
 *    DU databse 
 *    transId 
 *    list of E2 node cfg which needs to fill in IEs
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2NodeConfigUpdateAck(DuDb *duDb, uint8_t transId,  E2NodeConfigList *e2NodeList)
{
   uint8_t ret = ROK;
   uint8_t arrIdx = 0,elementCnt = 0;
   E2AP_PDU_t        *e2apMsg = NULLP;
   asn_enc_rval_t     encRetVal;       
   E2nodeConfigurationUpdateAcknowledge_t *e2NodeConfigUpdateAck = NULLP;

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
      
      elementCnt =1;
      if(e2NodeList->addedE2NodeCount)
         elementCnt++;
      if(e2NodeList->updatedE2NodeCount)
         elementCnt++;
      if(e2NodeList->removedE2NodeCount)
         elementCnt++;

      e2NodeConfigUpdateAck->protocolIEs.list.count = elementCnt;
      e2NodeConfigUpdateAck->protocolIEs.list.size  = elementCnt * sizeof(E2nodeConfigurationUpdateAcknowledge_IEs_t*);
      RIC_ALLOC(e2NodeConfigUpdateAck->protocolIEs.list.array, e2NodeConfigUpdateAck->protocolIEs.list.size);
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
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_TransactionID;
      e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.TransactionID = transId;
      
      if(e2NodeList->addedE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigUpdateAck_List;
         if(BuildE2nodeComponentConfigAdditionAck(&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List, NULL,\
                  e2NodeList->addedE2NodeCount, e2NodeList->addedE2Node, ProcedureCodeE2_id_E2nodeConfigurationUpdate)!=ROK)

         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config addition ack list");
            break;
         }
      }
      if(e2NodeList->updatedE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigUpdateAck_List;
         if(BuildE2nodeComponentConfigUpdateAck(&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigUpdateAck_List,\
                  e2NodeList->updatedE2NodeCount, e2NodeList->updatedE2Node)!=ROK)

         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config addition ack list");
            break;
         }
      }
      if(e2NodeList->removedE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigUpdateAck_List;
         if(BuildE2nodeComponentConfigRemovalAck(&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemovalAck_List,\
                  e2NodeList->removedE2NodeCount, e2NodeList->removedE2Node)!=ROK)

         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config addition ack list");
            break;
         }
      }      
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
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
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
 * @brief get Component ID And Interface Info 
 *
 * @details
 *
 *    Function : getComponentIDAndInterfaceInfo
 *
 *    Functionality:
 *         - get Component ID And Interface Info 
 * @params[in] 
 *    DU database
 *    Pointer to e2NodeCfg which has info 
 *    ProtocolId
 *    E2NodeConfigItem to be filled
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

void getComponentIDAndInterfaceInfo(DuDb *duDb, PTR e2NodeCfg, uint8_t protocolId, E2NodeConfigItem *storeCfg)
{
   uint8_t idx=0;
   bool configFound;
   E2nodeComponentID_t componentId;
   E2nodeComponentInterfaceType_t interface;
   E2nodeComponentConfigAddition_Item_t *addCfg=NULL;
   E2nodeComponentConfigUpdate_Item_t *updateCfg=NULL;
   E2nodeComponentConfigRemoval_Item_t *removeCfg=NULL;

   switch(protocolId)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
         {
            addCfg = (E2nodeComponentConfigAddition_Item_t *)e2NodeCfg;
            interface = addCfg->e2nodeComponentInterfaceType;
            componentId = addCfg->e2nodeComponentID;
            break;
         }
      case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate:
         {
            updateCfg = (E2nodeComponentConfigUpdate_Item_t *)e2NodeCfg;
            interface = updateCfg->e2nodeComponentInterfaceType;
            componentId = updateCfg->e2nodeComponentID;
            break;
         }
      case ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval:
         {
            removeCfg = (E2nodeComponentConfigRemoval_Item_t*)e2NodeCfg;
            interface = removeCfg->e2nodeComponentInterfaceType;
            componentId = removeCfg->e2nodeComponentID;
            break;
         }
   }
   
   storeCfg->componentInfo.interfaceType = convertE2ComponentInterfaceTypeToEnum(interface);
   switch(componentId.present)
   {
      case E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1:
         {
            if(componentId.choice.e2nodeComponentInterfaceTypeF1)
            {
               storeCfg->componentInfo.componentId = componentId.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0];
            }
            break;
         }
      default:
         {
            break;
         }
   }

   if(protocolId != ProtocolIE_IDE2_id_E2nodeComponentConfigAddition)
   {
      for(idx =0;idx< duDb->numOfE2NodeComponent; idx++)
      {
         if(duDb->e2NodeComponent[idx].interfaceType == storeCfg->componentInfo.interfaceType  && protocolId == ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate)
         {
            duDb->e2NodeComponent[idx].componentId = storeCfg->componentInfo.componentId;
            configFound = true;
            break;
         }
         else if(duDb->e2NodeComponent[idx].interfaceType == storeCfg->componentInfo.interfaceType && protocolId == ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval)
         {
            memset(&duDb->e2NodeComponent[idx], 0, sizeof(E2NodeComponent));
            configFound = true;
            break;
         }
      }
      
      if(configFound == true)
         storeCfg->isSuccessful = true;
      else
         storeCfg->isSuccessful = false;
   }
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
 * @params[in]
 *    DU id
 *    Pointer to E2nodeConfigurationUpdate
 * @return Void
 *
 ******************************************************************/

void ProcE2NodeConfigUpdate(uint32_t duId, E2nodeConfigurationUpdate_t *e2NodeConfigUpdate)
{
   E2NodeConfigList tmpE2NodeList;
   uint8_t ieIdx = 0, duIdx = 0, arrIdx=0,elementCnt=0;
   uint8_t transId = 0;
   DuDb    *duDb = NULLP;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList=NULL;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItemIe=NULL;
   E2nodeComponentConfigAddition_Item_t *e2NodeAddItem=NULL;
   E2nodeComponentConfigUpdate_List_t *e2NodeUpdateList=NULLP;
   E2nodeComponentConfigUpdate_ItemIEs_t *e2NodeUpdateItemIe=NULLP;
   E2nodeComponentConfigUpdate_Item_t *e2NodeUpdateItem =NULLP;
   E2nodeComponentConfigRemoval_List_t *e2NodeRemoveList=NULL;
   E2nodeComponentConfigRemoval_ItemIEs_t *e2NodeRemovalItemIe=NULL;
   E2nodeComponentConfigRemoval_Item_t *e2NodeRemovalItem=NULL;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }
   
   memset(&tmpE2NodeList, 0, sizeof(E2NodeConfigList));
   if(!e2NodeConfigUpdate)
   {
      DU_LOG("\nERROR  -->  E2AP : e2NodeConfigUpdate pointer is null");
      return;
   }
   if(!e2NodeConfigUpdate->protocolIEs.list.array)
   {
       DU_LOG("\nERROR  -->  E2AP : e2NodeConfigUpdate array pointer is null");
      return;
   }
   
   elementCnt = e2NodeConfigUpdate->protocolIEs.list.count;
   for(ieIdx=0; ieIdx < e2NodeConfigUpdate->protocolIEs.list.count; ieIdx++)
   {
      if(!e2NodeConfigUpdate->protocolIEs.list.array[ieIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : e2NodeConfigUpdate array idx %d pointer is null",arrIdx);
         break;
      }
      
      switch(e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
               if(transId < 0 || transId > 255)
               {
                  DU_LOG("\nERROR  -->  E2AP : Received invalid transId %d",transId);
                  return;
               }
               break;
            }

         case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
            {
               e2NodeAddList =&e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2nodeComponentConfigAddition_List;
               if(e2NodeAddList->list.array)
               {
                  for(arrIdx = 0; arrIdx< e2NodeAddList->list.count; arrIdx++)
                  {
                     e2NodeAddItemIe = (E2nodeComponentConfigAddition_ItemIEs_t *) e2NodeAddList->list.array[arrIdx];
                     e2NodeAddItem =  &e2NodeAddItemIe->value.choice.E2nodeComponentConfigAddition_Item;
                     getComponentIDAndInterfaceInfo(duDb, (PTR)e2NodeAddItem, ProtocolIE_IDE2_id_E2nodeComponentConfigAddition, &tmpE2NodeList.addedE2Node[tmpE2NodeList.addedE2NodeCount++]);
                     
                     /* Adding the new ran function in DB*/
                     duDb->e2NodeComponent[duDb->numOfE2NodeComponent].interfaceType = tmpE2NodeList.addedE2Node[tmpE2NodeList.addedE2NodeCount-1].componentInfo.interfaceType;
                     duDb->e2NodeComponent[duDb->numOfE2NodeComponent].componentId   = tmpE2NodeList.addedE2Node[tmpE2NodeList.addedE2NodeCount-1].componentInfo.componentId;
                     duDb->numOfE2NodeComponent++;

                  }
               }
               break;
            }

         case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate:
            {
               e2NodeUpdateList =&e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2nodeComponentConfigUpdate_List;
               if(e2NodeUpdateList->list.array)
               {
                  for(arrIdx = 0; arrIdx< e2NodeUpdateList->list.count; arrIdx++)
                  {
                     e2NodeUpdateItemIe = (E2nodeComponentConfigUpdate_ItemIEs_t*) e2NodeUpdateList->list.array[arrIdx];
                     e2NodeUpdateItem =  &e2NodeUpdateItemIe->value.choice.E2nodeComponentConfigUpdate_Item;
                     
                     getComponentIDAndInterfaceInfo(duDb, (PTR)e2NodeUpdateItem, ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate, &tmpE2NodeList.updatedE2Node[tmpE2NodeList.updatedE2NodeCount++]);
                  }
               }
               break;
            }

         case ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval:
            {
               e2NodeRemoveList = &e2NodeConfigUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2nodeComponentConfigRemoval_List;
               if(e2NodeRemoveList->list.array)
               {
                  for(arrIdx = 0; arrIdx< e2NodeRemoveList->list.count; arrIdx++)
                  {
                     e2NodeRemovalItemIe = (E2nodeComponentConfigRemoval_ItemIEs_t *) e2NodeRemoveList->list.array[arrIdx];
                     e2NodeRemovalItem =  &e2NodeRemovalItemIe->value.choice.E2nodeComponentConfigRemoval_Item;
                     
                     getComponentIDAndInterfaceInfo(duDb, (PTR)e2NodeRemovalItem, ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval, &tmpE2NodeList.removedE2Node[tmpE2NodeList.removedE2NodeCount++]);

                  }
               }
               break;
            }

         default:
            {
               break;
            }
      }
   }
   if(elementCnt == ieIdx)
   {
      if(BuildAndSendE2NodeConfigUpdateAck(duDb, transId, &tmpE2NodeList) !=ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 node config ack");
         return;
      }
   }
   else
   {
      if(BuildAndSendE2NodeConfigUpdateFailure(duDb->duId, transId, CauseE2_PR_misc, CauseE2Misc_unspecified) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 node config Failure");
         return;
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
         value.choice.E2nodeComponentConfigAdditionAck_List, duDb, 0, NULL, ProcedureCodeE2_id_E2setup) != ROK)
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
      actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID = ++ricActionId;
      ricSubsDb->actionSequence[ricActionId-1].id = \
         actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionID;

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

   memset(&ricSubsDb->actionSequence[ricActionId-1], 0, sizeof(ActionInfo));
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
   uint8_t duIdx = 0, ieIdx = 0,subsIdx = 0, notAdmitIdx = 0;
   uint8_t ranFuncId = 0, actionId = 0;
   DuDb *duDb = NULLP;
   bool ricReqIdDecoded = false;
   RicRequestId ricReqId;
   RanFunction  *ranFuncDb = NULLP;
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
                        if(duDb->ranFunction[ranFuncId-1].id == ranFuncId)
                        {
                           ranFuncDb = &duDb->ranFunction[ranFuncId-1];
                        }
                        else
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
                              for(subsIdx = 0; subsIdx < ranFuncDb->numOfSubscription; subsIdx++)
                              {
                                 if((ranFuncDb->subscriptionList[subsIdx].requestId.requestorId == ricReqId.requestorId) &&
                                       (ranFuncDb->subscriptionList[subsIdx].requestId.instanceId == ricReqId.instanceId))
                                 {
                                    if(ranFuncDb->subscriptionList[subsIdx].actionSequence[actionId-1].id == actionId)
                                    {
                                       memset(&ranFuncDb->subscriptionList[subsIdx].actionSequence[actionId-1], 0, \
                                          sizeof(ActionInfo));
                                       ranFuncDb->subscriptionList[subsIdx].numOfActions--;
                                       break;
                                    }
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
   uint8_t  ret= RFAILED;
   E2AP_PDU_t         *e2apMsg = NULL;
   E2setupFailure_t   *e2SetupFailure=NULL;
   asn_enc_rval_t     encRetVal;
   uint8_t            arrIdx=0;
   uint8_t            elementCnt=0;
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
      ret =ROK;
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
   DuDb    *duDb = NULLP;
   uint8_t arrIdx = 0, e2NodeAddListIdx =0;
   uint8_t duIdx = 0, transId =0, ranFuncIdx;
   RANfunction_ItemIEs_t *ranFuncItemIe =NULL;
   RANfunction_Item_t  *ranFunItem =NULL;
   RANfunctions_List_t *ranFunctionsList =NULL;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList =NULL;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem =NULL;

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
                                    duDb->e2NodeComponent[duDb->numOfE2NodeComponent].interfaceType = F1; 
                                    duDb->e2NodeComponent[duDb->numOfE2NodeComponent].componentId = \
                                       e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item.e2nodeComponentID.\
                                       choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]; 
                                    duDb->numOfE2NodeComponent++;
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
                     if(duDb->ranFunction[ranFuncItem->ranFunctionID-1].id != ranFuncItem->ranFunctionID)
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
                     if(duDb->ranFunction[ranFuncIdItem->ranFunctionID-1].id == ranFuncIdItem->ranFunctionID)
                     {
                        memset(&duDb->ranFunction[ranFuncIdItem->ranFunctionID-1], 0, sizeof(RanFunction));
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
   bool   ricReqIdDecoded = false;
   DuDb    *duDb = NULLP;
   RanFunction *ranFuncDb = NULLP;
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
                     ricReqIdDecoded = true;
                     break;
                  }
                  case ProtocolIE_IDE2_id_RANfunctionID:
                  {
                     ranFuncId = ricSubsFailIe->value.choice.RANfunctionID;
                     if(duDb->ranFunction[ranFuncId-1].id == ranFuncId)
                     {
                        ranFuncDb = &duDb->ranFunction[ranFuncId-1];
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

         /* Remove subscription entry from RAN Function */
         if(ranFuncDb && ricReqIdDecoded)
         {
            for(subsIdx = 0; subsIdx < ranFuncDb->numOfSubscription; subsIdx++)
            {
               if((ranFuncDb->subscriptionList[subsIdx].requestId.requestorId == ricReqId.requestorId) &&
                     (ranFuncDb->subscriptionList[subsIdx].requestId.instanceId == ricReqId.instanceId))
               {
                  memset(&ranFuncDb->subscriptionList[subsIdx], 0, sizeof(RicSubscription));
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
                     DU_LOG("\nINFO  -->  E2AP : RIC Indication Acknowledged");
                     break;
                  }
               case InitiatingMessageE2__value_PR_RICserviceUpdate:
                  {
                     DU_LOG("\nINFO  -->  E2AP : RIC Service update received");
                     ProcRicServiceUpdate(*duId,  &e2apMsg->choice.initiatingMessage->value.choice.RICserviceUpdate);
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
