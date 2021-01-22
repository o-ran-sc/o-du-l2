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
  
     File:     rg_com.c 
  
**********************************************************************/

/** @file rg_com.c
@brief This module does processing related to handling of upper interface APIs 
invoked by RRC towards MAC.
*/


/* header include files -- defines (.h) */
#include "common_def.h"
#include "tfu.h"
#include "du_app_mac_inf.h"
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

/* header/extern include files (.x) */
#include "tfu.x"
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "rg_sch_inf.x"
#include "rg_prg.x"       /* PRG interface typedefs*/
#include "rg.x"
#ifdef LTE_ADV
#include "rg_pom_scell.x"
#endif
/* local defines */
static S16 rgCOMHndlCfgReq ARGS((Inst inst,CrgCfg  *cfg, RgErrInfo *errInfo,Bool *isCfmRqrd,CrgCfgTransId transId));
static S16 rgCOMHndlRecfgReq ARGS((Inst inst,CrgRecfg *recfg, RgErrInfo *errInfo, \
         CrgCfgTransId transId,Bool *isCfmRqrd));
static S16 rgCOMHndlDelReq ARGS((Inst inst,CrgDel *del, RgErrInfo *errInfo, Bool *isCfmRqrd, CrgCfgTransId transId));
static S16 rgCOMHndlResetReq ARGS((Inst inst,CrgRst *reset,RgErrInfo *errInfo));
/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler to handle config request from RRC to MAC.
 *
 * @details
 *
 *     Function: rgCOMCfgReq
 *     
 *     This API handles processing for config request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - If configuration, process configuration request. Call rgCOMHndlCfgReq.
 *      - else If re-configuration, process re-configuration request. 
 *        Call rgCOMHndlRecfgReq.
 *      - else If reset, process reset request. Call rgCOMHndlResetReq.
 *      - else If delete, process delete request. Call rgCOMHndlDelReq.
 *      - If successful, send confirmation to RRC. Call rgUIMCrgCfgCfm.
 *      - If failed, FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgCfgTransId transId
 *  @param[in]  CrgCfgReqInfo *crgCfgReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgCOMCfgReq
(
Inst          inst,
CrgCfgTransId transId,
CrgCfgReqInfo *crgCfgReq
)
{
   S16             ret;
   uint8_t         cfmStatus = CRG_CFG_CFM_OK;
   RgErrInfo       errInfo;
   Bool            isCfmRqrd = TRUE;

   /* Process Config/Reconfig/Delete request from RRC */
   switch (crgCfgReq->action)
   {
      case CRG_CONFIG:
         {
            ret = rgCOMHndlCfgReq(inst,&crgCfgReq->u.cfgInfo, &errInfo,&isCfmRqrd, transId);
            break;
         }
      case CRG_RECONFIG:
         {
            ret = rgCOMHndlRecfgReq(inst,&crgCfgReq->u.recfgInfo, &errInfo, transId, &isCfmRqrd);
            break;
         }
         /* Start: LTEMAC_2.1_DEV_CFG */
      case CRG_RESET:
         {
            ret = rgCOMHndlResetReq(inst,&crgCfgReq->u.rstInfo, &errInfo);
            break;
         }
         /* End: LTEMAC_2.1_DEV_CFG */
      case CRG_DELETE:
         {
            ret = rgCOMHndlDelReq(inst,&crgCfgReq->u.delInfo, &errInfo, &isCfmRqrd, transId);
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  MAC : Invalid configuration action %d",
            crgCfgReq->action);

            ret = RFAILED;
         }
   }

   if (ret != ROK)
   {
      cfmStatus = CRG_CFG_CFM_NOK;
   }

   /* When UeSCellCfg is present then confirmation will be sent later once
      confirm from all SMAC are recved at PMAC. PMAC will send a consolidated
      confirm to RRC.Handling the failure of PMAC for Ue Scell add*/
#ifdef LTE_ADV
if(TRUE == isCfmRqrd)
   {
#endif
      /* Send back confirmation status to RRC */
      rgUIMCrgCfgCfm(inst,transId, cfmStatus); 
#ifdef LTE_ADV
   }
