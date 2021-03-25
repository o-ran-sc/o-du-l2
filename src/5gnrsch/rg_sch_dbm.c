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
  
     File:     rg_sch_dbm.c
  
**********************************************************************/

/** @file rg_sch_dbm.c
@brief This file contains the APIs exposed for the database handling of the scheduler.
*/
/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_err.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"
#include "rg_sch.x"
#include "rg_sch_cmn.x"


/* local defines */
static S16 rgSCHDbmInitUeCbLst ARGS(( RgSchCellCb *cellCb, uint16_t numBins));
#ifdef LTE_TDD
static S16 rgSCHDbmInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb, uint16_t numBins));
#endif
static Void rgSCHDbmInitDedLcLst ARGS((RgSchUeCb *ueCb));
static Void rgSCHDbmInitCmnLcLst ARGS((RgSchCellCb *cellCb));
#ifdef LTEMAC_SPS
static S16 rgSCHDbmInitSpsUeCbLst ARGS((RgSchCellCb *cellCb,
                      uint16_t numBins));
#endif
static Void rgSCHDbmInitRaCbLst ARGS(( RgSchCellCb *cellCb));
#ifndef LTE_TDD
static Void rgSCHDbmInitRaReqLst ARGS(( RgSchCellCb *cellCb));
#endif
static Void rgSCHDbmInitCrntRgrCfgLst ARGS(( RgSchCellCb *cellCb));
static Void rgSCHDbmInitPndngRgrCfgLst ARGS(( RgSchCellCb *cellCb));

#ifdef EMTC_ENABLE
 S16 rgSCHDbmPutEmtcRnti ARGS((RgSchCellCb *cellCb,RgSchRntiLnk *rntiLnk));
#endif

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler for Initializing the cell.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCell
 *     
 *   Initializes the lists belonging to the cell.
 *     
 *           
 *  @param[in]  RgSchCellCb *cellCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
