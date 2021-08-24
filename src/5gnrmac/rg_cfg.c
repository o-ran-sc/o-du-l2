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

/************************************************************************
 
     Name:     LTE-MAC layer
 
     Type:     C source file
 
     Desc:     C source code for Entry point fucntions
 
     File:     rg_cfg.c
 
**********************************************************************/

/** @file rg_cfg.c
@brief This module handles the configuration of MAC by RRC and RRM.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"           /* Layer manager interface includes*/
#include "crg.h"           /* CRG interface includes*/
#include "rgu.h"           /* RGU interface includes*/
#include "tfu.h"           /* TFU interface includes */
#include "rg_sch_inf.h"    /* SCH interface includes */
#include "rg_prg.h"       /* PRG (MAC-MAC) interface includes*/
#include "rg_env.h"       /* MAC environmental includes*/
#include "rg.h"           /* MAC includes*/
#include "rg_err.h"       /* MAC error includes*/
#include "mac_utils.h" 
/* header/extern include files (.x) */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* CRG interface includes */
#include "rg_sch_inf.x"    /* SCH interface typedefs */
#include "rg_prg.x"        /* PRG (MAC-MAC) Interface typedefs */
#include "du_app_mac_inf.h"
#include "rg.x"            /* typedefs for MAC */
#ifdef LTE_ADV
#include "rg_pom_scell.x"
#endif
/* LTE-MAC Control Block Structure */
RgCb rgCb[RG_MAX_INST];

/* local defines */
static S16 rgCFGVldtCrgDedLcCfg ARGS((Inst inst,CrgLchCfg *lcCfg, RgCellCb **cell,
         RgUeCb **ue, RgErrInfo *errInfo));
static S16 rgCFGVldtCrgCmnLcCfg ARGS((Inst inst,CrgLchCfg *lcCfg, RgCellCb **cell,
         RgErrInfo *errInfo));
static S16 rgCFGCrgDedLcCfg ARGS((RgCellCb *cell, RgUeCb *ue,
         CrgLchCfg *lcCfg, RgErrInfo *errInfo));
static S16 rgCFGCrgCmnLcCfg ARGS((Inst inst,RgCellCb *cell, CrgLchCfg *lcCfg,
         RgErrInfo *errInfo));

static Void rgCFGFreeCmnLcLst ARGS((RgCellCb *cell));
static Void rgCFGFreeUeLst ARGS((RgCellCb *cell));
/* Added support for SPS*/
#ifdef LTEMAC_SPS
static Void rgCFGFreeSpsUeLst ARGS((RgCellCb *cell));
#endif /* LTEMAC_SPS */


/* local typedefs */
 
/* local externs */
 
/* forward references */




/**
 * @brief Validates the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellCfg
 *
 *     Processing Steps:
 *      - Validate the cell configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgCellCfg
(
Inst        inst,
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_CFG;
   if ((rgCb[inst].cell != NULLP)
         || rgCb[inst].inactiveCell != NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Cell already exists");
      return RFAILED;
   }
   if ((cellCfg->bwCfg.dlTotalBw < RG_MIN_DL_BW
            || cellCfg->bwCfg.dlTotalBw > RG_MAX_DL_BW)
         || (cellCfg->bwCfg.ulTotalBw < RG_MIN_UL_BW
            || cellCfg->bwCfg.ulTotalBw > RG_MAX_UL_BW))
   {
      DU_LOG("\nERROR  -->  MAC : Invalid Bandwidth configuration: ul %d dl %d",
            cellCfg->bwCfg.ulTotalBw, cellCfg->bwCfg.dlTotalBw);
      return RFAILED;
   }
   if (cellCfg->rachCfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid RACH configuration: maxMsg3Tx %d",cellCfg->rachCfg.maxMsg3Tx);
      return RFAILED;
   }
#ifdef TENB_MULT_CELL_SUPPRT
   if((cellCfg->rguDlSapId > rgCb[inst].numRguSaps) ||
      (cellCfg->rguUlSapId > rgCb[inst].numRguSaps))
   {
      DU_LOG("\nERROR  -->  MAC : Invald Sap Id: DL %d UL %d for CellId %d failed\n",
               cellCfg->rguDlSapId,
               cellCfg->rguUlSapId,
               cellCfg->cellId);
      return RFAILED;
   }
#endif
   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgCellCfg */


/**
 * @brief Validates the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgCellCb  **cell
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgUeCfg
(
Inst      inst,
CrgUeCfg  *ueCfg,
RgCellCb  **cell,
RgErrInfo *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_CFG;
   if ((ueCfg->txMode.pres == PRSNT_NODEF) && 
       (ueCfg->txMode.tm == CRG_UE_TM_5))
   {
      DU_LOG("\nERROR  -->  MAC : Transmission Mode=%d not supported",
            ueCfg->txMode.tm);
      return RFAILED;
   }
   
   /* Fetch the Active cell */
   if(((*cell = rgCb[inst].cell) == NULLP) ||
       ((*cell)->cellId != ueCfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell does not exist for cellId%d",
            ueCfg->cellId);
      return RFAILED;
   }
   /* Check if UE already configured */
   if (rgDBMGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : UE already exists");
      return RFAILED;
   }

   if (ueCfg->ueUlHqCfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid Uplink HARQ config %d ",
            ueCfg->ueUlHqCfg.maxUlHqTx);
      return RFAILED;
   }
#ifdef TENB_MULT_CELL_SUPPRT
   if((ueCfg->rguDlSapId > rgCb[inst].numRguSaps) ||
      (ueCfg->rguUlSapId > rgCb[inst].numRguSaps))
   {
      DU_LOG("\nERROR  -->  MAC : Invald Sap Id: DL %d UL %d for ueId %d failed\n",
               ueCfg->rguDlSapId,
               ueCfg->rguUlSapId,
               ueCfg->crnti);
      return RFAILED;
   }
#endif

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgUeCfg */


/**
 * @brief Validates the logical channel configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcCfg
 *
 *     Processing Steps:
 *      - Validate the logical channel configuration request from RRC to
 *        MAC at CFG: validate if configured values are within the range.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell for common channels. Return
 *          pointer to cell and UE for dedicated logical channels.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgLchCfg  *lcCfg
 *  @param[in]  Inst        inst
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgLcCfg
(
Inst       inst, 
CrgLchCfg  *lcCfg,
RgCellCb   **cell,
RgUeCb     **ue,
RgErrInfo  *errInfo
)
{

   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      /* Dedicated logical channels */
      if ((rgCFGVldtCrgDedLcCfg(inst,lcCfg, cell, ue, errInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Validation for dedicated LC failed");
         return RFAILED;
      }
   }
   else if (lcCfg->lcType == CM_LTE_LCH_BCCH
         || lcCfg->lcType == CM_LTE_LCH_PCCH
         || lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      if ((rgCFGVldtCrgCmnLcCfg(inst,lcCfg, cell, errInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Validation for common logical channels failed");
         return RFAILED;
      }
   }
   else
   {
      DU_LOG("\nERROR  -->  MAC : Invalid logical channel type %d",
                lcCfg->lcType);
      return RFAILED;
   }
#ifdef LTE_L2_MEAS
   if ( lcCfg->qci <  RG_QCI_MIN ||
        lcCfg->qci >  RG_QCI_MAX
      )
   {
      DU_LOG("\nERROR  -->  MAC : Invalid qci %x",lcCfg->qci);
      return RFAILED;
   }
   /*validate qci */
#endif /*LTE_L2_MEAS */

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgLcCfg */


/**
 * @brief Validates the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgCellCb      **cell
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgCellRecfg
(
Inst          inst,
CrgCellRecfg  *cellRecfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_RECFG;
   
   if (((*cell = rgCb[inst].cell) == NULLP)
         && ((*cell = rgCb[inst].inactiveCell) == NULLP))
   {
      DU_LOG("\nERROR  -->  MAC : Cell does not exist");
      return RFAILED;
   }

   if((*cell)->cellId != cellRecfg->cellId)
   {
      DU_LOG("\nERROR  -->  MAC : Cell does not exist %d\n",cellRecfg->cellId);
      return RFAILED;
   }
   if (cellRecfg->rachRecfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid RACH configuration: maxMsg3Tx %d",cellRecfg->rachRecfg.maxMsg3Tx);
      return RFAILED;
   }
   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgCellRecfg */


/**
 * @brief Validates the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgUeRecfg *ueRecfg
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgUeRecfg
(
Inst        inst,
CrgUeRecfg  *ueRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RECFG;
   
   if ((ueRecfg->txMode.pres == PRSNT_NODEF) && 
       (ueRecfg->txMode.tm == CRG_UE_TM_5))
   {
      DU_LOG("\nERROR  -->  MAC : Transmission Mode=%d not supported",
                ueRecfg->txMode.tm);
      return RFAILED;
   }

    /* Fetch the Active cell */
   if (((*cell = rgCb[inst].cell) == NULLP) 
        || ((*cell)->cellId != ueRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell does not exist\n");
      return RFAILED;
   }
 
   /* Fix : syed UE ID change at MAC will now be controlled
    * by SCH. */
   if ((*ue = rgDBMGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]Old UE does not exist", ueRecfg->oldCrnti);
      return RFAILED;
   }
   if (ueRecfg->ueUlHqRecfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid Uplink HARQ config for UE %d",
            ueRecfg->ueUlHqRecfg.maxUlHqTx);
      return RFAILED;
   }
   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgUeRecfg */


/**
 * @brief Validates the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgLchRecfg  *lcRecfg
 *  @param[out] RgCellCb     **cell
 *  @param[out] RgUeCb       **ue
 *  @param[out] RgUlLcCb     **ulLc
 *  @param[out] RgDlLcCb     **dlLc
 *  @param[out] RgErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgLcRecfg
(
Inst        inst,
CrgLchRecfg *lcRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgUlLcCb    **ulLc,
RgErrInfo   *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_LC_RECFG;

   /* Fetch the cell */
   if ((((*cell = rgCb[inst].cell)) == NULLP)
      || ((*cell)->cellId != lcRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell %u does not exist for UE %u", lcRecfg->cellId, lcRecfg->crnti);
      return RFAILED;
   }
   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgDBMGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : UE does not exist for dedicated logical channel");
      return RFAILED;
   }

   if ((*ulLc = rgDBMGetUlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Dedicated UL LC does not exist %d",lcRecfg->lcId);
      return RFAILED;
   }

   if (lcRecfg->ulRecfg.lcgId > (RG_MAX_LCG_PER_UE - 1))
   {
      DU_LOG("\nERROR  -->  MAC : Invalid lcgId for uplink logical channel lcg %d lc %d",
                lcRecfg->ulRecfg.lcgId, lcRecfg->lcId);
      return RFAILED;
   }

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Validates the UE Reset request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block.
 *      - If Failue,
 *          - Return RFAILED.
 *      - Retrieve the UE control block.
 *      - If Failue,
 *          - Return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgRst     *reset,
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGVldtCrgUeReset
(
Inst        inst,
CrgRst      *reset,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
{
   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RESET;
   
   /* Fetch the Active cell */
   if (((*cell = rgCb[inst].cell) == NULLP)
      || ((*cell)->cellId != reset->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell does not exist %d",reset->cellId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, reset->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : UE does not exist");
      return RFAILED;
   }

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGVldtCrgUeReset*/
/* End: LTEMAC_2.1_DEV_CFG */



/**
 * @brief Handler for the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellCfg
 *
 *     Processing Steps:
 *      - Allocate and create cell control block.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add the control block to hash list of cells.
 *      - Update the statistics.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgCellCfg
(
Inst        inst,
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
{
   S16        ret;
   RgCellCb   *cell = NULLP;
   uint8_t    idx;
   SuId       rguUlSapId = 0;
   SuId       rguDlSapId = 0;
   /* RLC SAP to allocate flowCntrlInd buffer*/
   /* Commented below to resolve compilation warning */
   //Pst        *pst ;

   errInfo->errCause = RGERR_CFG_CRG_CELL_CFG;
   
   /* Allocate the cell control block */
   if((ret = rgAllocSBuf(inst,(Data**)&cell, sizeof(RgCellCb))) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation FAILED for cell");
      return RFAILED;
   }
   if (cell == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : Memory allocation FAILED for cell");
      return RFAILED;
   }

   /* Initialize the cell */
   cell->cellId  = cellCfg->cellId;
   cell->rachCfg = cellCfg->rachCfg;
   cell->bwCfg   = cellCfg->bwCfg;
#ifdef EMTC_ENABLE
   if(cellCfg->emtcEnable)
    {  
     cell->emtcEnable = cellCfg->emtcEnable;
    }
#endif
   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RG_INVALID_LC_ID;
   cell->dlCcchId = RG_INVALID_LC_ID;

   
   /* Initialize the lists of the cell */
   ret = rgDBMInitCell(cell);
   if (ret != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : DBM initialization for cell failed");
      rgCFGFreeInactvCellCb(cell);
      return RFAILED;
   }

#ifdef LTE_ADV
   if (RFAILED == RgLaaCellCbInit(cell))
   {
      rgCFGFreeInactvCellCb(cell);
      return RFAILED;
   }
#endif

   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      cell->subFrms[idx].txDone = TRUE;
   }
  
   cell->macInst           = inst + RG_INST_START; 
   /* Insert cell in the incative cell list */
   rgCb[inst].inactiveCell = cell;
   rgCb[inst].cell         = NULLP;
#ifdef TENB_MULT_CELL_SUPPRT
   rguDlSapId              = cellCfg->rguDlSapId;
   rguUlSapId              = cellCfg->rguUlSapId;
#else
   if(rgCb[inst].numRguSaps > 1)
   {
      rguDlSapId              = 1;
   }
#endif
   cell->rguDlSap          = &(rgCb[inst].rguSap[rguDlSapId]);
   cell->rguUlSap          = &(rgCb[inst].rguSap[rguUlSapId]);


#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
   for(idx = 0; idx < RG_NUM_UL_SUB_FRAMES; idx++)
   {
      memset(&cell->ulSf[idx], 0, sizeof(RgUlSf));
   }

   cell->ttiCycle = (uint32_t)RG_TTI_CYCLE_INVLD;   
#endif
   /* Update Statistics */
   rgUpdtCellCnt(inst,RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
  
   /* Commented below to resolve compilation warning */
   //pst = &rgCb[inst].rguSap[rguDlSapId].sapCfg.sapPst;
   /* Allocate a buffer for flowCntrlInd.*/
   MAC_ALLOC(cell->flowCntrlInd, sizeof(RguFlowCntrlInd));
   if(cell->flowCntrlInd == NULLP)
   {
      DU_LOG("ERROR  --> MAC : rgCFGCrgCellCfg(): Memory allocation failed");
      return RFAILED;
   }
   return ROK;
}  /* rgCFGCrgCellCfg */

#ifdef LTE_ADV
/**
 * @brief Add SCell Cfg recvd from primary MAC instance.
 *
 * @details
 *
 *     Function : rgCfgAddUeSCellCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Inst                 dstMacInst
 *  @param[in]  RgPrgUeSCellCfgInfo  *ueSCellCb
 *  @param[in]  RgCellCb              cell

 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCfgAddUeSCellCfg
(
Inst        dstMacInst,    
RgPrgUeSCellCfgInfo *ueSCellCb,
RgCellCb    *cell
)
{
   RgUeCb     *ueCb = NULLP;
   SuId       rguUlSapId = 0;
   SuId       rguDlSapId = 0;
   uint8_t    idx;
   RgErrInfo  errInfo;

#ifdef LTE_ADV
   rguDlSapId              = ueSCellCb->rguDlSapId;
   rguUlSapId              = ueSCellCb->rguUlSapId;
#endif
#ifndef TENB_MULT_CELL_SUPPRT
   if(rgCb[dstMacInst].numRguSaps > 1)
   {
      rguDlSapId              = 1;
   }
#endif

   if ((ueCb = rgDBMGetUeCb(cell, ueSCellCb->ueId)) != NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UE already exist in scell %d during scell addition\n", 
               ueSCellCb->ueId,
               cell->cellId);
      return RFAILED;
   }

   /* Create UeCb */
   if((ueCb = rgRAMCreateUeCb(cell, ueSCellCb->ueId,
               FALSE, &errInfo)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UeCb creation failed\n", ueSCellCb->ueId);
      return RFAILED;
   }

   if(rgDHMHqEntInit(dstMacInst, &ueCb->dl.hqEnt, 
            (rgCb[dstMacInst].cell)->maxDlHqProcPerUe) != ROK)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UeCb Harq Entity Initialization failed\n", ueSCellCb->ueId);
      return RFAILED;
   }
   rgDBMInsUeCb(cell, ueCb);


   ueCb->rguDlSap          = &(rgCb[dstMacInst].rguSap[rguDlSapId]);
   ueCb->rguUlSap          = &(rgCb[dstMacInst].rguSap[rguUlSapId]);

   /* Update satistics */
   rgUpdtUeCnt(dstMacInst, RG_CFG_ADD);
   /*Commit Added SCell info to UeCb */
   /*
   ueCb->sCelAddInfo[idx].isSCellAdded = TRUE;
   ueCb->sCelAddInfo[idx].macInst = dstMacInst;
   ueCb->sCelAddInfo[idx].sCellId = ueSCellCb->cellId;
   */

   ueCb->txMode = ueSCellCb->txMode;
   ueCb->ul.hqEnt.maxHqRetx = ueSCellCb->maxUlHqRetx;

   for (idx =0; idx <RG_MAX_LC_PER_UE; idx++)
   {
      ueCb->ul.lcCb[idx] = ueSCellCb->ulLcInfo[idx];
      ueCb->dl.lcCb[idx] = ueSCellCb->dlLcInfo[idx];
   }

   for (idx =0; idx < RG_MAX_LCG_PER_UE; idx++)
   {
      ueCb->ul.lcgArr[idx].lcgId = ueSCellCb->lcgInfo[idx].lcgId;
      ueCb->ul.lcgArr[idx].lcCount = ueSCellCb->lcgInfo[idx].lcCount;
      ueCb->ul.lcgArr[idx].isGbr = ueSCellCb->lcgInfo[idx].isGbr;
   }
   return ROK;
}/* rgCfgAddUeSCellCfg */

/**
 * @brief SCell Config Filling for added cell from RRC to MAC.
 *
 * @details
 *
 *     Function : rgFillAndAddSCellCfg 
 *
 *     Processing Steps:
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Update UE control block with the values present in the
 *        CellCb
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  CrgUeCfg      *ueCfg
 *  @param[in]  CrgCfgTransId transId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgFillAndAddSCellCfg
(
Inst            inst,
RgCellCb        *cell,
CrgUeRecfg      *ueRecfg,
CrgCfgTransId   transId,
Bool            *isCfmRqrd
)
{
   RgUeCb     *ue = NULLP;
   uint8_t     idx = 0;
   Inst        dstMacInst;
   RgPrgUeSCellCfgInfo ueSCellCb;
   Pst          dstInstPst;

  /* Fetch the Active cell */
   if(((cell = rgCb[inst].cell) == NULLP) ||
       (cell->cellId != ueRecfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : [%d]Active Cell does not exist %d\n",
                                  ueRecfg->oldCrnti, ueRecfg->cellId);
      return RFAILED;
   }

   DU_LOG("\nINFO  -->  MAC : Filling SCell Config : cellId %d ueId %d\n",
            cell->cellId, cell->ueId);

   if ((ue = rgDBMGetUeCb(cell, ueRecfg->oldCrnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UE does not exist\n", ueRecfg->oldCrnti);
      return RFAILED;
   }

   /* Initialize cfgCfmInfo in the ueCb. This is used while processing SCellAdd
    *confirmation*/
   ue->cfgCfmInfo.numSCells = ueRecfg->crgSCellCfg.numSCells;
   ue->cfgCfmInfo.cfgCfgCount = 0;
   ue->cfgCfmInfo.mask = 0x0;

   memcpy(&(ue->cfgCfmInfo.transId), &transId,
         sizeof(CrgCfgTransId));
   ueSCellCb.ueId = ueRecfg->oldCrnti;
   ueSCellCb.txMode = ue->txMode;
   ueSCellCb.maxUlHqRetx = ue->ul.hqEnt.maxHqRetx;
   memcpy(ueSCellCb.ulLcInfo, ue->ul.lcCb, sizeof(ue->ul.lcCb));
   memcpy(ueSCellCb.dlLcInfo, ue->dl.lcCb, sizeof(ue->dl.lcCb));
   for (idx =0; idx < RG_MAX_LCG_PER_UE; idx++)
   {
      ueSCellCb.lcgInfo[idx].lcgId = ue->ul.lcgArr[idx].lcgId;
      ueSCellCb.lcgInfo[idx].lcCount = ue->ul.lcgArr[idx].lcCount;
      ueSCellCb.lcgInfo[idx].isGbr = ue->ul.lcgArr[idx].isGbr;
   }

   for(idx = 0; 
         idx < ueRecfg->crgSCellCfg.numSCells; idx++)
   {
      dstMacInst = ueRecfg->crgSCellCfg.ueSCellCfg[idx].macInst - RG_INST_START;
      ueSCellCb.cellId = ueRecfg->crgSCellCfg.ueSCellCfg[idx].sCellId;
      ueSCellCb.rguDlSapId = ueRecfg->crgSCellCfg.ueSCellCfg[idx].rguDlSapId;
      ueSCellCb.rguUlSapId = ueRecfg->crgSCellCfg.ueSCellCfg[idx].rguUlSapId;

      /* Get post structure of the cell to whom ueSCellCb needs to be sent
       * And then send the sCell Add based on Mac instances */
      rgGetPstToInst(&dstInstPst, inst, dstMacInst);
      RgPrgPMacSMacUeSCellCfg(&dstInstPst, &ueSCellCb);

      /*Commit Added SCell info to UeCb is moved to config confirm*/
   } /*end of for loop */
   *isCfmRqrd = FALSE;

   return ROK;
}  /* rgFillAndAddSCellCfg */
#endif /* LTE_ADV */

/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Invoke RAM, SCH, UHM and DHM with created UE control block, to
 *        update random access, scheduler, uplink harq and downlink harq
 *        specific information respectively.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgCellCb  *cell
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgUeCfg
(
Inst      inst,
RgCellCb  *cell,
CrgUeCfg  *ueCfg,
RgErrInfo *errInfo
)
{
   RgUeCb    *ue = NULLP;
   Bool      handover = FALSE;
   SuId       rguUlSapId = 0;
   SuId       rguDlSapId = 0;

   errInfo->errCause = RGERR_CFG_CRG_UE_CFG;

/* Start: LTEMAC_2.1_DEV_CFG */
   if ((ue = rgDBMGetUeCbFromRachLst(cell, ueCfg->crnti)) == NULLP)
   {
      /* Create UeCb and Insert in Rach List */
      if((ue=rgRAMCreateUeCb(cell, ueCfg->crnti, FALSE, errInfo)) == NULLP)
      {
         DU_LOG("\nERROR  -->  MAC : UeCb creation failed");
         return RFAILED;
      }
      if(rgDHMHqEntInit(inst,&ue->dl.hqEnt, cell->maxDlHqProcPerUe) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : UeCb Harq Entity Initialization failed");
         return RFAILED;
      }
      handover = TRUE;
   }
/* End: LTEMAC_2.1_DEV_CFG */

   if(handover == FALSE)
   {
      /* Remove from the rachLst */
      rgDBMDelUeCbFromRachLst(cell, ue);
   }


   /* Initialize uplink HARQ related information for UE */
   rgUHMCrgUeCfg(cell, ue, ueCfg);

   rgDBMInsUeCb(cell, ue);

#ifdef TENB_MULT_CELL_SUPPRT
   rguDlSapId              = ueCfg->rguDlSapId;
   rguUlSapId              = ueCfg->rguUlSapId;
#else
   if(rgCb[inst].numRguSaps > 1)
   {
      rguDlSapId              = 1;
   }
#endif

   ue->rguDlSap          = &(rgCb[inst].rguSap[rguDlSapId]);
   ue->rguUlSap          = &(rgCb[inst].rguSap[rguUlSapId]);


   /* Update satistics */
   rgUpdtUeCnt(inst,RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGCrgUeCfg */


/**
 * @brief Handler for the logical channel configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcCfg
 *
 *     Processing Steps:
 *      - Check if the configuration is for dedicated or common logical channel.
 *      - For Dedicated logical channels:
 *        - Update the dedicated logical channel Cb with the configured values.
 *        - Invoke SCH will cell, UE and logical channel Cb to update scheduler
 *          specific information.
 *      - For Common logical channels:
 *        - Update the common logical channel Cb with the configured values.
 *        - Move cell to active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgLchCfg   *lcCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @param[in]  Bool        *isCfmRqrd
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgLcCfg
(
Inst            inst,
RgCellCb        *cell,
RgUeCb          *ue,
CrgLchCfg       *lcCfg,
RgErrInfo       *errInfo,
Bool            *isCfmRqrd,
CrgCfgTransId   transId
)
{

   /* Handle Config for dedicated/common logical channels */
   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {

      if ((rgCFGCrgDedLcCfg(cell, ue, lcCfg, errInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Dedicated logical channel configuration failed %d",lcCfg->lcId);
         return RFAILED;
      }
#ifdef LTE_ADV
      /*ERAB Multl Cell fix*/
       memcpy(&(ue->cfgCfmInfo.transId), &transId,
            sizeof(CrgCfgTransId));
       rgPomSndUeSCellLchAddToSmac(inst, cell, ue, lcCfg,isCfmRqrd);
#endif
   }
   else
   {
      if ((rgCFGCrgCmnLcCfg(inst,cell, lcCfg, errInfo)) != ROK)
      {
         DU_LOG("\nERROR  -->  MAC : Common logical channel configuration"
                  "failed %d\n", lcCfg->lcId);
         return RFAILED;
      }
   }

   errInfo->errCause = RGERR_NONE;
   DU_LOG("\nINFO   -->  MAC : CRG LC config done for UE lcId %d\n", lcCfg->lcId);
   return ROK;
}  /* rgCFGCrgLcCfg */


/**
 * @brief Handler for the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with updated Cell Cb to update scheduler specific
 *        parameters.
 *      - Update the cell Cb with the reconfigured values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgCellRecfg
(
Inst          inst,
RgCellCb      *cell,
CrgCellRecfg  *cellRecfg,
RgErrInfo     *errInfo
)
{
   /* Store the given rachCfg */
   cell->rachCfg = cellRecfg->rachRecfg;

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGCrgCellRecfg */


/**
 * @brief Handler for the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeRecfg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - Update the UE control block with the reconfigured values.
 *      - Invoke SCH, UHM and DHM with updated UE control block to 
 *        update scheduler, uplink HARQ and downlink HARQ specific
 *        parameters.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgUeRecfg  *ueRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgUeRecfg
(
Inst        inst,
RgCellCb    *cell,
RgUeCb      *ue,
CrgUeRecfg  *ueRecfg,
RgErrInfo   *errInfo
)
{
   errInfo->errCause = RGERR_CFG_CRG_UE_RECFG;

   /* Fix : syed UE ID change at MAC will now be controlled
    * by SCH. */

   /* Update uplink HARQ related information for UE */
   rgUHMCrgUeRecfg(cell, ue, ueRecfg);

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGCrgUeRecfg */


/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgUlLcCb    *ulLc
 *  @param[in]  RgDlLcCb    *dlLc
 *  @param[in]  CrgLchRecfg *lcRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgLcRecfg
(
Inst        inst,
RgCellCb    *cell,
RgUeCb      *ue,
RgUlLcCb    *ulLc,
CrgLchRecfg *lcRecfg,
RgErrInfo   *errInfo,
Bool        *isCfmRqrd
)
{

   if (ulLc->lcgId != lcRecfg->ulRecfg.lcgId)
   {
      rgDBMUpdUlDedLcCb(ue, ulLc, lcRecfg->ulRecfg.lcgId);
#ifdef LTE_ADV
      rgPomSndUeSCellLchModToSmac(inst, cell, ue,  lcRecfg,isCfmRqrd);
#endif
   }

   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeReset
 *
 *     Processing Steps:
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  CrgRst     *reset
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgUeReset
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgRst      *reset,
RgErrInfo   *errInfo
)
{

   DU_LOG("\nDEBUG  -->  MAC : UE of cell %d Reset\n", cell->cellId);
   rgDHMUeReset(cell, &ue->dl.hqEnt);

   errInfo->errCause = RGERR_NONE;

   return ROK;
}  /* rgCFGCrgUeReset */
/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Handler for the cell delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgDel      *cellDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgCellDel
(
Inst        inst,
CrgDel      *cellDelInfo,
RgErrInfo   *errInfo
)
{
   RgCellCb *cell;
   uint8_t  idx;


   errInfo->errCause = RGERR_CFG_CRG_CELL_DEL;
   if (((cell = rgCb[inst].cell) == NULLP)
       ||(cell->cellId != cellDelInfo->u.cellDel.cellId))  
   {
      if(((cell = rgCb[inst].inactiveCell) == NULLP)
          ||(cell->cellId != cellDelInfo->u.cellDel.cellId))  
      {

         
         DU_LOG("\nERROR  -->  MAC : Cell does not exist");
         return RFAILED;
      }

      /* Delete cell from inactive list */
      rgCb[inst].inactiveCell = NULLP ;

      /* Free the inactive cell */
      rgCFGFreeInactvCellCb(cell);

      errInfo->errCause = RGERR_NONE;
      return ROK;
   }

   /* Delete from the cell list */
   //rgDBMDelCellCb(cell);
   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      rgTOMRlsSf(inst,&cell->subFrms[idx]);
   }

   /* Free the active cell */
   rgCFGFreeCellCb(cell);

   rgCb[inst].cell = NULLP;

   errInfo->errCause    = RGERR_NONE;
   DU_LOG("\nINFO  -->  MAC : Cell %d deleted\n", cellDelInfo->u.cellDel.cellId);
   return ROK;
}  /* rgCFGCrgCellDel */


/**
 * @brief Handler for the UE delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgDel      *ueDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgUeDel
(
Inst        inst,
CrgDel      *ueDelInfo,
RgErrInfo   *errInfo
)
{

   errInfo->errCause = RGERR_CFG_CRG_UE_DEL;

   DU_LOG("\nDEBUG  -->  MAC : UE %d Deletion Req at MAC\n", \
            ueDelInfo->u.ueDel.crnti);
   if ((rgCb[inst].cell == NULLP)
       || (rgCb[inst].cell->cellId != ueDelInfo->u.ueDel.cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Cell does not exist %d",
                ueDelInfo->u.ueDel.cellId);
      return RFAILED;
   }

   errInfo->errCause = RGERR_NONE;
   /* Fix: syed Context Deletion is relied upon SCH indication */
   return ROK;
}  /* rgCFGCrgUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgDel      *lcDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgCFGCrgLcDel
(
Inst        inst,
CrgDel      *lcDelInfo,
RgErrInfo   *errInfo,
Bool        *isCfmRqrd,
CrgCfgTransId transId
)
{
   Bool      dirVld = FALSE;
   RgCellCb  *cell;
   RgUeCb    *ue;
   RgUlLcCb  *ulLc;
   RgDlLcCb  *dlLc;

   errInfo->errCause = RGERR_CFG_CRG_LC_DEL;

   /* Fetch the Active cell */
   if (((cell = rgCb[inst].cell) == NULLP) ||
       (rgCb[inst].cell->cellId != lcDelInfo->u.lchDel.cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Cell does not exist %d",
                lcDelInfo->u.lchDel.cellId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((ue = rgDBMGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : UE does not exist for dedicated logical channel");
      return RFAILED;
   }

   /* Validate downlink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_TX)
   {
      if ((dlLc = rgDBMGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         DU_LOG("\nERROR  -->  MAC : DL LC %d does not exist",
                   lcDelInfo->u.lchDel.lcId);
         return RFAILED;
      }
      rgDBMDelDlDedLcCb(ue, dlLc);
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_RX)
   {
      if ((ulLc = rgDBMGetUlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         DU_LOG("\nERROR  -->  MAC : UL LC %d does not exist",
                   lcDelInfo->u.lchDel.lcId);
         return RFAILED;
      }
      rgDBMDelUlDedLcCb(ue, ulLc);
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid direction %d for LC Delete",
            lcDelInfo->u.lchDel.dir);
      return RFAILED;
   }
#ifdef LTE_ADV
   /*ERAB - multicell fix*/
   memcpy( &(ue->cfgCfmInfo.transId), &transId,
         sizeof(CrgCfgTransId));
   rgPomSndUeSCellLchDelToSmac(inst, lcDelInfo, isCfmRqrd);
#endif
   errInfo->errCause = RGERR_NONE;
   return ROK;
}  /* rgCFGCrgLcDel */

/***********************************************************
 *
 *     Func : rgCFGVldtCrgDedLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgCFGVldtCrgDedLcCfg
(
Inst          inst, 
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgUeCb        **ue,
RgErrInfo     *errInfo
)
{
   uint8_t         dirVld   = FALSE;

   errInfo->errCause = RGERR_CFG_INVALID_CRG_DED_LC_CFG;

   /* Fetch the Active cell */
   if (((*cell = rgCb[inst].cell) == NULLP)
      || ((*cell)->cellId != lcCfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell does not exist: Cell %d",
                lcCfg->cellId);
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : UE  does not exist for dedicated logical channel %d",
                lcCfg->lcId);
      return RFAILED;
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RG_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RG_DEDLC_MAX_LCID))
   {
      DU_LOG("\nERROR  -->  MAC : Invalid logical channel Id %d",
                lcCfg->lcId);
      return RFAILED;
   }

   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (rgDBMGetDlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         DU_LOG("\nERROR  -->  MAC : UE Dedicated DL LC %d already configured",
                    lcCfg->lcId);
         return RFAILED;
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      if (lcCfg->ulInfo.lcgId > (RG_MAX_LCG_PER_UE - 1))
      {
         DU_LOG("\nERROR  -->  MAC : UE Invalid lcgId for uplink logical channel %d",
                   lcCfg->ulInfo.lcgId);
         return RFAILED;
      }
      if (rgDBMGetUlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         DU_LOG("\nERROR  -->  MAC : UE Dedicated UL LC %d already configured",
                   lcCfg->lcId);
         return RFAILED;
      }
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid Direction %d",
               lcCfg->dir);
      return RFAILED;
   }

   return ROK;
}  /* rgCFGVldtCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGVldtCrgCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgCFGVldtCrgCmnLcCfg
(
Inst          inst,
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
{
   uint8_t dirVld  = FALSE;

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CMN_LC_CFG;

   /* Ensure cell is not in the active list */
   if (((*cell = rgCb[inst].cell) != NULLP)
      && ((*cell)->cellId != lcCfg->cellId))
   {
      DU_LOG("\nERROR  -->  MAC : Active Cell exists for common channels");
      return RFAILED;
   }

   /* Fetch the inactive cell for common logical channels */
   if (((*cell = rgCb[inst].inactiveCell) == NULLP)
        || ((*cell)->cellId != lcCfg->cellId))
   {
      
      DU_LOG("\nERROR  -->  MAC : Inactive Cell does not exist for common channels");
      return RFAILED;
   }
   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            if (rgDBMGetBcchOnDlsch(*cell,lcCfg->lcId) != NULLP)
            {
               DU_LOG("\nERROR  -->  MAC : BCCH on DLSCH already configured for cell");
               return RFAILED;
            }
         }
         else if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_BCH)
         {
            if (rgDBMGetBcchOnBch(*cell) != NULLP)
            {
               DU_LOG("\nERROR  -->  MAC : BCCH on BCH already configured for cell ");
               return RFAILED;
            }
         }
         else
         {
            DU_LOG("\nERROR  -->  MAC : Invalid transport channel %d for cell",
                  lcCfg->dlInfo.dlTrchType);
            return RFAILED;
         }
      }
      else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
      {
         if (rgDBMGetPcch(*cell) != NULLP)
         {
            DU_LOG("\nERROR  -->  MAC : PCCH already configured for cell");
            return RFAILED;
         }
      }
      else if (RG_DLCCCH_ISCFGD(*cell))
      {
         DU_LOG("\nERROR  -->  MAC : DL CCCH already configured for cell ");
         return RFAILED;
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      /* Uplink CCCH */
      if (lcCfg->lcType != CM_LTE_LCH_CCCH)
      {
         DU_LOG("\nERROR  -->  MAC : Invalid UL common lcType %d for cell ",
                  lcCfg->lcType);
         return RFAILED;
      }
      if (RG_ULCCCH_ISCFGD(*cell))
      {
         DU_LOG("\nERROR  -->  MAC : UL CCCH already configured for cell ");
         return RFAILED;
      }
      dirVld = TRUE;
   }

   /* Invalid direction */
   if (!dirVld)
   {
      DU_LOG("\nERROR  -->  MAC : Invalid Direction %d", lcCfg->dir);
      return RFAILED;
   }

   return ROK;
}  /* rgCFGVldtCrgCmnLcCfg */