#endif
   DU_LOG("\nINFO  -->  MAC : CRG Configuration request processed\n");
   return (ret);
}  /* rgCOMCfgReq */
/**
 * @brief Handler for processing Cell/Ue/Logical channel configuration request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlCfgReq
 *     
 *     This API handles processing of configuration request from RRC to MAC. 
 *     
 *     Processing Steps:
 *        - Validate configuration request parameters at CFG module. 
 *          Call rgCFGVldtCrgCellCfg, rgCFGVldtCrgUeCfg, rgCFGVldtCrgLcCfg 
 *          for Cell, UE and Logical channel configuration respectively.
 *        - If validated successfully, Call rgCFGCrgCellCfg, rgCFGCrgUeCfg, 
 *          rgCFGCrgLcCfg for Cell, UE and Logical channel configuration 
 *          respectively, else FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgCfg    *cfg
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgCOMHndlCfgReq
(
Inst             inst,
CrgCfg           *cfg,
RgErrInfo        *errInfo,
Bool             *isCfmRqrd,
CrgCfgTransId    transId
)
{
   S16       ret;
   RgCellCb  *cell = NULLP;
   RgUeCb    *ue   = NULLP;

   errInfo->errType = RGERR_COM_CFG_REQ;

   /* Validate and process the configuration request */ 
   switch (cfg->cfgType)
   {
      case CRG_CELL_CFG:
      {
         ret = rgCFGVldtCrgCellCfg(inst,&cfg->u.cellCfg,errInfo);
         if (ret != ROK)
         {
              DU_LOG("\nERROR  -->  MAC : Cell configuration validation FAILED\n");
              return RFAILED;
         }
         ret = rgCFGCrgCellCfg(inst,&cfg->u.cellCfg, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {
         {
            ret = rgCFGVldtCrgUeCfg(inst,&cfg->u.ueCfg, &cell, errInfo);
            if (ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : UE configuration validation FAILED\n");
               return RFAILED;
            }
            ret = rgCFGCrgUeCfg(inst,cell, &cfg->u.ueCfg, errInfo);
         }
         break;
      }
      case CRG_LCH_CFG:
      {

         ret = rgCFGVldtCrgLcCfg(inst,&cfg->u.lchCfg, &cell, &ue,errInfo);
         if (ret != ROK)
         {
            
            DU_LOG("\nERROR  -->  MAC : LC configuration validation FAILED: LC %d\n", cfg->u.lchCfg.lcId);
            return RFAILED;
         }
         ret = rgCFGCrgLcCfg(inst,cell, ue, &cfg->u.lchCfg, errInfo,isCfmRqrd,transId);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  MAC : Should never come here: cfgType %d",cfg->cfgType);
         return RFAILED;
      }
   }

   return (ret);
}  /* rgCOMHndlCfgReq */


/**
 * @brief Handler for processing Cell/Ue/Logical channel re-configuration request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlRecfgReq
 *     
 *     This API handles processing of reconfiguration request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - Validate reconfiguration request parameters at CFG module. Call 
 *        rgCFGVldtCrgCellRecfg, rgCFGVldtCrgUeRecfg, rgCFGVldtCrgLchRecfg for 
 *        Cell, UE and logical channel reconfiguration respectively.
 *      - If validated, Call rgCFGCrgCellRecfg, rgCFGCrgUeRecfg, 
 *        rgCFGCrgLchRecfg for Cell, UE and Logical channel re-configuration 
 *        respectively else FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgRecfg   *recfg
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgCOMHndlRecfgReq
(
Inst             inst,
CrgRecfg         *recfg,
RgErrInfo        *errInfo,
CrgCfgTransId    transId,
Bool             *isCfmRqrd
)
{
   S16       ret;
   RgCellCb  *cell = rgCb[inst].cell;
   RgUeCb    *ue   = NULLP;
   RgUlLcCb  *ulLc = NULLP;

   errInfo->errType = RGERR_COM_RECFG_REQ;
   
   /* Validate and process the re-configuration request */ 
   switch (recfg->recfgType)
   {
      case CRG_CELL_CFG:
      {
         ret = rgCFGVldtCrgCellRecfg(inst,&recfg->u.cellRecfg, &cell, errInfo);
            if (ret != ROK) 
            {
               DU_LOG("\nERROR  -->  MAC : Cell Recfg Validation FAILED");
               return RFAILED;
            }
         ret = rgCFGCrgCellRecfg(inst,cell, &recfg->u.cellRecfg, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {
         /*ccpu00126865 - Added as a part of RRC Reestablishment issue with MAC
          * having a possibility of sending NOK */
         if (recfg->u.ueRecfg.oldCrnti != recfg->u.ueRecfg.newCrnti)
         {
            errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RECFG;
            ret = ROK;
         }    
         else
         {
#ifdef LTE_ADV
         /* Check for isSCellCfgPres */
         if(TRUE == recfg->u.ueRecfg.crgSCellCfg.isSCellCfgPres)
         {
            ret = rgFillAndAddSCellCfg(inst, cell, &recfg->u.ueRecfg, transId, isCfmRqrd);
            if (ret != ROK)
            {
               DU_LOG("\nERROR  -->  MAC : [%d]UE SCell configuration FAILED for inst [%d]\n",
                        recfg->u.ueRecfg.oldCrnti, inst);
               return RFAILED;
            }

         }
         else
         {
#endif /* LTE_ADV */
            ret = rgCFGVldtCrgUeRecfg(inst,&recfg->u.ueRecfg, &cell, &ue, errInfo);
            if ( ret != ROK)
            {
                DU_LOG("\nERROR  -->  MAC : UE Re-configuration validation FAILED OLD CRNTI:%d",
                      recfg->u.ueRecfg.oldCrnti);
               return RFAILED;
            }
            ret = rgCFGCrgUeRecfg(inst,cell, ue, &recfg->u.ueRecfg, errInfo);
         }
#ifdef LTE_ADV
         }
#endif
         break;
      }
      case CRG_LCH_CFG:
      {
         ret = rgCFGVldtCrgLcRecfg(inst,&recfg->u.lchRecfg, &cell, &ue, 
               &ulLc, errInfo);
         if (ret != ROK)
         {
            DU_LOG("\nERROR  -->  MAC : LC Re-configuration validation FAILED LCID:%d CRNTI:%d",
                      recfg->u.lchRecfg.lcId,recfg->u.lchRecfg.crnti);
            return RFAILED;
         }

#ifdef LTE_ADV
         /*ERAB- multicell fix*/
         memcpy( &(ue->cfgCfmInfo.transId), &transId,
               sizeof(CrgCfgTransId));
#endif
         ret = rgCFGCrgLcRecfg(inst,cell, ue, ulLc,
               &recfg->u.lchRecfg, errInfo,isCfmRqrd);

         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  MAC : Should never come here: recfgType %d",
                  recfg->recfgType);
         return RFAILED;
      }
   }

   return (ret);
}  /* rgCOMHndlRecfgReq */

/*Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Handler for processing UE Reset request recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlResetReq
 *     
 *     This API handles processing of Reset request from RRC to MAC. 
 *     
 *     Processing Steps:
 *      - Validate reset request parameters at CFG module. Call 
 *        rgCFGVldtCrgUeReset for UE reset.
 *      - If validated, Call rgCFGCrgUeReset for UE reset, else FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgRst     *reset
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgCOMHndlResetReq
(
Inst       inst,
CrgRst     *reset,
RgErrInfo  *errInfo
)
{
   /* Fix : ccpu00126865: ignore CRG reset. Let SCH trigger it. */
   
   errInfo->errCause = RGERR_NONE;
   DU_LOG("\nINFO  -->  MAC : CRG UE Reset processed \n");
   return ROK;
}  /* rgCOMHndlResetReq */
/*End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Handler for processing Cell/UE/Logical channel delete request
 * recieved from RRC.
 *
 * @details
 *
 *     Function: rgCOMHndlDelReq
 *     
 *     This API handles processing of delete request from RRC to MAC. 
 *     
 *     Processing Steps:
 *        - Fetch corresponding control block and pass it to CFG module.
 *        - If control block does not exist, FAIL.
 *
 *  @param[in]  Inst        inst
 *  @param[in]  CrgDel    *del
 *  @param[out] RgErrInfo *errInfo
    @param[out] Bool      *isCfmRqrd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgCOMHndlDelReq
(
Inst        inst,
CrgDel      *del,
RgErrInfo   *errInfo,
Bool        *isCfmRqrd,
CrgCfgTransId transId
)
{

   S16      ret;
   volatile uint32_t   startTime=0;

   errInfo->errType = RGERR_COM_DEL_REQ;
   
   /* Process the delete request */ 
   switch (del->delType)
   {
      case CRG_CELL_CFG:
      {
         ret = rgCFGCrgCellDel(inst,del, errInfo);
         break;
      }
      case CRG_UE_CFG:
      {
         /*starting Task*/ 
         SStartTask(&startTime,PID_MAC_UE_DEL);

         ret = rgCFGCrgUeDel(inst,del, errInfo);
         DU_LOG("\nINFO  -->  MAC : [%d] Delete UE Done \n", del->u.ueDel.crnti);

         /*stoping Task*/ 
         SStopTask(startTime,PID_MAC_UE_DEL);

         break;
      }
      case CRG_LCH_CFG:
      {
         ret = rgCFGCrgLcDel(inst,del, errInfo,isCfmRqrd, transId);
         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  MAC : Should never come here: delType %d",
                  del->delType);
         return RFAILED;
      }
   }

   return (ret);
}  /* rgCOMHndlDelReq */

