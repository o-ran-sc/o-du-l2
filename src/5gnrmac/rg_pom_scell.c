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
  
     Desc:     C source code for Entry point functions
  
     File:     rg_pom_scell.c 
  
**********************************************************************/

/** @file rg_pom_scell.c
@brief This module does processing related to handling of SCell related function.
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_prg.h"       /* PRG interface includes*/
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

/* header/extern include files (.x) */
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg_prg.x"       /* PRG interface typedefs*/
#include "du_app_mac_inf.h"
#include "rg.x"
#ifdef LTE_ADV
#include "rg_pom_scell.x"
static S16 rgPomVldtSCellDedLchCfg ARGS((
            Inst                       inst,
            RgPrgUeSCellLchAddInfo     *lcCfg,
            RgCellCb                   **cell,
            RgUeCb                     **ue
          ));
static S16 rgPomUeSCellDedLcCfg ARGS((RgCellCb                   *cell,
                                       RgUeCb                     *ue,
                                       RgPrgUeSCellLchAddInfo     *lcCfg
                                     ));
/**
 * @brief Handler for the Lch reconfiguration request from PMAC to SMAC.
 *
 * @details
 *
 *     Function : RgPrgPMacSMacUeSCellLchModReq
 *
 *     Processing Steps:
 *      - calls the function for validating cell, uecb and lch sent by PMAC
 *      - Updates the lch recfg into ueCb.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Pst                    *pst
 *  @param[in]  RgPrgUeSCellLchModInfo *lchCfgCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgPrgPMacSMacUeSCellLchModReq
(
Pst                      *pst,    
RgPrgUeSCellLchModInfo   *lchCfgCb
)
{
   RgPrgCfgCfmInfo  cfgCfm;
   Inst             inst = pst->dstInst;
   RgCellCb         *cell;
   S16              ret;
   Pst              cfmPst;    
   RgUeCb           *ue;
   RgUlLcCb         *ulLc;

   RGDBGPRM(inst,(rgPBuf(inst),
            "APPLYING CRG UE SCELL CONFIG: cellId %d ueId %d lcId %d lcgId %d\n",
         lchCfgCb->cellId, lchCfgCb->crnti,lchCfgCb->lcId,lchCfgCb->ulLchRecfg.lcgId));

   cfgCfm.ueId = lchCfgCb->crnti;
   cfgCfm.sCellId = lchCfgCb->cellId;
   cfgCfm.status = PRG_CFG_CFM_OK;
   cfgCfm.event = EVTPRGUESCELLLCHMODCFM; 
   rgGetPstToInst(&cfmPst, inst, pst->srcInst);

   ret = rgPomVltdModLch(inst,lchCfgCb, &cell, &ue,&ulLc);
  if(ret != ROK)
  {
     RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMAC SCell Lc Cfg failed:\
              cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));
     /* Set status as Not OK*/
     cfgCfm.status = PRG_CFG_CFM_NOK;
  }
  else
  {
     ret = rgPomUeSCellLcMod(inst, ue, ulLc, lchCfgCb);
     if(ret != ROK)
     {
        RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMac SCell lc cfg failed:\
                 cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));
        /* Set status as Not OK*/
        cfgCfm.status = PRG_CFG_CFM_NOK;
     }
  }
  
  RGDBGINFONEW(inst,(rgPBuf(inst), "[%d] Scell Lch Config done:\
           cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));

  /* Send positive confirmation to primary cell*/
  RgPrgSMacPMacCfg(&cfmPst, &cfgCfm);
  return ROK;
}  /* RgPrgPMacSMacUeSCellLchModReq */


/**
 * @brief A wrapper class to send the LCH Del req from PMAC to SMAC
 *
 * @details
 *
 *     Function : rgPomSndUeSCellLchDelToSmac 
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of values received in
 *          delete request.
 *        - Checks if the Ue has any scell added.
 *        - If validated successfully,
 *          - if SCells have been added ,then 
 *            send the lchDel structure to update the same values to SMAC.
 *        - Return ROK.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  CrgDel        *lcDel
 *  @param[in]  Bool          *isCfmRqrd
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomSndUeSCellLchDelToSmac
(
Inst            inst,
CrgDel          *lcDel,
Bool            *isCfmRqrd
)
{
   uint8_t                    idx = 0;
   Inst                       dstMacInst;
   Pst                        dstInstPst;
   RgPrgUeSCellLchDelInfo     delLcCb;
   RgCellCb                   *cell;
   RgUeCb                     *ue;

  /* Fetch the Active cell */
   if(((cell = rgCb[inst].cell) == NULLP) ||
       (cell->cellId != lcDel->u.lchDel.cellId))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), 
                       "[%d]Active Cell does not exist %d\n",
                                  lcDel->u.lchDel.crnti, lcDel->u.lchDel.cellId));
      return RFAILED;
   }

   RGDBGPRM(inst,(rgPBuf(inst), 
            "Filling SCell LCh Config : cellId %d ueId %d\n",
            cell->cellId, cell->ueId));

   if ((ue = rgDBMGetUeCb(cell, lcDel->u.lchDel.crnti)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), 
               "[%d]Ue does not exist\n", lcDel->u.lchDel.crnti));
      return RFAILED;
   }
   for(idx = 0; idx < RG_MAX_SCELL_PER_UE ; idx++)
   {
      if(TRUE == ue->sCelInfo[idx].isSCellAdded)
      {
         dstMacInst = ue->sCelInfo[idx].macInst - RG_INST_START;
         delLcCb.cellId = ue->sCelInfo[idx].sCellId;
         delLcCb.crnti = lcDel->u.lchDel.crnti;
         delLcCb.lcId = lcDel->u.lchDel.lcId;
         delLcCb.dir = lcDel->u.lchDel.dir;

      /* Get post structure of the cell to whom delLcCb needs to be sent
       * And then send the lch recfg based on Mac instances */
         rgGetPstToInst(&dstInstPst, inst, dstMacInst);
         RgPrgPMacSMacUeScellLchDel(&dstInstPst, &delLcCb);
         *isCfmRqrd = FALSE;
      }
   }
   return ROK;
}


