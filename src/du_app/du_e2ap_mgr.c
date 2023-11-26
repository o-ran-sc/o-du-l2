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
#include "common_def.h"
#include <sys/time.h>
#include "du_tmr.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "legtp.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
#include "du_e2ap_mgr.h"
#include "du_e2_conversions.h"
#include "du_e2ap_msg_hdl.h"
#include "du_cfg.h"
#include "du_sctp.h"
#include "du_mgr.h"
#include "du_mgr_main.h"
#include "du_utils.h"

/*******************************************************************
 *
 * @brief Assigns new transaction id to DU initiated procedure
 *
 * @details
 *
 *    Function : assignTransactionId
 *
 *    Functionality: Assigns new transaction id to a DU initiated
 *       procedure
 *
 * @params[in] Region region
 *             Pool pool
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t assignTransactionId()
{
   uint8_t currTransId = duCb.e2apDb.e2TransInfo.transIdCounter;

   /* Update to next valid value */
   duCb.e2apDb.e2TransInfo.transIdCounter++;
   if(duCb.e2apDb.e2TransInfo.transIdCounter == MAX_NUM_TRANSACTION)
      duCb.e2apDb.e2TransInfo.transIdCounter = 0;

   return currTransId;
}

/*******************************************************************
 *
 * @brief Decode subscription ID
 *
 * @details
 *
 *    Function : decodeSubscriptionId
 *
 *    Functionality: Decode subscription id to get RAN function ID
 *      and RIC Request ID
 *
 * @params[in] Subscription ID
 *             RAN Function ID to be extracted
 *             RIC Request ID to be extracted
 * @return Void
 *
 * ****************************************************************/
void decodeSubscriptionId(uint64_t subscriptionId, uint16_t *ranFuncId, RicRequestId *ricReqId)
{
   /* Extract following from 64 bit subscription-ID :
    *  First 16 MSB is unused
    *  Next 16 MSB = RAN-Function-ID
    *  Next 16 MSB = Requestor-ID in RIC-Request-ID
    *  Last 16 LSB = Instance-ID in RIC-Request-ID
    */
   ricReqId->instanceId = subscriptionId & 0xFFFF;
   ricReqId->requestorId = (subscriptionId >> 16) & 0xFFFF;
   *ranFuncId = (subscriptionId >> 32) & 0xFFFF;
}

/*******************************************************************
 *
 * @brief Encode subscription ID
 *
 * @details
 *
 *    Function : encodeSubscriptionId
 *
 *    Functionality: Encode subscription id to get RAN function ID
 *      and RIC Request ID
 *
 * @params[in] Subscription ID to be encoded
 *             RAN Function ID 
 *             RIC Request ID
 * @return Void
 *
 * ****************************************************************/
void encodeSubscriptionId(uint64_t *subscriptionId, uint16_t ranFuncId, RicRequestId ricReqId)
{
   /* Calculate 64 bit subscription-ID :
    *  First 16 MSB is unused
    *  Next 16 MSB = RAN-Function-ID
    *  Next 16 MSB = Requestor-ID in RIC-Request-ID
    *  Last 16 LSB = Instance-ID in RIC-Request-ID
    */
   *subscriptionId = ricReqId.instanceId;
   *subscriptionId |= ((uint64_t)ricReqId.requestorId << 16);
   *subscriptionId |= ((uint64_t)ranFuncId << 32);
}

/*******************************************************************
 *
 * @brief Stores the current time at the start of reporting period
 *
 * @details
 *
 *    Function : storeReportStartTime
 *
 *    Functionality: Stores the current time at the start of 
 *       reporting period
 *
 * @params[in] Start Timer to be stored
 * @return Void
 *
 * ****************************************************************/
void storeReportStartTime(ReportStartTime *startTime)
{
   struct timeval tv;

   gettimeofday(&tv, NULL); 
   startTime->timeInSec = tv.tv_sec;
   startTime->timeInMilliSec = (tv.tv_usec/1000);
}


/*******************************************************************
 *
 * @brief Fetch Measurement Info using measurement type name
 *
 * @details
 *
 *    Function : fetchMeasInfoFromMeasTypeName
 *
 *    Functionality: Fetch Measurement Info using measurement type name
 *
 * @params[in] Measurement type name to search
 *             Measurement Info list to search from
 *             Measurement Info node found from list
 * @return Measurement Info DB
 *
 * ****************************************************************/
MeasurementInfo *fetchMeasInfoFromMeasTypeName(char *e2MeasTypeName, CmLListCp *measInfoList, CmLList **measInfoNode)
{
   MeasurementInfo *measInfo = NULLP;

   /* Fetch subscription detail in RAN Function DB */
   CM_LLIST_FIRST_NODE(measInfoList, *measInfoNode);
   while(*measInfoNode)
   {
      measInfo = (MeasurementInfo *)((*measInfoNode)->node);
      if(measInfo && !strcmp(e2MeasTypeName, measInfo->measurementTypeName))
      {
         break;
      }
      *measInfoNode = (*measInfoNode)->next;
      measInfo = NULLP;
   }

   if(!measInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : fetchMeasInfoFromMeasTypeName: Measurement [%s] not found", e2MeasTypeName);
   }

   return measInfo;
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
 *             Ric Action Node
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
      if(actionInfoDb && actionInfoDb->actionId == actionId)
      {
         break;
      }
      *actionNode= (*actionNode)->next;
      actionInfoDb = NULLP;
   }
   
   if(!actionInfoDb) 
   {
      DU_LOG("\nERROR  -->  E2AP : fetchActionInfoFromActionId: Action Id [%d] not found in \
            subscription info [Requestor id : %d] [Instance Id : %d]", actionId,\
            ricSubscriptionInfo->requestId.requestorId, ricSubscriptionInfo->requestId.instanceId);

   }
   return actionInfoDb;
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
RanFunction *fetchRanFuncFromRanFuncId(uint16_t ranFuncId)
{
   RanFunction *ranFuncDb = NULLP;

   /* Fetch RAN Function DB */
   if(duCb.e2apDb.ranFunction[ranFuncId-1].id == ranFuncId)
   {
      ranFuncDb = &duCb.e2apDb.ranFunction[ranFuncId-1];
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : fetchRanFuncFromRanFuncId: Invalid RAN Function ID[%d]", ranFuncId);
   }

   return ranFuncDb;
}