#ifdef LTE_ADV
/**
 * @brief Handler for the SCell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : RgPrgPMacSMacUeSCellCfgReq
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  Pst          *pst
 *  @param[in]  RgPrgUeSCellCfgInfo *ueSCellCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgPrgPMacSMacUeSCellCfgReq
(
Pst         *pst,    
RgPrgUeSCellCfgInfo *ueSCellCb
)
{
   RgPrgCfgCfmInfo  cfgCfm;
   Inst             inst = pst->dstInst;
   RgCellCb         *cell = rgCb[inst].cell;
   S16              ret;
   Pst              cfmPst;    

   
   DU_LOG("\nDEBUG  -->  MAC : APPLYING CRG UE SCELL CONFIG: cellId %d ueId %d\n",
            ueSCellCb->cellId, ueSCellCb->ueId);

   cfgCfm.ueId = ueSCellCb->ueId;
   cfgCfm.sCellId = ueSCellCb->cellId;
   cfgCfm.status = PRG_CFG_CFM_OK;
   cfgCfm.event = EVTPRGUESCELLCFGCFM;
   rgGetPstToInst(&cfmPst, inst, pst->srcInst);

  ret = rgUtlVltdAddSCellCfg(ueSCellCb, cell, inst);
  if(ret != ROK)
  {
     DU_LOG("\nERROR  -->  MAC : [%d]Crg UE SCell failed:\
              cellId %d\n", ueSCellCb->ueId, ueSCellCb->cellId);
     /* Set status as Not OK*/
     cfgCfm.status = PRG_CFG_CFM_NOK;
  }
  else
  {
     ret = rgCfgAddUeSCellCfg(inst, ueSCellCb, cell);
     if(ret != ROK)
     {
        DU_LOG("\nERROR  -->  MAC : [%d]Crg UE SCell failed:\
                 cellId %d\n", ueSCellCb->ueId, ueSCellCb->cellId);
        /* Set status as Not OK*/
        cfgCfm.status = PRG_CFG_CFM_NOK;
     }
  }
  
  DU_LOG("\nINFO  -->  MAC : [%d]Crg UE SCell Config done:\
           cellId %d\n", ueSCellCb->ueId, ueSCellCb->cellId);

  /* Send positive confirmation to primary cell*/
  RgPrgSMacPMacCfg(&cfmPst, &cfgCfm);
  return ROK;
}  /* RgPrgPMacSMacUeSCellCfgReq */