S16 rgSCHDbmInitCell(RgSchCellCb *cellCb)
{
   S16 ret;
   
   /* Initialize ue list */
   if ((ret = rgSCHDbmInitUeCbLst(cellCb, RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      return (ret);
#ifdef LTE_TDD
   if ((ret = rgSCHDbmInitUeTfuPendLst(cellCb, 
                                       RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      return (ret);
#endif

#ifdef LTEMAC_SPS
   /* Initialize SPS Ue list */
   if ((ret = rgSCHDbmInitSpsUeCbLst(cellCb, RGSCH_MAX_UE_BIN_PER_CELL)) != ROK)
      return (ret);
#endif /* LTEMAC_SPS */

   /* Initialize BCCH/PCCH logical channels */
   rgSCHDbmInitCmnLcLst(cellCb);

   /* Initialize configuration lists */
   rgSCHDbmInitCrntRgrCfgLst(cellCb);
   rgSCHDbmInitPndngRgrCfgLst(cellCb);

#ifndef LTE_TDD
   /* Initialize raReq list */
   rgSCHDbmInitRaReqLst(cellCb);
#endif

   /* Initialize raCb list */
   rgSCHDbmInitRaCbLst(cellCb);

   /* Initialize l2mList */
#ifdef LTE_L2_MEAS 
   cmLListInit(&cellCb->l2mList);
#endif /* LTE_L2_MEAS */

   return (ret);

} /* rgSCHDbmInitCell */

/**
 * @brief Handler for initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHDbmInitUeCbLst(RgSchCellCb  *cellCb,uint16_t numBins)
{
   RgSchUeCellInfo ueCellInfo;

   /* Fix: syed It is better to compute offset dynamically
    * rather than hardcoding it as 0 */      
   return (cmHashListInit(&cellCb->ueLst, numBins, (uint16_t)((PTR)&(ueCellInfo.ueLstEnt) - (PTR)&ueCellInfo), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool));

}  /* rgSCHDbmInitUeCbLst */

/**
 * @brief Handler for de-initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDeInitUeCbLst(RgSchCellCb       *cellCb)
{

   return (cmHashListDeinit(&cellCb->ueLst));

}  /* rgSCHDbmDeInitUeCbLst */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHDbmInitSpsUeCbLst(RgSchCellCb *cellCb,uint16_t numBins)
{
   RgSchUeCb ue;

   return (cmHashListInit(&cellCb->spsUeLst, numBins, (uint16_t) ((PTR) &(ue.spsUeLstEnt) - (PTR) &ue), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool));

}  /* rgSCHDbmInitSpsUeCbLst */

/**
 * @brief Handler for de-initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDeInitSpsUeCbLst(RgSchCellCb  *cellCb)
{

   return (cmHashListDeinit(&cellCb->spsUeLst));

}  /* rgSCHDbmDeInitSpsUeCbLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for inserting the ueCb in to the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmInsUeCb(RgSchCellCb  *cellCb,RgSchUeCb  *ueCb)
{
   RgSchUeCellInfo *ueCellInfo = NULLP;

   ueCellInfo = ueCb->cellInfo[ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cellCb)]];

   return (cmHashListInsert(&cellCb->ueLst, (PTR)ueCellInfo, 
      (uint8_t *)&ueCb->ueId, (uint16_t)sizeof(ueCb->ueId)));

}  /* rgSCHDbmInsUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for inserting the ueCb in to the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsSpsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmInsSpsUeCb(RgSchCellCb *cellCb,RgSchUeCb *ueCb)
{
   return (cmHashListInsert(&cellCb->spsUeLst, (PTR)ueCb, 
      (uint8_t *)&ueCb->spsRnti, (uint16_t)sizeof(ueCb->spsRnti)));

}  /* end of rgSCHDbmInsSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for accessing the existing ueCb identified by the key ueId
 * in the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
RgSchUeCb* rgSCHDbmGetUeCb(RgSchCellCb *cellCb,CmLteRnti ueId)
{
   RgSchUeCellInfo *ueCellInfo = NULLP;

   cmHashListFind(&cellCb->ueLst, (uint8_t *)&ueId,
      sizeof(ueId), 0, (PTR *)&ueCellInfo);

   return (!ueCellInfo?NULLP:ueCellInfo->ue);
}  /* rgSCHDbmGetUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the existing ueCb identified by the key 
 * spsRnti in the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgSchUeCb*
 **/
RgSchUeCb* rgSCHDbmGetSpsUeCb(RgSchCellCb *cellCb, CmLteRnti  spsRnti)
{
   RgSchUeCb *ueCb = NULLP; 

   cmHashListFind(&cellCb->spsUeLst, (uint8_t *)&spsRnti,
      sizeof(spsRnti), 0, (PTR *)&ueCb);
   return (ueCb);
}  /* rgSCHDbmGetSpsUeCb */
#endif

/**
 * @brief Handler for accessing the existing next ueCb in the ueCbLst under the
 *  cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgSchUeCb*
 **/
RgSchUeCb* rgSCHDbmGetNextUeCb(RgSchCellCb *cellCb, RgSchUeCb *ueCb)
{
   RgSchUeCellInfo *ueCellInfo = NULLP;
   RgSchUeCellInfo *nextUeCellInfo = NULLP;

   if (ueCb)
   {
      ueCellInfo = ueCb->cellInfo[
         ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cellCb)]];
   }

   cmHashListGetNext(&cellCb->ueLst, (PTR) ueCellInfo, (PTR *)&nextUeCellInfo);
   return (!nextUeCellInfo?NULLP:nextUeCellInfo->ue);
}  /* rgSCHDbmGetNextUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the existing next ueCb stored in the spsUeCbLst
 *         using SPS-Rnti under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgSchUeCb*
 **/
RgSchUeCb* rgSCHDbmGetNextSpsUeCb(RgSchCellCb *cellCb, RgSchUeCb  *ueCb)
{
   RgSchUeCb *nextUeCb = NULLP; 

   cmHashListGetNext(&cellCb->spsUeLst, (PTR) ueCb, (PTR *)&nextUeCb);
   return (nextUeCb);
}  /* end of rgSCHDbmGetNextSpsUeCb */

#endif /* LTEMAC_SPS */

#ifdef LTE_L2_MEAS
/**
 * @brief Handler for Cleaning up L2 Meas related Data in
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelL2MUe
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDelL2MUe(RgSchCellCb *cellCb,RgSchUeCb *ueCb)
{
   uint8_t lcCnt = 0;
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);

   ueUl->hqEnt.numBusyHqProcs = 0;
   /* Clean cell level UE Active Count */
   for (lcCnt =0; lcCnt < RGSCH_MAX_LC_PER_UE; lcCnt++)
   {
      if (ueCb->ul.lcCb[lcCnt].isValid)
      {
         if((ueCb->ul.lcCb[lcCnt].qciCb->ulUeCount) &&
               (ueCb->ulActiveLCs &
                (1 << (ueCb->ul.lcCb[lcCnt].qciCb->qci -1))))
         {
            ueCb->ul.lcCb[lcCnt].qciCb->ulUeCount--;
            ueCb->ulActiveLCs &= ~(1 << 
                  (ueCb->ul.lcCb[lcCnt].qciCb->qci -1));
         }
      }

      if (ueCb->dl.lcCb[lcCnt])
      {
         if (ueCb->qciActiveLCs[ueCb->dl.lcCb[lcCnt]->qciCb->qci])
         {
            ueCb->dl.lcCb[lcCnt]->qciCb->dlUeCount--;
            ueCb->qciActiveLCs[ueCb->dl.lcCb[lcCnt]->qciCb->qci] = 0;
         }
      }
   }

   return ROK;
}  /* rgSCHDbmDelL2MUe */

#endif

/**
 * @brief Handler for deleting the existing ueCb from the ueCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDelUeCb(RgSchCellCb *cellCb,RgSchUeCb  *ueCb)
{
   RgSchUeCellInfo *ueCellInfo = NULLP;

   ueCellInfo = ueCb->cellInfo[ueCb->cellIdToCellIdxMap[RG_SCH_CELLINDEX(cellCb)]];

   return (cmHashListDelete(&cellCb->ueLst, (PTR)ueCellInfo));
}  /* rgSCHDbmDelUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for deleting the existing ueCb from the spsUeCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDelSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDelSpsUeCb(RgSchCellCb *cellCb,RgSchUeCb *ueCb)
{

   return (cmHashListDelete(&cellCb->spsUeLst, (PTR)ueCb));
}  /* end of rgSCHDbmDelSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for Initializing the UE.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUe
 *     
 *   Initializes the lists belonging to the UE.
 *     
 *           
 *  @param[in]  RgSchUeCb  *ueCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
S16 rgSCHDbmInitUe(RgSchUeCb  *ueCb)
{
   S16 ret = ROK;
   
   /* Initialize Dedicated logical channels */
   rgSCHDbmInitDedLcLst(ueCb);

   return (ret);
} /* rgSCHDbmInitUe */

/**
 * @brief Handler for Initializing the dedicated logical channels.
 *
 * @details
 *
 *     Function : rgSCHDbmInitDedLcLst
 *     
 *   Initializes dedicated logical channels.
 *           
 *  @param[in]  RgSchUeCb *ueCb
 *  @return     Void 
 **/
static Void rgSCHDbmInitDedLcLst(RgSchUeCb *ueCb)
{
   uint8_t idx;
   
   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      /* Set Dedicated LCs as not configured */
      ueCb->ul.lcCb[idx].isValid = FALSE;
      ueCb->dl.lcCb[idx] = NULLP;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;
  

} /* rgSCHDbmInitDedLcLst */

/**
 * @brief Handler for Initializing the common logical channel list of the cell.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCmnLcLst
 *     
 *   Initializes following common logical channels belonging to the cell.
 *     - BCCH on BCH
 *     - BCCH on DLSCH
 *     - PCCH
 *           
 *  @param[in]  RgSchCellCb *cellCb
 *  @return  Void
 **/
static Void rgSCHDbmInitCmnLcLst(RgSchCellCb *cellCb)
{
   uint8_t idx;
   
   for (idx = 0; idx < RGSCH_MAX_CMN_LC_CB; idx++)
   {
      cellCb->cmnLcCb[idx].lcId = RGSCH_INVALID_LC_ID;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

} /* rgSCHDbmInitCmnLcLst */

/**
 * @brief Handler for inserting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgSCHDbmInsDlDedLcCb
 *     
 *  @param[in]  RgSchUeCb *ueCb
 *  @param[in]  RgSchDlLcCb* dlLcCb
 *  @return  Void
 **/
Void rgSCHDbmInsDlDedLcCb(RgSchUeCb *ueCb, RgSchDlLcCb *dlLcCb )
{

   ueCb->dl.lcCb[dlLcCb->lcId - 1] = dlLcCb;

}  /* rgSCHDbmInsDlDedLcCb */

/**
 * @brief Handler for deleting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgSCHDbmDelDlDedLcCb
 *     
 *  @param[in]  RgSchUeCb *ueCb
 *  @param[in]  RgSchDlLcCb* dlLcCb
 *  @return  Void
 **/
Void rgSCHDbmDelDlDedLcCb(RgSchUeCb *ueCb, RgSchDlLcCb  *dlLcCb )
{

#ifdef LTE_L2_MEAS
   /* Clean cell level UE Active Count */

   if (ueCb->dl.lcCb[dlLcCb->lcId - 1])
   {
      if ((dlLcCb->qciCb)
            && (ueCb->qciActiveLCs[dlLcCb->qciCb->qci]))
      {
         ueCb->qciActiveLCs[dlLcCb->qciCb->qci]--;
         if (!(ueCb->qciActiveLCs[dlLcCb->qciCb->qci]))
         {
            dlLcCb->qciCb->dlUeCount--;
         }
      }
   }
#endif /* LTE_L2_MEAS */

   ueCb->dl.lcCb[dlLcCb->lcId - 1] = NULLP;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

}  /* rgSCHDbmDelDlDedLcCb */

/**
 * @brief Handler for accessing the existing dl dedicated lcCb at idx in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetDlDedLcCb
 *     
 *  @param[in]  *ueCb
 *  @param[in]  idx
 *  @return  RgSchDlLcCb*
 **/
RgSchDlLcCb* rgSCHDbmGetDlDedLcCb(RgSchUeCb *ueCb, CmLteLcId idx)
{
   if (idx < RGSCH_DEDLC_MIN_LCID || idx > RGSCH_DEDLC_MAX_LCID)
   {
      return (NULLP);
   }
   return (ueCb->dl.lcCb[idx-1]);

}  /* rgSCHDbmGetDlDedLcCb */

/**
 * @brief Handler for accessing the existing first dl dedicated lcCb at idx 
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetFirstDlDedLcCb
 *     
 *           
 *  @param[in]  *ueCb
 *  @return  RgSchDlLcCb*
 **/
RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb(RgSchUeCb  *ueCb)
{
   uint8_t idx;
   
   for(idx = 0; idx < RGSCH_DEDLC_MAX_LCID; idx++)
   {
      if(ueCb->dl.lcCb[idx])
      {
         return (ueCb->dl.lcCb[idx]);
      }
   }
   return (NULLP);
}  /* rgSCHDbmGetFirstDlDedLcCb */
/**
 * @brief Handler for accessing the existing next dl dedicated lcCb at idx 
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextDlDedLcCb
 *     
 *           
 *  @param[in]  *ueCb
 *  @param[in]  *lcCb
 *  @return  RgSchDlLcCb*
 **/
RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb(RgSchUeCb *ueCb,RgSchDlLcCb *lcCb)
{
   uint8_t idx;

   if (!lcCb)
   {
      return (rgSCHDbmGetFirstDlDedLcCb(ueCb));
   }

   for(idx = lcCb->lcId; idx < RGSCH_DEDLC_MAX_LCID; idx++)
   {
      if(ueCb->dl.lcCb[idx])
      {
         return (ueCb->dl.lcCb[idx]);
      }
   }
   return (NULLP);
}  /* rgSCHDbmGetNextDlDedLcCb */

/**
 * @brief Handler for accessing the existing dl common lcCb identified by the key lcId
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetCmnLcCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  lcId
 *  @return  RgSchClcDlLcCb*
 **/
RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb(RgSchCellCb *cellCb, CmLteLcId lcId)
{
   uint8_t idx;

   for(idx = 0; idx < RGSCH_MAX_CMN_LC_CB; idx++)
   {
      if(cellCb->cmnLcCb[idx].lcId == lcId)
      {
         return (&(cellCb->cmnLcCb[idx]));
      }
   }
   return (NULLP);
}  /* rgSCHDbmGetCmnLcCb */

/**
 * @brief Handler for accessing the existing BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch(RgSchCellCb *cellCb )
{
 
   if(cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].lcId != RGSCH_INVALID_LC_ID)
   {
      return (&(cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX]));
   }
   return (NULLP);
}  /* rgSCHDbmGetBcchOnBch */

/**
 * @brief Handler for accessing the existing BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetFirstBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch(RgSchCellCb *cellCb)
{

   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId != RGSCH_INVALID_LC_ID)
   {
      return (&(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1]));
   }
   return (NULLP);
}  /* rgSCHDbmGetFirstBcchOnDlsch */

/**
 * @brief Handler for accessing the existing BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetSecondBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch(RgSchCellCb  *cellCb)
{

   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId != RGSCH_INVALID_LC_ID)
   {
      return (&(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2]));
   }
   return (NULLP);
}  /* rgSCHDbmGetSecondBcchOnDlsch */

/**
 * @brief Handler for accessing the existing PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmGetPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchClcDlLcCb*
 **/
RgSchClcDlLcCb* rgSCHDbmGetPcch(RgSchCellCb *cellCb)
{
 
   if(cellCb->cmnLcCb[RGSCH_PCCH_IDX].lcId != RGSCH_INVALID_LC_ID)
   {
      return (&(cellCb->cmnLcCb[RGSCH_PCCH_IDX]));
   }
   return (NULLP);
}  /* rgSCHDbmGetPcch */

/**
 * @brief Handler for inserting the BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgSCHDbmInsBcchOnBch(RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb)
{

   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

}  /* rgSCHDbmInsBcchOnBch */

/**
 * @brief Handler for inserting the BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgSCHDbmInsBcchOnDlsch(RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb)
{
   if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId == RGSCH_INVALID_LC_ID)   
   { 
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].lcId = cmnDlLcCb->lcId;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].boLst = cmnDlLcCb->boLst;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX1].si = FALSE;
   }
   else if(cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId == RGSCH_INVALID_LC_ID)
   { 
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].lcId = cmnDlLcCb->lcId;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].boLst = cmnDlLcCb->boLst;
      cellCb->cmnLcCb[RGSCH_BCCH_DLSCH_IDX2].si = TRUE;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

}  /* rgSCHDbmInsBcchOnDlsch */


/**
 * @brief Handler for inserting the PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInsPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgSCHDbmInsPcch(RgSchCellCb *cellCb, RgSchClcDlLcCb *cmnDlLcCb)
{

   cellCb->cmnLcCb[RGSCH_PCCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_PCCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

}  /* rgSCHDbmInsPcch */

/**
 * @brief Handler for initializing the boLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCmnLcBoLst
 *     
 *           
 *  @param[in] *cmnDlLcCb 
 *  @return  Void
 **/
Void rgSCHDbmInitCmnLcBoLst(RgSchClcDlLcCb *cmnDlLcCb)
{

   cmLListInit(&cmnDlLcCb->boLst);
   return;
}  /* rgSCHDbmInitCmnLcBoLst */

/**
 * @brief Handler for inserting the bo report in to the boLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsCmnLcBoRpt
 *     
 *           
 *  @param[in] *cmnDlLcCb 
 *  @param[in] *boRpt 
 *  @return  Void
 **/
Void rgSCHDbmInsCmnLcBoRpt(RgSchClcDlLcCb *cmnDlLcCb,RgSchClcBoRpt *cmnBoRpt)
{
   cmnBoRpt->boLstEnt.next = NULLP;
   cmnBoRpt->boLstEnt.prev = NULLP;
   cmnBoRpt->boLstEnt.node = (PTR)cmnBoRpt;
   cmLListAdd2Tail(&cmnDlLcCb->boLst, &cmnBoRpt->boLstEnt);
   return;
}  /* rgSCHDbmInsCmnLcBoRpt */


/**
 * @brief Handler for initializing the raCbLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitRaCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
static Void rgSCHDbmInitRaCbLst(RgSchCellCb  *cellCb)
{

   cmLListInit(&cellCb->raInfo.raCbLst);
   return;
}  /* rgSCHDbmInitRaCbLst */


/**
 * @brief Handler for accessing the existing raCb in the raCbLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetRaCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  key
 *  @return  RgSchRaCb*
 **/
RgSchRaCb* rgSCHDbmGetRaCb(RgSchCellCb *cellCb, CmLteRnti key)
{
   CmLList *tmpNode;

   CM_LLIST_FIRST_NODE(&cellCb->raInfo.raCbLst,tmpNode);
   while(tmpNode)
   {
      if(((RgSchRaCb *)tmpNode->node)->tmpCrnti == key)
      {
         return ((RgSchRaCb *)(tmpNode->node));
      }
      CM_LLIST_NEXT_NODE(&cellCb->raInfo.raCbLst,tmpNode);
   }
   return (NULLP);
}  /* rgSCHDbmGetRaCb */

#ifndef LTE_TDD
/**
 * @brief Handler for initializing the raReqLst.
 g
 * @details
 *
 *     Function : rgSCHDbmInitRaReqLst
 *     
 *           
 *  @param[in]  *cellCb 
 *  @return  Void
 **/
static Void rgSCHDbmInitRaReqLst(RgSchCellCb *cellCb)
{
   uint8_t idx;

   /* ccpu00133557- Memory Leak Fix- initializing for the all nodes 
    * in RAREQ list*/
   for(idx = 0; idx < RGSCH_RAREQ_ARRAY_SIZE; idx++)
   {
      cmLListInit(&cellCb->raInfo.raReqLst[idx]);
   }
   return;
}  /* rgSCHDbmInitRaReqLst */
#endif

/**
 * @brief Handler for initializing the crnt rgr cfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitCrntRgrCfgLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
static Void rgSCHDbmInitCrntRgrCfgLst(RgSchCellCb *cellCb)
{

   cmLListInit(&cellCb->rgCfgInfo.crntRgrCfgLst);
   return;
}  /* rgSCHDbmInitCrntRgrCfgLst */

/**
 * @brief Handler for initializing the pndng rgr cfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInitPndngRgrCfgLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
static Void rgSCHDbmInitPndngRgrCfgLst(RgSchCellCb  *cellCb)
{

   cmLListInit(&cellCb->rgCfgInfo.pndngRgrCfgLst);
   return;
}  /* rgSCHDbmInitPndngRgrCfgLst */

/**
 * @brief Handler for inserting the cfgElem in to the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsCrntRgrCfgElem
 *           
 *
 *  @param[in] *cellCb 
 *  @param[in] *cfgElem 
 *  @return  Void
 **/
Void rgSCHDbmInsCrntRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem *cfgElem)
{
   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.crntRgrCfgLst, &cfgElem->cfgReqLstEnt);
   return;
}  /* rgSCHDbmInsCrntRgrCfgElem */

