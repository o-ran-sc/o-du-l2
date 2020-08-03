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
#if 0
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_lte.x"        /* Common LTE Defines */

#include "du_cfg.h"
#include "du_app_rlc_inf.h"
#include "cm.h"
#endif

#include "common_def.h"
#include "ss_queue.h"
#include "ss_task.h"
#include "ss_msg.h"
#include "du_cfg.h"
#include "du_app_rlc_inf.h"

/*******************************************************************
 *
 * @brief Packs and Sends UE create Request from DUAPP to RLC
 *
 * @details
 *
 *    Function : packDuRlcUlUeCreateReq
 *
 *    Functionality:
 *       Packs and Sends UE Create Request from DUAPP to RLC
 *
 *
 * @params[in] Post structure pointer
 *             RlcUeCfg pointer              
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t packDuRlcUlUeCreateReq(Pst *pst, RlcUeCfg *ueCfg)
{
   Buffer *mBuf = NULLP;
 
   if(pst->selector == ODU_SELECTOR_LWLC)
   {
      if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
      {
	      DU_LOG("\nRLC : Memory allocation failed at packDuRlcUeCreateReq");
	      return RFAILED;
      }
      /* pack the address of the structure */
      CMCHKPK(cmPkPtr,(PTR)ueCfg, mBuf);
   }
   else
   {
      DU_LOG("\nRLC: Only LWLC supported for packDuRlcUeCreateReq");
      return RFAILED;
   }

    return SPstTsk(pst,mBuf);
}

/*******************************************************************
 *
 * @brief Unpacks UE Create Request received from DU APP
 *
 * @details
 *
 *    Function : unpackRlcUlUeCreateReq
 *
 *    Functionality:
 *         Unpacks UE Create Request received from DU APP
 *
 * @params[in] Pointer to Handler
 *             Post structure pointer
 *             Message Buffer
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint8_t unpackRlcUlUeCreateReq(DuRlcUlUeCreateReq func, Pst *pst, Buffer *mBuf)
{
	if(pst->selector == ODU_SELECTOR_LWLC)
	{
		RlcUeCfg *ueCfg;

		/* unpack the address of the structure */
		CMCHKUNPK(cmUnpkPtr, (PTR *)&ueCfg, mBuf);
		SPutMsg(mBuf);
		return (*func)(pst, ueCfg);
	}
	else
	{
		/* Nothing to do for other selectors */
		DU_LOG("\nRLC: Only LWLC supported for UE Create Request ");
		SPutMsg(mBuf);
	}

	return RFAILED;
}

/**********************************************************************
         End of file
**********************************************************************/