/**
 * @brief Hander for config confim from sec MAC to Pri mac for Add Scell Cfg.
 *
 * @details
 *
 *     Function : RgPrgSMacPMacCfgCfm
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - If cfm event is lch recfg then send the confirmation to RRC for
 *        that event.
 *      - If cfm event is Scell addition then send the confirmation to RRC for
 *        the same.
 *         - Update UE control block with the values received in the
 *           configuration.
 *         - If successful, add the control block to hash list of UEs for the cell
 *           else Rollback and FAIL.
 *
 *  @param[in]  Inst        dstMacInst
 *  @param[in]  RgUrSCellCb  *ueSCellCb
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 RgPrgSMacPMacCfgCfm
(
Pst             *pst,    
RgPrgCfgCfmInfo *cfgCfm
)
{
   Inst      inst = pst->dstInst;
   RgCellCb *cell;
   RgUeCb   *ue;

   RG_IS_INST_VALID(inst);

   DU_LOG("\nINFO  -->  MAC : Config Confirm Rcvd from Inst %d ueId %d cellId %d\n",
            pst->srcInst, cfgCfm->ueId, cfgCfm->cellId);

   cell = rgCb[inst].cell;

   if ((ue = rgDBMGetUeCb(cell, cfgCfm->ueId)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UE does not exist\n", cfgCfm->ueId);
      return RFAILED;
   }
   switch(cfgCfm->event)
   {
      /* cfgCount increment for all cases */
      case EVTPRGUESCELLLCHMODCFM:
      case EVTPRGUESCELLLCHDELCFM:
      case EVTPRGUESCELLLCHADDCFM:
         {
            ue->cfgCfmInfo.cfgCfgCount++;
            ue->cfgCfmInfo.mask |= cfgCfm->status;
            if(ue->cfgCfmInfo.numSCells == ue->cfgCfmInfo.cfgCfgCount)
            {
               ue->cfgCfmInfo.cfgCfgCount = 0;
               /* Send back confirmation status to RRC */
               rgUIMCrgCfgCfm(inst, ue->cfgCfmInfo.transId, ue->cfgCfmInfo.mask);
               ue->cfgCfmInfo.mask = 0;
               DU_LOG("\nINFO  -->  MAC : CRG Configuration request processed\n");
            }
         }
         break;
      case EVTPRGUESCELLCFGCFM:
         {
            /*Commit Added SCell info to UeCb as we confirmation received */
            if(PRG_CFG_CFM_OK == cfgCfm->status)
            {
               ue->sCelInfo[pst->srcInst].isSCellAdded = TRUE;
               ue->sCelInfo[pst->srcInst].macInst = pst->srcInst;
               ue->sCelInfo[pst->srcInst].sCellId = cfgCfm->sCellId;
            }

            ue->cfgCfmInfo.cfgCfgCount++;
            ue->cfgCfmInfo.mask |= cfgCfm->status;
            if(ue->cfgCfmInfo.numSCells == ue->cfgCfmInfo.cfgCfgCount)
            {
               ue->cfgCfmInfo.cfgCfgCount = 0;
               /* Send back confirmation status to RRC */
               rgUIMCrgCfgCfm(inst, ue->cfgCfmInfo.transId, ue->cfgCfmInfo.mask);
               ue->cfgCfmInfo.mask = 0;
               DU_LOG("\nINFO  -->  MAC : CRG Configuration request processed\n");
            }
         }
         break;
      default:
         {
            DU_LOG("\nERROR  -->  MAC : Invalid configuration confirm event %d\n",
                     cfgCfm->event);

            return RFAILED;
         }

   }
   return ROK;
}  /* RgPrgSMacPMacCfgCfm */

