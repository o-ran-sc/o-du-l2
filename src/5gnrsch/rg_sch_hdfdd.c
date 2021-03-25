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
 
     Desc:     C source code for HD-FDD functions
 
     File:     rg_sch_hdfdd.c
 
**********************************************************************/

/** @file rg_sch_hdfdd.c
@brief This module handles the  Periodic CQI/PMI/RI, SRS, SR and Half Duplex 
       functionality 
*/

/* header include files -- defines (.h) */
#include "common_def.h"
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include  "rgr.h"
#include  "rgm.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"

/* header/extern include files (.x) */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"


#ifdef LTEMAC_HDFDD
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* @details
 *
 *     Function : rgSCHHdFddUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re/configuration. 
 *        
 *     Processing Steps:
 *     - For UE-specific Half Duplex  
 *      - Allocate the memory and place the UE in cellCb->hdUeLstCp 
 *        - Update subframes information state to defualt
 *        - Update subframes information sfn to defualt
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  Bool         *hdFddEnbl
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
*/ 

S16 rgSCHHdFddUeCfg(RgSchCellCb  *cellCb,RgSchUeCb *ueCb,Bool hdFddEnbl)
{
   uint8_t sfi;

   DU_LOG("\nDEBUG  -->  SCH : rgSCHHdFddUeCfg(): UeId =%d hdFddEnbl=%d",
               ueCb->ueId, hdFddEnbl);
   if(ueCb->hdFddEnbld == TRUE)
   {
      if (hdFddEnbl == FALSE)
      {
         /* Do not allow switch from HD-FDD to FD-FDD configuration */
         DU_LOG("\nERROR  -->  SCH : rgSCHHdFddUeCfg(): HD-FDD  to FD-FDD Configuration is not allowed"
               "CRNTI:%d",ueCb->ueId);
      }
      else
      {
         /* If already enabled then it can be second reconfiguration */
         DU_LOG("\nERROR  -->  SCH : rgSCHHdFddUeCfg(): HD-FDD already enabled for this UE"
                  "CRNTI:%d",ueCb->ueId);
      }
      return RFAILED;
   }

#ifdef LTEMAC_SPS
   /* Check is SPS enabled for this UE */
   if(hdFddEnbl == TRUE &&
       (ueCb->ul.ulSpsCfg.isUlSpsEnabled == TRUE ||
        ueCb->dl.dlSpsCfg.isDlSpsEnabled == TRUE))
   {
      DU_LOG("\nERROR  -->  SCH : rgSCHHdFddUeCfg(): Could'nt do HDFDD cfg, SPS already configured"
               "CRNTI:%d",ueCb->ueId);
      return RFAILED;
   }
#endif

   ueCb->hdFddEnbld = hdFddEnbl;
   if( hdFddEnbl == TRUE)
   {
      rgSCHUtlAllocSBuf(cellCb->instIdx,(Data **) &ueCb->hdFddCb,
             sizeof(RgSchUeHdFddCb));
      if (ueCb->hdFddCb != NULLP)
      {
         for (sfi = 0; sfi < RG_SCH_HDFDD_NUMSFINFO; sfi++)
         {
            ueCb->hdFddCb->subfrm[sfi].subFrmDir = RG_SCH_HDFDD_NOSCHD;
            ueCb->hdFddCb->subfrm[sfi].sfn      = RG_SCH_HDFDD_INVSFN;
         }
         /* Add this UE to list maintained in CellCb */
         /* cmLListAdd2Tail(&cellCb->hdFddLst,&ueCb->hdFddCb->hdFddLstEnt); */
      }
      else
      {
         DU_LOG("\nERROR  -->  SCH : rgSCHHdFddUeCfg(): Could not allocate memory for hd-fdd ueCb"
               "CRNTI:%d",ueCb->ueId);
         return RFAILED;
      }
   }
   return ROK;
}/*rgSCHHdFddUeCfg*/


