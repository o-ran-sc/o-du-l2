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
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=230;
static int RLOG_MODULE_ID=4096;
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
#include "rl_interface.h"
#include "rl_common.h"


/* local defines */
PRIVATE S16 rgSCHDbmInitUeCbLst ARGS(( RgSchCellCb *cellCb, uint16_t numBins));
#ifdef LTE_TDD
PRIVATE S16 rgSCHDbmInitUeTfuPendLst ARGS(( RgSchCellCb *cellCb, uint16_t numBins));
#endif
PRIVATE Void rgSCHDbmInitDedLcLst ARGS((RgSchUeCb *ueCb));
PRIVATE Void rgSCHDbmInitCmnLcLst ARGS((RgSchCellCb *cellCb));
#ifdef LTEMAC_SPS
PRIVATE S16 rgSCHDbmInitSpsUeCbLst ARGS((RgSchCellCb *cellCb,
                      uint16_t numBins));
#endif
PRIVATE Void rgSCHDbmInitRaCbLst ARGS(( RgSchCellCb *cellCb));
#ifndef LTE_TDD
PRIVATE Void rgSCHDbmInitRaReqLst ARGS(( RgSchCellCb *cellCb));
#endif
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst ARGS(( RgSchCellCb *cellCb));
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst ARGS(( RgSchCellCb *cellCb));

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
#ifdef ANSI
S16 rgSCHDbmInitCell
(
RgSchCellCb       *cellCb
)
#else
S16 rgSCHDbmInitCell(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   S16 ret;
   
   TRC2(rgSCHDbmInitCell);

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
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitUeCbLst
(
RgSchCellCb    *cellCb,
uint16_t            numBins
)
#else
PRIVATE S16 rgSCHDbmInitUeCbLst(cellCb, numBins)
RgSchCellCb    *cellCb;
uint16_t            numBins;
#endif
{
   RgSchUeCellInfo ueCellInfo;
   TRC2(rgSCHDbmInitUeCbLst)

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
#ifdef ANSI
S16 rgSCHDbmDeInitUeCbLst
(
RgSchCellCb       *cellCb
)
#else
S16 rgSCHDbmDeInitUeCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitUeCbLst)

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
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitSpsUeCbLst
(
RgSchCellCb       *cellCb,
uint16_t               numBins
)
#else
PRIVATE S16 rgSCHDbmInitSpsUeCbLst(cellCb, numBins)
RgSchCellCb       *cellCb;
uint16_t               numBins;
#endif
{
   RgSchUeCb ue;
   TRC2(rgSCHDbmInitSpsUeCbLst)

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
#ifdef ANSI
S16 rgSCHDbmDeInitSpsUeCbLst
(
RgSchCellCb       *cellCb
)
#else
S16 rgSCHDbmDeInitSpsUeCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitSpsUeCbLst)

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
#ifdef ANSI
S16 rgSCHDbmInsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
S16 rgSCHDbmInsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCellInfo *ueCellInfo = NULLP;
   TRC2(rgSCHDbmInsUeCb)

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
#ifdef ANSI
S16 rgSCHDbmInsSpsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
S16 rgSCHDbmInsSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmInsSpsUeCb)

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
#ifdef ANSI
RgSchUeCb* rgSCHDbmGetUeCb
(
RgSchCellCb       *cellCb, 
CmLteRnti      ueId
)
#else
RgSchUeCb* rgSCHDbmGetUeCb(cellCb, ueId)
RgSchCellCb       *cellCb;
CmLteRnti      ueId;
#endif
{
   RgSchUeCellInfo *ueCellInfo = NULLP;

   TRC2(rgSCHDbmGetUeCb)

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
#ifdef ANSI
RgSchUeCb* rgSCHDbmGetSpsUeCb
(
RgSchCellCb       *cellCb, 
CmLteRnti         spsRnti
)
#else
RgSchUeCb* rgSCHDbmGetSpsUeCb(cellCb, spsRnti)
RgSchCellCb       *cellCb;
CmLteRnti         spsRnti;
#endif
{
   RgSchUeCb *ueCb = NULLP; 

   TRC2(rgSCHDbmGetSpsUeCb)

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
#ifdef ANSI
RgSchUeCb* rgSCHDbmGetNextUeCb
(
RgSchCellCb       *cellCb, 
RgSchUeCb         *ueCb
)
#else
RgSchUeCb* rgSCHDbmGetNextUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCellInfo *ueCellInfo = NULLP;
   RgSchUeCellInfo *nextUeCellInfo = NULLP;

   TRC2(rgSCHDbmGetNextUeCb)

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
#ifdef ANSI
RgSchUeCb* rgSCHDbmGetNextSpsUeCb
(
RgSchCellCb       *cellCb, 
RgSchUeCb         *ueCb
)
#else
RgSchUeCb* rgSCHDbmGetNextSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCb *nextUeCb = NULLP; 

   TRC2(rgSCHDbmGetNextSpsUeCb)

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
#ifdef ANSI
S16 rgSCHDbmDelL2MUe
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
S16 rgSCHDbmDelL2MUe(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   uint8_t lcCnt = 0;
   RgSchCmnUlUe *ueUl = RG_SCH_CMN_GET_UL_UE(ueCb, cellCb);

   TRC2(rgSCHDbmDelL2MUe)

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
#ifdef ANSI
S16 rgSCHDbmDelUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
S16 rgSCHDbmDelUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   RgSchUeCellInfo *ueCellInfo = NULLP;
   TRC2(rgSCHDbmDelUeCb)

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
#ifdef ANSI
S16 rgSCHDbmDelSpsUeCb
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb
)
#else
S16 rgSCHDbmDelSpsUeCb(cellCb, ueCb)
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHDbmDelSpsUeCb)

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
#ifdef ANSI
S16 rgSCHDbmInitUe
(
RgSchUeCb       *ueCb
)
#else
S16 rgSCHDbmInitUe(ueCb)
RgSchUeCb       *ueCb;
#endif
{
   S16 ret = ROK;
   
   TRC2(rgSCHDbmInitUe);

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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitDedLcLst
(
RgSchUeCb       *ueCb
)
#else
PRIVATE Void rgSCHDbmInitDedLcLst(ueCb)
RgSchUeCb       *ueCb;
#endif
{
   uint8_t idx;
   
   TRC2(rgSCHDbmInitDedLcLst);

   for (idx = 0; idx < RGSCH_MAX_LC_PER_UE; ++idx)
   {
      /* Set Dedicated LCs as not configured */
      ueCb->ul.lcCb[idx].isValid = FALSE;
      ueCb->dl.lcCb[idx] = NULLP;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;
  

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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitCmnLcLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitCmnLcLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   uint8_t idx;
   
   TRC2(rgSCHDbmInitCmnLcLst);

   for (idx = 0; idx < RGSCH_MAX_CMN_LC_CB; idx++)
   {
      cellCb->cmnLcCb[idx].lcId = RGSCH_INVALID_LC_ID;
   }

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

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
#ifdef ANSI
Void rgSCHDbmInsDlDedLcCb
(
RgSchUeCb         *ueCb, 
RgSchDlLcCb       *dlLcCb 
)
#else
Void rgSCHDbmInsDlDedLcCb(ueCb, dlLcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *dlLcCb; 
#endif
{
   TRC2(rgSCHDbmInsDlDedLcCb);

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
#ifdef ANSI
Void rgSCHDbmDelDlDedLcCb
(
RgSchUeCb         *ueCb, 
RgSchDlLcCb       *dlLcCb 
)
#else
Void rgSCHDbmDelDlDedLcCb(ueCb, dlLcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *dlLcCb; 
#endif
{
   TRC2(rgSCHDbmDelDlDedLcCb);

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
   RETVOID;

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
#ifdef ANSI
RgSchDlLcCb* rgSCHDbmGetDlDedLcCb
(
RgSchUeCb         *ueCb, 
CmLteLcId        idx
)
#else
RgSchDlLcCb* rgSCHDbmGetDlDedLcCb(ueCb, idx)
RgSchUeCb         *ueCb; 
CmLteLcId        idx;
#endif
{
   TRC2(rgSCHDbmGetDlDedLcCb);

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
#ifdef ANSI
RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb
(
RgSchUeCb         *ueCb
)
#else
RgSchDlLcCb* rgSCHDbmGetFirstDlDedLcCb(ueCb)
RgSchUeCb         *ueCb; 
#endif
{
   uint8_t idx;
   TRC2(rgSCHDbmGetFirstDlDedLcCb)
   
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
#ifdef ANSI
RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb
(
RgSchUeCb         *ueCb,
RgSchDlLcCb       *lcCb
)
#else
RgSchDlLcCb* rgSCHDbmGetNextDlDedLcCb(ueCb, lcCb)
RgSchUeCb         *ueCb; 
RgSchDlLcCb       *lcCb;
#endif
{
   uint8_t idx;
   TRC2(rgSCHDbmGetNextDlDedLcCb);

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
#ifdef ANSI
RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb
(
RgSchCellCb       *cellCb, 
CmLteLcId         lcId
)
#else
RgSchClcDlLcCb* rgSCHDbmGetCmnLcCb(cellCb, lcId)
RgSchCellCb       *cellCb;
CmLteLcId         lcId;
#endif
{
   uint8_t idx;

   TRC2(rgSCHDbmGetCmnLcCb)

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
#ifdef ANSI
RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch
(
RgSchCellCb       *cellCb 
)
#else
RgSchClcDlLcCb* rgSCHDbmGetBcchOnBch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetBcchOnBch)
 
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
#ifdef ANSI
RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch
(
RgSchCellCb       *cellCb
)
#else
RgSchClcDlLcCb* rgSCHDbmGetFirstBcchOnDlsch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetFirstBcchOnDlsch)

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
#ifdef ANSI
RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch
(
RgSchCellCb       *cellCb
)
#else
RgSchClcDlLcCb* rgSCHDbmGetSecondBcchOnDlsch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetSecondBcchOnDlsch)

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
#ifdef ANSI
RgSchClcDlLcCb* rgSCHDbmGetPcch
(
RgSchCellCb       *cellCb
)
#else
RgSchClcDlLcCb* rgSCHDbmGetPcch(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmGetPcch)
 
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
#ifdef ANSI
Void rgSCHDbmInsBcchOnBch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
Void rgSCHDbmInsBcchOnBch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsBcchOnBch)

   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_BCCH_BCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

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
#ifdef ANSI
Void rgSCHDbmInsBcchOnDlsch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
Void rgSCHDbmInsBcchOnDlsch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsBcchOnDlsch)

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
   RETVOID;

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
#ifdef ANSI
Void rgSCHDbmInsPcch
(
RgSchCellCb       *cellCb, 
RgSchClcDlLcCb   *cmnDlLcCb
)
#else
Void rgSCHDbmInsPcch(cellCb, cmnDlLcCb)
RgSchCellCb       *cellCb;
RgSchClcDlLcCb   *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInsPcch)

   cellCb->cmnLcCb[RGSCH_PCCH_IDX].lcId = cmnDlLcCb->lcId;
   cellCb->cmnLcCb[RGSCH_PCCH_IDX].boLst = cmnDlLcCb->boLst;

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

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
#ifdef ANSI
Void rgSCHDbmInitCmnLcBoLst
(
RgSchClcDlLcCb       *cmnDlLcCb
)
#else
Void rgSCHDbmInitCmnLcBoLst(cmnDlLcCb)
RgSchClcDlLcCb       *cmnDlLcCb;
#endif
{
   TRC2(rgSCHDbmInitCmnLcBoLst)

   cmLListInit(&cmnDlLcCb->boLst);
   RETVOID;
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
#ifdef ANSI
Void rgSCHDbmInsCmnLcBoRpt
(
RgSchClcDlLcCb    *cmnDlLcCb,
RgSchClcBoRpt     *cmnBoRpt
)
#else
Void rgSCHDbmInsCmnLcBoRpt(cmnDlLcCb, cmnBoRpt)
RgSchClcDlLcCb    *cmnDlLcCb;
RgSchClcBoRpt     *cmnBoRpt;
#endif
{
   TRC2(rgSCHDbmInsCmnLcBoRpt)

   cmnBoRpt->boLstEnt.next = NULLP;
   cmnBoRpt->boLstEnt.prev = NULLP;
   cmnBoRpt->boLstEnt.node = (PTR)cmnBoRpt;
   cmLListAdd2Tail(&cmnDlLcCb->boLst, &cmnBoRpt->boLstEnt);
   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitRaCbLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitRaCbLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitRaCbLst)

   cmLListInit(&cellCb->raInfo.raCbLst);
   RETVOID;
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
#ifdef ANSI
RgSchRaCb* rgSCHDbmGetRaCb
(
RgSchCellCb       *cellCb, 
CmLteRnti      key
)
#else
RgSchRaCb* rgSCHDbmGetRaCb(cellCb, key)
RgSchCellCb       *cellCb; 
CmLteRnti      key;
#endif
{
   CmLList *tmpNode;

   TRC2(rgSCHDbmGetRaCb)

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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitRaReqLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitRaReqLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   uint8_t idx;

   TRC2(rgSCHDbmInitRaReqLst)

   /* ccpu00133557- Memory Leak Fix- initializing for the all nodes 
    * in RAREQ list*/
   for(idx = 0; idx < RGSCH_RAREQ_ARRAY_SIZE; idx++)
   {
      cmLListInit(&cellCb->raInfo.raReqLst[idx]);
   }
   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitCrntRgrCfgLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitCrntRgrCfgLst)

   cmLListInit(&cellCb->rgCfgInfo.crntRgrCfgLst);
   RETVOID;
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
#ifdef ANSI
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst
(
RgSchCellCb       *cellCb
)
#else
PRIVATE Void rgSCHDbmInitPndngRgrCfgLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmInitPndngRgrCfgLst)

   cmLListInit(&cellCb->rgCfgInfo.pndngRgrCfgLst);
   RETVOID;
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
#ifdef ANSI
Void rgSCHDbmInsCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
Void rgSCHDbmInsCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmInsCrntRgrCfgElem)

   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.crntRgrCfgLst, &cfgElem->cfgReqLstEnt);
   RETVOID;
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
#ifdef ANSI
Void rgSCHDbmInsPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
Void rgSCHDbmInsPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmInsPndngRgrCfgElem)

   cfgElem->cfgReqLstEnt.next = NULLP;
   cfgElem->cfgReqLstEnt.prev = NULLP;
   cfgElem->cfgReqLstEnt.node = (PTR)cfgElem;
   cmLListAdd2Tail(&cellCb->rgCfgInfo.pndngRgrCfgLst, &cfgElem->cfgReqLstEnt);
   RETVOID;
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
#ifdef ANSI
RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
RgSchCfgElem* rgSCHDbmGetNextCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb; 
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmGetNextCrntRgrCfgElem)

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
#ifdef ANSI
RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
RgSchCfgElem* rgSCHDbmGetNextPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb; 
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmGetNextPndngRgrCfgElem)

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
#ifdef ANSI
RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey
(
RgSchCellCb          *cellCb,
CmLteTimingInfo   key
)
#else
RgSchCfgElem* rgSCHDbmGetPndngRgrCfgElemByKey(cellCb, key)
RgSchCellCb          *cellCb;
CmLteTimingInfo   key;
#endif
{
   CmLList  *tmpNode;

   TRC2(rgSCHDbmGetPndngRgrCfgElemByKey)

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
#ifdef ANSI
RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
RgSchCfgElem* rgSCHDbmDelCrntRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmDelCrntRgrCfgElem)

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
#ifdef ANSI
RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem
(
RgSchCellCb       *cellCb,
RgSchCfgElem      *cfgElem
)
#else
RgSchCfgElem* rgSCHDbmDelPndngRgrCfgElem(cellCb, cfgElem)
RgSchCellCb       *cellCb;
RgSchCfgElem      *cfgElem;
#endif
{
   TRC2(rgSCHDbmDelPndngRgrCfgElem)

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
#ifdef ANSI
S16 rgSCHDbmRntiDbInit
(
RgSchCellCb       *cellCb,
uint16_t            rntiStart,
uint16_t            maxRntis
)
#else
S16 rgSCHDbmRntiDbInit(cellCb, rntiStart, maxRntis)
RgSchCellCb       *cellCb;
uint16_t            rntiStart;
uint16_t            maxRntis;
#endif
{
   uint16_t rnti;
   RgSchRntiLnk   *rntiPool;

   TRC2(rgSCHDbmRntiDbInit)

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
#ifdef ANSI
Void rgSCHDbmRntiDbDeInit
(
RgSchCellCb       *cellCb
)
#else
Void rgSCHDbmRntiDbDeInit(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmRntiDbDeInit)

   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(cellCb->instIdx, (Data **)(&(cellCb->rntiDb.rntiPool)),
                 cellCb->rntiDb.maxRntis*sizeof(RgSchRntiLnk));
   cellCb->rntiDb.maxRntis = 0;
   cellCb->rntiDb.freeRnti = NULLP;
   cellCb->rntiDb.lastRnti = NULLP;
   cmLListInit(&cellCb->rntiDb.rntiGuardPool);

   /* Stack Crash problems for TRACE5 Changes. Added the return below */
   RETVOID;

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
#ifdef ANSI
RgSchRntiLnk* rgSCHDbmGetRnti
(
RgSchCellCb       *cellCb
)
#else
RgSchRntiLnk* rgSCHDbmGetRnti(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   RgSchRntiLnk *rntiLnk;

   TRC2(rgSCHDbmGetRnti)

   if (!(cellCb->rntiDb.freeRnti))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"RNTI exhausted count:%d",
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

   printf("rgSCHDbmGetRnti::rntiLnk->rnti %u\n",rntiLnk->rnti);
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
#ifdef ANSI
Void rgSCHDbmRlsRnti
(
RgSchCellCb       *cellCb,
RgSchRntiLnk      *rntiLnk
)
#else
Void rgSCHDbmRlsRnti(cellCb, rntiLnk)
RgSchCellCb       *cellCb;
RgSchRntiLnk      *rntiLnk;
#endif
{
   TRC2(rgSCHDbmRlsRnti)
#ifdef EMTC_ENABLE
      if(ROK==rgSCHDbmPutEmtcRnti(cellCb,rntiLnk))
{      
RETVOID;
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
   RETVOID;

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
#ifdef ANSI
PRIVATE S16 rgSCHDbmInitUeTfuPendLst
(
RgSchCellCb    *cellCb,
uint16_t            numBins
)
#else
PRIVATE S16 rgSCHDbmInitUeTfuPendLst(cellCb, numBins)
RgSchCellCb    *cellCb;
uint16_t            numBins;
#endif
{
   RgSchUePucchRecpInfo pucchInfo;
   TRC2(rgSCHDbmInitUeTfuPendLst)

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
#ifdef ANSI
S16 rgSCHDbmDeInitUeTfuPendLst
(
RgSchCellCb       *cellCb
)
#else
S16 rgSCHDbmDeInitUeTfuPendLst(cellCb)
RgSchCellCb       *cellCb;
#endif
{
   TRC2(rgSCHDbmDeInitUeTfuPendLst)

   cmHashListDeinit(&cellCb->ueTfuPendLst);
   
   return ROK;
}  /* rgSCHDbmDeInitUeTfuPendLst */
#endif

/**********************************************************************
 
         End of file
**********************************************************************/