/***********************************************************
 *
 *     Func : rgCFGCrgDedLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgCFGCrgDedLcCfg
(
RgCellCb      *cell,
RgUeCb        *ue,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
{
   //Inst     inst = cell->macInst - RG_INST_START;
   errInfo->errCause = RGERR_CFG_CRG_DED_LC_CFG;

   /* Uplink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_RX)
   {
#ifdef LTE_L2_MEAS
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId, lcCfg->qci);
      cell->qciArray[lcCfg->qci].qci = lcCfg->qci;
      if(lcCfg->lcType == CM_LTE_LCH_DTCH)
      {
        rgAddToL2MeasPerQci(cell,lcCfg->qci);/*LTE_L2_MEAS_PHASE2*/ 
      }
#else
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId);
#endif
   }

   /* Downlink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      rgDBMInsDlDedLcCb(ue, lcCfg->lcId);
   }
   return ROK;
}  /* rgCFGCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGCrgCmnLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static S16 rgCFGCrgCmnLcCfg
(
Inst          inst,
RgCellCb      *cell,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
{
   errInfo->errCause = RGERR_CFG_CRG_CMN_LC_CFG;

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & CRG_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_DL_CCCH_CFG_DONE;
      }

      if (lcCfg->dir & CRG_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_UL_CCCH_CFG_DONE;
      }
   }
   else
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgDBMInsBcchOnDlsch(cell, lcCfg->lcId);
            
            if(cell->cellActvState & RG_BCCH_DLSCH_CFG1_DONE)
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG2_DONE;
            }
            else
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG1_DONE;
            }
         }
         else
         {
            rgDBMInsBcchOnBch(cell, lcCfg->lcId);
            cell->cellActvState |= RG_BCCH_BCH_CFG_DONE;
         }
      }
      else  /* PCCH configuration */
      {
         rgDBMInsPcch(cell, lcCfg->lcId);
         cell->cellActvState |= RG_PCCH_CFG_DONE;
      }
   }

   /* Add to active cell list if cell is active */
   if (cell->cellActvState == RG_CELL_ACTIVE)
   {
      rgCb[inst].cell = cell;
      rgCb[inst].inactiveCell = NULLP;
      DU_LOG("\nDEBUG  -->  MAC : Cell  added to active list after common LC %d\
               config\n", lcCfg->lcId);
   }

   return ROK;
}  /* rgCFGCrgCmnLcCfg */
#ifdef LTE_L2_MEAS
/***********************************************************
 *
 *     Func : rgCFGFreeUeUlAlloc 
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgCFGFreeUeUlAlloc(RgCellCb *cell)
{
   uint8_t    sfIdx;
   Inst inst = cell->macInst - RG_INST_START;
   
   for(sfIdx = 0; sfIdx < RG_NUM_UL_SUB_FRAMES; sfIdx++)
   {
      if(cell->ulSf[sfIdx].ueUlAllocInfo != NULLP)
      {
         /*ccpu00117052 - MOD- Passing double pointer for proper
          *                    NULLP assignment */
         rgFreeSBuf(inst,(Data **)&(cell->ulSf[sfIdx].ueUlAllocInfo), 
               (cell->ulSf[sfIdx].numUe * sizeof(RgUeUlAlloc)));
      }
   }
}/* rgCFGFreeUeUlAlloc */
#endif
/***********************************************************
 *
 *     Func : rgCFGFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgCFGFreeCellCb(RgCellCb  *cell)
{
   Inst inst = cell->macInst - RG_INST_START;

#ifdef LTE_ADV
   RgLaaCellCbDeInit(cell);
#endif
   /* Free lists of the cell */