/**
 * @brief Handler for inserting the cfgElem in to the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmInsPndngRgrCfgElem
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *cfgElem 
 *  @return  Void
 **/
Void rgSCHDbmInsPndngRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem  *cfgElem)
{

   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cfgElem->cfgReqLstEnt.node = (PTR)cfgElem;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.pndngRgrCfgLst, &cfgElem->cfgReqLstEnt);
   return;
}  /* rgSCHDbmInsPndngRgrCfgElem */

/**
 * @brief Handler for accessing the existing cfgElem in the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextCrntRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *cfgElem
 *  @return  RgSchCfgElem*
 **/
RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem *cfgElem)
{

   if(!cfgElem)
   {
      return ( cellCb->rgCfgInfo.crntRgrCfgLst.first ? 
               (RgSchCfgElem *)(cellCb->rgCfgInfo.crntRgrCfgLst.first->node) : NULLP );
   }
   return ( cfgElem->cfgReqLstEnt.next ? 
               (RgSchCfgElem *)(cfgElem->cfgReqLstEnt.next->node) : NULLP );
}  /* rgSCHDbmGetNextCrntRgrCfgElem */

/**
 * @brief Handler for accessing the existing cfgElem in the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetNextPndngRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *cfgElem
 *  @return  RgSchCfgElem*
 **/
RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem *cfgElem)
{

   if(!cfgElem)
   {
      return ( cellCb->rgCfgInfo.pndngRgrCfgLst.first ? 
               (RgSchCfgElem *)(cellCb->rgCfgInfo.pndngRgrCfgLst.first->node) : NULLP );
   }
   return ( cfgElem->cfgReqLstEnt.next ? 
               (RgSchCfgElem *)(cfgElem->cfgReqLstEnt.next->node) : NULLP );
}  /* rgSCHDbmGetNextPndngRgrCfgElem */

/**
 * @brief Handler for extracting the existing cfgElem from the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmGetPndngRgrCfgElemByKey
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  key 
 *  @return  RgSchCfgElem*
 **/
RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey(RgSchCellCb *cellCb,CmLteTimingInfo  key)
{
   CmLList  *tmpNode;

   CM_LLIST_FIRST_NODE(&cellCb->rgCfgInfo.pndngRgrCfgLst,tmpNode);
   while(tmpNode)
   {
      if((((RgSchCfgElem *)tmpNode->node)->actvTime.sfn == key.sfn) &&
            (((RgSchCfgElem *)tmpNode->node)->actvTime.slot == key.slot))
      {
         return ((RgSchCfgElem *)(tmpNode->node));
      }
      CM_LLIST_NEXT_NODE(&cellCb->rgCfgInfo.pndngRgrCfgLst,tmpNode);
   }
   return (NULLP);
}  /* rgSCHDbmGetPndngRgrCfgElemByKey */

