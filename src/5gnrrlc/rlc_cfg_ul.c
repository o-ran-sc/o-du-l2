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

/********************************************************************20**

        Name:    NR RLC - Configuration Manager file

        Type:    C source file

        Desc:    It contains the following configuraiton primtives
                 for different actions
                    -- rlcCfgValdtEntCfg
                    -- rlcCfgFillRbCb
                    -- rlcCfgRbInit

        File:     rlc_cfg_ul.c

*********************************************************************21*/
/** @file rlc_cfg_ul.c
@brief RLC Uplink Configuration Module
**/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* RLC error options */
#include "rlc_env.h"        /* RLC environment options */

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_ul.h"
#ifdef TENB_STATS
#ifdef L2_L3_SPLIT
#include "l2_tenb_stats.x"    /* Total EnodeB Stats declarations */
#endif
#endif
S16 rlcValidateRbCfgParams (RlcCb *gCb, CmLteRnti ueId, CmLteCellId cellId,
                                  RlcEntCfgInfo *cfgToValidate, CmStatus *status);
#define RLC_MODULE RLC_DBGMASK_CFG
#ifdef LTE_L2_MEAS
/**
 *
 * @brief Handle modification of UE ID for L2 Meas data structs
 *       
 *
 *  @param[in] ueId     ue ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
static S16 rlcHdlMeasUlUeIdChg(RlcCb *gCb, uint8_t cellId,uint8_t oldUeId, uint8_t newUeId)
{
   RlcL2MeasCb    *measCb    = NULLP;
   uint16_t       cntr;
   uint16_t       ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr].measCb);

      if( measCb->measType & LKW_L2MEAS_UL_IP)
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
         {
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == oldUeId)
            {
               measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId = newUeId;
               break;
            }
         }
      }
   }
   return ROK;
}

/**
 *
 * @brief Handler to delete an UE's L2 Meas ctxt
 *       
*
 *  @param[in] ueId     ue ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
static S16 rlcDelFrmUlL2Meas(RlcCb *gCb, uint8_t cellId,uint8_t ueId)
{
   RlcL2MeasCb    *measCb    = NULLP;
   uint16_t       cntr;
   uint16_t       ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr].measCb);


      if( measCb->measType & LKW_L2MEAS_UL_IP)
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
         {
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == ueId)
            {
               measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid = FALSE;
               if (measCb->val.ipThMeas.numUes-1 == ueIdx)
               {
                  measCb->val.ipThMeas.numUes--;
               }
               break;
            }
         }



      {
         uint32_t myIdx =0;
         S16                  ret;           /* Return value */
            RlcUlUeCb             *ueCb = NULL;  

            for (myIdx = 0; myIdx < measCb->val.ipThMeas.numUes; myIdx++)
            {
               ueCb = NULL;
               ret = rlcDbmFetchUlUeCb(gCb, measCb->val.ipThMeas.ueInfoLst[myIdx].ueId, cellId, &ueCb);
            }
         }

      }
   }

   return ROK;
}


