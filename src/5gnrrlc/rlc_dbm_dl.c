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
  
        Name:    RLC - Database module file
    
        Type:    C source file
  
        Desc:    Source code for Database Module functions such as, 

                 - rlcDbmInit
                 - kwDbmDeInit
                 - kwDbmCreateRbCb
                 - kwDbmFetchRbCb 
                 - kwDbmUpdateRbCb
                 - kwDbmDelRbCb
                 - kwDbmDelAllRb
                 - kwDbmCreateUeCb
                 - kwDbmFetchUeCb
                 - kwDbmUpdateUeCb
                 - kwDbmDelUeCb
                 - kwDbmDelAllUe
                 - kwDbmDelMeasEvtCb
                 - kwDbmCreateCellCb
                 - kwDbmFetchCellCb
                 - kwDbmUpdateCellCb
                 - kwDbmDelCellCb
                 - kwDbmDelAllCell
                 - kwDbmShutdown

        File:    rlc_dbm_dl.c
  
*********************************************************************21*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "rgu.h"           /* RGU defines */
#include "kwu.h"           /* KWU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "rgu.x"           /* RGU */
#include "kwu.x"           /* KWU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_dl_ul_inf.h"
#include "rlc_dl.h"

#ifdef TENB_STATS
TSL2UeStatsCb* TSL2MapUeStatsBlk (uint16_t rnti);
#endif
/** 
 * @file gp_dbm_dl.c
 * @brief RLC Downlink database module
*/
#define RLC_MODULE RLC_DBGMASK_DUT


/**
 * @brief Handler to initialize hash list
 *       
 * @details
 *    This function initializes the UeCb, CellCb hash lists
 *
 *  @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
S16 rlcDbmDlInit(RlcCb *gCb)
{

   /* Initialize ueCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.dlCb->ueLstCp), 
                            (uint16_t) RLC_UE_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {   
      DU_LOG("\nERROR  -->  RLC_DL : UeLstCp Initialization Failed");
      return RFAILED;
   }

   /* Initialize cellCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.dlCb->cellLstCp), 
                            (uint16_t) RLC_CELL_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.dlCb->ueLstCp);
      DU_LOG("\nERROR  -->  RLC_DL : CellLstCp Initialization Failed");
      return RFAILED;
   }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS_RLC
   /* Initialize qcI Hash List */
   if(ROK != cmHashListInit(&(rlcCb.rlcL2Cb.qciHlCp), 
                            (uint16_t) RLC_QCI_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) TRUE, 
                            (uint16_t) CM_HASH_KEYTYPE_DEF,
                            rlcCb.init.region, 
                            rlcCb.init.pool))
   {
      cmHashListDeinit(&gCb->u.dlCb->cellLstCp);
      cmHashListDeinit(&gCb->u.dlCb->ueLstCp);
      DU_LOG("\nERROR  -->  RLC_DL : rlcDbmInit: cmHashListInit Failed for rlcCb.qciHlCp");
      return RFAILED;
   }
   
   /* Initialize tbHlCp Hash List */
   if(ROK != cmHashListInit(&(rlcCb.rlcL2Cb.tbHlCp), 
                            (uint16_t) RLC_TB_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_DEF,
                            rlcCb.init.region, 
                            rlcCb.init.pool))
   {
      cmHashListDeinit(&rlcCb.rlcL2Cb.qciHlCp);
      cmHashListDeinit(&gCb->u.dlCb->cellLstCp);
      cmHashListDeinit(&gCb->u.dlCb->ueLstCp);
      DU_LOG("\nERROR  -->  RLC_DL : rlcDbmInit: cmHashListInit Failed for rlcCb.tbHlCp");
      return RFAILED;
   }
#endif /* LTE_L2_MEAS */

   return ROK;
} /* rlcDbmDlInit */