/**
 * @brief Handler for deleting the existing cfgElem from the crntRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmDelCrntRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *cfgElem 
 *  @return  RgSchCfgElem*
 **/
RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem *cfgElem)
{

   if(cmLListDelFrm(&cellCb->rgCfgInfo.crntRgrCfgLst,&cfgElem->cfgReqLstEnt))
   {
      return ((RgSchCfgElem *)(cfgElem->cfgReqLstEnt.node));
   }
   return (NULLP);
}  /* rgSCHDbmDelCrntRgrCfgElem */

/**
 * @brief Handler for deleting the existing cfgElem from the pndngRgrCfgLst.
 *
 * @details
 *
 *     Function : rgSCHDbmDelPndngRgrCfgElem
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *cfgElem 
 *  @return  RgSchCfgElem*
 **/
RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem(RgSchCellCb *cellCb,RgSchCfgElem *cfgElem)
{
   if(cmLListDelFrm(&cellCb->rgCfgInfo.pndngRgrCfgLst,&cfgElem->cfgReqLstEnt))
   {
      return ((RgSchCfgElem *)(cfgElem->cfgReqLstEnt.node));
   }
   return (NULLP);
}  /* rgSCHDbmDelPndngRgrCfgElem */

/**
 * @brief Handler for initializing the rntiDb.
 *
 * @details
 *
 *     Function : rgSCHDbmRntiDbInit
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  rntiStart
 *  @param[in]  maxRntis
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
S16 rgSCHDbmRntiDbInit(RgSchCellCb *cellCb,uint16_t rntiStart,uint16_t  maxRntis)
{
   uint16_t rnti;
   RgSchRntiLnk   *rntiPool;

   /* Fix for Change Request ccpu00099150 */
   if(rgSCHUtlAllocSBuf(cellCb->instIdx, 
       (Data **)&cellCb->rntiDb.rntiPool,maxRntis*sizeof(RgSchRntiLnk)) != ROK)
   {
      return RFAILED;
   }
   cellCb->rntiDb.rntiStart = rntiStart;
   cellCb->rntiDb.maxRntis = maxRntis;

   cellCb->rntiDb.count = maxRntis;

   rnti = rntiStart;
   rntiPool = cellCb->rntiDb.rntiPool;
   if (maxRntis == 1)
   {
      rntiPool[0].rnti = rnti;
      rntiPool[0].prv = NULLP;
      rntiPool[0].nxt = NULLP;
      cellCb->rntiDb.lastRnti = &rntiPool[0];
   }
   else
   {
      uint16_t idx;
      rntiPool[0].rnti = rnti++;
      rntiPool[0].prv = NULLP;
      rntiPool[0].nxt = &rntiPool[1];
      for (idx = 1; idx < maxRntis - 1; ++idx)
      {
         rntiPool[idx].rnti = rnti++;
         rntiPool[idx].prv = &rntiPool[idx-1];
         rntiPool[idx].nxt = &rntiPool[idx+1];
      }
      rntiPool[idx].rnti = rnti++;
      rntiPool[idx].prv = &rntiPool[idx-1];
      rntiPool[idx].nxt = NULLP;
      cellCb->rntiDb.lastRnti = &rntiPool[idx];
   }
   cellCb->rntiDb.freeRnti = &rntiPool[0];
   return ROK;
} /* rgSCHDbmRntiDbInit */

