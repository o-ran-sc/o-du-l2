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

        File:    kw_dbm_ul.c
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="DBM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=194;


/* header (.h) include files */
#include "common_def.h"
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"

/** 
 * @file gp_dbm_ul.c
 * @brief RLC Uplink database module
*/

#define KW_MODULE KW_DBGMASK_DUT


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
#ifdef ANSI
PUBLIC S16 kwDbmUlInit
(
KwCb *gCb
)
#else
PUBLIC S16 kwDbmUlInit(gCb)
KwCb *gCb;
#endif
{
   TRC3(kwUlDbmInit)

   /* Initialize ueCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->ueLstCp), 
                            (U16) KW_UE_LIST_BUCKET_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            KW_GET_MEM_REGION(gCb), 
                            KW_GET_MEM_POOL(gCb)))
   {
      RLOG0(L_ERROR, "UeLstCp Initialization Failed");
      RETVALUE(RFAILED);
   }

   /* Initialize cellCb Hash List */
   if(ROK != cmHashListInit(&(gCb->u.ulCb->cellLstCp), 
                            (U16) KW_CELL_LIST_BUCKET_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            KW_GET_MEM_REGION(gCb), 
                            KW_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      RLOG0(L_ERROR, "CellLstCp Initialization Failed");
      RETVALUE(RFAILED);
   }

   if(ROK != cmHashListInit(&(gCb->u.ulCb->transIdLstCp), 
                            (U16) KW_TRANS_ID_LST_BKT_SIZE,
                            (U16) 0, 
                            (Bool) FALSE, 
                            (U16) CM_HASH_KEYTYPE_CONID,
                            KW_GET_MEM_REGION(gCb), 
                            KW_GET_MEM_POOL(gCb)))
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
      cmHashListDeinit(&gCb->u.ulCb->cellLstCp);
      RLOG0(L_ERROR, "transIdLstCp Initialization Failed");
      RETVALUE(RFAILED);
   }

/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS
   /* Initialize qcI Hash List */
#endif /* LTE_L2_MEAS */

   RETVALUE(ROK);
} /* kwDbmUlInit */


/**
 * @brief Handler to De initialize hash list
 *       
 * @param[in]    gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwDbmUlDeInit
(
KwCb *gCb
)
#else
PUBLIC Void kwDbmUlDeInit(gCb)
KwCb *gCb;
#endif
{
   TRC3(kwDbmUlDeInit);


   /* De Initialize ueCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->ueLstCp));

   /* De Initialize cellCb Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->cellLstCp));

   /* De Initialize transaction Hash List */
   cmHashListDeinit(&(gCb->u.ulCb->transIdLstCp));
/* kw005.201 added support for L2 Measurement */         
#ifdef LTE_L2_MEAS_RLC
   /* De Initialize qciCb Hash List */
   cmHashListDeinit(&(kwCb.kwL2Cb.qciHlCp));