/* @brief Frees Half Duplex  related data structures
 *
 * @details
 *
 *     Function : rgSCHHdFddUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *
 *     Processing Steps:
 *       - if Half Duplex is enabled
 *       - if (ueCb->hdFddCb != NULL)
 *        - Remove ue from  cellCb->hdUeLstCp;
 *        - Dellocate memory 
 *       - else
 *        - Nothing to do
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return ( S16
 *      -# ROK
*
*/
S16 rgSCHHdFddUeDel(RgSchCellCb *cellCb,RgSchUeCb   *ueCb)
{

    DU_LOG("\nDEBUG  -->  SCH : rgSCHHdFddUeDel(): UeId =%d hdFdd=%x",
             ueCb->ueId, ueCb->hdFddEnbld);


    if (ueCb->hdFddCb)
    {
       /* ccpu00117052 - MOD - Passing double pointer
       for proper NULLP assignment*/
       rgSCHUtlFreeSBuf(cellCb->instIdx, (Data **)(&(ueCb->hdFddCb)),
             sizeof(RgSchUeHdFddCb));   
       ueCb->hdFddEnbld = FALSE;
    }

    return ROK;
} /* rgSCHHdFddUeDel */



#ifdef TFU_UPGRADE
/* @brief Mark the subframes as uplink for HD FDD if CQI/RI or SRS or RI is
 * expecting .
 *
 * @details
 *
 *     Function: rgSCHCmnHdFddPtUlMrk 
 *     Purpose:  Updation of Periodic CQI/PMI, SRS and SR tranmission
 *               instance updates
 *               for HD FDD UEs
 *  @param[in]  RgSchCellCb *cell
 *  @return    None
 */

Void rgSCHCmnHdFddPtUlMrk(RgSchCellCb *cellCb)
{
   uint16_t           sfn; /* System Frame Number */
   uint32_t           pti; /* Index into Periodic table */
   uint16_t           sfi; /* Index into HDFDD state table */
   CmLListCp          *cqiLst;
   CmLListCp          *srsLst;
   CmLListCp          *srLst;
   CmLListCp          *riLst;
   CmLList            *cqiNode;
   CmLList            *srsNode;
   CmLList            *srNode;
   CmLList            *riNode;
   CmLteTimingInfo    timeInfo;
   RgSchUePCqiCb      *cqiCb = NULLP;
   RgSchUePCqiCb      *riCb = NULLP;


   timeInfo = cellCb->crntTime;

   /* Determine indexes */
   pti = RG_SCH_HDFDD_GETPTI(timeInfo);
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_DELTA);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_DELTA);

   /* Get PT entries for */
   cqiLst = &cellCb->pCqiSrsSrLst[pti].cqiLst;
   srsLst = &cellCb->pCqiSrsSrLst[pti].srsLst;
   srLst  = &cellCb->pCqiSrsSrLst[pti].srLst;
   riLst  = &cellCb->pCqiSrsSrLst[pti].riLst;

   /* Get first node in each list */
   CM_LLIST_FIRST_NODE(cqiLst, cqiNode);
   CM_LLIST_FIRST_NODE(srsLst, srsNode);
   CM_LLIST_FIRST_NODE(riLst,  riNode);
   CM_LLIST_FIRST_NODE(srLst,  srNode);

   /* Mark corresponding the subframe as uplink control */
   while ((NULLP != cqiNode ) &&
         (NULLP != srsNode  ) &&
         (NULLP != srNode   )  &&
         (NULLP != riNode   ))
   {
     cqiCb = (RgSchUePCqiCb *)(cqiNode->node);
     RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)(cqiCb->servCellInfo->ue),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* SRS Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)srsNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* SR Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)srNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* RI Transmission instances */
     riCb = (RgSchUePCqiCb *)(riNode->node);
     RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)riCb->servCellInfo->ue,
                     RG_SCH_HDFDD_UL, sfn, sfi);

      /* Get next UeCb for all lists */
      CM_LLIST_NEXT_NODE(cqiLst, cqiNode);
      CM_LLIST_NEXT_NODE(srsLst, srsNode);
      CM_LLIST_NEXT_NODE(srLst, srNode);
      CM_LLIST_NEXT_NODE(riLst, riNode);
   }

   while ( NULLP != cqiNode)
   {
      /* CQI/PMI Transmission instances */
      cqiCb = (RgSchUePCqiCb *)(cqiNode->node);
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)(cqiCb->servCellInfo->ue),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(cqiLst, cqiNode);
   }
   while( NULLP != srsNode)
   {
      /* SRS Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)srsNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(srsLst, srsNode);
   }
   while( NULLP != srNode)
   {
      /* SR Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)srNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(srLst, srNode);
   }
   while( NULLP != riNode)
   {
      /* RI Transmission instances */
     riCb = (RgSchUePCqiCb *)(riNode->node);
     RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)riCb->servCellInfo->ue,
                     RG_SCH_HDFDD_UL, sfn, sfi);
     CM_LLIST_NEXT_NODE(riLst, riNode);
   }

   return;
} /* rgSCHCmnHdFddPtUlMrk */
#endif /* ifdef TFU_UPGRADE */



