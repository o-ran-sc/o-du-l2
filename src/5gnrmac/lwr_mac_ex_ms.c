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

/* This file is the entry point for Lower MAC */

#include "common_def.h"
#include "mac_utils.h"
#include "lwr_mac_fsm.h"
#include "lwr_mac_phy.h"
#ifdef INTEL_FAPI
#include "nr5g_fapi_internal.h"
#include "fapi_vendor_extension.h"
#endif
#ifndef INTEL_WLS_MEM
#include "lwr_mac_phy_stub_inf.h"
#endif

/**************************************************************************
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : lwrMacActvInit 
 *    
 *     Functionality:
 *             This function is supplied as one of parameters during Lower MAC's 
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
uint8_t lwrMacActvInit(Ent entity, Inst inst, Region region, Reason reason)
{
   return ROK;
}

#ifdef CALL_FLOW_DEBUG_LOG
/**************************************************************************
* @brief function prints src, dest, msg info about all the msgs received 
*
* @details
*
*     Function : callFlowlwrMacActvTsk 
*
*     Functionality:
*          function prints src, dest, msg info about all the msgs received
*
* @param[in]  Pst *pst
*
* @return void 
***************************************************************************/

void callFlowlwrMacActvTsk(Pst *pst)
{
   char sourceTask[50];
   char destTask[50]="ENTLWRMAC";
   char message[100];

   switch(pst->srcEnt)
   {
      case ENTLWRMAC:
         {
            strcpy(sourceTask,"ENTLWRMAC");
            switch(pst->event)
            {
#ifdef INTEL_WLS_MEM
               case EVT_START_WLS_RCVR:
                  {
                     strcpy(message,"EVT_START_WLS_RCVR");
                     break;
                  }
#endif
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }

#ifndef INTEL_WLS_MEM
      case ENTPHYSTUB:
         {
            strcpy(sourceTask,"PHY");
            switch(pst->event)
            {
               case EVT_PHY_STUB_SLOT_IND:
                  {
                     strcpy(message,"EVT_PHY_STUB_SLOT_IND");
                     break;
                  }

               case EVT_PHY_STUB_STOP_IND:
                 {
                    strcpy(message,"EVT_PHY_STUB_STOP_IND");
                    break;
                 }
               default:
                  {
                     strcpy(message,"Invalid Event");
                     break;
                  }
            }
            break;
         }
#endif

      default:
         {
            strcpy(sourceTask,"Invalid Source Entity Id");
         }
   }
   DU_LOG("\nCall Flow: %s -> %s : %s\n", sourceTask, destTask, message);
}
#endif

/**************************************************************************
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *      Function : lwrMacActvTsk 
 * 
 *      Functionality:
 *           Primitives invoked by Lower MAC's users/providers through
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
uint8_t lwrMacActvTsk(Pst *pst, Buffer *mBuf)
{
   uint8_t ret = ROK;

#ifdef CALL_FLOW_DEBUG_LOG
   callFlowlwrMacActvTsk(pst);
#endif
   
   switch(pst->srcEnt)
   {
      case ENTLWRMAC:
         {
            switch(pst->event)
            {
#ifdef INTEL_WLS_MEM
               case EVT_START_WLS_RCVR:
                  {
                     ODU_PUT_MSG_BUF(mBuf);

                     /* Allocate memory for intial UL transmission */
                     LwrMacEnqueueWlsBlock();

                     /* Start thread to receive from L1 */
                     DU_LOG("\nINFO  -->  LWR MAC: Starting WLS receiver thread");
                     LwrMacRecvPhyMsg();
                     break;
                  }
#endif
               default:
                  {
                     ODU_PUT_MSG_BUF(mBuf);
                     DU_LOG("\nERROR  -->  LWR MAC: Invalid event %d received", pst->event);
                     ret = RFAILED;
                  }
            }
            break;
         }

#ifndef INTEL_WLS_MEM
      case ENTPHYSTUB:
         {
            switch(pst->event)
            {
               case EVT_PHY_STUB_SLOT_IND:
                  {
                     fapi_slot_ind_t *slotIndMsg;

                     CMCHKUNPK(oduUnpackPointer, (PTR *)&slotIndMsg, mBuf);
                     ODU_PUT_MSG_BUF(mBuf);

                     procPhyMessages(slotIndMsg->header.msg_id, sizeof(fapi_slot_ind_t), (void*)slotIndMsg);
                     MAC_FREE_SHRABL_BUF(pst->region, pst->pool, slotIndMsg, sizeof(fapi_slot_ind_t));
                     break;
                  }

               case EVT_PHY_STUB_STOP_IND:
                 {
                    fapi_stop_ind_t *stopIndMsg;
                    CMCHKUNPK(oduUnpackPointer, (PTR *)&stopIndMsg, mBuf);
                    ODU_PUT_MSG_BUF(mBuf);

                    procPhyMessages(stopIndMsg->header.msg_id, sizeof(fapi_stop_ind_t), (void*)stopIndMsg);
                    MAC_FREE_SHRABL_BUF(pst->region, pst->pool, stopIndMsg, sizeof(fapi_stop_ind_t));
                    break;
                 }
               default:
                  {
                     DU_LOG("\nERROR  -->  LWR_MAC: Invalid event %d received from PHY STUB", pst->event);
                  }
            }
            break;
         }
#endif

      default:
         {
            ODU_PUT_MSG_BUF(mBuf);
            DU_LOG("\nERROR  -->  LWR MAC: Message from invalid source entity %d", pst->srcEnt);
            ret = RFAILED;
         }
   }
   return ret;
}

/**********************************************************************
  End of file
 **********************************************************************/
