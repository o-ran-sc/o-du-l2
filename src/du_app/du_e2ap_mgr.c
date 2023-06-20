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
#include "du_tmr.h"
#include "lrg.h"
#include "lkw.x"
#include "lrg.x"
#include "legtp.h"
#include "du_e2ap_mgr.h"
#include "du_app_mac_inf.h"
#include "du_app_rlc_inf.h"
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
uint8_t ResetE2Request(E2ProcedureDirection dir, E2CauseType type, E2Cause cause)
{
   /* Send Reset Request to RIC if DU detects any abnormal failure */
   if(dir == E2_NODE_INITIATED)
   {
      if(BuildAndSendE2ResetRequest(type, cause) != ROK)
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

/**********************************************************************
  End of file
 **********************************************************************/