/**
 * @brief Handler to De initialize hash list
 *       
 * @param[in]    gCb      RLC Instance Control Block
 *
 * @return  Void
*/
Void rlcDbmDlDeInit(RlcCb *gCb)
{


   /* De Initialize ueCb Hash List */
   cmHashListDeinit(&(gCb->u.dlCb->ueLstCp));

   /* De Initialize cellCb Hash List */
   cmHashListDeinit(&(gCb->u.dlCb->cellLstCp));

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS_RLC
   /* De Initialize qciCb Hash List */
   cmHashListDeinit(&(rlcCb.rlcL2Cb.qciHlCp));

   /* De Initialize tbHlCp Hash List */
   cmHashListDeinit(&(rlcCb.rlcL2Cb.tbHlCp));
   
#endif /* LTE_L2_MEAS */

   return;
} /* rlcDbmDlDeInit */


/**
 * @brief Handler to fetch rbCb by the rlcId
 *       
 * @details
 *    This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *    the upper interface (CKW/KWU).
 *
 *  @param[in]    gCb     RLC Instance Control Block
 *  @param[in]    rlcId   RLC Identifier 
 *  @param[out]   rbCb    RB Control Block
 *
 *  @return  Void
*/
void rlcDbmFetchDlRbCbByRbId(RlcCb *gCb,CmLteRlcId *rlcId, RlcDlRbCb **rbCb)
{
   *rbCb= NULLP;
   
   /* Check for UE CB or CELL CB */
   if (rlcId->ueId == 0)
   {
      RlcDlCellCb *cellCb;
      
      if(rlcId->rbId >= RLC_MAX_RB_PER_CELL)
      {
         DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbByRbId : Invalid RbId, Max is [%d] \
	    UEID:%d CELLID:%d", RLC_MAX_RB_PER_CELL, rlcId->ueId, rlcId->cellId);
         return;
      }

      rlcDbmFetchDlCellCb(gCb,rlcId->cellId, &cellCb);
      if(!cellCb)
      {
         DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbByRbId : CellCb[%d] not found UEID:%d \
	    RBID:%d", rlcId->cellId, rlcId->ueId, rlcId->rbId);
         return;
      }

      *rbCb = cellCb->rbCb[rlcId->rbId];
   }
   else
   {
      RlcDlUeCb *ueCb;
      if (!(RLC_VALIDATE_UE_RBID(rlcId->rbType, rlcId->rbId)))
      {
         DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbByRbId : Invalid RbId[%d] for RbType[%d] \
	    RBID:%d CELLID:%d", rlcId->rbId, rlcId->rbType, rlcId->ueId, rlcId->cellId);
         return;
      }
      
      if (ROK != rlcDbmFetchDlUeCb(gCb,rlcId->ueId, rlcId->cellId, &ueCb))
      {
         DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbByRbId : UeId [%d]: UeCb not found RBID:%d",\
            rlcId->ueId, rlcId->rbId);
         return;
      }

      RLC_DBM_GET_RBCB_FROM_UECB(rlcId->rbId, rlcId->rbType, ueCb, *rbCb);
   }

   return;
} /* rlcDbmFetchDlRbCbByRbId */


/**
 * @brief Handler to fetch rbCb from Local Channel Id 
 *
 * @details
 *    This function is invoked by CFG to fetch rbCb from the cellCb/ueCb
 *    from local Channel Id (CKW/KWU).
 *
 * @param[in]    gCb      -  RLC Instance Control Block
 * @param[in]    cellId   -  CELL Identifier 
 * @param[in]    ueId     -  UE Identifier 
 * @param[in]    lcId     -  Logical Channel Identifier
 * @param[out]   rbCb     -  RB Control Block
 *
 * @return  Void
 *
*/
void rlcDbmFetchDlRbCbFromLchId(RlcCb *gCb, CmLteRnti ueId, CmLteCellId cellId, \
CmLteLcId lcId, RlcDlRbCb **rbCb)
{
   RlcDlUeCb *ueCb;
   
   /* Check for UE CB or CELL CB */
   if (ueId == 0)
   {
      RlcDlCellCb *cellCb;
      
      rlcDbmFetchDlCellCb(gCb, cellId, &cellCb);
      if(!cellCb)
      {
         DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbFromLchId: CellCb[%d] not found UEID:%d", \
	    cellId, ueId);
         return;
      }

      *rbCb = cellCb->lCh[lcId - 1].dlRbCb;
      return;
   }

   if (rlcDbmFetchDlUeCb(gCb, ueId, cellId, &ueCb) != ROK)
   {
      DU_LOG("\nERROR  -->  RLC_DL : rlcDbmFetchDlRbCbFromLchId: UeId [%d]: UeCb not found",ueId);
      return;
   }

   *rbCb = ueCb->lCh[lcId - 1].dlRbCb;

   return;
} /* rlcDbmFetchDlRbCbFromLchId */