/**
 * @brief A wrapper class to send the LCH reconfig req from PMAC to SMAC
 *
 * @details
 *
 *     Function : rgPomSndUeSCellLchModToSmac 
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - Checks if the Ue has any scell added.
 *        - If validated successfully,
 *          - if SCells have been added ,then 
 *            send the lchRecfg structure to update the same values to SMAC.
 *        - Return ROK.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          inst
 *  @param[in]  RgCellCb      *cell
    @param[in]  RgUeCb        *ue,
    @param[in]  CrgLchRecfg   *lchRecfg,
    @param[in]  Bool          *isCfmRqrd
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomSndUeSCellLchModToSmac
(
Inst           inst,
RgCellCb       *cell,
RgUeCb         *ue,
CrgLchRecfg    *lchRecfg,
Bool           *isCfmRqrd
)
{
   uint8_t                   idx = 0;
   Inst                      dstMacInst;
   RgPrgUeSCellLchModInfo    lchCfgCb; 
   Pst                       dstInstPst;

   for(idx = 0; idx < RG_MAX_SCELL_PER_UE ; idx++)
   {
      if(TRUE == ue->sCelInfo[idx].isSCellAdded)
      {
         dstMacInst = ue->sCelInfo[idx].macInst - RG_INST_START;
         lchCfgCb.cellId = ue->sCelInfo[idx].sCellId;
         lchCfgCb.crnti = lchRecfg->crnti;
         lchCfgCb.lcId = lchRecfg->lcId;
         lchCfgCb.ulLchRecfg.lcgId = lchRecfg->ulRecfg.lcgId;

      /* Get post structure of the cell to whom lchCfgCb needs to be sent
       * And then send the lch recfg based on Mac instances */
         rgGetPstToInst(&dstInstPst, inst, dstMacInst);
         RgPrgPMacSMacUeScellLchMod(&dstInstPst, &lchCfgCb);
         *isCfmRqrd = FALSE;
      }
   }
   return ROK;
}