/* @brief Decides whether UE can be allowed for DL in given subframe
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkUlAllow 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars before allocating  UL grants .
 *
 *     Processing Steps:
 *       - if Half Duplex is enabled
 *       - If ue->sf[reqsf].state is "DONWLINK"
 *            set alloweUlSch=FALSE
 *       - else
 *            set alloweUlSch=TRUE
 *        This function Marking for BCCH/PCCH occasions is also done
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return    None
 *
 */
Void rgSCHCmnHdFddChkUlAllow(RgSchCellCb *cellCb,RgSchUeCb *ueCb,uint8_t *allow)
{
   uint16_t         sfn;
   uint16_t         sfi;
   CmLteTimingInfo  timeInfo;
   RgSchDlSf        *sf = NULLP; /* Dl subframe info */
   uint8_t          ulOffset

   DU_LOG("\nDEBUG  -->  SCH :  rgSCHCmnHdFddChkUlAllow: ueId=%d ", ueCb->ueId);

   *allow = FALSE;

   timeInfo = cellCb->crntTime;

   ulOffset = RGSCH_PDCCH_PUSCH_DELTA - 
          TFU_CRCIND_ULDELTA + RGSCH_PDCCH_PUSCH_DELTA;
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, ulOffset);

   /* Set default value */
   *allow = FALSE;

   /* Validate condition 1 */
   /* For (curretn time + DL_DELTA)th sf */

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);

   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   /* Validate condition 2 */
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
         /* Mark the BCCH/PCCH occasion */
         RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi);
         DU_LOG("\nDEBUG  -->  SCH : rgSCHCmnHdFddChkUlAllow: Already marked for Cmn DL, ueId = %d ",
            ueCb->ueId);
   }
   if ((ueCb->hdFddCb->subfrm[sfi].sfn == sfn) &&
         (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA ||
          ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLCNTRL))
   {
      /* Downlink scheduled */
      *allow = FALSE;
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkUlAllow: Already marked for DL, ueId = %d ",
               ueCb->ueId);
      return;
   }

   /* Validate condition 3 */
   /* For (curretn time + DL_DELTA + HRQ_DELTA)th sf
      - i.e. next HARQ Feedback occasion */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL)
   {
      /* No place for HARQ feedback */
      *allow = FALSE;
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkUlAllow: No Place for HARQ, ueId = %d ",
         ueCb->ueId);
      return;

   }
   /* Validate condition 4 */
   /* For (curretn time + DL_DELTA - HRQ_DELTA)th sf
      - i.e. previous HARQ Feedback occasion */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (-RG_SCH_CMN_HARQ_INTERVAL));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (-RG_SCH_CMN_HARQ_INTERVAL));
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL)
   {
      *allow = FALSE;
      DU_LOG("\nERROR  -->  SCH :  rgSCHCmnHdFddChkUlAllow: No Place for UL grant, ueId = %d ",
               ueCb->ueId);
      return;

   }
   /* Validate condition 5 */
   /* For (curretn time + DL_DELTA - 1)th sf -i.e. Guard time */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (-RG_SCH_HDFDD_GRDTIM_DUR));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (-RG_SCH_HDFDD_GRDTIM_DUR));
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA))
   {
      /* This subframe may be a switching gaurd time */
      *allow = FALSE;
      DU_LOG("\nERROR  -->  SCH :  rgSCHCmnHdFddChkUlAllow: No Place for Guard time, ueId = %d ",
             ueCb->ueId);
      return;

   }
   /* Adition guard time rule check: Above check is only for PDSCH, lets check
      is there is any BCCH/PCCH data scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, 
                      (ulOffset - RG_SCH_HDFDD_GRDTIM_DUR));
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Common channel scheduled */
      /* Mark the BCCH/PCCH occasion */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, timeInfo.sfn, sfi);
      *allow = FALSE;
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkUlAllow: Already marked for Cmn DL, ueId = %d ",
         ueCb->ueId);
      return;

   }

   /* All validation done. Safe to for UL */
   *allow = TRUE;
   return;
} /* rgSCHCmnHdFddChkUlAllow */


 /* @brief Decides whether UE can be allowed for UL in given subframe
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkDlAllow
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars before allocating for DL.
 *
 *     Processing Steps:
 *          Condition 1:  subframe n + DL_DELTA should not be uplink
 *          Condition 2:  subframe n+ DL_DELTA + 1 should meet guard time
 *                        creation rule. For more
 *                        information refer to section "2.25.7.1 Guard time
 *                        creation rule"
 *          Condition 3:  subframe n + DL_DELTA + HRQ_DELTA should not be
 *                        downlink so that downlink data (HARQ Feedback)
 *                        can be received in next 4 subframe. {n + 7} Above
 *                        conditions have to
 *                        be validated by taking SFN number into consideration.
 *          if all conditions are met then *allow is set to TRUE or lese to
 *          FALSE.
 *          if hd-fdd is not anabled for this UE, then *allow is always TRUE.
 *   
 *   Returns None
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] uint8_t *allow -- TRUE is allowed or FALSE if no allowedi.
 *                   Valdity of this pointer is not done in this function
 *
 */