/**
 * @brief Handler to delete RbCb
 *       
 * @details 
 *    This function is invoked by CFG to remove RbCb from Ue/Cell hashlist 
 *
 * @param[in] gCb       - RLC Instance Control Block
 * @param[in] rbCbLst   - Rb Cb List
 * @param[in] numRbCb   - Number of rbCbs
 *
 * @return  Void
 *
*/
Void rlcDbmDelAllDlRb(RlcCb *gCb,RlcDlRbCb **rbCbLst,uint8_t numRbCb)
{
   uint32_t idx;


   for (idx = 0; idx < numRbCb; idx++)
   {
      if (rbCbLst[idx] != NULLP)
      {
#ifdef LTE_L2_MEAS
         RLC_UPD_L2_DECR_NONIP_PER_QCI_RB_COUNT(gCb, (rbCbLst[idx]));
#endif
         if( RLC_MODE_UM == rbCbLst[idx]->mode)
         {
            rlcUmmFreeDlRbCb(gCb,rbCbLst[idx]);
            RLC_FREE (gCb,rbCbLst[idx], sizeof (RlcDlRbCb));       
         }
         else if( RLC_MODE_AM == rbCbLst[idx]->mode)
         {
            rlcAmmFreeDlRbCb(gCb,rbCbLst[idx]);
         }
         /* ccpu00136940 */
         else if(RLC_MODE_TM == rbCbLst[idx]->mode)
         {
            cmLListCatLList(&(gCb->u.dlCb->toBeFreed.sduLst),&(rbCbLst[idx]->m.tm.sduQ));
            RLC_FREE (gCb,rbCbLst[idx]->snssai, sizeof (Snssai));
            RLC_FREE (gCb,rbCbLst[idx], sizeof (RlcDlRbCb));       
         }

      }

   }

   return;
} /* rlcDbmDelAllDlRb */


/**
 * @brief  Handler to create an UeCb
 *
 * @details  
 *    This function is invoked by CFG to create UeCb and insert into the 
 *    Ue hashlist of RlcCb.
 *
 * @param[in] gCb       RLC Instance Control Block
 * @param[in] ueId      UE Identifier 
 * @param[in] cellId    Cell Identifier 
 * @param[in] ueCb      UE Control Block 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
S16 rlcDbmCreateDlUeCb(RlcCb *gCb,CmLteRnti ueId,CmLteCellId cellId,RlcDlUeCb **ueCb)
{
   RlcDlUeCb *tUeCb;        

   RLC_ALLOC(gCb,*ueCb, sizeof(RlcDlUeCb));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*ueCb == NULLP)
   {   
      DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed cellID:%d",
            cellId);
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tUeCb = *ueCb;
   tUeCb->ueId = ueId;
   tUeCb->cellId = cellId;

   if (ROK != cmHashListInsert(&(gCb->u.dlCb->ueLstCp), 
                               (PTR)tUeCb, 
                               (uint8_t *)&(tUeCb->ueId),
                               (uint16_t) sizeof(CmLteRnti)))
   {
      DU_LOG("\nERROR  -->  RLC_DL : UeId[%u] HashList Insertion Failed",
            ueId);
      return RFAILED;
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe++;


   return ROK;
} /* kwDbmCreateUeCb */