/**
 * @brief A wrapper class to send the LCH config req from PMAC to SMAC
 *
 * @details
 *
 *     Function : rgPomSndUeSCellLchAddToSmac
 *
 *     Processing Steps:
 *        - Checks if the Ue has any scell added.
 *          - if SCells have been added ,then 
 *            send the lchCfgCb structure to update the same values to SMAC.
 *        - Return ROK.
 *      - Else return RFAILED.
 *
 *  @param[in]  Inst          insg
 *
 *  @param[in]  RgCellCb      *cell
 *  @paran[in]  RgUeCb        *ue
 *  @paran[in]  CrgLchCfg     *lchCfg
 *  @paran[in]   Bool         *isCfmRqrd
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomSndUeSCellLchAddToSmac
(
Inst           inst,
RgCellCb       *cell,
RgUeCb         *ue,
CrgLchCfg      *lchCfg,
Bool           *isCfmRqrd
)
{
   uint8_t                   idx = 0;
   Inst                      dstMacInst;
   RgPrgUeSCellLchAddInfo    lchCfgCb; 
   Pst                       dstInstPst;

   for(idx = 0; idx < RG_MAX_SCELL_PER_UE ; idx++)
   {
      if(TRUE == ue->sCelInfo[idx].isSCellAdded)
      {
         dstMacInst = ue->sCelInfo[idx].macInst - RG_INST_START;
         lchCfgCb.cellId = ue->sCelInfo[idx].sCellId;
         lchCfgCb.crnti =  lchCfg->crnti;
         lchCfgCb.lcId = lchCfg->lcId;
         lchCfgCb.lcType = lchCfg->lcType;
         lchCfgCb.dir = lchCfg->dir;
         lchCfgCb.dlInfo.dlTrchType = lchCfg->dlInfo.dlTrchType;
         lchCfgCb.ulInfo.ulTrchType = lchCfg->ulInfo.ulTrchType;
         lchCfgCb.ulInfo.lcgId = lchCfg->ulInfo.lcgId;
#ifdef LTE_L2_MEAS
         lchCfgCb.qci = lchCfg->qci;
#endif /* LTE_L2_MEAS */

      /* Get post structure of the cell to whom lchCfgCb needs to be sent
       * And then send the lch recfg based on Mac instances */
         rgGetPstToInst(&dstInstPst, inst, dstMacInst);
         RgPrgPMacSMacUeScellLchAdd(&dstInstPst, &lchCfgCb);
         *isCfmRqrd = FALSE;
      }
   }
   return ROK;
} /* rgPomSndUeSCellLchAddToSmac */

