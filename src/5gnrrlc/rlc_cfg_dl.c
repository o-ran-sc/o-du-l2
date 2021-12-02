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
                    -- rlcCfgAddRb
                    -- rlcCfgReCfgRb
                    -- rlcCfgDelRb
                    -- rlcCfgReEstRb
                    -- rlcCfgDelUe

        File:    rlc_cfg_dl.c

*********************************************************************21*/

/** @file rlc_cfg_dl.c
@brief RLC Downlink Configuration Module
**/


/* header (.h) include files */
#include "common_def.h"
#include "math.h"
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
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"
#include "du_app_rlc_inf.h"

#define RLC_MODULE RLC_DBGMASK_CFG
/*Added for adding new Ue in onging L2 Meas*/
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
static S16 rlcHdlMeasDlUeIdChg(RlcCb *gCb, uint8_t cellId,uint8_t oldUeId, uint8_t newUeId)
{
   RlcL2MeasEvtCb *measEvtCb = NULLP;
   RlcL2MeasCb    *measCb    = NULLP;
   uint16_t       cntr;
   uint16_t       ueIdx = 0;     

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr]);
      measCb = &(measEvtCb->measCb);


      if(measCb->measType & LKW_L2MEAS_DL_IP ) 
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
static S16 rlcDelFrmDlL2Meas(RlcCb *gCb, uint8_t cellId,uint8_t ueId)
{
   RlcL2MeasEvtCb *measEvtCb = NULLP;
   RlcL2MeasCb    *measCb    = NULLP;
   uint16_t       cntr;
   uint16_t       ueIdx = 0;     


   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr];
      measCb = &(measEvtCb->measCb);


      if(measCb->measType & LKW_L2MEAS_DL_IP )
      {

         for(ueIdx = 0; ((ueIdx < measCb->val.ipThMeas.numUes) &&
                       (ueIdx < gCb->genCfg.maxUe)); ueIdx++)
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
      }
   }

   return ROK;
}