static S16 rlcAddToUlL2Meas(RlcCb *gCb, RlcUlRbCb *rlcRbCb,uint8_t cellId,uint8_t ueId)
{
   RlcL2MeasCb    *measCb    = NULLP;
   uint16_t       cntr;
   uint16_t       cntr1;
   uint16_t       ueIdx = 0;
   uint16_t       qciIdx = 0;
   uint16_t       *numQci;
   #ifndef XEON_SPECIFIC_CHANGES
   uint8_t        freeIdx = gCb->genCfg.maxUe;
   #else
   uint16_t       freeIdx = LKW_MAX_UE;
   #endif 

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measCb = &(gCb->u.ulCb->rlcL2Cb.rlcL2EvtCb[cntr].measCb);

      freeIdx = gCb->genCfg.maxUe;

      if(measCb->measType & LKW_L2MEAS_ACT_UE )
      {
         for(cntr1 =0;cntr1<measCb->val.nonIpThMeas.numQci;cntr1++)
         {
            if(measCb->val.nonIpThMeas.qci[cntr1] != rlcRbCb->qci)
            {
               measCb->val.nonIpThMeas.qci[cntr1]  = rlcRbCb->qci;
               gCb->u.ulCb->rlcL2Cb.measOn[rlcRbCb->qci] |=measCb->measType;
               break;
            }
         }
      }

      if(((rlcRbCb->rbL2Cb.measOn & measCb->measType) == LKW_L2MEAS_NONE))
      {
#ifdef LTE_L2_MEAS_RLC
         if (measCb->measType & LKW_L2MEAS_ACT_UE)
         {
            if((rlcRbCb->mode == RLC_MODE_UM) &&
                  (rlcRbCb->dir & RLC_DIR_DL ))
            {
               if (rlcRbCb->m.um.umDl.sduQ.count)
               {
                  if (rlcRbCb->ueCb->numActRb[rlcRbCb->qci] == 0)
                  {
                     rlcRbCb->ueCb->numActRb[rlcRbCb->qci]++;
                     gCb->u.ulCb->rlcL2Cb.numActUe[rlcRbCb->qci]++;
                  }
               }
            }
            else if (rlcRbCb->mode == RLC_MODE_AM)
            {
               if ((rlcRbCb->m.am.amDl.cntrlBo) ||
                     (rlcRbCb->m.am.amDl.retxBo)  ||
                     (rlcRbCb->m.am.amDl.bo))
               {
                  if (rlcRbCb->ueCb->numActRb[rlcRbCb->qci] == 0)
                  {
                     rlcRbCb->ueCb->numActRb[rlcRbCb->qci]++;
                     gCb->u.ulCb->rlcL2Cb.numActUe[rlcRbCb->qci]++;
                  }
               }
            }
         }
#endif
      }
      if((measCb->measType & LKW_L2MEAS_UL_IP))
      {

         for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
         {
            if ((freeIdx == gCb->genCfg.maxUe) && 
                  (measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid == FALSE))
            {
               freeIdx = ueIdx;
               continue;
            }
            if((measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId) == ueId)
            {
               break;
            }
         }

         if (ueIdx ==  measCb->val.ipThMeas.numUes)
         {
            if (gCb->genCfg.maxUe == measCb->val.ipThMeas.numUes)
            {
               return RFAILED;  
            }
            if (gCb->genCfg.maxUe == freeIdx)
            {               
               measCb->val.ipThMeas.numUes++;
            }
            else
            {
               ueIdx = freeIdx;
            }
            measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid = TRUE;
            memset(&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[0],0x00,(sizeof(RlcL2Cntr) *LKW_MAX_QCI));
            measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci = 0;
         }
         measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId = ueId;
         measCb->val.ipThMeas.ueInfoLst[ueIdx].cellId = cellId;
         numQci = &(measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci);

         for (qciIdx =0; qciIdx <  *numQci; qciIdx++)
         {
            if (measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] == rlcRbCb->qci)
            {
               break;
            }
         }

         if (qciIdx == *numQci)
         {
            measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] = rlcRbCb->qci;
            (*numQci)++;
         }

         rlcUtlPlcMeasDatInL2Sts(&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[rlcRbCb->qci],
               &rlcRbCb->rbL2Cb, measCb->measType);
      }
      rlcRbCb->rbL2Cb.measOn |= measCb->measType;      
   }
   return (ROK);
}/*rlcAddToDlL2Meas*/ 
#endif /*LTE_L2_MEAS*/

/** 
 * @brief
 *     This primitive fills the RbCb 
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [out]   rbCb     -  RB Control Block
 * @param [out]   ueCb     -  UE Control Block
 * @param [in]    entCfg   -  RLC Entity configuration
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
static S16 rlcCfgFillUlRbCb(RlcCb *gCb,RlcUlRbCb *rbCb,RlcUlUeCb *ueCb,RlcEntCfgInfo *entCfg)
{

   DU_LOG("\nDEBUG  -->  RLC_UL : rlcCfgFillRbCb(cellId(%d), rbId(%d), rbType(%d))",
                rbCb->rlcId.cellId, 
                entCfg->rbId, 
                entCfg->rbType);

   /* Initialize according to entMode */
   switch (entCfg->entMode)
   {
      case RLC_MODE_TM:
      {
         rbCb->lch.lChId  = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;
         break;
      }
      case RLC_MODE_UM:
      {
         rbCb->lch.lChId  = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;

         rbCb->m.umUl.snLen = entCfg->m.umInfo.ul.snLen;

         /* the bitmask for SN = 10 is 0x3ff and for SN = 5 is 0x1f */
         rbCb->m.umUl.modBitMask = (rbCb->m.umUl.umWinSz << 1) - 1; 

         rbCb->m.umUl.reAsmblTmrInt = 
            entCfg->m.umInfo.ul.reAsmblTmr;
         cmInitTimers(&(rbCb->m.umUl.reAsmblTmr), 1);
         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;
         break;
      }
      case RLC_MODE_AM:
      {
         /* Down Link Information 
          * indx = 1 as Up Link   */
         rbCb->lch.lChId  = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->dir = RLC_DIR_BOTH;

         rbCb->m.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
         rbCb->m.amUl.reAsmblTmrInt = entCfg->m.amInfo.ul.reAsmblTmr;

         rbCb->m.amUl.snLen = entCfg->m.amInfo.ul.snLen; /* 5GNR */
         
         if(RLC_AM_CFG_12BIT_SN_LEN == rbCb->m.amUl.snLen)
         {
            rbCb->m.amUl.snModMask = (1 << RLC_SN_LEN_12BITS) - 1; /* 5GNR */
         }
         else 
         {
            rbCb->m.amUl.snModMask = (1 << RLC_SN_LEN_18BITS) - 1; /* 5GNR */
         }

         cmInitTimers(&(rbCb->m.amUl.reAsmblTmr), 1);
         cmInitTimers(&(rbCb->m.amUl.staProhTmr), 1);

         rbCb->m.amUl.vrMr = rbCb->m.amUl.rxNext + (RLC_AM_GET_WIN_SZ(rbCb->m.amUl.snLen));

         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;

         break;
      }
      default:
      {
         DU_LOG("\nERROR  -->  RLC_UL : INVALID RB Mode cellId(%d), rbId(%d)",
                rbCb->rlcId.cellId, 
                entCfg->rbId); 
         return RFAILED;
      }
   }
   
   if(entCfg->snssai)
   {
      if(!rbCb->snssai)
      {
         RLC_ALLOC(gCb, rbCb->snssai, sizeof(Snssai));
         if(rbCb->snssai == NULLP)
         {
            DU_LOG("\nERROR  -->  RLC_UL : rlcCfgFillUlRbCb(): Failed to allocate memory for snssai ");
            return RFAILED;
         }
      }
      memcpy(rbCb->snssai, entCfg->snssai, sizeof(Snssai));

      /*Create the entry of this SNSSAI if not exist in Snssai Tput list*/
      if(rlcHandleSnssaiTputlist(gCb, rbCb->snssai, CREATE, DIR_UL) == NULLP)
      {
         DU_LOG("\nERROR  --> RLC_UL : rlcCfgFillUlRbCb(): SNSSAI insertion in Tput list failed");
      }
   }
   rbCb->mode = entCfg->entMode;
   
   return ROK;
} 


