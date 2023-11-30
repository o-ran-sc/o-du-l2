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
#include "E2connectionUpdate-Item.h"

/*******************************************************************
 *
 * @brief Printing Type and Cause of failure
 *
 * @details
 *
 *    Function : printE2ErrorCause
 *
 *    Functionality: Printing Type and Cause of failure
 *
 * @params[in] E2 Cause
 * @return void
 *
 ******************************************************************/

void printE2ErrorCause(CauseE2_t *cause)
{
   switch(cause->present)
   {
      case CauseE2_PR_ricRequest:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "RIC_Request", cause->choice.ricRequest);
            break;
         }
      case CauseE2_PR_ricService:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "RIC_Service", cause->choice.ricService);
            break;
         }
      case CauseE2_PR_e2Node:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "E2_Node", cause->choice.e2Node);
            break;
         }
      case CauseE2_PR_transport:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "Transport", cause->choice.transport);
            break;
         }
      case CauseE2_PR_protocol:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "Protocol", cause->choice.protocol);
            break;
         }
      case CauseE2_PR_misc:
         {
            DU_LOG("Failure_Type [%s] Cause [%ld]", "Miscellaneous", cause->choice.misc);
            break;
         }
      default:
         {
            DU_LOG("Failure_Type and Cause unknown");
            break;
         }
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
 * @brief add RIC Subs action info
 *
 * @details
 *
 *    Function : addRicSubsAction
 *
 * Functionality: add Ric Subs action info
 *
 * @parameter
 *    RIC action id
 *    List of action 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
CmLList *addRicSubsAction(uint8_t ricActionID, CmLListCp *actionSequence)
{
   ActionInfo *actionDb = NULLP;
   CmLList *actionNode=NULLP;
   
   RIC_ALLOC(actionDb, sizeof(ActionInfo));
   if(actionDb==NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      return NULLP;
   }
   
   actionDb->actionId = ricActionID;   
   RIC_ALLOC(actionNode, sizeof(CmLList));
   if(actionNode)
   {
      actionNode->node = (PTR) actionDb;
      cmLListAdd2Tail(actionSequence, actionNode);
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      RIC_FREE(actionDb, sizeof(ActionInfo));
      return NULLP;
   }
   
   return actionNode;
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
 * @brief Deallocate the memory allocated for RemovalRequest msg
 *
 * @details
 *
 *    Function : FreeRemovalRequest
 *
 *    Functionality:
 *       - freeing the memory allocated for RemovalRequest
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeRemovalRequest(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   E2RemovalRequest_t  *removalReq = NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         removalReq = &e2apMsg->choice.initiatingMessage->value.choice.E2RemovalRequest;
         if(removalReq->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < removalReq->protocolIEs.list.count; ieIdx++)
            {
               RIC_FREE(removalReq->protocolIEs.list.array[ieIdx], sizeof(E2RemovalRequestIEs_t));
            }
            RIC_FREE(removalReq->protocolIEs.list.array, removalReq->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build and send the removal request msg
 *
 * @details
 *
 *    Function : BuildAndSendRemovalRequest
 *
 *    Functionality:
 *         - Buld and send the removal request msg to E2 node
 *
 * @params[in]
 *    DU database
 *    Type of failure
 *    Cause of failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendRemovalRequest(DuDb *duDb)
{
   uint8_t ieIdx = 0, elementCnt = 0, transId = 0;
   uint8_t ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   E2RemovalRequest_t  *removalReq = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building Removal Request\n");

   do
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2removal;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_E2RemovalRequest;
      removalReq = &e2apMsg->choice.initiatingMessage->value.choice.E2RemovalRequest;

      elementCnt = 1;
      removalReq->protocolIEs.list.count = elementCnt;
      removalReq->protocolIEs.list.size = elementCnt * sizeof(E2RemovalRequestIEs_t *);

      RIC_ALLOC(removalReq->protocolIEs.list.array, removalReq->protocolIEs.list.size);
      if(!removalReq->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(removalReq->protocolIEs.list.array[ieIdx], sizeof(E2RemovalRequestIEs_t));
         if(!removalReq->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            break;
         }
      }

      /* In case of failure */
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      removalReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      removalReq->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      removalReq->protocolIEs.list.array[ieIdx]->value.present = E2RemovalRequestIEs__value_PR_TransactionID;
      transId = assignTransactionId(duDb);
      removalReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode removal request structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for removal request\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending removal request failed");
         break;
      }


      ret = ROK;
      break;
   }while(true);

   /* Free all memory */
   FreeRemovalRequest(e2apMsg);
   return ret;
}

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
RicSubscription *fetchSubsInfoFromRicReqId(RicRequestId ricReqId, RanFunction *ranFuncDb, CmLList **ricSubscriptionNode)
{
   RicSubscription *ricSubscriptionInfo = NULLP;

   /* Fetch subscription detail in RAN Function DB */
   CM_LLIST_FIRST_NODE(&ranFuncDb->subscriptionList, *ricSubscriptionNode);
   while(*ricSubscriptionNode)
   {
      ricSubscriptionInfo = (RicSubscription *)((*ricSubscriptionNode)->node);
      if(ricSubscriptionInfo && (ricSubscriptionInfo->requestId.requestorId == ricReqId.requestorId) &&
            (ricSubscriptionInfo->requestId.instanceId == ricReqId.instanceId))
      {
         break;
      }
      *ricSubscriptionNode = (*ricSubscriptionNode)->next;
      ricSubscriptionInfo = NULLP;
   }

   if(!ricSubscriptionInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : fetchSubsInfoFromRicReqId: Subscription not found for Requestor ID [%d] \
         Instance ID [%d] in RAN Function ID [%d]", ricReqId.requestorId, ricReqId.instanceId, ranFuncDb->id);
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
 *             List of action 
 * @return Action Info DB
 *         NULL, in case of failure
 *
 * ****************************************************************/
ActionInfo *fetchActionInfoFromActionId(uint8_t actionId, RicSubscription *ricSubscriptionInfo, CmLList ** actionNode)
{
   ActionInfo *actionInfoDb = NULLP;

   CM_LLIST_FIRST_NODE(&ricSubscriptionInfo->actionSequence, *actionNode);
   while(*actionNode)
   {
       actionInfoDb = (ActionInfo*)((*actionNode)->node);
      if(actionInfoDb->actionId == actionId)
      {
         break;
      }
      *actionNode= (*actionNode)->next;
   }
   if(actionInfoDb==NULLP) 
   {
      DU_LOG("\nERROR  -->  E2AP : fetchActionInfoFromActionId: Action Id [%d] not found in \
            subscription info [Requestor id : %d] [Instance Id : %d]", actionId,\
            ricSubscriptionInfo->requestId.requestorId, ricSubscriptionInfo->requestId.instanceId);

   }
   return actionInfoDb;
}

/******************************************************************
 *
 * @brief Search E2 node component with the help of interface type
 * and component Id
 *
 * @details
 *
 *    Function : fetchE2NodeComponentInfo
 *
 *    Functionality: Search E2 node component 
 *
 * @params[in]
 *       DU databse
 *       Type of interface
 *       Pointer to E2 component node to be searched
 * @return CmLList
 *
 * ****************************************************************/

E2NodeComponent *fetchE2NodeComponentInfo(DuDb *duDb, InterfaceType interfaceType,CmLList **e2ComponentNode)
{
   E2NodeComponent *e2NodeComponentInfo=NULLP;

   if(duDb->e2NodeComponent.count)
   {
      CM_LLIST_FIRST_NODE(&duDb->e2NodeComponent, *e2ComponentNode);
      while(*e2ComponentNode)
      {
         e2NodeComponentInfo = (E2NodeComponent*)((*e2ComponentNode)->node);
         if((e2NodeComponentInfo->interfaceType == interfaceType))
         {
            break;
         }

         *e2ComponentNode = (*e2ComponentNode)->next;
         e2NodeComponentInfo = NULLP;
      }
   }
   return e2NodeComponentInfo;
}

/*******************************************************************
 *
 * @brief update E2 node config list
 *
 * @details
 *
 *    Function : updateE2NodeConfigList
 *
 *    Functionality:
 *         - update E2 node config list
 * @params[in]
 *    DU databse
 *    Protocol Id
 *    Configuration which need to update in Database
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t updateE2NodeConfigList(DuDb **duDb, uint8_t protocolId, E2NodeConfigItem *tmpCfg)
{
   CmLList *node;
   E2NodeComponent * e2NodeComponentInfo;
   bool configFound= false;

   switch(protocolId)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAddition:
         {
            /* Adding the new e2 node component in DB*/
            RIC_ALLOC(e2NodeComponentInfo, sizeof(E2NodeComponent));
            e2NodeComponentInfo->interfaceType = tmpCfg->componentInfo.interfaceType;
            e2NodeComponentInfo->componentId =tmpCfg->componentInfo.componentId;
            RIC_ALLOC(node, sizeof(CmLList));
            if(node)
            {
               node->node = (PTR) e2NodeComponentInfo;
               cmLListAdd2Tail(&(*duDb)->e2NodeComponent, node);
               configFound =true;
            }
            else
            {
               DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for e2NodeComponentList node");
               RIC_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
               return RFAILED;
            }
            break;
         }
      case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate:
         {
            /* searching for information in a database */
            e2NodeComponentInfo = fetchE2NodeComponentInfo((*duDb),tmpCfg->componentInfo.interfaceType,  &node);
            if(!e2NodeComponentInfo)
            {
               DU_LOG("\nERROR  -->  E2AP : Failed to find the e2 component node");
               return RFAILED;
            }
            /* If the node is present then update the value */
            e2NodeComponentInfo->componentId =tmpCfg->componentInfo.componentId;
            configFound =true;
            break;
         }
      case ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval:
         {
            /* searching for information in a database */
            e2NodeComponentInfo = fetchE2NodeComponentInfo((*duDb),tmpCfg->componentInfo.interfaceType, &node);
            if(!e2NodeComponentInfo)
            {
               DU_LOG("\nERROR  -->  E2AP : Failed to find the e2 component node");
               return RFAILED;
            }
            /* Delete the node from the list  */
            e2NodeComponentInfo->componentId = tmpCfg->componentInfo.componentId;
            cmLListDelFrm(&(*duDb)->e2NodeComponent, node);
            RIC_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
            configFound =true;
            break;
         }
   }

   /* If the configuration update was successful, then mark the isSuccessful as
    * true; otherwise, mark it as false. */ 
   if(configFound == true)
      tmpCfg->isSuccessful = true;
   else
      tmpCfg->isSuccessful = false;

   return ROK;
}

/*******************************************************************
 *
 * @brief Handling the E2 node configuration depending on the add, 
 * update, or remove configuration type
 *
 * @details
 *
 *    Function : handleE2NodeComponentAction
 *
 *    Functionality:
 *         - Handling the E2 node configuration depending on the add,
 *         update, or remove configuration type
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

uint8_t handleE2NodeComponentAction(DuDb *duDb, PTR e2NodeCfg, uint8_t protocolId, E2NodeConfigItem *storeCfg)
{
   uint8_t configFound = ROK;
   E2NodeConfigItem tmpCfg;
   E2nodeComponentID_t componentId;
   E2nodeComponentInterfaceType_t interface;
   E2nodeComponentConfigAddition_Item_t *addCfg=NULL;
   E2nodeComponentConfigUpdate_Item_t *updateCfg=NULL;
   E2nodeComponentConfigRemoval_Item_t *removeCfg=NULL;
   
   /* fetching the interface and component id information from the e2NodeCfg */
   memset(storeCfg, 0, sizeof(E2NodeConfigItem));
   storeCfg->isSuccessful=false;
   memset(&tmpCfg, 0, sizeof(E2NodeConfigItem));

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
   
   /* Storing the information in temporary structure */
   tmpCfg.componentInfo.interfaceType = interface;

   switch(componentId.present)
   {
      case E2nodeComponentID_PR_e2nodeComponentInterfaceTypeF1:
         {
            if(componentId.choice.e2nodeComponentInterfaceTypeF1)
            {
               tmpCfg.componentInfo.componentId = componentId.choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0];
            }
            break;
         }
      default:
         {
            break;
         }
   }

   /* Updating the database configuration  */ 
   configFound = updateE2NodeConfigList(&duDb, protocolId, &tmpCfg);

   memcpy(storeCfg, &tmpCfg,sizeof(E2NodeConfigItem));  
   return configFound;

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
 * @params[in]
 *    DU id
 *    Pointer to E2nodeConfigurationUpdate
 * @return Void
 *
 ******************************************************************/

void ProcE2NodeConfigUpdate(uint32_t duId, E2nodeConfigurationUpdate_t *e2NodeConfigUpdate)
{
   DuDb    *duDb = NULLP;
   E2NodeConfigList tmpE2NodeList;
   uint16_t arrIdx=0;
   uint8_t ieIdx = 0, duIdx = 0, elementCnt=0, transId = 0; 
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
                     /* Storing the E2 node information in DB */
                     if(handleE2NodeComponentAction(duDb, (PTR)e2NodeAddItem, ProtocolIE_IDE2_id_E2nodeComponentConfigAddition,\
                     &tmpE2NodeList.addedE2Node[tmpE2NodeList.addedE2NodeCount++]) != ROK)
                     {
                        DU_LOG("\nERROR  -->  E2AP : Processing of E2 node component idx %d failed",arrIdx);
                     }
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

                     /* Updating the E2 node information in DB */
                     if(handleE2NodeComponentAction(duDb, (PTR)e2NodeUpdateItem, ProtocolIE_IDE2_id_E2nodeComponentConfigUpdate,\
                              &tmpE2NodeList.updatedE2Node[tmpE2NodeList.updatedE2NodeCount++]) != ROK)
                     {
                        DU_LOG("\nERROR  -->  E2AP : Processing of E2 node component idx %d failed",arrIdx);
                     }
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

                     /* Removing the E2 node information in DB */
                     if(handleE2NodeComponentAction(duDb, (PTR)e2NodeRemovalItem, ProtocolIE_IDE2_id_E2nodeComponentConfigRemoval,\
                              &tmpE2NodeList.removedE2Node[tmpE2NodeList.removedE2NodeCount++]) != ROK)
                     {
                        DU_LOG("\nERROR  -->  E2AP : Processing of E2 node component idx %d failed",arrIdx);
                     }
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
   /* If all of the IEs are processed successfully, we will send an e2 node
    * config update ack message. 
    * else we will be sendinf e2 node config update failure */
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
 * @brief fill e2 node configuration for ack msg 
 *
 * @details
 *
 *    Function : fillE2NodeConfigAck
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

uint8_t fillE2NodeConfigAck(PTR e2NodeCfg, uint8_t procedureCode, E2NodeComponent *componentInfo, bool isSuccessful)
{
   E2nodeComponentID_t *e2nodeComponentID=NULLP;
   E2nodeComponentInterfaceType_t *e2nodeComponentInterfaceType=NULLP;
   E2nodeComponentConfigurationAck_t *e2nodeComponentConfigurationAck=NULLP;
   E2nodeComponentConfigRemovalAck_Item_t *removalAckItem=NULLP;
   E2nodeComponentConfigUpdateAck_Item_t *updateAckItem=NULLP;
   E2nodeComponentConfigAdditionAck_Item_t *additionAckItem=NULLP;
   
   /* filling the interface type, component id, configuration ack based on the
    * e2 node configuration add, update, delete type  */
   switch(procedureCode)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
         {
            additionAckItem = (E2nodeComponentConfigAdditionAck_Item_t *)e2NodeCfg;
            e2nodeComponentInterfaceType = &((E2nodeComponentConfigAdditionAck_Item_t *)e2NodeCfg)->e2nodeComponentInterfaceType;
            e2nodeComponentID = &additionAckItem->e2nodeComponentID;
            e2nodeComponentConfigurationAck = &additionAckItem->e2nodeComponentConfigurationAck;
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
   if(componentInfo->interfaceType>=NG && componentInfo->interfaceType<=X2)
   {
      *e2nodeComponentInterfaceType = componentInfo->interfaceType;
   }
   else
   {
      DU_LOG("\nERROR  --> E2AP: Received an invalid interface value %d",componentInfo->interfaceType);
      return RFAILED;
   }

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
      e2nodeComponentID->choice.e2nodeComponentInterfaceTypeF1->gNB_DU_ID.buf[0]  = componentInfo->componentId;
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

uint8_t BuildE2nodeComponentConfigAdditionAck(E2nodeComponentConfigAdditionAck_List_t *e2NodeConfigAdditionAckList, \
uint16_t addedE2NodeCount, E2NodeConfigItem *addedE2Node)
{
   E2NodeComponent *e2NodeComponentInfo=NULLP;
   CmLList *node=NULLP;
   uint16_t arrIdx = 0;
   E2nodeComponentConfigAdditionAck_Item_t *e2NodeAddAckItem=NULLP;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *e2NodeAddAckItemIe=NULLP;
  
   e2NodeConfigAdditionAckList->list.count = addedE2NodeCount;
   
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
      e2NodeAddAckItemIe = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) e2NodeConfigAdditionAckList->list.array[arrIdx];
      e2NodeAddAckItemIe->id = ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck_Item;
      e2NodeAddAckItemIe->criticality = CriticalityE2_reject;
      e2NodeAddAckItemIe->value.present = E2nodeComponentConfigAdditionAck_ItemIEs__value_PR_E2nodeComponentConfigAdditionAck_Item;
      e2NodeAddAckItem = &e2NodeAddAckItemIe->value.choice.E2nodeComponentConfigAdditionAck_Item;

      /* Filling the e2 node config addition ack item */
      fillE2NodeConfigAck((PTR)&e2NodeAddAckItemIe->value.choice.E2nodeComponentConfigAdditionAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck,\
      &addedE2Node[arrIdx].componentInfo, addedE2Node[arrIdx].isSuccessful);
   }
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
   uint16_t ranFuncIdx = 0;
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
 *    Functionality: Builds and sends the E2SetupResponse
 *
 * @params[in] 
 *      Du datbase
 *      Trans id
 *      List of E2node cofniguration which needs to be send
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2SetupRsp(DuDb *duDb, uint8_t transId, E2NodeConfigList e2NodeList)
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
         value.choice.E2nodeComponentConfigAdditionAck_List, e2NodeList.addedE2NodeCount, e2NodeList.addedE2Node) != ROK)
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
uint8_t fillRicActionDef(RICactionDefinition_t *ricActionDef, uint8_t ricActionId, ConfigType configType)
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
      actionFormat1->granulPeriod = RIC_ACTION_GRANULARITY_PERIOD(configType, ricActionId); /* In ms */

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
   CmLList *actionNode=NULLP;
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
      actionNode = addRicSubsAction(ricActionId, &ricSubsDb->actionSequence);
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
      if(fillRicActionDef(actionItem->value.choice.RICaction_ToBeSetup_Item.ricActionDefinition, ricActionId, CONFIG_ADD) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      
      return ROK;
   }
   
   if(actionNode)
   {
      cmLListDelFrm(&ricSubsDb->actionSequence, actionNode);
      deleteActionSequence(actionNode);
   }
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