/**
 * @brief Handler for the Lch delete request from PMAC to SMAC.
 *
 * @details
 *
 *     Function : RgPrgPMacSMacUeSCellLchDelReq
 *
 *     Processing Steps:
 *      - calls the function for validating cell, uecb and Lch sent by PMAC
 *      - If successful, delete the logical channel 
 *        else Rollback and FAIL.
 *
 *  @param[in]  Pst                    *pst
 *  @param[in]  RgPrgUeSCellLchDelInfo *lchCfgCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgPrgPMacSMacUeSCellLchDelReq
(
Pst                       *pst,    
RgPrgUeSCellLchDelInfo    *delLcCb
)
{
   RgPrgCfgCfmInfo  cfgCfm;
   Inst             inst = pst->dstInst;
   RgCellCb         *cell;
   S16              ret;
   Pst              cfmPst;    
   RgUeCb           *ue;
   RgUlLcCb         *ulLc;
   RgDlLcCb         *dlLc;

   RGDBGPRM(inst,(rgPBuf(inst),
            "APPLYING CRG UE SCELL CONFIG: cellId %d ueId %d\n",
         lchCfgCb->cellId, lchCfgCb->crnti));

   cfgCfm.ueId = delLcCb->crnti;
   cfgCfm.sCellId = delLcCb->cellId;
   cfgCfm.status = PRG_CFG_CFM_OK;
   cfgCfm.event = EVTPRGUESCELLLCHDELCFM; 
   rgGetPstToInst(&cfmPst, inst, pst->srcInst);

   ret = rgPomVltdDelLch(inst,delLcCb, &cell, &ue,&ulLc,&dlLc);
  if(ret != ROK)
  {
     RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMAC SCell Lc Cfg failed:\
              cellId %d\n", delLcCb->crnti, delLcCb->cellId));
     /* Set status as Not OK*/
     cfgCfm.status = PRG_CFG_CFM_NOK;
  }
  else
  {
     ret = rgPomUeSCellLcDel(inst, delLcCb, ue, ulLc,dlLc);
     if(ret != ROK)
     {
        RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMac SCell lc cfg failed:\
                 cellId %d\n", delLcCb->crnti, delLcCb->cellId));
        /* Set status as Not OK*/
        cfgCfm.status = PRG_CFG_CFM_NOK;
     }
  }
  
  RGDBGINFONEW(inst,(rgPBuf(inst), "[%d] Scell Lch Config done:\
           cellId %d\n", delLcCb->crnti, delLcCb->cellId));

  /* Send positive confirmation to primary cell*/
  RgPrgSMacPMacCfg(&cfmPst, &cfgCfm);
  return ROK;
}  /* RgPrgPMacSMacUeSCellLchDelReq */


/**
 * @brief Handler for the Lch configuration request from PMAC to SMAC.
 *
 * @details
 *
 *     Function : RgPrgPMacSMacUeSCellLchAddReq
 *
 *     Processing Steps:
 *      - calls the function for validating cell, uecb and Lch sent by PMAC
 *      - Updates the lch recfg into ueCb.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Pst                 *pst
 *  @param[in]  RgPrgLchRecfgInfo   *lchCfgCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgPrgPMacSMacUeSCellLchAddReq
(
Pst                      *pst,    
RgPrgUeSCellLchAddInfo   *lchCfgCb
)
{
   RgPrgCfgCfmInfo   cfgCfm;
   Inst              inst = pst->dstInst;
   RgCellCb          *cell;
   S16               ret;
   Pst               cfmPst;    
   RgUeCb            *ue;

   RGDBGPRM(inst,(rgPBuf(inst),
            "APPLYING UE SCELL CONFIG AT SMAC : cellId %d ueId %d\n",
         lchCfgCb->cellId, lchCfgCb->crnti));

   cfgCfm.ueId = lchCfgCb->crnti;
   cfgCfm.sCellId = lchCfgCb->cellId;
   cfgCfm.status = PRG_CFG_CFM_OK;
   cfgCfm.event = EVTPRGUESCELLLCHADDCFM; 
   rgGetPstToInst(&cfmPst, inst, pst->srcInst);

   ret = rgPomVldtAddLch(inst,lchCfgCb, &cell, &ue);
  if(ret != ROK)
  {
     RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMAC SCell Lc Cfg failed:\
              cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));
     /* Set status as Not OK*/
     cfgCfm.status = PRG_CFG_CFM_NOK;
  }
  else
  {
     ret = rgPomUeSCellLcAdd(inst, cell, ue, lchCfgCb);
     if(ret != ROK)
     {
        RGDBGERRNEW(inst,(rgPBuf(inst), "[%d] SMac SCell lc cfg failed:\
                 cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));
        /* Set status as Not OK*/
        cfgCfm.status = PRG_CFG_CFM_NOK;
     }
  }
  
  RGDBGINFONEW(inst,(rgPBuf(inst), "[%d] Scell Lch Config done:\
           cellId %d\n", lchCfgCb->crnti, lchCfgCb->cellId));

  /* Send positive confirmation to primary cell*/
  RgPrgSMacPMacCfg(&cfmPst, &cfgCfm);
  return ROK;
}  /* RgPrgPMacSMacUeSCellLchAddReq */
/**
 * @brief Validates the logical channel configuration request from PMAC to SMAC.
 *
 * @details
 *
 *     Function : rPommVldtAddLch
 *
 *     Processing Steps:
 *      - Validate the logical channel configuration request from PMAC to
 *        SMAC : validate if configured values are within the range.
 *      - If validated successfully,
 *        - Return ROK and pointer to cell and UE for dedicated logical channels.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  Inst                     inst
 *  @param[in]  RgPrgUeSCellLchAddInfo   *lcCfg
 *  @param[out] RgCellCb                 **cell
 *  @param[out] RgUeCb                   **ue
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomVldtAddLch
(
Inst                    inst, 
RgPrgUeSCellLchAddInfo  *lcCfg,
RgCellCb                **cell,
RgUeCb                  **ue
)
{

   RGDBGPRM(inst,(rgPBuf(inst), "VALIDATE SMAC LC CONFIG: cellId %d ueId %d lcId %d\
            cell %p ue %p\n", lcCfg->cellId, lcCfg->crnti, lcCfg->lcId,
            (void*)*cell, (void*)*ue));

   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      /* Dedicated logical channels */
      if ((rgPomVldtSCellDedLchCfg(inst,lcCfg, cell, ue)) != ROK)
      {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UEID Validation for dedicated LC failed\n",
                  lcCfg->crnti));
         return RFAILED;
      }
   }
   else
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UEID Invalid logical channel type %d\n",\
               lcCfg->crnti, lcCfg->lcType));
      return RFAILED;
   }