/** 
 * @brief This primitive Initializes the RB Cb
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [out]   rbCb     -  RB Control Block
 * @param [in]    ptr      -  Void pointer
 * @param [in]    entCfg   -  Entity Configuration
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
static S16 rlcCfgUpdateUlRb(RlcCb *gCb,RlcUlRbCb *rbCb,void *ptr,RlcEntCfgInfo *entCfg)
{
   
   switch (rbCb->mode)
   {
      case RLC_MODE_TM:
      {
         RlcUlCellCb *cellCb = (RlcUlCellCb *)ptr;
         rbCb->dir = entCfg->dir;
         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         cellCb->lCh[rbCb->lch.lChId - 1].ulRbCb = rbCb;
         break;
      }
      case RLC_MODE_UM:
      {
         RlcUlUeCb *ueCb = (RlcUlUeCb *)ptr;
         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = NULLP;
         rlcCfgFillUlRbCb(gCb,rbCb, ueCb, entCfg);
         break;
      }
      case RLC_MODE_AM:
      {
         RlcUlUeCb *ueCb = (RlcUlUeCb *)ptr;

         ueCb->lCh[rbCb->lch.lChId - 1].ulRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[1].lChId - 1].ulRbCb = rbCb;
         /* Up Link */
         rbCb->lch.lChId = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->m.amUl.staProhTmrInt = entCfg->m.amInfo.ul.staProhTmr;
         rbCb->m.amUl.reAsmblTmrInt = entCfg->m.amInfo.ul.reAsmblTmr;
         break;
      }
   }
   
   if(entCfg->snssai)
   {
      if(!rbCb->snssai)
      {
         RLC_ALLOC(gCb, rbCb->snssai, sizeof(Snssai));
         if(rbCb->snssai == NULLP)
         {
            DU_LOG("\nERROR  -->  RLC_UL : rlcCfgUpdateUlRb(): Failed to allocate memory for snssai ");
            return RFAILED;
         }
      }
      memcpy(rbCb->snssai,entCfg->snssai,sizeof(Snssai));
   }
   
   return (CKW_CFG_REAS_NONE);
} 


