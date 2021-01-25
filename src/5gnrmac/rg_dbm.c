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
  
     File:     rg_dbm.c
  
**********************************************************************/

/** @file rg_dbm.c
@brief This file contains the APIs exposed for the insertion/fetching/deletion of cellCb/ueCb and traversal of LcCbLst.
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

/* header/extern include files (.x) */
#include "tfu.x"
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "rg_sch_inf.x"
#include "rg_prg.x"       /* PRG interface typedefs*/
#include "rg.x"

/* local defines */
static S16 rgDBMInitUeCbLst ARGS(( RgCellCb *cellCb, uint16_t numBins));
static Void rgDBMInitDedLcLst ARGS((RgUeCb *ueCb));
static Void rgDBMInitCmnLcLst ARGS((RgCellCb *cellCb));
static Void rgDBMInitRachLst ARGS((RgCellCb *cellCb));
#ifdef LTEMAC_SPS
static S16 rgDBMInitSpsUeCbLst ARGS((
RgCellCb       *cellCb,
uint16_t       numBins
));
#endif

/* local typedefs */
 
/* local externs */
 
/* forward references */

/**
 * @brief Handler for Initializing the cell.
 *
 * @details
 *
 *     Function : rgDBMInitCell
 *     
 *   Initializes the lists belonging to the cell.
 *     
 *           
 *  @param[in]  RgCellCb *cellCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
S16 rgDBMInitCell
(
RgCellCb       *cellCb
)
{
   S16 ret;
   
   /* Initialize ue list */
   if ((ret = rgDBMInitUeCbLst(cellCb, RG_MAX_UE_BIN_PER_CELL)) != ROK)
      return (ret);

#ifdef LTEMAC_SPS
   /* Initialize SPS Ue list */
   if ((ret = rgDBMInitSpsUeCbLst(cellCb, RG_MAX_UE_BIN_PER_CELL)) != ROK)
      return (ret);
#endif /* LTEMAC_SPS */

   /* Initialize BCCH/PCCH logical channels */
   rgDBMInitCmnLcLst(cellCb);


   /* Initialize rach ue list */
   rgDBMInitRachLst(cellCb);

   return (ret);

} /* rgDBMInitCell */

/**
 * @brief Handler for initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDBMInitUeCbLst
(
RgCellCb       *cellCb,
uint16_t       numBins
)
{
   Inst inst = cellCb->macInst - RG_INST_START;
   RgUeCb ue;  

   return (cmHashListInit(&cellCb->ueLst, numBins, (uint16_t)((PTR)&(ue.ueLstEnt) - (PTR)&ue), FALSE, 
               CM_HASH_KEYTYPE_CONID, rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool));

}  /* rgDBMInitUeCbLst */

/**
 * @brief Handler for de-initializing the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMDeInitUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMDeInitUeCbLst(RgCellCb *cellCb)
{

   return (cmHashListDeinit(&cellCb->ueLst));

}  /* rgDBMDeInitUeCbLst */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] numBins 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
static S16 rgDBMInitSpsUeCbLst(RgCellCb *cellCb,uint16_t numBins)
{
   Inst inst = cellCb->macInst - RG_INST_START;
   RgUeCb ue;

   return (cmHashListInit(&cellCb->spsUeLst, numBins, (uint16_t) ((PTR) &(ue.spsUeLstEnt) - (PTR) &ue), FALSE, 
               CM_HASH_KEYTYPE_CONID, rgCb[inst].rgInit.region, rgCb[inst].rgInit.pool));

}  /* rgDBMInitSpsUeCbLst */