Void rgSCHCmnHdFddChkDlAllow 
(
RgSchCellCb *cellCb,
RgSchUeCb *ueCb,
Bool *allow /* Valdity of this pointer is not done in this function */
)
{
   uint16_t     sfn;
   uint16_t     sfi;
   RgSchDlSf *sf = NULLP; /* Dl subframe info */
   CmLteTimingInfo timeInfo;
   CmLteTimingInfo tempTimeInfo;

   *allow = FALSE;

   timeInfo = cellCb->crntTime;
   RGSCH_INCR_SUB_FRAME(timeInfo, RG_SCH_CMN_DL_DELTA);

   DU_LOG("\nDEBUG  -->  SCH : rgSCHCmnHdFddDlSchAll (): ueId=%d ", ueCb->ueId);

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);

   /* Validate condition 1 */
   /* For (curretn time + DL_DELTA)th sf */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   if ((ueCb->hdFddCb->subfrm[sfi].sfn == sfn) &&
        (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL))
   {
      /* Uplink scheduled */
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkDlAllow: sf is UL, ueId=%d ", ueCb->ueId);
      *allow = FALSE;
      return;
   }

   /* It is not validation, but BCCH/PCCH marking is done here */
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Common channel scheduled */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi); /* NOT_HIT */
   }

   /* Validate condition 2 */
   /* For (curretn time + DL_DELTA + 1)th sf -i.e. Guard time */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
          (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL))
   {
      /* This subframe may be a switching guard time */
      DU_LOG("\nERROR  -->  SCH :  rgSCHCmnHdFddChkDlAllow: Guard time rule not met, ueId=%d ",
              ueCb->ueId);
      *allow = FALSE;
      return;
   }

   /* Validate condition 3 */
   /* For (curretn time + DL_DELTA + HRQ_DELTA)th sf - i.e. next HARQ
      Feedback occasion */

   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);

   /* First check for any Common channel info is scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(timeInfo, tempTimeInfo, RG_SCH_CMN_HARQ_INTERVAL)
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
      /* Do the marking for this subframe */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, tempTimeInfo.sfn, sfi);
      DU_LOG("\nDEBUG  -->  SCH : rgSCHCmnHdFddChkDlAllow: Possible systemInfo, ueId=%d ",
              ueCb->ueId);
   }

   /* Check for actual validation condition 3 */
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_UL)
   {
      /* No place for HARQ feedback */
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkDlAllow: No place for HARQ feedback, ueId=%d ",
            ueCb->ueId);
      *allow = FALSE;

      /* Mark this sf as DLCNTRL */
      ueCb->hdFddCb->subfrm[sfi].subFrmDir =RG_SCH_HDFDD_DLCNTRL;
      return;
   }


   /* If we are here then, subframe at HARQth location can be UL.
      But check if Guard violation is done */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL-1);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL-1);
   /* check for any Common channel info is scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(timeInfo, tempTimeInfo, (RG_SCH_CMN_HARQ_INTERVAL-1))
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
      /* Do the marking for this subframe */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, tempTimeInfo.sfn, sfi);
      DU_LOG("\nDEBUG  -->  SCH : rgSCHCmnHdFddChkDlAllow: (GT) Possible systemInfo, ueId=%d ",
              ueCb->ueId);
   }

   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA)
   {
      /* No place for HARQ feedback */
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkDlAllow: (GT) No place for HARQ feedback,"
             "ueId=%d ",ueCb->ueId);

      *allow = FALSE;
      return;
   }
   /* First check for any Common channel info is scheduled */

   *allow = TRUE;
   /* All validation done. Safe to for DL */
   return;
} /* rgSCHCmnHdFddChkDlAllow */