static S16 rlcAddToDlL2Meas(RlcCb *gCb, RlcDlRbCb *rlcRbCb,uint8_t cellId,uint8_t ueId)
{
   RlcL2MeasEvtCb *measEvtCb = NULLP;
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
      measEvtCb = &gCb->u.dlCb->rlcL2Cb.rlcL2EvtCb[cntr];
      measCb = &(measEvtCb->measCb);

      freeIdx = gCb->genCfg.maxUe;

      if(measCb->measType & 
          (LKW_L2MEAS_ACT_UE | LKW_L2MEAS_UU_LOSS | LKW_L2MEAS_DL_DELAY))
      {
         for(cntr1 =0;((cntr1 < measCb->val.nonIpThMeas.numQci) &&
                       (cntr1 < LKW_MAX_QCI));cntr1++)
         {
            if(measCb->val.nonIpThMeas.qci[cntr1] != rlcRbCb->qci)
            {
               measCb->val.nonIpThMeas.qci[cntr1]  = rlcRbCb->qci;
               gCb->u.dlCb->rlcL2Cb.measOn[rlcRbCb->qci] |=measCb->measType;
               break;
            }
         }
      }

      if(((rlcRbCb->rbL2Cb.measOn & measCb->measType) == LKW_L2MEAS_NONE))
      {
         if (measCb->measType & LKW_L2MEAS_ACT_UE)
         {
            if((rlcRbCb->mode == RLC_MODE_UM) &&
                  (rlcRbCb->dir & RLC_DIR_DL ))
            {
               if (rlcRbCb->m.umDl.sduQ.count)
               {
                  if (rlcRbCb->ueCb->numActRb[rlcRbCb->qci] == 0)
                  {
                     rlcRbCb->ueCb->numActRb[rlcRbCb->qci]++;
                     gCb->u.dlCb->rlcL2Cb.numActUe[rlcRbCb->qci]++;
                  }
               }
            }
            else if (rlcRbCb->mode == RLC_MODE_AM)
            {
               if ((rlcRbCb->m.amDl.cntrlBo) ||
                     (rlcRbCb->m.amDl.retxBo)  ||
                     (rlcRbCb->m.amDl.bo))
               {
                  if (rlcRbCb->ueCb->numActRb[rlcRbCb->qci] == 0)
                  {
                     rlcRbCb->ueCb->numActRb[rlcRbCb->qci]++;
                     gCb->u.dlCb->rlcL2Cb.numActUe[rlcRbCb->qci]++;
                  }
               }
            }
         }
      }
      if((measCb->measType & LKW_L2MEAS_DL_IP))
      {

         for(ueIdx = 0; ((ueIdx < measCb->val.ipThMeas.numUes) &&
                         (ueIdx < gCb->genCfg.maxUe)); ueIdx++)
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

         for (qciIdx =0; ((qciIdx <  *numQci) &&
                           (qciIdx < LKW_MAX_QCI)) ; qciIdx++)
         {
            if (measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] == rlcRbCb->qci)
            {
               break;
            }
         }

         /* Fix Klock Warning */
         if ((qciIdx == *numQci) && (qciIdx < LKW_MAX_QCI))
         {
            measCb->val.ipThMeas.ueInfoLst[ueIdx].qci[qciIdx] = rlcRbCb->qci;
            (*numQci)++;
         }

         rlcUtlPlcMeasDatInL2Sts(&measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[rlcRbCb->qci],
               &rlcRbCb->rbL2Cb, measCb->measType);
      }
      else if (measCb->measType & 
         (LKW_L2MEAS_DL_DISC | LKW_L2MEAS_DL_DELAY | LKW_L2MEAS_UU_LOSS))
      {
         rlcUtlPlcMeasDatInL2Sts(&measCb->val.nonIpThMeas.measData[rlcRbCb->qci],
               &rlcRbCb->rbL2Cb, measCb->measType);
         measCb->val.nonIpThMeas.qci[rlcRbCb->qci] = rlcRbCb->qci;
         measCb->val.nonIpThMeas.measData[rlcRbCb->qci].totDrbsPerQci++;
      }
      rlcRbCb->rbL2Cb.measOn |= measCb->measType;      
   }
   return ROK;
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
static S16 rlcCfgFillDlRbCb(RlcCb *gCb,RlcDlRbCb *rbCb,RlcDlUeCb *ueCb,RlcEntCfgInfo *entCfg)
{
   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgFillRbCb(ueId(%d),cellId(%d) rbType(%d))",
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId, 
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

            /* Spec 38.322 Section 7.1 
             * All UM state variables can take values from 0 to 63 for 6 bit SN or 
             * from 0 to 4095 for 12 bit SN. All arithmetic operations on UM state 
             * variables are affected by the UM modulus 
             * (i.e. final value = [value from arithmetic operation] modulo 64
             * for 6 bit SN and 4096 for 12 bit SN)
             */
            rbCb->m.umDl.snLen = entCfg->m.umInfo.dl.snLen;
            if (entCfg->m.umInfo.dl.snLen == RLC_UM_CFG_6BIT_SN_LEN)
               rbCb->m.umDl.modBitMask = 0x3f;
            else
               rbCb->m.umDl.modBitMask = 0xfff;

            ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;

            break;
         }
      case RLC_MODE_AM:
         {
            /* Down Link Information 
             * indx = 0 as Down Link   */
            rbCb->lch.lChId  = entCfg->lCh[0].lChId;
            rbCb->lch.lChType = entCfg->lCh[0].type;
            rbCb->dir = RLC_DIR_BOTH;

            rbCb->m.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
            rbCb->m.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
            rbCb->m.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;
            rbCb->m.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
            rbCb->m.amDl.snLen = entCfg->m.amInfo.dl.snLen;

            if(RLC_AM_CFG_12BIT_SN_LEN == rbCb->m.amDl.snLen)
            {
               rbCb->m.amDl.snModMask = (1 << RLC_SN_LEN_12BITS) - 1; /* 5GNR */
            }
            else 
            {
               rbCb->m.amDl.snModMask = (1 << RLC_SN_LEN_18BITS) - 1; /* 5GNR */
            }

            cmInitTimers(&(rbCb->m.amDl.pollRetxTmr), 1);
            ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;

#ifndef LTE_TDD 
            uint32_t hashIndex;
            RLC_ALLOC(gCb,
                  rbCb->m.amDl.txBufLst,
                  (RLC_TX_BUF_BIN_SIZE * sizeof(CmLListCp)));
            for(hashIndex = 0; hashIndex < RLC_TX_BUF_BIN_SIZE; hashIndex++)
            {
               cmLListInit(&(rbCb->m.amDl.txBufLst[hashIndex]));
            }
#endif
            break;
         }
      default:
         {
            DU_LOG("\nERROR  -->  RLC_DL : Invalid RB Mode ueId(%d),cellId(%d)",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
            return RFAILED;
         }
   }

   if(entCfg->snssai)
   {
      RLC_ALLOC(gCb, rbCb->snssai, sizeof(Snssai));
      if(rbCb->snssai == NULLP)
      {
         DU_LOG("\nERROR  --> RLC_DL : rlcCfgFillDlRbCb(): Failed to allocate memory");
         return RFAILED;
      }
      memcpy(rbCb->snssai, entCfg->snssai, sizeof(Snssai));

      /*Create the entry of this SNSSAI if not exist in Snssai Tput list*/
      if(rlcHandleSnssaiTputlist(gCb, rbCb->snssai, CREATE, DIR_DL) == NULLP)
      {
         DU_LOG("\nERROR  --> RLC_DL : rlcCfgFillDlRbCb(): SNSSAI insertion in Tput list failed");
      }
   }
   rbCb->mode = entCfg->entMode;
   rbCb->discTmrInt = entCfg->discardTmr;

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
static S16 rlcCfgUpdateDlRb
(
RlcCb           *gCb,
RlcDlRbCb       *rbCb,
void            *ptr,
RlcEntCfgInfo   *entCfg
)
{
   
   if (rbCb->mode != entCfg->entMode)
   {
      DU_LOG("\nERROR  -->  RLC_DL : RB Mode Mismatch : exp [%d] rcv [%d] UEID:%d CELLID:%d", 
            rbCb->mode, 
            entCfg->entMode,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
      return (CKW_CFG_REAS_RB_MODE_MIS);
   }

   switch (rbCb->mode)
   {
      case RLC_MODE_TM:
      {
         RlcDlCellCb *cellCb = (RlcDlCellCb *)ptr;

         rbCb->dir = entCfg->dir;
         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         cellCb->lCh[rbCb->lch.lChId - 1].dlRbCb = rbCb;
         break;
      }

      case RLC_MODE_UM:
      {
         RlcDlUeCb *ueCb = (RlcDlUeCb *)ptr;

         if (entCfg->lCh[0].type == CM_LTE_LCH_DCCH)
         {
            return (CKW_CFG_REAS_LCHTYPE_MIS);
         }
         
         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = rbCb;

         rbCb->lch.lChId = entCfg->lCh[0].lChId;
         rbCb->lch.lChType = entCfg->lCh[0].type;
         rbCb->dir = entCfg->dir;
         break;
      }

      case RLC_MODE_AM:
      {
         RlcDlUeCb *ueCb = (RlcDlUeCb *)ptr;

         ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = NULLP;
         ueCb->lCh[entCfg->lCh[1].lChId - 1].dlRbCb = rbCb;
         
         /* Down Link */
         rbCb->lch.lChId = entCfg->lCh[1].lChId;
         rbCb->lch.lChType = entCfg->lCh[1].type;
         rbCb->m.amDl.pollRetxTmrInt = entCfg->m.amInfo.dl.pollRetxTmr;
         rbCb->m.amDl.pollPdu = entCfg->m.amInfo.dl.pollPdu;
         rbCb->m.amDl.pollByte = entCfg->m.amInfo.dl.pollByte;
         rbCb->m.amDl.maxRetx = entCfg->m.amInfo.dl.maxRetx;
         
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
            DU_LOG("\nERROR  --> RLC_DL : rlcCfgFillDlRbCb(): Failed to allocate memory");
            return RFAILED;
         }
      }
      memcpy(rbCb->snssai,entCfg->snssai,sizeof(Snssai));
   }
/* AGHOSH */
   rbCb->discTmrInt = entCfg->discardTmr;
/* AGHOSH */
   return (CKW_CFG_REAS_NONE);
} 