/** 
 * @brief 
 *    This primitive validates uplink Add RB Configuration and reserve memory
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  ueId     -  UE Identifier
 * @param [in]  cellId   -  CELL Identifier
 * @param [in]  cfgToValidate   -  Entity Configuration needs to be validated. 
 * @param [out] status  -  status of configuration
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcValidateRbCfgParams(RlcCb *gCb,CmLteRnti ueId,CmLteCellId cellId,RlcEntCfgInfo *cfgToValidate,CmStatus *status)
{
   if (cellId == 0)
   {
      status->reason = CKW_CFG_REAS_CELL_UNKWN;
      return  (RFAILED);
   }
   if ((cfgToValidate->rguSapId >= gCb->genCfg.maxRguSaps) || (cfgToValidate->rguSapId < 0))
   {
      RLCDBGP_ERROR(gCb, "rlcValidateRbCfgParams ueId (%u) cellId (%u) Invalid rguSapId (%d))\n",
            ueId, cellId, cfgToValidate->rguSapId);
      status->reason = CKW_CFG_REAS_INVALID_RGUSAP;
      return RFAILED; 
   }  
  
   if((CKW_CFG_ADD == cfgToValidate->cfgType) ||
      (CKW_CFG_MODIFY == cfgToValidate->cfgType))
   {
      /* Validate LChId for UM and AM modes */
      if ((cfgToValidate->lCh[0].lChId <= 0) ||
           ((cfgToValidate->entMode == RLC_MODE_AM) &&
             (cfgToValidate->lCh[1].lChId <= 0)))
      {
         status->reason = CKW_CFG_REAS_INVALID_LCHID;
         return RFAILED; 
      }  
      if((cfgToValidate->entMode == RLC_MODE_UM) &&
         (cfgToValidate->m.umInfo.ul.snLen != RLC_UM_CFG_6BIT_SN_LEN) &&
         (cfgToValidate->m.umInfo.ul.snLen != RLC_UM_CFG_12BIT_SN_LEN))
      {   
         DU_LOG("\nERROR  -->  RLC_UL : UM Mode RB[%d],Invalid SN Len[%d]",
               cfgToValidate->rbId,
               cfgToValidate->m.umInfo.ul.snLen);
         status->reason = CKW_CFG_REAS_INVALID_SNLEN;
         return RFAILED; 
      }
      /* Process Adding new RB */
      if (ueId == 0)
      {
         if(cfgToValidate->rbId >= RLC_MAX_RB_PER_CELL)
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            return RFAILED;
         }

         if ((cfgToValidate->lCh[0].type != CM_LTE_LCH_CCCH) &&
               (cfgToValidate->entMode != RLC_MODE_TM))
         {
            status->reason= (cfgToValidate->entMode != RLC_MODE_TM)? CKW_CFG_REAS_RB_MODE_MIS:
               CKW_CFG_REAS_LCHTYPE_MIS;
            return RFAILED;
         }
      }
      else
      {
         if (!(RLC_VALIDATE_UE_RBID(cfgToValidate->rbType, cfgToValidate->rbId)))
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            return RFAILED;
         }

         if(cfgToValidate->entMode == RLC_MODE_TM)
         {
            status->reason = CKW_CFG_REAS_LCHTYPE_MIS;
            return RFAILED;
         }
         if (!(((cfgToValidate->lCh[0].type == CM_LTE_LCH_DCCH) && 
               (cfgToValidate->entMode != RLC_MODE_UM))|| 
               (cfgToValidate->lCh[0].type == CM_LTE_LCH_DTCH)) )
         {
            status->reason = CKW_CFG_REAS_RB_MODE_MIS;
            return RFAILED;
         }
      }
   }
   else /* cfgType is CKW_CFG_DELETE */
   {
      if (ueId == 0)
      {
         if(cfgToValidate->rbId >= RLC_MAX_RB_PER_CELL)
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            return RFAILED;
         }

      }
      else
      {
         if (!(RLC_VALIDATE_UE_RBID(cfgToValidate->rbType, cfgToValidate->rbId)))
         {
            status->reason = CKW_CFG_REAS_RB_UNKWN;
            return RFAILED;
         }
      }
   }
   return ROK;
}