/* @brief Decides whether NACK can be sent in a given subrame
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkNackAllow
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars.
 *
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @return  None
 *
 */

Void rgSCHCmnHdFddChkNackAllow(RgSchCellCb *cellCb,RgSchUeCb *ueCb,CmLteTimingInfo  timeInfo,Bool *sndNACK)
{
   RgSchDlSf *sf;
   CmLteTimingInfo  tempTimeInfo;

   /* Information in timeInfo contains (n+DL_DELTA) th subframe info*/

   *sndNACK = FALSE;

   /* Determine SFN and sf index for current subframe.
      Note: Round function used as example */
   tempTimeInfo = timeInfo;
   RGSCH_INCR_SUB_FRAME(tempTimeInfo, RG_SCH_CMN_HARQ_INTERVAL);

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);

   /* Check is this subframe has any Common Channel info scheduled */
   if(RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Yes, Cannot send NACK */
      DU_LOG("\nERROR  -->  SCH : rgSCHCmnHdFddChkNackAllow: Cannot send NACK, ueId = %d ",
               ueCb->ueId);
      *sndNACK = FALSE;
   }
   else
   {
      /* safe, Send NACK */
      DU_LOG("\nDEBUG  -->  SCH : rgSCHCmnHdFddChkNackAllow: NACk can be sent, ueId = %d ",
               ueCb->ueId);
      *sndNACK = TRUE;
   }

   return;
} /* rgSCHCmnHdFddChkNackAllow */




 /* @brief makes final marking for HD-FDD UL allocations
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddUpdULMark
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars at the time of UL allocation
 *        finalization.
 *
 *     Processing Steps:
 *   
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] uint8_t *allow -- TRUE is allowed or FALSE if no allowedi.
 *             Valdity of this pointer is not done in this function.
 *
 *  @return  None
 */
