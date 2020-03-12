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

/* This file handles the cell configurtaion for MAC CL */

#include "envdep.h"
#include "gen.h"
#include "ssi.h"
#include "cm_hash.h"

#include "gen.x"
#include "ssi.x"
#include "cm_hash.x"
#include "cm_lib.x"

#include "mac_interface.h"
#include "lwr_mac.h"

EXTERN S16 rgClBldAndSndFAPICfgReq ARGS((ClCellCb *cellCb));

/*******************************************************************
 *
 * @brief Validates CL cell configuration
 *
 * @details
 *
 *    Function : rgClVldtCellCfg
 *
 *    Functionality:
 *       - Validates all PHY cell configuration patameters
 *
 * @params[in] cell config
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
PRIVATE S16 rgClVldtCellCfg
(
MacCellCfg   *cellCfg
)
{
  RETVALUE(ROK);
}


/*******************************************************************
 *
 * @brief Cell configuration handler at MAC CL
 *
 * @details
 *
 *    Function : RgClCellCfgReq
 *
 *    Functionality:
 *        -Handler for new cell addition request from du_app
 *
 * @params[in] cell cfg 
 * @return ROK     - success
 *         RFAILED - failure
 *
 * ****************************************************************/
PUBLIC U16 RgClCellCfgReq
(
MacCellCfg   *cellCfg
)
{
   ClCellCb   *cellCb = NULLP;

   printf("\nReceived cell configuration request. Adding cellId[%d] phyCellId[%d]", cellCfg->cellId, cellCfg->phyCellId);

   if(clGlobalCp.numOfCells >= MAX_NUM_CELL_SUPP)
   {
      printf("\nMaximum number of cells [%d] already configured", MAX_NUM_CELL_SUPP);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(rgClVldtCellCfg(cellCfg) != ROK)
   {
      printf("\nCell configuration failure. Cell Id [%d]", cellCfg->cellId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if((cellCb = (ClCellCb *)rgClUtlGetCellCb(cellCfg->cellId)) == NULLP)
   {
      if((SGetSBuf(clGlobalCp.region, clGlobalCp.pool, (Data **)&cellCb, sizeof(ClCellCb)) != ROK) || (cellCb == NULLP))
      {
         printf("\nFailed to allocate memory for new cell");
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
      cmMemset((U8 *)cellCb, 0, sizeof(ClCellCb));
      cellCb->cellId = cellCfg->cellId;
      cellCb->phyState = PHY_STATE_IDLE;
   
      if((cmHashListInsert(&(clGlobalCp.cellCbLst), (PTR)cellCb, (U8 *)&cellCb->cellId, sizeof(U16))) != ROK)
      {
         printf("\nFailed to insert cell into list.");
         RETVALUE(RFAILED);
      }

      clGlobalCp.numOfCells++;
   }

   cmMemcpy((U8 *)&cellCb->cellCfg, (U8 *)cellCfg, sizeof(MacCellCfg));

   /* Build and send CONFIG request to PHY */
   if(rgClBldAndSndFAPICfgReq(cellCb) != ROK )
   {
      printf("\nrgClBldAndSndPhyCfgReq failed");
      RETVALUE(LCM_REASON_NEG_CFM);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
}

/**********************************************************************
         End of file
**********************************************************************/