/** 
 * @brief 
 *    This primitive adds new RB in Ue/Cell Cb.
 *
 * @details
 *    This function does following steps -
 *    - If UE ID is 0 then
 *    - Check for CELL CB is present
 *    - If yes, Check for RB ID
 *       - If RB ID is present Status Indication with reason
 *       - Else, Create New RB CB in CELL CB
 *    - If no Create New CELL CB and RB CB
 *    - Else,
 *       - Check for UE CB is present
 *       - If yes Check for RB ID
 *          - If RB ID is present Status Indication with reason
 *          - Else, Create New RB CB in UE CB
 *          - If no Create New UE CB and RB CB
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation.
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgAddDlRb
(
RlcCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   RlcDlUeCb     *ueCb = NULLP;   /* UE Control Block */
   RlcDlCellCb   *cellCb;         /* Cell Control Block */
   RlcDlRbCb     *rlcRbCb;         /* KW RB Control Block */
   uint8_t       reason;          /* Rb Identifier */

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgAddRb(cellId(%d),UEID:%d cfgType(%d))",
         cellId, 
         ueId,
         entCfg->cfgType);

   if (cellId == 0)
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_CELL_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : Add DLRb,CellId is 0 for UEID:%d",
            ueId);
      return RFAILED;
   }
   if ((entCfg->rguSapId >= gCb->genCfg.maxRguSaps) || (entCfg->rguSapId < 0))
   {
      RLCDBGP_ERROR(gCb, "rlcCfgAddDlRb(ueId(%u), cellId(%u), Invalid rguSapId (%d)\n",
            ueId, cellId, entCfg->rguSapId);
      return RFAILED; 
   }


   /* Process Adding new RB */
   if (ueId == 0)
   {
      if(entCfg->rbId >= RLC_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
               CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RbId ,Max is [%d] CELLID:%d UEID:%d",
               RLC_MAX_RB_PER_CELL,
               cellId,
               ueId);
         return RFAILED;
      }

      if (((entCfg->lCh[0].type == CM_LTE_LCH_BCCH) || 
               (entCfg->lCh[0].type == CM_LTE_LCH_PCCH) ||
               (entCfg->lCh[0].type == CM_LTE_LCH_CCCH)) &&
            (entCfg->entMode == RLC_MODE_TM))
      {
         /* Cell CB present */
         rlcDbmFetchDlCellCb(gCb, cellId, &cellCb);
         if(cellCb)
         {
            /* Get rbCb from cellCb->rbCb List */
            if (( cellCb->rbCb[entCfg->rbId] != NULLP))
            {
               /* Fill entCfm structure */
               RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                     CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_RB_PRSNT);
               DU_LOG("\nERROR  -->  RLC_DL : RbId [%d] already exists UEID:%d",
                     entCfg->rbId,
                     ueId);
               return RFAILED;
            }
         }
         else  /* Cell CB UNKNOWN */
         {
            /* Create CELL CB */
            if ( ROK != rlcDbmCreateDlCellCb(gCb,cellId, &cellCb))
            {
               /* Fill entCfm structure */
               RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                     CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_CELL_CREAT_FAIL);
               DU_LOG("\nERROR  -->  RLC_DL : cellCb Creation failed RBID:%d UEID:%d",
                     entCfg->rbId,
                     ueId);
               return RFAILED;
            }
         }

         /* Validate LChId */
         if(entCfg->lCh[0].lChId <= 0)
         {
            DU_LOG("\nERROR  -->  RLC_DL : Invalid LcId CELLID:%d UEID:%d RBID:%d",
                  cellId,
                  ueId,
                  entCfg->rbId);
            /* Fill entCfm structure */                               
            RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_LCHID);           
            return RFAILED;                                        
         }                                                            

         /* Create RB CB */
         RLC_ALLOC(gCb,rlcRbCb, sizeof (RlcDlRbCb));
         if (!rlcRbCb)
         {
            DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed for rbId:%d CELLID:%d",
                  entCfg->rbId,
                  ueId);
            /* Fill entCfm structure */                           
            RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_CREAT_FAIL); 
            return RFAILED; 
         }
         rlcRbCb->rlcId.rbId = entCfg->rbId;
         cellCb->rbCb[entCfg->rbId] = rlcRbCb;
         RLC_LMM_RB_STS_INC(gCb);
         cellCb->lCh[entCfg->lCh[0].lChId - 1].dlRbCb = rlcRbCb;
      }
      else
      {
         reason= (entCfg->entMode != RLC_MODE_TM)? CKW_CFG_REAS_RB_MODE_MIS:
            CKW_CFG_REAS_LCHTYPE_MIS;
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
               CKW_CFG_CFM_NOK, reason);
         return RFAILED;
      }
   }
   else
   {
      if (!(RLC_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RbId for RbType[%d] UEID:%d", 
               entCfg->rbType,
               ueId);
         return RFAILED;
      }
      if ((((entCfg->lCh[0].type == CM_LTE_LCH_DCCH) && 
                  (entCfg->entMode != RLC_MODE_UM) && 
                  (CM_LTE_SRB == entCfg->rbType)) ||
               ((entCfg->lCh[0].type == CM_LTE_LCH_DTCH) && 
                (CM_LTE_DRB == entCfg->rbType))) &&
            (entCfg->entMode != RLC_MODE_TM))
      {
         /* UE CB present */
         if ( rlcDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb) == ROK)
         {
            /* Get rbCb from ueCb->rbCb list */
            RLC_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, rlcRbCb);

            if(( rlcRbCb != NULLP))
            {
               /* Fill entCfm structure */
               RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_RB_PRSNT);
               DU_LOG("\nERROR  -->  RLC_DL : CellId[%u]:rbId [%d] already exists",
                     cellId,
                     entCfg->rbId);
               return RFAILED;
            }
         }
         else  /* UE CB UNKNOWN */
         {
            /* Create UE CB */
            if ( rlcDbmCreateDlUeCb(gCb,ueId, cellId, &ueCb) != ROK)
            {
               /* Fill entCfm structure */
               RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                     CKW_CFG_REAS_UE_CREAT_FAIL);
               DU_LOG("\nERROR  -->  RLC_DL : UeId [%u]:ueCb Creation Failed RBID:%d",
                     ueId,
                     entCfg->rbId);
               return RFAILED;
            }
            /* Start throughput calculation for this UE */
            gCb->rlcThpt.ueTputInfo.thptPerUe[ueId -1].ueId  = ueId;
            gCb->rlcThpt.ueTputInfo.thptPerUe[ueId -1].dataVol = 0;
            gCb->rlcThpt.ueTputInfo.numActvUe++;
         }

         /* Validate LChId for UM and AM modes */
         if ((entCfg->lCh[0].lChId <= 0) ||
               ((entCfg->entMode == RLC_MODE_AM)&&
                (entCfg->lCh[1].lChId <= 0)))
         {
            /* Fill entCfm structure */                               
            RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                  CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_LCHID);           
            return RFAILED;                                        
         }                                                            

         /* Create RB CB */
         RLC_ALLOC(gCb,rlcRbCb, sizeof (RlcDlRbCb));
         if (rlcRbCb == NULL)
         {
            /* Fill entCfm structure */                           
            RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,CKW_CFG_CFM_NOK,
                  CKW_CFG_REAS_RB_CREAT_FAIL); 
            DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed RBID:%d CELLID:%d",
                  entCfg->rbId,
                  cellId);
            return RFAILED; 
         }

         /* copy the RB Cb into UECb */
         rlcRbCb->rlcId.rbId = entCfg->rbId;
         if(entCfg->rbType == CM_LTE_SRB)
            ueCb->srbCb[entCfg->rbId] = rlcRbCb;
         else
            ueCb->drbCb[entCfg->rbId] = rlcRbCb;

         RLC_LMM_RB_STS_INC(gCb);

      }
      else
      {
         if (entCfg->entMode == RLC_MODE_TM)
         {
            reason = CKW_CFG_REAS_RB_MODE_MIS;
         }
         else
         {
            reason = CKW_CFG_REAS_LCHTYPE_MIS;
         }

         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK, reason);
         return RFAILED;
      }
   }


   rlcRbCb->rlcId.cellId = cellId;
   rlcRbCb->rlcId.ueId   = ueId;
   rlcRbCb->rlcId.rbType = entCfg->rbType;
   rlcRbCb->inst         = gCb->init.inst;
