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

        File:    rlc_dbm_ul.c
  
*********************************************************************21*/


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "rlc_err.h"        /* Err defines */
#include "rlc_env.h"        /* RLC environment options */


/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "rlc_utils.h"            /* RLC defines */
#include "rlc_ul.h"

/** 
 * @file gp_dbm_ul.c
 * @brief RLC Uplink database module
*/

#define RLC_MODULE RLC_DBGMASK_DUT


/**
 * @brief Handler to initialize hash list
 *       
 * @details
 *    This function initializes the UeCb, CellCb and transactions hash lists
 *
 * @param[in] gCb   RLC instance control block
 * 
 * @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
S16 rlcDbmUlInit(RlcCb *gCb)
{
   /* Initialize ueCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->ueLstCp), 
                            (uint16_t) RLC_UE_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      DU_LOG("\nERROR  -->  RLC_UL : UeLstCp Initialization Failed");
      return RFAILED;
   }

   /* Initialize cellCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->cellLstCp), 
                            (uint16_t) RLC_CELL_LIST_BUCKET_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      DU_LOG("\nERROR  -->  RLC_UL : CellLstCp Initialization Failed");
      return RFAILED;
   }

   if(ROK != cmHashListInit(&(gCb->u.ulCb->transIdLstCp), 
                            (uint16_t) RLC_TRANS_ID_LST_BKT_SIZE,
                            (uint16_t) 0, 
                            (Bool) FALSE, 
                            (uint16_t) CM_HASH_KEYTYPE_CONID,
                            RLC_GET_MEM_REGION(gCb), 
                            RLC_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      cmHashListDeinit(&gCb->u.ulCb->cellLstCp);
      DU_LOG("\nERROR  -->  RLC_UL : transIdLstCp Initialization Failed");
      return RFAILED;
   }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   /* Initialize qcI Hash List */
#endif /* LTE_L2_MEAS */

   return (ROK);
} /* rlcDbmUlInit */


/**
 * @brief Handler to De initialize hash list
 *       
 * @param[in]    gCb      RLC Instance Control Block
 *
 * @return  Void
*/
Void rlcDbmUlDeInit(RlcCb *gCb)
{

   /* De Initialize ueCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->ueLstCp));

   /* De Initialize cellCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->cellLstCp));

   /* De Initialize transaction Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->transIdLstCp));
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS_RLC
   /* De Initialize qciCb Hash List */
   cmHashListDeinit(&(rlcCb.rlcL2Cb.qciHlCp));
#endif /* LTE_L2_MEAS */

   return;
} /* kwDbmDeInit */


/**
 * @brief Handler to fetch rbCb by the rlcId
 *       
 * @details
 *    This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *    the upper interface (CKW/KWU).
 *
 * @param[in]    gCb     RLC Instance Control Block
 * @param[in]    rlcId   RLC Identifier 
 * @param[out]   rbCb    RB Control Block
 *
 * @return  Void
*/
Void rlcDbmFetchUlRbCbByRbId
(
RlcCb         *gCb,
CmLteRlcId   *rlcId,  
RlcUlRbCb     **rbCb  
)
{
   *rbCb= NULLP;

   /* Check for UE CB or CELL CB */
   if (rlcId->ueId == 0)
   {
      RlcUlCellCb *cellCb;

      if(rlcId->rbId >= RLC_MAX_RB_PER_CELL)
      {
         DU_LOG("\nERROR  -->  RLC_UL : Invalid RbId, cellId:%d UEID:%d Max is [%d]",
               rlcId->cellId, rlcId->ueId, RLC_MAX_RB_PER_CELL);
         return;
      }

      rlcDbmFetchUlCellCb(gCb,rlcId->cellId, &cellCb);
      if(!cellCb)
      {
         DU_LOG("\nERROR  -->  RLC_UL : CellCb not found RBID:%d UEID:%d",
               rlcId->rbId,
               rlcId->ueId);
         return;
      }

      *rbCb = cellCb->rbCb[rlcId->rbId];
   }
   else
   {
      RlcUlUeCb *ueCb;

      if (!(RLC_VALIDATE_UE_RBID(rlcId->rbType, rlcId->rbId)))
      {
         DU_LOG("\nERROR  -->  RLC_UL : Invalid RbId for RbType[%d] CELLID:%d UEID:%d", 
               rlcId->rbType,
               rlcId->cellId,
               rlcId->ueId);
         return;
      }

      if (rlcDbmFetchUlUeCb(gCb,rlcId->ueId, rlcId->cellId, &ueCb) != ROK)
      {
         DU_LOG("\nERROR  -->  RLC_UL : UeId [%d]: UeCb not found RBID:%d",
               rlcId->ueId,
               rlcId->rbId);
         return;
      }

      RLC_DBM_GET_RBCB_FROM_UECB(rlcId->rbId, rlcId->rbType, ueCb, *rbCb);
   }
   return;
} /* rlcDbmFetchUlRbCbByRbId */


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
void rlcDbmFetchUlRbCbFromLchId(RlcCb *gCb, CmLteRnti ueId, CmLteCellId cellId,\
   CmLteLcId lcId, RlcUlRbCb **rbCb)
{
   RlcUlUeCb *ueCb;

   *rbCb = NULLP;
   
   /* Check for UE CB or CELL CB */
   if (ueId == 0)
   {
      RlcUlCellCb *cellCb;
      
      rlcDbmFetchUlCellCb(gCb,cellId, &cellCb);
      if(!cellCb)
      {
         DU_LOG("\nERROR  -->  RLC_UL : rlcDbmFetchUlRbCbFromLchId: CellCb[%d] not found UEID:%d",\
	    cellId, ueId);
         return;
      }

      *rbCb = cellCb->lCh[lcId - 1].ulRbCb;
      return;
   }
   
   if (rlcDbmFetchUlUeCb(gCb,ueId, cellId, &ueCb) != ROK)
   {
      DU_LOG("\nERROR  -->  RLC_UL : rlcDbmFetchUlRbCbFromLchId: UeId [%d]: UeCb not found",ueId);
      return;
   }

   *rbCb = ueCb->lCh[lcId - 1].ulRbCb;

   return;
} /* kwDbmFetchRbCbFromLchId */


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
Void rlcDbmDelAllUlRb
(
RlcCb       *gCb,
RlcUlRbCb   **rbCbLst,          
uint8_t     numRbCb            
)
{
   uint32_t idx; /* Index */

   for (idx = 0; idx < numRbCb; idx++)
   {
      if (rbCbLst[idx] != NULLP)
      {
         /* Free the Buffers of RbCb */
         if( RLC_MODE_UM == rbCbLst[idx]->mode ) 
         {
            rlcUmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
         else if(RLC_MODE_AM == rbCbLst[idx]->mode) 
         {
            rlcAmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
     
         RLC_FREE(gCb,rbCbLst[idx], sizeof (RlcUlRbCb));
      }
   }

   return;
} /* kwDbmDelAllRb */


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
S16 rlcDbmAddUlUeCb
(
RlcCb          *gCb,
CmLteRnti     ueId,       
CmLteCellId   cellId,    
RlcUlUeCb      *ueCb    
)
{

   ueCb->ueId = ueId;
   ueCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->ueLstCp), 
                              (PTR)ueCb, 
                              (uint8_t *)&(ueCb->ueId),
                              (uint16_t) sizeof(CmLteRnti)))
   {
      DU_LOG("\nERROR  -->  RLC_UL : UeId[%u] HashList Insertion Failed",
            ueId);
      return RFAILED;
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe++;


   return ROK;
} 