#endif /* LTE_L2_MEAS */

   RETVOID;
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
#ifdef ANSI
PUBLIC Void kwDbmFetchUlRbCbByRbId
(
KwCb         *gCb,
CmLteRlcId   *rlcId,  
RlcUlRbCb     **rbCb  
)
#else
PUBLIC Void kwDbmFetchUlRbCbByRbId(gCb, rlcId, rbCb)
KwCb         *gCb;
CmLteRlcId   *rlcId;    
RlcUlRbCb     **rbCb;   
#endif
{
   TRC3(kwDbmFetchUlRbCbByRbId)

   *rbCb= NULLP;

   /* Check for UE CB or CELL CB */
   if (rlcId->ueId == 0)
   {
      RlcUlCellCb *cellCb;

      if(rlcId->rbId >= KW_MAX_RB_PER_CELL)
      {
         RLOG_ARG3(L_ERROR,DBG_RBID,rlcId->rbId ,
               "Invalid RbId, cellId:%d UEID:%d Max is [%d]",
               rlcId->cellId, 
               rlcId->ueId,
               KW_MAX_RB_PER_CELL);
         RETVOID;
      }

      kwDbmFetchUlCellCb(gCb,rlcId->cellId, &cellCb);
      if(!cellCb)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,rlcId->cellId,
               "CellCb not found RBID:%d UEID:%d",
               rlcId->rbId,
               rlcId->ueId);
         RETVOID;
      }

      *rbCb = cellCb->rbCb[rlcId->rbId];
   }
   else
   {
      RlcUlUeCb *ueCb;

      if (!(KW_VALIDATE_UE_RBID(rlcId->rbType, rlcId->rbId)))
      {
         RLOG_ARG3(L_ERROR,DBG_RBID, rlcId->rbId,
               "Invalid RbId for RbType[%d] CELLID:%d UEID:%d", 
               rlcId->rbType,
               rlcId->cellId,
               rlcId->ueId);
         RETVOID;
      }

      if (kwDbmFetchUlUeCb(gCb,rlcId->ueId, rlcId->cellId, &ueCb) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID, rlcId->cellId,
               "UeId [%d]: UeCb not found RBID:%d",
               rlcId->ueId,
               rlcId->rbId);
         RETVOID;
      }

      KW_DBM_GET_RBCB_FROM_UECB(rlcId->rbId, rlcId->rbType, ueCb, *rbCb);
   }
   RETVOID;
} /* kwDbmFetchUlRbCbByRbId */


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
#ifdef ANSI
PUBLIC Void kwDbmFetchUlRbCbFromLchId
(
KwCb          *gCb,
CmLteRnti     ueId,      
CmLteCellId   cellId,   
CmLteLcId     lcId,    
RlcUlRbCb      **rbCb  
)
#else
PUBLIC Void kwDbmFetchUlRbCbFromLchId(gCb, ueId, cellId, lcId, rbCb)
KwCb          *gCb;
CmLteRnti     ueId;     
CmLteCellId   cellId;  
CmLteLcId     lcId;   
RlcUlRbCb      **rbCb;
#endif
{
   RlcUlUeCb *ueCb;

   TRC3(kwDbmFetchUlRbCbFromLchId)


   *rbCb = NULLP;
   
   /* Check for UE CB or CELL CB */
   if (ueId == 0)
   {
      RlcUlCellCb *cellCb;
      
      kwDbmFetchUlCellCb(gCb,cellId, &cellCb);
      if(!cellCb)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellId,"CellCb not found UEID:%d",ueId);
         RETVOID;
      }

      *rbCb = cellCb->lCh[lcId - 1].ulRbCb;
      RETVOID;
   }
   
   if (kwDbmFetchUlUeCb(gCb,ueId, cellId, &ueCb) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID, cellId,"UeId [%d]: UeCb not found",ueId);
      RETVOID;
   }

   *rbCb = ueCb->lCh[lcId - 1].ulRbCb;

   RETVOID;
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
#ifdef ANSI
PUBLIC Void kwDbmDelAllUlRb
(
KwCb       *gCb,
RlcUlRbCb   **rbCbLst,          
U8         numRbCb            
)
#else
PUBLIC Void kwDbmDelAllUlRb(gCb, rbCbLst, numRbCb)
KwCb       *gCb;
RlcUlRbCb   **rbCbLst;        
U8         numRbCb;         
#endif
{
   U32 idx; /* Index */

   TRC3(kwDbmDelAllUlRb)


   for (idx = 0; idx < numRbCb; idx++)
   {
      if (rbCbLst[idx] != NULLP)
      {
         /* Free the Buffers of RbCb */
         if( CM_LTE_MODE_UM == rbCbLst[idx]->mode ) 
         {
            kwUmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
         else if(CM_LTE_MODE_AM == rbCbLst[idx]->mode) 
         {
            kwAmmFreeUlRbCb(gCb,rbCbLst[idx]);
         }
     
         RLC_FREE (gCb,rbCbLst[idx], sizeof (RlcUlRbCb));
      }
   }

   RETVOID;
} /* kwDbmDelAllRb */


/**
 * @brief  Handler to create an UeCb
 *
 * @details  
 *    This function is invoked by CFG to create UeCb and insert into the 
 *    Ue hashlist of KwCb.
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
#ifdef ANSI
PUBLIC S16 kwDbmAddUlUeCb
(
KwCb          *gCb,
CmLteRnti     ueId,       
CmLteCellId   cellId,    
RlcUlUeCb      *ueCb    
)
#else
PUBLIC S16 kwDbmAddUlUeCb(gCb, ueId, cellId, ueCb)
KwCb          *gCb;
CmLteRnti     ueId;    
CmLteCellId   cellId; 
RlcUlUeCb      *ueCb;
#endif
{
   TRC3(kwDbmAddUlUeCb)


   ueCb->ueId = ueId;
   ueCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->ueLstCp), 
                              (PTR)ueCb, 
                              (U8 *)&(ueCb->ueId),
                              (U16) sizeof(CmLteRnti)))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellId,
            "UeId[%u] HashList Insertion Failed",
            ueId);
      RETVALUE(RFAILED);
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe++;


   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 kwDbmAddUlTransaction 
(
KwCb        *gCb,
RlcUlCfgTmpData   *cfg
)
#else
PUBLIC S16 kwDbmAddUlTransaction(gCb, cfg)
KwCb        *gCb;
RlcUlCfgTmpData   *cfg;
#endif
{
   TRC3(kwDbmAddUlTransaction)

#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "(transId(%ld)", cfg->transId);
#else
   RLOG1(L_DEBUG, "(transId(%d))", cfg->transId);
#endif

   RETVALUE(cmHashListInsert(&(gCb->u.ulCb->transIdLstCp), 
                             (PTR)cfg, 
                             (U8 *)&(cfg->transId), 
                             (U16) sizeof(cfg->transId)));
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
#ifdef ANSI
PUBLIC S16 kwDbmFindUlTransaction
(
KwCb             *gCb,
U32              transId,
RlcUlCfgTmpData   **cfg
)
#else
PUBLIC S16 kwDbmFindUlTransaction(gCb, cfg)
KwCb             *gCb;
U32              transId;
RlcUlCfgTmpData   **cfg;
#endif
{
   TRC3(kwDbmFindUlTransaction)

   if(ROK != cmHashListFind(&(gCb->u.ulCb->transIdLstCp),
                            (U8 *) &transId, 
                            sizeof (transId), 
                            KW_DEF_SEQ_NUM,(PTR *) cfg))
   {
      RLOG1(L_ERROR,"TransId [%ld] not found",transId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 kwDbmDelUlTransaction
(
KwCb             *gCb,
RlcUlCfgTmpData   *cfg       
)
#else
PUBLIC S16 kwDbmDelUlTransaction(gCb, cfg)
KwCb             *gCb;
RlcUlCfgTmpData   *cfg;     
#endif
{
   TRC3(kwDbmDelUlTransaction)


   if(cmHashListDelete(&(gCb->u.ulCb->transIdLstCp),(PTR) (cfg)) != ROK) 
   {
      RLOG0(L_ERROR,"HashList Deletion failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
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
#ifdef ANSI
PUBLIC S16 kwDbmDelAllUlTransactions
(
KwCb *gCb
)
#else
PUBLIC S16 kwDbmDelAllUlTransactions(gCb)
KwCb *gCb;
#endif
{
   RlcUlCfgTmpData *cfg = NULL;

   TRC3(kwDbmDelAllUlTransctions)

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->transIdLstCp), 
                                   (PTR) cfg, 
                                   (PTR *)&cfg))
   {
      if(kwDbmDelUlTransaction(gCb, cfg) != ROK)
      {
         RETVALUE(RFAILED);
      }
      
      cfg = NULLP;
   }

   RETVALUE(ROK);
}


/**
 * @brief Handler to Fetch an UeCb
 *
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of KwCb.
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
#ifdef ANSI
PUBLIC S16 kwDbmFetchUlUeCb
(
KwCb          *gCb,
CmLteRnti     ueId,  
CmLteCellId   cellId, 
RlcUlUeCb      **ueCb   
)
#else
PUBLIC S16 kwDbmFetchUlUeCb(gCb,ueId, cellId, ueCb)
KwCb          *gCb;
CmLteRnti     ueId;     
CmLteCellId   cellId;    
RlcUlUeCb      **ueCb;     
#endif
{
   TRC3(kwDbmFetchUlUeCb)


   RETVALUE(cmHashListFind(&(gCb->u.ulCb->ueLstCp), 
                           (U8 *)&(ueId), sizeof(CmLteRnti),
                           KW_DEF_SEQ_NUM, 
                           (PTR *) ueCb));
}


/**
 * @brief Handler to delete an UeCb
 *
 * @details
 *    This function is invoked by CFG to delete UeCb from the Ue hashlist 
 *    of KwCb.
 *
 *
 * @param[in] gCb      RLC Instance Control Block
 * @param[in] ueCb     UE Control Block
 * @param[in] abrtFlag Abort Flag
 *
 * @return Void
 *
*/
#ifdef ANSI
PUBLIC Void kwDbmDelUlUeCb
(
KwCb       *gCb,
RlcUlUeCb   *ueCb,       
Bool       abortFlag   
)
#else
PUBLIC Void kwDbmDelUlUeCb(gCb,eCb, abortFlag)
KwCb       *gCb;
RlcUlUeCb   *ueCb;      
Bool       abortFlag; 
#endif
{
   TRC3(kwDbmDelUlUeCb)


#if  (!defined(KW_PDCP) || !(defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))
   UNUSED(abortFlag);
#endif /* (!defined(KW_PDCP) || ! (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)))*/

   /* Delete all logical channels */
   KW_MEM_ZERO(ueCb->lCh,sizeof(RlcUlLch) * KW_MAX_LCH_PER_UE);

   /* Delete all SRB RbCbs in UeCb */
   kwDbmDelAllUlRb(gCb,ueCb->srbCb, KW_MAX_SRB_PER_UE);

   /* Delete all DRB RbCbs in UeCb */
   kwDbmDelAllUlRb(gCb,ueCb->drbCb, KW_MAX_DRB_PER_UE);

   /* Delete ueCb entry from ueLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb))
   {
      RLOG_ARG1(L_ERROR,DBG_UEID,ueCb->ueId,
            "HashList Deletion Failed cellId(%d)",
            ueCb->cellId);
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.numUe--;
   /* Deallocate ueCb */
   RLC_FREE(gCb,ueCb, sizeof(RlcUlUeCb));

   RETVOID;
}

/**
 * @brief Handler to delete all UeCbs
 *
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue 
 *    hashlist of KwCb.
 *
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwDbmDelAllUlUe
(
KwCb *gCb
)
#else
PUBLIC Void kwDbmDelAllUlUe(gCb)
KwCb *gCb;
#endif
{
   RlcUlUeCb *ueCb = NULLP;  /* UE Control Block */

   TRC3(kwDbmDelAllUlUe)


   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->ueLstCp), 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      kwDbmDelUlUeCb(gCb,ueCb, TRUE);

      ueCb = NULLP;
   }

   RETVOID;
}