#ifdef TENB_MULT_CELL_SUPPRT
   rlcRbCb->rguSapId     = entCfg->rguSapId;
#endif


   /* Fill RB CB */
   if (rlcCfgFillDlRbCb(gCb,rlcRbCb, ueCb, entCfg) != ROK)
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_RB_CREAT_FAIL);

      /* Delete RB CB created */
      RLC_FREE(gCb,rlcRbCb, sizeof(RlcDlRbCb));
      DU_LOG("\nERROR  -->  RLC_DL : Filling of RbCb failed UEID:%d CELLID:%d",
            ueId,
            cellId);
      return RFAILED;
   }
   rlcRbCb->qci = entCfg->qci;
#ifdef LTE_L2_MEAS
   rlcRbCb->ueCb =  ueCb;
   if (entCfg->lCh[0].type == CM_LTE_LCH_DTCH)
   {
      /* ccpu00129778 */
      rlcAddToDlL2Meas(gCb, rlcRbCb,cellId,ueId); 
   }
#endif /* LTE_L2_MEAS */

   /* Fill entCfm structure */
   RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   return ROK;
} 


/** 
 * @brief
 *    This primitive reconfigures the existing RB in Ue/Cell Cb.
 *
 * @details
 *    This primitive executes following steps in reconfiguration of existing
 *    RB -
 *    - If UE ID is 0 then
 *       - Check for CELL CB is present
 *       - If yes, Check for RB ID
 *          - If RB ID is present Reconfigure the RB CB
 *          - Else, Status Indication with Reason
 *       - Else, Status Indication with Reason
 *    - Else,
 *       - Check for UE CB is present
 *       - If yes, Check for RB ID
 *          - If RB ID is prenset Reconfigure the CELL CB
 *          - Else, Status Indication with Reason
 *       - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
S16 rlcCfgReCfgDlRb
(
RlcCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   RlcDlRbCb     *rbCb;     /* RB Control Block */
   RlcDlRbCb     tRbCb;     /* KW RB Control Block */
   RlcDlCellCb   *cellCb;   /* Cell Control Block */
   RlcDlUeCb     *ueCb;     /* Ue Control Block */
   uint8_t       ret;

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgReCfgRb(cellId(%d), cfgType(%d)) RBID:%d",
            cellId, entCfg->cfgType,entCfg->rbId);


   /* Check for UeCb or CellCb */
   if (ueId == 0)
   { 
      if(entCfg->rbId >= RLC_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                             CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RbId , Max is [%d] UEID:%d CELLID:%d",
                  RLC_MAX_RB_PER_CELL,
                  ueId,
                  cellId);
         return RFAILED;
      }
      /* Get cellCb */
      rlcDbmFetchDlCellCb(gCb,cellId, &cellCb);
      if(!cellCb)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
                             CKW_CFG_REAS_CELL_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CellCb not found ueId:%d RBID:%d CELLID:%d",
                  ueId,
                  entCfg->rbId,
                  cellId);
         return RFAILED;
      }

      /* Get rbCb */
      RLC_DBM_GET_CELL_RBCB(entCfg->rbId, cellCb->rbCb, rbCb);

      if (!rbCb)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CELLID:%d RBID:%d not found",
                  cellId,
                  entCfg->rbId);
         return RFAILED;
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      memcpy(&tRbCb, rbCb, sizeof(RlcDlRbCb));

      /* Update rbCb */
      ret = rlcCfgUpdateDlRb(gCb,rbCb, cellCb,entCfg);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, 
                             entCfg->rbId, 
                             entCfg->rbType, 
                             CKW_CFG_CFM_NOK,
                             ret);

         DU_LOG("\nERROR  -->  RLC_DL : CELLID:%u RBID:%d updation failed",
                  cellId,
                  entCfg->rbId);
         memcpy(rbCb, &tRbCb, sizeof(RlcDlRbCb));

         return (ret);
      }
   }
   else
   {
      if (!(RLC_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CELLID:%d Invalid RBID:%d for RbType[%d]",
                  cellId,
                  entCfg->rbId,
                  entCfg->rbType);
         return RFAILED;
      }
      /* Get ueCb */
      ret = rlcDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : UEID:%d UeCb not found RBID:%d",
                  ueId,
                  entCfg->rbId);
         return (ret);
      }

      /* Get rbCb */
      RLC_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, rbCb);

      if ( rbCb == NULLP)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CELLID:%d RBID:%d not found",
                  cellId,
                  entCfg->rbId);
         return (ret);
      }

      /* Take backup of rbCb before updating.
       * Because in failure case restore original rbCb
       */
      memcpy(&tRbCb, rbCb, sizeof(RlcDlRbCb));

      /* Update rbCb */
      ret = rlcCfgUpdateDlRb(gCb,rbCb,ueCb, entCfg);
      if (ret != CKW_CFG_REAS_NONE)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               ret);
        DU_LOG("\nERROR  -->  RLC_DL : CELLID:%d RBID:%d updation failed",
                  cellId,
                  entCfg->rbId);
         memcpy(rbCb, &tRbCb, sizeof(RlcDlRbCb));

         return (ret);
      }
   }

   /* Fill entCfm structure */
   RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   return ROK;
} 