/*******************************************************************
 *
 * @brief Fetches subscription info
 *
 * @details
 *
 *    Function : fetchSubsInfoFromSubsId
 *
 *    Functionality: 
 *       1. Firstly, RAN Function ID and RIC request ID is extracted
 *          from Subscription ID 
 *       2. Using RAN Function ID, RAN Function DB is searched
 *       3. Using RIC Request ID, the subscription DB is searched in
 *          RAN Function DB
 *
 * @params[in] Subscription ID
 *             RAN Function DB
 *             RIC Subscription node from RAN Func's Subscription list
 * @return ROK
 *         RFAILED
 *
 * ****************************************************************/
uint8_t fetchSubsInfoFromSubsId(uint64_t subscriptionId, RanFunction **ranFuncDb, CmLList **ricSubscriptionNode, \
   RicSubscription **ricSubscriptionInfo)
{
   uint16_t ranFuncId = 0;
   RicRequestId ricReqId;

   memset(&ricReqId, 0, sizeof(RicRequestId));

   /* Decode subscription ID o get RIC Request ID and RAN Function ID */
   decodeSubscriptionId(subscriptionId, &ranFuncId, &ricReqId);

   /* Fetch RAN Function DB using RAN Function ID */
   *ranFuncDb = fetchRanFuncFromRanFuncId(ranFuncId);
   if((*ranFuncDb) == NULLP)
   {
      return RFAILED;
   }

   /* Fetch Sunscription DB from RAN Function using RIC Request ID */
   *ricSubscriptionInfo = fetchSubsInfoFromRicReqId(ricReqId, *ranFuncDb, ricSubscriptionNode);
   if((*ricSubscriptionInfo) == NULLP)
   {
      return RFAILED;
   }

   return ROK;
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

uint8_t SendE2APMsg(Region region, Pool pool, char *encBuf, int encBufSize)
{
   Buffer *mBuf=NULLP;

   if(ODU_GET_MSG_BUF(region, pool, &mBuf) == ROK)
   {
      if(ODU_ADD_POST_MSG_MULT((Data *)encBuf, encBufSize, mBuf) == ROK)
      {
         ODU_PRINT_MSG(mBuf, 0,0);

         if(sctpSend(mBuf, E2_INTERFACE) != ROK)
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
 * @brief Resets E2 
 *
 * @details
 *
 *    Function : ResetE2Request
 *
 *    Functionality: This function resets E2.
 *       As per ORAN WG3 E2GAP v3.0 Spec, section 5.5.3
 *       If E2 node initates reset procedure then:
 *        a. Send reset request to RIC
 *        b. Delete any pre-established RIC subscriptions
 *        c. Gracefully terminates any ongoing RIC services
 *       If RIC initiates reset procedure then :
 *        a. Delete any pre-established RIC subscriptions
 *        b. Gracefully terminates any ongoing RIC services
 *        c. Send reset response to RIC
 *
 * @params[in]
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t ResetE2Request(E2ProcedureDirection dir, E2FailureCause resetCause)
{
   /* Send Reset Request to RIC if DU detects any abnormal failure */
   if(dir == E2_NODE_INITIATED)
   {
      if(BuildAndSendE2ResetRequest(resetCause) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : BuildAndSendE2ResetRequest failed");
         return RFAILED;
      }
   }

   /* TODO when RIC subscription service model is implemented
      Process following steps of resetting E2
      1. Deletes any pre-established RIC subscriptions
      2. Gracefully terminates any ongoing RIC services
    */

   /* Send Reset Response if RIC initiated Reset request is received at DU */
   if(dir == RIC_INITIATED)
   {
      //BuildAndSendE2ResetResponse();
   }   
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RIC Subscription datils in MAC Statistics Request
 *
 * @details
 *
 *    Function : fillRicSubsInMacStatsReq
 *
 *    Functionality: Fill RIC Subscription datils in MAC Statistics 
 *       Request
 *
 * @params[in] MAC Statistics Request to be filled
 *             RIC Subscription Info
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRicSubsInMacStatsReq(MacStatsReq *macStatsReq, RicSubscription* ricSubscriptionInfo)
{
   uint8_t    actionIdx = 0, grpIdx = 0, statsIdx = 0;
   uint64_t   subscriptionId = 0;
   ActionInfo *actionDb = NULLP;
   CmLList *actionNode = NULLP;
   ActionDefFormat1 *format1Action = NULLP;

   /* Generate subscription ID using RIC Request ID and RAN Function ID */
   encodeSubscriptionId(&subscriptionId, ricSubscriptionInfo->ranFuncId, ricSubscriptionInfo->requestId);

   macStatsReq->subscriptionId = subscriptionId;
   CM_LLIST_FIRST_NODE(&ricSubscriptionInfo->actionSequence, actionNode);
   while(actionNode)
   {
      actionDb = (ActionInfo*)(actionNode->node);
      if(actionDb->action == CONFIG_ADD)
      {
         macStatsReq->statsGrpList[grpIdx].groupId = actionDb->actionId;
         switch(actionDb->definition.formatType)
         {
            case 1:
               {
                  format1Action = &actionDb->definition.choice.format1;
                  macStatsReq->statsGrpList[grpIdx].periodicity = format1Action->granularityPeriod;

                  CmLList *node = NULLP;
                  MeasurementInfo *measInfo = NULLP;
                  statsIdx = 0;
                  /* Update DL PRB Usage for all stats group which requested for DL Total PRB Usage */
                  node = cmLListFirst(&format1Action->measurementInfoList);
                  while(node)
                  {
                     measInfo = (MeasurementInfo *)(node->node);
                     switch(measInfo->measurementTypeId)
                     {
                        case 1:
                           {
                              macStatsReq->statsGrpList[grpIdx].statsList[statsIdx++] = MAC_DL_TOTAL_PRB_USAGE;
                              break;
                           }
                        case 2:
                           {
                              macStatsReq->statsGrpList[grpIdx].statsList[statsIdx++] = MAC_UL_TOTAL_PRB_USAGE;
                              break;
                           }
                        default:
                           {
                              DU_LOG("\nERROR  -->  E2AP : Invalid measurement name");
                              break;
                           }
                     }
                     node = node->next;
                  }
                  macStatsReq->statsGrpList[grpIdx].numStats = statsIdx;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : fillRicSubsInMacStatsReq: Only Action Definition Format 1 supported");
                  break;
               }
         }
         if(macStatsReq->statsGrpList[grpIdx].numStats)
            grpIdx++;
      }
      actionNode = actionNode->next;
   }

   macStatsReq->numStatsGroup = grpIdx;
   if(macStatsReq->numStatsGroup)
   {
      return ROK;
   }
   return RFAILED;
}

/*******************************************************************
 *
 * @brief Rejects all actions received in a subscription request
 *
 * @details
 *
 *    Function : duRejectAllStatsGroup
 *
 *    Functionality: Rejects all actions received in a subscription
 *       request by :
 *       a. Removing the subscription entry from RAN function
 *       b. Sending RIC Subscription Failure to RIC with appropriate
 *          cause of failure
 *
 * @params[in] RAN Function DB
 *             Subscription entry in RAN Function subscription list
 *             Statistics Response from MAC
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t rejectAllStatsGroup(RanFunction *ranFuncDb, CmLList *ricSubscriptionNode, MacStatsRsp *statsRsp)
{
   uint8_t ret = ROK;
   RicRequestId  requestId;
   E2FailureCause failureCause;

   /* Delete subcription from RAN Function */
   memcpy(&requestId, &((RicSubscription *)ricSubscriptionNode->node)->requestId, sizeof(RicRequestId));
   cmLListDelFrm(&ranFuncDb->subscriptionList, ricSubscriptionNode);
   deleteRicSubscriptionNode(ricSubscriptionNode);
   ricSubscriptionNode = NULLP;
   convertDuCauseToE2Cause(statsRsp->statsGrpRejectedList[0].cause, &failureCause);

   /* Send RIC subscription failure to RIC */
   ret = BuildAndSendRicSubscriptionFailure(requestId, ranFuncDb->id, failureCause);
   return ret;
}

/*******************************************************************
 *
 * @brief Process statistics response from MAC
 *
 * @details
 *
 *    Function : e2ProcStatsRsp
 *
 *    Functionality: Processes statistics configuration response
 *       from MAC. If configuration is succsessful, DUAPP starts
 *       reporting period timer for this subscription request
 *       from RIC
 *
 * @params[in] Statistics response received from MAC
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t e2ProcStatsRsp(MacStatsRsp *statsRsp)
{
   uint8_t idx = 0;
   uint8_t actionId = 0;
   uint32_t reportingPeriod = 0;
   RanFunction *ranFuncDb = NULLP;
   CmLList *ricSubscriptionNode = NULLP;
   CmLList *actionNode = NULLP;
   RicSubscription *ricSubscriptionInfo = NULLP;
   ActionInfo *actionInfoDb = NULLP;
   PendingSubsRspInfo *pendingSubsRsp = NULLP;

   /* Fetch RAN Function and Subscription DB using subscription Id received in statistics response */
   if(fetchSubsInfoFromSubsId(statsRsp->subscriptionId, &ranFuncDb, &ricSubscriptionNode, &ricSubscriptionInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : DuProcMacStatsRsp: Failed to fetch subscriprtion details");
      return RFAILED;
   }

   /* Fetch pre-stored statistics response info by DU APP */
   for(idx=0; idx<ranFuncDb->numPendingSubsRsp; idx++)
   {
      if((ranFuncDb->pendingSubsRspInfo[idx].requestId.requestorId == ricSubscriptionInfo->requestId.requestorId) &&
            (ricSubscriptionInfo->requestId.instanceId == ricSubscriptionInfo->requestId.instanceId))
      {
         pendingSubsRsp = &ranFuncDb->pendingSubsRspInfo[idx];
         break;
      }
   }

   /* If no action is accepted
    *  a. Remove subcription entry from RAN Function
    *  b. Send RIC subscription failure */
   if(statsRsp->numGrpAccepted == 0)
   {
      rejectAllStatsGroup(ranFuncDb, ricSubscriptionNode, statsRsp);
   }
   else
   {
      /* Once RIC subscription is successful, mark the config action as unknown */
      ricSubscriptionInfo->action = CONFIG_UNKNOWN;

      /* Start RIC Subscription reporting timer */
      switch(ricSubscriptionInfo->eventTriggerDefinition.formatType)
      {
         case 1:
            {
               reportingPeriod = ricSubscriptionInfo->eventTriggerDefinition.choice.format1.reportingPeriod;

               /* Save the start time of reporting period */
               storeReportStartTime(&ricSubscriptionInfo->eventTriggerDefinition.choice.format1.startTime);
               break;
            }
         default:
         {
            DU_LOG("\nERROR  -->  E2AP : Invalid event trigger format of RIC subscription");
            return RFAILED;
         }
      }
      if(duChkTmr((PTR)ricSubscriptionInfo, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR) != true)
      {
         duStartTmr((PTR)ricSubscriptionInfo, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR, reportingPeriod);
      }
      else
      {
         DU_LOG("\nERROR  -->  E2AP : RIC Subscription reporting timer already running for RIC Subscription");  
         return RFAILED;
      }


      /* If even 1 action is accepted :
       *
       * For accepted groups:
       *    Mark subscribed-action's -> action = CONFIG_UNKNOWN
       *    Add to accepted-action-list of subscription response
       */
      for(idx=0; idx<statsRsp->numGrpAccepted; idx++)
      {
         actionInfoDb = NULLP;

         actionId = statsRsp->statsGrpAcceptedList[idx];
         actionInfoDb = fetchActionInfoFromActionId(actionId, ricSubscriptionInfo, &actionNode);
         if(actionInfoDb && (actionInfoDb->action == CONFIG_ADD))
         {
            actionInfoDb->action = CONFIG_UNKNOWN;
            pendingSubsRsp->acceptedActionList[pendingSubsRsp->numOfAcceptedActions++] = actionId;
         }
      }

      /* For rejected groups:
       *    Remove entry from DU's RAN Function->subscription->actionList
       *    Add to rejected-action-list in subscription response
       */
      for(idx=0; idx<statsRsp->numGrpRejected; idx++)
      {
         actionInfoDb = NULLP;
         actionId = statsRsp->statsGrpRejectedList[idx].groupId;
         actionInfoDb = fetchActionInfoFromActionId(actionId, ricSubscriptionInfo, &actionNode);
         if(actionInfoDb->actionId == actionId)
         {
            deleteActionSequence(actionNode);
            pendingSubsRsp->rejectedActionList[pendingSubsRsp->numOfRejectedActions].id = actionId;
            convertDuCauseToE2Cause(statsRsp->statsGrpRejectedList[idx].cause, \
                  &pendingSubsRsp->rejectedActionList[pendingSubsRsp->numOfRejectedActions].failureCause);
            pendingSubsRsp->numOfRejectedActions++;
         }
      }

      /* Send subscription response with accepted and rejected action lists to RIC */
      BuildAndSendRicSubscriptionRsp(pendingSubsRsp);
   }
   memset(pendingSubsRsp, 0, sizeof(PendingSubsRspInfo));
   return ROK;
}

/*******************************************************************
 *
 * @brief Extract and store statistics received from DU layers
 *
 * @details
 *
 *    Function : e2ProcStatsInd
 *
 *    Functionality: Extract statistics received from DU layers
 *       and store in respective RAN function's subscription's
 *       action
 *
 * @params[in] Statistics Indication message received from MAC
 * @return ROK-success
 *         RFAILED-failure
 *
 * ****************************************************************/
uint8_t e2ProcStatsInd(MacStatsInd *statsInd)
{
   uint8_t statsIdx = 0;
   RanFunction *ranFuncDb = NULLP;
   CmLList *ricSubscriptionNode = NULLP;
   CmLList *actionNode = NULLP;
   RicSubscription *ricSubscriptionInfo = NULLP;
   ActionInfo *actionInfo = NULLP;
   ActionDefFormat1 *actionFormat = NULLP;
   char e2MeasTypeName[STRING_SIZE_150_BYTES] = "";
   MeasurementInfo *measInfo = NULLP;
   CmLList *measInfoNode = NULLP;
   double *measValue = NULLP;
   CmLList *measValueNode = NULLP;

   /* TODO : When stats indication is received
    * DU APP searches for the message type in E2AP RIC subscription
    * database and stores in the value in the list of subscribed measurements
    *
    * This will be implemented in next gerrit.
    */

   /* Fetch RAN Function and Subscription DB using subscription Id received
    * in statistics response */
   if(fetchSubsInfoFromSubsId(statsInd->subscriptionId, &ranFuncDb, &ricSubscriptionNode, &ricSubscriptionInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Failed to fetch subscriprtion details");
      return RFAILED;
   }

   /* Fetch RIC subscription's action DB */
   actionInfo = fetchActionInfoFromActionId(statsInd->groupId, ricSubscriptionInfo, &actionNode);
   if(actionInfo == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Failed to fetch action ID [%d]", statsInd->groupId);
      return RFAILED;
   }

   /* Check Action format */
   switch(actionInfo->definition.formatType)
   {
      case 1:
         {
            actionFormat = &actionInfo->definition.choice.format1;
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Action Format [%d] is not supported", \
                  actionInfo->definition.formatType);
            return RFAILED;
         }
   }

   /* Fetch each Measurement info from action info and store its reported value in DB */
   for(statsIdx = 0; statsIdx < statsInd->numStats; statsIdx++)
   {
      memset(e2MeasTypeName, 0, STRING_SIZE_150_BYTES);
      measInfo = NULLP;
      measInfoNode = NULLP;

      /* Convert Measurement type from MAC-supported format to E2-supported format */
      if(convertMacMeasTypeToE2MeasType(statsInd->measuredStatsList[statsIdx].type, e2MeasTypeName) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Failed to convert measurement type from MAC-supported\
            MAC-supported format to E2-supported format");
         continue;
      }
      
      /* Fetch Measurement Info using E2-supported measurement type name */
      measInfo = fetchMeasInfoFromMeasTypeName(e2MeasTypeName, &actionFormat->measurementInfoList, &measInfoNode); 
      if(measInfo == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Measurement Type Name [%s] not found", e2MeasTypeName); 
         continue;
      }
      
      /* Store the measurement value in the measurement info DB fetched */
      DU_ALLOC(measValue, sizeof(double));
      if(!measValue)
      {
         DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Memory allocation failed at line [%d]", __LINE__);
         return RFAILED; 
      }
      *measValue = statsInd->measuredStatsList[statsIdx].value;

      DU_ALLOC(measValueNode, sizeof(CmLList));
      if(!measValueNode)
      {
         DU_LOG("\nERROR  -->  E2AP : extractStatsMeasurement: Memory allocation failed at line [%d]", __LINE__);
         DU_FREE(measValue, sizeof(double));
         return RFAILED; 
      }
      measValueNode->node = (PTR) measValue;
      cmLListAdd2Tail(&measInfo->measuredValue, measValueNode);
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief Handle RIC Subscription reporting timer expry
 *
 * @details
 *
 *    Function : E2apHdlRicSubsReportTmrExp
 *
 *    Functionality: On expiry of RIC subscription reporting
 *       timer expiry, RIC indication is sent for all actions
 *       in RIC subscription
 *
 * @params[in] RIC subscription DB
 * @return void
 *
 * ****************************************************************/
void E2apHdlRicSubsReportTmrExp(RicSubscription *ricSubscription)
{
   uint8_t actionIdx = 0;
   uint32_t reportingPeriod = 0;
   ActionInfo *action=NULLP;
   CmLList *actionNode=NULLP;

   CM_LLIST_FIRST_NODE(&ricSubscription->actionSequence, actionNode);
   while(actionNode)
   {
      action = (ActionInfo*)actionNode->node;
      BuildAndSendRicIndication(ricSubscription, action);
      actionNode = actionNode->next;
   }

   /* Start RIC Subscription reporting timer again */
   switch(ricSubscription->eventTriggerDefinition.formatType)
   {
      case 1:
         {
            reportingPeriod = ricSubscription->eventTriggerDefinition.choice.format1.reportingPeriod;
            /* Save the start time of reporting period */
            storeReportStartTime(&ricSubscription->eventTriggerDefinition.choice.format1.startTime);
            break;
         }
      default:
         return;
   }
   if(duChkTmr((PTR)ricSubscription, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR) != true)
   {
      duStartTmr((PTR)ricSubscription, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR, reportingPeriod);
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Failed in %s at line %d", __func__, __LINE__);
      return;
   }
}

/******************************************************************
 *
 * @brief Search E2 node component with the help of action type
 *
 * @details
 *
 *    Function : fetchE2NodeComponentInfo 
 *
 *    Functionality: Search E2 node component with the help of action type 
 *
 * @params[in] 
 *       Type of interface 
 *       Component Id
 *       Pointer to E2 component node to be searched 
 * @return CmLList
 *
 * ****************************************************************/

E2NodeComponent *fetchE2NodeComponentInfo(InterfaceType interfaceType, uint64_t componentId, CmLList **e2ComponentNode)
{
   E2NodeComponent *e2NodeComponentInfo=NULLP;

   if(duCb.e2apDb.e2NodeComponentList.count)
   {
      CM_LLIST_FIRST_NODE(&duCb.e2apDb.e2NodeComponentList, *e2ComponentNode);
      while(*e2ComponentNode)
      {
         e2NodeComponentInfo = (E2NodeComponent*)((*e2ComponentNode)->node);
         if((e2NodeComponentInfo->interfaceType == interfaceType) && (e2NodeComponentInfo->componentId == componentId))
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
 * @brief fill E2 node component rsp info
 *
 * @details
 *
 *    Function : fillE2NodeComponentRspInfo 
 *
 * Functionality: fill E2 Node Component rsp info 
 *
 * @parameter
 *       Type of interface 
 *       Component action type
 *       Size of buffer which needs to be store
 *       buffer string which needs to be store 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/

uint8_t fillE2NodeComponentRspInfo(InterfaceType interfaceType, uint64_t componentId, uint8_t action, uint8_t bufSize, char *bufString)
{
   E2NodeConfig *configInfo=NULLP;
   E2NodeComponent *e2NodeComponentInfo= NULLP;
   CmLList  *node = NULLP;

   e2NodeComponentInfo = fetchE2NodeComponentInfo(interfaceType, componentId, &node);
   if(!e2NodeComponentInfo) 
   {
      DU_LOG("\nERROR  -->  E2AP : Unable to find the node");
      return RFAILED;
   }

   switch(action)
   {
      case E2_NODE_COMPONENT_ADD:
         {
            configInfo = e2NodeComponentInfo->addConfiguration;    
            break;
         }
      case E2_NODE_COMPONENT_UPDATE: 
         {
            configInfo = e2NodeComponentInfo->updateConfiguration;
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  E2AP : Invalid action %d received",action);
            return RFAILED;
         }
   }

   if(configInfo->componentRequestPart== NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : E2 node Component request part is not present");
      return RFAILED;
   }

   configInfo->rspBufSize = bufSize;
   DU_ALLOC(configInfo->componentResponsePart, bufSize);
   if(configInfo->componentResponsePart == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed to store the encoding of rsp");
      return RFAILED;
   }
   memcpy(configInfo->componentResponsePart, bufString, configInfo->rspBufSize);
   return ROK;
}

/*******************************************************************
 *
 * @brief add E2 node component req info
 *
 * @details
 *
 *    Function : addE2NodeComponent 
 *
 * Functionality: add E2 node component req info 
 *
 * @parameter
 *       Type of interface 
 *       Component action type
 *       Size of buffer which needs to be store
 *       buffer string which needs to be store 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t addE2NodeComponent(InterfaceType interfaceType, uint64_t componentId,  uint8_t bufSize, char *bufString)
{
   E2NodeComponent *e2NodeComponentInfo= NULLP;
   CmLList  *node = NULLP;
   
   DU_ALLOC(e2NodeComponentInfo, sizeof(E2NodeComponent));
   if(!e2NodeComponentInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      return RFAILED;
   }
   e2NodeComponentInfo->interfaceType =interfaceType;
   e2NodeComponentInfo->componentId=componentId;

   DU_ALLOC(e2NodeComponentInfo->addConfiguration, sizeof(E2NodeConfig));
   if(!e2NodeComponentInfo->addConfiguration)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      return RFAILED;
   }

   e2NodeComponentInfo->addConfiguration->reqBufSize = bufSize;

   DU_ALLOC(e2NodeComponentInfo->addConfiguration->componentRequestPart, bufSize);
   if(e2NodeComponentInfo->addConfiguration->componentRequestPart == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      DU_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
      return RFAILED;
   }
   memcpy(e2NodeComponentInfo->addConfiguration->componentRequestPart, bufString,\
   e2NodeComponentInfo->addConfiguration->reqBufSize);

   DU_ALLOC(node, sizeof(CmLList));
   if(node)
   {
      node->node = (PTR) e2NodeComponentInfo;
      cmLListAdd2Tail(&duCb.e2apDb.e2NodeComponentList, node);
   }
   else
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      DU_FREE(e2NodeComponentInfo->addConfiguration->componentRequestPart, bufSize);
      DU_FREE(e2NodeComponentInfo->addConfiguration, sizeof(E2NodeConfig));
      DU_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
      return RFAILED;
   }
   return ROK;
}

/*******************************************************************
 *
 * @brief update E2 node component req info
 *
 * @details
 *
 *    Function : updateE2NodeComponent 
 *
 * Functionality: update E2 node component req info 
 *
 * @parameter
 *       Type of interface 
 *       Size of buffer which needs to be store
 *       buffer string which needs to be store 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t updateE2NodeComponent(InterfaceType interfaceType, uint64_t componentId, uint8_t bufSize, char *bufString)
{
   E2NodeComponent *e2NodeComponentInfo= NULLP;
   CmLList  *node = NULLP;
         
   e2NodeComponentInfo = fetchE2NodeComponentInfo(interfaceType, componentId, &node);
   if(!e2NodeComponentInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : Received null information in %s",__func__);
      return RFAILED;
   }
   
   DU_ALLOC(e2NodeComponentInfo->updateConfiguration, sizeof(E2NodeConfig));
   if(!e2NodeComponentInfo->updateConfiguration)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      return RFAILED;
   }

   e2NodeComponentInfo->updateConfiguration->reqBufSize = bufSize;

   DU_ALLOC(e2NodeComponentInfo->updateConfiguration->componentRequestPart, bufSize);
   if(e2NodeComponentInfo->updateConfiguration->componentRequestPart == NULLP)
   {
      DU_LOG("\nERROR  -->  E2AP : Memory allocation failed in %s at %d",__func__,__LINE__);
      DU_FREE(e2NodeComponentInfo->updateConfiguration, sizeof(E2NodeConfig));
      return RFAILED;
   }

   memcpy(e2NodeComponentInfo->updateConfiguration->componentRequestPart, bufString,\
   e2NodeComponentInfo->updateConfiguration->reqBufSize);
   return ROK;

}

/*******************************************************************
 *
 * @brief delete E2 node component req info
 *
 * @details
 *
 *    Function : deleteE2NodeComponent 
 *
 * Functionality: delete E2 node component req info 
 *
 * @parameter
 *       Type of interface 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t deleteE2NodeComponent(InterfaceType interfaceType, uint64_t componentId)
{
   E2NodeComponent *e2NodeComponentInfo= NULLP;
   CmLList  *node = NULLP;
         
   e2NodeComponentInfo = fetchE2NodeComponentInfo(interfaceType, componentId, &node);
   if(!e2NodeComponentInfo)
   {
      DU_LOG("\nERROR  -->  E2AP : Received null information in %s",__func__);
      return RFAILED;
   }
   
   e2NodeComponentInfo->deleteConfiguration = true;
   return ROK;
}

/*******************************************************************
 *
 * @brief fill E2 node component req info
 *
 * @details
 *
 *    Function : fillE2NodeComponentReqInfo 
 *
 * Functionality: fill E2 node component req info 
 *
 * @parameter
 *       Type of interface 
 *       Component action type
 *       Size of buffer which needs to be store
 *       buffer string which needs to be store 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t fillE2NodeComponentReqInfo(InterfaceType interfaceType, uint64_t componentId, uint8_t action, uint8_t bufSize, char *bufString)
{
   switch(action)
   {
      case E2_NODE_COMPONENT_ADD:
         {
            if(addE2NodeComponent(interfaceType, componentId, bufSize, bufString) != ROK)
            {
               DU_LOG("\nERROR  -->  E2AP : Failed to add e2 node component");
               return RFAILED;
            }
            break;
         }
      case E2_NODE_COMPONENT_UPDATE: 
         {
            if(updateE2NodeComponent(interfaceType, componentId, bufSize, bufString) != ROK)
            {
               DU_LOG("\nERROR  -->  E2AP : Failed to update e2 node component");
               return RFAILED;
            }
            break;
         }
      case E2_NODE_COMPONENT_DEL:
         {
            if(deleteE2NodeComponent(interfaceType, componentId) != ROK)
            {
               DU_LOG("\nERROR  -->  E2AP : Failed to delete e2 node component");
               return RFAILED;
            }
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  E2AP : Invalid action %d received",action);
            return RFAILED;
         }
   }

   return ROK;
}

/******************************************************************
 *
 * @brief Delete measured Value list
 *
 * @details
 *
 *    Function : deleteMeasuredValueList
 *
 *    Functionality: Delete measured Value list
 *
 * @params[in] List of measured Value
 *          
 * @return void 
 *
 * ****************************************************************/
void deleteMeasuredValueList(CmLListCp *measuredValueList)
{
   CmLList *measValNode = NULLP;

   CM_LLIST_FIRST_NODE(measuredValueList, measValNode);

   while(measValNode)
   {
      cmLListDelFrm(measuredValueList, measValNode);
      DU_FREE(measValNode->node, sizeof(double));
      DU_FREE(measValNode, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(measuredValueList, measValNode);
   }
}

/******************************************************************
 *
 * @brief Delete Measurement Info List 
 *
 * @details
 *
 *    Function : deleteMeasurementInfoList
 *
 *    Functionality: Delete Measurement Info List 
 *
 * @params[in] List of Measurement Info List
 *          
 * @return void 
 *
 * ****************************************************************/
void deleteMeasurementInfoList(CmLListCp *measInfoList)
{
   CmLList *measInfoNode = NULLP;
   MeasurementInfo *measInfo = NULLP;

   CM_LLIST_FIRST_NODE(measInfoList, measInfoNode);
   while(measInfoNode)
   {
      measInfo = (MeasurementInfo *)measInfoNode->node;
      cmLListDelFrm(measInfoList, measInfoNode);
      deleteMeasuredValueList(&measInfo->measuredValue);
      DU_FREE(measInfo, sizeof(MeasurementInfo));
      DU_FREE(measInfoNode, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(measInfoList, measInfoNode);
   }
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
   ActionDefinition *definition=NULLP;
   
   if(actionNode)
   {
      action = (ActionInfo*)actionNode->node;
      definition= &action->definition;       

      switch(definition->formatType)
      {
         case 1:
            {
               deleteMeasurementInfoList(&definition->choice.format1.measurementInfoList);
               break;
            }

         case 2:
         case 3:
         case 4:
         case 5:
         default:
            {
               DU_LOG("\nERROR  -->  E2AP : Format %d does not supported", definition->formatType);
               break;
            }
      }
      memset(action, 0, sizeof(ActionInfo));
      DU_FREE(actionNode->node, sizeof(ActionInfo));
      DU_FREE(actionNode, sizeof(CmLList));
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
   RicSubscription *ricSubscriptionInfo = NULLP;

   ricSubscriptionInfo = (RicSubscription*)subscriptionNode->node;    

   deleteActionSequenceList(&ricSubscriptionInfo->actionSequence);
   if(duChkTmr((PTR)ricSubscriptionInfo, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR) == TRUE)
   {
      duStopTmr((PTR)ricSubscriptionInfo, EVENT_RIC_SUBSCRIPTION_REPORTING_TMR);
   }

   memset(ricSubscriptionInfo, 0, sizeof(RicSubscription));
   DU_FREE(subscriptionNode->node, sizeof(RicSubscription));
   DU_FREE(subscriptionNode, sizeof(CmLList));
}

/******************************************************************
 *
 * @brief Delete ric subscription list from the database
 *
 * @details
 *
 *    Function : deleteRicSubscriptionList
 *
 *    Functionality: Delete ric subscription list 
 *
 * @params[in]
 *    Subscription List to be deleted 

 * @return void 
 *
 * ****************************************************************/
void deleteRicSubscriptionList(CmLListCp *subscriptionList)
{
   CmLList *subscriptionNode=NULLP;
   
   CM_LLIST_FIRST_NODE(subscriptionList, subscriptionNode);
   while(subscriptionNode)
   {
      /* TODO - Remove subscription information from MAC and SCH as well */ 
      cmLListDelFrm(subscriptionList, subscriptionNode);
      deleteRicSubscriptionNode(subscriptionNode);
      CM_LLIST_FIRST_NODE(subscriptionList, subscriptionNode);
   }
}

/*******************************************************************
 *
 * @brief Find all RIC subscriptions to be deleted in all RAN
 *    Functions
 *
 * @details
 *
 *    Function : fetchRicSubsToBeDeleted
 *
 * Functionality: Find all RIC subscriptions to be deleted in all
 *    RAN functions and store in a temporary list
 *
 * @parameter Temporary list to store subscriptions to be deleted
 * @return void
 *
 ******************************************************************/
void fetchRicSubsToBeDeleted(CmLListCp *ricSubsToBeDelList)
{
   uint16_t ranFuncIdx = 0;
   CmLList *subsNode = NULLP;
   CmLList *subsToDelNode = NULLP;

   for(ranFuncIdx = 0; ranFuncIdx < MAX_RAN_FUNCTION; ranFuncIdx++)
   {
      if(duCb.e2apDb.ranFunction[ranFuncIdx].id > 0)
      {
         CM_LLIST_FIRST_NODE(&duCb.e2apDb.ranFunction[ranFuncIdx].subscriptionList, subsNode);
         while(subsNode)
         {
            if(((RicSubscription *)subsNode->node)->action == CONFIG_DEL)
            {
               DU_ALLOC(subsToDelNode, sizeof(CmLList));
               if(!subsToDelNode)
               {
                  DU_LOG("\nERROR  -->  E2AP : %s: Memory allocation failure at %d", __func__, __LINE__); 
                  return;
               }
               subsToDelNode->node = subsNode->node;
               cmLListAdd2Tail(ricSubsToBeDelList, subsToDelNode);
            }
            subsToDelNode = NULLP;
            subsNode = subsNode->next;
         }
      }
   }
}

/******************************************************************
 *
 * @brief Delete e2 node information from the database
 *
 * @details
 *
 *    Function : removeE2NodeInformation 
 *
 *    Functionality: Delete e2 node information from the database 
 *
 * @params[in]
 *
 * @return void 
 *
******************************************************************/
void removeE2NodeInformation()
{
   uint16_t ranFuncIdx = 0;
   
   DU_LOG("\nINFO  -->  E2AP : Deleting all the E2 node configuration");
   for(ranFuncIdx=0; ranFuncIdx<MAX_RAN_FUNCTION; ranFuncIdx++)
   {
      if(duCb.e2apDb.ranFunction[ranFuncIdx].id >0)
      {
         deleteRicSubscriptionList(&(duCb.e2apDb.ranFunction[ranFuncIdx].subscriptionList));
         memset(&(duCb.e2apDb.ranFunction[ranFuncIdx].pendingSubsRspInfo), 0, MAX_PENDING_SUBSCRIPTION_RSP*sizeof(PendingSubsRspInfo));
      }
   }
   memset(&duCb.e2apDb.ricId, 0, sizeof(GlobalRicId));
   duCb.e2apDb.numOfTNLAssoc = 0;
   memset(&duCb.e2apDb.tnlAssoc, 0, MAX_TNL_ASSOCIATION*sizeof(TNLAssociation));
   memset(&ricParams, 0, sizeof(DuSctpDestCb));
}

/*******************************************************************
 *
 * @brief Extract statistics received from DU layers and delete
 * Ric subscription info
 *
 * @details
 *
 *    Function :e2ProcStatsDeleteRsp 
 *
 *    Functionality: Extract statistics received from DU layers
 *       and delete ric subscription iformation form db
 *
 * @params[in] Statistics delete rsp from MAC
 * @return ROK-success
 *         RFAILED-failure
 *
 * ****************************************************************/
uint8_t e2ProcStatsDeleteRsp(MacStatsDeleteRsp *statsDeleteRsp)
{
   RicRequestId    requestId;
   uint16_t        ranFuncId;
   RanFunction *ranFuncDb = NULLP;
   CmLList *ricSubscriptionNode = NULLP;
   RicSubscription *ricSubscriptionInfo = NULLP;
   E2FailureCause failureCause;

   /* Fetch RAN Function and Subscription DB using subscription Id received
    * in statistics delete response */
   if(fetchSubsInfoFromSubsId(statsDeleteRsp->subscriptionId, &ranFuncDb, &ricSubscriptionNode, &ricSubscriptionInfo) != ROK)
   {
      DU_LOG("\nERROR  -->  E2AP : e2ProcStatsDeleteRsp: Failed to fetch subscriprtion details");
      return RFAILED;
   }
   ranFuncId = ricSubscriptionInfo->ranFuncId;
   memcpy(&requestId, &ricSubscriptionInfo->requestId, sizeof(RicRequestId));

   deleteRicSubscriptionNode(ricSubscriptionNode);
   
   if(statsDeleteRsp->subsDelRsp == MAC_DU_APP_RSP_NOK)
   {
      if(statsDeleteRsp->subsDelCause == STATS_ID_NOT_FOUND)
      {
         failureCause.causeType =E2_RIC_REQUEST;
         failureCause.cause = E2_REQUEST_INFORMATION_UNAVAILABLE;
      }
      else
      {
         failureCause.causeType = E2_MISCELLANEOUS;
         failureCause.cause = E2_MISCELLANEOUS_CAUSE_UNSPECIFIED;
      }

      if(BuildAndSendRicSubscriptionDeleteFailure(ranFuncId, requestId, failureCause) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : e2ProcStatsDeleteRsp: failed to build and send ric subs delete failure");
         return RFAILED;
      }
   }
   else
   {
      if(BuildAndSendRicSubscriptionDeleteResponse(ranFuncId, requestId) != ROK)
      {
         DU_LOG("\nERROR  -->  E2AP : e2ProcStatsDeleteRsp: failed to build and send ric subs delete rsp");
         return RFAILED;
      }
   }
   
   return ROK;
}

/*******************************************************************
 *
 * @brief Fill RIC Subscription datils in MAC Statistics 
 * ModificationRequest
 *
 * @details
 *
 *    Function : fillRicSubsInMacStatsModificationReq
 *
 *    Functionality: Fill RIC Subscription datils in MAC 
 * Modification Statistics Request
 *    [Step -1] Generate subscription ID using RIC Request ID and 
 *    RAN Function ID
 *    [Step -2] Check all the action staus of each action present
 *    in the ric subscription. If action is CONFIG_MOD then fill
 *    the information in stats group list.
 *    [Step -3] Fill group related information in stats modification 
 *    req's in stats group list
 *    [Step -4] fill measurement information in stats group list
 *    [Step -5] If the number of stats which needs to modify is 
 *    greater then zero then return ROK else return RFAILED
 *
 * @params[in] MAC Statistics Modification Request to be filled
 *             RIC Subscription Info
 *
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t fillRicSubsInMacStatsModificationReq(MacStatsModificationReq *macStatsModificationReq, RicSubscription* ricSubscriptionInfo)
{
   uint8_t    grpIdx = 0;
   uint8_t statsModifyReqIdx = 0;
   uint64_t   subscriptionId = 0;
   ActionInfo *actionDb = NULLP;
   CmLList *actionNode = NULLP;
   ActionDefFormat1 *format1Action = NULLP;

   /* [Step -1] */
   encodeSubscriptionId(&subscriptionId, ricSubscriptionInfo->ranFuncId, ricSubscriptionInfo->requestId);

   macStatsModificationReq->subscriptionId = subscriptionId;
   CM_LLIST_FIRST_NODE(&ricSubscriptionInfo->actionSequence, actionNode);
   while(actionNode)
   {
      actionDb = (ActionInfo*)(actionNode->node);
      /* [Step -2] */
      if(actionDb->action == CONFIG_MOD)
      {
         /* [Step -3] */
         macStatsModificationReq->statsGrpList[grpIdx].groupId = actionDb->actionId;
         switch(actionDb->definition.formatType)
         {
            case 1:
               {
                  format1Action = &actionDb->definition.choice.format1;
                  macStatsModificationReq->statsGrpList[grpIdx].periodicity = format1Action->granularityPeriod;

                  CmLList *node = NULLP;
                  MeasurementInfo *measInfo = NULLP;
                  statsModifyReqIdx = 0;
                  node = cmLListFirst(&format1Action->measurementInfoList);
                  while(node)
                  {
                     /* [Step -4] */
                     measInfo = (MeasurementInfo *)(node->node);
                     switch(measInfo->measurementTypeId)
                     {
                        case 1:
                           {
                              macStatsModificationReq->statsGrpList[grpIdx].statsList[statsModifyReqIdx++] = MAC_DL_TOTAL_PRB_USAGE;
                              break;
                           }
                        case 2:
                           {
                              macStatsModificationReq->statsGrpList[grpIdx].statsList[statsModifyReqIdx++] = MAC_UL_TOTAL_PRB_USAGE;
                              break;
                           }
                        default:
                           {
                              DU_LOG("\nERROR  -->  E2AP : Invalid measurement name");
                              break;
                           }
                     }
                     node = node->next;
                  }
                  macStatsModificationReq->statsGrpList[grpIdx].numStats = statsModifyReqIdx;
                  break;
               }
            default:
               {
                  DU_LOG("\nERROR  -->  E2AP : fillRicSubsInMacStatsModificationReq: Only Action Definition Format 1 supported");
                  break;
               }
         }
         if(macStatsModificationReq->statsGrpList[grpIdx].numStats)
            grpIdx++;
      }
      actionNode = actionNode->next;
   }

   /* [Step -5] */
   macStatsModificationReq->numStatsGroup = grpIdx;
   if(macStatsModificationReq->numStatsGroup)
   {
      return ROK;
   }
   return RFAILED;
}

/**********************************************************************
  End of file
 **********************************************************************/