#ifdef LTEMAC_SPS
   rgCFGFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
   rgCFGFreeUeLst(cell);
   rgRAMFreeCell(cell);
   rgCFGFreeCmnLcLst(cell);
#ifdef LTE_L2_MEAS
   rgCFGFreeUeUlAlloc(cell);
#endif
   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                           assignment */
   /* Update satistics */
   rgUpdtCellCnt(inst,RG_CFG_DEL);
   rgDHMFreeAllTbBufs(inst);

   rgFreeSBuf(inst,(Data **)&cell->flowCntrlInd, sizeof(RguFlowCntrlInd));

   /* De-allocate the Cell */
   rgFreeSBuf(inst,(Data **)&cell, sizeof(*cell));
 

   DU_LOG("\nINFO  -->  MAC : Cell freed\n");

  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  return; 
}  /* rgCFGFreeCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeInactvCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees inactive cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgCFGFreeInactvCellCb(RgCellCb *cell)
{
   Inst      inst = cell->macInst - RG_INST_START;
   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);
#ifdef LTEMAC_SPS
   rgDBMDeInitSpsUeCbLst(cell);
#endif

   rgCFGFreeCmnLcLst(cell);

   rgFreeSBuf(inst, (Data **)&cell->flowCntrlInd, sizeof(RguFlowCntrlInd));
   /*ccpu00117052 - MOD- Passing double pointer for proper
                        NULLP assignment */
   /* De-allocate the Cell */
   rgFreeSBuf(inst,(Data **)&cell, sizeof(*cell));

   /* Update satistics */
   rgUpdtCellCnt(inst,RG_CFG_DEL);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  return; 
}  /* rgCFGFreeInactvCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
Void rgCFGFreeUeCb(RgCellCb *cell,RgUeCb *ue)
{
   Inst inst = cell->macInst - RG_INST_START;

   rgDHMFreeUe(inst,&ue->dl.hqEnt);

   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP
                          assignment */
   /* De-allocate the Ue */
   rgFreeSBuf(inst,(Data **)&ue, sizeof(*ue));

   /* Update Statistics */
   rgUpdtUeCnt(inst,RG_CFG_DEL);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  return; 
}  /* rgCFGFreeUeCb */