/**
 * @brief Handler for de-initializing the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMDeInitSpsUeCbLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMDeInitSpsUeCbLst(RgCellCb *cellCb)
{

   return (cmHashListDeinit(&cellCb->spsUeLst));

}  /* rgDBMDeInitSpsUeCbLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for inserting the ueCb in to the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMInsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMInsUeCb(RgCellCb *cellCb,RgUeCb *ueCb)
{
   return (cmHashListInsert(&cellCb->ueLst, (PTR)ueCb, 
      (uint8_t *)&ueCb->ueId, (uint16_t)sizeof(ueCb->ueId)));

}  /* rgDBMInsUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for inserting the ueCb in to the spsUeCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMInsSpsUeCb
 *     
 *           
 *  @param[in] *cellCb 
 *  @param[in] *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMInsSpsUeCb(RgCellCb *cellCb,RgUeCb  *ueCb)
{

   return (cmHashListInsert(&cellCb->spsUeLst, (PTR)ueCb, 
      (uint8_t *)&ueCb->spsRnti, (uint16_t)sizeof(ueCb->spsRnti)));

}  /* end of rgDBMInsSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for accessing the existing ueCb identified by the key ueId
 * in the ueCbLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMGetUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetUeCb(RgCellCb *cellCb, CmLteRnti ueId)
{
   RgUeCb *ueCb = NULLP; 

   cmHashListFind(&cellCb->ueLst, (uint8_t *)&ueId,
      sizeof(ueId), 0, (PTR *)&ueCb);
   return (ueCb);
}  /* rgDBMGetUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the ueCb identified by the key sps-rnti
 * in the spsUeLst under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMGetSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  ueId
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetSpsUeCb(RgCellCb  *cellCb, CmLteRnti spsRnti)
{
   RgUeCb *ueCb = NULLP; 

   cmHashListFind(&cellCb->spsUeLst, (uint8_t *)&spsRnti,
      sizeof(spsRnti), 0, (PTR *)&ueCb);
   return (ueCb);
}  /* rgDBMGetSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for accessing the existing next ueCb in the ueCbLst under the
 *  cellCb.
 *
 * @details
 *
 *     Function : rgDBMGetNextUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetNextUeCb(RgCellCb *cellCb, RgUeCb *ueCb)
{
   RgUeCb *nextUeCb = NULLP; 

   cmHashListGetNext(&cellCb->ueLst, (PTR) ueCb, (PTR *)&nextUeCb);
   return (nextUeCb);
}  /* rgDBMGetNextUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for accessing the existing next ueCb stored in the spsUeCbLst
 *         using SPS-Rnti under the cellCb.
 *
 * @details
 *
 *     Function : rgDBMGetNextSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetNextSpsUeCb(RgCellCb *cellCb, RgUeCb *ueCb)
{
   RgUeCb *nextUeCb = NULLP; 

   cmHashListGetNext(&cellCb->spsUeLst, (PTR) ueCb, (PTR *)&nextUeCb);
   return (nextUeCb);
}  /* end of rgDBMGetNextSpsUeCb */

#endif /* LTEMAC_SPS */


/**
 * @brief Handler for deleting the existing ueCb from the ueCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgDBMDelUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMDelUeCb(RgCellCb *cellCb, RgUeCb *ueCb)
{
   return (cmHashListDelete(&cellCb->ueLst, (PTR)ueCb));
}  /* rgDBMDelUeCb */

#ifdef LTEMAC_SPS
/**
 * @brief Handler for deleting the existing ueCb from the spsUeCbLst under the
 * cellCb.
 *
 * @details
 *
 *     Function : rgDBMDelSpsUeCb
 *     
 *           
 *  @param[in]  *cellCb 
 *  @param[in]  *ueCb 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
S16 rgDBMDelSpsUeCb(RgCellCb  *cellCb, RgUeCb *ueCb)
{

   return (cmHashListDelete(&cellCb->spsUeLst, (PTR)ueCb));
}  /* end of rgDBMDelSpsUeCb */

#endif /* LTEMAC_SPS */

/**
 * @brief Handler for Initializing the UE.
 *
 * @details
 *
 *     Function : rgDBMInitUe
 *     
 *   Initializes the lists belonging to the UE.
 *     
 *           
 *  @param[in]  RgUeCb  *ueCb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
Void rgDBMInitUe(RgUeCb *ueCb)
{
#ifdef LTEMAC_SPS
   uint8_t        idx;
#endif

   /* Initialize Dedicated logical channels */
   rgDBMInitDedLcLst(ueCb);