/**
 * @brief Function for handling UE release for SCELL
 * triggered from Primary Cell
 *
 * @details
 *
 *     Function : RgPrgPMacSMacUeSCellDelReq
 *     
 *        - This Function should be invoked by PCell of UE
 *        - Remove the UE context from SCELL corresponding to rnti.
 *           
 *  @param[in] Pst                 *pst
 *  @param[in] RgPrgUeSCellDelInfo *ueSCellDelInfo
 *  @return  ROK is SUCCESS 
 **/
S16 RgPrgPMacSMacUeSCellDelReq
(
Pst                 *pst,
RgPrgUeSCellDelInfo *ueSCellDelInfo
)
{
   Inst        inst     = pst->dstInst - RG_INST_START;
   RgCellCb    *sCell   = rgCb[inst].cell;
   RgUeCb      *sCellUe = NULLP;

   /* Checking for cell Cb because in case of shutdownReq it is possible that
    * cell is already deleted for this cell*/
   if(sCell == NULLP)
   {
      return ROK;
   }
   /* Retrive the UeCb from sec cell*/
   if ((sCellUe = rgDBMGetUeCb(sCell, ueSCellDelInfo->ueId)) == NULLP)
   {
      DU_LOG("\nERROR  -->  MAC : [%d]UE:does not exist in sCell(%d)\n",
               ueSCellDelInfo->ueId, sCell->cellId);
      return RFAILED;
   }
   
   /*PMAC_Reest: ueId and newRnti is different that means its a UeId change
    *request from PMAC to SMAC during PCell reestablishment
    */
  if(ueSCellDelInfo->ueId != ueSCellDelInfo->newRnti)
  {
     /* Retrive the UeCb from sec cell*/
     if ((rgDBMGetUeCb(sCell, ueSCellDelInfo->newRnti)) != NULLP)
     {
        DU_LOG("\nERROR  -->  MAC : [%d]UE:UE context already exist in\
                 sCell(%d)",ueSCellDelInfo->newRnti, sCell->cellId);
        return RFAILED;
     }

     rgDBMDelUeCb(sCell, sCellUe);

     sCellUe->ueId = ueSCellDelInfo->newRnti;

     /* Reset harq procs*/
     rgDHMUeReset(sCell, &sCellUe->dl.hqEnt);

     rgDBMInsUeCb(sCell, sCellUe);
  }
  else
  {
     rgDBMDelUeCb(sCell, sCellUe);
     rgCFGFreeUeCb(sCell, sCellUe);
  }

   return ROK;
} /* RgPrgPMacSMacUeSCellDelReq */
#endif /*LTE_ADV */
/**********************************************************************
 
         End of file
**********************************************************************/