/**
 * @brief Handler to Fetch an UeCb
 *
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 * @param[in]    gCb       RLC Instance Control Block 
 * @param[in]    ueId      UE Identifier 
 * @param[in]    cellId    Cell Identifier 
 * @param[out]   ueCb      UE Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
*/
uint8_t rlcDbmFetchDlUeCb(RlcCb *gCb, CmLteRnti ueId, CmLteCellId  cellId, RlcDlUeCb **ueCb)
{
   return (cmHashListFind(&(gCb->u.dlCb->ueLstCp), 
                           (uint8_t *)&(ueId), 
                           sizeof(CmLteRnti),
                           RLC_DEF_SEQ_NUM, 
                           (PTR *) ueCb));
} /* rlcDbmFetchDlUeCb */


/**
 * @brief Handler to delete an UeCb
 *
 * @details
 *    This function is invoked by CFG to delete UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 *
 * @param[in] gCb      RLC Instance Control Block
 * @param[in] ueCb     UE Control Block
 * @param[in] abrtFlag Abort Flag
 *
 * @return Void
 *
*/
Void rlcDbmDelDlUeCb(RlcCb *gCb,RlcDlUeCb *ueCb,Bool abortFlag)
{

#if  (!defined(KW_PDCP) || !(defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))
   UNUSED(abortFlag);
#endif /* (!defined(KW_PDCP) || ! (defined(PJ_SEC_ASYNC) || 
          defined(PJ_CMP_ASYNC)))*/

   /* Delete all logical channels */
   RLC_MEM_ZERO(ueCb->lCh,sizeof(RlcDlLch) * RLC_MAX_LCH_PER_UE);

   /* Delete all SRB RbCbs in UeCb */
   rlcDbmDelAllDlRb(gCb,ueCb->srbCb, RLC_MAX_SRB_PER_UE);

   /* Delete all DRB RbCbs in UeCb */
   rlcDbmDelAllDlRb(gCb,ueCb->drbCb, RLC_MAX_DRB_PER_UE);

   /* Delete ueCb entry from ueLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_DL : UeId[%u] HashList Insertion Failed",
            ueCb->ueId);
   }
   memset(&gCb->rlcThpt.ueTputInfo.thptPerUe[ueCb->ueId -1], 0, sizeof(RlcThptPerUe));
   gCb->rlcThpt.ueTputInfo.numActvUe--;
   
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe--;
   
   /* Deallocate ueCb */
   RLC_FREE(gCb,ueCb, sizeof(RlcDlUeCb));

   return;
} /* kwDbmDelUeCb */


/**
 * @brief Handler to delete all UeCbs
 *
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue 
 *    hashlist of RlcCb.
 *
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
Void rlcDbmDelAllDlUe(RlcCb  *gCb)
{
   RlcDlUeCb *ueCb = NULLP; 

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.dlCb->ueLstCp), 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      /* Delete ueCb */
      rlcDbmDelDlUeCb(gCb,ueCb, TRUE);

      ueCb = NULLP;
   }

   return;
} /* kwDbmDelAllUe */

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
Void rlcDbmDelAllDlL2MeasTbFrmUe(RlcCb *gCb,RlcDlUeCb  *ueCb)
{
   uint8_t       tbIdx;
   RlcL2MeasTb   *l2MeasTb = NULLP;
   for(tbIdx = 0; tbIdx < RLC_MAX_TB_PER_UE; tbIdx++)
   { 
       l2MeasTb = ueCb->l2MeasTbCb[tbIdx];
       if(l2MeasTb != NULLP)
       {
          RLC_FREE(gCb,l2MeasTb, sizeof(RlcL2MeasTb));
          ueCb->l2MeasTbCb[tbIdx] = NULLP;
       }       
   }  
   return;   
}/* End of kwDbmDelL2MeasTb */
#endif /* LTE_L2_MEAS */

