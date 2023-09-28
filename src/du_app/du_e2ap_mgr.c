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
   ActionDefFormat1 *format1Action = NULLP;

   /* Generate subscription ID using RIC Request ID and RAN Function ID */
   encodeSubscriptionId(&subscriptionId, ricSubscriptionInfo->ranFuncId, ricSubscriptionInfo->requestId);

   macStatsReq->subscriptionId = subscriptionId;
   for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
   {
      if(ricSubscriptionInfo->actionSequence[actionIdx].action == CONFIG_ADD)
      {
         actionDb = &ricSubscriptionInfo->actionSequence[actionIdx];
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
   DU_FREE(ricSubscriptionNode->node, sizeof(RicSubscription));
   DU_FREE(ricSubscriptionNode, sizeof(CmLList));

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
         actionInfoDb = fetchActionInfoFromActionId(actionId, ricSubscriptionInfo);
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
         actionId = statsRsp->statsGrpRejectedList[idx].groupId;
         if(ricSubscriptionInfo->actionSequence[actionId].actionId == actionId)
         {
            memset(&ricSubscriptionInfo->actionSequence[actionId], 0, sizeof(ActionInfo));
            ricSubscriptionInfo->numOfActions--;

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
   actionInfo = fetchActionInfoFromActionId(statsInd->groupId, ricSubscriptionInfo);
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

   for(actionIdx = 0; actionIdx < MAX_RIC_ACTION; actionIdx++)
   {
      if(ricSubscription->actionSequence[actionIdx].actionId >= 0)
      {
         BuildAndSendRicIndication(ricSubscription, &ricSubscription->actionSequence[actionIdx]);  
      }
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
 *       Component action type
 *       Pointer to E2 component node to be searched 
 * @return CmLList
 *
 * ****************************************************************/

E2NodeComponent *fetchE2NodeComponentInfo(InterfaceType interfaceType, uint8_t componentActionType, CmLList **e2ComponentNode)
{
   E2NodeComponent *e2NodeComponentInfo=NULLP;

   if(duCb.e2apDb.e2NodeComponentList.count)
   {
      CM_LLIST_FIRST_NODE(&duCb.e2apDb.e2NodeComponentList, *e2ComponentNode);
      while(*e2ComponentNode)
      {
         e2NodeComponentInfo = (E2NodeComponent*)((*e2ComponentNode)->node);
         if((e2NodeComponentInfo->interfaceType == interfaceType) && (e2NodeComponentInfo->componentActionType == componentActionType))
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
 * @brief add or modify E2NodeComponent list
 *
 * @details
 *
 *    Function : addOrModifyE2NodeComponent 
 *
 * Functionality: add or modify E2NodeComponent list 
 *
 * @parameter
 *       Type of interface 
 *       Component action type
 *       boolean variable to check req or rsp msg type
 *       Size of buffer which needs to be store
 *       buffer string which needs to be store 
 * @return ROK     - success
 *         RFAILED - failure
 *
 ******************************************************************/

uint8_t addOrModifyE2NodeComponent(InterfaceType interfaceType, uint8_t action, bool reqPart, uint8_t bufSize, char *bufString)
{
   E2NodeComponent *e2NodeComponentInfo= NULL;
   CmLList  *node = NULLP;
   
   if(reqPart == true)
   {
      DU_ALLOC(e2NodeComponentInfo, sizeof(E2NodeComponent));
      if(!e2NodeComponentInfo)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for e2NodeComponentInfo in %s",__func__);
         return RFAILED;
      }
      e2NodeComponentInfo->interfaceType =interfaceType;
      e2NodeComponentInfo->componentId=duCfgParam.duId;
      e2NodeComponentInfo->componentActionType = action;
      e2NodeComponentInfo->reqBufSize = bufSize;

      DU_ALLOC(e2NodeComponentInfo->componentRequestPart, bufSize);
      if(e2NodeComponentInfo->componentRequestPart == NULLP)
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for componentRequestPart");
         DU_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
         return RFAILED;
      }
      memcpy(e2NodeComponentInfo->componentRequestPart, bufString, e2NodeComponentInfo->reqBufSize);
      DU_ALLOC(node, sizeof(CmLList));
      if(node)
      {
         node->node = (PTR) e2NodeComponentInfo;
         cmLListAdd2Tail(&duCb.e2apDb.e2NodeComponentList, node);
      }
      else
      {
         DU_LOG("\nERROR  -->  E2AP : Memory allocation failed for e2NodeComponentList node");
         DU_FREE(e2NodeComponentInfo->componentRequestPart, bufSize);
         DU_FREE(e2NodeComponentInfo, sizeof(E2NodeComponent));
         return RFAILED;
      }
   }
   else
   {
      if(duCb.e2apDb.e2NodeComponentList.count)
      {
         e2NodeComponentInfo = fetchE2NodeComponentInfo(interfaceType, action, &node);
         if(e2NodeComponentInfo->componentRequestPart== NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : E2 node Component request part is not present");
            return RFAILED;
         }
         
         e2NodeComponentInfo->rspBufSize = bufSize;
         DU_ALLOC(e2NodeComponentInfo->componentResponsePart, bufSize);
         if(e2NodeComponentInfo->componentResponsePart == NULLP)
         {
            DU_LOG("\nERROR  -->  E2AP : Memory allocation failed to store the encoding of rsp");
            return RFAILED;
         }
         memcpy(e2NodeComponentInfo->componentResponsePart, bufString, e2NodeComponentInfo->rspBufSize);
         return ROK;
      }
      else
      {
         DU_LOG("\nERROR  -->  E2AP : Unable to find the node");
         return RFAILED;
      }
   } 
   return ROK;
}

/**********************************************************************
  End of file
 **********************************************************************/