/**
 * @brief Handler to add a transaction 
 *
 * @details
 *    This function adds a transaction. 
 *
 * @param[in] gCb     RLC Instance Control Block
 * @param[in] cfg     Configuration information
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
S16 rlcDbmAddUlTransaction 
(
RlcCb        *gCb,
RlcUlCfgTmpData   *cfg
)
{
#ifndef ALIGN_64BIT
   DU_LOG("\nDEBUG  -->  RLC_UL : (transId(%ld)", cfg->transId);
#else
   DU_LOG("\nDEBUG  -->  RLC_UL : (transId(%d))", cfg->transId);
#endif

   return (cmHashListInsert(&(gCb->u.ulCb->transIdLstCp), 
                             (PTR)cfg, 
                             (uint8_t *)&(cfg->transId), 
                             (uint16_t) sizeof(cfg->transId)));
} 


/**
 * @brief Handler to find a transaction 
 *       
 * @details
 *    This function find transaction using transaction Id 
 *
 *
 * @param[in] gCb       RLC Instance Control Block
 * @param[in] transId   Transaction Id 
 * @param[out] cfg      Configuration information attached to this transaction
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
S16 rlcDbmFindUlTransaction
(
RlcCb             *gCb,
uint32_t          transId,
RlcUlCfgTmpData   **cfg
)
{

   if(ROK != cmHashListFind(&(gCb->u.ulCb->transIdLstCp),
                            (uint8_t *) &transId, 
                            sizeof (transId), 
                            RLC_DEF_SEQ_NUM,(PTR *) cfg))
   {
      DU_LOG("\nERROR  -->  RLC_UL : TransId [%d] not found",transId);
      return RFAILED;
   }
   return ROK;
}


/**
 *
 * @brief Handler to delete a transaction 
 *
 * @details
 *    This function deletes a transaction 
 *
 *
 *  @param[in] gCb     RLC Instance Control Block
 *  @param[in] cfg     Configuration information
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
S16 rlcDbmDelUlTransaction
(
RlcCb             *gCb,
RlcUlCfgTmpData   *cfg       
)
{

   if(cmHashListDelete(&(gCb->u.ulCb->transIdLstCp),(PTR) (cfg)) != ROK) 
   {
      DU_LOG("\nERROR  -->  RLC_UL : HashList Deletion failed");
      return RFAILED;
   }

   return ROK;
}


/**
 * @brief Handler to delete all transaction 
 *
 * @details
 *    This function deletes all transaction 
 *
 * @param[in] gCb     RLC Instance Control Block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 *
*/
S16 rlcDbmDelAllUlTransactions(RlcCb *gCb)
{
   RlcUlCfgTmpData *cfg = NULL;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->transIdLstCp), 
                                   (PTR) cfg, 
                                   (PTR *)&cfg))
   {
      if(rlcDbmDelUlTransaction(gCb, cfg) != ROK)
      {
         return RFAILED;
      }
      
      cfg = NULLP;
   }

   return ROK;
}


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
uint8_t rlcDbmFetchUlUeCb(RlcCb *gCb, CmLteRnti ueId, CmLteCellId  cellId, RlcUlUeCb **ueCb)
{
   return (cmHashListFind(&(gCb->u.ulCb->ueLstCp), 
                           (uint8_t *)&(ueId), sizeof(CmLteRnti),
                           RLC_DEF_SEQ_NUM, 
                           (PTR *) ueCb));
}


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
Void rlcDbmDelUlUeCb
(
RlcCb       *gCb,
RlcUlUeCb   *ueCb,       
Bool       abortFlag   
)
{

#if  (!defined(KW_PDCP) || !(defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))
   UNUSED(abortFlag);
#endif /* (!defined(KW_PDCP) || ! (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))*/

   /* Delete all logical channels */
   RLC_MEM_ZERO(ueCb->lCh,sizeof(RlcUlLch) * RLC_MAX_LCH_PER_UE);

   /* Delete all SRB RbCbs in UeCb */
   rlcDbmDelAllUlRb(gCb,ueCb->srbCb, RLC_MAX_SRB_PER_UE);

   /* Delete all DRB RbCbs in UeCb */
   rlcDbmDelAllUlRb(gCb,ueCb->drbCb, RLC_MAX_DRB_PER_UE);

   /* Delete ueCb entry from ueLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : HashList Deletion Failed cellId(%d)",
            ueCb->cellId);
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe--;
   /* Deallocate ueCb */
   RLC_FREE(gCb,ueCb, sizeof(RlcUlUeCb));

   return;
}

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
Void rlcDbmDelAllUlUe(RlcCb *gCb)
{
   RlcUlUeCb *ueCb = NULLP;  /* UE Control Block */

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->ueLstCp), 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      rlcDbmDelUlUeCb(gCb,ueCb, TRUE);

      ueCb = NULLP;
   }

   return;
}


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
S16 rlcDbmAddUlCellCb
(
RlcCb          *gCb,
CmLteCellId   cellId,    
RlcUlCellCb    *cellCb  
)
{
   RlcUlCellCb *tCellCb; 

   tCellCb = cellCb;
   tCellCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->cellLstCp), 
                              (PTR) tCellCb,
                              (uint8_t *)&(tCellCb->cellId), 
                              (uint16_t) sizeof(CmLteCellId)))
   {
      DU_LOG("\nERROR  -->  RLC_UL : HashList Insertion Failed");
      return RFAILED;
   }

   return ROK;
} 


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
void rlcDbmFetchUlCellCb(RlcCb *gCb, CmLteCellId cellId, RlcUlCellCb **cellCb)
{
   *cellCb = NULLP;
   if(ROK != cmHashListFind(&(gCb->u.ulCb->cellLstCp), 
                            (uint8_t *)&(cellId),sizeof(CmLteCellId), 
                            RLC_DEF_SEQ_NUM, (PTR*) cellCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : rlcDbmFetchUlCellCb : CellCb[%d] not found", cellId);
   }

   return;
}


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
Void rlcDbmDelUlCellCb(RlcCb *gCb,RlcUlCellCb *cellCb)
{

   /* Delete all rbCbs in cellCb */
   rlcDbmDelAllUlRb(gCb,cellCb->rbCb, RLC_MAX_RB_PER_CELL);

   /* Delete cellCb entry in hash list cellLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->cellLstCp), (PTR) cellCb))
   {
      DU_LOG("\nERROR  -->  RLC_UL : HashList Deletion Failed");
   }
   /* Deallocate cellCb */
   RLC_FREE(gCb, cellCb, sizeof(RlcUlCellCb));

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
Void rlcDbmDelAllUlCell(RlcCb *gCb)
{
   RlcUlCellCb *cellCb = NULLP; /* Cell Control Block */

   /* Until no more cellCb is ueLstCp hash list get and delete cellCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->cellLstCp), 
                                   (PTR) cellCb, 
                                   (PTR *)&cellCb))
   {
      rlcDbmDelUlCellCb(gCb,cellCb);

      cellCb = NULLP;
   }

   return;
} 


/**
 * @brief Handler to delete all UeCbs and CellCbs
 *       
 * @details
 *    This function is invoked by LMM to delete all UeCbs from the Ue
 *    hashlist of RlcCb and cellCbs from the Cell hashlist of rlcCb.
 * 
 * @param[in] gCb      RLC Instance Control Block
 *
*/
Void rlcDbmUlShutdown(RlcCb *gCb)
{

   rlcDbmDelAllUlCell(gCb);

   rlcDbmDelAllUlUe(gCb);

   rlcDbmUlDeInit(gCb);

   return;
}


/********************************************************************30**
  
         End of file
**********************************************************************/