/**
 * @brief Handler for de-initializing the rntiDb.
 *
 * @details
 *
 *     Function : rgSCHDbmRntiDbDeInit
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgSCHDbmRntiDbDeInit(RgSchCellCb *cellCb)
{
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cellCb->instIdx, (Data **)(&(cellCb->rntiDb.rntiPool)),
                 cellCb->rntiDb.maxRntis*sizeof(RgSchRntiLnk));
   cellCb->rntiDb.maxRntis = 0;
   cellCb->rntiDb.freeRnti = NULLP;
   cellCb->rntiDb.lastRnti = NULLP;
   cmLListInit(&cellCb->rntiDb.rntiGuardPool);

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

} /* rgSCHDbmRntiDbDeInit */

/**
 * @brief Handler for accessing the free RNTI.
 *
 * @details
 *
 *     Function : rgSCHDbmGetRnti
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgSchRntiLnk*
 **/
RgSchRntiLnk* rgSCHDbmGetRnti(RgSchCellCb *cellCb)
{
   RgSchRntiLnk *rntiLnk;

   if (!(cellCb->rntiDb.freeRnti))
   {
      DU_LOG("\nERROR  -->  SCH : RNTI exhausted count:%d",
                cellCb->rntiDb.count);
      return (NULLP);
   }

   rntiLnk = cellCb->rntiDb.freeRnti;
   cellCb->rntiDb.freeRnti = rntiLnk->nxt;

   /* setting prv and nxt to NULLP may not be needed */
   rntiLnk->prv = NULLP;
   rntiLnk->nxt = NULLP;

   if (cellCb->rntiDb.freeRnti != NULLP)
   {
      cellCb->rntiDb.freeRnti->prv = NULLP;
   }
   else
   {
      cellCb->rntiDb.lastRnti = NULLP;
   }

   cellCb->rntiDb.count--;

   DU_LOG("\nINFO  -->  SCH : rgSCHDbmGetRnti::rntiLnk->rnti %u\n",rntiLnk->rnti);
   return (rntiLnk);
} /* rgSCHDbmGetRnti */