/***********************************************************
 *
 *     Func : rgCFGFreeCmnLcLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees common logical channels in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgCFGFreeCmnLcLst(RgCellCb *cell)
{
   rgDBMFreeCmnLcLst(cell);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  return; 
}  /* rgCFGFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgCFGFreeUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgCFGFreeUeLst(RgCellCb *cell)
{
   RgUeCb     *ue;

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextUeCb(cell, NULLP)) != NULLP)
   {
#ifdef LTE_ADV
      rgDelUeFrmAllSCell(cell,ue);
#endif
      rgDBMDelUeCb(cell, ue);
      rgCFGFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  return; 
}  /* rgCFGFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgCFGFreeSpsUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees Sps UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
static Void rgCFGFreeSpsUeLst(RgCellCb *cell)
{
   RgUeCb     *ue;

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextSpsUeCb(cell, NULLP)))
   {
      rgDBMDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitSpsUeCbLst(cell);

}  /* rgCFGFreeSpsUeLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Function for registering cell- scheduler instance mapping
 *
 * @details
 *
 *     Function : RgSchMacCellRegReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     cell configuration successfully.
 *     This shall create a mapping of the cell, scheduler instance that
 *     is serving the cell and the unique identifier of the cell on the 
 *     scheduler at MAC. This mapping shall be used for further 
 *     communication to the scheduler instance for this cell.
 *     
 *           
 *  @param[in] Pst*                pst,
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] RaRespReqInfo       raRespReq
 *  @return  S16
 *      -# ROK 
 **/