uint8_t BuildRicSubsDetails(RICsubscriptionDetails_t *subsDetails, RicSubscription *ricSubsInfo)
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
         ricSubsInfo) != ROK)
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
   uint8_t         elementCnt = 0;
   uint8_t         idx = 0;
   uint8_t         actionIdx = 0;
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   E2AP_PDU_t                 *e2apRicMsg = NULL;
   RICsubscriptionRequest_t   *ricSubscriptionReq;
   RanFunction  *ranFuncDb = &duDb->ranFunction[0];
   CmLList *ricSubsNode = NULLP;
   RicSubscription *ricSubsInfo = NULLP;

   DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Request\n");

   /* Allocate memory to store RIC subscription info in RIC DB */
   RIC_ALLOC(ricSubsInfo, sizeof(RicSubscription));
   if(!ricSubsInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

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
         &ricSubsInfo->requestId) != ROK)
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
      ricSubsInfo->ranFuncId = ranFuncDb->id;

      /* Filling RIC Subscription Details */
      idx++;
      ricSubscriptionReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICsubscriptionDetails;
      ricSubscriptionReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;
      if(BuildRicSubsDetails(&(ricSubscriptionReq->protocolIEs.list.array[idx]->value.choice.RICsubscriptionDetails),\
         ricSubsInfo) != ROK)
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

      /* Add RIC Subscription Info to RAN Function's RIC Subscription List */
      RIC_ALLOC(ricSubsNode , sizeof(CmLList));
      if(!ricSubsNode)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      ricSubsNode->node = (PTR)ricSubsInfo;
      cmLListAdd2Tail(&ranFuncDb->subscriptionList, ricSubsNode);

      ret = ROK;
      break;
   }

   if(ret == RFAILED)
   {
      RIC_FREE(ricSubsInfo, sizeof(RicSubscription));
      RIC_FREE(ricSubsNode , sizeof(CmLList));
   }

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
   CmLList *ricSubsNode = NULLP;
   CmLList *actionNode = NULLP;
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
                           actionNode=NULLP;
                           actionId = ((RICaction_NotAdmitted_ItemIEs_t *)(notAdmitList->list.array[notAdmitIdx]))->\
                              value.choice.RICaction_NotAdmitted_Item.ricActionID;

                           /* Remove action from RAN Function's subscription list */
                           ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
                           if(ricSubs)
                           {
                              action = fetchActionInfoFromActionId(actionId, ricSubs, &actionNode);
                              if(action)
                              {
                                 cmLListDelFrm(&ricSubs->actionSequence, actionNode);
                                 deleteActionSequence(actionNode);
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
 * @return void
 *
 ******************************************************************/

void ProcE2SetupReq(uint32_t *duId, E2setupRequest_t  *e2SetupReq)
{
   uint8_t arrIdx = 0, duIdx = 0, transId =0;
   uint16_t ranFuncIdx=0, e2NodeAddListIdx =0;
   E2NodeConfigList tmpE2NodeList;
   DuDb    *duDb = NULLP;
   bool ieProcessingFailed = false;
   E2nodeComponentConfigAddition_List_t *e2NodeAddList=NULLP;
   E2nodeComponentConfigAddition_ItemIEs_t *e2NodeAddItem=NULLP;
   RANfunction_ItemIEs_t *ranFuncItemIe=NULLP;
   RANfunction_Item_t  *ranFunItem=NULLP;
   RANfunctions_List_t *ranFunctionsList=NULLP;

   memset(&tmpE2NodeList, 0, sizeof(E2NodeConfigList));
   if(!e2SetupReq)
   {
      DU_LOG("\nERROR  -->  E2AP : e2SetupReq pointer is null");
      return;
   }
   if(!e2SetupReq->protocolIEs.list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : e2SetupReq array pointer is null");
      return;
   }

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
                        cmLListInit(&duDb->ranFunction[ranFunItem->ranFunctionID-1].subscriptionList);
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
                           /* Storing the E2 node information in DB */
                           e2NodeAddItem = (E2nodeComponentConfigAddition_ItemIEs_t *)e2NodeAddList->list.array[e2NodeAddListIdx];
                           if(handleE2NodeComponentAction(duDb, (PTR)&e2NodeAddItem->value.choice.E2nodeComponentConfigAddition_Item,\
                                    ProtocolIE_IDE2_id_E2nodeComponentConfigAddition, &tmpE2NodeList.addedE2Node[tmpE2NodeList.addedE2NodeCount++]) != ROK)
                           {
                              DU_LOG("\nERROR  -->  E2AP : Processing of E2 node component idx %d failed",e2NodeAddListIdx);
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
   
   if(BuildAndSendE2SetupRsp(duDb, transId, tmpE2NodeList) !=ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send E2 setup response");
   }
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
   ResetResponseE2_t *resetResponse =NULLP;

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
 * @brief Buld and send the Reset Response msg
 *
 * @details
 *
 *    Function : BuildAndSendResetResponse 
 *
 *    Functionality:
 *         - Buld and send the Reset Response Message
 *
 * @params[in] 
 *    DU id
 *    TransId Id
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendResetResponse(uint32_t duId, uint8_t transId)
{
   uint8_t           ieIdx = 0, elementCnt = 0;
   uint8_t           ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   ResetResponseE2_t *resetResponse=NULL;
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
   uint8_t ieIdx = 0, duIdx = 0;
   uint8_t ranFuncId = 0;
   DuDb    *duDb = NULLP;
   RanFunction *ranFuncDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   CmLList *ricSubsNode = NULLP;
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
                        ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
                        if(ricSubs)
                        {
                           cmLListDelFrm(&ranFuncDb->subscriptionList, ricSubsNode);
                           deleteRicSubscriptionNode(ricSubsNode);
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
   CmLList *ricSubsNode = NULLP;
   CmLList *actionNode = NULLP;
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

               ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
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
                  actionNode=NULLP;
                  actionToBeMod = (RICaction_RequiredToBeModified_ItemIEs_t *)actionToBeModList->list.array[actionIdx];
                  actionId = actionToBeMod->value.choice.RICaction_RequiredToBeModified_Item.ricActionID;
                  action = fetchActionInfoFromActionId(actionId, ricSubs, &actionNode);
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
                  actionNode=NULLP;
                  actionToBeRmv = (RICaction_RequiredToBeRemoved_ItemIEs_t *)actionToBeRmvList->list.array[actionIdx];
                  actionId = actionToBeRmv->value.choice.RICaction_RequiredToBeRemoved_Item.ricActionID;
                  action = fetchActionInfoFromActionId(actionId, ricSubs, &actionNode);
                  if(action)
                  {
                     tmpActionList.actionRemovedList[tmpActionList.numActionRemoved++] = actionId;
                     cmLListDelFrm(&ricSubs->actionSequence, actionNode);
                     deleteActionSequence(actionNode);
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
 * @brief Deallocate the memory allocated for ResetRequest msg
 *
 * @details
 *
 *    Function : FreeResetRequest
 *
 *    Functionality:
 *       - freeing the memory allocated for ResetRequest
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeResetRequest(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   ResetRequestE2_t  *resetReq = NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         resetReq = &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2;
         if(resetReq->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < resetReq->protocolIEs.list.count; ieIdx++)
            {
               RIC_FREE(resetReq->protocolIEs.list.array[ieIdx], sizeof(ResetRequestIEs_t));
            }
            RIC_FREE(resetReq->protocolIEs.list.array, resetReq->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Build and send the reset request msg
 *
 * @details
 *
 *    Function : BuildAndSendResetRequest
 *
 *    Functionality:
 *         - Buld and send the reset request msg to E2 node
 *
 * @params[in]
 *    DU database
 *    Type of failure 
 *    Cause of failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendResetRequest(DuDb *duDb, CauseE2_PR causePresent, uint8_t reason)
{
   uint8_t ieIdx = 0, elementCnt = 0, transId = 0;
   uint8_t ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   ResetRequestE2_t  *resetReq = NULLP;
   asn_enc_rval_t     encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building Reset Request\n");

   do
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetRequest(): Memory allocation for E2AP-PDU failed");
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetRequest(): Memory allocation for initiatingMessage");
         break;
      }

      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_Reset;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_ResetRequestE2;
      resetReq = &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2;

      elementCnt = 2;
      resetReq->protocolIEs.list.count = elementCnt;
      resetReq->protocolIEs.list.size = elementCnt * sizeof(ResetRequestIEs_t *);

      RIC_ALLOC(resetReq->protocolIEs.list.array, resetReq->protocolIEs.list.size);
      if(!resetReq->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetRequest(): Memory allocation failed for \
               Reset Request IE array");
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(resetReq->protocolIEs.list.array[ieIdx], sizeof(ResetRequestIEs_t));
         if(!resetReq->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : BuildAndSendResetRequest(): Memory allocation failed for \
                  Reset Request IE array element");
            break;
         }
      }

      /* In case of failure */
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      resetReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_TransactionID;
      resetReq->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      resetReq->protocolIEs.list.array[ieIdx]->value.present = ResetRequestIEs__value_PR_TransactionID;
      transId = assignTransactionId(duDb);
      resetReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      ieIdx++;
      resetReq->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      resetReq->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_ignore;
      resetReq->protocolIEs.list.array[ieIdx]->value.present = ResetRequestIEs__value_PR_CauseE2;
      fillE2FailureCause(&resetReq->protocolIEs.list.array[ieIdx]->value.choice.CauseE2, causePresent, reason);

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf,\
            encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode reset request structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG   -->  E2AP : Created APER encoded buffer for reset request\n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         }
#endif
      }
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duDb->duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Sending reset request failed");
         break;
      }


      ret = ROK;
      break;
   }while(true);

   /* Free all memory */
   FreeResetRequest(e2apMsg);
   return ret;
}

/******************************************************************
 *
 * @brief Delete Ric subscription action
 *
 * @details
 *
 *    Function : deleteActionSequence
 *
 *    Functionality: Delete Ric subscription action
 *
 * @params[in] Action info
 *
 * @return void
 *
 * ****************************************************************/
void deleteActionSequence(CmLList *actionNode)
{
   ActionInfo *action = NULLP;

   if(actionNode)
   {
      action = (ActionInfo*)actionNode->node;
      memset(action, 0, sizeof(ActionInfo));
      RIC_FREE(actionNode->node, sizeof(ActionInfo));
      RIC_FREE(actionNode, sizeof(CmLList));
   }
}

/******************************************************************
 *
 * @brief Delete Ric subscription action list
 *
 * @details
 *
 *    Function : deleteActionSequenceList
 *
 *    Functionality: Delete Ric subscription action list
 *
 * @params[in] Action info list
 *
 * @return void
 *
 * ****************************************************************/
void deleteActionSequenceList(CmLListCp *actionList)
{
   CmLList *actionNode=NULLP;

   CM_LLIST_FIRST_NODE(actionList, actionNode);
   while(actionNode)
   {
      cmLListDelFrm(actionList, actionNode);
      deleteActionSequence(actionNode);
      CM_LLIST_FIRST_NODE(actionList, actionNode);
   }

}

/******************************************************************
 *
 * @brief Delete Ric subscription node
 *
 * @details
 *
 *    Function : deleteRicSubscriptionNode
 *
 *    Functionality: Delete Ric subscription node
 *
 * @params[in] Ric subscription info
 *
 * @return void
 *
 * ****************************************************************/
void deleteRicSubscriptionNode(CmLList *subscriptionNode)
{
   uint8_t actionIdx=0;
   RicSubscription *ricSubscriptionInfo = NULLP;

   ricSubscriptionInfo = (RicSubscription*)subscriptionNode->node;
   
   deleteActionSequenceList(&ricSubscriptionInfo->actionSequence);
   
   memset(ricSubscriptionInfo, 0, sizeof(RicSubscription));
   RIC_FREE(subscriptionNode->node, sizeof(RicSubscription));
   RIC_FREE(subscriptionNode, sizeof(CmLList));
}

/*******************************************************************
 *
 * @brief Delete RIC subscription List
 *
 * @details
 *
 *    Function : deleteRicSubscriptionList 
 *
 * Functionality: Delete RIC subscription list
 *
 * @params[in] RIC Subscription list
 * @return void

 *
 ******************************************************************/
void deleteRicSubscriptionList(CmLListCp *subscriptionList)
{
   CmLList *subscriptionNode = NULLP;

   CM_LLIST_FIRST_NODE(subscriptionList, subscriptionNode);
   while(subscriptionNode)
   {
      cmLListDelFrm(subscriptionList, subscriptionNode);
      deleteRicSubscriptionNode(subscriptionNode);
      CM_LLIST_FIRST_NODE(subscriptionList, subscriptionNode);
   }
}

/*******************************************************************
 *
 * @brief process the E2 Reset Response
 *
 * @details
 *
 *    Function : ProcResetResponse 
 *
 * Functionality: Process E2 Reset Response 
 *
 * @params[in] 
 *       du Id
 *       Pointer to reset response 
 * @return void
 *
 ******************************************************************/

void ProcResetResponse(uint32_t duId, ResetResponseE2_t *resetRsp)
{
   uint8_t ieIdx = 0, duIdx =0;
   DuDb *duDb = NULLP;
   RanFunction *ranFuncDb = NULLP;
   uint16_t ranFuncIdx = 0;

   SEARCH_DU_DB(duIdx, duId, duDb); 
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }
   
   if(!resetRsp)
   {
      DU_LOG("\nERROR  -->  E2AP : resetRsp pointer is null"); 
      return;
   }

   if(!resetRsp->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : resetRsp array pointer is null");
      return;
   }
   
   for(ieIdx=0; ieIdx < resetRsp->protocolIEs.list.count; ieIdx++)
   {
      if(resetRsp->protocolIEs.list.array[ieIdx])
      {
         switch(resetRsp->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  for(ranFuncIdx = 0; ranFuncIdx < MAX_RAN_FUNCTION; ranFuncIdx++)
                  {
                     ranFuncDb = &duDb->ranFunction[ranFuncIdx];
                     if(ranFuncDb->id > 0)
                     {
                        deleteRicSubscriptionList(&ranFuncDb->subscriptionList);
                     }
                  }
                  break;
               }
            case ProtocolIE_IDE2_id_CriticalityDiagnosticsE2:
               {
                  break;
               }
         }
      }
   }
}


/*******************************************************************
 *
 * @brief process the E2 Reset Request
 *
 * @details
 *
 *    Function : ProcResetRequest 
 *
 * Functionality: Process E2 Reset Request 
 *
 * @params[in] 
 *       du Id
 *       Pointer to reset response 
 * @return void
 *
 ******************************************************************/

void ProcResetRequest(uint32_t duId, ResetRequestE2_t *resetReq)
{
   uint8_t ieIdx = 0, duIdx =0, transId=0;
   DuDb *duDb = NULLP;
   RanFunction *ranFuncDb = NULLP;
   uint16_t ranFuncIdx = 0;

   SEARCH_DU_DB(duIdx, duId, duDb); 
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }
   
   if(!resetReq)
   {
      DU_LOG("\nERROR  -->  E2AP : resetReq pointer is null"); 
      return;
   }

   if(!resetReq->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : resetReq array pointer is null");
      return;
   }
   
   for(ieIdx=0; ieIdx < resetReq->protocolIEs.list.count; ieIdx++)
   {
      if(resetReq->protocolIEs.list.array[ieIdx])
      {
         switch(resetReq->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  transId = resetReq->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
                  break;
               }
            case ProtocolIE_IDE2_id_CauseE2:
               {
                  for(ranFuncIdx = 0; ranFuncIdx < MAX_RAN_FUNCTION; ranFuncIdx++)
                  {
                     ranFuncDb = &duDb->ranFunction[ranFuncIdx];
                     if(ranFuncDb->id > 0)
                     {
                        deleteRicSubscriptionList(&ranFuncDb->subscriptionList);
                     }
                  }
                  break;
               }
         }
      }
   }

   if(BuildAndSendResetResponse(duId, transId) !=ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : Failed to build and send reset response");
   }
}

