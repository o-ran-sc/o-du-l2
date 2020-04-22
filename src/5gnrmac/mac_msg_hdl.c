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

/* header include files -- defines (.h)  */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"        /* common tokens */
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "lrg.h"
#include "crg.h"
#include "rg.h"
#include "du_log.h"
#include "lwr_mac.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "lrg.x"
#include "crg.x"
#include "rg_prg.x"
#include "du_app_mac_inf.h"
#include "rg.x"
#include "lwr_mac_fsm.h"

/* This file contains message handling functionality for MAC */

/*******************************************************************
 *
 * @brief Handles cell start reuqest from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *      Handles cell start reuqest from DU APP
 *
 * @params[in] Post structure pointer
 *             Cell Start Request info pointer 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStartReq(Pst *pst, MacCellStartInfo  *cellStartInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   sendToLowerMac(FAPI_START_REQUEST, 0, cellStartInfo);

   MAC_FREE_MEM(pst->region, pst->pool, cellStartInfo, \
	   sizeof(MacCellStartInfo));

   return ROK;
}

/*******************************************************************
 *
 * @brief Handles cell stop request from DU APP
 *
 * @details
 *
 *    Function : MacHdlCellStartReq
 *
 *    Functionality:
 *        Handles cell stop request from DU APP
 *
 * @params[in] Post structure pointer
 *             Mac Cell stop information
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
uint16_t MacHdlCellStopReq(Pst *pst, MacCellStopInfo  *cellStopInfo)
{
   DU_LOG("\nMAC : Handling cell start request");
   sendToLowerMac(FAPI_STOP_REQUEST, 0, cellStopInfo);

   MAC_FREE_MEM(pst->region, pst->pool, cellStopInfo, \
	   sizeof(MacCellStopInfo));
 
   return ROK;
}

/**********************************************************************
         End of file
**********************************************************************/