#ifdef LTE_L2_MEAS
   if ( lcCfg->qci <  RG_QCI_MIN ||
        lcCfg->qci >  RG_QCI_MAX
      )
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UEID Invalid qci %x\n",
               lcCfg->crnti, lcCfg->qci));
      return RFAILED;
   }
   /*validate qci */
#endif /*LTE_L2_MEAS */

   RGDBGINFONEW(inst,(rgPBuf(inst),"[%d]UEID CRG LCconfig validation done:cellId:%d lcId:%d\n",
            lcCfg->crnti, lcCfg->cellId, lcCfg->lcId));
   return ROK;
}
/**
 * @brief Handler for the logical channel configuration request from
 * PMAC to SMAC.
 *
 * @details
 *
 *     Function : rgPomUeSCellLcAdd
 *
 *     Processing Steps:
 *      - Update the dedicated logical channel Cb with the configured values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgLchCfg   *lcCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomUeSCellLcAdd
(
Inst                     inst,
RgCellCb                 *cell,
RgUeCb                   *ue,
RgPrgUeSCellLchAddInfo   *lcCfg
)
{

   RGDBGPRM(inst,(rgPBuf(inst), "APPLYING CRG LC CONFIG: cellId %d ueId %d\
            lcId %d dir %d cell %p ue %p\n", lcCfg->cellId, lcCfg->crnti,
            lcCfg->lcId, lcCfg->dir, (void*)cell, (void*)ue));
   
      if ((rgPomUeSCellDedLcCfg(cell, ue, lcCfg)) != ROK)
      {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Dedicated logical channel configuration"
                  "failed in SCell%d\n", lcCfg->crnti, lcCfg->lcId));
         return RFAILED;
      }

   RGDBGINFONEW(inst,(rgPBuf(inst), "[%d]SCell LC config done: cellId %d lcId %d\n",
          lcCfg->crnti, lcCfg->cellId, lcCfg->lcId));
   return ROK;
}  /* rgPomUeSCellLcAdd */

