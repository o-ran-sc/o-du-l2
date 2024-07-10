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


/* This file is the entry point for NFAPI P7 CLOCK*/
#ifdef NFAPI_ENABLED

#include "common_def.h"
#include "lwr_mac.h"
#include "nfapi_interface.h"
#include "nfapi_common.h"
#include "lwr_mac_upr_inf.h"
#include "mac_utils.h"
#include "nfapi_p7_msg_hdl.h"

extern uint32_t PER_TTI_TIME_USEC;
extern uint8_t  NUM_SLOTS_PER_SUBFRAME;

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : nfapiP7ClkActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during UDP P7
 *             task registration. SSI will invoke this function once, after
 *             it creates and attaches this TAPA Task to a system task.
 *     
 * @param[in]  Ent entity, the entity ID of this task.     
 * @param[in]  Inst inst, the instance ID of this task.
 * @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 * @param[in]  Reason reason.
 * @return ROK     - success
 *         RFAILED - failure
 ***************************************************************************/
uint8_t nfapiP7ClkActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : nfapiP7ClkActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by NFAPI P7 users/providers through
 *           a loosely coupled interface arrive here by means of 
 *           SSI's message handling. This API is registered with
 *           SSI during the Task Registration of DU APP.
 *     
 * @param[in]  Pst     *pst, Post structure of the primitive.     
 * @param[in]  Buffer *mBuf, Packed primitive parameters in the
 *  buffer.
 * @return ROK     - success
 *         RFAILED - failure
 *
 ***************************************************************************/
uint8_t nfapiP7ClkActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

   switch(pst->srcEnt)
   {
      case ENTUDPP7:
      {
         switch(pst->event)
         {
            case EVENT_NFAPI_P7_START_CLK:
            {
               nfapiGenerateTicks();
               break;
            }
            default:
            {
                DU_LOG("ERROR  -> NFAPI_VNF: P7 CLOCK Act task received Incorrect event:%d",\
                                                                  pst->event);
                ret = RFAILED;
                break;
            }
         }
         break;    
      }
      default:
      {
         DU_LOG("ERROR  --> NFAPI_VNF: P7 CLOCK Act task received from wrong Entity:%d",\
                                                                  pst->srcEnt);
         ret = RFAILED;
         break;
      }
   }

   ODU_EXIT_TASK();
   return ret;
}


/*******************************************************************
 *
 * @brief Generates slot indications
 *
 * @details
 *
 *    Function : GenerateTicks
 *
 *    Functionality: Generates slot indications
 *
 * @params[in] 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
void nfapiGenerateTicks()
{
   float  slotDur_ms = 0;
   struct timespec tti_req = {0}, currTime = {0};
   uint8_t ratio = 1;
   uint32_t currTime_ns = 0;

   /* Currently the code takes longer that one slot indication to execute.
    * Hence, multiplying slot time interval by 2 in order to give enough time 
    * for L2 to complete one slot processing.
    * The ratio must be removed once code optimization is complete */

   while(true)
   {
      if(vnfDb.cellId == 0xFFFF)
      {
         /*Still Cell not configured thus skipping Slot Ind*/
         continue;
      }

      if((vnfDb.vnfP7Info.p7SyncInfo.frameInfo.slot == 0xFF && vnfDb.vnfP7Info.p7SyncInfo.frameInfo.sfn == 0xFFFF))
      {
         /*Calculating the Slot Duration*/
         slotDur_ms = 1/pow(2,vnfDb.numerology);
         tti_req.tv_sec = 0;
         tti_req.tv_nsec = slotDur_ms * 1000000L * ratio;
         
         clock_gettime(CLOCK_REALTIME, &currTime);
         currTime_ns = currTime.tv_sec * 1000000000 +  currTime.tv_nsec;
         vnfDb.vnfP7Info.t_ref_ns = currTime_ns;

         PER_TTI_TIME_USEC = slotDur_ms * 1000;
         NUM_SLOTS_PER_SUBFRAME = (pow(2, vnfDb.numerology) * 10);
      
         vnfDb.vnfP7Info.p7SyncInfo.frameInfo.sfn++;
         vnfDb.vnfP7Info.p7SyncInfo.frameInfo.slot++;
         DU_LOG("INFO  --> VNF_NFAPI : Starting to generate slot indications t_ref:%llu, slotDur:%f, perTTi:%u, slotsPerFrame:%d, nanoSec:%d",\
               vnfDb.vnfP7Info.t_ref_ns, slotDur_ms, PER_TTI_TIME_USEC, NUM_SLOTS_PER_SUBFRAME, tti_req.tv_nsec);
         nfapiBuildAndSendDlNodeSync(); 
      }
      else
      {
         CALC_NEXT_SFN_SLOT(vnfDb.vnfP7Info.p7SyncInfo.frameInfo);
      }
#if 0
      /*TODO: To enable when P5 messages are all done and implemented*/
      if(nfapiSendSlotIndToMac() != ROK)
      {
         DU_LOG("ERROR  -> NFAPI_VNF: Memory Corruption issue while sending SLOT IND to MAC");
         break;
      }
#endif

#ifdef ODU_SLOT_IND_DEBUG_LOG
      DU_LOG("VNF_NFAPI -->  DEBUG:  SFN/Slot:%d,%d",\
               vnfDb.vnfP7Info.p7SyncInfo.frameInfo.sfn, vnfDb.vnfP7Info.p7SyncInfo.frameInfo.slot);
#endif
      clock_nanosleep(CLOCK_REALTIME, 0, &tti_req, NULL); 
   }
}