#ifdef LTEMAC_SPS
   /* Initialize the SPS LC Ids */
   for (idx=0; idx < RG_MAX_LC_PER_UE; idx++)
   {
      ueCb->ul.spsLcId[idx] = FALSE;
   }
#endif


   return;
} /* rgDBMInitUe */

/**
 * @brief Handler for Initializing the dedicated logical channels.
 *
 * @details
 *
 *     Function : rgDBMInitDedLcLst
 *     
 *   Initializes dedicated logical channels.
 *           
 *  @param[in]  RgUeCb *ueCb
 *  @return     Void 
 **/
static Void rgDBMInitDedLcLst(RgUeCb *ueCb)
{
   uint8_t idx;
   
   for (idx = 0; idx < RG_MAX_LC_PER_UE; ++idx)
   {
      /* Set Dedicated LCs as not configured */
      ueCb->ul.lcCb[idx].lcId = RG_INVALID_LC_ID;
      ueCb->ul.lcCb[idx].lcgId = RG_INVALID_LCG_ID;
      ueCb->dl.lcCb[idx].lcId = RG_INVALID_LC_ID;
   }

   for (idx = 0; idx < RG_MAX_LCG_PER_UE; ++idx)
   {
      /* Set LCGs as not configured */
      ueCb->ul.lcgArr[idx].lcgId = RG_INVALID_LCG_ID;
      ueCb->ul.lcgArr[idx].lcCount = 0;
   }

   /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
} /* rgDBMInitDedLcLst */

/**
 * @brief Handler for Initializing the common logical channel list of the cell.
 *
 * @details
 *
 *     Function : rgDBMInitCmnLcLst
 *     
 *   Initializes following common logical channels belonging to the cell.
 *     - BCCH on BCH
 *     - BCCH on DLSCH
 *     - PCCH
 *           
 *  @param[in]  RgCellCb *cellCb
 *  @return  Void
 **/
static Void rgDBMInitCmnLcLst(RgCellCb *cellCb)
{
   uint8_t idx;
   
   cellCb->bcchBchInfo.lcId = RG_INVALID_LC_ID;
   cellCb->pcchInfo.lcId = RG_INVALID_LC_ID;

   cellCb->numBcchDlschInfo = 0;
   for (idx = 0; idx < RG_MAX_BCCH_DLSCH; idx++)
   {
      cellCb->bcchDlschInfo[idx].lcId = RG_INVALID_LC_ID;
      cellCb->bcchDlschInfo[idx].tb = NULLP;
   }

   /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
} /* rgDBMInitCmnLcLst */

/**
 * @brief Handler for Initializing the common logical channel list of the cell.
 *
 * @details
 *
 *     Function : rgDBMFreeCmnLcLst
 *     
 *   Initializes following common logical channels belonging to the cell.
 *     - BCCH on BCH
 *     - BCCH on DLSCH
 *     - PCCH
 *           
 *  @param[in]  RgCellCb *cellCb
 *  @return  Void
 **/
Void rgDBMFreeCmnLcLst(RgCellCb *cellCb)
{
   uint8_t idx;
   
   cellCb->bcchBchInfo.lcId = RG_INVALID_LC_ID;
   cellCb->pcchInfo.lcId = RG_INVALID_LC_ID;

   for (idx = 0; idx < cellCb->numBcchDlschInfo; idx++)
   {
      cellCb->bcchDlschInfo[idx].lcId = RG_INVALID_LC_ID;
      RG_FREE_MSG(cellCb->bcchDlschInfo[idx].tb);
   }
   cellCb->numBcchDlschInfo = 0;

   /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
} /* rgDBMFreeCmnLcLst */

/**
 * @brief Handler for inserting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgDBMInsDlDedLcCb
 *     
 *  @param[in]  RgUeCb *ueCb
 *  @param[in]  RgDlLcCb* dlLcCb
 *  @return  Void
 **/