/**
 * @brief Handler for releasing the RNTI.
 *
 * @details
 *
 *     Function : rgSCHDbmRlsRnti
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  rntiLnk
 *  @return  Void
 **/
Void rgSCHDbmRlsRnti(RgSchCellCb *cellCb,RgSchRntiLnk *rntiLnk)
{
#ifdef EMTC_ENABLE
      if(ROK==rgSCHDbmPutEmtcRnti(cellCb,rntiLnk))
{      
return;
}
#endif
   rntiLnk->nxt = NULLP;
   if (cellCb->rntiDb.lastRnti)
   {
      cellCb->rntiDb.lastRnti->nxt = rntiLnk;
      rntiLnk->prv = cellCb->rntiDb.lastRnti;
   }
   else
   {
      rntiLnk->prv = NULLP;
      cellCb->rntiDb.freeRnti = rntiLnk;
   }
   cellCb->rntiDb.lastRnti = rntiLnk;

   cellCb->rntiDb.count++;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   return;

} /* rgSCHDbmRlsRnti */

#ifdef LTE_TDD
/**
 * @brief Handler for initializing the ueTfuPendLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmInitUeTfuPendLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgSCHDbmInitUeTfuPendLst(RgSchCellCb    *cellCb,uint16_t  numBins)
{
   RgSchUePucchRecpInfo pucchInfo;

   /* Fix: syed It is better to compute offset dynamically
    * rather than hardcoding it as 0 */      
   if(cmHashListInit(&cellCb->ueTfuPendLst, numBins, (uint16_t)((PTR)&(pucchInfo.hashLstEnt) - (PTR)&pucchInfo), FALSE, 
               CM_HASH_KEYTYPE_CONID,
               rgSchCb[cellCb->instIdx].rgSchInit.region, 
               rgSchCb[cellCb->instIdx].rgSchInit.pool) != ROK)
   {
      return RFAILED;
   }

   return ROK;

}  /* rgSCHDbmInitUeTfuPendLst */

/**
 * @brief Handler for de-initializing the ueTfuPendLst under the cellCb.
 *
 * @details
 *
 *     Function : rgSCHDbmDeInitUeTfuPendLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgSCHDbmDeInitUeTfuPendLst(RgSchCellCb *cellCb)
{

   cmHashListDeinit(&cellCb->ueTfuPendLst);
   
   return ROK;
}  /* rgSCHDbmDeInitUeTfuPendLst */
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