/***********************************************************
 *
 *     Func : rgPomVldtSCellDedLchCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration received from PMAC.
 *
 *     @param[in] Inst                     inst
 *     @param[in] RgPrgUeScellLchAddInfo   *lcCfg 
 *     @param[in] RgCellCb                 **cell
 *     @param[in] RgUeCb                   **ue
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
static S16 rgPomVldtSCellDedLchCfg
(
Inst                       inst, 
RgPrgUeSCellLchAddInfo     *lcCfg,
RgCellCb                   **cell,
RgUeCb                     **ue
)
{
   RGDBGPRM(inst,(rgPBuf(inst), "VALIDATING CRG DEDICATED LC CONFIG \n"));


   /* Fetch the Active cell */
   if (((*cell = rgCb[inst].cell) == NULLP)
      || ((*cell)->cellId != lcCfg->cellId))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Active Cell does not exist: Cell %d\n",
               lcCfg->crnti, lcCfg->cellId));
      return RFAILED;
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UE  does not exist for dedicated logical"
               "channel %d\n", lcCfg->crnti, lcCfg->lcId));
      return RFAILED;
   }

   /* Validate logical channel Id */
   /*if ((lcCfg->lcId < RG_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RG_DEDLC_MAX_LCID))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Invalid logical channel Id %d\n",
                lcCfg->crnti, lcCfg->lcId));
      return RFAILED;
   }*/
   /* Validate downlink info */
   if (lcCfg->dir & PRG_DIR_TX)
   {
      if (rgDBMGetDlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UE: Dedicated DL LC %d already configured\n",
                   lcCfg->crnti, lcCfg->lcId));
         return RFAILED;
      }
/*      dirVld = TRUE;*/
   }

   /* Validate uplink info */
   if (lcCfg->dir & PRG_DIR_RX)
   {
      if (lcCfg->ulInfo.lcgId > (RG_MAX_LCG_PER_UE - 1))
      {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UE: Invalid lcgId for uplink logical"
                  "channel %d\n", lcCfg->crnti, lcCfg->ulInfo.lcgId));
         return RFAILED;
      }
      if (rgDBMGetUlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]UE: Dedicated UL LC %d already configured\n",
                   lcCfg->crnti, lcCfg->lcId));
         return RFAILED;
      }
/*      dirVld = TRUE;*/
   }
/*
   if (!dirVld)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Invalid Direction %d \n",
               lcCfg->crnti, lcCfg->dir));
      return RFAILED;
   }
*/
   RGDBGINFONEW(inst,(rgPBuf(inst), "[%d]Dedicated logical channel %d validated"
            "for cell %d\n", lcCfg->crnti, lcCfg->lcId, lcCfg->cellId));
   return ROK;
}  /* rgPomVldtSCellDedLchCfg */
/***********************************************************
 *
 *     Func : rgPomUeSCellDedLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration received from PMAC.
 *
 *     @param[in] RgCellCb                 *cell
 *     @param[in] RgUeCb                   *ue 
 *     @param[in] RgPrgUeSCellLchAddInfo   *lcCfg
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
static S16 rgPomUeSCellDedLcCfg
(
RgCellCb                   *cell,
RgUeCb                     *ue,
RgPrgUeSCellLchAddInfo     *lcCfg
)
{
   //Inst     inst = cell->macInst - RG_INST_START;
   //RGDBGPRM(inst,(rgPBuf(inst), "APPLYING DEDICATED LC CONFIG\n"));

   /* Uplink/Bi-directional logical channel */
   if (lcCfg->dir & PRG_DIR_RX)
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
   if (lcCfg->dir & PRG_DIR_TX)
   {
      rgDBMInsDlDedLcCb(ue, lcCfg->lcId);
   }
   RGDBGINFO(inst,(rgPBuf(inst), "Dedicated LC config done\n"));
   return ROK;

  }  /* rgPomUeSCellDedLcCfg */