/** 
 * @brief 
 *    This primitive validates uplink Add RB Configuration and reserve memory
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  cfgToValidate   -  Entity Configuration needs to be validated. 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgValidateUlRb
(
RlcCb             *gCb,
RlcEntCfgInfo    *cfgToValidate,
RlcUlEntTmpData   *cfgEntData,
RlcUlCfgTmpData   *cfgTmpData 
)
{

   DU_LOG("\nDEBUG  -->  RLC_UL : cellId(%d), cfgType(%d)",
                cfgTmpData->cellId,
                cfgToValidate->cfgType);

   if(ROK != rlcValidateRbCfgParams(gCb, 
                                   cfgTmpData->ueId, 
                                   cfgTmpData->cellId, 
                                   cfgToValidate, 
                                   &cfgEntData->entUlCfgCfm.status))
   {
      return RFAILED;
   }
   
   cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_NONE;
   switch(cfgToValidate->cfgType)
   {
      case CKW_CFG_ADD:
      {
         if (cfgTmpData->ueId == 0)
         {
            /* Cell Cb is added if it not present , it is not roll backed if the
             * configuration fails */
            rlcDbmFetchUlCellCb(gCb,cfgTmpData->cellId, &cfgTmpData->cellCb);
            if(!cfgTmpData->cellCb)
            {
               /* cell cb does not exist we need to create a new one */
               RLC_ALLOC(gCb,cfgTmpData->cellCb, sizeof(RlcUlCellCb));
               if(!cfgTmpData->cellCb)
               {
                  DU_LOG("\nERROR  -->  RLC_UL : Memory allocation failure CELLID:%d",
                        cfgTmpData->cellId);
                  cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_CELL_CREAT_FAIL;
                  return RFAILED;
               }
               rlcDbmAddUlCellCb(gCb, cfgTmpData->cellId, cfgTmpData->cellCb);
            }
            else
            {
               if (( cfgTmpData->cellCb->rbCb[cfgToValidate->rbId] != NULLP))
               {
                  cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_PRSNT;
                  return RFAILED;
               }
            }

            RLC_ALLOC(gCb,cfgEntData->rbCb, sizeof (RlcUlRbCb));
            if (!cfgEntData->rbCb)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_CREAT_FAIL;
               return RFAILED;
            }
         }
         else
         {
            /* Ue Cb is added if it not present , it is not roll backed if the
             * configuration fails */
            rlcDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb);
            if(!cfgTmpData->ueCb)
            {
               RLC_ALLOC(gCb,cfgTmpData->ueCb, sizeof(RlcUlUeCb));
               if(!cfgTmpData->ueCb)
               {
                  DU_LOG("\nERROR  -->  RLC_UL : Memory allocation failure CELLID:%d",
                        cfgTmpData->cellId);
                  cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_UE_CREAT_FAIL;
                  return RFAILED;
               }
               rlcDbmAddUlUeCb(gCb, cfgTmpData->ueId, cfgTmpData->cellId, cfgTmpData->ueCb);
               cfgTmpData->ueCb->ueDeleteInfo.ueDelTmr.tmrEvnt = TMR_NONE;
            }
            else
            {
               RLC_DBM_GET_RBCB_FROM_UECB(cfgToValidate->rbId, 
                     cfgToValidate->rbType,
                     cfgTmpData->ueCb,
                     cfgEntData->rbCb);
               if(cfgEntData->rbCb != NULLP)
               {
                  cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_PRSNT;
                  return RFAILED;
               }
            }
            RLC_ALLOC(gCb,cfgEntData->rbCb, sizeof (RlcUlRbCb));
            if (!cfgEntData->rbCb)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_CREAT_FAIL;
               return RFAILED;
            }
         }
         /*Allocating the memory for receive buffer */
         if(RLC_MODE_UM == cfgToValidate->entMode)
         {
            uint16_t hashIndex;
        
            /* Spec 38.322 Section 7.2 
	     * UM_Window_Size = 32 when a 6 bit SN is configured, 
	     * UM_Window_Size = 2048 when a 12 bit SN is configured.
	     */
            cfgEntData->rbCb->m.umUl.umWinSz = RLC_POWER(2, 
                  ((cfgToValidate->m.umInfo.ul.snLen *6)-1));
            RLC_ALLOC(gCb, cfgEntData->rbCb->m.umUl.recBufLst,
              (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
            for(hashIndex = 0; hashIndex < RLC_RCV_BUF_BIN_SIZE; hashIndex++ )
            {
               cmLListInit(&(cfgEntData->rbCb->m.umUl.recBufLst[hashIndex]));
            }
         }
         else if(RLC_MODE_AM == cfgToValidate->entMode)
         {
#ifndef LTE_TDD 
             uint32_t hashIndex;
              RLC_ALLOC(gCb,
                    cfgEntData->rbCb->m.amUl.recBufLst,
                    (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp )));
              for(hashIndex = 0; hashIndex < RLC_RCV_BUF_BIN_SIZE; hashIndex++)
              {
                  cmLListInit(&(cfgEntData->rbCb->m.amUl.recBufLst[hashIndex]));
              }
#endif
         }
         break;
      }
      case CKW_CFG_MODIFY: 
      case CKW_CFG_DELETE:
      {
         if (cfgTmpData->ueId == 0)
         {
            /* Try to get the CellCb if it already exists */
            rlcDbmFetchUlCellCb(gCb,cfgTmpData->cellId, &cfgTmpData->cellCb);
            if(!cfgTmpData->cellCb)
            {
               DU_LOG("\nERROR  -->  RLC_UL : CellCb not found UEID:%d",
                      cfgTmpData->ueId);
               /*how can a modify request come for a cell which does not exist*/
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_CELL_UNKWN;
               return RFAILED;
            }

            cfgEntData->rbCb = cfgTmpData->cellCb->rbCb[cfgToValidate->rbId];
            if (!cfgEntData->rbCb)
            {
               /* something is wrong the rbId for this cell does not exist */
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_UNKWN;
               return RFAILED;
            }
            return ROK;
         }
         else
         {
            rlcDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb);
            if(!cfgTmpData->ueCb)
            {   
               DU_LOG("\nERROR  -->  RLC_UL : UeId [%d]: UeCb not found",
                     cfgTmpData->ueId);
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_UE_UNKWN;
               return RFAILED;
            }

            /* Get rbCb */
            RLC_DBM_GET_RBCB_FROM_UECB(cfgToValidate->rbId, 
                                      cfgToValidate->rbType, 
                                      cfgTmpData->ueCb, 
                                      cfgEntData->rbCb);
            if ( cfgEntData->rbCb == NULLP)
            {
               cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_UNKWN;
               return RFAILED;
            }
            return ROK;
         }
         break;
      }
   }

   if(cfgToValidate->cfgType == CKW_CFG_MODIFY)
   {
      if(cfgToValidate->entMode != cfgEntData->rbCb->mode)
      {
         cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_RB_MODE_MIS;
         return RFAILED;
      }

      if(cfgToValidate->m.umInfo.ul.snLen != cfgEntData->rbCb->m.umUl.snLen)
      {
         cfgEntData->entUlCfgCfm.status.reason = CKW_CFG_REAS_SNLEN_MIS;
         return RFAILED;
      }
   }
   return ROK;
}