/** 
 * @brief This primitive deletes the existing RB in Ue/Cell Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Check for CELL CB is present
 *       - If yes, Check for RB ID
 *      - If RB ID is prenset Delete the RB CB
 *       - If there is no RB CB exist in CELL CB then Delete CELL CB.
 *      - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset Delete the RB CB
 *          - If there is no RB CB exist in UE CB then Delete UE CB.
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
S16 rlcCfgDelDlRb
(
RlcCb               *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   S16          ret;       /* Return Value */
   RlcDlUeCb     *ueCb;     /* UE Control Block */
   RlcDlCellCb   *cellCb;   /* UE Control Block */
   RlcDlRbCb     *rlcRbCb;   /* KW RB Control Block */

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgDelRb(RBID(%d), cellId(%d), cfgType(%d))",
            entCfg->rbId, 
            cellId, 
            entCfg->cfgType);

   ret = ROK;

   /* Get cellCb and delete rbCb from it */
   if (ueId == 0)
   {
      if(entCfg->rbId >= RLC_MAX_RB_PER_CELL)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RbId, Max is [%d] UEID:%d CELLID:%d",
                  RLC_MAX_RB_PER_CELL,
                  ueId,
                  cellId);
         return RFAILED;
      }
      /* Get cellCb */
      rlcDbmFetchDlCellCb(gCb,cellId, &cellCb);
      if (!cellCb)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CellCb not found UEID:%d RBID:%d",
                  ueId,
                  entCfg->rbId);
         return (ret);
      }

      /* Get rbCb */
      RLC_DBM_GET_CELL_RBCB(entCfg->rbId, cellCb->rbCb, rlcRbCb);

      if ( rlcRbCb == NULLP)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CellId[%u]:RbId[%d] not found",
                  cellId,
                  entCfg->rbId);
         return (ret);
      }

      /* Assign NULLP to rbCb in rbCbLst */
      cellCb->rbCb[entCfg->rbId] = NULLP;

      /* Assign NULLP to dlRbCb/ulRbCb.
       * Delete Hashlist allocated for it if any */
      cellCb->lCh[rlcRbCb->lch.lChId - 1].dlRbCb = NULLP;
      RLC_FREE(gCb,rlcRbCb->snssai, sizeof(Snssai)); 
      RLC_FREE(gCb,rlcRbCb, sizeof(RlcDlRbCb));   /*Vartika: Mem leak fix */  
   }
    /* Get ueCb and delete rbCb from it */
   else
   {
      if (!(RLC_VALIDATE_UE_RBID(entCfg->rbType, entCfg->rbId)))
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : Invalid RbId for RbType[%d] UEID:%d CELLID:%d", 
                  entCfg->rbType,
                  ueId,
                  cellId);
         return RFAILED;
      }

      /* Get ueCb */
      ret = rlcDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
      if (ret != ROK)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, 
                             CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
          DU_LOG("\nERROR  -->  RLC_DL : UeId [%d]: UeCb not found RBID:%d",
                  ueId,
                  entCfg->rbId);
         return (ret);
      }

      /* Get rbCb */
      RLC_DBM_GET_RBCB_FROM_UECB(entCfg->rbId, entCfg->rbType, ueCb, rlcRbCb);

      if ( rlcRbCb == NULLP)
      {
         /* Fill entCfm structure */
         RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
               CKW_CFG_REAS_RB_UNKWN);
         DU_LOG("\nERROR  -->  RLC_DL : CellId[%u]:RbId[%d] not found",
                  cellId,
                  entCfg->rbId);
         return (ret);
      }
      ueCb->lCh[rlcRbCb->lch.lChId - 1].dlRbCb = NULLP;