/**
 * @brief Handler to create CellCb
 *
 * @details
 *    This function is invoked by CFG to create CellCb and insert into
 *    the Cell hashlist of RlcCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellId   Cell Identifier 
 *  @param[in] cellCb   Cell Control Block 
 *
 *  @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
S16 rlcDbmCreateDlCellCb(RlcCb *gCb,CmLteCellId cellId,RlcDlCellCb **cellCb)
{
   RlcDlCellCb *tCellCb; 
   

   RLC_ALLOC(gCb,*cellCb, sizeof(RlcDlCellCb));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*cellCb == NULLP)
   {
      DU_LOG("\nERROR  -->  RLC_DL : Memory allocation failed");
      return RFAILED;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tCellCb = *cellCb;
   tCellCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.dlCb->cellLstCp), 
                              (PTR) tCellCb,
                              (uint8_t *)&(tCellCb->cellId), 
                              (uint16_t) sizeof(CmLteCellId)))
   {
      DU_LOG("\nERROR  -->  RLC_DL : HashList Insertion Failed");
      return RFAILED;
   }

   return (ROK);
} /* rlcDbmCreateDlCellCb */


/**
 * @brief Handler to Fetch an CellCb
 *       
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of RlcCb.
 *
 * @param[in]    gCb      RLC Instance Control Block
 * @param[in]    cellId   Cell Identifier 
 * @param[out]   cellCb   UE Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
uint8_t rlcDbmFetchDlCellCb(RlcCb *gCb, CmLteCellId  cellId, RlcDlCellCb **cellCb)
{
   *cellCb = NULLP;
   
   if(ROK != cmHashListFind(&(gCb->u.dlCb->cellLstCp), 
                            (uint8_t *)&(cellId),
                            sizeof(CmLteCellId), 
                            RLC_DEF_SEQ_NUM, 
                            (PTR*) cellCb))
   {
      DU_LOG("\nERROR  -->  RLC_DL : CellCb [%d] not found", cellId);
      return RFAILED;
   }

   return ROK;
} /* kwDbmFetchCellCb */


/**
 * @brief Handler to delete CellCb
 *
 * @details
 *    This function is invoked by CFG to delete CellCb from the Cell hashlist 
 *    of RlcCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellCb   Cell Control Block
 *
 *  @return  Void
*/
Void rlcDbmDelDlCellCb(RlcCb *gCb,RlcDlCellCb *cellCb)
{

   /* Delete all rbCbs in cellCb */
   rlcDbmDelAllDlRb(gCb,cellCb->rbCb, RLC_MAX_RB_PER_CELL);
   
   /* Delete cellCb entry in hash list cellLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.dlCb->cellLstCp), (PTR) cellCb))
   {
      DU_LOG("\nERROR  -->  RLC_DL : HashList Deletion Failed");
   }

   /* Deallocate cellCb */
   RLC_FREE(gCb, cellCb, sizeof(RlcDlCellCb));

   return;
} /* kwDbmDelCellCb */


/**
 * @brief Handler to delete all UeCbs
 *       
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue
 *    hashlist of RlcCb.
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
Void rlcDbmDelAllDlCell(RlcCb *gCb)
{
   RlcDlCellCb *cellCb = NULLP;

   /* Until no more cellCb is ueLstCp hash list get and delete cellCb */
   while (ROK == cmHashListGetNext(&(gCb->u.dlCb->cellLstCp), 
                                   (PTR) cellCb, 
                                   (PTR *)&cellCb))
   {
      rlcDbmDelDlCellCb(gCb, cellCb);
      
      cellCb = NULLP;
   }

   return;
} /* rlcDbmDelAllDlCell */


/**
 * @brief Handler to delete all UeCbs and CellCbs
 *       
 * @details
 *    This function is invoked by LMM to delete all UeCbs from the Ue
 *    hashlist of RlcCb and cellCbs from the Cell hashlist of rlcCb.
 * 
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
S16 rlcDbmDlShutdown(RlcCb *gCb)
{

   rlcDbmDelAllDlCell(gCb);

   rlcDbmDelAllDlUe(gCb);

   rlcDbmDlDeInit(gCb);

   return ROK;
} /* kwDbmShutdown */


/********************************************************************30**
  
         End of file
**********************************************************************/