/** 
 * @brief 
 *    This primitive roll back the RB Configuration
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  ueId     -  UE Identifier
 * @param [in]  cfg   -     Configuration entity. 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgRollBackUlRb
(
RlcCb             *gCb,
CmLteRnti        ueId,
RlcEntCfgInfo    *cfg,
RlcUlEntTmpData   *cfgEntData
)
{

   if(CKW_CFG_ADD == cfg->cfgType)
   {
      if(RLC_MODE_UM == cfg->entMode)
      {
         RLC_FREE(gCb, cfgEntData->rbCb->m.umUl.recBufLst, (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
      }
      else if(RLC_MODE_AM == cfg->entMode)
      {
#ifndef LTE_TDD 
      RLC_FREE(gCb,cfgEntData->rbCb->m.amUl.recBufLst, (RLC_RCV_BUF_BIN_SIZE * sizeof(CmLListCp)));
#endif
      }
      RLC_FREE(gCb,cfgEntData->rbCb, sizeof(RlcUlRbCb));
   }

   return ROK;
}


/** 
 * @brief 
 *    This primitive apply RB Configuration. 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgToAply  -   Configuration to be applied 
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
Void rlcCfgApplyUlRb
(
RlcCb             *gCb,
RlcEntCfgInfo    *cfgToAply,
RlcUlEntTmpData   *cfgEntData,
RlcUlCfgTmpData   *cfgTmpData
)
{
  
   switch(cfgToAply->cfgType)
   {
      case CKW_CFG_ADD:
      {
         /* copy the RB Cb into UeCb */
         cfgEntData->rbCb->rlcId.rbId = cfgToAply->rbId;
         if (cfgTmpData->ueId == 0)
         {
            cfgTmpData->cellCb->rbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            cfgTmpData->cellCb->lCh[cfgToAply->lCh[0].lChId -1].ulRbCb = cfgEntData->rbCb;
         }
         else
         {
            if(cfgToAply->rbType == CM_LTE_SRB)
            {
               cfgTmpData->ueCb->srbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            }
            else
            {
               cfgTmpData->ueCb->drbCb[cfgToAply->rbId] = cfgEntData->rbCb;
            }
         }

         RLC_LMM_RB_STS_INC(gCb);

         cfgEntData->rbCb->rlcId.cellId = cfgTmpData->cellId;
         cfgEntData->rbCb->rlcId.ueId   = cfgTmpData->ueId;
         cfgEntData->rbCb->rlcId.rbType = cfgToAply->rbType;
         cfgEntData->rbCb->inst         = gCb->init.inst;

         /* Fill RB CB */
         rlcCfgFillUlRbCb(gCb,
                         cfgEntData->rbCb, 
                         cfgTmpData->ueCb,
                         cfgToAply);

#ifdef LTE_L2_MEAS
         cfgEntData->rbCb->qci = cfgToAply->qci;
         cfgEntData->rbCb->ueCb =  cfgTmpData->ueCb;
         if (cfgToAply->lCh[1].type == CM_LTE_LCH_DTCH)
         {
            /* ccpu00129778 */
            rlcAddToUlL2Meas(gCb, cfgEntData->rbCb,
                  cfgTmpData->cellId,cfgTmpData->ueId); 
         }
#endif /* LTE_L2_MEAS */
         break;
      }
      case CKW_CFG_MODIFY:
      {
         if(cfgTmpData->ueId == 0)
         {
            rlcCfgUpdateUlRb(gCb,
                            cfgEntData->rbCb, 
                            (void *)cfgTmpData->cellCb, 
                            cfgToAply);
         }
         else
         {
            rlcCfgUpdateUlRb(gCb,
                            cfgEntData->rbCb, 
                            (void*)cfgTmpData->ueCb, 
                            cfgToAply);
         }
         break;
      }
      case CKW_CFG_DELETE:
      {
         if (cfgTmpData->ueId == 0)
         {
            cfgTmpData->cellCb->rbCb[cfgToAply->rbId] = NULLP;
            cfgTmpData->cellCb->lCh[cfgEntData->rbCb->lch.lChId - 1].ulRbCb = 
                                                                        NULLP;
         }
         else
         {
            cfgTmpData->ueCb->lCh[cfgEntData->rbCb->lch.lChId - 1].ulRbCb = 
                                                                       NULLP;

            /* Free the Buffers of RbCb */
            if( RLC_MODE_UM == cfgEntData->rbCb->mode )
            {
               rlcUmmFreeUlRbCb(gCb, cfgEntData->rbCb);
            }
            else if(RLC_MODE_AM == cfgEntData->rbCb->mode)
            {
               rlcAmmFreeUlRbCb(gCb,cfgEntData->rbCb);
            }

            /* Assign NULLP to rbCb in rbCbLst */
            if ( cfgToAply->rbType == CM_LTE_SRB )
            {
               cfgTmpData->ueCb->srbCb[cfgToAply->rbId] = NULLP;
            }
            else
            {
               cfgTmpData->ueCb->drbCb[cfgToAply->rbId] = NULLP;
            }
         }
         /* Delete RbCb  */
         RLC_FREE(gCb,cfgEntData->rbCb, sizeof(RlcUlRbCb));

         RLC_LMM_RB_STS_DEC(gCb);
         break;
      }
   }
   return;
}