Void rgDBMInsDlDedLcCb(RgUeCb  *ueCb, CmLteLcId idx)
{
   if( idx >= RG_DEDLC_MIN_LCID )
   {
      ueCb->dl.lcCb[idx-1].lcId = idx;
   }
  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMInsDlDedLcCb */

/**
 * @brief Handler for deleting dedicated DL logical channel.
 *
 * @details
 *
 *     Function : rgDBMDelDlDedLcCb
 *     
 *  @param[in]  RgUeCb *ueCb
 *  @param[in]  RgDlLcCb* dlLcCb
 *  @return  Void
 **/
Void rgDBMDelDlDedLcCb(RgUeCb  *ueCb, RgDlLcCb *dlLcCb)
{
   ueCb->dl.lcCb[dlLcCb->lcId - 1].lcId = RG_INVALID_LC_ID;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
  
}  /* rgDBMDelDlDedLcCb */

/**
 * @brief Handler for accessing the existing DL dedicated lcCb at idx in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMGetDlDedLcCb
 *     
 *  @param[in]  *ueCb
 *  @param[in]  idx
 *  @return  RgDlLcCb*
 **/
RgDlLcCb* rgDBMGetDlDedLcCb(RgUeCb *ueCb, CmLteLcId idx)
{
   if (idx > RG_DEDLC_MAX_LCID || idx <= 0)
   {
      return (NULLP);
   }
   if(ueCb->dl.lcCb[idx-1].lcId == RG_INVALID_LC_ID)
   {
      return (NULLP);
   }

   return (&ueCb->dl.lcCb[idx-1]);

}  /* rgDBMGetDlDedLcCb */

/**
 * @brief Handler for inserting dedicated UL logical channel.
 *
 * @details
 *
 *     Function : rgDBMInsUlDedLcCb
 *     
 *           
 *  @param[in]  RgUeCb *ueCb
 *  @param[in]  CmLteLcId idx
 *  @param[in]  LteLcgId gId
 *  @return  Void
 **/
#ifdef LTE_L2_MEAS
Void rgDBMInsUlDedLcCb
(
RgUeCb         *ueCb, 
CmLteLcId      idx,
LteLcgId       gId,
uint8_t        qci
)
#else
Void rgDBMInsUlDedLcCb
(
RgUeCb         *ueCb, 
CmLteLcId      idx,
LteLcgId       gId
)
#endif
{
   if ( idx >= RG_DEDLC_MIN_LCID)
   {
      ueCb->ul.lcCb[idx - 1].lcId = idx;
      ueCb->ul.lcCb[idx - 1].lcgId = gId;
#ifdef LTE_L2_MEAS
      ueCb->ul.lcCb[idx - 1].qci = qci;
#endif
   }
   if(ueCb->ul.lcgArr[gId].lcgId == RG_INVALID_LCG_ID)
   {
      ueCb->ul.lcgArr[gId].lcgId = gId;
   }
   ueCb->ul.lcgArr[gId].lcCount = ueCb->ul.lcgArr[gId].lcCount + 1;


  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;

}  /* rgDBMInsUlDedLcCb */

/**
 * @brief Handler for deleting the dedicated UL logical channel.
 *
 * @details
 *
 *     Function : rgDBMUpdUlDedLcCb
 *     
 *           
 *  @param[in]  RgUeCb *ueCb
 *  @param[in]  RgUlLcCb* ulLcCb
 *  @param[in]  LteLcgId gId
 *  @return  Void
 **/
Void rgDBMUpdUlDedLcCb
(
RgUeCb         *ueCb, 
RgUlLcCb       *ulLcCb,
LteLcgId       gId
)
{
   ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount = 
               ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount - 1;
   if(ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount == 0)
   {
      ueCb->ul.lcgArr[ulLcCb->lcgId].lcgId = RG_INVALID_LCG_ID;
   }
   ueCb->ul.lcCb[ulLcCb->lcId - 1].lcgId = gId;

   if(ueCb->ul.lcgArr[gId].lcgId == RG_INVALID_LCG_ID)
   {
      ueCb->ul.lcgArr[gId].lcgId = gId;
   }
   ueCb->ul.lcgArr[gId].lcCount = ueCb->ul.lcgArr[gId].lcCount + 1;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMUpdUlDedLcCb */

/**
 * @brief Handler for updating the dedicated UL logical channel.
 *
 * @details
 *
 *     Function : rgDBMDelUlDedLcCb
 *     
 *           
 *  @param[in]  RgUeCb *ueCb
 *  @param[in]  RgUlLcCb* ulLcCb
 *  @return  Void
 **/
Void rgDBMDelUlDedLcCb(RgUeCb *ueCb, RgUlLcCb *ulLcCb)
{

   ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount = 
               ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount - 1;
   if(ueCb->ul.lcgArr[ulLcCb->lcgId].lcCount == 0)
   {
      ueCb->ul.lcgArr[ulLcCb->lcgId].lcgId = RG_INVALID_LCG_ID;
   }
   ueCb->ul.lcCb[ulLcCb->lcId - 1].lcgId = RG_INVALID_LCG_ID;
   ueCb->ul.lcCb[ulLcCb->lcId - 1].lcId = RG_INVALID_LC_ID;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMDelUlDedLcCb */

/**
 * @brief Handler for accessing the existing UL dedicated lcCb at idx in the
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMGetUlDedLcCb
 *     
 *           
 *  @param[in]  *ueCb
 *  @param[in]  idx
 *  @return  RgUlLcCb*
 **/
RgUlLcCb* rgDBMGetUlDedLcCb(RgUeCb *ueCb, CmLteLcId  idx)
{

   if (idx > RG_DEDLC_MAX_LCID || idx < RG_DEDLC_MIN_LCID)
   {
      return (NULLP);
   }
   if(ueCb->ul.lcCb[idx-1].lcId == RG_INVALID_LC_ID)
   {
      return (NULLP);
   }

   return (&ueCb->ul.lcCb[idx-1]);
}  /* rgDBMGetDlDedLcCb */

/**
 * @brief Handler for accessing the existing DL common lcCb identified by the key lcId
 * in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMChkCmnLcCb
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  lcId
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
S16 rgDBMChkCmnLcCb(RgCellCb *cellCb, CmLteLcId lcId)
{
   uint8_t idx;

   if(cellCb->bcchBchInfo.lcId == lcId)
   {
      return ROK;
   } 
   if(cellCb->pcchInfo.lcId == lcId)
   {
      return ROK;
   }

   for (idx = 0; idx < cellCb->numBcchDlschInfo; idx++)
   {
      if(cellCb->bcchDlschInfo[idx].lcId == lcId)
      {
         return ROK;
      } 
   }
   return RFAILED;
}  /* rgDBMChkCmnLcCb */

/**
 * @brief Handler for accessing the existing BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMGetBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgBcchBchLcCb*
 **/
RgBcchBchLcCb* rgDBMGetBcchOnBch(RgCellCb *cellCb)
{

   if(cellCb->bcchBchInfo.lcId != RG_INVALID_LC_ID)
   {
      return (&(cellCb->bcchBchInfo));
   }
   return (NULLP);
}  /* rgDBMGetBcchOnBch */

/**
 * @brief Handler for accessing the existing BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMGetBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  lcId
 *  @return  RgBcchDlschLcCb*
 **/
RgBcchDlschLcCb* rgDBMGetBcchOnDlsch(RgCellCb *cellCb,CmLteLcId lcId)
{
   uint8_t idx;

   for (idx = 0; idx < RG_MAX_BCCH_DLSCH; idx++)
   {
      if(cellCb->bcchDlschInfo[idx].lcId == lcId)
      {
         return (&(cellCb->bcchDlschInfo[idx]));
      }
   }
   return (NULLP);
}  /* rgDBMGetBcchOnDlsch */

/**
 * @brief Handler for accessing the existing PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMGetPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  RgPcchLcCb*
 **/
RgPcchLcCb* rgDBMGetPcch(RgCellCb *cellCb)
{
 
   if(cellCb->pcchInfo.lcId != RG_INVALID_LC_ID)
   {
      return (&(cellCb->pcchInfo));
   }
   return (NULLP);
}  /* rgDBMGetPcch */

/**
 * @brief Handler for inserting the BCCH mapped on to BCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMInsBcchOnBch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgDBMInsBcchOnBch(RgCellCb *cellCb, CmLteLcId idx)
{

   cellCb->bcchBchInfo.lcId = idx;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMInsBcchOnBch */

/**
 * @brief Handler for inserting the BCCH mapped on to DLSCH in the 
 * lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMInsBcchOnDlsch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgDBMInsBcchOnDlsch(RgCellCb *cellCb, CmLteLcId idx)
{

   cellCb->bcchDlschInfo[cellCb->numBcchDlschInfo].lcId = idx;
   cellCb->numBcchDlschInfo++;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMInsBcchOnDlsch */


/**
 * @brief Handler for inserting the PCCH in the lcCbLst of the ueCb.
 *
 * @details
 *
 *     Function : rgDBMInsPcch
 *     
 *           
 *  @param[in]  *cellCb
 *  @return  Void
 **/
Void rgDBMInsPcch(RgCellCb *cellCb, CmLteLcId idx)
{

   cellCb->pcchInfo.lcId = idx;

  /* Stack Crash problem for TRACE5 Changes. Added the return below */
  return;
}  /* rgDBMInsPcch */

/**
 * @brief Handler for initializing the rachLst
 *
 * @details
 *
 *     Function : rgDBMInitRachLst
 *     
 *           
 *  @param[in] *cellCb 
 *  @return  Void
 **/
static Void rgDBMInitRachLst(RgCellCb *cellCb)
{

   cmLListInit(&cellCb->raInfo.ueRachLst);
   return;
}  /* rgDBMInitRachLst */

/**
 * @brief Handler for inserting the ueCb in the rachLst.
 *
 * @details
 *
 *     Function : rgDBMInsUeCbInRachLst
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  Void
 **/
Void rgDBMInsUeCbInRachLst(RgCellCb *cellCb, RgUeCb *ueCb)
{

   cmLListAdd2Tail(&cellCb->raInfo.ueRachLst,&ueCb->rachLstEnt);

   return;
}  /* rgDBMInsUeCbInRachLst */

/**
 * @brief Handler for accessing the existing raCb in the rachLst.
 *
 * @details
 *
 *     Function : rgDBMGetUeCbFromRachLst
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  key
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetUeCbFromRachLst(RgCellCb *cellCb, CmLteRnti key)
{
   CmLList *tmpNode;

   CM_LLIST_FIRST_NODE(&cellCb->raInfo.ueRachLst,tmpNode);
   while(tmpNode)
   {
      if(((RgUeCb *)tmpNode->node)->ueId == key)
      {
         return ((RgUeCb *)(tmpNode->node));
      }
      CM_LLIST_NEXT_NODE(&cellCb->raInfo.ueRachLst,tmpNode);
   }
   return (NULLP);
}  /* rgDBMGetUeCbFromRachLst */

/**
 * @brief Handler for accessing the existing ueCb from rachLst.
 *
 * @details
 *
 *     Function : rgDBMGetNextUeCbFromRachLst
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  *ueCb
 *  @return  RgUeCb*
 **/
RgUeCb* rgDBMGetNextUeCbFromRachLst(RgCellCb *cellCb,RgUeCb *ueCb)
{

   if(!ueCb)
   {
      return ( cellCb->raInfo.ueRachLst.first ? 
               (RgUeCb *)(cellCb->raInfo.ueRachLst.first->node) : NULLP );
   }
   return ( ueCb->rachLstEnt.next ? 
               (RgUeCb *)(ueCb->rachLstEnt.next->node) : NULLP );
}  /* rgDBMGetNextUeCbFromRachLst */

/**
 * @brief Handler for deleting the existing ueCb in the rachLst.
 *
 * @details
 *
 *     Function : rgDBMDelUeCbFromRachLst
 *     
 *           
 *  @param[in]  *cellCb
 *  @param[in]  key
 *  @return  RgUeCb*
 **/
Void rgDBMDelUeCbFromRachLst(RgCellCb *cellCb, RgUeCb *ueCb)
{

   cmLListDelFrm(&cellCb->raInfo.ueRachLst, &ueCb->rachLstEnt);
   return;
}  /* rgDBMDelUeCbFromRachLst */

/**********************************************************************
 
         End of file
**********************************************************************/