S16 RgSchMacCellRegReq(Pst*  pst,RgInfCellReg* regReq)
{
   Inst      inst;
   RgCellCb *cell = NULLP;

   RG_IS_INST_VALID(pst->dstInst);
   inst = pst->dstInst - RG_INST_START;
   cell = rgCb[inst].cell;

   if(NULLP == regReq)
   {
      return RFAILED;
   }
      
   if((cell  == NULLP) || (cell->cellId != regReq->cellId))
   {
      return RFAILED;
   }
   if(regReq->maxDlHqProcPerUe > RG_MAX_DL_HARQ_NUM) 
   {
      return RFAILED;
   }
   /* Initialize */
   cell->schInstMap.cellSapId = regReq->cellSapId;
   cell->schInstMap.schInst   = pst->srcInst;
   cell->maxDlHqProcPerUe = regReq->maxDlHqProcPerUe;

   return ROK;

} /* end of RgSchMacCellRegReq */

/*Added Ue for Onging L2 Meas*/
#ifdef LTE_L2_MEAS
/*LTE_L2_MEAS_PHASE2*/
S16 rgAddToL2MeasPerQci(RgCellCb  *cell,uint8_t qci)
{
   S16        ret = ROK;	
   CmLList    *lnk;
   RgL2MeasCb *measCb;
   uint16_t   idx;

   lnk = cell->l2mList.first;
   while(lnk != NULLP )
   {
      measCb = (RgL2MeasCb *)lnk->node;
      if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
      {
	 for(idx = 0;idx< measCb->measReq.t.prbReq.numQci;idx++)
	 {
	    if(measCb->measReq.t.prbReq.qci[idx] == qci)
	    {
	       break; /*exit from for loop*/
	    } 
	 }	
	 if(idx == measCb->measReq.t.prbReq.numQci)
	 {
	    cell->qciArray[qci].mask = TRUE; 
	    measCb->measReq.t.prbReq.qci[measCb->measReq.t.prbReq.numQci++] = qci;
	 }		
      } 	  
      lnk = lnk->next;
   }  /* End of While*/



   return (ret);
}


#endif

/**********************************************************************
 
         End of file
**********************************************************************/