/** 
 * @brief 
 *    This primitive validates uplink Delete UE request 
 *    for RB creation.
 *  
 * @param [in]  gCb      -  RLC Instance Control Block
 * @param [in]  cfgToValidate   -  Entity Configuration to be validated.
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 * @param [out] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgValidateDelUlUe
(
RlcCb             *gCb,
RlcEntCfgInfo    *cfgToValidate,
RlcUlEntTmpData   *cfgEntData,
RlcUlCfgTmpData   *cfgTmpData
)
{

    DU_LOG("\nDEBUG  -->  RLC_UL : cellId(%d), cfgType(%d)",
              cfgTmpData->cellId,cfgToValidate->cfgType);
   
   /* Check for ueId is present or not */
   if ( cfgTmpData->ueId == 0 )
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_UE_UNKWN;
      DU_LOG("\nERROR  -->  RLC_UL : UeId is 0 for CELLID;%d",
            cfgTmpData->cellId);
      return RFAILED;
   }

   /* Fetch Ue Cb */
   if(ROK != rlcDbmFetchUlUeCb(gCb,cfgTmpData->ueId, cfgTmpData->cellId, &cfgTmpData->ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : UeId [%d]: UeCb not found",
            cfgTmpData->ueId);
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_UE_UNKWN;
      return RFAILED;
   }
   return ROK;
}


/** 
 * @brief 
 *    This primitive apply Uplink Delete Ue request 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 */
Void rlcCfgApplyDelUlUe
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfgTmpData
)
{
   
#ifdef LTE_L2_MEAS
   rlcDelFrmUlL2Meas(gCb,cfgTmpData->cellId,cfgTmpData->ueId);
#endif
   rlcDbmDelUlUeCb(gCb,cfgTmpData->ueCb, FALSE);
   return;
}
  

/** 
 * @brief 
 *    This primitive validates uplink Delete UE request 
 *    for RB creation.
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] ueId     -  UE Identifier
 * @param [in] cfgToValidate   -  Entity Configuration to be done.
 * @param [in] cfgEntData    -  Configuration Temporary Data Entity
 * @param [in] cfgTmpData    -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgValidateDelUlCell
(
RlcCb             *gCb,
CmLteCellId      cellId,
RlcEntCfgInfo    *cfgToValidate,
RlcUlEntTmpData   *cfgEntData,
RlcUlCfgTmpData   *cfgTmpData
)
{

   DU_LOG("\nDEBUG  -->  RLC_UL : cfgType(%d)",
              cfgToValidate->cfgType);
   
   cfgTmpData->cellCb = NULLP;

   /* Check for cellId is present or not */
   if ( cellId == 0 )
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_CELL_UNKWN;
      DU_LOG("\nERROR  -->  RLC_UL : CellId is 0");
      return RFAILED;
   }

   /* Fetch Cell Cb */
   rlcDbmFetchUlCellCb(gCb,cellId, &cfgTmpData->cellCb);
   if (!cfgTmpData->cellCb)
   {
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_CELL_UNKWN;
      DU_LOG("\nERROR  -->  RLC_UL : CellCb not found");
      return RFAILED;
   }
   return ROK;
}


/** 
 * @brief 
 *    This primitive apply Uplink Delete Ue request 
 *  
 * @param [in] gCb      -  RLC Instance Control Block
 * @param [in] cfgEntData  - Temporary Data maintained for a transaction 
 *
 * @return  S16
 *    -# ROK
 */
Void rlcCfgApplyDelUlCell(RlcCb *gCb,RlcUlCfgTmpData *cfgInfo)
{
   
   rlcDbmDelUlCellCb(gCb,cfgInfo->cellCb);
   return;
}

/** 
 * @brief 
 *    This primitive validates reestablishment of RB.
 *  
 * @param [in]   gCb      -  RLC Instance Control Block
 * @param [in]   ueId     -  UE Identifier
 * @param [in]   cellId   -  CELL Identifier
 * @param [in]   cfgToValidate   -  Entity Configuration to be done.
 * @param [out] cfgEntData    -  Configuration Temporary Data Entity
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgValidateReEstRb
(
RlcCb             *gCb,
CmLteRnti        ueId,
CmLteCellId      cellId,
RlcEntCfgInfo    *cfgToValidate,
RlcUlEntTmpData   *cfgEntData
)
{
   CmLteRlcId   rlcId;   /* RLC Identifier */

    DU_LOG("\nDEBUG  -->  RLC_UL : cellId(%d) RBID:%d",
         cellId,
         cfgToValidate->rbId); 

   /* Get rlcId */
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = cfgToValidate->rbId;
   rlcId.rbType = cfgToValidate->rbType;

   rlcDbmFetchUlRbCbByRbId(gCb,&rlcId, &cfgEntData->rbCb);
   if (cfgEntData->rbCb == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC_UL : CellId [%u]: rbId [%d] not found",
             cellId,
             cfgToValidate->rbId);
      cfgEntData->entUlCfgCfm.status.reason =  CKW_CFG_REAS_RB_UNKWN;
      return RFAILED;
   }

   return ROK;
}