/**
 * @brief Function to validate SCellLchReCfg.
 *
 * @details
 *
 *     Function : rgPomVltdModLch
 *     
 *           
 *  @param[in] inst        instance number to fetch rgCb instance
 *  @param[in] lchCfg Cb   lchCfg CB for validation
 *  @param[out] cell        cell control block
 *  @param[out] RgCellCb     **cell
 *  @param[out] RgUeCb       **ue
 *  @param[out] RgUlLcCb     **ulLc
 *  @return  S16
 *      -# ROK 
 **/
S16 rgPomVltdModLch
(
Inst                     inst,
RgPrgUeSCellLchModInfo   *lchCfgCb,
RgCellCb                 **cell,
RgUeCb                   **ue,
RgUlLcCb                 **ulLc
)
{
   RGDBGPRM(inst,(rgPBuf(inst), "VALIDATE SMAC LC RECONFIG: cellId %d ueId %d \
            lcId %d cell %p ue %p ulLc %p\n",lchCfgCb->cellId,
            lchCfgCb->crnti,lchCfgCb->lcId, (void*)*cell, (void*)*ue,
            (void*)*ulLc));
   /* Fetch the cell */
   if ((((*cell = rgCb[inst].cell)) == NULLP)
         || ((*cell)->cellId != lchCfgCb->cellId))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Active Cell does not exist %d\n",lchCfgCb->crnti, lchCfgCb->cellId));
      return RFAILED;
   }
   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgDBMGetUeCb(*cell, lchCfgCb->crnti)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Ue does not exist for dedicated logical channel\n",
               lchCfgCb->crnti));
      return RFAILED;
   }
   if ((*ulLc = rgDBMGetUlDedLcCb((*ue), lchCfgCb->lcId)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Dedicated UL LC does not exist %d\n",
               lchCfgCb->crnti, lchCfgCb->lcId));
      return RFAILED;
   }
  /* if (lchCfgCb->ulLchRecfg.lcgId > (RG_MAX_LCG_PER_UE - 1))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Invalid lcgId for uplink logical channel: lcg %d"
               "lc %d\n",lchCfgCb->crnti, lchCfgCb->ulLchRecfg.lcgId, lchCfgCb->lcId));
      return RFAILED;
   }*/
   return ROK;
} /*rgPomVltdModLch*/
/**
 * @brief Handler for the logical channel re-configuration request from
 * PMAC to SMAC.
 *
 * @details
 *
 *     Function : rgPomUeSCellLcMod
 *
 *     Processing Steps:
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  Inst              inst
 *  @param[in]  RgUlUeCb          *ue
 *  @param[in]  RgUlLcCb          *ulLc
 *  @param[in]  RgPrgLchRecfgInfo *lchCfgCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomUeSCellLcMod
(
Inst                    inst,
RgUeCb                  *ue,
RgUlLcCb                *ulLc,
RgPrgUeSCellLchModInfo  *lchCfgCb
)
{
   RGDBGPRM(inst,(rgPBuf(inst), "APPLYING SMAC LC RECONFIG: cellId %d ueId %d\
            lcId %d  \n",
            lchCfgCb->cellId, lchCfgCb->crnti, lchCfgCb->lcId));

   if (ulLc->lcgId != lchCfgCb->ulLchRecfg.lcgId)
   {
      rgDBMUpdUlDedLcCb(ue, ulLc, lchCfgCb->ulLchRecfg.lcgId);
   }

   RGDBGINFO(inst,(rgPBuf(inst), "LC %d of Ue %d of cell %d Reconfigured\n", 
            lchCfgCb->lcId, ue->ueId, cell->cellId));
   return ROK;
}  /* rgPomUeSCellLcMod */
/**
 * @brief Function to validate SCellLchDel.
 *
 * @details
 *
 *     Function : rgPomVltdDelLch
 *     
 *           
 *  @param[in] inst        instance number to fetch rgCb instance
 *  @param[in] lchCfg Cb   lchCfg CB for validation
 *  @param[out] cell        cell control block
 *  @param[out] RgCellCb     **cell
 *  @param[out] RgUeCb       **ue
 *  @param[out] RgUlLcCb     **ulLc
 *  @return  S16
 *      -# ROK 
 **/