/**
 * @brief Handler to create CellCb
 *
 * @details
 *    This function is invoked by CFG to create CellCb and insert into
 *    the Cell hashlist of KwCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellId   Cell Identifier 
 *  @param[in] cellCb   Cell Control Block 
 *
 *  @return  S16
 *     -# ROK 
 *     -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 kwDbmAddUlCellCb
(
KwCb          *gCb,
CmLteCellId   cellId,    
RlcUlCellCb    *cellCb  
)
#else
PUBLIC S16 kwDbmAddUlCellCb(gCb, cellId, cellCb)
KwCb          *gCb;
CmLteCellId   cellId;    
RlcUlCellCb    *cellCb;  
#endif
{
   RlcUlCellCb *tCellCb; 

   TRC3(kwDbmAddUlCellCb)



   tCellCb = cellCb;
   tCellCb->cellId = cellId;

   if(ROK != cmHashListInsert(&(gCb->u.ulCb->cellLstCp), 
                              (PTR) tCellCb,
                              (U8 *)&(tCellCb->cellId), 
                              (U16) sizeof(CmLteCellId)))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,tCellCb->cellId,
            "HashList Insertion Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} 


/**
 * @brief Handler to Fetch an CellCb
 *       
 * @details
 *    This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *    of KwCb.
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
#ifdef ANSI
PUBLIC Void kwDbmFetchUlCellCb
(
KwCb          *gCb,
CmLteCellId   cellId,     
RlcUlCellCb    **cellCb   
)
#else
PUBLIC Void kwDbmFetchUlCellCb(gCb, cellId, cellCb)
KwCb          *gCb;
CmLteCellId   cellId;    
RlcUlCellCb    **cellCb;  
#endif
{
   TRC3(kwDbmFetchUlCellCb)


   *cellCb = NULLP;
   if(ROK != cmHashListFind(&(gCb->u.ulCb->cellLstCp), 
                            (U8 *)&(cellId),sizeof(CmLteCellId), 
                            KW_DEF_SEQ_NUM, (PTR*) cellCb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID, cellId,"CellCb not found");
   }

   RETVOID;
}


/**
 * @brief Handler to delete CellCb
 *
 * @details
 *    This function is invoked by CFG to delete CellCb from the Cell hashlist 
 *    of KwCb.
 *
 *  @param[in] gCb      RLC Instance Control Block
 *  @param[in] cellCb   Cell Control Block
 *
 *  @return  Void
*/
#ifdef ANSI
PUBLIC Void kwDbmDelUlCellCb
(
KwCb         *gCb,
RlcUlCellCb   *cellCb     
)
#else
PUBLIC Void kwDbmDelUlCellCb(gCb, cellCb)
KwCb         *gCb;
RlcUlCellCb   *cellCb;     
#endif
{
   TRC3(kwDbmDelUlCellCb)


   /* Delete all rbCbs in cellCb */
   kwDbmDelAllUlRb(gCb,cellCb->rbCb, KW_MAX_RB_PER_CELL);

   /* Delete cellCb entry in hash list cellLstCp */
   if(ROK != cmHashListDelete(&(gCb->u.ulCb->cellLstCp), (PTR) cellCb))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
            "HashList Deletion Failed");
   }
   /* Deallocate cellCb */
   RLC_FREE(gCb, cellCb, sizeof(RlcUlCellCb));

   RETVOID;
} /* kwDbmDelCellCb */