/*******************************************************************
 *
 * @brief Free RIC Subscription Delete Request Message
 *
 * @details
 *
 *    Function : FreeRicSubscriptionDeleteRequest
 *
 * Functionality:  Free RIC Subscription Delete Request
 *
 * @param  E2AP Message PDU
 * @return void
 *
 ******************************************************************/
void FreeRicSubscriptionDeleteRequest(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx = 0, arrIdx = 0;
   RICsubscriptionDeleteRequest_t *ricSubsDelReq = NULLP;

   if(e2apMsg)
   {
      if(e2apMsg->choice.initiatingMessage)
      {
         ricSubsDelReq = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionDeleteRequest;
         if(ricSubsDelReq->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < ricSubsDelReq->protocolIEs.list.count; ieIdx++)
            {
               RIC_FREE(ricSubsDelReq->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionDeleteRequired_IEs_t));
            }
            RIC_FREE(ricSubsDelReq->protocolIEs.list.array, ricSubsDelReq->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));;
   }
}

/*******************************************************************
 *
 * @brief Builds and Send RIC Subscription delete request
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionDeleteRequest
 *
 * Functionality: Build and send RIC subscription delete request.
 *
 * @params[in] DU ID
 *             RIC subscription info to be deleted
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/
uint8_t BuildAndSendRicSubscriptionDeleteRequest(uint32_t duId, RicSubscription *ricSubsDb)
{
   uint8_t elementCnt = 0, ieIdx = 0, ret = RFAILED;
   E2AP_PDU_t         *e2apMsg = NULLP;
   RICsubscriptionDeleteRequest_t *ricSubsDelReq = NULLP;
   RICsubscriptionDeleteRequest_IEs_t *ricSubsDelReqIe = NULLP;
   asn_enc_rval_t     encRetVal;        /* Encoder return value */

   while(true)
   {
      DU_LOG("\nINFO   -->  E2AP : Building RIC Subscription Delete Request Message\n");

      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation for E2AP-PDU failed at line %d",__func__, __LINE__);
         break;
      }

      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;
      RIC_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation for E2AP-PDU failed at line %d",__func__, __LINE__);
         break;
      }
      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscriptionDelete;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionDeleteRequest;

      ricSubsDelReq = &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionDeleteRequest;

      elementCnt = 2;
      ricSubsDelReq->protocolIEs.list.count = elementCnt;
      ricSubsDelReq->protocolIEs.list.size = elementCnt * sizeof(RICsubscriptionDeleteRequest_IEs_t *);

      RIC_ALLOC(ricSubsDelReq->protocolIEs.list.array, ricSubsDelReq->protocolIEs.list.size);
      if(ricSubsDelReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed for array elements at line %d",__func__, __LINE__);
         break;
      }

      for(ieIdx = 0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(ricSubsDelReq->protocolIEs.list.array[ieIdx], sizeof(RICsubscriptionDeleteRequest_IEs_t));
         if(ricSubsDelReq->protocolIEs.list.array[ieIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failed for index [%d] at line %d", \
                  __func__, ieIdx, __LINE__);
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;
      
      /* RIC Request ID */
      ieIdx = 0;
      ricSubsDelReqIe = ricSubsDelReq->protocolIEs.list.array[ieIdx];
      ricSubsDelReqIe->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubsDelReqIe->criticality = CriticalityE2_reject;
      ricSubsDelReqIe->value.present = RICsubscriptionDeleteRequest_IEs__value_PR_RICrequestID;
      ricSubsDelReqIe->value.choice.RICrequestID.ricRequestorID = ricSubsDb->requestId.requestorId;
      ricSubsDelReqIe->value.choice.RICrequestID.ricInstanceID = ricSubsDb->requestId.instanceId;

      /* RAN Function ID */
      ieIdx++;
      ricSubsDelReqIe = ricSubsDelReq->protocolIEs.list.array[ieIdx];
      ricSubsDelReqIe->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubsDelReqIe->criticality = CriticalityE2_reject;
      ricSubsDelReqIe->value.present = RICsubscriptionDeleteRequest_IEs__value_PR_RANfunctionID;
      ricSubsDelReqIe->value.choice.RANfunctionID = ricSubsDb->ranFuncId;

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);
      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RIC Subscription Delete Request Message (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RIC Subscription Delete Request Message \n");
#ifdef DEBUG_ASN_PRINT
         for(int i=0; i< encBufSize; i++)
         {
            printf("%x",encBuf[i]);
         } 
#endif
      }

      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR   -->  E2AP : Failed to send RIC Susbcription Delete Request Message");      
         break;
      }

      ret = ROK;
      break;
   }

   FreeRicSubscriptionDeleteRequest(e2apMsg);  
   return ret;
}

/*******************************************************************
 *
 * @brief Processing of RIC Subscription Delete Required
 *
 * @details
 *
 *    Function : ProcRicSubsDeleteReqd
 *
 * Functionality: Processing of RIC Subscription Delete Required
 *    When received, RIC stub will initiate the RIC subscription
 *    deletion procedure towards DU
 *
 * @param  DU ID
 *         RIC Subscription Delete Required IEs
 * @return ROK-success
 *         RFAILED-failure
 *
 ******************************************************************/