S16 rgPomVltdDelLch
(
Inst                      inst,
RgPrgUeSCellLchDelInfo    *delLcCb,
RgCellCb                  **cell,
RgUeCb                    **ue,
RgUlLcCb                  **ulLc,
RgDlLcCb                  **dlLc
)
{
   RGDBGPRM(inst,(rgPBuf(inst), "VALIDATE SMAC LC RECONFIG: cellId %d ueId %d \
            lcId %d cell %p ue %p ulLc %p\n",delLcCb->cellId,
            delLcCb->crnti,delLcCb->lcId, (void*)*cell, (void*)*ue,
            (void*)*ulLc));
   /* Fetch the cell */
   if ((((*cell = rgCb[inst].cell)) == NULLP)
         || ((*cell)->cellId != delLcCb->cellId))
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Active Cell does not exist %d\n",delLcCb->crnti, delLcCb->cellId));
      return RFAILED;
   }
   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgDBMGetUeCb(*cell, delLcCb->crnti)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Ue does not exist for dedicated logical channel\n",
               delLcCb->crnti));
      return RFAILED;
   }
   if ((*ulLc = rgDBMGetUlDedLcCb((*ue), delLcCb->lcId)) == NULLP)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Dedicated UL LC does not exist %d\n",
               delLcCb->crnti, delLcCb->lcId));
      return RFAILED;
   }
   if ((*dlLc = rgDBMGetDlDedLcCb((*ue), delLcCb->lcId)) == NULLP)
   {
         RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]DL LC %d does not exist\n",
               delLcCb->crnti, delLcCb->lcId));
         return RFAILED;
   }
   return ROK;
} /*rgPomVltdDelLch*/
/**
 * @brief Handler for the logical channel delete request from
 * PMAC to SMAC.
 *
 * @details
 *
 *     Function : rgPomUeSCellLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  Inst                      inst
 *  @param[in]  RgPrgUeSCellLchDelInfo    *delLcCb
 *  @param[in]  RgUeCb                    *ue
 *  @param[in]  RgUlLcCb                  *ulLc
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgPomUeSCellLcDel
(
Inst                      inst,
RgPrgUeSCellLchDelInfo    *delLcCb,
RgUeCb                    *ue,
RgUlLcCb                  *ulLc,
RgDlLcCb                  *dlLc
)
{

   RGDBGPRM(inst,(rgPBuf(inst), "APPLYING CRG LC DELETE: cellId %d ueId %d\
            lcId %d dir %d\n", delLcCb->cellId,
            delLcCb->crnti, delLcCb->lcId,
            delLcCb->dir));


   /* Validate downlink info */
   if (delLcCb->dir & PRG_DIR_TX)
   {
      rgDBMDelDlDedLcCb(ue, dlLc);
/*      dirVld = TRUE;*/
   }

   /* Validate uplink info */
   if (delLcCb->dir & PRG_DIR_RX)
   {
      rgDBMDelUlDedLcCb(ue, ulLc);
/*      dirVld = TRUE;*/
   }

   /*if (!dirVld)
   {
      RGDBGERRNEW(inst,(rgPBuf(inst), "[%d]Invalid direction %d for LC Delete\n",
            delLcCb->crnti, delLcCb->dir));
      return RFAILED;
   }*/
   RGDBGINFONEW(inst,(rgPBuf(inst), "[%d]UE's Logical channel %d deleted from cell %d\n",
         delLcCb->crnti, delLcCb->lcId,
         delLcCb->cellId));
   return ROK;
}  /* rgPomUeSCellLcDel */
#endif /*LTE_ADV */
/**********************************************************************
 
         End of file
**********************************************************************/