#ifdef LTE_L2_MEAS
      RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(gCb, rlcRbCb);
#endif
      /* Free the Buffers of RbCb */
      if( RLC_MODE_UM == rlcRbCb->mode)
      {
         rlcUmmFreeDlRbCb(gCb,rlcRbCb);
         /* Delete RbCb  */
         RLC_FREE(gCb,rlcRbCb, sizeof(RlcDlRbCb));     
      }
      else if( RLC_MODE_AM == rlcRbCb->mode)
      {
         rlcAmmFreeDlRbCb(gCb,rlcRbCb);
      }

      /* Assign NULLP to rbCb in rbCbLst */
      if ( entCfg->rbType == CM_LTE_SRB )
      {
         ueCb->srbCb[entCfg->rbId] = NULLP;
      }
      else
      {
         ueCb->drbCb[entCfg->rbId] = NULLP;
      }
   }

   RLC_LMM_RB_STS_DEC(gCb);

   /* Fill entCfm structure */
   RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType, CKW_CFG_CFM_OK, 
                       CKW_CFG_REAS_NONE);

   return (ret);
} 


/** 
 * @brief This primitive re-establish the existing RB in Ue/Cell Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Check for CELL CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset initialize the parameters of the RB CB
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Check for RB ID
 *        - If RB ID is prenset initialize the parameters of the RB CB
 *        - Else, Status Indication with Reason
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgReEstDlRb
(
RlcCb              *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
Bool               sndReEstInd,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   RlcDlRbCb     *rbCb;   /* RB Control Block */
   CmLteRlcId   rlcId;   /* RLC Identifier */

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgReEstDlRb(ueId(%d), cellId(%d), cfgType(%d))",
            ueId, 
            cellId, 
            entCfg->cfgType);

   /* Get rlcId */
   rlcId.ueId = ueId;
   rlcId.cellId = cellId;
   rlcId.rbId = entCfg->rbId;
   rlcId.rbType = entCfg->rbType;

   rlcDbmFetchDlRbCbByRbId(gCb,&rlcId, &rbCb);
   if (rbCb == NULLP)
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, rlcId.rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_RB_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : CellId[%u]:RbId[%d] not found",
               cellId,
               entCfg->rbId);
      return RFAILED;
   }

   rbCb->rlcId.ueId = ueId;

   switch (rbCb->mode)
   {
      case RLC_MODE_TM:
         {
            rlcDlTmmReEstablish(gCb,rbCb);
            break;
         }

      case RLC_MODE_UM:
         {
            rlcDlUmmReEstablish(gCb,rlcId,sndReEstInd,rbCb);
            break;
         }

      case RLC_MODE_AM:
         {           
            rlcAmmDlReEstablish(gCb, rlcId, rbCb);
            break;
         }
   }

   /* Fill entCfm structure */
   RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   return ROK;
} 