/*******************************************************************
 *
 * @brief Triggers and sends P7 Clock Event
 *
 * @details
 *
 *    Function : nfapiTriggerP7Clock
 *
 *    Functionality:
 *       Packs polling request and sends to P7 Clock Trigger
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
uint8_t nfapiTriggerP7Clock()
{
   Pst      nfapiP7Pst;
   Buffer   *mBuf;

   memset(&nfapiP7Pst, 0, sizeof(nfapiP7Pst));
   nfapiP7Pst.srcEnt = (Ent)ENTUDPP7;
   nfapiP7Pst.srcInst = (Inst)NFAPI_UDP_P7_INST;
   nfapiP7Pst.srcProcId = NFAPI_UDP_P7_PROC;
   nfapiP7Pst.dstEnt = (Ent)ENTP7CLK;
   nfapiP7Pst.dstInst = (Inst)NFAPI_P7_CLK_INST;
   nfapiP7Pst.dstProcId = NFAPI_P7_CLK_PROC;
   nfapiP7Pst.event = EVENT_NFAPI_P7_START_CLK;
   nfapiP7Pst.selector = ODU_SELECTOR_LC;
   nfapiP7Pst.pool = NFAPI_UDP_P7_POOL;

   if(ODU_GET_MSG_BUF(DFLT_REGION, nfapiP7Pst.pool, &mBuf) != ROK)
   {
      DU_LOG("ERROR  -->  NFAPI_VNF : Failed to allocate memory");
      return RFAILED;
   }

   ODU_POST_TASK(&nfapiP7Pst, mBuf);
   return ROK;
}

/*******************************************************************
 *
 * @brief Build  and sends P7 Slot Indication to MAC
 *
 * @details
 *
 *    Function : nfapiSendSlotIndToMac
 *
 *    Functionality:
 *       Build and Packs Slot Ind to MAC for each SLot
 *
 * @params[in] Post structure
 * @return ROK     - success
 *         RFAILED - failure
 *
 *******************************************************************/
uint8_t nfapiSendSlotIndToMac()
{
   Pst pst;
   SlotTimingInfo *slotInd = {0};

   NFAPI_P7_CLK_ALLOC_SHRABL_BUF(slotInd, sizeof(SlotTimingInfo));
   memset(&pst, 0, sizeof(Pst));
   if(slotInd)
   {
      slotInd->cellId = vnfDb.cellId; 
      slotInd->sfn    = vnfDb.vnfP7Info.p7SyncInfo.frameInfo.sfn;
      slotInd->slot   = vnfDb.vnfP7Info.p7SyncInfo.frameInfo.slot;
      pst.selector    = ODU_SELECTOR_LWLC;
      pst.srcEnt      = ENTP7CLK;
      pst.dstEnt      = ENTMAC;
      pst.dstInst     = 0;
      pst.srcInst     = NFAPI_P7_CLK_INST;
      pst.dstProcId   = ODU_GET_PROCID();
      pst.srcProcId   = NFAPI_P7_CLK_PROC;
      pst.region      = NFAPI_P7_CLK_MEM_REGION;
      pst.pool        = NFAPI_P7_CLK_POOL;
      pst.event       = EVENT_SLOT_IND_TO_MAC;
      pst.route       = 0;
      pst.prior       = 0;
      pst.intfVer     = 0;
      if(packSlotInd(&pst, slotInd) != ROK)
      {
         DU_LOG("ERROR  --> NFAPI_VNF:  Unable to Send Slot Ind to MAC");
         NFAPI_P7_CLK_FREE_SHRABL_BUF(pst.region, pst.pool, slotInd, sizeof(SlotTimingInfo));
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("ERROR  -->  NFAPI_VNF: Memory allocation failed in procSlotInd");
      return RFAILED;
   }
   return ROK;
}
#endif