/**
 * @brief Handler to delete all UeCbs
 *       
 * @details
 *    This function is invoked by CFG to delete all UeCbs from the Ue
 *    hashlist of KwCb.
 * @param[in] gCb      RLC Instance Control Block
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwDbmDelAllUlCell
(
KwCb *gCb
)
#else
PUBLIC Void kwDbmDelAllUlCell(gCb)
KwCb *gCb;
#endif
{
   RlcUlCellCb *cellCb = NULLP; /* Cell Control Block */

   TRC3(kwDbmDelAllUlCell)


   /* Until no more cellCb is ueLstCp hash list get and delete cellCb */
   while (ROK == cmHashListGetNext(&(gCb->u.ulCb->cellLstCp), 
                                   (PTR) cellCb, 
                                   (PTR *)&cellCb))
   {
      kwDbmDelUlCellCb(gCb,cellCb);

      cellCb = NULLP;
   }

   RETVOID;
} 


/**
 * @brief Handler to delete all UeCbs and CellCbs
 *       
 * @details
 *    This function is invoked by LMM to delete all UeCbs from the Ue
 *    hashlist of KwCb and cellCbs from the Cell hashlist of kwCb.
 * 
 * @param[in] gCb      RLC Instance Control Block
 *
*/
#ifdef ANSI
PUBLIC Void kwDbmUlShutdown
(
KwCb *gCb
)
#else
PUBLIC Void kwDbmUlShutdown(gCb)
KwCb *gCb;
#endif
{
   TRC3(kwDbmUlShutdown)

   kwDbmDelAllUlCell(gCb);

   kwDbmDelAllUlUe(gCb);

   kwDbmUlDeInit(gCb);

   RETVOID;
}


/********************************************************************30**
  
         End of file
**********************************************************************/