Void rgSCHCmnHdFddUpdULMark(RgSchCellCb *cellCb,RgSchUeCb   *ueCb)
{

   uint16_t sfn;
   uint16_t sfi;
   CmLteTimingInfo  timeInfo;
   uint8_t   ulOffset;

   ulOffset = RGSCH_PDCCH_PUSCH_DELTA - 
              TFU_CRCIND_ULDELTA + RGSCH_PDCCH_PUSCH_DELTA;
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, ulOffset)


   /* Mark (n + UL_DELTA)th subframe as UL */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_UL, sfn, sfi);

   /* Mark (n + UL_DELTA + HARQ_DELTA)th subframe as DL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Mark (n + UL_DELTA - HARQ_DELTA)th subframe as DL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (0-RG_SCH_CMN_HARQ_INTERVAL));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (0-RG_SCH_CMN_HARQ_INTERVAL));
   if (ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_DLDATA)
   {
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);
   }

   /* Mark (n + UL_DELTA - 1)th subframe as DL_CNTRL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, -RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, -RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Remove marking for older subframes */

   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (S16)(ulOffset * -1));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (ulOffset * -1));
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_NOSCHD, RG_SCH_HDFDD_INVSFN, sfi);

   return;
} /* rgSCHCmnHdFddUpdULMark */




 /* @brief makes final marking for HD-FDD DL allocations
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddUpdDLMark
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars at the time of DL allocation
 *          finalization.
 *
 *     Processing Steps:
 *   
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] uint8_t *allow -- TRUE is allowed or FALSE if no allowed.
 *                Valdity of this pointer is not done in this function
 *
 *  @return  None
 */

Void rgSCHCmnHdFddUpdDLMark(RgSchCellCb *cellCb,RgSchUeCb *ueCb)
{

   uint16_t sfn;
   uint16_t sfi;
   CmLteTimingInfo  timeInfo;

   timeInfo = cellCb->crntTime;
   RGSCH_INCR_SUB_FRAME(timeInfo, RG_SCH_CMN_DL_DELTA);

   /* Mark (n + DL_DELTA)th subframe as DL */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi);

   /* Mark (n + 1)th subframe as DL_CNTRL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Mark (n + DL_DELTA + HARQ_DELTA )th subframe as UL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_UL, sfn, sfi);

   /* Mark (n + DL_DELTA + HARQ_DELTA - 1)th subframe
      as DL control for Guard period */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo,
          (RG_SCH_CMN_HARQ_INTERVAL - RG_SCH_HDFDD_GRDTIM_DUR));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo,
         (RG_SCH_CMN_HARQ_INTERVAL - RG_SCH_HDFDD_GRDTIM_DUR));
   if (ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_UL)
   {
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);
   }

   return;
} /* rgSCHCmnHdFddUpdDLMark */


 /* @brief determines effective SFN
 *
 * @details
 *
 *     Function : rgSCHHdFddGetSfn
 *
 *     Invoking Module Processing:
 *       Any HD-FDD module can invoke this function.
 *
 *     Processing Steps:
 *   
 *  @param[out]  *sfn uint32_t
 *  @param[in]  timeInfo timing information subframe of interest 
 *  @param[in]  offsest  Offest with w.r.t which SFN has to be determined
 *
 *  @return  None
 */

Void rgSCHHdFddGetSfn(uint16_t *sfn,CmLteTimingInfo  timeInfo,S16 offset)
{
   uint16_t tempSfn;
   S16 tempSfCount;

   if(((S16)(timeInfo.subframe) + offset) >= RGSCH_NUM_SUB_FRAMES)
   {
      /* Increament to number of times of SFNs that can be possible
         with this offset */
      tempSfn = (timeInfo.sfn +
                ((timeInfo.subframe + offset) / RGSCH_NUM_SUB_FRAMES))
                & (RGSCH_MAX_SFN -1); /* Mod with MAX SFN supported */
   }
   else
   {
      if(((S16)(timeInfo.subframe) + offset) < 0)
      {
         /* If negative, then definitely at least previous SFN */
         tempSfn = (timeInfo.sfn - 1) & (RGSCH_MAX_SFN -1);

         /* Now find how many more times we need to decreament */
         tempSfCount = timeInfo.subframe + offset;
         RG_SCH_HDFDD_ROLLSFN(tempSfCount, tempSfn);
      }
      else
      {
         /* No change in sfn */
         tempSfn = timeInfo.sfn;
      }
   }
   *sfn = tempSfn;

   return;
} /* End of rgSCHHdFddGetSfn */


#ifdef __cplusplus
}
 /* extern C */
#endif /* __cplusplus */

#endif /* LTEMAC_HDFDD */




/**********************************************************************
 
         End of file
**********************************************************************/