/** 
 * @brief 
 *    This primitive apply reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueId       -  UE Identifier
 * @param [in] cellId     -  CELL Identifier
 * @param [in] sndReEstInd  -  ReEstablish Indication Flag 
 * @param [in] cfgEntData -  Configuration Temporary Data Entity 
 *
 * @return  S16
 *    -# ROK
 */
Void rlcCfgApplyReEstUlRb
(
RlcCb           *gCb,
CmLteRnti       ueId,
CmLteCellId     cellId,
Bool            sndReEstInd,
RlcUlEntTmpData *cfgEntData
)
{
   CmLteRlcId   rlcId;   /* RLC Identifier */
   
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = cfgEntData->entUlCfgCfm.rbId;
   rlcId.rbType = cfgEntData->entUlCfgCfm.rbType;
   cfgEntData->rbCb->rlcId.ueId = ueId;
   switch (cfgEntData->rbCb->mode)
   {
      case RLC_MODE_TM:
         {
            rlcTmmUlReEstablish(gCb,cfgEntData->rbCb);
            break;
         }

      case RLC_MODE_UM:
         {
            rlcUmmUlReEstablish(gCb,&rlcId,cfgEntData->rbCb);
            break;
         }

      case RLC_MODE_AM:
         {
            rlcAmmUlReEstablish(gCb,rlcId,sndReEstInd,cfgEntData->rbCb);
            break;
         }
   }
   return;
}

/** 
 * @brief 
 *    This primitive validates reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueInfo     -  UE Identifier
 * @param [in] newUeInfo  -  CELL Identifier
 * @param [in] cfgTmpData -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgValidateUeIdChng
(
RlcCb            *gCb,
CkwUeInfo        *ueInfo,
CkwUeInfo        *newUeInfo,
RlcUlCfgTmpData  *cfgTmpData
)
{
   RlcUlUeCb   *ueCb;

#define CFM_STATUS  cfgTmpData->cfgEntData[0].entUlCfgCfm.status
   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      CFM_STATUS.reason = CKW_CFG_REAS_SAME_UEID;
      CFM_STATUS.status = CKW_CFG_CFM_NOK; 
      return RFAILED;
   } 
   
   if(ROK == rlcDbmFetchUlUeCb(gCb,newUeInfo->ueId, newUeInfo->cellId, &ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : NewUeId[%d]:ueCb already exists",
             newUeInfo->ueId);
      CFM_STATUS.reason = CKW_CFG_REAS_UE_EXISTS;
      CFM_STATUS.status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }
  
   if(ROK != rlcDbmFetchUlUeCb(gCb,ueInfo->ueId, ueInfo->cellId, 
                              &cfgTmpData->ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : UeId [%d]: UeCb not found",
            ueInfo->ueId);
      CFM_STATUS.reason = CKW_CFG_REAS_UE_UNKWN;
      CFM_STATUS.status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }
#undef CFM_STATUS
   return ROK;
}



/** 
 * @brief 
 *    This primitive apply reestablishment of RB.
 *  
 * @param [in] gCb        -  RLC Instance Control Block
 * @param [in] ueInfo     -  UE Identifier
 * @param [in] newUeInfo  -  CELL Identifier
 * @param [in] cfgTmpData -  Configuration Temporary Data 
 *
 * @return  S16
 *    -# ROK
 */
Void rlcCfgApplyUlUeIdChng
(
RlcCb            *gCb,
CkwUeInfo        *ueInfo,
CkwUeInfo        *newUeInfo,
RlcUlCfgTmpData  *cfgTmpData
)
{
   
#ifdef LTE_L2_MEAS
   rlcHdlMeasUlUeIdChg(gCb, ueInfo->cellId, ueInfo->ueId, newUeInfo->ueId);
#endif   
   cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) cfgTmpData->ueCb);
   /* update the hash key with new values */ 
   cfgTmpData->ueCb->ueId = newUeInfo->ueId;
   cfgTmpData->ueCb->cellId =newUeInfo->cellId;
   if(ROK != cmHashListInsert(&(gCb->u.ulCb->ueLstCp),
                              (PTR)cfgTmpData->ueCb, 
                              (uint8_t *)&(cfgTmpData->ueCb->ueId),
                              (uint16_t) sizeof(CmLteRnti)))

   {
       DU_LOG("\nERROR  -->  RLC_UL : UeId[%u] HashList Insertion Failed",
            newUeInfo->ueId);
   }

   return;
}
/********************************************************************30**

         End of file
**********************************************************************/