uint8_t ProcRicSubsDeleteReqd(uint32_t duId, RICsubscriptionDeleteRequired_t *ricSubsDelRqd)
{
   uint8_t ieIdx = 0, duIdx = 0;
   uint16_t arrIdx = 0;
   DuDb *duDb = NULLP;
   RicRequestId ricReqId;
   RanFunction *ranFuncDb = NULLP;
   RicSubscription *subsDb = NULLP;
   CmLList *ricSubsNode = NULLP;

   RICsubscriptionDeleteRequired_IEs_t *ricSubsDelRqdIe = NULLP;
   RICsubscription_List_withCause_t *ricSubsList = NULLP;
   RICsubscription_withCause_Item_t *subsItem = NULLP;

   memset(&ricReqId, 0, sizeof(RicRequestId));

   if(!ricSubsDelRqd)
   {
      DU_LOG("\nERROR  -->  E2AP : %s: Received NULL message", __func__);
      return RFAILED;
   }

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return RFAILED;
   }

   for(ieIdx = 0; ieIdx < ricSubsDelRqd->protocolIEs.list.count; ieIdx++)
   {
      ricSubsDelRqdIe = ricSubsDelRqd->protocolIEs.list.array[ieIdx];
      switch(ricSubsDelRqdIe->id)
      {
         case ProtocolIE_IDE2_id_RICsubscriptionToBeRemoved:
         {
            ricSubsList = &ricSubsDelRqdIe->value.choice.RICsubscription_List_withCause;
            for(arrIdx = 0; arrIdx < ricSubsList->list.count; arrIdx++)
            {
               subsItem = &(((RICsubscription_withCause_ItemIEs_t *)ricSubsList->list.array[arrIdx])->\
                  value.choice.RICsubscription_withCause_Item);
               ranFuncDb = fetchRanFuncFromRanFuncId(duDb, subsItem->ranFunctionID);
               if(!ranFuncDb)
               {
                  DU_LOG("\nERROR  -->  E2AP : %s: RAN Function ID [%ld] not found", __func__, subsItem->ranFunctionID);
                  return RFAILED;
               }
               
               ricReqId.requestorId = subsItem->ricRequestID.ricRequestorID;
               ricReqId.instanceId = subsItem->ricRequestID.ricInstanceID;
               subsDb = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
               if(!subsDb)
               {
                  DU_LOG("\nERROR  -->  E2AP : %s: RIC Subscription not found for Requestor_ID [%ld] Instance_ID [%ld]", \
                     __func__, subsItem->ricRequestID.ricRequestorID, subsItem->ricRequestID.ricInstanceID);
                  return RFAILED;
               }

               /* Delete RIC Subcription from RAN Function */
               cmLListDelFrm(&ranFuncDb->subscriptionList, ricSubsNode);
               
               /* Send RIC Subscription delete request and then free any memory
                * allocated to store subscription info at RIC */
               BuildAndSendRicSubscriptionDeleteRequest(duId, (RicSubscription *)ricSubsNode->node);
               deleteRicSubscriptionNode(ricSubsNode);
            }
            
            break;
         }
         default:
            break;
      }
   }  
   
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
   E2nodeComponentConfigAdditionAck_Item_t *e2NodeAdditionAckItemIe=NULLP;
   E2nodeComponentConfigRemovalAck_Item_t *e2NodeRemovalAckItemIe=NULLP;
   E2nodeComponentConfigUpdateAck_Item_t *e2NodeUpdateAckItemIe=NULLP;
   
    /* Extracting the component interface and configuration ack information from
     * e2NodeCfg based on the protocol id */
   switch(protocolIe)
   {
      case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
      {
         e2NodeAdditionAckItemIe= (E2nodeComponentConfigAdditionAck_Item_t*)e2NodeCfg;
         switch(e2NodeAdditionAckItemIe->e2nodeComponentInterfaceType)
         {
            case E2nodeComponentInterfaceType_f1:
               {
                  f1InterfaceInfo = e2NodeAdditionAckItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
                  break;
               }
            default:
               {
                  break;
               }

         }
         cfgAck = &e2NodeAdditionAckItemIe->e2nodeComponentConfigurationAck;
      }

      case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck:
      {
         e2NodeUpdateAckItemIe = (E2nodeComponentConfigUpdateAck_Item_t*)e2NodeCfg;
         switch(e2NodeUpdateAckItemIe->e2nodeComponentInterfaceType)
         {
            case E2nodeComponentInterfaceType_f1:
               {
                  f1InterfaceInfo = e2NodeUpdateAckItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
                  break;
               }
            default:
               {
                  break;
               }
         }
         cfgAck = &e2NodeUpdateAckItemIe->e2nodeComponentConfigurationAck;
      }

      case ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck:
      {
         e2NodeRemovalAckItemIe= (E2nodeComponentConfigRemovalAck_Item_t*)e2NodeCfg;
         switch(e2NodeRemovalAckItemIe->e2nodeComponentInterfaceType)
         {
            case E2nodeComponentInterfaceType_f1:
               {
                  f1InterfaceInfo = e2NodeRemovalAckItemIe->e2nodeComponentID.choice.e2nodeComponentInterfaceTypeF1;
                  break;
               }
            default:
               {
                  break;
               }
         }
         cfgAck = &e2NodeRemovalAckItemIe->e2nodeComponentConfigurationAck;
      }
   }
   
   /* Freeing the memory allocated to component interface and configuration ack */
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
   E2nodeConfigurationUpdateAcknowledge_t *updateAckMsg=NULL;
   E2nodeComponentConfigUpdateAck_ItemIEs_t *updateAckItemIe=NULL;
   E2nodeComponentConfigUpdateAck_List_t *updateAckList=NULL;
   E2nodeComponentConfigRemovalAck_ItemIEs_t *removalAckItemIe=NULL;
   E2nodeComponentConfigRemovalAck_List_t *removalAckList=NULL;
   E2nodeComponentConfigAdditionAck_ItemIEs_t *additionAckItemIte=NULL;
   E2nodeComponentConfigAdditionAck_List_t *additionAckList=NULL;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         updateAckMsg = &e2apMsg->choice.successfulOutcome->value.choice.E2nodeConfigurationUpdateAcknowledge;
         if(updateAckMsg->protocolIEs.list.array != NULLP)
         {
            for(arrIdx = 0; arrIdx < updateAckMsg->protocolIEs.list.count; arrIdx++)
            {
               if(updateAckMsg->protocolIEs.list.array[arrIdx])
               {
                  switch(updateAckMsg->protocolIEs.list.array[arrIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck:
                        {
                           additionAckList =&updateAckMsg->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List;
                           if(additionAckList->list.array)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<additionAckList->list.count; e2NodeConfigIdx++)
                              {
                                 additionAckItemIte = (E2nodeComponentConfigAdditionAck_ItemIEs_t*) additionAckList->list.array[e2NodeConfigIdx];
                                 if(additionAckItemIte)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigAdditionAck,\
                                    (PTR)&additionAckItemIte->value.choice.E2nodeComponentConfigAdditionAck_Item);
                                    RIC_FREE(additionAckItemIte, sizeof(E2nodeComponentConfigAdditionAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(additionAckList->list.array, additionAckList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck:
                        {
                           updateAckList =&updateAckMsg->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigUpdateAck_List;
                           if(updateAckList->list.array)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<updateAckList->list.count; e2NodeConfigIdx++)
                              {
                                 updateAckItemIe = (E2nodeComponentConfigUpdateAck_ItemIEs_t*) updateAckList->list.array[e2NodeConfigIdx];
                                 if(updateAckItemIe)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck,\
                                    (PTR)&updateAckItemIe->value.choice.E2nodeComponentConfigUpdateAck_Item);
                                    RIC_FREE(updateAckItemIe, sizeof(E2nodeComponentConfigUpdateAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(updateAckList->list.array, updateAckList->list.size);
                           }
                           break;
                        }
                     case ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck:
                        {
                           removalAckList =&updateAckMsg->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemovalAck_List;
                           if(removalAckList->list.array)
                           {
                              for(e2NodeConfigIdx=0; e2NodeConfigIdx<removalAckList->list.count; e2NodeConfigIdx++)
                              {
                                 removalAckItemIe = (E2nodeComponentConfigRemovalAck_ItemIEs_t*) removalAckList->list.array[e2NodeConfigIdx];
                                 if(removalAckItemIe)
                                 {
                                    freeE2NodeConfigItem(ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck,\
                                    (PTR)&removalAckItemIe->value.choice.E2nodeComponentConfigRemovalAck_Item);
                                    RIC_FREE(removalAckItemIe, sizeof(E2nodeComponentConfigRemovalAck_ItemIEs_t));
                                 }
                              }
                              RIC_FREE(removalAckList->list.array, removalAckList->list.size);
                           }
                           break;
                        }
                  }
                  RIC_FREE(updateAckMsg->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdateAcknowledge_IEs_t));
               }
            }
            RIC_FREE(updateAckMsg->protocolIEs.list.array, updateAckMsg->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
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

uint8_t BuildE2nodeComponentConfigRemovalAck(E2nodeComponentConfigRemovalAck_List_t *e2NodeConfigRemovalAckList,\
uint16_t removalE2NodeCount, E2NodeConfigItem *removaldE2Node)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigRemovalAck_ItemIEs_t *e2NodeRemovalAckItem=NULL;
   
   /* Filling the e2 node config removal ack list */
   e2NodeConfigRemovalAckList->list.count = removalE2NodeCount;
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
      
      /* Filling the e2 node config removal ack item */
      fillE2NodeConfigAck((PTR)&e2NodeRemovalAckItem->value.choice.E2nodeComponentConfigRemovalAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck,\
      &removaldE2Node[arrIdx].componentInfo, removaldE2Node[arrIdx].isSuccessful);
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

uint8_t BuildE2nodeComponentConfigUpdateAck(E2nodeComponentConfigUpdateAck_List_t *e2NodeConfigUpdateAckList,\
uint16_t updatedE2NodeCount, E2NodeConfigItem *updatedE2Node)
{
   uint8_t arrIdx = 0;
   E2nodeComponentConfigUpdateAck_ItemIEs_t *e2NodeUpdateAckItem=NULL;
   
   /* Filling the e2 node config update ack list */
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
      
      /* Filling the e2 node config update ack item */
      fillE2NodeConfigAck((PTR)&e2NodeUpdateAckItem->value.choice.E2nodeComponentConfigUpdateAck_Item, ProtocolIE_IDE2_id_E2nodeComponentConfigUpdateAck,\
      &updatedE2Node[arrIdx].componentInfo, updatedE2Node[arrIdx].isSuccessful);

   }
   return ROK;  
}

/*******************************************************************
 *
 * @brief Buld and send the E2 node config update ack msg 
 *
 * @details
 *
 *    Function : BuildAndSendE2NodeConfigUpdateAck 
 *
 *    Functionality:
 *         - Buld and send the E2 node config update ack msg
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
   uint8_t ret = RFAILED;
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
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__,__LINE__);
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_successfulOutcome;
      RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__,__LINE__);
         break;
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
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__,__LINE__);
         break;
      }
      
      for(arrIdx =0; arrIdx<elementCnt; arrIdx++)
      {
         RIC_ALLOC(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx], sizeof(E2nodeConfigurationUpdateAcknowledge_IEs_t));
         if(e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx] == NULLP)
         {
            
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__,__LINE__);
            break;
         }
      }
      
      if(arrIdx<elementCnt)
         break;

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
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigAdditionAck_List;
         if(BuildE2nodeComponentConfigAdditionAck(&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigAdditionAck_List,\
                  e2NodeList->addedE2NodeCount, e2NodeList->addedE2Node)!=ROK)

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
            DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config update ack list");
            break;
         }
      }
      if(e2NodeList->removedE2NodeCount)
      {
         arrIdx++;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->id = ProtocolIE_IDE2_id_E2nodeComponentConfigRemovalAck;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->criticality = CriticalityE2_reject;
         e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.present = E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigRemovalAck_List;
         if(BuildE2nodeComponentConfigRemovalAck(&e2NodeConfigUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.E2nodeComponentConfigRemovalAck_List,\
                  e2NodeList->removedE2NodeCount, e2NodeList->removedE2Node)!=ROK)

         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build E2Node Component config removal ack list");
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
         break;
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
         break;
      }
      ret = ROK;
      break;
   }while(true);
   
   FreeE2NodeConfigUpdateAck(e2apMsg);
   return ret;
}


/******************************************************************
 *
 * @brief Processes the E2 removal failure msg
 *
 * @details
 *
 *    Function : procE2RemovalFailure
 *
 *    Functionality: Processes the E2 removal failure msg
 *
 * @params[in] 
 *       E2 Removal Failure information
 *
 * @return void
 *
 * ****************************************************************/
void ProcE2RemovalFailure(E2RemovalFailure_t *e2RemovalFailure) 
{
   uint8_t ieIdx = 0, transId=0;
   CauseE2_t *cause = NULLP;

   if(!e2RemovalFailure)
   {
      DU_LOG("\nERROR  -->  E2AP : e2RemovalFailure pointer is null"); 
      return;
   }

   if(!e2RemovalFailure->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : e2RemovalFailure array pointer is null");
      return;
   }
   
   for(ieIdx=0; ieIdx < e2RemovalFailure->protocolIEs.list.count; ieIdx++)
   {
      if(e2RemovalFailure->protocolIEs.list.array[ieIdx])
      {
         switch(e2RemovalFailure->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  transId = e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
                  DU_LOG("\nERROR  -->  E2AP : Received transID %d", transId);
                  break;
               }
            case ProtocolIE_IDE2_id_CauseE2:
               {
                   cause = &e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.choice.CauseE2;
                   printE2ErrorCause(cause);
                   break; 
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%d]", e2RemovalFailure->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
}
/*******************************************************************
 *
 * @brief Delete E2 component node list
 *
 * @details
 *
 *    Function : deleteE2ComponentNodeList 
 *
 * Functionality: Delete E2 component node  list
 *
 * @params[in] E2 component node list
 * @return void

 *
 ******************************************************************/

void deleteE2ComponentNodeList(CmLListCp *componentList)
{
   E2NodeComponent *cfgInfo = NULLP;
   CmLList *e2ComponentNode = NULLP;

   CM_LLIST_FIRST_NODE(componentList, e2ComponentNode);
   while(e2ComponentNode)
   {
      cfgInfo = (E2NodeComponent*)e2ComponentNode->node;
      cmLListDelFrm(componentList, e2ComponentNode);
      memset(cfgInfo, 0, sizeof(E2NodeComponent));
      CM_LLIST_FIRST_NODE(componentList, e2ComponentNode);
   }
}

/*******************************************************************
 *
 * @brief process the E2 node information from ric db
 *
 * @details
 *
 *    Function : deleteE2NodeInfo
 *
 * Functionality: process the E2 node information from ric db
 *
 * @params[in] 
 *       du Id
 *      
 * @return void
 *
 ******************************************************************/
void deleteE2NodeInfo(DuDb *duDb)
{
   uint16_t ranFuncIdx =0;
   RanFunction *ranFuncDb=NULLP;

   DU_LOG("\nINFO  -->  E2AP : Removing all the E2 node information");
   for(ranFuncIdx = 0; ranFuncIdx < MAX_RAN_FUNCTION; ranFuncIdx++)
   {
      ranFuncDb = &duDb->ranFunction[ranFuncIdx];
      if(ranFuncDb->id > 0)
      {
         deleteRicSubscriptionList(&ranFuncDb->subscriptionList);
      }
   }
   deleteE2ComponentNodeList(&duDb->e2NodeComponent);
}

/*******************************************************************
 *
 * @brief process the E2 Removal Response
 *
 * @details
 *
 *    Function : ProcE2RemovalResponse 
 *
 * Functionality: Process E2 Removal Response 
 *
 * @params[in] 
 *       du Id
 *       Pointer to removal response 
 * @return void
 *
 ******************************************************************/

void ProcE2RemovalResponse(uint32_t duId, E2RemovalResponse_t *removalRsp)
{
   uint8_t ieIdx = 0, duIdx =0;
   DuDb *duDb = NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }

   if(!removalRsp)
   {
      DU_LOG("\nERROR  -->  E2AP : removalRsp pointer is null"); 
      return;
   }

   if(!removalRsp->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : removalRsp array pointer is null");
      return;
   }

   for(ieIdx=0; ieIdx < removalRsp->protocolIEs.list.count; ieIdx++)
   {
      if(removalRsp->protocolIEs.list.array[ieIdx])
      {
         switch(removalRsp->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  DU_LOG("\nINFO  -->  E2AP : Sending request to close the sctp connection");
                  cmInetClose(&sctpCb.e2LstnSockFd);
                  deleteE2NodeInfo(duDb);
                  exit(0);
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", removalRsp->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
}
/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2 Removal Failure
 *
 * @details
 *
 *    Function : FreeE2RemovalFailure
 *
 *    Functionality:
 *       - freeing the memory allocated for E2RemovalFailure
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return void
 *
 * ****************************************************************/
void FreeE2RemovalFailure(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   E2RemovalFailure_t *e2RemovalFailure=NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.unsuccessfulOutcome != NULLP)
      {
         e2RemovalFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2RemovalFailure;
         if(e2RemovalFailure->protocolIEs.list.array)
         {
            for(ieIdx=0; ieIdx < e2RemovalFailure->protocolIEs.list.count; ieIdx++)
            {
               if(e2RemovalFailure->protocolIEs.list.array[ieIdx])
               {
                  RIC_FREE(e2RemovalFailure->protocolIEs.list.array[ieIdx], sizeof(E2RemovalFailureIEs_t));
               }
            }
            RIC_FREE(e2RemovalFailure->protocolIEs.list.array, e2RemovalFailure->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Removal Failure msg
 *
 * @details
 *
 *    Function : BuildAndSendE2RemovalFailure
 *
 *    Functionality:
 *         - Buld and send the E2 Removal Failure Message
 * @params[in] 
 *    DU Id
 *    Trans Id 
 *    Type of failure 
 *    Cause of failure
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendRemovalFailure(uint32_t duId, uint16_t transId,  CauseE2_PR causePresent, uint8_t reason)
{
   uint8_t           ieIdx = 0, elementCnt = 0;
   uint8_t           ret = RFAILED;
   E2AP_PDU_t        *e2apMsg = NULLP;
   E2RemovalFailure_t *e2RemovalFailure=NULLP;
   asn_enc_rval_t    encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Removal Failure Message\n");
   do
   {
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_unsuccessfulOutcome;

      RIC_ALLOC(e2apMsg->choice.unsuccessfulOutcome, sizeof(UnsuccessfulOutcomeE2_t));
      if(e2apMsg->choice.unsuccessfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.unsuccessfulOutcome->procedureCode = ProcedureCodeE2_id_E2removal;
      e2apMsg->choice.unsuccessfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcomeE2__value_PR_E2RemovalFailure;
      e2RemovalFailure = &e2apMsg->choice.unsuccessfulOutcome->value.choice.E2RemovalFailure;

      elementCnt = 2;
      e2RemovalFailure->protocolIEs.list.count = elementCnt;
      e2RemovalFailure->protocolIEs.list.size = elementCnt * sizeof(E2RemovalFailureIEs_t *);
      RIC_ALLOC(e2RemovalFailure->protocolIEs.list.array, e2RemovalFailure->protocolIEs.list.size);
      if(!e2RemovalFailure->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx=0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(e2RemovalFailure->protocolIEs.list.array[ieIdx], sizeof(E2RemovalFailureIEs_t));
         if(!e2RemovalFailure->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_TransactionID;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.present = E2RemovalFailureIEs__value_PR_TransactionID;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      /* Cause */
      ieIdx++;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->id = ProtocolIE_IDE2_id_CauseE2;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_ignore;
      e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.present = ErrorIndicationE2_IEs__value_PR_CauseE2;
      fillE2FailureCause(&e2RemovalFailure->protocolIEs.list.array[ieIdx]->value.choice.CauseE2, causePresent, reason);
      
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 removal failure structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Removal Failure \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Removal Failure");
         break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeE2RemovalFailure(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2 Removal Response
 *
 * @details
 *
 *    Function : FreeE2RemovalResponse
 *
 *    Functionality:
 *       - freeing the memory allocated for E2RemovalResponse
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeE2RemovalResponse(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0;
   E2RemovalResponse_t *e2RemovalResponse=NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.successfulOutcome != NULLP)
      {
         e2RemovalResponse = &e2apMsg->choice.successfulOutcome->value.choice.E2RemovalResponse;
         if(e2RemovalResponse->protocolIEs.list.array)
         {
            for(ieIdx=0; ieIdx < e2RemovalResponse->protocolIEs.list.count; ieIdx++)
            {
               if(e2RemovalResponse->protocolIEs.list.array[ieIdx])
               {
                  RIC_FREE(e2RemovalResponse->protocolIEs.list.array[ieIdx], sizeof(E2RemovalResponseIEs_t));
               }
            }
            RIC_FREE(e2RemovalResponse->protocolIEs.list.array, e2RemovalResponse->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Removal Response msg
 *
 * @details
 *
 *    Function : BuildAndSendE2RemovalResponse
 *
 *    Functionality:
 *         - Buld and send the E2 Removal Response Message
 * @params[in] 
 *    Du Id
 *    Trans Id 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t BuildAndSendRemovalResponse(uint32_t duId, uint16_t transId)
{
   uint8_t ieIdx = 0, elementCnt = 0;
   uint8_t ret = RFAILED, duIdx =0;
   E2AP_PDU_t *e2apMsg = NULLP;
   DuDb *duDb = NULLP;
   E2RemovalResponse_t *e2RemovalResponse=NULLP;
   asn_enc_rval_t    encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Removal Response Message\n");
   do
   {
      SEARCH_DU_DB(duIdx, duId, duDb);
      if(duDb == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
         return;
      }
   
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_successfulOutcome;

      RIC_ALLOC(e2apMsg->choice.successfulOutcome, sizeof(SuccessfulOutcomeE2_t));
      if(e2apMsg->choice.successfulOutcome == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.successfulOutcome->procedureCode = ProcedureCodeE2_id_E2removal;
      e2apMsg->choice.successfulOutcome->criticality = CriticalityE2_reject;
      e2apMsg->choice.successfulOutcome->value.present = SuccessfulOutcomeE2__value_PR_E2RemovalResponse;
      e2RemovalResponse = &e2apMsg->choice.successfulOutcome->value.choice.E2RemovalResponse;

      elementCnt = 1;
      e2RemovalResponse->protocolIEs.list.count = elementCnt;
      e2RemovalResponse->protocolIEs.list.size = elementCnt * sizeof(E2RemovalResponseIEs_t *);
      RIC_ALLOC(e2RemovalResponse->protocolIEs.list.array, e2RemovalResponse->protocolIEs.list.size);
      if(!e2RemovalResponse->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx=0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(e2RemovalResponse->protocolIEs.list.array[ieIdx], sizeof(E2RemovalResponseIEs_t));
         if(!e2RemovalResponse->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      e2RemovalResponse->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_TransactionID;
      e2RemovalResponse->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      e2RemovalResponse->protocolIEs.list.array[ieIdx]->value.present = E2RemovalResponseIEs__value_PR_TransactionID;
      e2RemovalResponse->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = transId;

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 removal response structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Removal Response \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Removal Response");
         break;
      }

      ret = ROK;
      break;
   }while(true);

   FreeE2RemovalResponse(e2apMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Process Removal req received from RIC
 *
 * @details
 *
 *    Function : procE2RemovalRequest
 *
 * Functionality: Process Removal req received from RIC
 *
 * @param  
 *    DU id
 *    E2 Removal Request 
 * @return void
 *
 ******************************************************************/

void procE2RemovalRequest(uint32_t duId, E2RemovalRequest_t *removalReq)
{
   uint8_t arrIdx =0;
   uint16_t transId =0;

   DU_LOG("\nINFO   -->  E2AP : E2 Removal request received");
   
   for(arrIdx=0; arrIdx<removalReq->protocolIEs.list.count; arrIdx++)
   {
      switch(removalReq->protocolIEs.list.array[arrIdx]->id)
      {
         case ProtocolIE_IDE2_id_TransactionID:
            {
               transId = removalReq->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
               break;
            }
         default:
            {
               DU_LOG("\nERROR  -->  E2AP : Invalid IE recevied [%d]", transId);
               break;
            }
      }
   }
   
   if(transId>=0 && transId<=255)
   {
      if(BuildAndSendRemovalResponse(duId, transId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send Removal response");
      }
   }
   else
   {
      if(BuildAndSendRemovalFailure(duId, transId, CauseE2_PR_protocol, CauseE2Protocol_abstract_syntax_error_falsely_constructed_message) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to build and send Removal response");
      }
   }
}

/*******************************************************************
 *
 * @brief fill E2 connection update item
 *
 * @details
 *
 *    Function : fillE2connectionUpdateItem 
 *
 *    Functionality: fill E2 connection update item
 *
 * @params[in]
 *    E2connectionUpdate Item to be filled
 *    Protocol Id
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t fillE2connectionUpdateItem(PTR connectionInfo, uint8_t protocolId)
{
   E2connectionUpdateRemove_Item_t *connectionRemoveITem=NULLP;
   E2connectionUpdate_Item_t *connectionModifyItem=NULLP;
   TNLinformation_t *tnlInformation = NULLP;
   TNLusage_t  *tnlUsage=NULLP;

   switch(protocolId)
   {
      case ProtocolIE_IDE2_id_E2connectionUpdate_Item:
      {
         connectionModifyItem = (E2connectionUpdate_Item_t*)connectionInfo;
         tnlInformation = &connectionModifyItem->tnlInformation;
         tnlUsage = &connectionModifyItem->tnlUsage;
         break;
      }

      case ProtocolIE_IDE2_id_E2connectionUpdateRemove_Item:
      {
         connectionRemoveITem = (E2connectionUpdateRemove_Item_t*)connectionInfo;
         tnlInformation= &connectionRemoveITem->tnlInformation;
         break;
      }
   }
   
   tnlInformation->tnlAddress.size =  4*sizeof(uint8_t);
   RIC_ALLOC(tnlInformation->tnlAddress.buf, tnlInformation->tnlAddress.size);
   if(!tnlInformation->tnlAddress.buf)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
      return RFAILED;
   }
   
   tnlInformation->tnlAddress.buf[3] =  ricCb.ricCfgParams.sctpParams.localIpAddr.ipV4Addr & 0xFF; 
   tnlInformation->tnlAddress.buf[2] = (ricCb.ricCfgParams.sctpParams.localIpAddr.ipV4Addr>> 8) & 0xFF;
   tnlInformation->tnlAddress.buf[1] = (ricCb.ricCfgParams.sctpParams.localIpAddr.ipV4Addr>> 16) & 0xFF;
   tnlInformation->tnlAddress.buf[0] = (ricCb.ricCfgParams.sctpParams.localIpAddr.ipV4Addr>> 24) & 0xFF;
   tnlInformation->tnlAddress.bits_unused = 0;
   if(protocolId == ProtocolIE_IDE2_id_E2connectionUpdate_Item)
   {
      *tnlUsage = TNLusage_support_function; 
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Build E2 connection modification list
 *
 * @details
 *
 *    Function : BuildE2ConnectionModifyList
 *
 *    Functionality: Build E2 connection modification list
 *
 * @params[in]
 *    E2 connection modification list to be filled
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2ConnectionModifyList(E2connectionUpdate_List_t *connectionToBeModifyList)
{
   uint8_t arrIdx = 0;
   E2connectionUpdate_ItemIEs_t *connectionModify=NULL;

   connectionToBeModifyList->list.count = 1;
 
   connectionToBeModifyList->list.size = connectionToBeModifyList->list.count*sizeof(E2connectionUpdate_ItemIEs_t*);
   RIC_ALLOC(connectionToBeModifyList->list.array, connectionToBeModifyList->list.size);
   if(connectionToBeModifyList->list.array)
   {
      for(arrIdx = 0; arrIdx< connectionToBeModifyList->list.count; arrIdx++)
      {
         RIC_ALLOC(connectionToBeModifyList->list.array[arrIdx], sizeof(E2connectionUpdate_ItemIEs_t));
         if(connectionToBeModifyList->list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            return RFAILED;
         }
         connectionModify = (E2connectionUpdate_ItemIEs_t*)connectionToBeModifyList->list.array[arrIdx];
         connectionModify->id = ProtocolIE_IDE2_id_E2connectionUpdate_Item;
         connectionModify->criticality= CriticalityE2_ignore;
         connectionModify->value.present = E2connectionUpdate_ItemIEs__value_PR_E2connectionUpdate_Item;
         if(fillE2connectionUpdateItem((PTR)&connectionModify->value.choice.E2connectionUpdate_Item, ProtocolIE_IDE2_id_E2connectionUpdate_Item) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to fill E2 connection update item");
            return RFAILED;
         }
         
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Build E2 connection remove list
 *
 * @details
 *
 *    Function : BuildE2ConnectionRemoveList
 *
 *    Functionality: Build E2 connection remove list
 *
 * @params[in]
 *    E2 connection remove list to be filled
 *
 * @return ROK - success
 *         RFAILED - failure
 * ****************************************************************/

uint8_t BuildE2ConnectionRemoveList(E2connectionUpdateRemove_List_t *connectionToBeRemoveList)
{
   uint8_t arrIdx = 0;
   E2connectionUpdateRemove_ItemIEs_t *connectionRemove=NULL;

   connectionToBeRemoveList->list.count = 1;
 
   connectionToBeRemoveList->list.size = connectionToBeRemoveList->list.count*sizeof(E2connectionUpdateRemove_ItemIEs_t*);
   RIC_ALLOC(connectionToBeRemoveList->list.array, connectionToBeRemoveList->list.size);
   if(connectionToBeRemoveList->list.array)
   {
      for(arrIdx = 0; arrIdx< connectionToBeRemoveList->list.count; arrIdx++)
      {
         RIC_ALLOC(connectionToBeRemoveList->list.array[arrIdx], sizeof(E2connectionUpdateRemove_ItemIEs_t));
         if(connectionToBeRemoveList->list.array[arrIdx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            return RFAILED;
         }
         connectionRemove = (E2connectionUpdateRemove_ItemIEs_t*)connectionToBeRemoveList->list.array[arrIdx];
         connectionRemove->id = ProtocolIE_IDE2_id_E2connectionUpdateRemove_Item;
         connectionRemove->criticality= CriticalityE2_ignore;
         connectionRemove->value.present = E2connectionUpdateRemove_ItemIEs__value_PR_E2connectionUpdateRemove_Item;
         if(fillE2connectionUpdateItem((PTR)&connectionRemove->value.choice.E2connectionUpdateRemove_Item, ProtocolIE_IDE2_id_E2connectionUpdateRemove_Item) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to fill E2 connection update item");
            return RFAILED;
         }
         
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Deallocate the memory allocated for E2ConnectionUpdate msg
 *
 * @details
 *
 *    Function : FreeE2ConnectionUpdate
 *
 *    Functionality:
 *       - freeing the memory allocated for E2ConnectionUpdate
 *
 * @params[in] E2AP_PDU_t *e2apMsg
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void FreeE2ConnectionUpdate(E2AP_PDU_t *e2apMsg)
{
   uint8_t ieIdx =0, arrIdx=0;
   E2connectionUpdate_t  *connectionUpdate = NULLP;
   E2connectionUpdate_List_t *connectionToBeModifyList = NULLP;
   E2connectionUpdateRemove_List_t *connectionToBeRemoveList = NULLP;

   if(e2apMsg != NULLP)
   {
      if(e2apMsg->choice.initiatingMessage != NULLP)
      {
         connectionUpdate = &e2apMsg->choice.initiatingMessage->value.choice.E2connectionUpdate;
         if(connectionUpdate->protocolIEs.list.array)
         {
            for(ieIdx = 0; ieIdx < connectionUpdate->protocolIEs.list.count; ieIdx++)
            {
               if(connectionUpdate->protocolIEs.list.array[ieIdx])
               {
                  switch(connectionUpdate->protocolIEs.list.array[ieIdx]->id)
                  {
                     case ProtocolIE_IDE2_id_TransactionID:
                        break;

                     case ProtocolIE_IDE2_id_E2connectionUpdateModify:
                        {
                           connectionToBeModifyList = &connectionUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2connectionUpdate_List;
                           if(connectionToBeModifyList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < connectionToBeModifyList->list.count; arrIdx++)
                              {
                                 RIC_FREE(connectionToBeModifyList->list.array[arrIdx], sizeof(E2connectionUpdate_ItemIEs_t));
                              }
                              RIC_FREE(connectionToBeModifyList->list.array, connectionToBeModifyList->list.size);
                           }
                           break;
                        }

                     case ProtocolIE_IDE2_id_E2connectionUpdateRemove:
                        {
                           connectionToBeRemoveList = &connectionUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2connectionUpdateRemove_List;
                           if(connectionToBeRemoveList->list.array)
                           {
                              for(arrIdx = 0; arrIdx < connectionToBeRemoveList->list.count; arrIdx++)
                              {
                                 RIC_FREE(connectionToBeRemoveList->list.array[arrIdx], sizeof(E2connectionUpdateRemove_ItemIEs_t));
                              }
                              RIC_FREE(connectionToBeRemoveList->list.array, connectionToBeRemoveList->list.size);
                           }
                           break;
                        }
                  }
                  RIC_FREE(connectionUpdate->protocolIEs.list.array[ieIdx], sizeof(E2connectionUpdate_IEs_t));
               }
            }
            RIC_FREE(connectionUpdate->protocolIEs.list.array, connectionUpdate->protocolIEs.list.size);
         }
         RIC_FREE(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
 *
 * @brief Buld and send the E2 Connection Update msg
 *
 * @details
 *
 *    Function : BuildAndSendE2ConnectionUpdate
 *
 *    Functionality:
 *         - Buld and send the E2 Connection Update Message
 * @params[in] 
 *    Du Id
 *    E2 connection to be modify or delete
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t BuildAndSendE2ConnectionUpdate(uint32_t duId, E2Connection connectionInfo)
{
   uint8_t ieIdx = 0, elementCnt = 0;
   uint8_t ret = RFAILED, duIdx =0;
   DuDb    *duDb = NULLP;
   E2AP_PDU_t *e2apMsg = NULLP;
   E2connectionUpdate_t *e2ConnectionUpdate=NULLP;
   asn_enc_rval_t    encRetVal;       /* Encoder return value */

   DU_LOG("\nINFO   -->  E2AP : Building E2 Connection Update Message\n");
   do
   {
      SEARCH_DU_DB(duIdx, duId, duDb);
      if(duDb == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
         return RFAILED;
      }
   
      RIC_ALLOC(e2apMsg, sizeof(E2AP_PDU_t));
      if(e2apMsg == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }
      e2apMsg->present = E2AP_PDU_PR_initiatingMessage;

      RIC_ALLOC(e2apMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      if(e2apMsg->choice.initiatingMessage == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      e2apMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_E2connectionUpdate;
      e2apMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_E2connectionUpdate;
      e2ConnectionUpdate = &e2apMsg->choice.initiatingMessage->value.choice.E2connectionUpdate;

      elementCnt = 1;
      if(connectionInfo == MODIFY_CONNECTION) 
         elementCnt++;
      if(connectionInfo == REMOVE_CONNECTION)
         elementCnt++;

      e2ConnectionUpdate->protocolIEs.list.count = elementCnt;
      e2ConnectionUpdate->protocolIEs.list.size = elementCnt * sizeof(E2connectionUpdate_IEs_t*);
      RIC_ALLOC(e2ConnectionUpdate->protocolIEs.list.array, e2ConnectionUpdate->protocolIEs.list.size);
      if(!e2ConnectionUpdate->protocolIEs.list.array)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
         break;
      }

      for(ieIdx=0; ieIdx < elementCnt; ieIdx++)
      {
         RIC_ALLOC(e2ConnectionUpdate->protocolIEs.list.array[ieIdx], sizeof(E2connectionUpdate_IEs_t));
         if(!e2ConnectionUpdate->protocolIEs.list.array[ieIdx])
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at line %d", __func__, __LINE__);
            break;
         }
      }
      if(ieIdx < elementCnt)
         break;

      ieIdx = 0;
      e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_TransactionID;
      e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
      e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.present = E2connectionUpdate_IEs__value_PR_TransactionID;
      e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.choice.TransactionID = assignTransactionId(duDb);

      if(connectionInfo == MODIFY_CONNECTION)
      {
         ieIdx++;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_E2connectionUpdateModify;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.present = E2connectionUpdate_IEs__value_PR_E2connectionUpdate_List;
         if(BuildE2ConnectionModifyList(&e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2connectionUpdate_List) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build the connection update modify list");
            break;
         }
      }
      
      if(connectionInfo == REMOVE_CONNECTION)
      {
         ieIdx++;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->id =  ProtocolIE_IDE2_id_E2connectionUpdateRemove;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->criticality = CriticalityE2_reject;
         e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.present = E2connectionUpdate_IEs__value_PR_E2connectionUpdateRemove_List;
         if(BuildE2ConnectionRemoveList(&e2ConnectionUpdate->protocolIEs.list.array[ieIdx]->value.choice.E2connectionUpdateRemove_List) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed to build the connection update modify list");
            break;
         }
      }

      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode E2 connection update structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for E2 Connection Update \n");
         for(int i=0; i< encBufSize; i++)
         {
            DU_LOG("%x",encBuf[i]);
         }
      }

      /* Sending msg */
      if(SendE2APMsg(RIC_APP_MEM_REG, RIC_POOL, duId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed to send E2 Connection Update");
         break;
      }

      ret = ROK;
      break;
   }while(true);
   
   FreeE2ConnectionUpdate(e2apMsg);
   return ret;
}

/******************************************************************
 *
 * @brief Processes the E2 connection update failure msg
 *
 * @details
 *
 *    Function : procE2connectionUpdateFailure
 *
 *    Functionality: Processes the E2 connection update failure msg
 *
 * @params[in] 
 *       E2 connection update failure information
 *
 * @return void
 *
 * ****************************************************************/
void ProcE2connectionUpdateFailure(E2connectionUpdateFailure_t *updateFailure) 
{
   uint8_t ieIdx = 0;
   uint16_t transId=0;
   CauseE2_t *cause = NULLP;

   if(!updateFailure)
   {
      DU_LOG("\nERROR  -->  E2AP : updateFailure pointer is null"); 
      return;
   }

   if(!updateFailure->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : updateFailure array pointer is null");
      return;
   }
   
   for(ieIdx=0; ieIdx < updateFailure->protocolIEs.list.count; ieIdx++)
   {
      if(updateFailure->protocolIEs.list.array[ieIdx])
      {
         switch(updateFailure->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  transId = updateFailure->protocolIEs.list.array[ieIdx]->value.choice.TransactionID;
                  DU_LOG("\nERROR  -->  E2AP : Received transID %d", transId);
                  break;
               }
            case ProtocolIE_IDE2_id_CauseE2:
               {
                   cause = &updateFailure->protocolIEs.list.array[ieIdx]->value.choice.CauseE2;
                   printE2ErrorCause(cause);
                   break; 
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%d]", updateFailure->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
}

/*******************************************************************
 *
 * @brief process the E2 Connection update ack
 *
 * @details
 *
 *    Function : ProcE2ConnectionUpdateAck 
 *
 * Functionality: Process E2 Connection update ack 
 *
 * @params[in] 
 *       du Id
 *       Pointer to Connection update ack 
 * @return void
 *
 ******************************************************************/

void ProcE2ConnectionUpdateAck(uint32_t duId, E2connectionUpdateAcknowledge_t *connectionUpdateAck)
{
   uint16_t transId =0;
   uint32_t ipAddress=0;
   DuDb *duDb = NULLP;
   uint8_t ieIdx = 0, duIdx =0, arrIdx=0;
   E2connectionUpdate_Item_t *connectionSetupItem=NULLP;
   E2connectionUpdate_ItemIEs_t *connectionSetupItemIe=NULLP;
   E2connectionUpdate_List_t *connectionSetupList=NULLP;
   E2connectionSetupFailed_Item_t *setupFailedItem =NULLP;
   E2connectionSetupFailed_List_t *setupFailedList=NULLP;
   E2connectionSetupFailed_ItemIEs_t *setupFailedItemIe =NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }

   if(!connectionUpdateAck)
   {
      DU_LOG("\nERROR  -->  E2AP : connectionUpdateAck pointer is null"); 
      return;
   }

   if(!connectionUpdateAck->protocolIEs.list.array)      
   {
      DU_LOG("\nERROR  -->  E2AP : connectionUpdateAck array pointer is null");
      return;
   }

   for(ieIdx=0; ieIdx < connectionUpdateAck->protocolIEs.list.count; ieIdx++)
   {
      if(connectionUpdateAck->protocolIEs.list.array[ieIdx])
      {
         switch(connectionUpdateAck->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_TransactionID:
               {
                  transId = connectionUpdateAck->protocolIEs.list.array[arrIdx]->value.choice.TransactionID;
                  if(transId>255)
                  {
                     DU_LOG("\nERROR  -->  E2AP : Received invalid trans id %d ",transId);
                     return;
                  }
                  break;
               }
            case ProtocolIE_IDE2_id_E2connectionSetup:
               {
                  connectionSetupList=&connectionUpdateAck->protocolIEs.list.array[ieIdx]->value.choice.E2connectionUpdate_List;
                  if(connectionSetupList->list.array)
                  {
                     for(arrIdx = 0; arrIdx< connectionSetupList->list.count; arrIdx++)
                     {
                        connectionSetupItemIe = (E2connectionUpdate_ItemIEs_t*)connectionSetupList->list.array[arrIdx];
                        connectionSetupItem = &connectionSetupItemIe->value.choice.E2connectionUpdate_Item;
                        bitStringToInt(&connectionSetupItem->tnlInformation.tnlAddress, &ipAddress);
                        if(ricCb.ricCfgParams.sctpParams.localIpAddr.ipV4Addr == ipAddress)
                        {
                            ricCb.ricCfgParams.sctpParams.usage = connectionSetupItem->tnlUsage;
                        }
                     }
                  }
                  break;
               }

            case ProtocolIE_IDE2_id_E2connectionSetupFailed:
               {
                  setupFailedList=&connectionUpdateAck->protocolIEs.list.array[ieIdx]->value.choice.E2connectionSetupFailed_List;
                  if(setupFailedList->list.array)
                  {
                     for(arrIdx = 0; arrIdx< setupFailedList->list.count; arrIdx++)
                     {
                        setupFailedItemIe = (E2connectionSetupFailed_ItemIEs_t*)setupFailedList->list.array[arrIdx];
                        setupFailedItem = &setupFailedItemIe->value.choice.E2connectionSetupFailed_Item;
                        printE2ErrorCause(&setupFailedItem->cause);
                     }
                  }
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", connectionUpdateAck->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
}

/******************************************************************
 *
 * @brief Processes the Ric Subs delete failure msg
 *
 * @details
 *
 *    Function : procRicSubsDeleteFailure
 *
 *    Functionality: Processes the Ric Subs delete failure msg
 *
 * @params[in]
 *       Ric Subs delete failure information
 *
 * @return void
 *
 * ****************************************************************/
void ProcRicSubsDeleteFailure(RICsubscriptionDeleteFailure_t *ricSubsDeleteFail)
{
   uint8_t ieIdx = 0;
   uint16_t ranFuncId=0;
   CauseE2_t *cause = NULLP;
   RICrequestID_t  ricRequestID;

   if(!ricSubsDeleteFail)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsDeleteFail pointer is null");
      return;
   }

   if(!ricSubsDeleteFail->protocolIEs.list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsDeleteFail array pointer is null");
      return;
   }

   for(ieIdx=0; ieIdx < ricSubsDeleteFail->protocolIEs.list.count; ieIdx++)
   {
      if(ricSubsDeleteFail->protocolIEs.list.array[ieIdx])
      {
         switch(ricSubsDeleteFail->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_RICrequestID:
               {
                  memcpy(&ricSubsDeleteFail->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID, &ricRequestID, sizeof(RICrequestID_t));
                  DU_LOG("\nERROR  -->  E2AP : Received RicReqId %ld and InstanceId %ld", ricRequestID.ricRequestorID, ricRequestID.ricInstanceID);
                  break;
               }
            case ProtocolIE_IDE2_id_RANfunctionID:
               {
                  ranFuncId = ricSubsDeleteFail->protocolIEs.list.array[ieIdx]->value.choice.RANfunctionID;
                  DU_LOG("\nERROR  -->  E2AP : Received ranfuncId %d", ranFuncId);
                  break;
               }
            case ProtocolIE_IDE2_id_CauseE2:
               {
                   cause = &ricSubsDeleteFail->protocolIEs.list.array[ieIdx]->value.choice.CauseE2;
                   printE2ErrorCause(cause);
                   break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", ricSubsDeleteFail->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
}


/******************************************************************
 *
 * @brief Processes the Ric Subs delete rsp msg
 *
 * @details
 *
 *    Function : ProcRicSubsDeleteRsp
 *
 *    Functionality: Processes the Ric Subs delete rsp msg
 *
 * @params[in]
 *       Ric Subs delete rsp information
 *
 * @return void
 *
 * ****************************************************************/
void ProcRicSubsDeleteRsp(uint32_t duId, RICsubscriptionDeleteResponse_t *ricSubsDeleteRsp)
{
   uint8_t ieIdx = 0;
   uint8_t duIdx= 0;
   uint16_t ranFuncId=0;
   RanFunction *ranFuncDb = NULLP;
   RicRequestId ricReqId;
   DuDb    *duDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   CmLList *ricSubsNode = NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }

   if(!ricSubsDeleteRsp)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsDeleteRsp pointer is null");
      return;
   }

   if(!ricSubsDeleteRsp->protocolIEs.list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsDeleteRsp array pointer is null");
      return;
   }
   for(ieIdx=0; ieIdx < ricSubsDeleteRsp->protocolIEs.list.count; ieIdx++)
   {
      if(ricSubsDeleteRsp->protocolIEs.list.array[ieIdx])
      {
         switch(ricSubsDeleteRsp->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_RICrequestID:
               {
                  ricReqId.requestorId = ricSubsDeleteRsp->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID.ricRequestorID;
                  ricReqId.instanceId = ricSubsDeleteRsp->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID.ricInstanceID;
                  break;
               }
            case ProtocolIE_IDE2_id_RANfunctionID:
               {
                  ranFuncId = ricSubsDeleteRsp->protocolIEs.list.array[ieIdx]->value.choice.RANfunctionID;
                  ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncId);
                  if(!ranFuncDb)
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid Ran Function id %d received",ranFuncId);
                     return;
                  }

                  ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
                  if(ricSubs)
                  {
                     deleteRicSubscriptionNode(ricSubsNode);
                     DU_LOG("\nINFO  -->  E2AP : Ric subscription node deleted successfully");
                  }
                  else
                  {
                     DU_LOG("\nERROR  -->  E2AP : Ric subscription node is not present ");
                     return;
                  }
                  break;
               }
         }
      }
   }
}


 /******************************************************************
  *
  * @brief Processes the Ric Subs modification failure msg
  *
  * @details
  *
  *    Function : procRicSubsModificationFailure
  *
  *    Functionality: Processes the Ric Subs modification failure msg
  *
  * @params[in]
  *       Ric Subs modification failure information
  *
  * @return void
  *
  * ****************************************************************/
 void ProcRicSubsModificationFailure(RICsubscriptionModificationFailure_t *ricSubsModificationFail)
 {
    uint8_t ieIdx = 0;
    uint16_t ranFuncId=0;
    CauseE2_t *cause = NULLP;
    RICrequestID_t  ricRequestID;

    DU_LOG("\nINFO  -->  E2AP : Ric subscription modification failure received");

    if(!ricSubsModificationFail)
    {
       DU_LOG("\nERROR  -->  E2AP : ricSubsModificationFail pointer is null");
       return;
    }

    if(!ricSubsModificationFail->protocolIEs.list.array)
    {
       DU_LOG("\nERROR  -->  E2AP : ricSubsModificationFail array pointer is null");
       return;
    }

    for(ieIdx=0; ieIdx < ricSubsModificationFail->protocolIEs.list.count; ieIdx++)
    {
       if(ricSubsModificationFail->protocolIEs.list.array[ieIdx])
       {
          switch(ricSubsModificationFail->protocolIEs.list.array[ieIdx]->id)
          {
             case ProtocolIE_IDE2_id_RICrequestID:
                {
                   memcpy(&ricSubsModificationFail->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID, &ricRequestID, sizeof(RICrequestID_t));
                   DU_LOG("\nERROR  -->  E2AP : Received RicReqId %ld and InstanceId %ld", ricRequestID.ricRequestorID, ricRequestID.ricInstanceID);
                   break;
                }
             case ProtocolIE_IDE2_id_RANfunctionID:
                {
                   ranFuncId = ricSubsModificationFail->protocolIEs.list.array[ieIdx]->value.choice.RANfunctionID;
                   DU_LOG("\nERROR  -->  E2AP : Received ranfuncId %d", ranFuncId);
                   break;
                }
             case ProtocolIE_IDE2_id_CauseE2:
                {
                    cause = &ricSubsModificationFail->protocolIEs.list.array[ieIdx]->value.choice.CauseE2;
                    printE2ErrorCause(cause);
                    break;
                }
             default:
                {
                   DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", ricSubsModificationFail->protocolIEs.list.array[ieIdx]->id);
                   break;
                }
          }
       }
    }
 }

/*******************************************************************
 *
 * @brief Free RIC Subscription  action to be added list
 *
 * @details
 *
 *    Function : FreeRicSubsActionToBeAdded
 *
 *    Functionality: Free the RIC Subscription action to be added list
 *
 * @params[in] RICactions_ToBeAddedForModification_List_t *subsDetails
 * @return void
 *
 * ****************************************************************/
void FreeRicSubsActionToBeAdded(RICactions_ToBeAddedForModification_List_t *subsDetails)
{
   uint8_t elementIdx = 0;
   RICaction_ToBeAddedForModification_ItemIEs_t *addedActionItemIe=NULLP;

   if(subsDetails->list.array)
   {
      for(elementIdx = 0; elementIdx < subsDetails->list.count; elementIdx++)
      {
         if(subsDetails->list.array[elementIdx])
         {
            addedActionItemIe = (RICaction_ToBeAddedForModification_ItemIEs_t*)subsDetails->list.array[elementIdx];
            RIC_FREE(addedActionItemIe->value.choice.RICaction_ToBeAddedForModification_Item.ricActionDefinition.buf, \
            addedActionItemIe->value.choice.RICaction_ToBeAddedForModification_Item.ricActionDefinition.size);
            RIC_FREE(subsDetails->list.array[elementIdx], sizeof(RICaction_ToBeAddedForModification_ItemIEs_t));
         }
      }
      RIC_FREE(subsDetails->list.array, subsDetails->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free RIC Subscription  action to be removed list
 *
 * @details
 *
 *    Function : FreeRicSubsActionToBeRemoved
 *
 *    Functionality: Free the RIC Subscription action to be removed list
 *
 * @params[in] RICactions_ToBeRemovedForModification_List_t *subsDetails
 * @return void
 *
 * ****************************************************************/
void FreeRicSubsActionToBeRemoved(RICactions_ToBeRemovedForModification_List_t *subsDetails)
{
   uint8_t elementIdx = 0;

   if(subsDetails->list.array)
   {
      for(elementIdx = 0; elementIdx < subsDetails->list.count; elementIdx++)
      {
         RIC_FREE(subsDetails->list.array[elementIdx], sizeof(RICaction_ToBeRemovedForModification_ItemIEs_t));
      }
      RIC_FREE(subsDetails->list.array, subsDetails->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free RIC Subscription action to be modify
 *
 * @details
 *
 *    Function : FreeRicSubsActionToBeModified
 *
 *    Functionality: Free the RIC Subscription action to be modify
 *
 * @params[in] RICactions_ToBeModifiedForModification_List_t List
 * @return void
 *
 * ****************************************************************/
void FreeRicSubsActionToBeModified(RICactions_ToBeModifiedForModification_List_t *subsDetails)
{
   uint8_t elementIdx = 0;
   RICaction_ToBeModifiedForModification_ItemIEs_t *actionItem = NULLP;

   if(subsDetails->list.array)
   {
      for(elementIdx = 0; elementIdx < subsDetails->list.count; elementIdx++)
      {
         if(subsDetails->list.array[elementIdx])
         {
            actionItem = (RICaction_ToBeModifiedForModification_ItemIEs_t *)subsDetails->list.array[elementIdx];
            if(actionItem->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition)
            {
               RIC_FREE(actionItem->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition->buf, \
                  actionItem->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition->size);
               RIC_FREE(actionItem->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition, sizeof(RICactionDefinition_t));
            }
            RIC_FREE(subsDetails->list.array[elementIdx], sizeof(RICaction_ToBeModifiedForModification_ItemIEs_t))
         }
      }
      RIC_FREE(subsDetails->list.array, subsDetails->list.size);
   }
}

/*******************************************************************
 *
 * @brief Free RIC Subscription modification Request
 *
 * @details
 *
 *    Function :FreeRicSubscriptionModReq
 *
 * Functionality : Free RIC Subscription modification Request
 *
 * @params[in] E2AP_PDU
 * @return void
 *
 ******************************************************************/
void FreeRicSubscriptionModReq(E2AP_PDU_t *e2apRicMsg)
{
   uint8_t idx = 0;
   RICsubscriptionModificationRequest_t   *ricSubscriptionModReq =NULLP;
   RICsubscriptionModificationRequest_IEs_t *ricSubscriptionModReqIe=NULLP;

   if(e2apRicMsg)
   {
      if(e2apRicMsg->choice.initiatingMessage)
      {
         ricSubscriptionModReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionModificationRequest;
         if(ricSubscriptionModReq->protocolIEs.list.array)
         {
            for(idx=0; idx < ricSubscriptionModReq->protocolIEs.list.count; idx++)
            {
               if(ricSubscriptionModReq->protocolIEs.list.array[idx])
               {
                  ricSubscriptionModReqIe = ricSubscriptionModReq->protocolIEs.list.array[idx];
                  switch(ricSubscriptionModReq->protocolIEs.list.array[idx]->id)
                  {
                     case ProtocolIE_IDE2_id_RICrequestID:
                        break;
                     case ProtocolIE_IDE2_id_RANfunctionID:
                        break;
                     case ProtocolIE_IDE2_id_RICactionsToBeRemovedForModification_List:
                        {
                           FreeRicSubsActionToBeRemoved(&(ricSubscriptionModReqIe->value.choice.RICactions_ToBeRemovedForModification_List));
                           break;
                        }
                     case ProtocolIE_IDE2_id_RICactionsToBeModifiedForModification_List:
                        {
                           FreeRicSubsActionToBeModified(&(ricSubscriptionModReqIe->value.choice.RICactions_ToBeModifiedForModification_List));
                           break;
                        }
                     case ProtocolIE_IDE2_id_RICactionsToBeAddedForModification_List:
                        {
                           FreeRicSubsActionToBeAdded(&(ricSubscriptionModReqIe->value.choice.RICactions_ToBeAddedForModification_List));
                           break;
                        }
                     default:
                        {
                           DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", ricSubscriptionModReq->protocolIEs.list.array[idx]->id);
                           break;
                        }

                  }

                  RIC_FREE(ricSubscriptionModReq->protocolIEs.list.array[idx], sizeof(RICsubscriptionModificationRequest_IEs_t));
               }
            }
            RIC_FREE(ricSubscriptionModReq->protocolIEs.list.array, ricSubscriptionModReq->protocolIEs.list.size);
         }
         RIC_FREE(e2apRicMsg->choice.initiatingMessage, sizeof(InitiatingMessageE2_t));
      }
      RIC_FREE(e2apRicMsg, sizeof(E2AP_PDU_t));
   }
}

/*******************************************************************
*
* @brief Build Ric subscription action to be modify list
*
* @details
*
*    Function : BuildRicSubsActionToBeModify
*
*    Functionality: Build Ric subscription action to be modify list
*
* @params[in]
*    RICactions_ToBeModifiedForModification_List_t to be filled
*    Num of action to be modify
*    List of action to be modify
*
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/

uint8_t BuildRicSubsActionToBeModify(RICactions_ToBeModifiedForModification_List_t *modifyActionList, uint8_t numOfActionToBeModify, ActionInfo *actionToBeModify)
{
   uint8_t arrIdx=0;
   RICaction_ToBeModifiedForModification_ItemIEs_t *modifiedActionItemIe=NULLP;

   modifyActionList->list.count = numOfActionToBeModify;
   modifyActionList->list.size = modifyActionList->list.count *  sizeof(RICaction_ToBeModifiedForModification_ItemIEs_t*);
   RIC_ALLOC(modifyActionList->list.array, modifyActionList->list.size);
   if(!modifyActionList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< modifyActionList->list.count; arrIdx++)
   {
      RIC_ALLOC(modifyActionList->list.array[arrIdx], sizeof(RICaction_ToBeModifiedForModification_ItemIEs_t));
      if(!modifyActionList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }
      modifiedActionItemIe = (RICaction_ToBeModifiedForModification_ItemIEs_t*)modifyActionList->list.array[arrIdx];
      modifiedActionItemIe->id = ProtocolIE_IDE2_id_RICaction_ToBeModifiedForModification_Item;
      modifiedActionItemIe->criticality = CriticalityE2_ignore;
      modifiedActionItemIe->value.present = RICaction_ToBeModifiedForModification_ItemIEs__value_PR_RICaction_ToBeModifiedForModification_Item;
      modifiedActionItemIe->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionID = actionToBeModify[arrIdx].actionId;

      /* RIC Action Definition */
      RIC_ALLOC(modifiedActionItemIe->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition, sizeof(RICactionDefinition_t));
      if(!modifiedActionItemIe->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      if(fillRicActionDef(modifiedActionItemIe->value.choice.RICaction_ToBeModifiedForModification_Item.ricActionDefinition,\
               actionToBeModify[arrIdx].actionId, CONFIG_MOD) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

   }
   return ROK;
}

/*******************************************************************
*
* @brief Build Ric subscription action to be removed list
*
* @details
*
*    Function : BuildRicSubsActionToBeRemoved
*
*    Functionality: Build Ric subscription action to be removed list
*
* @params[in]
*    RICactions_ToBeRemovedForModification_List_t to be filled
*    Num Of Action To Be Remove 
*    Action remove list
*
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/

uint8_t BuildRicSubsActionToBeRemoved(RICactions_ToBeRemovedForModification_List_t *removeActionList, uint8_t numOfActionToBeRemove, ActionInfo *actionToBeRemove)
{
   uint8_t arrIdx=0;
   RICaction_ToBeRemovedForModification_ItemIEs_t *removeActionItemIe=NULLP;

   removeActionList->list.count = numOfActionToBeRemove;
   removeActionList->list.size = removeActionList->list.count *  sizeof(RICaction_ToBeRemovedForModification_ItemIEs_t*);
   RIC_ALLOC(removeActionList->list.array, removeActionList->list.size);
   if(!removeActionList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< removeActionList->list.count; arrIdx++)
   {
      RIC_ALLOC(removeActionList->list.array[arrIdx], sizeof(RICaction_ToBeRemovedForModification_ItemIEs_t));
      if(!removeActionList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }
      removeActionItemIe = (RICaction_ToBeRemovedForModification_ItemIEs_t*)removeActionList->list.array[arrIdx];
      removeActionItemIe->id = ProtocolIE_IDE2_id_RICaction_ToBeRemovedForModification_Item;
      removeActionItemIe->criticality = CriticalityE2_ignore;
      removeActionItemIe->value.present = RICaction_ToBeRemovedForModification_ItemIEs__value_PR_RICaction_ToBeRemovedForModification_Item;
      removeActionItemIe->value.choice.RICaction_ToBeRemovedForModification_Item.ricActionID = actionToBeRemove[arrIdx].actionId;
   }
   return ROK;
}

/*******************************************************************
*
* @brief Build Ric subscription action to be added list
*
* @details
*
*    Function : BuildRicSubsActionToBeAdded 
*
*    Functionality: Build Ric subscription action to be added list
*
* @params[in]
*    RICactions_ToBeAddedForModification_List_t to be filled
*    Num Of Action To Be added 
*    Action add list
*
* @return ROK     - success
*         RFAILED - failure
*
******************************************************************/

uint8_t BuildRicSubsActionToBeAdded(RICactions_ToBeAddedForModification_List_t *addedActionList, RicSubscription **ricSubsInfo, uint8_t numOfActionToBeAdded, ActionInfo *actionToBeAdded)
{
   uint8_t arrIdx=0;
   CmLList *actionNode=NULLP;
   RICaction_ToBeAddedForModification_ItemIEs_t *addedActionItemIe;

   addedActionList->list.count = numOfActionToBeAdded;
   addedActionList->list.size = addedActionList->list.count *  sizeof(RICaction_ToBeAddedForModification_ItemIEs_t*);
   RIC_ALLOC(addedActionList->list.array, addedActionList->list.size);
   if(!addedActionList->list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
      return RFAILED;
   }

   for(arrIdx = 0; arrIdx< addedActionList->list.count; arrIdx++)
   {
      RIC_ALLOC(addedActionList->list.array[arrIdx], sizeof(RICaction_ToBeAddedForModification_ItemIEs_t));
      if(!addedActionList->list.array[arrIdx])
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }
      addedActionItemIe = (RICaction_ToBeAddedForModification_ItemIEs_t*)addedActionList->list.array[arrIdx];
      addedActionItemIe->id = ProtocolIE_IDE2_id_RICaction_ToBeAddedForModification_Item;
      addedActionItemIe->criticality = CriticalityE2_ignore;
      addedActionItemIe->value.present = RICaction_ToBeAddedForModification_ItemIEs__value_PR_RICaction_ToBeAddedForModification_Item;
      addedActionItemIe->value.choice.RICaction_ToBeAddedForModification_Item.ricActionID = actionToBeAdded[arrIdx].actionId;
      
      addedActionItemIe->value.choice.RICaction_ToBeAddedForModification_Item.ricActionType = RICactionType_report;

      if(fillRicActionDef(&addedActionItemIe->value.choice.RICaction_ToBeAddedForModification_Item.ricActionDefinition, \
      actionToBeAdded[arrIdx].actionId, CONFIG_ADD) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }
      
      actionNode = addRicSubsAction((*ricSubsInfo)->actionSequence.count, &(*ricSubsInfo)->actionSequence);
      if(actionNode == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
         return RFAILED;
      }
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Builds and Send the RicSubscriptionModReq
 *
 * @details
 *
 *    Function : BuildAndSendRicSubscriptionModReq
 *
 * Functionality:Builds and Send the RicSubscriptionModReq
 *
 * @params[in]
 *    Du databse
 *    Ric subs information
 *    List of ric subs action which needs to modify/add/remove
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t BuildAndSendRicSubscriptionModReq(DuDb *duDb, RicSubscription **ricSubsInfo, RicSubsModReq ricSubsModReq)
{
   uint8_t         ret = RFAILED;
   uint8_t         elementCnt = 0;
   uint8_t         idx = 0, cfgIdx=0;
   asn_enc_rval_t  encRetVal;        /* Encoder return value */
   E2AP_PDU_t                 *e2apRicMsg = NULL;
   RICsubscriptionModificationRequest_t   *ricSubscriptionModReq;
   RanFunction  *ranFuncDb = &duDb->ranFunction[0];
   CmLList *subscriptionNode = NULLP;
   
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
      e2apRicMsg->choice.initiatingMessage->procedureCode = ProcedureCodeE2_id_RICsubscriptionModification;
      e2apRicMsg->choice.initiatingMessage->criticality = CriticalityE2_reject;
      e2apRicMsg->choice.initiatingMessage->value.present = InitiatingMessageE2__value_PR_RICsubscriptionModificationRequest;

      ricSubscriptionModReq = &e2apRicMsg->choice.initiatingMessage->value.choice.RICsubscriptionModificationRequest;

      /* Increasing the elment count based on the number of configured action to  be add, mod, delete */
      elementCnt = 2;
      if(ricSubsModReq.numOfActionToBeAdded)
         elementCnt++;
      if(ricSubsModReq.numOfActionToBeModify)
         elementCnt++;
      if(ricSubsModReq.numOfActionToBeRemove)
         elementCnt++;

      ricSubscriptionModReq->protocolIEs.list.count = elementCnt;
      ricSubscriptionModReq->protocolIEs.list.size  = elementCnt * sizeof(RICsubscriptionModificationRequest_IEs_t);

      /* Initialize the subscription members */
      RIC_ALLOC(ricSubscriptionModReq->protocolIEs.list.array, ricSubscriptionModReq->protocolIEs.list.size);
      if(ricSubscriptionModReq->protocolIEs.list.array == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
         break;
      }

      for(idx=0; idx<elementCnt; idx++)
      {
         RIC_ALLOC(ricSubscriptionModReq->protocolIEs.list.array[idx], sizeof(RICsubscriptionModificationRequest_IEs_t));
         if(ricSubscriptionModReq->protocolIEs.list.array[idx] == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }
      if(idx < elementCnt)
         break;

      /* Filling RIC Request Id */
      idx = 0;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICrequestID;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionModificationRequest_IEs__value_PR_RICrequestID;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricRequestorID = (*ricSubsInfo)->requestId.requestorId;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RICrequestID.ricInstanceID =  (*ricSubsInfo)->requestId.instanceId;

      /* Filling RAN Function Id */
      idx++;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RANfunctionID;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_reject;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->value.present =\
                                                                      RICsubscriptionModificationRequest_IEs__value_PR_RANfunctionID;
      ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RANfunctionID = (*ricSubsInfo)->ranFuncId;

      if(ricSubsModReq.numOfActionToBeRemove)
      {
         /* Filling RIC Subscription action to be removed */
         idx++;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionsToBeRemovedForModification_List;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_ignore;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->value.present = RICsubscriptionModificationRequest_IEs__value_PR_RICactions_ToBeRemovedForModification_List;
         if(BuildRicSubsActionToBeRemoved(&ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RICactions_ToBeRemovedForModification_List,\
                  ricSubsModReq.numOfActionToBeRemove, ricSubsModReq.actionToBeRemove) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }
      if(ricSubsModReq.numOfActionToBeModify)
      {
         /* Filling RIC Subscription action to be modified */
         idx++;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionsToBeModifiedForModification_List;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_ignore;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->value.present = RICsubscriptionModificationRequest_IEs__value_PR_RICactions_ToBeModifiedForModification_List;
         if(BuildRicSubsActionToBeModify(&ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RICactions_ToBeModifiedForModification_List,\
                  ricSubsModReq.numOfActionToBeModify, ricSubsModReq.actionToBeModify) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }

      if(ricSubsModReq.numOfActionToBeAdded)
      {
         /* Filling RIC Subscription action to be added */
         idx++;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->id = ProtocolIE_IDE2_id_RICactionsToBeAddedForModification_List;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->criticality = CriticalityE2_ignore;
         ricSubscriptionModReq->protocolIEs.list.array[idx]->value.present = RICsubscriptionModificationRequest_IEs__value_PR_RICactions_ToBeAddedForModification_List;
         if(BuildRicSubsActionToBeAdded(&ricSubscriptionModReq->protocolIEs.list.array[idx]->value.choice.RICactions_ToBeAddedForModification_List,\
         ricSubsInfo, ricSubsModReq.numOfActionToBeAdded, ricSubsModReq.actionToBeAdded) != ROK)
         {
            DU_LOG("\nERROR  -->  E2AP : Failed at [%s] : line [%d]", __func__, __LINE__);
            break;
         }
      }

      /* Prints the Msg formed */
      xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2apRicMsg);

      memset(encBuf, 0, ENC_BUF_MAX_LEN);
      encBufSize = 0;
      encRetVal = aper_encode(&asn_DEF_E2AP_PDU, 0, e2apRicMsg, PrepFinalEncBuf, encBuf);
      if(encRetVal.encoded == ENCODE_FAIL)
      {
         DU_LOG("\nERROR  -->  E2AP : Could not encode RicSubscriptionModRequest structure (at %s)\n",\
               encRetVal.failed_type ? encRetVal.failed_type->name : "unknown");
         break;
      }
      else
      {
         DU_LOG("\nDEBUG  -->  E2AP : Created APER encoded buffer for RicSubscriptionModRequest\n");
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

      ret = ROK;
      break;
   }

   FreeRicSubscriptionModReq(e2apRicMsg);
   return ret;
}

/*******************************************************************
 *
 * @brief Builds RicSubscriptionModReq
 *
 * @details
 *
 *    Function : BuildRicSubscriptionModReq
 *
 * Functionality:Builds the RicSubscriptionModReq
 *
 * @params[in]
 *    Du databse
 * @return void
 *
 * ****************************************************************/

void BuildRicSubsModificationReq(DuDb *duDb, RicSubscription *ricSubsInfo)
{
   CmLList *actionNode=NULLP;
   uint8_t actionToBeAdded =0;
   uint8_t actionIdx =0, tmpActionIdx=0;
   ActionInfo *actionInfoDb = NULLP;
   RicSubsModReq ricSubsModReq;

   if(ricSubsInfo)
   {
      memset(&ricSubsModReq, 0, sizeof(RicSubsModReq));
      

      CM_LLIST_FIRST_NODE(&ricSubsInfo->actionSequence, actionNode);
      while(actionNode)
      {
         actionInfoDb = (ActionInfo*)(actionNode->node);
         /* Change the condition based on the action required to be modiified or removed */
         if(((actionInfoDb->actionId+1)%2) == 0)
         {
            tmpActionIdx = ricSubsModReq.numOfActionToBeModify; 
            ricSubsModReq.actionToBeModify[tmpActionIdx].actionId = actionInfoDb->actionId;
            ricSubsModReq.numOfActionToBeModify++;
         }
         else
         {
            tmpActionIdx = ricSubsModReq.numOfActionToBeRemove; 
            ricSubsModReq.actionToBeRemove[tmpActionIdx].actionId = actionInfoDb->actionId;
            ricSubsModReq.numOfActionToBeRemove++;
         }
         actionNode= actionNode->next;
      }
      /* Change the value of actionToBeAdded based on the number of action required to be added */
      actionToBeAdded =1;
      tmpActionIdx = ricSubsInfo->actionSequence.count;
      for(actionIdx=0; actionIdx<actionToBeAdded; actionIdx++)
      {
         ricSubsModReq.actionToBeAdded[actionIdx].actionId = tmpActionIdx;
         ricSubsModReq.numOfActionToBeAdded++;
         tmpActionIdx++;
      }

      if(BuildAndSendRicSubscriptionModReq(duDb, &ricSubsInfo, ricSubsModReq) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : failed to build and send RIC Subscription Modification");
         return ;
      }
   }
}

/****************************************************************
 *
 * @brief Processing RIC Subscription action modified list
 *
 * @details
 *
 *    Function :ProcessingRicSubsActionModified 
 *
 *    Functionality: Processing the RIC Subscription action modified list
 *
 * @params[in] RICactions_AddedForModification_List_t
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionModified(RICactions_ModifiedForModification_List_t *actionModifiedList)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   ActionInfo *action=NULLP;
   CmLList *actionNode =NULLP;
   RICaction_ModifiedForModification_ItemIEs_t *modifiedActionItemIe =NULLP;

   if(actionModifiedList->list.array)
   {
      for(elementIdx = 0; elementIdx < actionModifiedList->list.count; elementIdx++)
      {
         if(actionModifiedList->list.array[elementIdx])
         {
            modifiedActionItemIe=(RICaction_ModifiedForModification_ItemIEs_t*)actionModifiedList->list.array[elementIdx];
            actionId = modifiedActionItemIe->value.choice.RICaction_ModifiedForModification_Item.ricActionID;
            DU_LOG("\nInfo  -->  E2AP : Action id %d modified successfully",  actionId);
         }

      }

   }
}

/****************************************************************
 *
 * @brief Processing RIC Subscription action added list
 *
 * @details
 *
 *    Function : ProcessingRicSubsActionAdded
 *
 *    Functionality: Processing RIC Subscription action added  list
 *
 * @params[in] RICactions_AddedForModification_List_t
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionAdded(RICactions_AddedForModification_List_t *actionAddedList)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   ActionInfo *action=NULLP;
   CmLList *actionNode =NULLP;
   RICaction_AddedForModification_ItemIEs_t *addedActionItemIe =NULLP;

   if(actionAddedList->list.array)
   {
      for(elementIdx = 0; elementIdx < actionAddedList->list.count; elementIdx++)
      {
         if(actionAddedList->list.array[elementIdx])
         {
            addedActionItemIe=(RICaction_AddedForModification_ItemIEs_t*)actionAddedList->list.array[elementIdx];
            actionId = addedActionItemIe->value.choice.RICaction_AddedForModification_Item.ricActionID;
            DU_LOG("\nInfo  -->  E2AP : Action id %d added successfully",  actionId);
         }

      }

   }
}

/****************************************************************
 *
 * @brief Processing RIC Subscription action deleted list
 *
 * @details
 *
 *    Function : ProcessingRicSubsActionRemoved
 *
 *    Functionality: Processing RIC Subscription action deleted list
 *
 * @params[in] RICactions_RemovedForModification_List_t
 *             Ric Subscription info
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionRemoved(RICactions_RemovedForModification_List_t *actionRemovedList, RicSubscription *ricSubs)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   ActionInfo *action=NULLP;
   CmLList *actionNode =NULLP;
   RICaction_RemovedForModification_ItemIEs_t *removedActionItemIe =NULLP;

   if(actionRemovedList->list.array)
   {
      for(elementIdx = 0; elementIdx < actionRemovedList->list.count; elementIdx++)
      {
         if(actionRemovedList->list.array[elementIdx])
         {
            removedActionItemIe=(RICaction_RemovedForModification_ItemIEs_t*)actionRemovedList->list.array[elementIdx];
            actionId = removedActionItemIe->value.choice.RICaction_RemovedForModification_Item.ricActionID;
            action = fetchActionInfoFromActionId(actionId, ricSubs, &actionNode);
            if(action)
            {
               cmLListDelFrm(&ricSubs->actionSequence, actionNode);
               deleteActionSequence(actionNode);
               DU_LOG("\nInfo  -->  E2AP : Action id %d removed successfully",  actionId);
            }
         }

      }

   }
}

/*******************************************************************
 *
 * @brief Processing RIC Subscription action failed to be
 * removed list
 *
 * @details
 *
 *    Function : ProcessingRicSubsActionFailedToBeRemoved
 *
 *    Functionality: Processing the RIC Subscription action failed
 *    to be removed list
 *
 * @params[in] RICactions_FailedToBeRemovedForModification_List_t 
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionFailedToBeRemoved(RICactions_FailedToBeRemovedForModification_List_t *actionFailedToBeRemoved)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   RICaction_FailedToBeRemovedForModification_ItemIEs_t *failedToBeRemovedActionItemIe =NULLP;

   if(actionFailedToBeRemoved->list.array)
   {
      for(elementIdx = 0; elementIdx < actionFailedToBeRemoved->list.count; elementIdx++)
      {
         if(actionFailedToBeRemoved->list.array[elementIdx])
         {
            failedToBeRemovedActionItemIe=(RICaction_FailedToBeRemovedForModification_ItemIEs_t*)actionFailedToBeRemoved->list.array[elementIdx];
            actionId = failedToBeRemovedActionItemIe->value.choice.RICaction_FailedToBeRemovedForModification_Item.ricActionID;
            DU_LOG("\nERROR  -->  E2AP : Failed to remove action id %d in %s", actionId, __func__);
            printE2ErrorCause(&failedToBeRemovedActionItemIe->value.choice.RICaction_FailedToBeRemovedForModification_Item.cause);
         }

      }

   }
}

/****************************************************************
 *
 * @brief Processing RIC Subscription action failed to be
 * add list
 *
 * @details
 *
 *    Function : ProcessingRicSubsActionFailedToBeAdded
 *
 *    Functionality: Processing the RIC Subscription action failed
 *    to be add list
 *
 * @params[in] RICactions_FailedToBeAddedForModification_List_t 
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionFailedToBeAdded(RICactions_FailedToBeAddedForModification_List_t *actionfailedToBeAddedList, RicSubscription *ricSubs)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   ActionInfo *action=NULLP;
   CmLList *actionNode =NULLP;
   RICaction_FailedToBeAddedForModification_ItemIEs_t *failedToBeAddedActionItemIe =NULLP;

   if(actionfailedToBeAddedList->list.array)
   {
      for(elementIdx = 0; elementIdx < actionfailedToBeAddedList->list.count; elementIdx++)
      {
         if(actionfailedToBeAddedList->list.array[elementIdx])
         {
            failedToBeAddedActionItemIe=(RICaction_FailedToBeAddedForModification_ItemIEs_t*)actionfailedToBeAddedList->list.array[elementIdx];
            actionId = failedToBeAddedActionItemIe->value.choice.RICaction_FailedToBeAddedForModification_Item.ricActionID;
            action = fetchActionInfoFromActionId(actionId, ricSubs, &actionNode);
            if(action)
            {
               cmLListDelFrm(&ricSubs->actionSequence, actionNode);
               deleteActionSequence(actionNode);
            }
            DU_LOG("\nERROR  -->  E2AP : Failed to remove action id %d in %s", actionId,__func__);
            printE2ErrorCause(&failedToBeAddedActionItemIe->value.choice.RICaction_FailedToBeAddedForModification_Item.cause);
         }

      }

   }
}

/*******************************************************************
 *
 * @brief Processing RIC Subscription action failed to be
 * modified list
 *
 * @details
 *
 *    Function :ProcessingRicSubsActionFailedToBeModified 
 *
 *    Functionality: Processing the RIC Subscription action failed
 *    to be modified list
 *
 * @params[in] RICactions_FailedToBeModifiedForModification_List_t 
 * @return void
 *
 * ****************************************************************/
void ProcessingRicSubsActionFailedToBeModified(RICactions_FailedToBeModifiedForModification_List_t *actionFailedToBeModifiedList)
{
   uint8_t actionId=0;
   uint8_t elementIdx = 0;
   RICaction_FailedToBeModifiedForModification_ItemIEs_t *failedToBeModifiedActionItemIe =NULLP;

   if(actionFailedToBeModifiedList->list.array)
   {
      for(elementIdx = 0; elementIdx < actionFailedToBeModifiedList->list.count; elementIdx++)
      {
         if(actionFailedToBeModifiedList->list.array[elementIdx])
         {
            failedToBeModifiedActionItemIe=(RICaction_FailedToBeModifiedForModification_ItemIEs_t*)actionFailedToBeModifiedList->list.array[elementIdx];
            actionId = failedToBeModifiedActionItemIe->value.choice.RICaction_FailedToBeModifiedForModification_Item.ricActionID;
            DU_LOG("\nERROR  -->  E2AP : Failed to remove action id %d in %s", actionId,__func__);
            printE2ErrorCause(&failedToBeModifiedActionItemIe->value.choice.RICaction_FailedToBeModifiedForModification_Item.cause);
         }

      }

   }
}

/******************************************************************
 *
 * @brief Processes the Ric Subs modification rsp msg
 *
 * @details
 *
 *    Function : ProcRicSubsModificationRsp
 *
 *    Functionality: Processes the Ric Subs modification rsp msg
 *
 * @params[in]
 *       Ric Subs modification rsp information
 *
 * @return void
 *
 * ****************************************************************/
void ProcRicSubsModificationRsp(uint32_t duId, RICsubscriptionModificationResponse_t *ricSubsModificationRsp)
{
   uint8_t ieIdx = 0;
   uint8_t duIdx= 0;
   uint16_t ranFuncId=0;
   RanFunction *ranFuncDb = NULLP;
   RicRequestId ricReqId;
   DuDb    *duDb = NULLP;
   RicSubscription *ricSubs = NULLP;
   CmLList *ricSubsNode = NULLP;

   SEARCH_DU_DB(duIdx, duId, duDb);
   if(duDb == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : duDb is not present for duId %d",duId);
      return;
   }

   if(!ricSubsModificationRsp)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsModificationRsp pointer is null");
      return;
   }

   if(!ricSubsModificationRsp->protocolIEs.list.array)
   {
      DU_LOG("\nERROR  -->  E2AP : ricSubsModificationRsp array pointer is null");
      return;
   }

   for(ieIdx=0; ieIdx < ricSubsModificationRsp->protocolIEs.list.count; ieIdx++)
   {
      if(ricSubsModificationRsp->protocolIEs.list.array[ieIdx])
      {
         switch(ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->id)
         {
            case ProtocolIE_IDE2_id_RICrequestID:
               {
                  ricReqId.requestorId = ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID.ricRequestorID;
                  ricReqId.instanceId = ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICrequestID.ricInstanceID;
                  break;
               }
            case ProtocolIE_IDE2_id_RANfunctionID:
               {
                  ranFuncId = ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RANfunctionID;
                  ranFuncDb = fetchRanFuncFromRanFuncId(duDb, ranFuncId);
                  if(!ranFuncDb)
                  {
                     DU_LOG("\nERROR  -->  E2AP : Invalid Ran Function id %d received",ranFuncId);
                     return;
                  }

                  ricSubs = fetchSubsInfoFromRicReqId(ricReqId, ranFuncDb, &ricSubsNode);
                  if(!ricSubs)
                  {
                     DU_LOG("\nERROR  -->  E2AP : Ric subscription node is not present ");
                     return;
                  }
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsRemovedForModification_List:
               {
                  ProcessingRicSubsActionRemoved(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_RemovedForModification_List, ricSubs);
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsFailedToBeRemovedForModification_List:
               {
                  ProcessingRicSubsActionFailedToBeRemoved(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_FailedToBeRemovedForModification_List);
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsModifiedForModification_List:
               {
                  ProcessingRicSubsActionModified(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_ModifiedForModification_List);
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsFailedToBeModifiedForModification_List:
               {
                  ProcessingRicSubsActionFailedToBeModified(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_FailedToBeModifiedForModification_List);
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsAddedForModification_List:
               {
                  ProcessingRicSubsActionAdded(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_AddedForModification_List);
                  break;
               }
            case ProtocolIE_IDE2_id_RICactionsFailedToBeAddedForModification_List:
               {
                  ProcessingRicSubsActionFailedToBeAdded(&ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->value.choice.RICactions_FailedToBeAddedForModification_List, ricSubs);
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : Received Invalid Ie [%ld]", ricSubsModificationRsp->protocolIEs.list.array[ieIdx]->id);
                  break;
               }
         }
      }
   }
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
                     ProcResetRequest(*duId,  &e2apMsg->choice.initiatingMessage->value.choice.ResetRequestE2);
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
               case InitiatingMessageE2__value_PR_RICsubscriptionDeleteRequired:
                  {
                      DU_LOG("\nINFO  -->  E2AP : RIC Subscription Delete Required");
                      ProcRicSubsDeleteReqd(*duId, \
                         &e2apMsg->choice.initiatingMessage->value.choice.RICsubscriptionDeleteRequired);
                      break;
                  }

               case InitiatingMessageE2__value_PR_ErrorIndicationE2:
                  {
                     DU_LOG("\nINFO  -->  E2AP : Error indication received");
                     break;
                  }
               case InitiatingMessageE2__value_PR_E2RemovalRequest:
                  {
                     DU_LOG("\nINFO  -->  E2AP : E2 Removal request received");
                     procE2RemovalRequest(*duId,\
                     &e2apMsg->choice.initiatingMessage->value.choice.E2RemovalRequest);
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
               case SuccessfulOutcomeE2__value_PR_ResetResponseE2:
                  {
                     DU_LOG("\nINFO  -->  E2AP : Reset response received");
                     ProcResetResponse(*duId,  &e2apMsg->choice.successfulOutcome->value.choice.ResetResponseE2);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_RICsubscriptionResponse:  
                  {
                     ProcRicSubscriptionResponse(*duId, \
                        &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionResponse);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_E2RemovalResponse:
                  {
                     ProcE2RemovalResponse(*duId, &e2apMsg->choice.successfulOutcome->value.choice.E2RemovalResponse);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_E2connectionUpdateAcknowledge:
                  {
                     ProcE2ConnectionUpdateAck(*duId, &e2apMsg->choice.successfulOutcome->value.choice.E2connectionUpdateAcknowledge);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_RICsubscriptionDeleteResponse:
                  {
                     ProcRicSubsDeleteRsp(*duId, &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionDeleteResponse);
                     break;
                  }
               case SuccessfulOutcomeE2__value_PR_RICsubscriptionModificationResponse:
                  {
                     ProcRicSubsModificationRsp(*duId, &e2apMsg->choice.successfulOutcome->value.choice.RICsubscriptionModificationResponse);
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
               case UnsuccessfulOutcomeE2__value_PR_E2RemovalFailure:
                  {
                     ProcE2RemovalFailure(&e2apMsg->choice.unsuccessfulOutcome->value.choice.E2RemovalFailure);
                     break;
                  }
               case UnsuccessfulOutcomeE2__value_PR_E2connectionUpdateFailure:
                  {
                     ProcE2connectionUpdateFailure(&e2apMsg->choice.unsuccessfulOutcome->value.choice.E2connectionUpdateFailure);
                     break;
                  }
               case UnsuccessfulOutcomeE2__value_PR_RICsubscriptionDeleteFailure:
                  {
                     ProcRicSubsDeleteFailure(&e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionDeleteFailure);
                     break;
                  }
               case UnsuccessfulOutcomeE2__value_PR_RICsubscriptionModificationFailure:
                  {
                     ProcRicSubsModificationFailure(&e2apMsg->choice.unsuccessfulOutcome->value.choice.RICsubscriptionModificationFailure);
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