/** 
 * @brief This primitive deletes the RBs in Ue Cb.
 *
 * @details
 *    - If UE ID is 0 then
 *      - Status Indication with Reason
 *    - Else,
 *      - Check for UE CB is present
 *      - If yes, Delete all RB CB in UE CB and Delete UE CB also.
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    gCb      -  RLC Instance Control Block
 * @param [in]    ueId     -  UE Identifier
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return  S16
 *    -#ROK
 *    -#RFAILED
 */
S16 rlcCfgDelDlUe
(
RlcCb              *gCb,
CmLteRnti          ueId,
CmLteCellId        cellId,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   S16        ret;     /* Return Value */
   RlcDlUeCb   *ueCb;   /* UE Control Block */

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgDelUe(ueId(%d), cellId(%d), cfgType(%d))",
            ueId, 
            cellId, 
            entCfg->cfgType);

   ret = ROK;

   /* Check for ueId is present or not */
   if ( ueId == 0 )
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
          CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : ueId(%d), cellId(%d)",
               ueId, 
               cellId);
      return RFAILED;
   }

   /* Fetch Ue Cb */
   ret = rlcDbmFetchDlUeCb(gCb,ueId, cellId, &ueCb);
   if (ret != ROK)
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
         CKW_CFG_CFM_NOK, CKW_CFG_REAS_UE_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : UEID:%d UeCb not found RBID:%d",
               ueId,
               entCfg->rbId);
      return RFAILED;
   }

#ifdef LTE_L2_MEAS
   rlcDelFrmDlL2Meas(gCb,cellId,ueId);
   rlcDbmDelAllDlL2MeasTbFrmUe(gCb,ueCb);
#endif
   /* Delete Ue Cb */
   rlcDbmDelDlUeCb(gCb,ueCb, FALSE);

   /* Fill entCfm structure */
   RLC_CFG_FILL_CFG_CFM(entCfm, entCfg->rbId, entCfg->rbType,
      CKW_CFG_CFM_OK, CKW_CFG_REAS_NONE);

   return ROK;
} 


/**
 * @brief This primitive deletes the RBs in Ue Cb.
 *
 * @details
 *    - If CELL ID is 0 then
 *      - Status Indication with Reason
 *    - Else,
 *      - Check for CELL CB is present
 *      - If yes, Delete all RB CB in CELL CB and Delete CELL CB also.
 *      - Else, Status Indication with Reason
 *    - Fill entity confirmation
 *
 * @param [in]    cellId   -  CELL Identifier
 * @param [in]    entCfg   -  Entity Configuration to be done.
 * @param [out]   entCfm   -  Entity Confirmation
 *
 * @return S16
 *    -#ROK
 *    -#RFAILED
 */
S16 rlcCfgDelDlCell
(
RlcCb               *gCb,
CmLteCellId        cellId,
RlcEntCfgInfo      *entCfg,
RlcEntCfgCfmInfo   *entCfm
)
{
   RlcDlCellCb   *cellCb;   /* UE Control Block */
   uint8_t       rbId;      /* RB Identifier */

   DU_LOG("\nDEBUG  -->  RLC_DL : rlcCfgDelCell( cellId(%d), cfgType(%d)",
         cellId, 
         entCfg->cfgType);

   cellCb = NULLP;
   rbId = entCfg->rbId;

   /* Check for ueId is present or not */
   if ( cellId == 0 )
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_CELL_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : cellId is 0 (%d) ",
               cellId);
      return RFAILED;
   }

   /* Fetch Ue Cb */
   rlcDbmFetchDlCellCb(gCb,cellId, &cellCb);
   if (!cellCb)
   {
      /* Fill entCfm structure */
      RLC_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_NOK,
            CKW_CFG_REAS_CELL_UNKWN);
      DU_LOG("\nERROR  -->  RLC_DL : CellCb not found for RBID:%d",
               entCfg->rbId);
      return RFAILED;
   }

   /* Delete Ue Cb */
   rlcDbmDelDlCellCb(gCb,cellCb);

   /* Fill entCfm structure */
  /* kw005.201 added support for L2 Measurement */         
   RLC_CFG_FILL_CFG_CFM(entCfm, rbId, entCfg->rbType, CKW_CFG_CFM_OK,
                       CKW_CFG_REAS_NONE);

   return ROK;
} 

/**
 * @brief This primitive changes the ueId of Ue Cb.
 *
 * @details
 *    - If oldUeId and newUeId are
 *      - Confirm the Status with Reason
 *    - If UeId not present
 *      - Confirm the Status with Reason
 *    - Create New UeCb
 *    - Copy rbCbs from old UeCb to new UeCb
 *    - Delete old UeCb
 *    - Fill entity confirmation
 *
 * @param [in]    ueInfo      -  Old UE Information
 * @param [in]    newUeInfo   -  New UE Information
 * @param [out]   status      -  Status
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 */
S16 rlcCfgDlUeIdChng
(
RlcCb        *gCb,
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo,
CmStatus    *status
)
{
   RlcDlUeCb *ueCb;
/*kw004.201 Adding of Missing Trace in RLC PDCP*/

   if ( (ueInfo->ueId == newUeInfo->ueId) && 
        (ueInfo->cellId == newUeInfo->cellId))
   {
      status->reason = CKW_CFG_REAS_SAME_UEID;
      status->status = CKW_CFG_CFM_NOK; 
      DU_LOG("\nERROR  -->  RLC_DL : Old UeId[%d] same as new UeId[%d]",
            ueInfo->ueId,
            newUeInfo->ueId);
      return RFAILED;
   } 
   
   if(ROK == rlcDbmFetchDlUeCb(gCb,newUeInfo->ueId, newUeInfo->cellId, &ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_DL : NewUeId[%d]:ueCb already exists",
            newUeInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_EXISTS;
      status->status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }
  
   if(ROK != rlcDbmFetchDlUeCb(gCb,ueInfo->ueId, ueInfo->cellId, &ueCb))
   {

      DU_LOG("\nERROR  -->  RLC_DL : UeId [%d]: UeCb not found",
            ueInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_UNKWN;
      status->status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }
  
#ifdef LTE_L2_MEAS
   rlcHdlMeasDlUeIdChg(gCb, ueInfo->cellId, ueInfo->ueId, newUeInfo->ueId);
#endif   
   if(ROK != cmHashListDelete(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_DL : UeId[%u] HashList Deletion Failed",
            ueInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
      status->status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }
   
   /* update the hash key with new values */ 
   ueCb->ueId = newUeInfo->ueId;
   ueCb->cellId = newUeInfo->cellId;

   if(ROK != cmHashListInsert(&(gCb->u.dlCb->ueLstCp), 
                              (PTR)ueCb, (uint8_t *)&(ueCb->ueId),
                              (uint16_t) sizeof(CmLteRnti)))

   {
      DU_LOG("\nERROR  -->  RLC_DL : UeId[%u] HashList Insertion Failed",
            newUeInfo->ueId);
      status->reason = CKW_CFG_REAS_UE_CREAT_FAIL;
      status->status = CKW_CFG_CFM_NOK;
      return RFAILED;
   }  
  
   return ROK;
} 

/********************************************************************30**

         End of file
**********************************************************************/
